#include<iostream>
#include<vector>
#include <ctime>
#include<cstdlib>
#include<random>
#include <unordered_set>
#include <utility>
using namespace std;
vector<vector<bool>> matrix;
int row,clmn;
vector<pair<int,int>> stckarr;
struct PairHash {
    size_t operator()(const std::pair<int,int>& p) const {
        size_t h1 = std::hash<int>()(p.first);
        size_t h2 = std::hash<int>()(p.second);
        return h1 ^ (h2 << 1);  // XOR + shift for mixing
    }
};
std::unordered_set<std::pair<int,int>, PairHash> stack;
int random_val() {
    return rand() % stckarr.size();
}
void push(int a, int b)
{
    if((a+2)<row&&matrix[a+2][b]==true)
    {
        auto [it, inserted] = stack.insert({a+2, b});
        if(inserted)
        {
            stckarr.push_back({a+2, b});
        }
    }
    if((a-2)>0&&matrix[a-2][b]==true)
    {
        auto [it, inserted] = stack.insert({a-2, b});
        if(inserted)
        {
            stckarr.push_back({a-2, b});
        }
    }
    if((b+2)<clmn&&matrix[a][b+2]==true)
    {
        auto [it, inserted] = stack.insert({a, b+2});
        if(inserted)
        {
            stckarr.push_back({a, b+2});
        }
    }
    if((b-2)>0&&matrix[a][b-2]==true)
    {
        auto [it, inserted] = stack.insert({a, b-2});
        if(inserted)
        {
            stckarr.push_back({a, b-2});
        }
    }
}
std::pair<int,int> choose(int pa,int pb)
{
    int rd=random_val();
    std::pair<int,int> popval=stckarr[rd];
    int a=popval.first;
    int b=popval.second;
    stckarr[rd]=stckarr.back();
    stckarr.pop_back();
    std::pair<int,int> toRemove = {a,b};
    stack.erase(toRemove); 
    matrix[a][b]=false;
    if(pa>a)matrix[pa-1][b]=false;
    if(pa<a)matrix[pa+1][b]=false;
    if(pb<b)matrix[a][pb+1]=false;
    if(pb>b)matrix[a][pb-1]=false;
    return {a,b};
}
int main()
{

    srand(time(0));
    cout<< "enter the no of rows and colums : \n";
    cout<<"rows ";
    cin>> row;
    cout<<"columns ";
    cin>> clmn;
    row=(row*2)+1;
    clmn=(clmn*2)+1;
    matrix.resize(row, vector<bool>(clmn, true));//true indicates that there are walls and the cells have not been visited
    cout<< "enter the the cell index value from which you want the maze to originate odd value for both always: \n";
    int a,b;
    cout<< "row index  ";
    cin>> a;
    cout<<"cols index  ";
    cin>>b;
    int cmplt=1;
    matrix[a][b]=false;
    while(cmplt!=0)
    {
        push(a,b);
        std::pair<int,int> ab=choose(a,b);
        a=ab.first;
        b=ab.second;
        cmplt=stckarr.size();
    }
    int i,j;
    cout<<"the maze generated is\n";
    for(i=0;i<row;i++)
    {
        for(j=0;j<clmn;j++)
        {
            if(matrix[i][j]==true)cout<<"#";
            else cout<<" ";
        }
        cout<<"\n";
    }
    cout<<"he maze row is "<<row<<"cloumns are "<<clmn<<"\n";
    return 0;
}
