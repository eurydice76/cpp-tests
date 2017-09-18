#include <iostream>

struct Toto {
	
	Toto(int x) : _x(x){}

	int _x;
};

int main() {

	Toto* pi = new Toto(100);
	pi->~Toto();
	std::cout<<pi->_x<<std::endl;
	new (pi) Toto(500);
	std::cout<<pi->_x<<std::endl;
}
