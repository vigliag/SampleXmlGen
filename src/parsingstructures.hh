#include <iostream>
#include <string>
#include <iterator>
#include <regex>

using namespace std;

struct ChildDefinitionExp {
    std::string name;
    unsigned amount; //TODO, can be easily extended with an xpath query (currently not supported)
};

struct PoolDefinition {
    bool distinct;
    std::string identifier;
};

struct ElementDefinitionSTM {
    std::string kind;
    unsigned parameter;
    std::string name;
    std::vector<ChildDefinitionExp> children;
    std::vector<PoolDefinition> *dataPools;
};


void parse(string& s){
    std::regex mainregex(R"(<!ELEMENT\s+(\w*\[\d+\])?\s*(\w+)\((.*)\)\s*((has\s+(shared|distinct)\s+\w+,?\s*)*)\s*>)");
    std::sregex_iterator it(s.begin(),s.end(), mainregex);
    std::sregex_iterator reg_end;

    for (; it != reg_end; ++it) {
         std::cout << "Substring found: ";
         std::cout << it->str() << std::endl;
         for(int i = 0; i < it->size(); i++ ){
            std::cout << i << "  " << it->str(i) << std::endl;
         }
         std::cout << std::endl;
    }
        
}