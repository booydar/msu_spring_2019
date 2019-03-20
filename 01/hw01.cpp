//#include "timer.h"
#include "numbers.dat"
#include <iostream>

int main(int arg1, char** arg2)
{
	using namespace std;
	//Timer t;
	long int n = 100002;
	long int i=0, j=0;
	long int lpos, rpos;
	long int left, right;
	//h++long int pairs[30,2]
	

	if((arg1 <= 2) || (arg1%2 ==0))
		return -1;
	for(i=1; i<arg1; i+=2)
	{
	
	left = atoi(arg2[i]);
	right = atoi(arg2[i+1]);
	//cin >> left >> right;
	
	
	
	for(i=0; i<n; i++)
	{
		if(Data[i] >= left)
			break;
	}
	if(Data[i] == left)
	{
		lpos = i;

	}
	else
	{
		cout << 0 << endl;
		lpos=-1;
		
	}

	
	for(i=0; i<n; i++)
	{
		if(Data[i] > right)
		{
			break;
		}
	}			
	if(Data[i-1] == right)
	{
		rpos = i-1;
		
	}
	else
	{
		cout << 0 << endl;		
		//cout << "\n" << 0 << "rposfail" << '\n';
		rpos=-1;
	}
	
	
	if((lpos != -1) & (rpos != -1))
	{

	//calculating number of primes
	long int m = rpos+1;
	int isprime[n];


	for(i=0; i<n; i++)
		isprime[i]=1;
	isprime[0] = 0;
	isprime[1] = 0;

	long int p=2;

	while(1)
	{
	

	for(i=p*p; i<n; i+=p)
	{
   		isprime[i] = 0;
	}
	
	for(i=p+1; i<n; i++)
		if(isprime[i] == 1)
			break;	

	p = i;
	if(p==n)
		break;



	}
	
	j = 0;

	
	for(i=lpos; i<m; i++)
	{
		if(isprime[Data[i]] == 1){
			j ++;
//			cout << Data[i] << " ";
		}
	}

	cout << j << endl;

	
	}


	}

	


	return 0;

}
