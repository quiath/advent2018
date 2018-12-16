#include <string>
#include <list>
#include <vector>
#include <iostream>

using std::begin;
using std::end;
using std::string;
using std::cin;
using std::cout;
using std::endl;


using List = std::list<int>;
using ListIter = std::list<int>::iterator;
using IntVec = std::vector<int>;

#define DBG(x) x  

struct Circular
{
    List lst;
    ListIter curr; 
    Circular() 
    {
        lst.insert(begin(lst), 0);
        curr = begin(lst);
    }
    auto next(ListIter it)
    {
        //cout << "next " << *it << "->"; 
        ++it;

        if (it == end(lst))
        {
            //cout << *begin(lst) << endl; 
            return begin(lst);
        }
        //cout << *it << endl; 
        return it;
    }
    auto prev(ListIter it)
    {
        if (it == begin(lst))
        {
            it = end(lst);
        }
        --it;
        return it;
    }
    
};

auto print = [](const auto& cont)
{
    for (auto sc : cont)
    {
        cout << sc << " " ;
    }
    cout << endl;
};

int task1(int nplayers, int last, int nsteps = -1)
{
    Circular c;
    IntVec scores(nplayers, 0);
    auto step{1};
    auto last_marble_worth{0};
    auto high_score{0};
    while (step <= nsteps || nsteps < 0)
    {
        if (step % 23 > 0)
        {
            auto it = c.next(c.next(c.curr));
            c.curr = c.lst.insert(it, step);
        }
        else
        {
            auto pid{(step - 1) % nplayers};
            last_marble_worth = step;
            //scores[pid] += step;
            auto it = c.curr;
            for (auto i = 0; i < 6; ++i)
            {
                it = c.prev(it);
            }
            auto rem = c.prev(it);
            /*auto rem = c.curr;
            for (auto i = 0; i < 7; ++i)
            {
                rem = c.prev(rem);
            }
            auto it =c.next(rem);*/

            last_marble_worth += *rem;
            scores[pid] += last_marble_worth; //*rem;
            high_score = std::max(high_score, scores[pid]);
            if (false) 
            {
                std::cout << step << " lmb=" << last_marble_worth << " r=" << *rem << " hs=" << high_score << endl;
            }
            c.lst.erase(rem);
            c.curr = it;
        }
        
        if (!true)
        {
            cout << step << "*" << *c.curr << " : ";
            print(c.lst);
        }

        if (last_marble_worth == last)
        {
            break;
        }

        ++step;
    }

    //print(scores);

    //return *std::max_element(begin(scores), end(scores));
    return high_score;
}

int main()
{
    //auto ret = task1(464, -1, 71730);
    auto ret = task1(464, -1, 71730);
    //auto ret = task1(9, 99999, 25);
    //auto ret = task1(9, 32, 111);
    //auto ret = task1(10, -1, 1618);
    //auto ret = task1(13, 7999);
    //auto ret = task1(21, 54718);
    //auto ret = task1(30, 5807);

    cout << "ret=" << ret << endl;
    return 0;
}
