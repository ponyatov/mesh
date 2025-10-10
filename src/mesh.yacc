%{
    #include "mesh.hpp"
%}

%defines %union { int n; float f; }

%token LIST
%%
ini:|ini command    // recursive parsing

command: LIST       { std::cerr << "\nlist NIC's:\n"; }
