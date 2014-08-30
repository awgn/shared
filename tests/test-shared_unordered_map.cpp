#include <yats.hpp>
#include "shared_unordered_map.hpp"

using namespace yats;

Context(SharedMap)
{
    Test(default_ctor)
    {
        more::shared_unordered_map<int, int> m;

        Assert(m.size(), is_equal_to(0));
        Assert(m.maybe_size(), is_equal_to(0));
        Assert(m.empty());

        Assert(m.begin() == m.end());
    }


    Test(range_ctor)
    {
        std::vector<std::pair<int,int>> v0;
        std::vector<std::pair<int,int>> v1 { {1,10} };
        std::vector<std::pair<int,int>> v2 { {1,10}, {2,20} };
        std::vector<std::pair<int,int>> v3 { {1,10}, {2,20}, {3,30} };

        more::shared_unordered_map<int, int> m0  (v0.begin(), v0.end(), 3);
        more::shared_unordered_map<int, int> m1  (v1.begin(), v1.end(), 3);
        more::shared_unordered_map<int, int> m2  (v2.begin(), v2.end(), 3);
        more::shared_unordered_map<int, int> m3  (v3.begin(), v3.end(), 3);
        more::shared_unordered_map<int, int> m3_ (v3.begin(), v3.end(), 1);
        more::shared_unordered_map<int, int> m3__(v3.begin(), v3.end(), 2);

        Assert(m0.size(),   is_equal_to(0));
        Assert(m1.size(),   is_equal_to(1));
        Assert(m2.size(),   is_equal_to(2));
        Assert(m3.size(),   is_equal_to(3));
        Assert(m3_.size(),  is_equal_to(3));
        Assert(m3__.size(), is_equal_to(3));

        m0.clear();
        m1.clear();
        m2.clear();
        m3.clear();
        m3_.clear();
        m3__.clear();
    }


    Test (init_ctor)
    {
        more::shared_unordered_map<int, int> m0 (3);
        more::shared_unordered_map<int, int> m1 ({{1,10}}, 3);
        more::shared_unordered_map<int, int> m2 ({{1,10}, {2,20}}, 3);
        more::shared_unordered_map<int, int> m3 ({{1,10}, {2,20}, {3,30}}, 3);

        Assert(m0.size(), is_equal_to(0));
        Assert(m1.size(), is_equal_to(1));
        Assert(m2.size(), is_equal_to(2));
        Assert(m3.size(), is_equal_to(3));

        m0.clear();
        m1.clear();
        m2.clear();
        m3.clear();
    }


    Test (copy_ctor)
    {
        more::shared_unordered_map<int, int> m0 (3);
        more::shared_unordered_map<int, int> m1 ({{1,10}}, 3);
        more::shared_unordered_map<int, int> m2 ({{1,10}, {2,20}}, 3);
        more::shared_unordered_map<int, int> m3 ({{1,10}, {2,20}, {3,30}}, 3);

        more::shared_unordered_map<int, int> x0 (m0);
        more::shared_unordered_map<int, int> x1 (m1);
        more::shared_unordered_map<int, int> x2 (m2);
        more::shared_unordered_map<int, int> x3 (m3);

        Assert(m0.size(), is_equal_to(0));
        Assert(m1.size(), is_equal_to(1));
        Assert(m2.size(), is_equal_to(2));
        Assert(m3.size(), is_equal_to(3));

        Assert(m0 == x0);
        Assert(m1 == x1);
        Assert(m2 == x2);
        Assert(m3 == x3);

        Assert(m0 != x1);
        Assert(m0 != x2);
        Assert(m0 != x3);

        Assert(m1 != x0);
        Assert(m1 != x2);
        Assert(m1 != x3);

        Assert(m2 != x0);
        Assert(m2 != x1);
        Assert(m2 != x3);

        Assert(m3 != x0);
        Assert(m3 != x1);
        Assert(m3 != x2);

        m0.clear();
        m1.clear();
        m2.clear();
        m3.clear();

        x0.clear();
        x1.clear();
        x2.clear();
        x3.clear();
    }


    Test (move_ctor)
    {
        more::shared_unordered_map<int, int> m0 (3);
        more::shared_unordered_map<int, int> m1 ({{1,10}}, 3);
        more::shared_unordered_map<int, int> m2 ({{1,10}, {2,20}}, 3);
        more::shared_unordered_map<int, int> m3 ({{1,10}, {2,20}, {3,30}}, 3);

        more::shared_unordered_map<int, int> n0 (m0);
        more::shared_unordered_map<int, int> n1 (m1);
        more::shared_unordered_map<int, int> n2 (m2);
        more::shared_unordered_map<int, int> n3 (m3);

        more::shared_unordered_map<int, int> x0 (std::move(n0));
        more::shared_unordered_map<int, int> x1 (std::move(n1));
        more::shared_unordered_map<int, int> x2 (std::move(n2));
        more::shared_unordered_map<int, int> x3 (std::move(n3));

        Assert(m0.size(), is_equal_to(0));
        Assert(m1.size(), is_equal_to(1));
        Assert(m2.size(), is_equal_to(2));
        Assert(m3.size(), is_equal_to(3));

        Assert(m0 == x0);
        Assert(m1 == x1);
        Assert(m2 == x2);
        Assert(m3 == x3);

        Assert(m0 != x1);
        Assert(m0 != x2);
        Assert(m0 != x3);

        Assert(m1 != x0);
        Assert(m1 != x2);
        Assert(m1 != x3);

        Assert(m2 != x0);
        Assert(m2 != x1);
        Assert(m2 != x3);

        Assert(m3 != x0);
        Assert(m3 != x1);
        Assert(m3 != x2);

        m0.clear();
        m1.clear();
        m2.clear();
        m3.clear();

        x0.clear();
        x1.clear();
        x2.clear();
        x3.clear();
    }


    Test (oper_assign)
    {
        more::shared_unordered_map<int, int> m0 (3);
        more::shared_unordered_map<int, int> m1 ({{1,10}}, 3);
        more::shared_unordered_map<int, int> m2 ({{1,10}, {2,20}}, 3);
        more::shared_unordered_map<int, int> m3 ({{1,10}, {2,20}, {3,30}}, 3);

        more::shared_unordered_map<int, int> x0 (m0);
        more::shared_unordered_map<int, int> x1 (m3);
        more::shared_unordered_map<int, int> x2 (m2);
        more::shared_unordered_map<int, int> x3 (m1);

        x0 = m0;
        x1 = m1;
        x2 = m2;
        x3 = m3;

        Assert(m0.size(), is_equal_to(0));
        Assert(m1.size(), is_equal_to(1));
        Assert(m2.size(), is_equal_to(2));
        Assert(m3.size(), is_equal_to(3));

        Assert(m0 == x0);
        Assert(m1 == x1);
        Assert(m2 == x2);
        Assert(m3 == x3);

        Assert(m0 != x1);
        Assert(m0 != x2);
        Assert(m0 != x3);

        Assert(m1 != x0);
        Assert(m1 != x2);
        Assert(m1 != x3);

        Assert(m2 != x0);
        Assert(m2 != x1);
        Assert(m2 != x3);

        Assert(m3 != x0);
        Assert(m3 != x1);
        Assert(m3 != x2);

        m0.clear();
        m1.clear();
        m2.clear();
        m3.clear();

        x0.clear();
        x1.clear();
        x2.clear();
        x3.clear();
    }


    Test (move_assign)
    {
        more::shared_unordered_map<int, int> m0 (3);
        more::shared_unordered_map<int, int> m1 ({{1,10}}, 3);
        more::shared_unordered_map<int, int> m2 ({{1,10}, {2,20}}, 3);
        more::shared_unordered_map<int, int> m3 ({{1,10}, {2,20}, {3,30}}, 3);

        more::shared_unordered_map<int, int> x0 (m0);
        more::shared_unordered_map<int, int> x1 (m3);
        more::shared_unordered_map<int, int> x2 (m2);
        more::shared_unordered_map<int, int> x3 (m1);

        x0 = std::move(m0);
        x1 = std::move(m1);
        x2 = std::move(m2);
        x3 = std::move(m3);

        Assert(m0.size(), is_equal_to(0));
        Assert(m1.size(), is_equal_to(1));
        Assert(m2.size(), is_equal_to(2));
        Assert(m3.size(), is_equal_to(3));

        Assert(m0 == x0);
        Assert(m1 == x1);
        Assert(m2 == x2);
        Assert(m3 == x3);

        Assert(m0 != x1);
        Assert(m0 != x2);
        Assert(m0 != x3);

        Assert(m1 != x0);
        Assert(m1 != x2);
        Assert(m1 != x3);

        Assert(m2 != x0);
        Assert(m2 != x1);
        Assert(m2 != x3);

        Assert(m3 != x0);
        Assert(m3 != x1);
        Assert(m3 != x2);

        m0.clear();
        m1.clear();
        m2.clear();
        m3.clear();

        x0.clear();
        x1.clear();
        x2.clear();
        x3.clear();
    }

    Test(iterator)
    {
        more::shared_unordered_map<int, int> m(3);

        m.insert(std::make_pair(1, 10));
        m.insert(std::make_pair(2, 20));
        m.insert(std::make_pair(3, 30));
        m.insert(std::make_pair(4, 40));

        {
            auto it = m.begin();
            auto it_e = m.end();

            size_t n = 0;
            for(; it != it_e; ++it)
                n += it->second;

            Assert(n, is_equal_to(100));
        }

        {
            auto const & mm = m;
            auto it = mm.begin();
            auto it_e = mm.end();

            size_t n = 0;
            for(; it != it_e; ++it)
                n += it->second;

            Assert(n, is_equal_to(100));
        }

        {
            auto it = m.cbegin();
            auto it_e = m.cend();

            size_t n = 0;
            for(; it != it_e; ++it)
                n += it->second;

            Assert(n, is_equal_to(100));
        }

    }


    Test(insert1)
    {
        more::shared_unordered_map<int, int> m(2);

        auto i1 = m.insert(std::make_pair(1,1));
        auto i2 = m.insert(std::make_pair(2,2));
        auto i3 = m.insert(std::make_pair(3,3));

        auto i4 = m.insert(std::make_pair(3,4));

        Assert(m.size(), is_equal_to(3));

        Assert(i1.second, is_true());
        Assert(i2.second, is_true());
        Assert(i3.second, is_true());
        Assert(i4.second, is_false());

        Assert(i1.first->first, is_equal_to(1));
        Assert(i2.first->first, is_equal_to(2));
        Assert(i3.first->first, is_equal_to(3));
    }

    Test(insert2)
    {
        std::vector<std::pair<int,int>> v {{1,10}, {2,20}, {3,30}};

        more::shared_unordered_map<int, int> m(2);

        m.insert(v.begin(), v.end());

        Assert(m.size(), is_equal_to(3));

        Assert(m.find(1) != m.end());
        Assert(m.find(2) != m.end());
        Assert(m.find(3) != m.end());
        Assert(m.find(4) == m.end());
    }

    Test(insert3)
    {
        more::shared_unordered_map<int, int> m(2);

        m.insert({{1,10}, {2,20}, {3,30}});

        Assert(m.size(), is_equal_to(3));

        Assert(m.find(1) != m.end());
        Assert(m.find(2) != m.end());
        Assert(m.find(3) != m.end());
        Assert(m.find(4) == m.end());
    }


    Test(erase)
    {
        more::shared_unordered_map<int,int> m ({ {1,10}, {2,20}, {3,30}, {4,40} }, 3);

        size_t n;
        while((n = m.size()))
        {
            m.erase(m.begin());
            Assert(m.size(), is_equal_to(n-1));
        }
    }


    Test(erase2)
    {
        more::shared_unordered_map<int, int> m ({ {1,10}, {2,20}, {3,30}, {4,40} }, 3);

        Assert(m.erase(1), is_equal_to(1));
        Assert(m.size(), is_equal_to(3));

        Assert(m.erase(0), is_equal_to(0));
        Assert(m.size(), is_equal_to(3));

        Assert(m.erase(4), is_equal_to(1));
        Assert(m.size(), is_equal_to(2));

        Assert(m.erase(-1), is_equal_to(0));
        Assert(m.size(), is_equal_to(2));

        Assert(m.erase(2), is_equal_to(1));
        Assert(m.size(), is_equal_to(1));

        Assert(m.erase(3), is_equal_to(1));
        Assert(m.size(), is_equal_to(0));

        Assert(m.erase(3), is_equal_to(0));
        Assert(m.size(), is_equal_to(0));

        Assert(m.empty());
    }


    Test(erase3)
    {
        more::shared_unordered_map<int, int> m ({ {1,10}, {2,20}, {3,30}, {4,40} }, 3);

        auto it = m.begin();
        while(m.size())
        {
            it = m.erase(it, std::next(it));
        }

        Assert(it == m.end());
    }


    Test(erase4)
    {
        more::shared_unordered_map<int, int> m ({ {1,10}, {2,20}, {3,30}, {4,40} }, 3);

        auto it = m.erase(m.begin(), m.end());

        Assert(it == m.end());
        Assert(m.size(), is_equal_to(0));
        Assert(m.empty(), is_true());
    }


    Test(erase5)
    {
        more::shared_unordered_map<int, int> m ({ {1,10}, {2,20}, {3,30}, {4,40} }, 3);

        auto it = m.begin();
        it = m.erase(it, std::next(it,2));

        Assert(it != m.end());

        Assert(m.size(), is_equal_to(2));
        Assert(m.empty(), is_false());
    }


    Test(clear)
    {
        more::shared_unordered_map<int, int> m ({ {1,10}, {2,20}, {3,30}, {4,40} }, 3);
        more::shared_unordered_map<int, int> n (3);

        m.clear();
        n.clear();

        m.shrink();
        n.shrink();

        Assert(m.empty());
        Assert(n.empty());

        Assert(m.size(), is_equal_to(0));
        Assert(n.size(), is_equal_to(0));
    }


    Test(swap)
    {
        more::shared_unordered_map<int, int> m ({ {1,10}, {2,20}, {3,30}, {4,40} }, 3);
        more::shared_unordered_map<int, int> n (3);

        n.swap(m);

        Assert(m.size(), is_equal_to(0));
        Assert(m.empty());

        Assert(n.size(), is_equal_to(4));
        Assert(n.empty(), is_false());
    }


    Test(find)
    {
        more::shared_unordered_map<int, int> m(3);

        m.insert(std::make_pair(1, 10));
        m.insert(std::make_pair(2, 20));
        m.insert(std::make_pair(3, 30));
        m.insert(std::make_pair(4, 40));

        auto it1 = m.find(1);
        auto it2 = m.find(2);
        auto it3 = m.find(3);
        auto it4 = m.find(4);
        auto it5 = m.find(5);

        Assert(it1 != m.end() );
        Assert(it2 != m.end() );
        Assert(it3 != m.end() );
        Assert(it4 != m.end() );
        Assert(it5 == m.end() );

        Assert(it1->second, is_equal_to(10));
        Assert(it2->second, is_equal_to(20));
        Assert(it3->second, is_equal_to(30));
        Assert(it4->second, is_equal_to(40));
    }


    Test(count)
    {
        more::shared_unordered_map<int, int> m(3);

        m.insert(std::make_pair(1, 10));
        m.insert(std::make_pair(2, 20));
        m.insert(std::make_pair(3, 30));
        m.insert(std::make_pair(4, 40));

        auto n1 = m.count(1);
        auto n2 = m.count(2);
        auto n3 = m.count(3);
        auto n4 = m.count(4);
        auto n5 = m.count(5);

        Assert(n1 == 1);
        Assert(n2 == 1);
        Assert(n3 == 1);
        Assert(n4 == 1);
        Assert(n5 == 0);
    }


    Test(equal_range)
    {
        more::shared_unordered_map<int, int> m(3);

        m.insert(std::make_pair(1, 10));
        m.insert(std::make_pair(2, 20));
        m.insert(std::make_pair(3, 30));
        m.insert(std::make_pair(4, 40));


        auto p1 = m.equal_range(1);
        auto p2 = m.equal_range(5);

        Assert(std::next(p1.first) == p1.second);
        Assert(p1.first->first, is_equal_to(1));

        Assert(p2.first == p2.second);
        Assert(p2.first == m.end());
    }


    Test(operator_q)
    {
        more::shared_unordered_map<int, int> m(3);

        m[1] = 10;
        m[2] = 20;
        m[3] = 30;

        Assert(m[1], is_equal_to(10));
        Assert(m[2], is_equal_to(20));
        Assert(m[3], is_equal_to(30));

        Assert(m.size(), is_equal_to(3));

        m[3] = 300;

        Assert(m.size(), is_equal_to(3));

        Assert(m[3], is_equal_to(300));
    }


    Test(at)
    {
        more::shared_unordered_map<int, int> m(3);

        m.insert(std::make_pair(1, 10));
        m.insert(std::make_pair(2, 20));
        m.insert(std::make_pair(3, 30));
        m.insert(std::make_pair(4, 40));

        AssertNoThrow(m.at(1));
        AssertNoThrow(m.at(2));
        AssertNoThrow(m.at(3));
        AssertNoThrow(m.at(4));

        AssertThrow(m.at(5));
    }


    Test(bucket)
    {
        more::shared_unordered_map<int, int> m(3);

        Assert(m.bucket_count(), is_equal_to(3));
    }


    Test(bucket_iterator)
    {
        more::shared_unordered_map<int, int> m(3);

        m.insert(std::make_pair(1, 10));
        m.insert(std::make_pair(2, 20));
        m.insert(std::make_pair(3, 30));
        m.insert(std::make_pair(4, 40));

        auto s = 0;

        for(unsigned int i = 0; i < m.bucket_count(); ++i)
        {
            s += std::distance(m.begin(i), m.end(i));
        }

        Assert(s, is_equal_to(4));
    }

    Test(load_factor)
    {
        more::shared_unordered_map<int, int> m(4);

        Assert(m.load_factor(), is_equal_to(0.0));

        m.insert(std::make_pair(1, 10));
        m.insert(std::make_pair(2, 20));
        m.insert(std::make_pair(3, 30));
        m.insert(std::make_pair(4, 40));

        Assert(m.load_factor(), is_equal_to(1.0));
    }

}


int
main(int argc, char * argv[])
{
    return yats::run(argc, argv);
}
