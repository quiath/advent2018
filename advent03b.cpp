#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

auto s2 = [](const auto& s, auto& x, auto& y)
{
    size_t i{0};
    x = std::stoi(s, &i);  
    y = std::stoi(s.substr(i + 1), nullptr);
};

int task2()
{
    std::ifstream inf("input3.txt");
    std::string s;
    auto col{0};

    //int const K = 1000;
    //std::vector<int> v(K * K, 0);

    //auto cnt = 0;
    auto id{0}, x{0}, y{0}, w{0}, h{0};

    struct S {
        int x{0}, y{0}, w{0}, h{0}, id{0};
    };
    std::vector<S> q;

    while (inf >> s)
    {
        //std::cout <<  col << "'" << s << "'" << std::endl;
        switch (col)
        {
        case 0:
            id = std::stoi(s.substr(1)); 
            break;
        case 2:
            s2(s, x, y);
            //std::cout << x << "," << y << std::endl;
            break;
        case 3:
            s2(s, w, h);
            //std::cout << w << "," << h << std::endl;
            q.emplace_back(S{x, y, w, h, id});


            break;

        default:
            ;
        
        }

        col = (col + 1) % 4;
    }

    std::set<int> bad;

    for (auto i = 0; i < q.size(); ++i)
    {
        for (auto j = i + 1; j < q.size(); ++j)
        {
            auto& r{q[i]};
            auto& s{q[j]};
            if (s.x < r.x + r.w && r.x < s.x + s.w &&
                s.y < r.y + r.h && r.y < s.y + s.h)
            {
                bad.insert(r.id);
                bad.insert(s.id);

            if (r.id == 71 && s.id == 298) 
            {
                std::cout << (s.x < r.x + r.w) << (r.x < s.x + s.w) <<
                             (s.y < r.y + r.h) << (r.y < s.y + s.h) << std::endl;
            }



            }
        }
    }
   
   for (auto i = 1; i < q.size() + 1; ++i)
   {
       if (bad.count(i) == 0)
       {
           std::cout << "id==" << i << std::endl;
       }
   }

    
    auto prev = 0;
    for (auto id : bad)
    {
        if (id - prev > 1)
        {
            std::cout << "id " << prev + 1 << std::endl;
        }
        prev = id;
    }

    return 0;
}

int main()
{
    std::cout << task2() << std::endl;
    return 0;
}
