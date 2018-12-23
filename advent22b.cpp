#include <tuple>
#include <set>
#include <map>
#include <queue>
#include "advent.h"

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
    int getW() const { return w_; };
    int getH() const { return h_; };

    void fill(int val)
    {
        std::fill(begin(v_), end(v_), val);
    }
    int w_;
    int h_;
    IntVec v_;
};


Grid getMap(int depth, int tx, int ty)
{
    auto w{tx * 2 + 5}, h{ty * 2 + 5};
    Grid g(w, h);
    Grid gmap(w, h);
    
    auto MOD{20183};
    auto MY0{16807};
    auto MX0{48271};

    auto vel = [&](int value)
    {
        return (value + depth) % MOD; 
    };

    auto rl{vel(0) % 3};
    gmap(0, 0) = vel(0) % 3;

    for (auto x = 0; x < w; ++x)
    {
        g(x, 0) = vel(x * MY0);
        //cout << x << "," << 0 << ":" << g(x, 0) << endl;
        rl += g(x, 0) % 3;
        gmap(x, 0) = g(x, 0) % 3;
    }
    for (auto y = 0; y < h; ++y)
    {
        g(0, y) = vel(y * MX0);
        //cout << 0 << "," << y << ":" << g(0, y) << endl;
        rl += g(0, y) % 3;
        gmap(0, y) = g(0, y) % 3;
    }
    for (auto y = 1; y < h; ++y)
    {
        for (auto x = 1; x < w; ++x)
        {
            auto gimod{0};
            if (x != tx || y != ty)
            {
                gimod = g(x - 1, y) * g(x, y - 1);
            }
            g(x, y) = vel(gimod);

            //cout << x << "," << y << ":" << g(x, y) << ";" << g(x,y) % 3 << endl;
            rl += g(x, y) % 3;
            gmap(x, y) = g(x, y) % 3;
        }
    }
    return gmap;

}

void printMap(const Grid& gmap)
{
    string conv{".=|"};
    for (auto y = 0; y < gmap.getH(); ++y)
    {
        for (auto x = 0; x < gmap.getW(); ++x)
        {
            cout << conv[gmap(x, y)];
        }
        cout << '\n';
    }
}

const int Neither{0};
const int Torch{1};
const int Gear{2};

using Node = std::tuple<int, int, int>;
using NodeSet = std::set<Node>;
using Node2Int = std::map<const Node, int>;
using DistNode = std::pair<int, Node>;
using PQ = std::priority_queue<DistNode, std::vector<DistNode>, std::greater<DistNode>>;
using IntPair = std::pair<int, int>;
using std::get;

IntPair dir[4]{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

struct Finder
{
    Finder(const Grid& gmap, int tx, int ty) 
    : gmap_{gmap}, 
    w_{gmap.getW()}, 
    h_{gmap.getH()},
    start_{std::make_tuple(0, 0, Torch)},
    target_{std::make_tuple(tx, ty, Torch)}
    {}

    const Grid& gmap_;
    int w_{0};
    int h_{0};
    Node start_;
    Node target_;
    Node2Int dist_;

    // 0 R    1 W    2 N
    // TG     NG     NT

    int search();
};

int Finder::search()
{
    PQ pq;

    auto start{DistNode{0, start_}};
    pq.push(start);

    while (pq.size())
    {
        auto curr{pq.top()};

        auto dist{curr.first};
        auto node{std::move(curr.second)}; 
        pq.pop();

        if (node == target_)
        {
            return dist;
        }
        
        auto x{get<0>(node)};
        auto y{get<1>(node)};
        auto eq{get<2>(node)};

        // check the other equipment

        auto neq{0};
        for (auto i = 1; i <= 2; ++i)
        {
            neq = (eq + i) % 3;
            if (neq != gmap_(x, y))
            {
                break;
            }
        }
        {
            auto ndist{dist + 7};
            auto nnode{std::make_tuple(x, y, neq)};
            auto it{dist_.find(nnode)};
            if (it == dist_.end() || it->second > ndist)
            {
                dist_[nnode] = ndist;
                auto nitem{std::make_pair(ndist, nnode)};
                pq.push(nitem);
            }
        }

        for (auto i = 0; i < 4; ++i)
        {
            auto nx{x + dir[i].first};
            auto ny{y + dir[i].second};

            if (nx < 0 || ny < 0 || nx >= w_ || ny >= h_)
            {
                continue;
            }
            if (eq == gmap_(nx, ny)) // cannot enter
            {
                continue;
            }

            auto ndist{dist + 1};
            auto nnode{std::make_tuple(nx, ny, eq)};
            auto it{dist_.find(nnode)};
            if (it == dist_.end() || it->second > ndist)
            {
                dist_[nnode] = ndist;
                auto nitem{std::make_pair(ndist, nnode)};
                pq.push(nitem);
            }
    
        }    
    }


    return 0;
};

int main()
{
    int d{10689}, x{11}, y{722};
    //d = 510; x = 10; y = 10;
    
    int ret;
    auto gmap{getMap(d, x, y)};
    //printMap(gmap);

    Finder finder(gmap, x, y);
    auto dist{finder.search()};
    cout << "dist=" << dist << endl;

    return 0;
}
