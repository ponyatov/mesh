%{
    #include "mesh.hpp"
%}

%defines %union { Object* o; }

%token      DUMP CLEAR
%token      DUP DROP PRESS SWAP OVER
%token<o>   INT NUM
%token      LIST DPDK STATUS RECV SEND
%%
ini:|ini command    // recursive parsing

command : DUMP      { dump();                               }
        // stack
        | CLEAR     { clear();                              }
        | DUP       { dup();                                }
        | DROP      { drop();                               }
        | PRESS     { press();                              }
        | SWAP      { swap();                               }
        | OVER      { over();                               }
        // dpdk
        | LIST      { Eth::list();                          }
        | DPDK      { push(new Eth(((Int*)pop())->value));  }
        | STATUS    { ((Eth*)pop())->status();              }
        | RECV      { Recv((Eth*)pop()).run(0); }
        | SEND      { Send((Eth*)pop()).run(0); }
        // numbers
        | INT       { push($1);                             }
        | NUM       { push($1);                             }
