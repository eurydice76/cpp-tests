#include <iostream>

class A {

public:
	// Default ctor
	A();

	// Copy ctor
	A(const A& other);

	// Move ctor
	A(A&& other);

	// Copy assignement otor
	A& operator=(const A& other);

	// Move assignement otor
	A& operator=(A&& other);

	// dtor
	~A();

private:

	int _x;
};

A::A()
{
	std::cout<<"A: default ctor"<<std::endl;
}

A::A(const A& other)
{
	std::cout<<"A: copy ctor"<<std::endl;
	_x = other._x;
}

A::A(A&& other)
{
	std::cout<<"A: move ctor"<<std::endl;
	_x = std::move(other._x);
}

A& A::operator=(const A& other)
{
	std::cout<<"A: copy assignment otor"<<std::endl;
	if (this != &other)
		_x = other._x;
	return *this;
}

A& A::operator=(A&& other)
{
	std::cout<<"A: move assignment otor"<<std::endl;
	if (this != &other)
		_x = std::move(other._x);
	return *this;
}

A::~A()
{
	std::cout<<"A: dtor"<<std::endl;
}

class AContainer {

public:

	// Default ctor
	AContainer();

	// Copy ctor
	AContainer(const AContainer& other);

	// Move ctor
	AContainer(AContainer&& other);

	// Copy assignment otor
	AContainer& operator=(const AContainer& other);

	// Move assignment otor
	AContainer& operator=(AContainer&& other);

	// dtor
	~AContainer();

public:

	A _a;

	int* _ptr;
};

AContainer::AContainer() : _a(), _ptr(new int(100))
{
	std::cout<<"ACONTAINER: default ctor of "<<this<<std::endl;
}

AContainer::AContainer(const AContainer& other)
{
	std::cout<<"ACONTAINER: copy ctor from "<<&other<<" to "<<this<<std::endl;
	_a = other._a;
	_ptr = new int(*other._ptr);

}

AContainer::AContainer(AContainer&& other)
{
	std::cout<<"ACONTAINER: move ctor from "<<&other<<" to "<<this<<std::endl;
	_a = std::move(other._a);
	_ptr = other._ptr;
	other._ptr = nullptr;
}

AContainer& AContainer::operator=(const AContainer& other)
{
	std::cout<<"ACONTAINER: copy assignment otor from "<<&other<<" to "<<this<<std::endl;
	if (this != &other)	{
		_a = other._a;
		_ptr = new int(*other._ptr);
	}
	return *this;
}

AContainer& AContainer::operator=(AContainer&& other)
{
	std::cout<<"ACONTAINER: move assignment otor from "<<&other<<" to "<<this<<std::endl;
	if (this != &other)	{
		_a = std::move(other._a); // other is a rvalue reference but a rvalue reference is an lvalue whence the use of std::move 
		delete _ptr; // The memory block targeted by this pointer has to be deleted before assigning this pointer to the other  		
		_ptr = other._ptr;
		other._ptr = nullptr; // The other pointer has to be nullified to definitely move its memory block to the target  
	}
	return *this;
}

AContainer::~AContainer()
{
	std::cout<<"ACONTAINER: dtor of "<<this<<std::endl;
	delete _ptr;
}

AContainer getContainer() {
	std::cout<<"Within getContainer"<<std::endl;
	AContainer a_container;
	return a_container;
}

int main() {
	AContainer a,b;
	std::cout<<std::endl;

	b = std::move(a); // Move a to b. A is then is an unspecified state (http://www.cplusplus.com/reference/utility/move/)
	std::cout<<std::endl;

	AContainer&& ra = getContainer(); // ra is the rvalue reference, It will "steal" the contents of the temp returned by getContainer
	std::cout<<std::endl;
	return 0;
}


