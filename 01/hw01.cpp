#include "numbers.dat"
#include <iostream>

using namespace std;

long int pos(long int value)
{
	int i=0;
	long int n = 100002, ps;

	for(i=0; i<n; i++)
	{
		if(Data[i] >= value)
			break;
	};
	if(Data[i] == value)
		ps = i;
	else
	{
		cout << 0 << endl;
		ps = -1;			
	}

	return ps;

}

int main(int arg1, char** arg2)
{
	long int n = 100002;
	long int i=0, j=0;
	long int lpos, rpos;
	long int left, right;

	if((arg1 <= 2) || (arg1%2 ==0))
		return -1;
	for(int k=1; k<arg1; k+=2)
	{
	
		left = atoi(arg2[k]);
		right = atoi(arg2[k+1]);

		if(left > right)
		{
			cout << 0 << endl;
			continue;
		}
		
		lpos = pos(left);
		rpos = pos(right);
		while(Data[rpos+1] == right)
			rpos++;
		
		if((lpos == -1) || (rpos == -1))
			cout << 0 << endl;
			
		else
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
				if(isprime[Data[i]] == 1)
				{
					j ++;
				}
			}
			
		cout << j << endl;			
		}
	}
	return 0;
}
