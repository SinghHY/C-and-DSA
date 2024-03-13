#include<iostream>
using namespace std;

int main()
{
	int n;
	int sum=0;
	cout<<"Enter number, upto you want the sum "<<endl;
	cin>>n;	//gets the value	
	for(int i=0;i<=n;i++)	
	{
		sum+=i;	//adding the number
	}
	cout<<"The sum of the digits:"<<sum; //prints the sum of all numbers
	return 0;
}

