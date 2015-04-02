/* 
 * File:   main.cpp
 * Author: vigliag
 *
 * Created on 27 marzo 2015, 16.58
 */
#include "src/structures.hpp"
#include "src/generator.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;


/**
 * Debug function that prints the definition tree
 * @param ed
 */
void printDefinitionTree(ElementDefinition& ed) {
    cerr << "Element: " << ed.name <<  endl;
    if (ed.isLeaf) {
        cerr << "leaf" << endl;
    } else {
        cerr << "children: {" << endl;
        for (ChildDefinition cd : ed.children) {
            cerr << "node address: " << cd.node << ", resourceid: " << cd.resourceId << endl;
            if (cd.node != NULL) printDefinitionTree(*cd.node);
        }
        cerr << "}" << endl;
    }
}

xml_document* generateResult(vector<PoolDefinition>* poolDefinitions, ElementDefinition* rootDefinition){
    ResourceMap generatedResources;
    for (PoolDefinition pd : *poolDefinitions) {
        cerr << "Generating pool for: " << pd.resourceId << endl;
        Resource newres(pd.resourceId, generateRoot(pd.ed, pd.amount, generatedResources), pd.exclusive);
        generatedResources[pd.resourceId] = newres;
        newres.data->save(std::cerr);
    }

    cerr << "#### Generate actual root node" << endl;
    return generateRoot(*rootDefinition, 1, generatedResources);
}

int main(int argc, char** argv) {
    istream *in;
    if (argc > 1) {
        in = new ifstream(argv[1], std::ios::in | std::ios::binary);
    } else {
        in = &cin;
    }
    
    auto beginCIN = istreambuf_iterator<char>(*in);
    auto endCIN = istreambuf_iterator<char>();
    string inputString(beginCIN, endCIN);
    //delete in?
    
    //2) use the AST to create an elementDefinition tree
    parsing::ParsingResults parsedTree = parsing::parse(inputString);

    if (parsedTree.rootDefinition == NULL) {
        cerr << "Couldn't find any root element, aborting" << endl;
        return -1;
    }
    printDefinitionTree(*parsedTree.rootDefinition);
    
    cerr << "Entering Generation phase" << endl;
    xml_document* result = generateResult(parsedTree.poolDefinitions, parsedTree.rootDefinition);
    result->save(cout);
    return 0;
}
