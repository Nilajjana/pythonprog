#include <iostream>
#include <vector>
#include <map>
using namespace std;
vector<int> arr1;
vector<int> arr2;
vector<int> arr3;
int main()
{
    int nt;// no of test cases
    cout << "print the value of nt no of test cases";
    cin >> nt;
    int i;
    for(i=0;i<nt;i++)
    {
        int n;
        cout << "print the value of n no of values n arr";
        cin >> n;
        int j;
        int k;
        for(j=0;j<n;j++)
        {
            cin >> arr1[j];
            cin >> arr2[j];
        }
        int lnds=0;
        arr3[0]=arr1[0];
        int comp=0;
        int l=0;
        if(n>1)
        {
            while(lnds==1)
            {
                map<int,int> ds;
                int dsc;
                for(j=1;j<n;j++)
                {
                    while(comp!=arr2[j])
                    {
                        if(arr1[j-1]<=arr2[j]q)
                        {
                            j++;
                        }
                        else()
                    }
                }
            }
        }
        else
            cout <<arr3[0]+"/n";
    }
}