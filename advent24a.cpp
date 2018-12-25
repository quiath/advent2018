#include "advent.h"
#include <array>
#include <regex>
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


    auto getDamageInt(const Group& to) const
    {
        return units_ * damage_ * to.def_[attack_];
    };
    auto getDamage(const Group& to) const
    {
        //return units_ * damage_ * to.def_[attack_];
        return getDamageInt(to);
    };
    int getModDamage(const Group& to) const
    {
        return (getDamageInt(to)/ to.hitpoints_) * to.hitpoints_;
    };

    bool operator==(const Group& other) const
    {
        return team_ == other.team_ &&
        id_== other.id_ &&
        units_== other.units_ &&
        hitpoints_== other.hitpoints_ &&
        attack_== other.attack_ &&
        damage_ == other.damage_ &&
        init_ == other.init_ && def_[0] == other.def_[0]/*&&
        def_ == other.def_*/;


    }

};

using GroupVec = std::vector<Group>;

GroupVec example
{
    { 0, 1, 17, 5390, Fire, 4507, 2, {2, 2, 1, 1, 1}  },
    { 0, 2, 989, 1274, Slash, 25, 3, {1, 2, 0, 2, 1}  },
    { 1, 1, 801, 4706, Blud, 116, 1, {2, 1, 1, 1, 1}  },
    { 1, 2, 4485, 2961, Slash, 12, 4, {0, 1, 2, 1, 2}  },
};

GroupVec example1
{
    { 0, 1, 4, 2, Fire, 1, 1, {1, 0, 1, 1, 1}  },
    { 1, 1, 1, 1, Blud, 1, 4, {1, 1, 1, 1, 1}  },
    { 1, 2, 1, 1, Slash, 1, 2, {1, 1, 1, 1, 1}  },
    { 1, 3, 1, 1, Slash, 1, 3, {1, 1, 0, 1, 1}  },
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
{ 1, 4, 288, 41029, Cold, 279, 20, {2, 0, 1, 1, 1} },
{ 1, 5, 3310, 38913, Radi, 21, 19, {1, 1, 1, 1, 1} },
{ 1, 6, 3886, 16567, Cold, 7, 9, {1, 0, 1, 1, 0} },
{ 1, 7, 39, 7078, Blud, 300, 14, {1, 1, 1, 1, 1} },
{ 1, 8, 241, 40635, Fire, 304, 6, {1, 1, 1, 1, 2} },
{ 1, 9, 7990, 7747, Radi, 1, 10, {1, 1, 0, 1, 1} },
{ 1, 10, 80, 30196, Blud, 702, 17, {1, 1, 2, 1, 1} }
};

StrVec immv {
"2086 units each with 11953 hit points with an attack that does 46 cold damage at initiative 13",
"329 units each with 3402 hit points (weak to bludgeoning) with an attack that does 90 slashing damage at initiative 1",
"414 units each with 7103 hit points (weak to bludgeoning; immune to radiation) with an attack that does 170 radiation damage at initiative 4",
"2205 units each with 7118 hit points (immune to cold; weak to fire) with an attack that does 26 radiation damage at initiative 18",
"234 units each with 9284 hit points (weak to slashing; immune to cold, fire) with an attack that does 287 radiation damage at initiative 12",
"6460 units each with 10804 hit points (weak to fire) with an attack that does 15 slashing damage at initiative 11",
"79 units each with 1935 hit points with an attack that does 244 radiation damage at initiative 8",
"919 units each with 2403 hit points (weak to fire) with an attack that does 22 slashing damage at initiative 2",
"172 units each with 1439 hit points (weak to slashing; immune to cold, fire) with an attack that does 69 slashing damage at initiative 3",
"1721 units each with 2792 hit points (weak to radiation, fire) with an attack that does 13 cold damage at initiative 16    "
};

StrVec infv {
"1721 units each with 29925 hit points (weak to cold, radiation; immune to slashing) with an attack that does 34 radiation damage at initiative 5",
"6351 units each with 21460 hit points (weak to cold) with an attack that does 6 slashing damage at initiative 15",
"958 units each with 48155 hit points (weak to bludgeoning) with an attack that does 93 radiation damage at initiative 7",
"288 units each with 41029 hit points (immune to bludgeoning; weak to radiation) with an attack that does 279 cold damage at initiative 20",
"3310 units each with 38913 hit points with an attack that does 21 radiation damage at initiative 19",
"3886 units each with 16567 hit points (immune to bludgeoning, cold) with an attack that does 7 cold damage at initiative 9",
"39 units each with 7078 hit points with an attack that does 300 bludgeoning damage at initiative 14",
"241 units each with 40635 hit points (weak to cold) with an attack that does 304 fire damage at initiative 6",
"7990 units each with 7747 hit points (immune to fire) with an attack that does 1 radiation damage at initiative 10",
"80 units each with 30196 hit points (weak to fire) with an attack that does 702 bludgeoning damage at initiative 17"
};

StrVec w_vec {"radiation", "bludgeoning", "fire", "slashing", "cold"};

void parse(const StrVec& sv, GroupVec& gv, int team)
{
    std::regex uregex{"([0-9][0-9]*) units"};
    std::regex hregex{"([0-9][0-9]*) hit"};
    std::regex iregex{"initiative ([0-9][0-9]*)"};
    std::regex dregex{"([0-9][0-9]*) ([a-z][a-z]*) damage"};
    std::regex wregex{"weak to ([a-z, ]*)"};
    std::regex imregex{"immune to ([a-z, ]*)"};

    auto id{1};
    for (const auto& s : sv)
    {
        Group g;

        std::smatch m;
        if (std::regex_search(s, m, uregex))
        {
            g.units_ = std::stoi(m[1], nullptr);
        }
        else
        {
            throw string("units");
        }
        if (std::regex_search(s, m, hregex))
        {
            g.hitpoints_ = std::stoi(m[1], nullptr);
        }
        else
        {
            throw string("hit");
        }

        if (std::regex_search(s, m, iregex))
        {
            g.init_ = std::stoi(m[1], nullptr);
        }
        else
        {
            throw string("initiative");
        }

        if (std::regex_search(s, m, dregex))
        {
            g.damage_ = std::stoi(m[1], nullptr);
            std::string ms{m[2]};
            for (auto i = 0; i < w_vec.size(); ++i)
            {
                if (ms.find(w_vec[i]) != string::npos)
                {
                    g.attack_ = Attack(i);
                }
            }
        }
        else
        {
            throw string("damage");
        }
        if (std::regex_search(s, m, wregex))
        {
            std::string ms{m[1]};
            for (auto i = 0; i < w_vec.size(); ++i)
            {
                if (ms.find(w_vec[i]) != string::npos)
                {
                    g.def_[i] = 2;
                }
            }
        }
        if (std::regex_search(s, m, imregex))
        {
            std::string ms{m[1]};
            for (auto i = 0; i < w_vec.size(); ++i)
            {
                if (ms.find(w_vec[i]) != string::npos)
                {
                    g.def_[i] = 0;
                }
            }
        }
        g.team_ = team;
        g.id_ = id++;
        gv.emplace_back(g); 
    }

}

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
         if (a.team_ == b.team_)
         {
             return gtDesired(a, b);
         }
        return a.team_ < b.team_;
     });


    IntVec ato(g_.size(), -1);
    IntVec afrom(g_.size(), -1);

    IntVec order(n_ + 1, -1);

    for (auto i = 0; i < g_.size(); ++i)
    {
        auto& c{g_[i]};
        if (c.units_ == 0)
        {
            continue;
        }
        auto otherid{-1}, maxdmg{-1};
        for (auto j = 0; j < g_.size(); ++j)
        {
            if (i != j && c.team_ != g_[j].team_ && afrom[j] < 0 && g_[j].units_ > 0)
            {
                auto totdamage{c.getDamage(g_[j])};
                cout << 
                name[c.team_] << " " << c.id_ << "(" << c.getEffpower() << "/" << c.getInit() << ")" <<
                " enemyid=" << g_[j].id_ <<
                " enemyefi=" << g_[j].getEffpower() 
                << " dmg=" <<  totdamage << "<>" << maxdmg << " ";
                if (otherid >= 0) 
                {
                    cout << (gtDesired(g_[j], g_[otherid])) <<
                    " a=" << g_[j].getInit() << "<>" << g_[otherid].getInit();
                }
                cout << endl;
                if (totdamage == 0)
                {
                    continue;
                }
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
            cout << 
                name[c.team_] << " " << c.id_ << " chose " << g_[otherid].id_ << " of team " << name[g_[otherid].team_] << endl;
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

        auto totdamage{c.getDamage(gto)};

        auto fullunits{totdamage / gto.hitpoints_};
        auto killed{std::min(gto.units_, fullunits)};
        gto.units_-= killed;

        cout << "attack " << name[c.team_] << " " << c.id_ << 
                 "| " << gto.id_ << " dmg=" << totdamage << " killing " << killed << " remain=" << gto.units_ << endl;
    
    }
    cout << "round ended" << endl;

    /*g_.erase(std::remove_if(begin(g_), end(g_),
        [](const auto& a){ return a.units_ == 0;}), end(g_));
    
    cout << "erased" << endl;*/
    auto cnt_imm{std::count_if(begin(g_), end(g_), 
        [](const auto& a){ return a.team_ == 0 && a.units_ > 0;}
    )};
    auto cnt_inf{std::count_if(begin(g_), end(g_), 
        [](const auto& a){ return a.team_ == 1 && a.units_ > 0;}
    )};
    cout << "counted " << cnt_imm << "," <<cnt_inf << endl;
    //21768-22811

    for (const auto& c : g_)
    {
        cout << "Rem:" << name[c.team_] << " " << c.id_ << 
                 " units=" << c.units_ << endl;

    }

    if (cnt_imm == 0) { return 1; }
    if (cnt_inf == 0) { return -1; }
    //if (cnt_imm == g_.size()) { return -1; }
    return 0;

}

int main()
{
    //Logic logic{example};
    //Logic logic{problem};

#if 1  
    GroupVec parsed;
    parse(immv, parsed, 0);
    parse(infv, parsed, 1);
    Logic logic{parsed};

    cout << "equal " << (parsed == problem) << endl;

    auto pr_ind = [](const auto& v)
    {
        for (auto i = 0; i < v.size(); ++i)
        {
            cout << v[i] << " ";
        }
    };

    for (auto i = 0; i < parsed.size(); ++i)
    {
        if (!(parsed[i] == problem[i]))
        {
            cout << "diff " << i  << endl;
            pr_ind(parsed[i].def_);
            cout << endl;
            pr_ind(problem[i].def_);
            cout << endl;
        }
    }
#endif

    auto ret{0};
    do 
    {
        ret = logic.iter();
    }
    while (ret == 0);
    cout << "winner=" << ret << " rem=" << logic.count() << endl;
    return 0;
}
