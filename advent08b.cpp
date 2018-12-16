#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <utility>
#include <set>



using IntVector = std::vector<int>;

using CharSet = std::set<char>;
using CharToCharSet = std::map<char, std::set<char>>;

using std::begin;
using std::end;
using std::string;
using std::cin;
using std::cout;
using std::endl;

struct TreeNode
{
    IntVector children;
    IntVector meta;
};

using TreeNodeVector = std::vector<TreeNode>;

int value(const TreeNodeVector& tree, int index)
{
    const auto& node{tree[index]};
    if (node.children.size() == 0)
    {
        return std::accumulate(begin(node.meta), end(node.meta), 0);
    }
    int ret{0};
    for (auto i : node.meta)
    {
        if (i > 0 && i <= node.children.size())
        {
            // TODO: memoize
            ret += value(tree, node.children[i - 1]);
        }
    }
    return ret;
}


int task2(string fn)
{
    std::ifstream inf(fn);

    int k {0}, n{0}, metasum{0};

    enum class State { QChild, QMeta, Meta };
    auto state{ State::QChild};

    struct StackNode
    {
        int qchild{0};
        int cread{0};
        int qmeta{0};
        int mread{0};
        int index{0};
    };

    TreeNodeVector tree;

    std::vector<StackNode> stack;

    while (inf >> k)
    {
        cout << int(state) << ":" << k << endl;
        switch (state)
        {
        case State::QChild:
            
            tree.emplace_back(TreeNode{});
            if (stack.size() > 0)
            {
                --stack.back().cread;
                tree[stack.back().index].children.emplace_back(tree.size() - 1);
            }
            stack.emplace_back(StackNode{k, k});
            stack.back().index = tree.size() - 1;

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
            tree[stack.back().index].meta.emplace_back(k);
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

    int ret{value(tree, 0)};

    return ret;
}

int main()
{
    //std::cout << task2("example8.txt") << std::endl;
    std::cout << task2("input8.txt") << std::endl;
    return 0;
}
