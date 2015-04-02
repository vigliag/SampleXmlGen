/**
 * Specifies the structures of the abstract tree defining elements hierarchies
 */

#ifndef STRUCTURES_HH_INCLUDED
#define STRUCTURES_HH_INCLUDED

#include <string>
#include <map>
#include <vector>
#include "vendor/pugixml.hpp"

using pugi::xml_node;
using pugi::xml_document;
using std::string;
using std::map;
using std::vector;

struct ElementDefinition;

/***
 * used in generation phase to define the resources that are needed 
 * to generate the ResourceMap of a new element
 */
struct ResourceRequest {
    string resourceId; //id to lookup the resource in a ResourceMap
    unsigned amount;
    bool thePoolForThisResourceShouldBeExclusive;
};

/**
 * Specifies the quantity of children to generate as amount
 * and either an ElementDefinition to use as blueprint to recursively generate 
 * children or a resourceId to query a resource
 */
struct ChildDefinition {
    ElementDefinition* node;
    unsigned int amount;
    /**
     * identifier of a resource. If not null, take verbatim from resource, else generate
     */
    string* resourceId; 
};

/**
 * Definition of a node (or element) in the abstract tree describing the structure
 * An element is made of
 * a name
 * a list of of requiredResources (id and amount)
 * a list of children
 */
struct ElementDefinition {
    string name;
    bool isLeaf;
    vector<ResourceRequest> requiredResources; //define how to create own dataPool, used by him and his descendants
    vector<ChildDefinition> children;
    //vector<DataPoolQuery> childrenFromDataPool; //define how children are created querying the datapool
};

/**
 * Definition of a Pool of generated xml elements
 * Acts as root of the tree
 */
struct PoolDefinition {
    string resourceId; //resourceId where to put the new generated pool
    unsigned int amount;
    ElementDefinition ed;
    bool exclusive;
};

#endif
