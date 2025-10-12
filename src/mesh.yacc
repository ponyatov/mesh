%{
    #include "mesh.hpp"
%}

%defines %union { Object* o; }

%token LIST DUMP
%token<o> INT,NUM
%%
ini:|ini command    // recursive parsing

command : DUMP      { dump();   }
        | LIST      { list();   }
        | INT       { push($1); } 
        | NUM       { push($1); }
