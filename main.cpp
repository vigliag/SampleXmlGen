/* 
 * File:   main.cpp
 * Author: vigliag
 *
 * Created on 27 marzo 2015, 16.58
 */
#include "src/structures.hh"
#include "src/generator.hh"
#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;


//TODO write a printMetaModel function
void printTree(ElementDefinition& ed) {
    cout << "Element: " << ed.name <<  endl;
    if (ed.isLeaf) {
        cout << "leaf" << endl;
    } else {
        cout << "children: {" << endl;
        for (ChildDefinition cd : ed.children) {
            cout << "node address: " << cd.node << ", resourceid: " << cd.resourceId << endl;
            if (cd.node != NULL) printTree(*cd.node);
        }
        cout << "}" << endl;
    }
}

int main(int argc, char** argv) {
    istream *in;
    if (argc > 1) {
        in = new ifstream(argv[1], std::ios::in | std::ios::binary);
    } else {
        in = &std::cin;
    }
    //1) parse input file and obtain an AST
    auto beginCIN = std::istreambuf_iterator<char>(*in);
    auto endCIN = std::istreambuf_iterator<char>();
    std::string inputString(beginCIN, endCIN);
    //delete in?


    //2) use the AST to create an elementDefinition tree
    parsing::ParsingResults parsedTree = parsing::parse(inputString);

    if (parsedTree.rootDefinition == NULL) {
        cout << "Couldn't find any root element, aborting";
        return -1;
    }

    printTree(*parsedTree.rootDefinition);
    //3) Generation phase
    cout << "Entering Generation phase" << endl;

    //generate each pool

    ResourceMap generatedResources;
    for (PoolDefinition pd : *(parsedTree.poolDefinitions)) {
        cout << "Generating pool for: " << pd.resourceId << endl;
        //TODO DEBUG HERE. IS LHS EVALUATED BEFORE RHS?
        Resource newres(pd.resourceId, generateRoot(pd.ed, pd.amount, generatedResources), pd.exclusive);
        generatedResources[pd.resourceId] = newres;
        //generatedResources[pd.resourceId].data->save(std::cout); //TODO REMOVEME
    }

    //generate root element
    cout << "#### Generate actual root node" << endl;
    xml_document* result = generateRoot(*(parsedTree.rootDefinition), 1, generatedResources);

    //output result
    result->save(std::cout);
    return 0;
}
