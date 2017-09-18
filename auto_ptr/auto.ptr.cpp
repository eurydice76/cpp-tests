#include <iostream>

int main() {
    std::cout<<"auto"<<std::endl;
    int x1 = 1;
    auto r1 = x1;
    std::cout<<"int        "<<&x1<<" "<<&r1<<" "<<x1<<" "<<r1<<std::endl;

    const int x2 = x1;
    auto r2 = x2;
    r2 = 1000;
    std::cout<<"const int  "<<&x2<<" "<<&r2<<" "<<x2<<" "<<r2<<std::endl;

    const int& x3 = x1;
    auto r3 = x3;
    r3 = 1000;
    std::cout<<"const int& "<<&x3<<" "<<&r3<<" "<<x3<<" "<<r3<<std::endl;

    std::cout<<std::endl;

    std::cout<<"auto&"<<std::endl;
    int x4 = 4;
    auto& r4 = x4;
    r4 = 5000;
    std::cout<<"int        "<<&x4<<" "<<&r4<<" "<<x4<<" "<<r4<<std::endl;

    const int x5 = x4;
    auto& r5 = x5;
    // r5 = 1000; --> will not compile because r5 is now a const int&
    std::cout<<"const int  "<<&x5<<" "<<&r5<<" "<<x5<<" "<<r5<<std::endl;

    const int& x6 = x4;
    auto& r6 = x6;
    // r6 = 1000; --> will not compile because r6 is now a const int&
    std::cout<<"const int& "<<&x6<<" "<<&r6<<" "<<x6<<" "<<r6<<std::endl;

    std::cout<<std::endl;

    std::cout<<"auto&&"<<std::endl;
    int x7 = 7;
    auto&& r7 = x7;
    r7 = 8000;
    std::cout<<"int        "<<&x7<<" "<<&r7<<" "<<x7<<" "<<r7<<std::endl;

    const int x8 = x7;
    auto&& r8 = x8;
    // r8 = 9000; --> will not compile because r8 is now a const int&;
    std::cout<<"const int  "<<&x8<<" "<<&r8<<" "<<x8<<" "<<r8<<std::endl;

    const int& x9 = x7;
    auto&& r9 = x9;
    // r9 = 9000; --> will not compile because r9 is now a const int&;
    std::cout<<"const int& "<<&x9<<" "<<&r9<<" "<<x9<<" "<<r9<<std::endl;

    std::cout<<std::endl;

    auto&& r10 = 10;
    std::cout<<"temp       "<<&r10<<" "<<r10<<std::endl;
    r10 = 100;
    std::cout<<"temp       "<<&r10<<" "<<r10<<std::endl;
 
}
