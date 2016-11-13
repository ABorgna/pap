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
    auto it = v.begin();

    while (it != v.end()) {
        auto itNext =
            find_if_not(it + 1, v.end(), [&it](int k) { return k == *it; });

        int count = distance(it, itNext);
        int restantes = distance(it, v.end());
        for (int i = 0; i < count; i++) {
            res += (double)(restantes - i) / (double)(count - i) - 1;
        }

        it = itNext;
    }

    cout << fixed << setprecision(6) << res << endl;

    return 0;
}
