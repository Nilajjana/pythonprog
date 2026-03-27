#include <bits/stdc++.h>
using namespace std;

int main() {
    int g, c, l;
    if (!(cin >> g >> c >> l)) return 0;

    int mx = max({g, c, l});
    int mn = min({g, c, l});

    if (mx - mn >= 10) {
        cout << "check again\n";
    } else {
        // median is the middle element after sorting
        vector<int>arr(3);
        arr={g,c,l};
        sort(arr.begin(),arr.end());
        cout << "final " << arr[1] << '\n';
    }
    return 0;
}
