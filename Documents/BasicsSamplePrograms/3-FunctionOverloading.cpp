#include<iostream>
using namespace std;

int sum(int x,int y)
{
	cout<<x+y;
}
int sum(int x,int y,int z)
{
	cout<<x+y+z;
}
int sum(double x,double y)
{
	cout<<x+y;
}
//main function
int main()
{
	//calling the function
  	sum(10,20);
  	sum(10,20,30);
  	sum(10.20,30.7689);
	return 0;
}

