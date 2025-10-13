%{
    #include "mesh.hpp"
%}

%defines %union { Object* o; }

%token      DUMP CLEAR
%token<o>   INT NUM
%token      LIST DPDK
%%
ini:|ini command    // recursive parsing

command : DUMP      { dump();               }
        | CLEAR     { clear();              }
        | LIST      { Eth::list();          }
        | DPDK      { push(new Eth(((Int*)pop())->value)); }
        | INT       { push($1);             } 
        | NUM       { push($1);             }
