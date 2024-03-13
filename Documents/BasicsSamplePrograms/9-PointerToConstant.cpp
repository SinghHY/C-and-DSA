#include<iostream>
using namespace std;
int main()
{
	int k = 20;
	int const *p = &k;
	k=30;
	cout<<*p;                 //30
}

