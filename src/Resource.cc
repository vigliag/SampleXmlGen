#include "Resource.hh"

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

//Todo, move implementation here from Resources.hh