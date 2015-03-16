#include "../src/parsingstructures.hh"

int main(){

    auto beginCIN = std::istreambuf_iterator<char>( std::cin );
    auto endCIN = std::istreambuf_iterator<char>();
	
	std::string inputString(beginCIN,endCIN);

	parse(inputString);
	//std::string::const_iterator iter = inputString.begin();
    //std::string::const_iterator end = inputString.end();

    //std::vector<ElementDefinitionSTM> document;
    //std::cout << "trying parsing " << inputString << std::endl;

    //here parsecode
}