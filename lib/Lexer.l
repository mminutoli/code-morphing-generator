%{
#include <AlternativesScanner.hpp>

#include <string>

typedef IA::AlternativesParser::token token;

#define TOKEN(x)  yylval->sval = new std::string(yytext); return x

#define yyterminate() return(token::END)

%}

%option debug
%option nodefault
%option yyclass="AlternativesScanner"
%option noyywrap
%option c++


DIGIT        ([1-9][0-9]*)|0
REGISTER     r{DIGIT}

%%

[ \r\n\t\f\v]+ { /* Ignore white spaces */ }
{REGISTER}     TOKEN(token::REG);
"{"            TOKEN(token::LBRACE);
"}"            TOKEN(token::RBRACE);
"["            TOKEN(token::LSQUARE);
"]"            TOKEN(token::RSQUARE);
"("            TOKEN(token::LPAR);
")"            TOKEN(token::RPAR);
"="            TOKEN(token::ASSIGN);
"not"          TOKEN(token::OP);
"and"          TOKEN(token::BINOP);
"or"           TOKEN(token::BINOP);
"xor"          TOKEN(token::BINOP);
"add"          TOKEN(token::BINOP);
"sub"          TOKEN(token::BINOP);
"mul"          TOKEN(token::BINOP);
"lsh"          TOKEN(token::BINOP);
"rsh"          TOKEN(token::BINOP);
"load"         TOKEN(token::BINOP);
"store"        TOKEN(token::BINOP);
.              { }
