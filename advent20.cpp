#include "advent.h"

struct Node
{
    int next_sib{-1};
    int next_end{-1};
};

void rec(const string& s, const vector<Node>& meta, int from, string done)
{
    cout << "rec " << from << endl;

    auto i{from};
    while (i < s.length())
    {
        if (meta[i].next_end >= 0)
        {
            i = meta[i].next_end;
            continue;
        }

        auto j{meta[i].next_sib};
        if (j >= 0)
        {
            do
            {
                rec(s, meta, i + 1, done);
                i = meta[i].next_sib;
            }
            while (i >= 0);
            return;
        }
        done += s[i];

        ++i;
    }
    cout << done << endl;

}

int test()
{
    string s{"^NNN|W|SS|EE$"};

    vector<Node> meta(s.length());
    vector<Node> stack;

    int i{int(s.length()) - 1};
    int epos{i};
    int spos{-1};

    while (i >= 0)
    {
        if (s[i] == '^')
        {
            if (spos != -1)
            {
                meta[i].next_sib = spos;
            }
        } 
        else if (s[i] == '|')
        {
            if (spos != -1)
            {
                meta[i].next_sib = spos;
            }
            meta[i].next_end = epos;
            spos = i;
        }
        --i;
    }

    rec(s, meta, 0, "");

    return 0;
}

int main()
{
    test();
    return 0;
}
