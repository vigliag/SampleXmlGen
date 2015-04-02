#include "Resource.hpp"
#include <set>
#include <cstdlib>
#include <utility>
#include <ctime>

std::set<int>* valuesToTake(int amount, int total) {
    if(amount > total){
        amount = total;
    }
    std::set<int>* result = new std::set<int>();
    std::pair<std::set<int>::iterator,bool> r;
    while(amount--){
        do {
            r = result->insert(rand()%total);
        } while(r.second == false);
    }
    return result;
}

void takeNodes(xml_node* src, xml_node* dest, int amount, bool move) {
    int taken = 0;
    int nodesSeen = 0;
    auto it = src->begin();
    auto end = src->end();

    int childrenCount = std::distance(it,end); //note, this can be optimized putting the count in the resource
    auto indexToTake = valuesToTake(amount, childrenCount);
    
    while ( (it != end) && taken < amount) {
        xml_node tmp = *it;
        bool shouldTake = indexToTake->count(nodesSeen);
        
        if(shouldTake){
            dest->append_copy(tmp);
            taken++;
        }
        
        it++;
        nodesSeen++;
        
        if (shouldTake && move) {
            src->remove_child(tmp);
        }
    }
}

//Todo, move implementation here from Resources.hh