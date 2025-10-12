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

/// @defgroup vm vm
/// @{

/// @defgroup ograph ograph
/// @{

/// @brief root object graph class
class Object {
    size_t ref;

   public:
    Object();
    virtual ~Object();
};

class Primitive : public Object {
   public:
    Primitive();
};

class Int : public Primitive {
    int value;

   public:
    Int(char *);
};
class Num : public Primitive {
    float value;
    Num(char *);
};
/// @}

/// @defgroup config config
/// @{
/// @ref M size
#define Msz 0x10000
/// @ref R size
#define Rsz 0x100
/// @ref D size
#define Dsz 0x10
/// @}

/// @defgroup type
/// @{
typedef uint8_t byte;   ///< single byte
typedef uint16_t addr;  ///< short @ref M address
typedef Object *cell;   ///< storge element type
/// @}

/// @defgroup memory memory
/// @{

extern cell D[Dsz];  ///< data stack
extern byte Dp;      ///< @ref D pointer

extern void push(Object *o);  ///< `( -- o )` push to @ref D
extern Object *pop();         ///< `( o -- )` pop from @ref D
extern Object *top();         ///< `( o -- o )` get @ref D top
/// @}

/// @defgroup command command
/// @{

extern void dump();  ///< dump @ref VM state

/// @}

/// @}

/// @defgroup dpdk dpdk
/// @{
extern void list();  ///< list available nic's
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
#define TOKEN(C, X)               \
    {                             \
        yylval.o = new C(yytext); \
        return X;                 \
    }
/// @}
