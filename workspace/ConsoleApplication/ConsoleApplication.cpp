#include <iostream>
using namespace std;

void Foo(int a, int& b, int* pA)
{
	a++;
	b++;
	*pA = *pA + 1;
}
void swap(int& a, int& b)
{
	int tmp;
	tmp = a;
	a = b;
	b = tmp;
}

int main()
{
	int n;
	n = 10;
	int& m = n;/*Storing n value in the address of m*/
	n++;
	cout << m << " " << n << endl;	/*printing m,n values*/
	m++;
	cout << m << " " << n << endl;	/*printing the m,n values after incrementing the m value*/
	int x, y, z;
	x = y = z = 10;
	Foo(x, y, &z);	/*calling Foo() function from main() by passing x,y values and address of z as paarmeters*/
	cout << x << " " << y << " " << z << endl;
	x = 100;
	y = 200;
	swap(x, y);
	cout << x << " " << y << endl;
}

