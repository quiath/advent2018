#include "advent.h"
#include <array>

struct Coord
{
    int x{0};
    int y{0};
    void add(const Coord& other) { x += other.x; y += other.y;};
    bool operator<(const Coord& other) const 
    { 
        if (y == other.y) { return x < other.x; }
        return y < other.y;
    }

    bool operator==(const Coord& other) const 
    {
        return x == other.x && y == other.y;
    }
};

enum Dir { R=0, D, L, U, NDir };
std::array<Coord, 4> delta{{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};

struct Cart
{
    Coord pos{0, 0};
    Dir dir{R};
    Dir next_cross{L};
    bool active{true};
    bool operator<(const Cart& other) const
    {
        return pos < other.pos;
    }
};



using CartVec = vector<Cart>;

void initialize(StrVec& v, CartVec& c, StrVec& hit)
{
    auto y{0};
    for (auto& s : v)
    {
        hit.emplace_back(string(s.length(), ' '));
        auto x{0};
        for (auto& ch : s)
        {
            switch (ch)
            {
                case '>':
                    ch = '-';
                    c.emplace_back(Cart{{x, y}});
                    hit[y][x] = '*';
                    break;
                case '<':
                    ch = '-';
                    c.emplace_back(Cart{{x, y}, L});
                    hit[y][x] = '*';
                    break;
                case 'v':
                    ch = '|';
                    c.emplace_back(Cart{{x, y}, D});
                    hit[y][x] = '*';
                    break;
                case '^':
                    ch = '|';
                    c.emplace_back(Cart{{x, y}, U});
                    hit[y][x] = '*';
                    break;
                default:
                    ;

            }

            ++x;
        }

        ++y;
    }
}

string task2(const string& fn)
{
    auto v{read_lines(fn)};
    StrVec hit;
    std::ostringstream ss;

    CartVec carts;
    initialize(v, carts, hit);
    
    while (true)
    {
        std::sort(begin(carts), end(carts));
        
        for (auto& c : carts)
        {
            //ss << c.pos.x << "," << c.pos.y << endl;
            if (!c.active)
            {
                continue;
            }

            Coord npos{c.pos};
            npos.add(delta[c.dir]);
            if (hit[npos.y][npos.x] != ' ')
            {
                //ss << npos.x << "," << npos.y << endl;
                for (auto& kill : carts)
                {
                    if (kill.pos == npos)
                    {
                        kill.active = false;
                    }
                }
                c.active = false;
                hit[npos.y][npos.x] = ' ';
                hit[c.pos.y][c.pos.x] = ' ';
                continue;
            }
            hit[npos.y][npos.x] = '*';
            hit[c.pos.y][c.pos.x] = ' ';
            c.pos = npos;

            auto under{v[c.pos.y][c.pos.x]};
            switch (under)
            {
                case '+':
                    if (c.next_cross == L)
                    {
                        c.dir =  static_cast<Dir>((c.dir + 3) % 4);
                        c.next_cross = U;
                    } 
                    else if (c.next_cross == U)
                    {
                        c.next_cross = R;
                    }
                    else
                    {
                        c.dir = static_cast<Dir>((c.dir + 1) % 4);
                        c.next_cross = L;
                    }
                    break;
                case '/':
                case '\\':
                    {
                        auto first_case{under == '/'};
                        if ((delta[c.dir].x == 0) == first_case)
                        {
                            c.dir = static_cast<Dir>((c.dir + 1) % 4);
                        }
                        else
                        {   
                            c.dir = static_cast<Dir>((c.dir + 3) % 4);
                        }
                    }
                    break;
                default:
                   ; 
            }
        }

        carts.erase(std::remove_if(
                        begin(carts), 
                        end(carts), 
                        [](const auto& e){ return !e.active; }), 
                    end(carts));
        if (carts.size() == 1)
        {
            auto& c{carts.front()};
            ss << c.pos.x << "," << c.pos.y << endl;
            return ss.str();
        }
        
        if (false)
        {
            for (auto y = 0; y < v.size(); y++)
            {
                for (auto x = 0; x < v[y].length(); x++)
                {
                    if (hit[y][x] != ' ')
                    {
                        cout << '*';
                    }
                    else
                    {
                        cout << v[y][x];
                    }
                }
                cout << '\n';
            }
            cout << '\n';
        }
        //return ss.str();
    }


    ss << std::to_string(v.size()) << endl;
    ss << std::to_string(carts.size()) << endl;
    return ss.str();
};

int main()
{
    auto ret = task2("input13.txt");
    //auto ret = task1("example13.txt");
    cout << ret << endl;
    return 0;
}
