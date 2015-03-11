
#include "../src/parsingstructures.hh"
#include <boost/spirit/include/qi_parse.hpp>
#include <iostream>

int main(){
	using boost::spirit::qi::phrase_parse;
    using boost::spirit::qi::ascii::space;

    auto beginCIN = std::istreambuf_iterator<char>( std::cin );
    auto endCIN = std::istreambuf_iterator<char>();
	std::vector<ElementDefinitionSTM> document;
	std::string inputString(beginCIN,endCIN);

	std::string::const_iterator iter = inputString.begin();
    std::string::const_iterator end = inputString.end();

    std::cout << "trying parsing " << inputString << std::endl;

	dtd_grammar<std::string::const_iterator> dtdGrammar;
    if (phrase_parse(iter, end, dtdGrammar, space, document))
    {
    	std::cout << "ok" << std::endl;
    }
    else
    {
        std::cout << "fail" << std::endl;
    }
}