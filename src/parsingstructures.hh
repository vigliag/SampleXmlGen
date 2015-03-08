//define the grammar for the language to recognize
template <typename Iterator>
struct dtd_grammar : 
	qi::grammar<Iterator, //to make the grammar generic
		std::vector<ElementDefinitionSTM>(), //return attribute
		ascii::space_type> /*type of parser*/
	{
	//constructor
	dtd_grammar() 
	: dtd_grammar::base_type(document) //starting rule is document
    {
        using qi::lit; //literal parser
        using qi::uint_;
        //using qi::lexeme; //does the work of a lexer, supplying terminals, we don't need it
        //using ascii::char_; //parses one char
        //using ascii::string; //parses a string (returning it as attribute)
        using ascii::alnum; //alphanumeric-only string
        using namespace qi::labels;
        using phoenix::at_c; //accesses a field in a struct

        document %= +(element_definition);

        element_definition %= 
        	   "<ELEMENT" 
        	>> ( alnum | attr("tag") )
        	>> ( ( "(" >> uint_ >> ")" ) | attr(1) )
        	>> alnum 
        	>> "(" 
        	>> child_definition % ','
        	>> ")"
			>> pool_definition % ','
			>> ">"
			;

		child_definition %= 
				alnum 
			>> ("[" >> uint_ >> "]" | attr(1))
			;

		pool_definition %=
				   "has" 
				>> ( lit("shared") [at_c<1>(_val) = false] 
					| lit("distinct") [at_c<1>(_val) = true] )
				>> alnum 
			;
    }
    //rule/nonterminal type definitions
    qi::rule<Iterator, std::vector<ElementDefinitionSTM>(), ascii::space_type> document;
    qi::rule<Iterator, ElementDefinitionSTM(), ascii::space_type> element_definition;
    qi::rule<Iterator, ChildDefinitionExp(), ascii::space_type> child_definition;
    qi::rule<Iterator, PoolDefinition(), ascii::space_type> pool_definition;
};

struct ElementDefinitionSTM {
	std::string kind;
	unsigned parameter;
	std::string name;
	std::vector<ChildDefinitionExp> children;
	std::vector<DatapoolExp> dataPools;
};

BOOST_FUSION_ADAPT_STRUCT(
    ElementDefinitionSTM,
    (unsigned, parameter)
	(std::string, name)
	(std::vector<ChildDefinitionExp>, children)
	(std::vector<DatapoolExp> dataPools)
);

struct ChildDefinitionExp {
	string name;
	unsigned amount; //TODO, can be easily extended with an xpath query (currently not supported)
};

BOOST_FUSION_ADAPT_STRUCT(
    ChildDefinitionExp,
    (string, name)
	(unsigned, amount)
);

struct PoolDefinition {
	bool distinct;
	string identifier;
};

BOOST_FUSION_ADAPT_STRUCT(
    PoolDefinition,
    (bool, distinct)
	(string, identifier)
)
