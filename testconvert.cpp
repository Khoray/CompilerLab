#include <bits/stdc++.h>
using namespace std;
struct op {
    string s;
};
struct cmp {
    bool operator () (const op& a, const op& b) const { return a.s < b.s; }
};

int main() {
    map<op, int, cmp> mp;
    return 0;
}