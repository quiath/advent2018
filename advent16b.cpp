#include "advent.h"
#include <array>
#include <set>

using StrSet = std::set<string>;
enum class Mode
{
    Imm = 0, Reg = 1
};

/*
struct Par
{
    int val{0};
    Mode mode{Reg};
};*/

static const int NR{4};
//using Tup = std::array<int, NR>;
using Tup = IntVec;

struct Opcode
{
    char op{'+'};
    Mode amode{Mode::Reg};
    Mode bmode{Mode::Reg};
};

using namespace std::string_literals;

using Str2Op = std::map<const string, Opcode>;

Str2Op opcodes = {
    {"addr"s, Opcode{'+'}},

    {"addi"s, Opcode{'+', Mode::Reg, Mode::Imm}},
    {"mulr"s, Opcode{'*'}},
    {"muli"s, Opcode{'*', Mode::Reg, Mode::Imm}},
    {"banr"s, Opcode{'&'}},
    {"bani"s, Opcode{'&', Mode::Reg, Mode::Imm}},
    {"borr"s, Opcode{'|'}},
    {"bori"s, Opcode{'|', Mode::Reg, Mode::Imm}},
    {"setr"s, Opcode{'%'}},
    {"seti"s, Opcode{'%', Mode::Imm}},
    {"gtir"s, Opcode{'>', Mode::Imm, Mode::Reg}},
    {"gtri"s, Opcode{'>', Mode::Reg, Mode::Imm}},
    {"gtrr"s, Opcode{'>'}},
    {"eqir"s, Opcode{'=', Mode::Imm, Mode::Reg}},
    {"eqri"s, Opcode{'=', Mode::Reg, Mode::Imm}},
    {"eqrr"s, Opcode{'='}}   
};
/*
struct Machine
{
    Tup reg{0, 0, 0, 0};

    void exec()
    {
    }
};
*/

Tup exec(const Tup& reg0, const Tup& par, Opcode opcode)
{
    Tup reg{reg0};

    auto va{par[1]};
    auto vb{par[2]};
    auto outregi{par[3]};
    auto erra{false}, errb{false};
    if (opcode.amode == Mode::Reg)
    {
        if (va >= 0 && va < int(reg.size()))
        {
            va = reg[va];
        }
        else
        {
            erra = true;
        }
    }
    if (opcode.bmode == Mode::Reg)
    {
        if (vb >= 0 && vb < int(reg.size()))
        {
            vb = reg[vb];
        }
        else
        {
            errb = true;
        }
    }
    
    switch (opcode.op)
    {
        case '+':
            reg[outregi] = va + vb;
        break;
        case '*':
            reg[outregi] = va * vb;
        break;
        case '&':
            reg[outregi] = va & vb;
        break;
        case '|':
            reg[outregi] = va | vb;
        break;
        case '%':
            reg[outregi] = va;
            errb = false;
        break;
        case '>':
            reg[outregi] = int(va > vb);
        break;
        case '=':
            reg[outregi] = int(va == vb);
        break;
        default:
        ;
    };

    if (erra || errb)
    {
        return { -1000, -1000, -1000, -1000 };
    }

    return reg;
}

template<typename T>
void pr(const T& v, bool nl = false)
{
    for (const auto& x : v)
    {
        cout << x << " ";
    }
    if (nl)
    {
        cout << endl;
    }
}

IntVec getNaturalNumbers(const string& fn)
{
    std::ifstream inf(fn);
    
    IntVec v((std::istream_iterator<int>(inf)), std::istream_iterator<int>());
    return v;
}

StrVec solve(std::vector<StrSet>& v)
{
    auto front = [](const auto& c) { return *begin(c);};

    StrSet done;

    StrVec mapping(16, "");

    while (done.size() < v.size())
    {
        auto j{0};
        for (const auto& st : v)
        {
            if (st.size() == 1 && done.find(*st.cbegin()) == end(done))
            {
                done.insert(*st.cbegin());
                mapping[j] = *st.cbegin();
                for (auto i = 0; i < v.size(); ++i)
                {
                    if (i != j)
                    {
                        v[i].erase(*st.cbegin());
                    }
                }
            }

            cout << j << ":";
            pr(st);
            cout << '\n';


            ++j;
        }
        
    }

    return mapping;
    
}

int task2()
{
    Tup reg{3, 2, 1, 1};
    Tup par{9, 2, 1, 2};

    pr(reg, true);
    pr(par, true);

    //auto ret{exec(reg, par, opcodes["mulr"])};
    //auto ret{exec(reg, par, opcodes["addi"])};
    auto ret{exec(reg, par, opcodes["seti"])};

    

    pr(ret, true);

    cout << "***\n\n";

    auto v{getNaturalNumbers("input16a.txt")};
    //pr(v, true);
    cout << "v.size=" << v.size() << endl;

    StrSet all_op;
    for (const auto& p : opcodes)
    {
        all_op.insert(p.first);
    }

    std::vector<StrSet> possible(16, all_op);


    auto it{begin(v)};
    auto cnt{0}, total{0};
    while (it != end(v))
    {
        auto it2{it};
        std::advance(it2, NR);
        Tup reg(it, it2); 
        //pr(reg);


        std::advance(it, NR);
        std::advance(it2, NR);
        Tup par(it, it2); 
        //pr(par);
        std::advance(it, NR);
        std::advance(it2, NR);

        Tup expected(it, it2);
        //pr(expected, true);
        std::advance(it, NR);

        auto op_ok{0};
        for (const auto& p : opcodes)
        {

            auto ret{exec(reg, par, p.second)};
            if (ret == expected)
            {
                ++op_ok;
            }
            else
            {
                possible[par[0]].erase(p.first);
            }
        }
        if (op_ok >= 3)
        {
            ++cnt;
        }
        ++total;
    }
    cout << "cnt=" << cnt << " total=" << total << endl;

    auto mapping = solve(possible);

    //pr(possible, true);
    
    auto j{0};
    for (const auto& s : mapping)
    {
        cout << j++ << ":" << s;
        
        cout << '\n';
    }

    auto prog{getNaturalNumbers("input16b.txt")};
    cout << "plen=" << prog.size() << ", " << prog.size() / 4 << endl;
    
    {
        Tup reg{0, 0, 0, 0};

        auto it{begin(prog)};
        
        auto step{0};
        while (it != end(prog))
        {
            cout << "step " << step << endl;
            auto it2{it};
            std::advance(it2, NR);

            Tup par(it, it2); 

            auto ret{exec(reg, par, opcodes[mapping[par[0]]] )};

            std::advance(it, NR);
            reg = ret;
            ++step;

        }
        return reg[0];
    }

    return cnt;
}

int main()
{
    auto ret{task2()};

    cout << ret << endl;
}
