
#include <yats.hpp>

#include <shared_unordered_map.hpp>

using namespace yats;

Context(mt_tests)
{
    std::atomic<bool> stop;

    struct visitor
    {
        template <typename Fun>
        void operator()(Fun pred)
        {
            //size_t n = 0;
            while(!stop.load(std::memory_order_relaxed))
            {
                auto b = pred();
                if (!b)
                    throw std::runtime_error("predicate falsifiable");
            }
        }
    };

    //Skipping c-tors
    Test(iterator)
    {
        more::shared_unordered_map<int, int> m(3);
        more::shared_unordered_map<int, int> & c = m;
        //auto const & cref = m;

        stop.store(false, std::memory_order_relaxed);

        std::thread t(visitor(), [&c]() -> bool
                      {
                            for(auto & elem: c){
                                auto fir = elem.second;
                                auto sec = elem.second;
                                ++sec;
                                ++fir;
                            }
                            return true;
                      });

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

        stop.store(true, std::memory_order_relaxed);
        t.join();
    }

    Test(usage)
    {
    }
}

int
main(int argc, char *argv[])
{
    return yats::run();
}

