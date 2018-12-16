#include "advent.h"
#include <array>

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

int task1()
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

    auto it{begin(v)};
    auto cnt{0}, total{0};
    while (it != end(v))
    {
        auto it2{it};
        std::advance(it2, NR);
        Tup reg(it, it2); 
        pr(reg);


        std::advance(it, NR);
        std::advance(it2, NR);
        Tup par(it, it2); 
        pr(par);
        std::advance(it, NR);
        std::advance(it2, NR);

        Tup expected(it, it2);
        pr(expected, true);
        std::advance(it, NR);

        auto op_ok{0};
        for (const auto& p : opcodes)
        {

            auto ret{exec(reg, par, p.second)};
            if (ret == expected)
            {
                ++op_ok;
            }
        }
        if (op_ok >= 3)
        {
            ++cnt;
        }
        ++total;
    }
    cout << "cnt=" << cnt << " total=" << total << endl;

    return cnt;
}

int main()
{
    auto ret{task1()};

    cout << ret << endl;
}
