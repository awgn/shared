#include <shared_list.hpp>

#include <iostream>
#include <vector>
#include <thread>

int
main(int argc, char *argv[])
{
    more::shared_list<int> l;
    more::shared_list<int> r;

    r = std::move(l);

    more::shared_list<int> l2(10);

    std::vector<int> v{1,2,3};

    more::shared_list<int> l3(v.begin(), v.end());

    std::cout << l3.size() << std::endl;

    more::shared_list<int> l4({1,2,3,4});

    std::cout << l4.size() << std::endl;
    std::cout << l4.front() << std::endl;

    l4.dump();

    more::shared_list<int> l5;

    int n = 0;
    l5.push_back(++n);
    l5.push_back(++n);
    l5.push_back(++n);

    l5.dump();

    more::shared_list<int> l6;

    n = 4;
    l6.push_front(--n);
    l6.push_front(--n);
    l6.push_front(--n);

    l6.dump();

    more::shared_list<int> l7;

    n = 0;
    l7.emplace_back(++n);
    l7.emplace_back(++n);
    l7.emplace_back(++n);

    l7.dump();

    std::cout << "Test: " << std::endl;
    l7.pop_front();
    l7.dump();
    l7.pop_front();
    l7.dump();
    l7.pop_front();
    l7.dump();

    l6.pop_back();
    l6.dump();
    l6.pop_back();
    l6.dump();
    l6.pop_back();
    l6.dump();

    for(auto &elem : l5)
    {
        std::cout << elem << std::endl;
    }

    more::shared_list<int> l8;

    l8.insert(l8.end(), 1);
    l8.dump();
    l8.insert(l8.end(), 3);
    l8.dump();
    l8.insert(l8.begin(), 0);
    l8.dump();

    auto i8 = l8.begin();
    std::advance(i8, 2);

    l8.insert(i8, 2);
    l8.dump();

    more::shared_list<int> l9;
    l9.insert(l9.begin(), 5, 42);
    l9.dump();

    more::shared_list<int> l10;

    l10.push_back(10);
    l10.erase(l10.begin());
    l10.dump();

    l10.push_back(1);
    l10.push_back(2);
    l10.erase(l10.begin());
    l10.dump();

    l10.clear();
    l10.push_back(1);
    l10.push_back(2);

    auto it = l10.begin();
    l10.erase(++it);
    l10.dump();

    l10.clear();
    l10.push_back(1);
    l10.push_back(2);
    l10.push_back(3);

    auto it2 = l10.begin();
    l10.erase(++it2);
    l10.dump();

    std::cout << "---------------\n";

    more::shared_list<int> l11 {0,1,2};
    l11.dump();

    l11.atomic_assign(l11.begin(), 10);
    l11.dump();

    l11.atomic_assign(std::next(l11.begin(),1), 11);
    l11.dump();

    l11.atomic_assign(std::next(l11.begin(),2), 12);
    l11.dump();

    std::cout << "---------------\n";
    more::shared_list<int> l12(l11);
    l12.dump();

    more::shared_list<int> l13 {42,42,42};

    l13 = l12;
    l13.dump();

    std::swap(l12, l13);

    std::cout << "---------------\n";
    more::shared_list<int> l14 {1,2,3,4,5,6,7,8,9,10};

    for(int i = 0; i < 10; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        l14.pop_front();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(95));

    std::cout << l14.shrink() << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << l14.shrink() << std::endl;

    return 0;
}

