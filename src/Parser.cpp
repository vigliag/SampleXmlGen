#include "parsingstructures.hpp"
#include "structures.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

namespace parsing {

    typedef pair<string, unsigned> id_amount;

    struct id_amount_exclusive {string id; unsigned amount; bool exclusive;};
    
    inline void printMatches(sregex_iterator& it) {
        cerr << "Matches for " << it->str() << endl;
        for (unsigned i = 0; i < it->size(); i++) {
            cerr << i << "  " << it->str(i) << endl;
        }
    }

    inline int parseQuantifier(string s) {
        int quantifier = strtol(s.c_str(), 0, 10);
        return quantifier != 0 ? quantifier : 1;
    }

    /***
     * Contains all the information associated to a single Element statement
     */
    struct ElementDefinitionSTM {
        string kind;
        unsigned quantifier;
        string name;
        vector<id_amount> children;
        vector<id_amount_exclusive> resourceRequests;
        
        friend std::ostream& operator<<(std::ostream& os, const ElementDefinitionSTM& obj) {
            os << "Name: " << obj.name << endl;
            os << "Kind: " << obj.kind << endl;
            os << "Quantifier: " << obj.quantifier << endl;
            
            os << "Children: [" ;
            for (auto a : obj.children) {
                os << a.first <<"->" << a.second << " ";
            }
            os << "]" << endl;
            os << "ResourceRequests: [" ;
            for (auto a : obj.resourceRequests) {
                os << a.id <<"-" << a.exclusive << "-" << a.amount << " ";
            }
            os << "]" << endl;
            return os;
        }

    };

    /**
     * Matches string like this TagName[24]
     * 
     * @param in the string to parse
     * @return a vector of pair<string, unsigned>
     */
    vector<id_amount> parseChildren(const string& in) {
        regex childrenRegex(R"((\w+)\s?(\[(\d+)\])?)");
        sregex_iterator it(in.begin(), in.end(), childrenRegex);
        sregex_iterator reg_end;
        vector<id_amount> result;

        for (; it != reg_end; ++it) {
            printMatches(it);
            unsigned amount = parseQuantifier(it->str(3));
            result.push_back(id_amount(it->str(1), amount));
        }

        return result;
    }

    /**
     * Matches string like "has shared children"
     * @param s the string to parse
     * @return a vector of pairs <string, bool>
     *  where the boolean is one if distinct is specified
     */
    vector<id_amount_exclusive> parseResourcesList(const string& s) {
        regex resourceListRegex(R"((shared|distinct)\s+(\w+)\[(\d+)\])");
        sregex_iterator it(s.begin(), s.end(), resourceListRegex);
        sregex_iterator reg_end;

        vector<id_amount_exclusive> result;
        
        for (; it != reg_end; ++it) {
            bool exclusive = (it->str(1) == "distinct");
            string id = it->str(2);
            unsigned quantifier = parseQuantifier(it->str(3));
            result.push_back(id_amount_exclusive{id, quantifier, exclusive});
        }
        return result;
    }

    /**
     * Matches a whole ElementStatement
     * @param s the string to parse
     * @return a vector of ElementDefinitionSTM
     */
    vector<ElementDefinitionSTM> parseElementDefinitionList(const string& s) {
        regex mainregex(R"(<\S*\s+((\w*)\[(\d*)\])?\s*(\w+)\s*\((.*)\)\s*(has\s+(.*))?\s*>)");
        sregex_iterator it(s.begin(), s.end(), mainregex);
        sregex_iterator reg_end;

        vector<ElementDefinitionSTM> result;
        for (; it != reg_end; ++it) {
            
            //print debug info
            printMatches(it);

            ElementDefinitionSTM edstm;
            
            edstm.kind = it->str(2);
            if(edstm.kind.empty()) edstm.kind = "element";
            edstm.quantifier = parseQuantifier(it->str(3));
            edstm.name = it->str(4);
            edstm.children = parseChildren(it->str(5));
            edstm.resourceRequests = parseResourcesList(it->str(7));

            //print parse result
            cerr << edstm << endl;

            result.push_back(edstm);
        }

        return result;
    }

    ChildDefinition resolveChild(map<string, ElementDefinitionSTM*> elemap, id_amount child) {
        
        ChildDefinition result;
        result.amount = child.second;
        result.resourceId = NULL;
        result.node = NULL;
        
        //Query elemap to see if an element with the specified id is explicitly defined
        ElementDefinitionSTM* ed = NULL;
        if (elemap.count(child.first))
            ed = elemap.at(child.first);

        //Now create the node
        //if it doesn't exist in elemap, then it is a leaf
        if (ed == NULL) {
            result.node = new ElementDefinition();
            result.node->isLeaf = true;
            result.node->name = child.first;
            
        } else {
            //if the given specifier is the one of a pool
            if (ed->kind == "pool") {
                result.resourceId = new string(child.first);
                result.node = NULL;
                
            } else { //it is an element for which we have a definition, go recursively
                
                //Build a new complete Element Definition
                result.node = new ElementDefinition();
                result.node->isLeaf = false;
                result.node->name = child.first;
                
                for (id_amount_exclusive rr : ed->resourceRequests) {
                    ResourceRequest nrr;
                    nrr.resourceId = rr.id;
                    nrr.thePoolForThisResourceShouldBeExclusive = rr.exclusive;
                    nrr.amount = rr.amount;
                    result.node->requiredResources.push_back(nrr);
                }

                for (id_amount child : ed->children) {
                    result.node->children.push_back(resolveChild(elemap, child));
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

        //first stage, separating the rootStatememt, and putting all the othersi in a map
        ElementDefinitionSTM rootSTM;
        map<string, ElementDefinitionSTM*> elemap;
        for (ElementDefinitionSTM edstm : elementStatements) {
            if (edstm.kind == "root") {
                rootSTM = edstm;
            } else {
                elemap[edstm.name] = new ElementDefinitionSTM(edstm);
            }
        }

        cerr << "Obtained Element Map:" << endl;
        for (auto e : elemap) {
            cerr << e.first << " = " << e.second << endl;
        }
        cerr << endl;

        //second stage: turn children into reference to elements
        //process each pool first
        //then go to the root
        ParsingResults result;
        result.poolDefinitions = new vector<PoolDefinition>();
        
        //Obtain a PoolDefinition (populating result.poolDefinitions) for each Statement defining a pool
        for (ElementDefinitionSTM edstm : elementStatements) {
            if (edstm.kind == "pool") {
                PoolDefinition pd;
                pd.resourceId = edstm.name;
                pd.amount = edstm.quantifier;
                pd.ed.name = edstm.name;
                pd.ed.isLeaf = false;
                pd.exclusive = false; //TODO, make specifiable in the language?
                for (id_amount child : edstm.children) {
                    pd.ed.children.push_back(resolveChild(elemap, child));
                }
                result.poolDefinitions->push_back(pd);
            }
        }

        //Create the rootDefinition from the rootStatement
        result.rootDefinition = new ElementDefinition();
        result.rootDefinition->name = rootSTM.name;
        result.rootDefinition->isLeaf = false;

        for (id_amount child : rootSTM.children) {
            result.rootDefinition->children.push_back(resolveChild(elemap, child));
        }
        return result;
    }

}

