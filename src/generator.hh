#ifndef GENERATOR_HH_INCLUDED
#define GENERATOR_HH_INCLUDED

#include <string>
#include "vendor/pugixml.hpp"
#include "parsingstructures.hh"
#include "Resource.hh"

using pugi::xml_node;
using pugi::xml_document;
using std::string;

xml_document* generateRoot(ElementDefinition& root, unsigned amount, ResourceMap& availablePoolData);
void generate(ElementDefinition& en,
        xml_node* parentNode,
        unsigned amount,
        ResourceMap& parentResources,
        const string prefix);

#endif // GENERATOR_HH_INCLUDED
