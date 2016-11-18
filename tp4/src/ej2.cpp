#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n;
    cin >> n;

    vector<int> v(n);
    for (int i = 0; i < n; i++) cin >> v[i];

    sort(v.begin(), v.end());

    double res = 1;
    auto itEnd = v.begin();
    for (auto it = v.begin(); it != v.end(); it++) {
        itEnd = find_if_not(itEnd, v.end(), [&it](int k) { return k == *it; });
        int iguales = distance(it, itEnd);
        int total = distance(it, v.end());
        res += (double)iguales / total - 1;
    }

    cout << fixed << setprecision(6) << res << endl;

    return 0;
}
