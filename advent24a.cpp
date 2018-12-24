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

GroupVec problem
{
{ 0, 1, 2086, 11953, Cold, 46, 13, {1, 1, 1, 1, 1} },
{ 0, 2, 329, 3402, Slash, 90, 1, {1, 2, 1, 1, 1} },
{ 0, 3, 414, 7103, Radi, 170, 4, {0, 2, 1, 1, 1} },
{ 0, 4, 2205, 7118, Radi, 26, 18, {1, 1, 2, 1, 0} },
{ 0, 5, 234, 9284, Radi, 287, 12, {1, 1, 0, 2, 0} },
{ 0, 6, 6460, 10804, Slash, 15, 11 , {1, 1, 2, 1, 1} },
{ 0, 7, 79, 1935, Radi, 244, 8 , {1, 1, 1, 1, 1} },
{ 0, 8, 919, 2403, Slash, 22, 2, {1, 1, 2, 1, 1} },
{ 0, 9, 172, 1439, Slash, 69, 3, {1, 1, 0, 2, 0} },
{ 0, 10, 1721, 2792, Cold, 13, 16, {2, 1, 2, 1, 1} },
 

{ 1, 1, 1721, 29925, Radi, 34, 5, {2, 1, 1, 0, 2} },
{ 1, 2, 6351, 21460, Slash, 6, 15, {1, 1, 1, 1, 2} },
{ 1, 3, 958, 48155, Radi, 93, 7, {1, 2, 1, 1, 1} },
{ 1, 4, 288, 41029, Cold, 279, 20, {1, 0, 1, 1, 1} },
{ 1, 5, 3310, 38913, Radi, 21, 19, {1, 1, 1, 1, 1} },
{ 1, 6, 3886, 16567, Cold, 7, 9, {1, 0, 1, 1, 0} },
{ 1, 7, 39, 7078, Blud, 300, 14, {1, 1, 1, 1, 1} },
{ 1, 8, 241, 40635, Fire, 304, 6, {1, 1, 1, 1, 2} },
{ 1, 9, 7990, 7747, Radi, 1, 10, {1, 1, 0, 1, 1} },
{ 1, 10, 80, 30196, Blud, 702, 17, {1, 1, 2, 1, 1} }
};

struct Logic
{
    explicit Logic(const GroupVec g) : g_{g}, n_{g.size()} {} // copy
    GroupVec g_;
    size_t n_;

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

    IntVec order(n_ + 1, -1);

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
                << " dmg=" <<  totdamage << "<>" << maxdmg << " ";
                if (otherid >= 0) 
                {
                    cout << (gtDesired(g_[j], g_[otherid])) <<
                    " a=" << g_[j].getInit() << "<>" << g_[otherid].getInit();
                }
                cout << endl;
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
    cout << "round ended" << endl;

    g_.erase(std::remove_if(begin(g_), end(g_),
        [](const auto& a){ return a.units_ == 0;}), end(g_));
    
    cout << "erased" << endl;
    auto cnt_imm{std::count_if(begin(g_), end(g_), 
        [](const auto& a){ return a.team_ == 0;}
    )};
    cout << "counted" << endl;

    for (const auto& c : g_)
    {
        cout << "Rem:" << name[c.team_] << " " << c.id_ << 
                 " units=" << c.units_ << endl;

    }

    if (cnt_imm == 0) { return 1; }
    if (cnt_imm == g_.size()) { return -1; }
    return 0;

}

int main()
{
    //Logic logic{example};
    Logic logic{problem};
    auto ret{0};
    do 
    {
        ret = logic.iter();
    }
    while (ret == 0);
    cout << "winner=" << ret << " rem=" << logic.count() << endl;
    return 0;
}
