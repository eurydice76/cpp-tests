#include <iostream>

struct A {

    A(){std::cout<<"Default ctor"<<std::endl;}

    A(const A& other) {std::cout<<"Copy ctor"<<std::endl;}

    A(A&& other) {std::cout<<"Move ctor"<<std::endl;}

    A& operator=(const A& other) {std::cout<<"Copy assignment otor"<<std::endl;}

    A& operator=(A&& other) {std::cout<<"Move assignment otor"<<std::endl;}

    ~A(){std::cout<<"dtor"<<std::endl;}

    int _x;
};

// Return a temporary
A getA() {

    return A();
}

int main() {

    const A& ra1 = getA(); // OK: const lvalue ref to a temporary
    //ra1._x = 10; Fail: ra1 is a reference to const A
    
    // A& ra2 = getA(); --> Fail: non-const lvalue reference to a temporary

    A&& ra3 = getA(); // OK: non-const rvalue reference to a temporary
    ra3._x = 10; // OK: ra3 is a reference A

    std::cout<<"A ra4 = ra3 --> ";
    A ra4 = ra3; // OK but will call the A copy ctor because ra3 is a rvalue reference which IS a lvalue
    ra4._x = 1000;    
    std::cout<<"ra3._x = "<<ra3._x<<" ra4._x = "<<ra4._x<<std::endl;

    std::cout<<"A ra5 = std::move(ra3) --> ";
    A ra5 = std::move(ra3); // OK and will call the move constructor because the ra3 lvalue has been casted to a rvalue reference
}

