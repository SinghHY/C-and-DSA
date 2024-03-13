#include<iostream>
using namespace std;
int main()
{
	char *const str="TAT";
	str ++;	//Error:deprecated conversion rom string to char*
	cout<<str;
	return 0;
}

