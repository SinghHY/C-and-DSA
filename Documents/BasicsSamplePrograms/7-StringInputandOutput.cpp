#include <iostream>
#include <string>
using namespace std;

int main()
{
	string firstName, lastName;
	cout << "What is your first name: ";
	cin >> firstName;
	cout << "What is your last name: ";
	cin >> lastName;
	cout << "Hello " << firstName << " " << lastName << endl;//Printing firstname and lastname
	cout << "What is your full name: ";
	cin.ignore();//statement will discard the input value
	string fullName;
	getline(cin,fullName);//reads complete string including spaces,everything aftre first space is ignored 
	cout << fullName;
	return 0;
}

