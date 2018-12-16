#include <iostream>
#include <fstream>
#include <string>
#include <vector>

auto s2 = [](const auto& s, auto& x, auto& y)
{
    size_t i{0};
    x = std::stoi(s, &i);  
    y = std::stoi(s.substr(i + 1), nullptr);
};

int task1()
{
    std::ifstream inf("input3.txt");
    std::string s;
    auto col{0};

    int const K = 1000;
    std::vector<int> v(K * K, 0);

    auto cnt = 0;
    auto x{0}, y{0}, w{0}, h{0}, id{0};
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

            {
                cnt = 0;
                for (auto i = 0; i < w; ++i)
                {
                    for (auto j = 0; j < h; ++j)
                    {
                        auto& z = v[(y + j) * K + (x + i)];

                        if (id == 298)
                        {
                            std::cout << (x + i) << "," << (y + j) << ":" << z << std::endl;
                        }


                        z = id ; //z++;
                        cnt += (z >= 2);
                    }
                }
                if (cnt == 0)
                {
                    std::cout << id << ":" << cnt << std::endl;
                }
            }
            break;

        default:
            ;
        
        }

        col = (col + 1) % 4;
    }
    return cnt;
}

int main()
{
    std::cout << task1() << std::endl;
    return 0;
}
