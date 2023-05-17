#include<bits/stdc++.h>
using namespace std;
int main() {
    vector<int> a(1, 5);
    int &x = a[0];
    cout << "x origin:" << x << "\n";
    a.push_back(1);
    cout << "x after:" << x << "\n";

    return 0;
}