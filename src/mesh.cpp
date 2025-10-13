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
    for (int i = 0; i < Dp; i++) std::cerr << D[i]->dump(" ");
    std::cerr << " ]\n";
}

void push(Object *o) {
    assert(Dp < Dsz);
    D[Dp++] = o;
    o->ref++;
}

Object *pop() {
    assert(Dp < Dsz);
    Object *o = D[--Dp];
    assert(o->ref);
    o->ref--;
    return o;
}

Object *top() {
    assert(Dp < Dsz);
    Object *o = D[Dp - 1];
    assert(o->ref);
    return o;
}

void clear() {
    for (int i = 0; i < Dp; i++) pop();
}

void list() {  //
    std::cerr << "list:"
              << "\n";
}

Object::Object() : ref(0) {}

Object::~Object() { assert(!ref); }

#include <cxxabi.h>

std::string Object::tag() {
    std::string ret =
        abi::__cxa_demangle(typeid(*this).name(), NULL, NULL, nullptr);
    for (char &c : ret) c = tolower(c);
    return ret;
}

std::string Object::val() { return ""; }

std::string Object::dump(std::string prefix) {
    std::ostringstream os;
    os << prefix << tag() << ':' << val();
    return os.str();
}

Primitive::Primitive() : Object() {}

Int::Int(char *V) : Primitive(), value(atoi(V)) {}
Num::Num(char *V) : Primitive(), value(atof(V)) {}

std::string Int::val() {
    std::ostringstream os;
    os << value;
    return os.str();
}

std::string Num::val() {
    std::ostringstream os;
    os << value;
    return os.str();
}
