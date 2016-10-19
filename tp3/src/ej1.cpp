#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<int64_t> make_kmp_table(const string& s) {
    vector<int64_t> v(s.size());
    v[0] = -1;
    v[1] = 0;

    int64_t pos = 2;
    int64_t next = 0;
    while (pos < (unsigned)s.length()) {
        if (s[pos - 1] == s[next]) {
            v[pos] = next + 1;
            next++;
            pos++;
        } else if (next > 0) {
            next = v[next];
            v[pos] = 0;
        } else {
            v[pos] = 0;
            pos++;
        }
    }

    return v;
}

bool kmp(const string& t, const string& s) {
    if (t.length() < s.length())
        return false;

    auto table = make_kmp_table(s);

    int64_t match = 0;
    int64_t pos = 0;
    while (match + pos < (unsigned)t.length()) {
        if (s[pos] == t[match + pos]) {
            if (pos == (unsigned)s.length() - 1)
                return true;
            pos++;
        } else {
            if (table[pos] > -1) {
                match += pos - table[pos];
                pos = table[pos];
            } else {
                match++;
                pos = 0;
            }
        }
    }

    return false;
}

int main() {
    string t, s;
    cin >> t >> s;

    cout << (kmp(t, s) ? 'S' : 'N') << endl;

    return 0;
}
