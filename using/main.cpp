#include <iostream>
#include <typeinfo>
#include <vector>

#include "A.h"

int main() {

    A::vecint v;

    A a;

    A::vecint vint = a.vint();

    std::cout<<(typeid(A::vecint)==typeid(std::vector<int>))<<std::endl;

}
