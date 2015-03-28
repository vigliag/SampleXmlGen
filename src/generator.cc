#include "generator.hh"
#include <sstream>

/**
 * Generates a list of xml elements inside a given parent node
 * uses the information of the given ElementDefinition as blueprint
 * calls itself recursively.
 *
 */
void generate(ElementDefinition& en,
        xml_node* parentNode,
        unsigned amount,
        ResourceMap& parentResources,
        const string prefix) {

    //Generate own resourceMap
    ResourceMap ownDataPool = ResourceMap(parentResources); //COPY
    for (ResourceRequest rr : en.requiredResources) {
        ownDataPool[rr.resourceId] = parentResources[rr.resourceId].getSubset(rr.amount);
        ownDataPool[rr.resourceId].exclusive = rr.thePoolForThisResourceShouldBeExclusive;
    }

    //Generate each element
    cout << "Generating " << amount << " elements of " << en.name << endl;
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
            cout << "generating children " << child.resourceId << endl;
            //if children is to be taken from a resource, copy verbatim from there
            if (child.resourceId != NULL) {
                string resId = *(child.resourceId);
                Resource res = ownDataPool[resId];
                pugi::xml_document* data = res.data;
                
                
                //TODO: randomize here
                int i = 0;
                for (pugi::xml_node res_elem = data->first_child();
                        res_elem;
                        res_elem = res_elem.next_sibling()){
                    if(i>=amount) break;
                    currentNode.append_copy(res_elem);
                    i++;
                }
                    
            
            //if a resourceId isn't specified, generate recursively
            } else { //node must be set
                //WARNING
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

