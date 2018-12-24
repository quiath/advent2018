#include "advent.h"
#include <array>
//int const BLU = 0;

int const N{5};
enum /*class*/ Attack { Radi = 0, Blud, Fire, Slash, Cold };

StrVec name{"Imm", "Inf"};

using Tup = std::array<int, 5>;

int const Types{2};

struct Group
{
    int team_{0};
    int id_{0};
    int units_{0};
    int hitpoints_{0};
    Attack attack_{Radi};
    int damage_{0};
    int init_{0};
    Tup def_{1, 1, 1, 1, 1};

    int getEffpower() const { return units_ * damage_; }
    int getInit() const { return init_; }
};

using GroupVec = std::vector<Group>;

GroupVec example
{
    { 0, 1, 17, 5390, Fire, 4507, 2, {2, 2, 1, 1, 1}  },
    { 0, 2, 989, 1274, Slash, 25, 3, {1, 2, 0, 2, 1}  },
    { 1, 1, 801, 4706, Blud, 116, 1, {2, 1, 1, 1, 1}  },
    { 1, 2, 4485, 2961, Slash, 12, 4, {0, 1, 2, 1, 2}  },
};



struct Logic
{
    explicit Logic(const GroupVec g) : g_{g} {} // copy
    GroupVec g_;

    int iter();

    int count()
    {
        auto ret{0};
        for (const auto& c : g_)
        {
            ret += c.units_;
        }
        return ret;
    }
};

auto gtDesired = [](const auto& a, const auto& b)
{
    if (a.getEffpower() > b.getEffpower()) { return true; }
    if (a.getEffpower() < b.getEffpower()) { return false; }

    return a.getInit() > b.getInit();
    //if (a.getInit() > b.getInit()) { return true; }
    //if (a.getInit() <= b.getInit()) { return false; }
};

int Logic::iter()
{
    std::sort(begin(g_), end(g_), 
     [](auto a, auto b)
     {
         if (a.getEffpower() == b.getEffpower())
         {
             return a.getInit() > b.getInit();
         }
         return a.getEffpower() > b.getEffpower();
     });


    IntVec ato(g_.size(), -1);
    IntVec afrom(g_.size(), -1);

    IntVec order(g_.size() + 1, -1);

    for (auto i = 0; i < g_.size(); ++i)
    {
        auto& c{g_[i]};
        auto otherid{-1}, maxdmg{-1};
        for (auto j = 0; j < g_.size(); ++j)
        {
            if (i != j && c.team_ != g_[j].team_ && afrom[j] < 0)
            {
                auto totdamage{g_[j].def_[c.attack_] * c.damage_ * c.units_};
                cout << 
                name[c.team_] << " " << c.id_ << 
                " enemyid=" << g_[j].id_ <<
                " enemyefi=" << g_[j].getEffpower() 
                << " dmg=" <<  totdamage << "<>" << maxdmg << " " << 
                (gtDesired(g_[j], g_[otherid])) <<
                " a=" << g_[j].getInit() << "<>" << g_[otherid].getInit() << endl;
                if (totdamage > maxdmg ||
                    (totdamage == maxdmg && gtDesired(g_[j], g_[otherid])
                    ))
                {
                    maxdmg = totdamage;
                    otherid = j;

                }

                cout << name[c.team_] << " " << c.id_ << 
                 "| " << g_[j].id_ << " dmg=" << totdamage << endl;
                    
            }
            
        }
        if (otherid >= 0 && maxdmg > 0)
        {
            ato[i] = otherid;
            afrom[otherid] = i;
            order[c.getInit()] = i;
        }

    }
    
    for (auto it = std::rbegin(order); it != std::rend(order); ++it)
    {
        auto ind{*it};
        if (ind < 0)
        {
            continue;
        }
        auto& c{g_[ind]};
        auto& gto{g_[ato[ind]]};
        if (c.units_ == 0 || gto.units_ == 0)
        {
            continue;
        }

        auto totdamage{gto.def_[c.attack_] * c.damage_ * c.units_};

        auto fullunits{totdamage / gto.hitpoints_};
        auto killed{std::min(gto.units_, fullunits)};
        gto.units_-= killed;

        cout << "attack " << name[c.team_] << " " << c.id_ << 
                 "| " << gto.id_ << " dmg=" << totdamage << " killing " << killed << " remain=" << gto.units_ << endl;
    
    }

    g_.erase(std::remove_if(begin(g_), end(g_),
        [](const auto& a){ return a.units_ == 0;}), end(g_));
    auto cnt_imm{std::count_if(begin(g_), end(g_), 
        [](const auto& a){ return a.team_ == 0;}
    )};

    if (cnt_imm == 0) { return 1; }
    if (cnt_imm == g_.size()) { return -1; }
    return 0;

}

int main()
{
    Logic logic{example};
    auto ret{0};
    do 
    {
        ret = logic.iter();
    }
    while (ret == 0);
    cout << "winner=" << ret << " rem=" << logic.count() << endl;
    return 0;
}
