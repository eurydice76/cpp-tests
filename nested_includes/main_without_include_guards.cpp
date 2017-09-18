#include "B_without_include_guards.h"

int main () {

    /*
    This code will compile because in spite of the fact that no include guards have been defined 
    in A_without_include_guards and B_without_include_guards.h, the A_without_include_guards.h is 
    just included once and only once in the main_without_include_guards.cpp file. The include is done
    in the B_without_include_guards.h file. However, this code is much less readable and safe than the 
    one where the include guards has been properly set in the A and B files.

    Here is the results of the preprocessing (g++ -E main_without_include_guards)

    # 1 "main_without_include_guards.cpp"
    # 1 "<built-in>"
    # 1 "<command-line>"
    # 1 "/usr/include/stdc-predef.h" 1 3 4
    # 1 "<command-line>" 2
    # 1 "main_without_include_guards.cpp"
    # 1 "B_without_include_guards.h" 1
    # 1 "A_without_include_guards.h" 1
    struct A {
    };
    # 2 "B_without_include_guards.h" 2

    struct B {
        A a;
    };
    # 2 "main_without_include_guards.cpp" 2

    int main () {
    # 38 "main_without_include_guards.cpp"
        A a;
    }

    */

    A a;
}
