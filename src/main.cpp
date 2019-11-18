#include <iostream>

class C0
{
public:
	C0() = default;
	virtual ~C0() = default;
	virtual C0* clone() const { return new C0(); }
	virtual void foo()  {  std::cout << "I'm C0!" << std::endl;  }
};

template <typename T = int>
class C1 : virtual public C0
{
public:
	C1() = default;
	virtual ~C1() = default;
	// here I would like to return a C1<T>* but there is an unresolved bug with diamond inheritance and covariance in MSVC
	virtual C0* clone() const { return new C1(); }
	virtual void foo() { std::cout << "I'm C1!" << std::endl; }
};

template <typename T = int>
class C2 : virtual public C0
{
public:
	C2() = default;
	virtual ~C2() = default;
	// here I would like to return a C2<T>* but there is an unresolved bug with diamond inheritance and covariance in MSVC
	virtual C0* clone() const { return new C2(); }
	virtual void foo() { std::cout << "I'm C2!" << std::endl; }
};

template <typename T = int>
class C3 : public C1<T>, C2<T>
{
public:
	C3() : C0(), C1<T>(), C2<T>() {}
	virtual ~C3() = default;
	// here I would like to return a C3<T>* but there is an unresolved bug with diamond inheritance and covariance in MSVC
	virtual C0* clone() const { return new C3(); }
	virtual void foo() { std::cout << "I'm C3!" << std::endl; }
};

template <typename T>
using proto = T (*)();

template <typename T>
T func() { return (T)10; }

// P is a non-typed template
template <typename T = int, proto<T> P = func>
class C4 : public C3<T>
{
public:
	C4() : C0(), C3<T>() {}
	virtual ~C4() = default;
	// here is my bug, I can't force MSVC to compile this function (even if I replace 'C4<T,P>*' 'by C3<T>*', note that it compiles with 'C0*' type')
	virtual C4<T,P>* clone() const { return new C4(); }
	virtual void foo() {  std::cout << "I'm C4 : " << P() << "!" << std::endl; }
};

// This code works (without the non-typed template param)
/*
template <typename T = int>
class C4 : public C3<T>
{
public:
	C4() : C0(), C3<T>() {}
	virtual ~C4() = default;
	virtual C4<T>* clone() const { return new C4(); }
	virtual void foo() {  std::cout << "I'm C4!" << std::endl; }
};
*/

int main(int argc, char** argv)
{
	C0* ca = new C4<>();
	C0* cb = ca->clone();

	cb->foo();

	delete ca;
	delete cb;

	return 0;
}