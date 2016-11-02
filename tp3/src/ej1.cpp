#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<int64_t> make_kmp_table(const string& s) {
    vector<int64_t> table(s.size());
    table[0] = -1;
    table[1] = 0;

    uint64_t pos = 2;
    int64_t prevBorde = 0;
    while (pos < s.length()) {
        if (s[pos - 1] == s[prevBorde]) {
            prevBorde++;
            table[pos] = prevBorde;
            pos++;
        } else if (prevBorde > 0) {
            prevBorde = table[prevBorde];
        } else {
            table[pos] = 0;
            pos++;
        }
    }

    for(auto i : table) cerr << i << " ";
    cerr << endl;

    return table;
}

bool kmp(const string& t, const string& s) {
    if (t.length() < s.length())
        return false;

    auto table = make_kmp_table(s);

    int64_t i = 0;
    int64_t pos = 0;
    while (pos <= (unsigned)t.length() - (unsigned)s.length()) {
        if (s[i] == t[i + pos]) {
            if (i == (unsigned)s.length() - 1)
                return true;
            i++;
        } else {
            pos += i - table[i];
            i = max((int64_t)0, table[i]);
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
