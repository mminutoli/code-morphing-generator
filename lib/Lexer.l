%{
#include <AlternativesScanner.hpp>

#include <sstream>
#include <string>

typedef IA::AlternativesParser::token token;

#define TOKEN(x)  yylval->sval = new std::string(yytext); return x
#define NUMBER(x) std::istringstream(yytext) >> yylval->eval; return x

#define yyterminate() return(token::END)

%}

%option debug
%option nodefault
%option yyclass="AlternativesScanner"
%option noyywrap
%option c++


DIGIT        ([1-9][0-9]*)|0
REGISTER     r{DIGIT}
CONSTANT     const{DIGIT}
RANDOM       rand{DIGIT}

%%

[ \r\n\t\f\v]+ { /* Ignore white spaces */ }
{REGISTER}     TOKEN(token::REG);
{CONSTANT}     TOKEN(token::CONST);
{RANDOM}       TOKEN(token::RAND);
"{"            TOKEN(token::LBRACE);
"}"            TOKEN(token::RBRACE);
"["            TOKEN(token::LSQUARE);
"]"            TOKEN(token::RSQUARE);
"("            TOKEN(token::LPAR);
")"            TOKEN(token::RPAR);
"="            TOKEN(token::ASSIGN);
"not"          TOKEN(token::OP);
"neg"          TOKEN(token::OP);
"and"          TOKEN(token::BINOP);
"or"           TOKEN(token::BINOP);
"xor"          TOKEN(token::BINOP);
"add"          TOKEN(token::BINOP);
"sub"          TOKEN(token::BINOP);
"mul"          TOKEN(token::BINOP);
"lsh"          TOKEN(token::BINOP);
"rsh"          TOKEN(token::BINOP);
"load"         TOKEN(token::OP);
"store"        TOKEN(token::BINOP);
{DIGIT}        NUMBER(token::NUMBER);
"!"            TOKEN(token::LNOT);
"~"            TOKEN(token::BNOT);
"*"            TOKEN(token::MULT);
"/"            TOKEN(token::DIV);
"+"            TOKEN(token::PLUS);
"-"            TOKEN(token::MINUS);
"^"            TOKEN(token::XOR);
"&"            TOKEN(token::AND);
"|"            TOKEN(token::OR);
.              { }
