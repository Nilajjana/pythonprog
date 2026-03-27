#include<iostream>
#include<vector>
#include <random>
using namespace std;
vector<int> arr1;
int main()
{
    cout<<"enter the no of array elements\n";
    int n,i;
    cin>>n;
    arr1.resize(n);
    random_device rd;            // Seed from hardware entropy source
    mt19937 gen(rd());           // Mersenne Twister engine
    uniform_int_distribution<int> dist(1, 100); // Range [1, 100]
    for ( i = 0; i < n; i++) {
        arr1[i] = dist(gen);
    }
    cout<<"the set taken is: {";
    for(i=0;i<n;i++)
    {
        cout<<arr1[i]<<", ";
    }
    cout<<"}";
    cout<<"\n the possible subsets of this array are \n";
    int mask,j;
    cout<<"(";
    for(mask=0;mask<(1<<n);mask++)
    {
        cout<<"{";
        for(j=0;j<n;j++)
        {
            if(mask&(1<<j))
            {
                cout<<arr1[j]<<",";
            }
        }
        cout<<"} \n";
    }
    cout<<") \n";
    return 0;
}
