#include "A_with_include_guards.h"
#include "B_with_include_guards.h"

int main () {

    /*
    This code will compile because thanks to the include guards defined in A.h and B.h
    the struct A is defined once and only once in the main.cpp

    Here is the results of the preprocessing (g++ -E main_with_include_guards)

    # 1 "main_with_include_guards.cpp"
    # 1 "<built-in>"
    # 1 "<command-line>"
    # 1 "/usr/include/stdc-predef.h" 1 3 4
    # 1 "<command-line>" 2
    # 1 "main_with_include_guards.cpp"
    # 1 "A_with_include_guards.h" 1



    struct A {
    };
    # 2 "main_with_include_guards.cpp" 2
    # 1 "B_with_include_guards.h" 1





    struct B {
        A a;
    };
    # 3 "main_with_include_guards.cpp" 2

    int main () {
    # 35 "main_with_include_guards.cpp"
        A a;
    }

    */

    A a;
}
