#include <bits/stdc++.h>
using namespace std;

// median of three: return iterator pointing to the median of *a, *b, *c
template<typename RandomIt>
RandomIt median_of_three_it(RandomIt a, RandomIt b, RandomIt c) {
    if (*a < *b) {
        if (*b < *c) return b;    // a < b < c
        else if (*a < *c) return c;// a < c <= b
        else return a;             // c <= a < b
    } else {
        if (*a < *c) return a;     // b <= a < c
        else if (*b < *c) return c;// b < c <= a
        else return b;             // c <= b <= a
    }
}

// Tukey's ninther pivot selection
// Returns iterator to chosen pivot within [first, last)
// If range has size < 9, falls back to median_of_three on (first, mid, last-1)
template<typename RandomIt>
RandomIt tukeys_ninther_it(RandomIt first, RandomIt last) {
    using diff_t = typename std::iterator_traits<RandomIt>::difference_type;
    diff_t n = last - first;
    if (n <= 0) return first; // empty range (caller should avoid)
    if (n < 9) {
        RandomIt mid = first + n/2;
        RandomIt lastm = last - 1;
        return median_of_three_it(first, mid, lastm);
    }

    auto clamp_index = [&](diff_t idx) -> RandomIt {
        if (idx < 0) idx = 0;
        if (idx >= n) idx = n - 1;
        return first + idx;
    };

    // chosen offsets (spread across the range)
    diff_t a_i = 0;
    diff_t b_i = n/8;
    diff_t c_i = n/4;

    diff_t d_i = (n/2) - n/8;
    diff_t e_i = (n/2);
    diff_t f_i = (n/2) + n/8;

    diff_t g_i = n - 1 - n/4;
    diff_t h_i = n - 1 - n/8;
    diff_t i_i = n - 1;

    RandomIt a = clamp_index(a_i);
    RandomIt b = clamp_index(b_i);
    RandomIt c = clamp_index(c_i);

    RandomIt d = clamp_index(d_i);
    RandomIt e = clamp_index(e_i);
    RandomIt f = clamp_index(f_i);

    RandomIt g = clamp_index(g_i);
    RandomIt h = clamp_index(h_i);
    RandomIt iit = clamp_index(i_i);

    // medians of each trio
    RandomIt m1 = median_of_three_it(a, b, c);
    RandomIt m2 = median_of_three_it(d, e, f);
    RandomIt m3 = median_of_three_it(g, h, iit);

    // median of medians -> final pivot
    return median_of_three_it(m1, m2, m3);
}

// Hoare partition using Tukey's ninther pivot
template<typename RandomIt>
RandomIt hoare_partition(RandomIt first, RandomIt last) {
    // pick pivot using Tukey's ninther
    RandomIt pivot_it = tukeys_ninther_it(first, last);
    auto pivot = *pivot_it;

    RandomIt i = first;
    RandomIt j = last - 1;

    while (true) {
        while (*i < pivot) ++i;
        while (*j > pivot) --j;

        if (i >= j) return j;

        std::iter_swap(i, j);
        ++i;
        --j;
    }
}

// insertion sort for tiny ranges
template<typename RandomIt>
void insertion_sort(RandomIt first, RandomIt last) {
    for (RandomIt i = first + 1; i < last; ++i) {
        auto key = *i;
        RandomIt j = i;
        while (j > first && *(j - 1) > key) {
            *j = *(j - 1);
            --j;
        }
        *j = key;
    }
}

// QuickSort using Hoare partition + ninther pivot (tail recursion elimination)
template<typename RandomIt>
void quicksort(RandomIt first, RandomIt last) {
    while (last - first > 32) {   // cutoff for insertion sort
        RandomIt p = hoare_partition(first, last);
        // recurse on smaller side, iterate on larger (tail recursion)
        if (p - first < last - (p + 1)) {
            quicksort(first, p + 1);
            first = p + 1;  // tail-recurse to sort larger part iteratively
        } else {
            quicksort(p + 1, last);
            last = p + 1;
        }
    }
    // final insertion sort cleanup
    insertion_sort(first, last);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    if (!(cin >> t)) return 0;

    while (t--) {
        int n;
        cin >> n;
        vector<long long> arr;        // store odd fields here
        arr.reserve(n);
        long long sum = 0;
        long long odn = 0;

        for (int j = 0; j < n; ++j) {
            long long a;
            cin >> a;
            if (a % 2 != 0) {
                arr.push_back(a);
                ++odn;
            } else {
                sum += a;
            }
        }

        long long lengt = (long long)arr.size();
        if (lengt > 1) {
            // sort using your quicksort (Tukey's ninther + Hoare)
            quicksort(arr.begin(), arr.end());

            long long ari = lengt / 2;
            for (long long ari2 = lengt - 1; ari2 >= ari; --ari2) {
                sum += arr[(size_t)ari2];
            }
        } else if (lengt == 1) {
            sum += arr[0];
        }

        if (odn > 0) cout << sum << "\n";
        else cout << 0 << "\n";
    }
    return 0;
}
