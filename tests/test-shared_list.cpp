#include <yats.hpp>

#include <thread>
#include <shared_list.hpp>

using namespace yats;


Context(single_thread)
{

    Test(ctor_default)
    {
        more::shared_list<int> l;
        Assert(l.size(), is_equal_to(0));
        Assert(l.size(), is_equal_to(l.reverse_size()));
    }


    Test(ctor)
    {
        more::shared_list<int> l1(1);
        Assert(l1.size(), is_equal_to(1));
        Assert(l1.size(), is_equal_to(l1.reverse_size()));

        more::shared_list<int> l2(2);
        Assert(l2.size(), is_equal_to(2));
        Assert(l2.size(), is_equal_to(l2.reverse_size()));

        more::shared_list<int> l3(3);
        Assert(l3.size(), is_equal_to(3));
        Assert(l3.size(), is_equal_to(l3.reverse_size()));

        for(auto &e : l1)
            Assert(e, is_equal_to(0));
        for(auto &e : l2)
            Assert(e, is_equal_to(0));
        for(auto &e : l3)
            Assert(e, is_equal_to(0));
    }


    Test(ctor_init_range)
    {
        //ctor, init from range
        //

        std::vector<int> v0;
        std::vector<int> v1{1};
        std::vector<int> v2{1,2};
        std::vector<int> v3{1,2,3};

        more::shared_list<int> l0(v0.begin(), v0.end());
        more::shared_list<int> l1(v1.begin(), v1.end());
        more::shared_list<int> l2(v2.begin(), v2.end());
        more::shared_list<int> l3(v3.begin(), v3.end());

        Assert(l0.size(), is_equal_to(0));
        Assert(l1.size(), is_equal_to(1));
        Assert(l2.size(), is_equal_to(2));
        Assert(l3.size(), is_equal_to(3));

        Assert(l0.size(), is_equal_to(l0.reverse_size()));
        Assert(l1.size(), is_equal_to(l1.reverse_size()));
        Assert(l2.size(), is_equal_to(l2.reverse_size()));
        Assert(l3.size(), is_equal_to(l3.reverse_size()));

        Assert(std::equal(v0.begin(), v0.end(), l0.begin()));
        Assert(std::equal(v1.begin(), v1.end(), l1.begin()));

        Assert(std::equal(v2.begin(), v2.end(), l2.begin()));
        Assert(std::equal(v3.begin(), v3.end(), l3.begin()));
    }


    Test(ctor_init)
    {
        more::shared_list<int> l0({});
        more::shared_list<int> l1({1});
        more::shared_list<int> l2({1,2});
        more::shared_list<int> l3({1,2,3});

        Assert(l0.size(), is_equal_to(0));
        Assert(l1.size(), is_equal_to(1));
        Assert(l2.size(), is_equal_to(2));
        Assert(l3.size(), is_equal_to(3));

        Assert(l0.size(), is_equal_to(l0.reverse_size()));
        Assert(l1.size(), is_equal_to(l1.reverse_size()));
        Assert(l2.size(), is_equal_to(l2.reverse_size()));
        Assert(l3.size(), is_equal_to(l3.reverse_size()));
    }


    Test(copy_ctor)
    {
        more::shared_list<int> l0({});
        more::shared_list<int> l1({1});
        more::shared_list<int> l2({1,2});
        more::shared_list<int> l3({1,2,3});

        more::shared_list<int> c0(l0);
        more::shared_list<int> c1(l1);
        more::shared_list<int> c2(l2);
        more::shared_list<int> c3(l3);

        Assert(std::equal(l0.begin(), l0.end(), c0.begin()));
        Assert(std::equal(l1.begin(), l1.end(), c1.begin()));
        Assert(std::equal(l2.begin(), l2.end(), c2.begin()));
        Assert(std::equal(l3.begin(), l3.end(), c3.begin()));

        Assert(c0.size(), is_equal_to(c0.reverse_size()));
        Assert(c1.size(), is_equal_to(c1.reverse_size()));
        Assert(c2.size(), is_equal_to(c2.reverse_size()));
        Assert(c3.size(), is_equal_to(c3.reverse_size()));
    }


    Test (assign_operator)
    {
        more::shared_list<int> l0({});
        more::shared_list<int> l1({1});
        more::shared_list<int> l2({1,2});
        more::shared_list<int> l3({1,2,3});

        more::shared_list<int> c0{10,11,12};
        more::shared_list<int> c1{10,11};
        more::shared_list<int> c2{10};
        more::shared_list<int> c3;

        c0 = l0;
        c1 = l1;
        c2 = l2;
        c3 = l3;

        Assert(std::equal(l0.begin(), l0.end(), c0.begin()));
        Assert(std::equal(l1.begin(), l1.end(), c1.begin()));
        Assert(std::equal(l2.begin(), l2.end(), c2.begin()));
        Assert(std::equal(l3.begin(), l3.end(), c3.begin()));

        Assert(c0.size(), is_equal_to(c0.reverse_size()));
        Assert(c1.size(), is_equal_to(c1.reverse_size()));
        Assert(c2.size(), is_equal_to(c2.reverse_size()));
        Assert(c3.size(), is_equal_to(c3.reverse_size()));
    }

    Test (move_ctor)
    {
        more::shared_list<int> l0({});
        more::shared_list<int> l1({1});
        more::shared_list<int> l2({1,2});
        more::shared_list<int> l3({1,2,3});

        more::shared_list<int> c0(std::move(l0));
        more::shared_list<int> c1(std::move(l1));
        more::shared_list<int> c2(std::move(l2));
        more::shared_list<int> c3(std::move(l3));

        Assert(c0.size(), is_equal_to(0));
        Assert(c1.size(), is_equal_to(1));
        Assert(c2.size(), is_equal_to(2));
        Assert(c3.size(), is_equal_to(3));

        Assert(c0.size(), is_equal_to(c0.reverse_size()));
        Assert(c1.size(), is_equal_to(c1.reverse_size()));
        Assert(c2.size(), is_equal_to(c2.reverse_size()));
        Assert(c3.size(), is_equal_to(c3.reverse_size()));

    }

    Test (assign_move_operator)
    {
        more::shared_list<int> l0({});
        more::shared_list<int> l1({1});
        more::shared_list<int> l2({1,2});
        more::shared_list<int> l3({1,2,3});

        more::shared_list<int> c0{10,11,12};
        more::shared_list<int> c1{10,11};
        more::shared_list<int> c2{10};
        more::shared_list<int> c3;

        c0 = std::move(l0);
        c1 = std::move(l1);
        c2 = std::move(l2);
        c3 = std::move(l3);

        Assert(c0.size(), is_equal_to(0));
        Assert(c1.size(), is_equal_to(1));
        Assert(c2.size(), is_equal_to(2));
        Assert(c3.size(), is_equal_to(3));

        Assert(c0.size(), is_equal_to(c0.reverse_size()));
        Assert(c1.size(), is_equal_to(c1.reverse_size()));
        Assert(c2.size(), is_equal_to(c2.reverse_size()));
        Assert(c3.size(), is_equal_to(c3.reverse_size()));
    }


    Test(clear)
    {
        more::shared_list<int> l0({});
        more::shared_list<int> l1({1});
        more::shared_list<int> l2({1,2});
        more::shared_list<int> l3({1,2,3});

        l0.clear();
        l1.clear();
        l2.clear();
        l3.clear();

        Assert(l0.empty());
        Assert(l1.empty());
        Assert(l2.empty());
        Assert(l3.empty());

        Assert(l0.size(), is_equal_to(l0.reverse_size()));
        Assert(l1.size(), is_equal_to(l1.reverse_size()));
        Assert(l2.size(), is_equal_to(l2.reverse_size()));
        Assert(l3.size(), is_equal_to(l3.reverse_size()));
    }

    Test(iterator)
    {
        more::shared_list<int> l0({});
        more::shared_list<int> l1({1});
        more::shared_list<int> l2({1,2});
        more::shared_list<int> l3({1,2,3});

        size_t n;

        n = 0;
        for(auto & elem: l0)
            n++;
        Assert(n, is_equal_to(0));

        n = 0;
        for(auto & elem: l1)
            n++;
        Assert(n, is_equal_to(1));

        n = 0;
        for(auto & elem: l2)
            n++;
        Assert(n, is_equal_to(2));

        n = 0;
        for(auto & elem: l3)
            n++;
        Assert(n, is_equal_to(3));

        Assert(*l1.begin(), is_equal_to(1));
        Assert(*l2.begin(), is_equal_to(1));
        Assert(*l3.begin(), is_equal_to(1));

        Assert(std::next(l1.begin(),1) == l1.end());
        Assert(std::next(l2.begin(),2) == l2.end());
        Assert(std::next(l3.begin(),3) == l3.end());

        Assert(*l1.cbegin(), is_equal_to(1));
        Assert(*l2.cbegin(), is_equal_to(1));
        Assert(*l3.cbegin(), is_equal_to(1));

        Assert(std::next(l1.cbegin(),1) == l1.cend());
        Assert(std::next(l2.cbegin(),2) == l2.cend());
        Assert(std::next(l3.cbegin(),3) == l3.cend());
    }


    Test(front)
    {
        more::shared_list<int> l1({1});
        more::shared_list<int> l2({1,2});
        more::shared_list<int> l3({1,2,3});

        auto const & c1 = l1;
        auto const & c2 = l2;
        auto const & c3 = l3;

        Assert(l1.front(), is_equal_to(1));
        Assert(l2.front(), is_equal_to(1));
        Assert(l3.front(), is_equal_to(1));

        Assert(c1.front(), is_equal_to(1));
        Assert(c2.front(), is_equal_to(1));
        Assert(c3.front(), is_equal_to(1));

        l1.front() = 10;
        l2.front() = 20;
        l3.front() = 30;

        Assert(l1.front(), is_equal_to(10));
        Assert(l2.front(), is_equal_to(20));
        Assert(l3.front(), is_equal_to(30));

    }

    Test(back)
    {
        more::shared_list<int> l1({1});
        more::shared_list<int> l2({1,2});
        more::shared_list<int> l3({1,2,3});

        auto const & c1 = l1;
        auto const & c2 = l2;
        auto const & c3 = l3;

        Assert(l1.back(), is_equal_to(1));
        Assert(l2.back(), is_equal_to(2));
        Assert(l3.back(), is_equal_to(3));

        Assert(c1.back(), is_equal_to(1));
        Assert(c2.back(), is_equal_to(2));
        Assert(c3.back(), is_equal_to(3));

        l1.back() = 10;
        l2.back() = 20;
        l3.back() = 30;

        Assert(l1.back(), is_equal_to(10));
        Assert(l2.back(), is_equal_to(20));
        Assert(l3.back(), is_equal_to(30));

    }


    Test(push_back)
    {
        more::shared_list<int> l0({});
        more::shared_list<int> l1({1});
        more::shared_list<int> l2({1,2});
        more::shared_list<int> l3({1,2,3});

        l0.push_back(42);
        l1.push_back(42);
        l2.push_back(42);
        l3.push_back(42);

        Assert(l0.size(), is_equal_to(l0.reverse_size()));
        Assert(l1.size(), is_equal_to(l1.reverse_size()));
        Assert(l2.size(), is_equal_to(l2.reverse_size()));
        Assert(l3.size(), is_equal_to(l3.reverse_size()));

        Assert(l0.size(), is_equal_to(1));
        Assert(l1.size(), is_equal_to(2));
        Assert(l2.size(), is_equal_to(3));
        Assert(l3.size(), is_equal_to(4));

        Assert(l0.back(), is_equal_to(42));
        Assert(l1.back(), is_equal_to(42));
        Assert(l2.back(), is_equal_to(42));
        Assert(l3.back(), is_equal_to(42));
    }

    Test(push_back2)
    {
        more::shared_list<int> l0({});
        l0.push_back(0);
        l0.push_back(1);
        l0.push_back(2);
        l0.push_back(3);

        Assert(l0.size(), is_equal_to(l0.reverse_size()));
        Assert(l0.size(), is_equal_to(4));

        auto it = l0.begin();

        Assert(*it++, is_equal_to(0));
        Assert(*it++, is_equal_to(1));
        Assert(*it++, is_equal_to(2));
        Assert(*it++, is_equal_to(3));
    }

    Test(push_front)
    {
        more::shared_list<int> l0({});
        more::shared_list<int> l1({1});
        more::shared_list<int> l2({1,2});
        more::shared_list<int> l3({1,2,3});

        l0.push_front(42);
        l1.push_front(42);
        l2.push_front(42);
        l3.push_front(42);

        Assert(l0.size(), is_equal_to(l0.reverse_size()));
        Assert(l1.size(), is_equal_to(l1.reverse_size()));
        Assert(l2.size(), is_equal_to(l2.reverse_size()));
        Assert(l3.size(), is_equal_to(l3.reverse_size()));

        Assert(l0.size(), is_equal_to(1));
        Assert(l1.size(), is_equal_to(2));
        Assert(l2.size(), is_equal_to(3));
        Assert(l3.size(), is_equal_to(4));

        Assert(l0.front(), is_equal_to(42));
        Assert(l1.front(), is_equal_to(42));
        Assert(l2.front(), is_equal_to(42));
        Assert(l3.front(), is_equal_to(42));
    }

    Test(push_front2)
    {
        more::shared_list<int> l0({});
        l0.push_front(3);
        l0.push_front(2);
        l0.push_front(1);
        l0.push_front(0);

        Assert(l0.size(), is_equal_to(l0.reverse_size()));
        Assert(l0.size(), is_equal_to(4));

        auto it = l0.begin();

        Assert(*it++, is_equal_to(0));
        Assert(*it++, is_equal_to(1));
        Assert(*it++, is_equal_to(2));
        Assert(*it++, is_equal_to(3));
    }


    Test(pop_front)
    {
        more::shared_list<int> l1({1});
        more::shared_list<int> l2({1,2});
        more::shared_list<int> l3({1,2,3});

        l1.pop_front();
        l2.pop_front();
        l3.pop_front();

        Assert(l1.size(), is_equal_to(l1.reverse_size()));
        Assert(l2.size(), is_equal_to(l2.reverse_size()));
        Assert(l3.size(), is_equal_to(l3.reverse_size()));

        Assert(l1.size(), is_equal_to(0));
        Assert(l2.size(), is_equal_to(1));
        Assert(l3.size(), is_equal_to(2));

        Assert(l2.front(), is_equal_to(2));
        Assert(l3.front(), is_equal_to(2));

        l2.pop_front();
        l3.pop_front();
        l3.pop_front();

        Assert(l2.size(), is_equal_to(l2.reverse_size()));
        Assert(l3.size(), is_equal_to(l3.reverse_size()));

        Assert(l1.empty());
        Assert(l2.empty());
        Assert(l3.empty());
    }


    Test(pop_back)
    {
        more::shared_list<int> l1({1});
        more::shared_list<int> l2({1,2});
        more::shared_list<int> l3({1,2,3});

        l1.pop_back();
        l2.pop_back();
        l3.pop_back();

        Assert(l1.size(), is_equal_to(l1.reverse_size()));
        Assert(l2.size(), is_equal_to(l2.reverse_size()));
        Assert(l3.size(), is_equal_to(l3.reverse_size()));

        Assert(l1.size(), is_equal_to(0));
        Assert(l2.size(), is_equal_to(1));
        Assert(l3.size(), is_equal_to(2));

        Assert(l2.front(), is_equal_to(1));
        Assert(l3.front(), is_equal_to(1));

        l2.pop_back();
        l3.pop_back();
        l3.pop_back();

        Assert(l2.size(), is_equal_to(l2.reverse_size()));
        Assert(l3.size(), is_equal_to(l3.reverse_size()));

        Assert(l1.empty());
        Assert(l2.empty());
        Assert(l3.empty());
    }

    Test(emplace_back)
    {
        more::shared_list<int> l0({});
        more::shared_list<int> l1({1});
        more::shared_list<int> l2({1,2});
        more::shared_list<int> l3({1,2,3});

        l0.emplace_back(42);
        l1.emplace_back(42);
        l2.emplace_back(42);
        l3.emplace_back(42);

        Assert(l0.size(), is_equal_to(l0.reverse_size()));
        Assert(l1.size(), is_equal_to(l1.reverse_size()));
        Assert(l2.size(), is_equal_to(l2.reverse_size()));
        Assert(l3.size(), is_equal_to(l3.reverse_size()));

        Assert(l0.size(), is_equal_to(1));
        Assert(l1.size(), is_equal_to(2));
        Assert(l2.size(), is_equal_to(3));
        Assert(l3.size(), is_equal_to(4));

        Assert(l0.back(), is_equal_to(42));
        Assert(l1.back(), is_equal_to(42));
        Assert(l2.back(), is_equal_to(42));
        Assert(l3.back(), is_equal_to(42));
    }

    Test(emplace_back2)
    {
        more::shared_list<int> l0({});
        l0.emplace_back(0);
        l0.emplace_back(1);
        l0.emplace_back(2);
        l0.emplace_back(3);

        Assert(l0.size(), is_equal_to(l0.reverse_size()));
        Assert(l0.size(), is_equal_to(4));

        auto it = l0.begin();

        Assert(*it++, is_equal_to(0));
        Assert(*it++, is_equal_to(1));
        Assert(*it++, is_equal_to(2));
        Assert(*it++, is_equal_to(3));
    }

    Test(emplace_front)
    {
        more::shared_list<int> l0({});
        more::shared_list<int> l1({1});
        more::shared_list<int> l2({1,2});
        more::shared_list<int> l3({1,2,3});

        l0.emplace_front(42);
        l1.emplace_front(42);
        l2.emplace_front(42);
        l3.emplace_front(42);

        Assert(l0.size(), is_equal_to(l0.reverse_size()));
        Assert(l1.size(), is_equal_to(l1.reverse_size()));
        Assert(l2.size(), is_equal_to(l2.reverse_size()));
        Assert(l3.size(), is_equal_to(l3.reverse_size()));

        Assert(l0.size(), is_equal_to(1));
        Assert(l1.size(), is_equal_to(2));
        Assert(l2.size(), is_equal_to(3));
        Assert(l3.size(), is_equal_to(4));

        Assert(l0.front(), is_equal_to(42));
        Assert(l1.front(), is_equal_to(42));
        Assert(l2.front(), is_equal_to(42));
        Assert(l3.front(), is_equal_to(42));
    }

    Test(emplace_front2)
    {
        more::shared_list<int> l0({});
        l0.emplace_front(3);
        l0.emplace_front(2);
        l0.emplace_front(1);
        l0.emplace_front(0);

        Assert(l0.size(), is_equal_to(l0.reverse_size()));
        Assert(l0.size(), is_equal_to(4));

        auto it = l0.begin();

        Assert(*it++, is_equal_to(0));
        Assert(*it++, is_equal_to(1));
        Assert(*it++, is_equal_to(2));
        Assert(*it++, is_equal_to(3));
    }


    Test(swap)
    {
        more::shared_list<std::string> l0{"a","b","c","d","e"};
        more::shared_list<std::string> l1;

        Assert(l1.empty());
        l1.swap(l0);

        Assert(l0.empty());
        Assert(l1.size(), is_equal_to(5));

        Assert(l0.size() == l0.reverse_size());
        Assert(l1.size() == l1.reverse_size());
    }

    Test(insert)
    {
        more::shared_list<int> l0;
        more::shared_list<int> l1;
        more::shared_list<int> l2;

        l0.insert(l0.end(), 1);
        l0.insert(l0.end(), 2);
        l0.insert(l0.end(), 3);

        l1.insert(l1.begin(), 3);
        l1.insert(l1.begin(), 2);
        l1.insert(l1.begin(), 1);

        l2.insert(l2.begin(), 1);
        l2.insert(l2.end(),   3);
        l2.insert(std::next(l2.begin()), 2);

        Assert(l0.size(), is_equal_to(3));
        Assert(l0.size() == l0.reverse_size());

        Assert(l1.size(), is_equal_to(3));
        Assert(l1.size() == l1.reverse_size());

        Assert(l2.size(), is_equal_to(3));
        Assert(l2.size() == l2.reverse_size());

        auto i = std::initializer_list<int>{1,2,3};

        Assert(std::equal(i.begin(), i.end(), l0.begin()));
        Assert(std::equal(i.begin(), i.end(), l1.begin()));
        Assert(std::equal(i.begin(), i.end(), l2.begin()));
    }

    Test(erase)
    {
        more::shared_list<int> l0 {1,2,3};

        auto i2 = std::initializer_list<int>{1,2};
        auto i1 = std::initializer_list<int>{1};

        l0.erase(std::next(l0.begin(), 2));
        Assert(l0.size() == l0.reverse_size());
        Assert(std::equal(l0.begin(), l0.end(), i2.begin()));

        l0.erase(std::next(l0.begin(), 1));
        Assert(l0.size() == l0.reverse_size());
        Assert(std::equal(l0.begin(), l0.end(), i1.begin()));

        l0.erase(l0.begin());
        Assert(l0.size() == l0.reverse_size());
        Assert(l0.empty());
    }

    Test(erase2)
    {
        more::shared_list<int> l0 {1,2,3};

        auto i2 = std::initializer_list<int>{2,3};
        auto i1 = std::initializer_list<int>{3};

        l0.erase(l0.begin());
        Assert(l0.size() == l0.reverse_size());
        Assert(std::equal(l0.begin(), l0.end(), i2.begin()));

        l0.erase(l0.begin());
        Assert(l0.size() == l0.reverse_size());
        Assert(std::equal(l0.begin(), l0.end(), i1.begin()));

        l0.erase(l0.begin());
        Assert(l0.size() == l0.reverse_size());
        Assert(l0.empty());
    }


    Test(erase3)
    {
        more::shared_list<int> l0 {1,2,3};

        auto i2 = std::initializer_list<int>{1,3};

        l0.erase(std::next(l0.begin(), 1));
        Assert(l0.size() == l0.reverse_size());
        Assert(std::equal(l0.begin(), l0.end(), i2.begin()));
    }


    Test(atomic_assign)
    {
        more::shared_list<int> l1 {1};
        more::shared_list<int> l2 {1,2};
        more::shared_list<int> l3 {1,2,3};

        auto i1 = std::initializer_list<int>{10};
        auto i2 = std::initializer_list<int>{10,20};
        auto i3 = std::initializer_list<int>{10,20,30};

        l1.atomic_assign(l1.begin(), 10);

        l2.atomic_assign(l2.begin(), 10);
        l2.atomic_assign(std::next(l2.begin()), 20);

        l3.atomic_assign(l3.begin(), 10);
        l3.atomic_assign(std::next(l3.begin()), 20);
        l3.atomic_assign(std::next(l3.begin(), 2), 30);

        Assert(std::equal(l1.begin(), l1.end(), i1.begin()));
        Assert(std::equal(l2.begin(), l2.end(), i2.begin()));
        Assert(std::equal(l3.begin(), l3.end(), i3.begin()));

        Assert(l1.size() == l1.reverse_size());
        Assert(l2.size() == l2.reverse_size());
        Assert(l3.size() == l3.reverse_size());
    }

    Test(shrink)
    {
        more::shared_list<int> l {1,2,3,4,5,6,7,8,9,10};

        for(int i = 0; i < 10; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            l.pop_front();
        }

        Assert(l.shrink(), is_equal_to(0));
        Assert(l.size() == l.reverse_size());

        std::this_thread::sleep_for(std::chrono::milliseconds(95));

        auto x = l.shrink();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        auto y = l.shrink();

        Assert(x+y, is_equal_to(10));
    }


    Test(multiple_destroy)
    {
        more::shared_list<int> l1 {1,2,3,4,5,6,7,8,9,10};
        more::shared_list<int> l2 {1,2,3,4,5,6,7,8,9,10};
        more::shared_list<int> l3 {1,2,3,4,5,6,7,8,9,10};
        more::shared_list<int> l4 {1,2,3,4,5,6,7,8,9,10};
        more::shared_list<int> l5 {1,2,3,4,5,6,7,8,9,10};
        more::shared_list<int> l6 {1,2,3,4,5,6,7,8,9,10};
        more::shared_list<int> l7 {1,2,3,4,5,6,7,8,9,10};
        more::shared_list<int> l8 {1,2,3,4,5,6,7,8,9,10};
        more::shared_list<int> l9 {1,2,3,4,5,6,7,8,9,10};
        more::shared_list<int> l0 {1,2,3,4,5,6,7,8,9,10};

        l0.clear();
        l1.clear();
        l2.clear();
        l3.clear();
        l4.clear();
        l5.clear();
        l6.clear();
        l7.clear();
        l8.clear();
        l9.clear();
    }
}


int
main(int argc, char * argv[])
{
    return yats::run(argc, argv);
}
