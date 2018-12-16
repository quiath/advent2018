#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>

using std::begin;
using std::end;
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using IntVec = vector<int>;
using StrVec = vector<string>;

inline StrVec read_lines(const string& fn)
{
    std::ifstream inf(fn);
    string s;
    StrVec v;
    while (std::getline(inf, s))
    {
        v.emplace_back(s);
    }
    return v;
};
