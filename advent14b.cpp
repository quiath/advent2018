#include "advent.h"
#include <array>



string task2(string search)
{

    string v("37");

    v.reserve(10000);

    auto p{0u};
    auto q{1u};

    while (true)
    {
        auto n{v[p] - '0' + v[q] - '0'};
        auto d1{n / 10};
        auto d0{n % 10};
        if (d1 > 0)
        {
            v += char('0' + d1);
        }
        v += char('0' + d0);
        p = (p + 1 + v[p] - '0') % v.length();
        q = (q + 1 + v[q] - '0') % v.length();

#if 1
        if (search.length() <= v.length())
        {
            for (auto j = 0; j < 2; ++j)
            {
                auto where{std::rbegin(v)};
                std::advance(where, j);
                auto pr = std::mismatch(std::rbegin(search), std::rend(search), where);
                if (pr.first == std::rend(search))
                {
                    std::ostringstream ss;
                    ss << std::to_string(std::distance(pr.second, std::rend(v)));
                    return ss.str();
                }
            }
        }
#else
        //cout << v << endl;
        if (search.length() <= v.length() && v.length() % 1000000 == 0)
        {
            //cout << v.substr(v.length() - search.length()) << endl;
            //if (v.substr(v.length() - search.length()) == search)
            auto ind = v.find(search);
            if (ind != string::npos)
            {
                cout << "->" << v.substr(ind, search.length()) << endl;

                std::ostringstream ss;
                //ss << std::to_string(v.length() - search.length());
                ss << std::to_string(ind);
                return ss.str();
            }
        }
#endif

    }
}

int main()
{
    //auto ret = task2("51589");
    //auto ret = task2("59414");
    auto ret = task2("074501");
    cout << ret << endl;
    return 0;
}
