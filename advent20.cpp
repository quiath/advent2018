#include <set>
#include <deque>
#include <fstream>
#include "advent.h"

struct Node
{
    int next_sib{-1};
    int next_end{-1};
};

using IntPair = std::pair<int, int>;
//using PairPair = std::pair<IntPair, IntPair>;
using Graph = std::map<const IntPair, string>;

std::map<char, IntPair> dir
{
    {'E', {1, 0}},
    {'W', {-1, 0}},
    {'N', {0, -1}},
    {'S', {0, 1}},
};

std::map<char, char> opp
{
    {'E', 'W'},
    {'W', 'E'},
    {'S', 'N'},
    {'N', 'S'},
};

std::map<char, int> ind
{
    {'E', 0},
    {'W', 1},
    {'N', 2},
    {'S', 3},
};

std::string SD{"EWNS"};


using PosIntSet = std::set<std::pair<IntPair, int>>;

struct Mapper
{
    Mapper() = default;
    string s;
    vector<Node> meta;
    Graph g;
    PosIntSet done;
    int minx{0}, miny{0}, maxx{0}, maxy{0};

    void rec(int from, /*string done,*/ int x = 0, int y = 0);
    void iter(int from, /*string done,*/ int x, int y);


    void parse(const string& news);
    void draw();
    int bfs(int) const;
};

void Mapper::rec(int from, /*string done,*/ int x, int y)
{
    //cout << "rec " << from << endl;

    auto item{std::make_pair(IntPair{x, y}, from)};
    auto it{done.find(item)};
    if (it != done.end())
    {
        return;
    }
    done.insert(item);

    auto i{from};
    while (i < s.length())
    {
        //cout << "i=" << i << " e=" << meta[i].next_end  << " s=" << meta[i].next_sib << "\n";
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
                rec(i + 1, /*done,*/ x, y);
                i = meta[i].next_sib;
            }
            while (i >= 0);
            if (from == 0)
            {
                cout << "Extent "  << minx << "," << miny << " : " << maxx << "," << maxy << endl;
            }

            return;
        }
        auto ch{s[i]};

        switch (ch)
        {
            case 'N':      
            case 'E':
            case 'S':
            case 'W':
                {
                    const auto& d{dir[ch]}; 
                    int nx{x + d.first};
                    int ny{y + d.second};

                    auto xy{std::make_pair(x, y)};
                    auto nxy{std::make_pair(nx, ny)};
                    if (g[nxy].length() == 0)
                    {
                        g[nxy] = SD;
                    }
                    if (g[xy].length() == 0)
                    {
                        g[xy] = SD;
                    }
                    //cout << "___" << g[xy] << endl;


                    g[xy][ind[ch]] = '+'; 
                    //cout << "set [" << ind[ch] << "] " << x << "," << y << " " << g[xy][ind[ch]] << endl;
                    //cout << "___" << g[xy] << endl;
                    g[nxy][ind[opp[ch]]] = '+';
                    //cout << "---" << g[nxy] << endl;
                    

                    x = nx;
                    y = ny;
                    maxx = std::max(x, maxx);
                    maxy = std::max(y, maxy);
                    minx = std::min(x, minx);
                    miny = std::min(y, miny);
                    
                    /*
                    done += ch;
                    */

                    //cout << done << " " << x << "," << y << endl;
                    //cout << x << "," << y << endl;
                }
            break;
            default: ;
        }

        ++i;
    }
    //cout << done << endl;
    //if (from == 0)
    {
        cout << "Extent "  << minx << "," << miny << " : " << maxx << "," << maxy << 
           " from=" << from << " size=" << g.size() << endl;
    }
   
    /*for (auto& pp : g)
    {
        IntPair p{pp.first};
        auto s{pp.second};
        cout << "* " << p.first << "," << p.second << "=" << s << endl;
    }*/

}

#if 0
void Mapper::iter(int from, /*string done,*/ int x, int y)
{
    //cout << "rec " << from << endl;

    /*string init;
    for (auto i = 0; i < 100; ++i)
    {
        init += "EWNS:";
    }
    StrVec v(100, init);*/

    struct Item
    {
        IntPair xy{0, 0};
        int index{0};
    };
    std::vector<Item> stack;

    auto i{from};
    while (i < s.length())
    {
        if (i == s.length() - 1 && stack.size() > 0)
        {
            x = stack.back().xy.first;
            y = stack.back().xy.second;
            i = stack.back().index;
            stack.pop_back();
        }

        //cout << "i=" << i << " e=" << meta[i].next_end  << " s=" << meta[i].next_sib << "\n";
        if (meta[i].next_end >= 0)
        {
            i = meta[i].next_end;
            continue;
        }

        auto j{meta[i].next_sib};
        if (j >= 0)
        {
            stack.emplace_back(Item{IntPair{x, y}, i + 1});
            i = meta[i].next_sib;
            continue;
        }
        auto ch{s[i]};

        switch (ch)
        {
            case 'N':      
            case 'E':
            case 'S':
            case 'W':
                {
                    const auto& d{dir[ch]}; 
                    int nx{x + d.first};
                    int ny{y + d.second};

                    auto xy{std::make_pair(x, y)};
                    auto nxy{std::make_pair(nx, ny)};
                    if (g[nxy].length() == 0)
                    {
                        g[nxy] = SD;
                    }
                    if (g[xy].length() == 0)
                    {
                        g[xy] = SD;
                    }
                    //cout << "___" << g[xy] << endl;


                    g[xy][ind[ch]] = '+'; 
                    //cout << "set [" << ind[ch] << "] " << x << "," << y << " " << g[xy][ind[ch]] << endl;
                    //cout << "___" << g[xy] << endl;
                    g[nxy][ind[opp[ch]]] = '+';
                    //cout << "---" << g[nxy] << endl;
                    

                    x = nx;
                    y = ny;
                    maxx = std::max(x, maxx);
                    maxy = std::max(y, maxy);
                    minx = std::min(x, minx);
                    miny = std::min(y, miny);
                    
                    /*
                    done += ch;
                    */

                    //cout << done << " " << x << "," << y << endl;
                    //cout << x << "," << y << endl;
                }
            break;
            default: ;
        }

        ++i;
    }
    //cout << done << endl;
    //if (from == 0)
    {
        cout << "Extent "  << minx << "," << miny << " : " << maxx << "," << maxy << 
           " from=" << from << " size=" << g.size() << endl;
    }
   
    /*for (auto& pp : g)
    {
        IntPair p{pp.first};
        auto s{pp.second};
        cout << "* " << p.first << "," << p.second << "=" << s << endl;
    }*/

}
#endif


void Mapper::parse(const string& news)
{
    s = news;
    meta.clear();
    meta.resize(s.length());

    minx = miny = maxx = maxy = 0;

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
    cout << "length=" << s.length() << endl;

}

int Mapper::bfs(int threshold) const
{
    std::set<IntPair> visited{{IntPair{0, 0}}};
    std::map<IntPair, int> distance{{IntPair{0, 0}, 0}};
    std::deque<IntPair> q{IntPair{0, 0}};

    int maxd{0};
    int cnt{0};

    while (q.size())
    {
        auto p{q.front()};
        auto it{g.find(p)};
        if (it == end(g))
        {
            throw string("unknown!");
        }
        const auto& s{it->second};
        int dist{distance[p]};
        //cout << p.first << "," << p.second << "=" << s << " d=" << dist << endl;
        if (dist > maxd)
        {
            maxd = dist;
        }
        for (auto i = 0; i < 4; ++i)
        {
            if (s[i] == '+')
            {
                const auto& d{dir[SD[i]]}; 
                int nx{p.first + d.first};
                int ny{p.second + d.second};
                IntPair nxy{nx, ny};
                auto it{visited.find(nxy)};
                if (it != end(visited))
                {
                    continue;
                }
                visited.insert(nxy);
                distance[nxy] = dist + 1;
                q.emplace_back(nxy);
                if (dist + 1 >= threshold)
                {
                    ++cnt;
                }
            }

        }
        q.pop_front();

    }
    cout << "cnt over " << threshold << ": " << cnt << endl;

    return maxd;

}


/*void Mapper::draw()

{
    

}*/

int test()
{
    //string s{"^NNN|W|SS|EE$"};
    //string s{"^NNNEE$"};
    //string s{"^NN(EE)SSS$"};
    //string s{"^NN(EE|)SSS$"};
    //string s{"^(EE|WW)(SSS|NNN)$"};
    //string s{"^(EE(WW|SSS|(N|NN)))$"};

#if 0
    string s{"^ENWWW(NEEE|SSE(EE|N))$"};
    //string s{"^ESSWWN(E|NNENN(EESS(WNSE|)SSS|WWWSSSSE(SW|NNNE)))$"};
    //string s{"^WSSEESWWWNW(S|NENNEEEENN(ESSSSW(NWSW|SSEN)|WSWWN(E|WWS(E|SS))))$"};
    //string s{"^(NN(EE(SS|)|)|)$"};
    //string s{"^((NN|SS)|(EE|WW))|((NN|SS)|(EE|WW))#"};
#else
    std::ifstream ifs("input20.txt");
    //std::ifstream ifs("i20.txt");
    string s((std::istreambuf_iterator<char>(ifs)),
                std::istreambuf_iterator<char>());
#endif    
    
    Mapper mapper;
    mapper.parse(s);
    mapper.rec(0/*, ""*/);
    
    //mapper.iter(0, 0, 0);
    auto d{mapper.bfs(1000)};
    cout << "bfs=" << d << endl;

#if 0    
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
#endif
    return 0;
}

int main()
{
    test();
    return 0;
}
