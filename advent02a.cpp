#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

std::vector<std::string> read()
{
    std::ifstream inf("input2.txt");
    std::string x;
    std::vector<std::string> v;
    while (inf >> x)
    {
        v.emplace_back(x);
        //std::cout << ">" << x << "<" << std::endl;
    }
    return v;
}

int task()
{
    auto v = read();
    int s2{0}, s3{0};
    for (auto&& s : v)
    {
        std::map<char, int> cnt_ch;
        std::map<int, int> cnt_cnt;
        for (auto ch : s)
        {
            cnt_ch[ch]++;
            cnt_cnt[cnt_ch[ch]]++;
            if (cnt_ch[ch] > 1)
            {
                cnt_cnt[cnt_ch[ch] - 1]--;
            }
        }
        s2 += (cnt_cnt[2] > 0);
        s3 += (cnt_cnt[3] > 0);

    }
    return s2 * s3;
}


int main()
{
    std::cout << task() << std::endl;
}
