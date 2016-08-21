#include <stdint.h>
#include <vector>
#include <iostream>
#include <math.h>
#include <algorithm>

using namespace std;

int main () {
    uint64_t n;
    int64_t res = 0;
    int64_t border = 0;

    cin >> n;

    for(uint64_t i=0; i<n; i++) {
        int64_t x;
        cin >> x;

        border = max((int64_t)0, border+x);
        res = max(res,border);
    }

    cout << res << endl;

    return 0;
}

