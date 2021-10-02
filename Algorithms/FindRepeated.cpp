#include <iostream>
#include <map>
#include <string>
#include <list>

using namespace std;

unsigned int ELFHash(string str)
{
    unsigned int hash = 0, x = 0;
    for (string::iterator iter = str.begin(); iter != str.end(); iter++) {
        hash = (hash << 4) + (*iter);
        if ((x = hash & 0xF0000000L) != 0) {
            hash ^= (x >> 24);
            hash &= ~x;
        }
    }
    return (hash & 0x7FFFFFFF);
}

list<string> FindDNA(string str)
{
    string tmp_str;
    unsigned int key;
    map<unsigned int, list<string> > Cand;
    list<string> res;
    for (int i = 0; i + 9 < str.length(); i++) {
        tmp_str.assign(str, i, 10);
        key = ELFHash(tmp_str);
        // find if ELFHash(tmp_str) in Cand
        if (Cand.find(key) != Cand.end()) {
            list<string> tmp_list = (Cand.find(key)->second);
            // find if tmp_str in Cand
            bool find_in_Cand = false;
            for (list<string>::iterator it1 = tmp_list.begin(); it1 != tmp_list.end(); it1++) {
                if (*it1 == tmp_str) {
                    find_in_Cand = true;
                    // find if tmp_str in res
                    bool find_in_res = false;
                    for (list<string>::iterator it2 = res.begin(); it2 != res.end(); it2++) {
                        if (*it2 == tmp_str) {
                            find_in_res = true;
                            break;
                        }
                    }
                    if (find_in_res == false)
                        res.push_back(tmp_str);
                    break;
                }
            }
            if (find_in_Cand == false)
                (Cand.find(key)->second).push_back(tmp_str);
        }
        else {
            list<string> tmp_list;
            tmp_list.push_back(tmp_str);
            Cand.insert(pair<unsigned int, list<string> >(key, tmp_list));
        }
    }
    return res;
}


int main() 
{
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        string str;
        cin >> str;
        list<string> res = FindDNA(str);
        if (res.begin() == res.end())
            cout << "[]" << endl;
        else {
            list<string>::iterator iter = res.begin();
            cout << '[';
            cout << *iter++;
            while (iter != res.end())
                cout << ',' << *iter++;
            cout << ']' << endl;
        }
    }
    return 0;
}