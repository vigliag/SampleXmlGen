#ifndef GENERATOR_HH_INCLUDED
#define GENERATOR_HH_INCLUDED

#include <string>
#include "vendor/pugixml.hpp"
#include "parsingstructures.hpp"
#include "structures.hpp"
#include "Resource.hpp"

using pugi::xml_node;
using pugi::xml_document;
using std::string;

xml_document* generateRoot(ElementDefinition& root, unsigned amount, ResourceMap& availablePoolData);
xml_document* generateResult(vector<PoolDefinition>* poolDefinitions, ElementDefinition* rootDefinition);
void generate(ElementDefinition& en,
        xml_node* parentNode,
        unsigned amount,
        ResourceMap& parentResources,
        const string prefix);

#endif // GENERATOR_HH_INCLUDED
