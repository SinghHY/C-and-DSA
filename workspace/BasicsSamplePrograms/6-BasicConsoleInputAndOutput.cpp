#include <iostream>
#include <string>
using namespace std;

int main()
{
	int number;
	cout << "Please enter the number: ";
	cin >> number;
	for(int i=1;i<=10;i++)
	{
		cout << number << " * " << i << " = " << number*i << endl;//Multiplying the Number with i value
	}
	cout << "Enter the two numbers to be added: ";
	int n1,n2;
	cin >> n1 >> n2;
	cout << "Sum of two numbers is: " << n1+n2 << endl;//Printing sum of two numbers
}

