#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

int main(int argc, char const *argv[])
{
	int n;
	cout<<"Enter the number of bits\n";
	cin>>n;
	int m1 = log2(n)+1;
	int m2 = n+log2(n+m1)+1;
	vector<int> v(m2,0);
	cout<<"Enter the bits"<<endl;
	int ind = 2,mask = 4,j,m;
	while(ind<m2)
	{
		cin>>v[ind];
		m = 1;
		while(m<=ind+1)
		{
			if(m&(ind+1))
			{
				v[m-1]^=v[ind];
			}
			m = m<<1;
			//j++;
		}
		ind++;
		if(ind==mask-1)
		{
			ind++;
			mask = mask<<1;
		}
	}
	int pos;
	cout<<"Enter the position where you want to introduce error\n";
	cin>>pos;
	v[pos-1] = !v[pos-1];
	pos = 0;
	vector<int> v1(m2-n,0);
	mask = 1;
	for(int i=0;i<m2;i++)
	{
		if(i==mask-1)
		{
			mask = mask<<1;
			continue;
		}
		m = 1;j = 0;
		while(m<=i+1)
		{
			if(m&(i+1))
			{
				v1[j]^=v[i];
			}
			m = m<<1;
			j++;
		}
	}
	for(int i=0;i<m2-n;i++)
	{
		if(v1[i]!=v[(1<<i)-1])
		{
			pos+=(1<<i);
		}
	}
	cout<<"ERROR at: "<<pos<<endl;
	return 0;
}