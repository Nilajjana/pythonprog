
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <utility>
#include <cstdlib>
#include <ctime>

namespace py = pybind11;

struct PairHash {
    size_t operator()(const std::pair<int,int>& p) const {
        size_t h1 = std::hash<int>()(p.first);
        size_t h2 = std::hash<int>()(p.second);
        return h1 ^ (h2 << 1);
    }
};

static int row, clmn;
static std::vector<std::vector<bool>> matrix;
static std::vector<std::pair<int,int>> stckarr;
static std::unordered_set<std::pair<int,int>, PairHash> stackset;

inline int to_index(int r, int c) {
    return r * clmn + c;
}

int random_val() {
    return rand() % stckarr.size();
}

void push(int a, int b) {
    if((a+2)<row && matrix[a+2][b]) {
        if(stackset.insert({a+2, b}).second) stckarr.push_back({a+2, b});
    }
    if((a-2)>0 && matrix[a-2][b]) {
        if(stackset.insert({a-2, b}).second) stckarr.push_back({a-2, b});
    }
    if((b+2)<clmn && matrix[a][b+2]) {
        if(stackset.insert({a, b+2}).second) stckarr.push_back({a, b+2});
    }
    if((b-2)>0 && matrix[a][b-2]) {
        if(stackset.insert({a, b-2}).second) stckarr.push_back({a, b-2});
    }
}

std::pair<int,int> choose(int pa,int pb, std::vector<std::pair<int,bool>>& updates) {
    int rd=random_val();
    std::pair<int,int> popval=stckarr[rd];
    int a=popval.first;
    int b=popval.second;

    stckarr[rd]=stckarr.back();
    stckarr.pop_back();
    stackset.erase({a,b});

    matrix[a][b]=false;
    updates.push_back({to_index(a,b), false});

    if(pa>a) { matrix[pa-1][b]=false; updates.push_back({to_index(pa-1,b), false}); }
    if(pa<a) { matrix[pa+1][b]=false; updates.push_back({to_index(pa+1,b), false}); }
    if(pb<b) { matrix[a][pb+1]=false; updates.push_back({to_index(a,pb+1), false}); }
    if(pb>b) { matrix[a][pb-1]=false; updates.push_back({to_index(a,pb-1), false}); }

    return {a,b};
}

std::vector<std::pair<int,bool>> prims_algo(int rows, int cols, int seed_index) {
    row = (rows*2)+1;
    clmn = (cols*2)+1;
    matrix.assign(row, std::vector<bool>(clmn, true));
    stckarr.clear();
    stackset.clear();

    srand(time(0));

    int a = seed_index / clmn;
    int b = seed_index % clmn;

    std::vector<std::pair<int,bool>> updates;

    matrix[a][b] = false;
    updates.push_back({to_index(a,b), false});

    while(!stckarr.empty() || updates.size()==1) {
        push(a,b);
        if(stckarr.empty()) break;
        auto ab = choose(a,b,updates);
        a=ab.first; b=ab.second;
    }

    return updates;
}

PYBIND11_MODULE(maze_cpp, m) {
    m.doc() = "Prim's maze generator wrapped with pybind11";
    m.def("generate", &prims_algo,
          py::arg("rows"), py::arg("cols"), py::arg("seed_index"),
          "Generate maze and return list of (index, is_wall)");
}
