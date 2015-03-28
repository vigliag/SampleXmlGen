#include "parsingstructures.hh"
#include "structures.hh"
#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

namespace parsing {

    typedef pair<string, unsigned> id_amount;
    typedef pair<string, bool> id_exclusive;

    struct ElementDefinitionSTM {
        string kind;
        unsigned quantifier;
        string name;
        vector<id_amount> children;
        vector<id_exclusive> resourceRequests;
    };

    vector<id_amount> parseChildren(const string& in) {
        regex childrenRegex(R"((\w+)(\[(\d+)\])?)");
        sregex_iterator it(in.begin(), in.end(), childrenRegex);
        sregex_iterator reg_end;
        vector<id_amount> result;
        cout << "children list" << endl;
        for (; it != reg_end; ++it) {
            for (unsigned i = 0; i < it->size(); i++) {
                cout << i << "  " << it->str(i) << endl;
            }
            unsigned amount = (unsigned) strtol(it->str(3).c_str(), 0, 10);
            if (amount == 0) amount = 1; //assume 1 if quantifier is missing
            result.push_back(id_amount(it->str(1), amount));
        }

        return result;
    }

    vector<id_exclusive> parseResourcesList(const string& s) {
        regex resourceListRegex(R"(has\s+(shared|distinct)\s+(\w+))");
        sregex_iterator it(s.begin(), s.end(), resourceListRegex);
        sregex_iterator reg_end;

        vector<id_exclusive> result;
        cout << "resource list:" << endl;
        for (; it != reg_end; ++it) {
            for (unsigned i = 0; i < it->size(); i++) {
                cout << i << "  " << it->str(i) << endl;
            }
            bool exclusive = (it->str(1) == "exclusive");
            string id = it->str(2);
            result.push_back(id_exclusive(id, exclusive));
        }
        return result;
    }

    vector<ElementDefinitionSTM> parseElementDefinitionList(const string& s) {
        regex mainregex(R"(<!ELEMENT\s+((\w*)\[(\d*)\])?\s*(\w+)\((.*)\)\s*((has\s+(shared|distinct)\s+\w+,?\s*)*)\s*>)");
        sregex_iterator it(s.begin(), s.end(), mainregex);
        sregex_iterator reg_end;

        vector<ElementDefinitionSTM> result;
        for (; it != reg_end; ++it) {
            cout << "Element found: ";
            cout << it->str() << endl;
            
            //print debug info
            for (unsigned i = 0; i < it->size(); i++) {
                cout << i << "  " << it->str(i) << endl;
            }
            
            ElementDefinitionSTM edstm;
            
            edstm.kind = it->str(2);
            
            int quantifier = strtol(it->str(3).c_str(), 0, 10);
            if(quantifier == 0) quantifier =1 ;
            edstm.quantifier = quantifier;
            
            edstm.name = it->str(4);
            
            string childrenList = it->str(5);
            edstm.children = parseChildren(childrenList);
            
            string resourcesList = it->str(6);
            edstm.resourceRequests = parseResourcesList(resourcesList);

            for(auto a : edstm.resourceRequests){
                cout << a.first << a.second << endl;
            }
            
            result.push_back(edstm);
            cout << endl;
        }

        return result;
    }

    ChildDefinition resolveChild(map<string, ElementDefinitionSTM*> elemap, id_amount child){
        ElementDefinitionSTM* ed = NULL;
        if(elemap.count(child.first))
            ed = elemap.at(child.first);
        
        for(auto e: elemap){
            cout << e.first << " = " << e.second << endl;
            cout << e.second->resourceRequests.size() <<endl;
        }
        
        ChildDefinition result;
        result.amount = child.second;
        result.resourceId = NULL;
        result.node = NULL;
        //if it doesn't exist in elemap, then it is a leaf
        if (ed==NULL){
            result.node = new ElementDefinition();
            result.node->isLeaf = true;
            result.node->name = child.first;
        } else {
            //the given specifier is the one of a pool
            if(ed->kind == "pool"){
                result.resourceId = new string(child.first);
                result.node = NULL;
            } else { //it is an element for which we have a definition, go recursively
                result.node = new ElementDefinition();
                result.node->isLeaf = false;
                result.node->name = child.first;
                cout << ed->resourceRequests.size() <<endl;
                for(id_exclusive rr : ed->resourceRequests){
                    ResourceRequest nrr;
                    nrr.resourceId = rr.first;
                    nrr.thePoolForThisResourceShouldBeExclusive = rr.second;
                    nrr.amount = 2; //TODO FIXME
                    result.node->requiredResources.push_back(nrr);
                }
                
                for(id_amount child: ed->children){
                    result.node->children.push_back(resolveChild(elemap,child));
                }
            }
        }
        
        return result;
    }
    
    /**
     * Method taking a string to parse and returning
     * the abstract structure of the new document resolving references
     * @param s the string to parse
     */
    ParsingResults parse(const string & s) {
        const vector<ElementDefinitionSTM> elementStatements =
                parseElementDefinitionList(s);
        
        
        
        //first stage, create a map of defined elements
        ElementDefinitionSTM rootSTM;
        map<string, ElementDefinitionSTM*> elemap;
        
        for(ElementDefinitionSTM edstm : elementStatements){
            if(edstm.kind == "root"){
                rootSTM = edstm;
            } else {
                elemap[edstm.name] = new ElementDefinitionSTM(edstm);
            }
        }
        
        cout << "Elemap:" << endl;
        for(auto e: elemap){
            cout << e.first << " = " << e.second << endl;
            cout << e.second->resourceRequests.size() <<endl;
        }
        
        //second stage: turn children into reference to elements
        //process each pool first
        //then go to the root
        
        ParsingResults result;
        result.poolDefinitions =  new vector<PoolDefinition>();
        for(ElementDefinitionSTM edstm : elementStatements){
            if(edstm.kind == "pool"){
                PoolDefinition pd;
                pd.resourceId = edstm.name;
                pd.amount = edstm.quantifier;
                pd.ed.name = edstm.name;
                pd.ed.isLeaf = false;
                pd.exclusive = false; //TODO, make specifiable in the language?
                for(id_amount child: edstm.children){
                    pd.ed.children.push_back(resolveChild(elemap,child));
                }
                result.poolDefinitions->push_back(pd);
            }
        }
        
        result.rootDefinition = new ElementDefinition();
        result.rootDefinition->name =  rootSTM.name;
        result.rootDefinition->isLeaf = false;
        
        for(id_amount child: rootSTM.children){
            cout << "children node" << child.first << endl;
            result.rootDefinition->children.push_back(resolveChild(elemap,child));
        }
        return result;
    }
  
}

