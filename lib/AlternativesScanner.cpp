#include <AlternativesScanner.hpp>


using namespace IA;


AlternativesScanner::AlternativesScanner(std::istream *in)
    : yyFlexLexer(in), yylval(NULL)
{}


int
AlternativesScanner::yylex(AlternativesParser::semantic_type * lval)
{
  yylval = lval;
  return yylex();
}
