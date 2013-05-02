#ifndef _ALTERNATIVESSCANNER_H_
#define _ALTERNATIVESSCANNER_H_

#if ! defined (yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#undef YY_DECL
#define YY_DECL int IA::AlternativesScanner::yylex()

#include "Parser.tab.hh"

namespace IA {

class AlternativesScanner : public yyFlexLexer
{
 public:
  AlternativesScanner(std::istream *in);

  int yylex(AlternativesParser::semantic_type * lval);

 private:
  int yylex();
  AlternativesParser::semantic_type * yylval;
};

}

#endif /* _ALTERNATIVESSCANNER_H_ */
