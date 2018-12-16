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
    int attack{3};
    bool active{true};
    int hitpoints{200};
    bool operator<(const Entity& other) const
    {
        return pos < other.pos;
    }
};

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

    Game(const string& fn, int elf_attack = 3)
    {
        board = read_lines(fn);
        initialize(elf_attack);
    }
    Game(const Game&) = default;
    void initialize(int elf_attack);
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
    auto elfCount() const
    { 
        return std::count_if(begin(ent), end(ent), [](const auto& entity){ return entity.species == Species::Elf;});
    };

};


void Game::initialize(int elf_attack)
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
                    ent.emplace_back(Entity{{x, y}, Species::Elf, elf_attack});
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
                }


                found = true;
                stop_at = std::min(dist + 1, stop_at);

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

    auto ecnt{elfCount()};
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
        
        // now if touching attack
        
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
                    (it->hitpoints == weakest_hp && it->pos < weakest_pos))
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

    ecnt = elfCount();

    return ecnt > 0 && ecnt < ent.size();
}

string task2(const string& fn)
{

    for (auto attack = 4; attack < 100; ++attack)
    {
        
        Game g(fn, attack);

        std::ostringstream ss;

        g.print();

        auto ecnt = g.elfCount();
        
        for (auto i = 1; i <= MAX; ++i)
        {
            cout << "Step " << i << " a=" << attack << " e=" << g.elfCount() << " totalhp=" << g.sum_hp() << endl;


            auto active = g.step();

            if (g.elfCount() < ecnt)
            {
                break;
            }

            if (!active)
            {
                g.print();
                std::ostringstream ss;
                ss << i - 1 << "*" << (g.sum_hp()) << "=" << ((i - 1) * g.sum_hp())  << endl;
                return ss.str();
            }


            //g.print();
        }
    }
    return "";
};


int main()
{
    //auto ret = task2("example15f.txt");
    auto ret = task2("input15.txt");
    cout << ret << endl;
    return 0;
}
