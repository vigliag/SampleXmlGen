#ifndef RESOURCE_HH_INCLUDED
#define RESOURCE_HH_INCLUDED

#include "structures.hh"

/**
 * A collection of pregenerated xml_nodes used in generation phase
 * 
 * the getSubset function allows generating subset of the nodes
 * to be used by children elments.
 * If the exclusive flag is set, then a given xml_node is exclusively
 * given to only one of the children
 */
class Resource {
public:
    Resource(): name("unnamed"), data(NULL)  {
    }

    
    Resource(const Resource& other) :
    name(other.name), data(other.data), exclusive(other.exclusive) {
    }
    
    Resource(string _name ) : name(_name){
        data = new xml_document();
    };
    
    Resource(string _name, xml_document* _data, bool _exclusive = false)
    : name(_name), data(_data), exclusive(_exclusive) {
    };
    
    Resource getSubset(int amount){
        xml_document* newData = new xml_document();
        for (unsigned i = 0; i < amount; i++) {
            xml_node currNode = data->first_child();
            if (!currNode) throw "Not enough elements in resource " + name;
            newData->append_copy(currNode);
            if (exclusive)
                data->remove_child(currNode);
        }
        return Resource(name, newData);
    };
    
    string name;
    xml_document* data;
    bool exclusive;
};

typedef map<string, Resource> ResourceMap;

#endif // RESOURCE_HH_INCLUDED
