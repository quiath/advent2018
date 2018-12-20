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
        
        switch (s[i])
        {
            case 'N':
            case 'E':
            case 'S':
            case 'W':
                done += s[i];
            break;
            default: ;
        }

        ++i;
    }
    cout << done << endl;

}

int test()
{
    //string s{"^NNN|W|SS|EE$"};
    //string s{"^NNNEE$"};
    //string s{"^NN(EE)SSS$"};
    //string s{"^NN(EE|)SSS$"};
    //string s{"^(EE|WW)(SSS|NNN)$"};
    //string s{"^(EE(WW|SSS|(N|NN)))$"};
    string s{"^ENWWW(NEEE|SSE(EE|N))$"};

    vector<Node> meta(s.length());

    int i{int(s.length()) - 1};
    vector<Node> stack{Node{-1, i}};

    auto spos = [&stack]()->int&{ return stack.back().next_sib; };
    auto epos = [&stack]()->int&{ return stack.back().next_end; };

    while (i >= 0)
    {
        if (s[i] == '^')
        {
            if (spos() != -1)
            {
                meta[i].next_sib = spos();
            }
        } 
        else if (s[i] == '|')
        {
            if (spos() != -1)
            {
                meta[i].next_sib = spos();
            }
            meta[i].next_end = epos();
            spos() = i;
        }
        else if (s[i] == ')')
        {
            stack.emplace_back(Node{-1, i});
        }
        else if (s[i] == '(')
        {
            if (spos() != -1)
            {
                meta[i].next_sib = spos();
            }
            
            stack.pop_back();
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
