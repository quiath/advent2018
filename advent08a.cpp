#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>
#include <set>



using IntPair = std::pair<int, int>;
using IPV = std::vector<IntPair>;

using CharSet = std::set<char>;
using CharToCharSet = std::map<char, std::set<char>>;

using std::begin;
using std::end;
using std::string;
using std::cin;
using std::cout;
using std::endl;


int task1(string fn)
{
    std::ifstream inf(fn);

    int k {0}, n{0}, metasum{0};

    enum class State { QChild, QMeta, Meta };
    auto state{ State::QChild};

    struct Node
    {
        int qchild{0};
        int cread{0};
        int qmeta{0};
        int mread{0};
    };

    std::vector<Node> stack;

    while (inf >> k)
    {
        cout << int(state) << ":" << k << endl;
        switch (state)
        {
        case State::QChild:
            if (stack.size() > 0)
            {
                --stack.back().cread;
            }
            stack.emplace_back(Node{k, k}); 
            state = State::QMeta;
            break;
        case State::QMeta:
            stack.back().qmeta = k;
            stack.back().mread = k;
            if (stack.back().qchild > 0)
            {
                state = State::QChild;
            }
            else
            {
                state = State::Meta;
            }
            break;
        case State::Meta:
            metasum += k;
            if (--stack.back().mread == 0)
            {
                stack.pop_back();
                if (stack.size() > 0)
                {
                    if (stack.back().cread > 0)
                    {
                        state = State::QChild;
                    }
                    else
                    {
                        state = State::Meta;
                    }
                }
            }
        }
        ++n;
    }
     return metasum;
}

int main()
{
    //std::cout << task1("example8.txt") << std::endl;
    std::cout << task1("input8.txt") << std::endl;
    return 0;
}
