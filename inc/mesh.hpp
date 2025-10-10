#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>

/// @defgroup main main
/// @{
extern int main(int argc, char *argv[]);  ///< POSIX entry point
extern void arg(int argc, char *argv);    ///< process command line argument
/// @}

/// @defgroup skelex skelex
/// @{
extern int yylex();                    ///< lexer
extern int yylineno;                   ///< current file line number
extern char *yyfile;                   ///< current file name
extern FILE *yyin;                     ///< current @ref FILE handler
extern char *yytext;                   ///< lexeme (token) string value
extern int yyparse();                  ///< parser
extern void yyerror(const char *msg);  ///< syntax error callback
#include "mesh.yacc.hpp"
/// @}
