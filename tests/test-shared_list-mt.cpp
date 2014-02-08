#include <yats.hpp>

#include <thread>
#include <shared_list.hpp>

using namespace yats;


Context(multiple_thread)
{
    std::atomic<bool> stop;

    struct visitor
    {
        template <typename Fun>
        void operator()(Fun pred)
        {
            size_t n = 0;
            while(!stop.load(std::memory_order_relaxed))
            {
                auto b = pred();
                if (!b)
                    throw std::runtime_error("predicate falsifiable");
            }
        }
    };


    Test(clear)
    {
        more::shared_list<int> l  {1,2,3,4,5,6,7,8,9};
        more::shared_list<int> l2 {1,2,3,4,5,6,7,8,9};

        stop.store(false, std::memory_order_relaxed);

        std::thread t(visitor(), [&l]() -> bool
                      {
                            auto s = l.size();
                            return s >= 0 && s <= 10;
                      });

        for(int i = 0; i < 20; i++)
        {
            std::cout << "." << std::flush;
            l.clear();
            l = l2;
        }

        stop.store(true, std::memory_order_relaxed);
        t.join();
    }


    Test(front)
    {
        more::shared_list<int> l  {1,2,3,4,5,6,7,8,9};

        stop.store(false, std::memory_order_relaxed);

        std::thread t(visitor(), [&l]() -> bool
                      {
                            auto n = l.front();
                            return n == 1 || n == 2;
                      });

        for(int i = 0; i < 10000000; i++)
        {
            l.pop_front();
            l.push_front(1);
        }

        stop.store(true, std::memory_order_relaxed);
        t.join();
    }

    Test(front2)
    {
        more::shared_list<int> l  {1};

        stop.store(false, std::memory_order_relaxed);

        std::thread t(visitor(), [&l]() -> bool
                      {
                            return l.size() <= 1;
                      });

        for(int i = 0; i < 10000000; i++)
        {
            l.pop_front();
            l.push_front(1);
        }

        stop.store(true, std::memory_order_relaxed);
        t.join();
    }


    Test(front3)
    {
        more::shared_list<int> l  {1,2};

        stop.store(false, std::memory_order_relaxed);

        std::thread t(visitor(), [&l]() -> bool
                      {
                            auto n = l.back();
                            return n == 2;
                      });

        for(int i = 0; i < 10000000; i++)
        {
            l.pop_front();
            l.push_front(1);
        }

        stop.store(true, std::memory_order_relaxed);
        t.join();
    }

    Test(back)
    {
        more::shared_list<int> l  {1,2,3,4,5,6,7,8,9};

        stop.store(false, std::memory_order_relaxed);

        std::thread t(visitor(), [&l]() -> bool
                      {
                            auto n = l.back();
                            return n == 8 || n == 9;
                      });

        for(int i = 0; i < 10000000; i++)
        {
            l.pop_back();
            l.push_back(9);
        }

        stop.store(true, std::memory_order_relaxed);
        t.join();
    }

    Test(back2)
    {
        more::shared_list<int> l  {1};

        stop.store(false, std::memory_order_relaxed);

        std::thread t(visitor(), [&l]() -> bool
                      {
                            return l.size() <= 1;
                      });

        for(int i = 0; i < 10000000; i++)
        {
            l.pop_back();
            l.push_back(1);
        }

        stop.store(true, std::memory_order_relaxed);
        t.join();
    }

    Test(back3)
    {
        more::shared_list<int> l  {1,2};

        stop.store(false, std::memory_order_relaxed);

        std::thread t(visitor(), [&l]() -> bool
                      {
                            auto n = l.front();
                            return n == 1;
                      });

        for(int i = 0; i < 10000000; i++)
        {
            l.pop_back();
            l.push_back(2);
        }

        stop.store(true, std::memory_order_relaxed);
        t.join();
    }

    Test(emplace_front)
    {
        more::shared_list<int> l  {1,2,3,4,5,6,7,8,9};

        stop.store(false, std::memory_order_relaxed);

        std::thread t(visitor(), [&l]() -> bool
                      {
                            auto n = l.front();
                            return n == 1 || n == 2;
                      });

        for(int i = 0; i < 10000000; i++)
        {
            l.pop_front();
            l.emplace_front(1);
        }

        stop.store(true, std::memory_order_relaxed);
        t.join();
    }

    Test(emplace_front2)
    {
        more::shared_list<int> l  {1};

        stop.store(false, std::memory_order_relaxed);

        std::thread t(visitor(), [&l]() -> bool
                      {
                            return l.size() <= 1;
                      });

        for(int i = 0; i < 10000000; i++)
        {
            l.pop_front();
            l.emplace_front(1);
        }

        stop.store(true, std::memory_order_relaxed);
        t.join();
    }


    Test(emplace_front3)
    {
        more::shared_list<int> l  {1,2};

        stop.store(false, std::memory_order_relaxed);

        std::thread t(visitor(), [&l]() -> bool
                      {
                            auto n = l.back();
                            return n == 2;
                      });

        for(int i = 0; i < 10000000; i++)
        {
            l.pop_front();
            l.emplace_front(1);
        }

        stop.store(true, std::memory_order_relaxed);
        t.join();
    }

    Test(emplace_back)
    {
        more::shared_list<int> l  {1,2,3,4,5,6,7,8,9};

        stop.store(false, std::memory_order_relaxed);

        std::thread t(visitor(), [&l]() -> bool
                      {
                            auto n = l.back();
                            return n == 8 || n == 9;
                      });

        for(int i = 0; i < 10000000; i++)
        {
            l.pop_back();
            l.emplace_back(9);
        }

        stop.store(true, std::memory_order_relaxed);
        t.join();
    }

    Test(emplace_back2)
    {
        more::shared_list<int> l  {1};

        stop.store(false, std::memory_order_relaxed);

        std::thread t(visitor(), [&l]() -> bool
                      {
                            return l.size() <= 1;
                      });

        for(int i = 0; i < 10000000; i++)
        {
            l.pop_back();
            l.emplace_back(1);
        }

        stop.store(true, std::memory_order_relaxed);
        t.join();
    }

    Test(emplace_back3)
    {
        more::shared_list<int> l  {1,2};

        stop.store(false, std::memory_order_relaxed);

        std::thread t(visitor(), [&l]() -> bool
                      {
                            auto n = l.front();
                            return n == 1;
                      });

        for(int i = 0; i < 10000000; i++)
        {
            l.pop_back();
            l.emplace_back(2);
            l.shrink();
        }

        stop.store(true, std::memory_order_relaxed);
        t.join();
    }


    Test(insert_erase)
    {
        more::shared_list<int> l  {1,2,3,4,5,6,7,8,9};

        stop.store(false, std::memory_order_relaxed);


        std::thread t(visitor(), [&]() -> bool
                      {
                            auto it = std::next(l.begin(), 5);
                            auto n = *it;
                            return n == 6 || n == 7;
                      });

        auto it = std::next(l.begin(), 5);
        for(int i = 0; i < 10000000; i++)
        {
            it = l.erase(it);
            it = l.insert(it, 6);
        }

        stop.store(true, std::memory_order_relaxed);
        t.join();
    }


    Test(atomic_assign)
    {
        more::shared_list<int> l  {1,2,3,4,5,6,7,8,9};

        stop.store(false, std::memory_order_relaxed);


        std::thread t(visitor(), [&]() -> bool
                      {
                            auto it = std::next(l.begin(), 5);
                            auto n = *it;
                            return n == 6 || n == 42;
                      });

        auto it = std::next(l.begin(), 5);

        for(int i = 0; i < 10000000; i++)
        {
            it = l.atomic_assign(it, i & 1 ? 42 : 6);
        }

        stop.store(true, std::memory_order_relaxed);
        t.join();
    }


    Test(swap)
    {
        more::shared_list<int> l1  {1,2,3,4,5,6,7,8,9};
        more::shared_list<int> l2  {9,8,7,6,5,4,3,2,1};

        stop.store(false, std::memory_order_relaxed);

        std::thread t1(visitor(), [&]() -> bool
                      {
                            return l1.size() == 9;
                      });

        std::thread t2(visitor(), [&]() -> bool
                      {
                            return l2.size() == 9;
                      });

        for(int i = 0; i < 10000000; i++)
        {
            l1.swap(l2);
        }

        stop.store(true, std::memory_order_relaxed);
        t1.join();
        t2.join();
    }

}


int
main(int argc, char * argv[])
{
    return yats::run(argc, argv);
}
