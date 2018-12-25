#include "advent.h"
#include <regex>

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

StrVec w_vec {"radiation", "bludgeoning", "fire", "slashing", "cold"};

int main()
{
    //string s{"abc 25 units 123 g"};
    
    std::regex uregex{"([0-9][0-9]*) units"};
    std::regex hregex{"([0-9][0-9]*) hit"};
    std::regex iregex{"([0-9][0-9]*) initiative"};
    std::regex dregex{"([0-9][0-9]*) ([a-z][a-z]*) damage"};
    std::regex wregex{"weak to ([a-z,]*)"};
    std::regex imregex{"immune to ([a-z,]*)"};
    
    for (const auto& s : immv)
    {
        std::smatch m, m2;
        
        if (std::regex_search(s, m, uregex))
        {
            cout << m[1] << endl;
        }


        if (std::regex_search(s, m, wregex))
        {
            cout << m[1] << endl;
        }

        if (std::regex_search(s, m, dregex))
        {
            cout << m[1] << " " << m[2] << endl;
            std::string ms{m[2]};
            for (int i = 0; i < 5; ++i)
            {
                if (ms.find(w_vec[i]) != string::npos)
                {
                    cout << i << endl;
                }
            }
        }


    }



    return 0;
}
