#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace fusion = boost::fusion;
namespace phoenix = boost::phoenix;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

struct ChildDefinitionExp {
    std::string name;
    unsigned amount; //TODO, can be easily extended with an xpath query (currently not supported)
};

BOOST_FUSION_ADAPT_STRUCT(
    ChildDefinitionExp,
    (std::string, name)
    (unsigned, amount)
);

struct PoolDefinition {
    bool distinct;
    std::string identifier;
};

BOOST_FUSION_ADAPT_STRUCT(
    PoolDefinition,
    (bool, distinct)
    (std::string, identifier)
)

typedef fusion::vector2<std::string, unsigned> kindDef;

struct ElementDefinitionSTM {
    const boost::optional<kindDef>* kindDefinition();
    std::string name;
    std::vector<ChildDefinitionExp> children;
    const boost::optional<std::vector<PoolDefinition>> *dataPools;
};

BOOST_FUSION_ADAPT_STRUCT(
    ElementDefinitionSTM,
    (const boost::optional<kindDef>*, kindDefinition())
    (std::string, name)
    (std::vector<ChildDefinitionExp>, children)
    (const boost::optional<std::vector<PoolDefinition>>*, dataPools)
);

void print()
    {
        std::cout << "element parsed" << std::endl;
    }
//define the grammar for the language to recognize
template <typename Iterator>
struct dtd_grammar : 
    qi::grammar<Iterator, //to make the grammar generic
        std::vector<ElementDefinitionSTM>(), //return attribute
        ascii::space_type> /*type of parser*/
    {
    //constructor
    dtd_grammar() 
    : dtd_grammar::base_type(document, "dtd_document") //starting rule is document
    {
        using qi::lit; //literal parser
        using qi::uint_;
        using qi::attr;
        using qi::on_error;
        using qi::fail;
        using qi::lexeme; //does the work of a lexer, supplying terminals, we don't need it
        //using ascii::char_; //parses one char
        //using ascii::string; //parses a string (returning it as attribute)
        using ascii::alnum; //alphanumeric-only character
        using namespace qi::labels;
        using phoenix::at_c; //accesses a field in a struct
        using phoenix::construct;
        using phoenix::val;

        tag_identifier %= lexeme[+ alnum];
        tag_identifier.name("tag_identifier");

        type_specifier =  tag_identifier [at_c<0>(_val) = _1]
                         >> ( "[" >> uint_ >> "]" ) [at_c<1>(_val) = _1]
                ;

        pool_definition =
                   lit("has") 
                > ( 
                    lit("shared")     [at_c<0>(_val) = false] 
                    | lit("distinct")   [at_c<0>(_val) = true] 
                    )
                >> tag_identifier                [at_c<1>(_val) = _1]
            ;
        pool_definition.name("pool_definition");


        child_definition %= 
                tag_identifier 
            >> (("[" >> uint_ >> "]") | attr(1U))
            ;
        child_definition.name("child_definition");

        element_definition %= 
               lit("<!ELEMENT")
            > ( - type_specifier ) 
            > tag_identifier                        //[at_c<2>(_val) = _1]
            > lit("(") 
            > (child_definition % ',')              //[at_c<3>(_val) = _1]
            > lit(")")
            >> ( - (pool_definition % ',') )             //[at_c<4>(_val) = _1]
            >> lit(">")
            ;
        element_definition.name("element_definition");

        document %= +(element_definition);
        document.name("document");

        on_error<fail>
            (
                document
              , std::cout
                    << val("Error! Expecting ")
                    << _4                               // what failed?
                    << val(" here: \"")
                    << construct<std::string>(_3, _2)   // iterators to error-pos, end
                    << val("\"")
                    << std::endl
            );
    }
    //rule/nonterminal type definitions
    qi::rule<Iterator, std::vector<ElementDefinitionSTM>(), ascii::space_type> document;
    qi::rule<Iterator, ElementDefinitionSTM(), ascii::space_type> element_definition;
    qi::rule<Iterator, ChildDefinitionExp(), ascii::space_type> child_definition;
    qi::rule<Iterator, PoolDefinition(), ascii::space_type> pool_definition;
    qi::rule<Iterator, std::string(), ascii::space_type> tag_identifier;
    qi::rule<Iterator, kindDef(), ascii::space_type> type_specifier;
    
};
