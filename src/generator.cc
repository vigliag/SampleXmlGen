#include "generator.hh"
#include <sstream>
#include <set>
/**
 * Generates a certain amount of xml elements inside a given parent node.
 * Each element is generated as specified in the ElementDefinition.
 * 
 * A ResourceMap is used to generate children that are marked (in the ChildDefinition)
 * as taken verbatim from a Resource.
 * The ResourceMap is modified (a new version is created) according to indications
 * in ElementDefinition and passed down to the children (see method getSubset of Resource).
 * 
 * The function calls itself recursively for each of the children nodes it generates
 * (so for each of the children of each of genenrated nodes)
 * 
 * @param en The element to use as blueprint
 * @param parentNode The xml_node where the generated nodes will be appended
 * @param parentResources The map indicating all resources avaiable to generate children
 * @param prefix A string that will be used to univocally generate content for the leaf nodes
 */
void generate(ElementDefinition& en,
        xml_node* parentNode,
        unsigned amount,
        ResourceMap& parentResources,
        const string prefix) {

    //Generate own resourceMap
    ResourceMap ownDataPool;
    
    std::set<string> processedResources; 
    //Apply modifications to the map as specified in the resourceRequests
    for (ResourceRequest rr : en.requiredResources) {
        ownDataPool[rr.resourceId] = parentResources.at(rr.resourceId).getSubset(rr.amount, true);
        ownDataPool[rr.resourceId].exclusive = rr.thePoolForThisResourceShouldBeExclusive;
        processedResources.insert(rr.resourceId); //"mark" as processed
    }
    //Simply take all elements for all other pools
    for(auto& m : parentResources){
        bool alreadyProcessed = (processedResources.count(m.first) == 1);
        if (!alreadyProcessed)
            ownDataPool[m.first] = m.second.getSubset(numeric_limits<int>::max(), false);
    }
    
    cout << "Generating " << amount << " elements of " << en.name << endl;
    
    //Loop Generating each element
    for (unsigned i = 0; i < amount; i++) {

        //obtain new prefix (will be used to name leaves)
        stringstream ss;
        ss << prefix << en.name << (i + 1);
        string childrenPrefix = ss.str();

        //create the new node
        xml_node currentNode = parentNode->append_child(en.name.c_str());

        //add content as pcdata if we are creating a leaf node
        if (en.isLeaf) {
            currentNode.append_child(pugi::node_pcdata).set_value(childrenPrefix.c_str());
            continue;
        }

        //if it is not a leaf, for each children
        for (ChildDefinition child : en.children) {
            
            
            //if children is to be taken from a resource, copy verbatim from there
            if (child.resourceId != NULL) {
                string resId = *(child.resourceId);
                Resource res = ownDataPool[resId];
                pugi::xml_document* data = res.data;
                
                cout << "Copying "<< child.amount << " children from resource " << res.name << endl;
                res.data->save(std::cout); //TODO REMOVEME
                //TODO: randomize here
                
                takeNodes(data, &currentNode, child.amount, res.exclusive);
            
            //if a resourceId isn't specified, generate recursively
            } else { //node must be set
                //WARNING passing address of currentNode, which is in stack
                //cout << "generating Element " << child.node->name << end;
                generate(*(child.node), &currentNode, child.amount, ownDataPool, childrenPrefix);
            }
        }
    }
}

xml_document* generateRoot(ElementDefinition& root,
        unsigned int amount,
        ResourceMap& previouslyGeneratedResources) {
    cout << "Generating root node " << root.name << endl;
    xml_document* doc = new xml_document();
    //xml_node root_node = doc->append_child(root.name.c_str());
    
    generate(root, doc, amount, previouslyGeneratedResources, "");
    return doc;
}

