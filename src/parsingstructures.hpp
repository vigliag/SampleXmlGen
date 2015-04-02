#ifndef PARSINGSTRUCTURES_HH_INCLUDED
#define PARSINGSTRUCTURES_HH_INCLUDED

#include "structures.hpp"
#include <iostream>
#include <string>
#include <iterator>
#include <regex>

using namespace std;

namespace parsing {

struct ParsingResults{
    vector<PoolDefinition>* poolDefinitions;
    ElementDefinition* rootDefinition;
};    

ParsingResults parse(const string& s);
}
#endif // PARSINGSTRUCTURES_HH_INCLUDED
