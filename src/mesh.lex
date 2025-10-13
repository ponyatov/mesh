%{
    #include "mesh.hpp"
%}

%option noyywrap yylineno

%x comment

s [+\-]
n [0-9]

%%
\#!.*           {}                  // #! shebang
\#.*            {}                  // # line comment

"/*"            {BEGIN(comment);}   // \ block comment
<comment>"*/"   {BEGIN(INITIAL);}   // |
<comment>.      {}                  // /

{s}?{n}+\.{n}?  TOKEN(Num,NUM)
{s}?{n}+        TOKEN(Int,INT)

"dump"          {return DUMP;}
"?"             {return DUMP;}
"clear"         {return CLEAR;}
"."             {return CLEAR;}

"list"          {return LIST;}

[ \t\r\n]+      {}                  // drop spaces
.               {yyerror("");}      // any undetected char
%%
char *yyfile = nullptr;
