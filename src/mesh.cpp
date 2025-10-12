#include "mesh.hpp"

int main(int argc, char *argv[]) {  //
    arg(0, argv[0]);
    for (int i = 1; i < argc; i++) {  //
        arg(i, argv[i]);
    }
}

void arg(int argc, char *argv) {  //
    std::cerr << "argv[" << argc << "] = <" << argv << "]\n";
    if (argc) {
        yyfile = argv;
        assert(yyin = fopen(yyfile, "r"));
        yyparse();
        fclose(yyin);
        yyfile = nullptr;
    }
}

void yyerror(const char *msg) {  //
    std::cerr << "\n\n"
              << yyfile << ":" << yylineno << " " << msg << " [" << yytext
              << "]\n\n";
    exit(-1);
}
