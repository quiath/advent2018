#include "advent.h"
#include <array>
#include <deque>


static const int MAX = 1000000;
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

Coord operator+(const Coord& a, const Coord& b)
{
    return Coord{a.x + b.x, a.y + b.y};
}

std::array<Coord, 4> delta{{{0, -1}, {-1, 0}, {1, 0}, {0, 1}}};

enum class Species { Elf = 0, Goblin };

struct Entity
{
    Coord pos{0, 0};
    Species species{Species::Elf};
    bool active{true};
    int hitpoints{200};
    int attack{3};
    bool operator<(const Entity& other) const
    {
        return pos < other.pos;
    }
};
/*
template <typename T>
struct Arr2D
{
    Arr2D(int w, int h, )

};*/

struct Grid
{
    Grid(int w, int h, int val = 0) : w_(w), h_(h) 
    {
        v_.resize(w * h, val);
    }
    int& operator()(int x, int y)
    {
        return v_[x + w_ * y];
    }
    const int& operator()(int x, int y) const
    {
        return v_[x + w_ * y];
    }


    void fill(int val)
    {
        std::fill(begin(v_), end(v_), val);
    }
    int w_;
    int h_;
    IntVec v_;
};




struct Game
{
    StrVec board;
    StrVec layerp;
    std::vector<Entity> ent;
    int w{0};
    int h{0};

    Game(const string& fn)
    {
        board = read_lines(fn);
        initialize();
    }
    void initialize();
    bool step();

    static char to_char(Species sp)  { return (sp == Species::Goblin) ? 'G' : 'E';};
    bool isTarget(int x, int y, Species searched) const
    {
        return layerp[y][x] == to_char(searched);
    };
    bool isOpen(int x, int y) const
    {
        return board[y][x] == '.' && layerp[y][x] == ' ';
    }

    bool bfs(Grid& grd, Coord start, bool find_closest, Species enemy, Coord& target);


    Coord getFirst(const Grid& grd, Coord pos, int wanted) const
    {
        for (auto i = 0; i < delta.size(); ++i)
        {
            auto np{pos + delta[i]};
            if (grd(np.x, np.y) == wanted)
            {
                return np;
            }
        }
        return Coord{MAX, MAX};
    }
    Coord getFirst(Coord pos, Species wanted) const
    {
        for (auto i = 0; i < delta.size(); ++i)
        {
            auto np{pos + delta[i]};
            if (to_char(wanted) == layerp[np.y][np.x])
            {
                return np;
            }
        }
        return Coord{MAX, MAX};
    }

    void print()
    {
        for (auto y = 0; y < h; ++y)
        {
            const auto& s{board[y]};
            const auto& t{layerp[y]};
            for (auto x = 0; x < w; ++x)
            {
                if (t[x] != ' ')
                {
                    cout << t[x];
                }
                else
                {
                    cout << s[x];
                }
            }
            cout << '\n';
        }
    }

    int sum_hp() const
    {
        auto hp{0};
        for (const auto& e : ent)
        {
            hp += e.hitpoints;
        }
        return hp;
    }
};


void Game::initialize()
{
    auto y{0};
    for (auto& s : board)
    {
        layerp.emplace_back(string(s.length(), ' '));
        auto x{0};
        for (auto& ch : s)
        {
            switch (ch)
            {
                case 'E':
                    ch = '.';
                    ent.emplace_back(Entity{{x, y}});
                    layerp[y][x] = 'E';
                    break;
                case 'G':
                    ch = '.';
                    ent.emplace_back(Entity{{x, y}, Species::Goblin});
                    layerp[y][x] = 'G';
                    break;
                default:
                    ;

            }

            ++x;
        }

        ++y;
        w = std::max(int(s.length()), w);
        h = std::max(y, h);
    }
}


bool Game::bfs(Grid& grd, Coord start, bool find_closest, Species enemy, Coord& target)
{
    grd.fill(MAX);
    grd(start.x, start.y) = 0;

    std::deque<Coord> q{Coord{start.x, start.y}};
    
    auto stop_at{MAX};
    auto found{false};

    //cout << endl;
    //cout << (find_closest ? "clo " : "back " ) << start.x << "," << start.y << " " << 1 - int(enemy) << endl;
    
    while (q.size())
    {
        auto p{q.front()};
        q.pop_front();
        auto dist{grd(p.x, p.y)};
        
        if (grd(p.x, p.y) >= stop_at)
        {
            return true;
        }

        for (auto i = 0; i < delta.size(); ++i)
        {
            auto np{p + delta[i]};
            if (find_closest && isTarget(np.x, np.y, enemy))
            {

                if (p < target)
                {
                    target = p;
                    //cout << "accepted adjacent  to enemy " << target.x << "," << target.y << " d=" << dist << endl;
                }
                /*
                if (grd(np.x, np.y) < dist + 1) // not possible
                {
                    continue;
                }
                if (grd(np.x, np.y) == dist + 1)
                {

                    cout << "f d=" << dist + 1 << " " << np.x << "," << np.y << " " << int(enemy) << endl;
                    if (!(np < target))
                    {
                        continue;
                    }
                }*/

                //grd(np.x, np.y) = dist + 1;
                found = true;
                stop_at = std::min(dist + 1, stop_at);

                // 
            }
            else if (!find_closest && target == np)
            {
                grd(np.x, np.y) = std::min(grd(np.x, np.y), dist + 1);
                stop_at = std::min(dist + 1, stop_at);
            }
            else if (isOpen(np.x, np.y) && grd(np.x, np.y) == MAX)
            {
                grd(np.x, np.y) = dist + 1;
                q.emplace_back(np);
            }

        }
    }
    return found;
}


bool Game::step()
{
    std::sort(begin(ent), end(ent));

    auto ecnt{std::count_if(begin(ent), end(ent), [](const auto& entity){ return entity.species == Species::Elf;})};

    if (ecnt == 0 || ecnt == ent.size())
    {
        return false;
    }

    Grid grd(w, h, MAX);

    for (auto& plr : ent)
    {

        if (!plr.active)
        {
            continue;
        }
        Species enemy_spec{plr.species == Species::Elf ? Species::Goblin : Species::Elf};

        Coord adj_enemy_pos{MAX, MAX};
        auto found = bfs(grd, plr.pos, true, enemy_spec, adj_enemy_pos);
        if (!found)
        {
            continue;
        }

        auto dist{grd(adj_enemy_pos.x, adj_enemy_pos.y)};
        if (dist > 0)
        {
            Coord back_pos{plr.pos};
            grd.fill(MAX);
            found = bfs(grd, adj_enemy_pos, false, plr.species, back_pos); // must be found
            
            auto move_to{getFirst(grd, plr.pos, dist - 1)};

            layerp[move_to.y][move_to.x] = layerp[plr.pos.y][plr.pos.x];
            layerp[plr.pos.y][plr.pos.x] = ' ';
            plr.pos = move_to;

            dist -= 1;
        }
        //if (dist > 0)
        //{
        //    continue;
        //}
        
        // now if touching attack
        //adj_enemy_pos = getFirst(plr.pos, enemy_spec);
        
        Coord weakest_pos{MAX, MAX};
        auto weakest_hp{MAX};
        auto weakest_it{begin(ent)};
        for (auto i = 0; i < delta.size(); ++i)
        {
            auto np{plr.pos + delta[i]};
            if (to_char(enemy_spec) == layerp[np.y][np.x])
            {
                auto it = std::find_if(
                    begin(ent),
                    end(ent),
                    [&np](const auto e) {return e.pos == np;});
                if (it == end(ent))
                {
                    // alert 
                    continue;
                }
                if (it->hitpoints < weakest_hp ||
                    it->hitpoints == weakest_hp && it->pos < weakest_pos)
                {
                    weakest_it = it;
                    weakest_pos = it->pos;
                    weakest_hp = it->hitpoints;
                }
            }
        }
        if (weakest_hp == MAX)
        {
            continue;
        }
        weakest_it->hitpoints -= plr.attack;
        if (weakest_it->hitpoints <= 0)
        {
            weakest_it->active = false;
            layerp[weakest_pos.y][weakest_pos.x] = ' ';
        }
        

    }
    
    ent.erase(std::remove_if(
                        begin(ent), 
                        end(ent), 
                        [](const auto& e){ return !e.active; }), 
                    end(ent));

    ecnt = std::count_if(begin(ent), end(ent), [](const auto& entity){ return entity.species == Species::Elf;});

    return ecnt > 0 && ecnt < ent.size();
}

string task1(const string& fn)
{
    Game g(fn);

    std::ostringstream ss;

    g.print();
    
    for (auto i = 1; i <= MAX; ++i)
    {
        cout << "Step " << i << " totalhp=" << g.sum_hp() << endl;
        auto active = g.step();

        // TODO: sometimes there is one too many round!!!!!
        if (!active)
        {
            g.print();
            ss << i - 1 << "*" << (g.sum_hp()) << "=" << ((i - 1) * g.sum_hp())  << endl;
            return ss.str();
        }


        //g.print();
    }

#if 0    
    while (true)
    {

      
        std::sort(begin(carts), end(carts));


        for (auto& c : carts)
        {
            //ss << c.pos.x << "," << c.pos.y << endl;
            Coord npos{c.pos};
            npos.add(delta[c.dir]);
            if (hit[npos.y][npos.x] != ' ')
            {
                ss << npos.x << "," << npos.y << endl;
                return ss.str();
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
#endif

    ss << std::to_string(g.w) << endl;
    ss << std::to_string(g.h) << endl;
    return ss.str();
};

int main()
{
    auto ret = task1("example15d.txt");
    //auto ret = task1("input15.txt");
    cout << ret << endl;
    return 0;
}
