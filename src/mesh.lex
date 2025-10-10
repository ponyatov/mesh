%{
    #include "mesh.hpp"
%}

%option noyywrap yylineno

%x comment

%%
\#!.*           {}                  // #! shebang
\#.*            {}                  // # line comment

"/*"            {BEGIN(comment);}   // \ block comment
<comment>"*/"   {BEGIN(INITIAL);}   // |
<comment>.      {}                  // /

"list"          {return LIST;}

[ \t\r\n]+      {}                  // drop spaces
.               {yyerror("");}      // any undetected char
%%
char *yyfile = nullptr;
