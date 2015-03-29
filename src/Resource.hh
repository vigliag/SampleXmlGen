#ifndef RESOURCE_HH_INCLUDED
#define RESOURCE_HH_INCLUDED

#include "structures.hh"

void takeNodes(xml_node* src, xml_node* dest, int amount, bool move) {
    int i = 0;
    xml_node curr = src->first_child();
    while (curr && i < amount) {
        dest->append_copy(curr);
        //xml_node tmp = res_elem;

        xml_node tmp = curr;
        curr = curr.next_sibling();
        i++;

        if (move) {
            src->remove_child(tmp);
        }
    }
}

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

    Resource() : name("unnamed"), data(NULL) {
    }

    Resource(const Resource& other) :
    name(other.name), data(other.data), exclusive(other.exclusive) {
      //  data = new xml_document(); LOGIC MOVED OUTSIDE
      //  data->reset(*other.data);
    }
    
    ~Resource(){
        delete data;
    }

    Resource(string _name) : name(_name) {
        data = new xml_document();
    };

    Resource(string _name, xml_document* _data, bool _exclusive = false)
    : name(_name), data(_data), exclusive(_exclusive) {
    };

    
    
    Resource getSubset(int amount, bool applyExclusive) {
        xml_document* newData = new xml_document();
        takeNodes(data, newData, amount, exclusive && applyExclusive );
        return Resource(name, newData);
    };

    string name;
    xml_document* data;
    bool exclusive;
};

typedef map<string, Resource> ResourceMap;

#endif // RESOURCE_HH_INCLUDED
