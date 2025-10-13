#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <sstream>

/// @defgroup REPL REPL

/// @defgroup main main
/// @brief system startup
/// @ingroup REPL
/// @{
extern int main(int argc, char *argv[]);  ///< POSIX entry point
extern void arg(int argc, char *argv);    ///< process command line argument
/// @}

/// @defgroup gc gc
/// @brief Garbage collection
/// @ingroup memory

/// @defgroup vm VM
/// @brief FORTH Virtual Machine (for scripting)
/// @{

/// @defgroup ograph ograph
/// @brief object graph
/// @{

/// @brief root object graph class
class Object {
    std::string tag();
    virtual std::string val();

   public:
    /// @ingroup gc
    /// reference counter
    size_t ref;
    Object();
    virtual ~Object();
    virtual std::string dump(std::string prefix = "");
};

/// @defgroup prim primitive

/// @brief primitive types
/// @ingroup prim
class Primitive : public Object {
   public:
    Primitive();
};

/// @brief integer number
/// @ingroup prim
class Int : public Primitive {
    int value;

   public:
    Int(char *V);
    std::string val();
};

/// @brief floating point number
/// @ingroup prim
class Num : public Primitive {
    float value;

   public:
    Num(char *V);
    std::string val();
};
/// @}

/// @defgroup config config
/// @brief @ref memory sizes
/// @{

/// @brief @ref M size
#define Msz 0x10000
/// @brief @ref R size
#define Rsz 0x100
/// @brief @ref D size
#define Dsz 0x10
/// @}

/// @defgroup type type
/// @brief @ref vm specific types
/// @{
typedef uint8_t byte;   ///< single byte
typedef uint16_t addr;  ///< short @ref M address
typedef Object *cell;   ///< storage element type
/// @}

/// @defgroup memory memory
/// @{

extern cell M[Msz];  ///< main memory
extern addr Cp;      ///< compiler pointer
extern addr R[Rsz];  ///< return stack
extern byte Rp;      ///< @ref R pointer
extern cell D[Dsz];  ///< data stack
extern byte Dp;      ///< @ref D pointer

/// @}

/// @defgroup command command
/// @{

/// @name debug
/// @{
extern void dump();           ///< `( -- )` dump @ref VM state
/// @}

/// @name stack
/// @{
extern void push(Object *o);  ///< `( -- o )` push to @ref D
extern Object *pop();         ///< `( o -- )` pop from @ref D
extern Object *top();         ///< `( o -- o )` get @ref D top
extern void clear();          ///< `( ... -- )` clear @ref D
/// @}

/// @}

/// @}

/// @defgroup net net
/// @brief raw networking
/// @{
extern void list();  ///< list available nic's
/// @}

/// @defgroup skelex skelex
/// @brief syntax parser
/// @ingroup REPL
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
