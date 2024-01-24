#include<iostream>
using namespace std;

struct Complex
{
	int Real;
	int Imaginary;
};

Complex operator+(const Complex &c1,const Complex &c2)
{
	Complex c3;
	c3.Real = c1.Real + c2.Real;
	c3.Imaginary = c1.Imaginary + c2.Imaginary;
	return c3;
}
Complex& operator+(const Complex &c,int n)
{
	Complex c3;
	c3.Real = c.Real + n;
	c3.Imaginary = c.Imaginary;
	return c3;
}
Complex& operator+(int n, const Complex &c)
{
	return c+n;
}
ostream& operator<<(ostream &out,Complex c)
{
	out << c.Real << " " << c.Imaginary << endl;
	return out;
}

int main()
{
	Complex c1, c2;
	c1.Real = 10;
	c1.Imaginary = 100;
	c2.Real = 20;
	c2.Imaginary = 200;
	Complex c3 = c1 + c2;
	cout << c3.Real << " " << c3.Imaginary << endl;//display real,imaginary using cout without operator overloading
	c3 = c1 + 10;//By using '+' operator by passing structure and number
	c3 = 10 + c1;//Here,we are calling '+' opeartor  by number and structure
	cout << c1 << c2 << c3;//By using operator overloading displaying complex strcture 
}

