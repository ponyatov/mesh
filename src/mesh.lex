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

{s}{n}+         TOKEN(Int,INT)

"?"             {return DUMP;}
"list"          {return LIST;}

[ \t\r\n]+      {}                  // drop spaces
.               {yyerror("");}      // any undetected char
%%
char *yyfile = nullptr;
