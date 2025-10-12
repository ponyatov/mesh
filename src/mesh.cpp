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

cell D[Dsz];
byte Dp = 0;

void dump() {  //
    std::cerr << "D:[";
    for (int i = 0; i < Dp; i++) std::cerr << ' ' << D[i];
    std::cerr << "]\n";
}

extern void push(Object *o) {
    assert(Dp < Dsz);
    D[Dp++] = o;
}

extern Object *pop() {
    assert(Dp < Dsz);
    return D[--Dp];
}

extern Object *top() {
    assert(Dp < Dsz);
    return D[Dp - 1];
}

void list() {  //
    std::cerr << "list:"
              << "\n";
}

Object::Object() : ref(0) {}

Object::~Object() { assert(!ref); }

Primitive::Primitive() : Object() {}

Int::Int(char *V) : Primitive(), value(atoi(V)) {}
