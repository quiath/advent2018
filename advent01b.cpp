#include <iostream>
#include <fstream>
#include <vector>
#include <set>

int task()
{
    std::ifstream inf("input1.txt");
    std::vector<int> data;
    std::set<int> reached{0};
    auto v{0}, s{0};
    while (inf >> v)
    {
        data.emplace_back(v);
    }

    while (true)
    {   
        for (auto x : data)
        {
            s += x;
            if (reached.find(s) != reached.end())
            {
                return s;
            }
            reached.insert(s);
        }
    }
    return -99999;
}

int main()
{
    std::cout << task() << std::endl;
    return 0;
}
