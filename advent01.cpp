#include <iostream>
#include <fstream>

int task1()
{
    std::ifstream inf("input1.txt");
    auto v{0}, s{0};
    while (inf >> v)
    {
        s += v;
    }
    return s;
}

int main()
{
    std::cout << task1() << std::endl;
    return 0;
}
