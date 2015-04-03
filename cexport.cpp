#include "src/structures.hpp"
#include "src/generator.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

extern "C" char* samplexmlgen(char* input) {
    string inputString(input);
    parsing::ParsingResults parsedTree = parsing::parse(inputString);

    if (parsedTree.rootDefinition == NULL) {
        cerr << "Couldn't find any root element, aborting" << endl;
        return new char{'\0'};
    }
    //printDefinitionTree(*parsedTree.rootDefinition);
    
    cerr << "Entering Generation phase" << endl;
    xml_document* result = generateResult(parsedTree.poolDefinitions, parsedTree.rootDefinition);
    
    stringstream ss;
    result->save(ss);
    string res = ss.str();
    char* c_str = new char[res.length() + 1];
    std::strcpy(c_str, res.c_str());
    return c_str;
}
