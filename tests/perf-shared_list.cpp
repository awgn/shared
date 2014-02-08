#include <yats.hpp>

#include <thread>
#include <shared_list.hpp>
#include <mutex>
#include <list>

#include <boost/thread/shared_mutex.hpp>

using namespace yats;


Context(multiple_thread)
{
    std::atomic<bool> stop;
    std::atomic<int>  start;

    struct visitor
    {
        template <typename Fun>
        void operator()(Fun pred)
        {
            size_t n = 0;
            start++;
            while(!stop.load(std::memory_order_relaxed))
            {
                auto b = pred();
                if (!b)
                    throw std::runtime_error("predicate falsifiable");
            }
        }
    };

    Test(test_STL)
    {
        std::list<int> l {1,2};

        stop.store(false, std::memory_order_relaxed);
        start.store(0, std::memory_order_relaxed);

        for(int i = 0; i < 10000000; i++)
        {
                l.pop_back();
                l.push_back(2);
        }

        stop.store(true, std::memory_order_relaxed);
    }


    Test(test_lockless_0)
    {
        more::shared_list<int> l {1,2};

        stop.store(false, std::memory_order_relaxed);
        start.store(0, std::memory_order_relaxed);

        for(int i = 0; i < 10000000; i++)
        {
                l.pop_back();
                l.push_back(2);
        }

        stop.store(true, std::memory_order_relaxed);
    }


    Test(test_lockless)
    {
        more::shared_list<int> l {1,2};

        stop.store(false, std::memory_order_relaxed);
        start.store(0, std::memory_order_relaxed);

        std::thread t(visitor(), [&]() -> bool
                      {
                            auto n = l.front();
                            return n == 1;
                      });

        while (start.load(std::memory_order_relaxed) != 1)
            std::this_thread::yield();

        for(int i = 0; i < 10000000; i++)
        {
                l.pop_back();
                l.push_back(2);
        }

        stop.store(true, std::memory_order_relaxed);
        t.join();
    }

    Test(test_lockless_2)
    {
        more::shared_list<int> l {1,2};

        stop.store(false, std::memory_order_relaxed);
        start.store(0, std::memory_order_relaxed);

        std::thread t1(visitor(), [&]() -> bool
                      {
                            auto n = l.front();
                            return n == 1;
                      });

        std::thread t2(visitor(), [&]() -> bool
                      {
                            auto n = l.front();
                            return n == 1;
                      });

        while (start.load(std::memory_order_relaxed) != 2)
            std::this_thread::yield();

        for(int i = 0; i < 10000000; i++)
        {
                l.pop_back();
                l.push_back(2);
        }

        stop.store(true, std::memory_order_relaxed);

        t1.join();
        t2.join();
    }

    Test(test_lockless_3)
    {
        more::shared_list<int> l {1,2};

        stop.store(false, std::memory_order_relaxed);
        start.store(0, std::memory_order_relaxed);

        std::thread t1(visitor(), [&]() -> bool
                      {
                            auto n = l.front();
                            return n == 1;
                      });
        std::thread t2(visitor(), [&]() -> bool
                      {
                            auto n = l.front();
                            return n == 1;
                      });

        std::thread t3(visitor(), [&]() -> bool
                      {
                            auto n = l.front();
                            return n == 1;
                      });

        while (start.load(std::memory_order_relaxed) != 3)
            std::this_thread::yield();

        for(int i = 0; i < 10000000; i++)
        {
                l.pop_back();
                l.push_back(2);
        }

        stop.store(true, std::memory_order_relaxed);
        t1.join();
        t2.join();
        t3.join();
    }


    Test(test_mutex)
    {
        std::list<int> l {1,2};

        stop.store(false, std::memory_order_relaxed);
        start.store(0, std::memory_order_relaxed);

        std::mutex m;

        std::thread t(visitor(), [&]() -> bool
                      {
                            std::lock_guard<std::mutex> lock(m);
                            auto n = l.front();
                            return n == 1;
                      });

        while (start.load(std::memory_order_relaxed) != 1)
            std::this_thread::yield();

        for(int i = 0; i < 10000000; i++)
        {
            {
                std::lock_guard<std::mutex> lock(m);
                l.pop_back();
            }
            {
                std::lock_guard<std::mutex> lock(m);
                l.push_back(2);
            }
        }

        stop.store(true, std::memory_order_relaxed);
        t.join();
    }

    Test(test_mutex_2)
    {
        std::list<int> l {1,2};

        stop.store(false, std::memory_order_relaxed);
        start.store(0, std::memory_order_relaxed);

        std::mutex m;

        std::thread t1(visitor(), [&]() -> bool
                      {
                            std::lock_guard<std::mutex> lock(m);
                            auto n = l.front();
                            return n == 1;
                      });

        std::thread t2(visitor(), [&]() -> bool
                      {
                            std::lock_guard<std::mutex> lock(m);
                            auto n = l.front();
                            return n == 1;
                      });

        while (start.load(std::memory_order_relaxed) != 2)
            std::this_thread::yield();

        for(int i = 0; i < 10000000; i++)
        {
            {
                std::lock_guard<std::mutex> lock(m);
                l.pop_back();
            }
            {
                std::lock_guard<std::mutex> lock(m);
                l.push_back(2);
            }
        }

        stop.store(true, std::memory_order_relaxed);
        t1.join();
        t2.join();
    }

    Test(test_mutex_3)
    {
        std::list<int> l {1,2};

        stop.store(false, std::memory_order_relaxed);
        start.store(0, std::memory_order_relaxed);

        std::mutex m;

        std::thread t1(visitor(), [&]() -> bool
                      {
                            std::lock_guard<std::mutex> lock(m);
                            auto n = l.front();
                            return n == 1;
                      });

        std::thread t2(visitor(), [&]() -> bool
                      {
                            std::lock_guard<std::mutex> lock(m);
                            auto n = l.front();
                            return n == 1;
                      });

        std::thread t3(visitor(), [&]() -> bool
                      {
                            std::lock_guard<std::mutex> lock(m);
                            auto n = l.front();
                            return n == 1;
                      });

        while (start.load(std::memory_order_relaxed) != 3)
            std::this_thread::yield();

        for(int i = 0; i < 10000000; i++)
        {
            {
                std::lock_guard<std::mutex> lock(m);
                l.pop_back();
            }
            {
                std::lock_guard<std::mutex> lock(m);
                l.push_back(2);
            }
        }

        stop.store(true, std::memory_order_relaxed);
        t1.join();
        t2.join();
        t3.join();
    }


    Test(test_rw_mutex)
    {
        std::list<int> l {1,2};

        stop.store(false, std::memory_order_relaxed);
        start.store(0, std::memory_order_relaxed);

        boost::shared_mutex sm;

        std::thread t(visitor(), [&]() -> bool
                      {
                            boost::shared_lock<boost::shared_mutex> lock(sm);
                            auto n = l.front();
                            return n == 1;
                      });


        while (start.load(std::memory_order_relaxed) != 1)
            std::this_thread::yield();

        for(int i = 0; i < 10000000; i++)
        {
            {
                std::lock_guard<boost::shared_mutex> lock(sm);
                l.pop_back();
            }
            {
                std::lock_guard<boost::shared_mutex> lock(sm);
                l.push_back(2);
            }
        }

        stop.store(true, std::memory_order_relaxed);
        t.join();
    }

    Test(test_rw_mutex_2)
    {
        std::list<int> l {1,2};

        stop.store(false, std::memory_order_relaxed);
        start.store(0, std::memory_order_relaxed);

        boost::shared_mutex sm;

        std::thread t1(visitor(), [&]() -> bool
                      {
                            boost::shared_lock<boost::shared_mutex> lock(sm);
                            auto n = l.front();
                            return n == 1;
                      });

        std::thread t2(visitor(), [&]() -> bool
                      {
                            boost::shared_lock<boost::shared_mutex> lock(sm);
                            auto n = l.front();
                            return n == 1;
                      });


        while (start.load(std::memory_order_relaxed) != 2)
            std::this_thread::yield();

        for(int i = 0; i < 10000000; i++)
        {
            {
                std::lock_guard<boost::shared_mutex> lock(sm);
                l.pop_back();
            }
            {
                std::lock_guard<boost::shared_mutex> lock(sm);
                l.push_back(2);
            }
        }

        stop.store(true, std::memory_order_relaxed);
        t1.join();
        t2.join();
    }


    Test(test_rw_mutex_3)
    {
        std::list<int> l {1,2};

        stop.store(false, std::memory_order_relaxed);
        start.store(0, std::memory_order_relaxed);

        boost::shared_mutex sm;

        std::thread t1(visitor(), [&]() -> bool
                      {
                            boost::shared_lock<boost::shared_mutex> lock(sm);
                            auto n = l.front();
                            return n == 1;
                      });

        std::thread t2(visitor(), [&]() -> bool
                      {
                            boost::shared_lock<boost::shared_mutex> lock(sm);
                            auto n = l.front();
                            return n == 1;
                      });

        std::thread t3(visitor(), [&]() -> bool
                      {
                            boost::shared_lock<boost::shared_mutex> lock(sm);
                            auto n = l.front();
                            return n == 1;
                      });

        while (start.load(std::memory_order_relaxed) != 3)
            std::this_thread::yield();

        for(int i = 0; i < 10000000; i++)
        {
            {
                std::lock_guard<boost::shared_mutex> lock(sm);
                l.pop_back();
            }
            {
                std::lock_guard<boost::shared_mutex> lock(sm);
                l.push_back(2);
            }
        }

        stop.store(true, std::memory_order_relaxed);
        t1.join();
        t2.join();
        t3.join();
    }
}


int
main(int argc, char * argv[])
{
    return yats::run(argc, argv);
}
