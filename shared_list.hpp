/*
 *  Copyright (c) 2011-2014 Bonelli Nicola <nicola.bonelli@cnit.it>
 *                          Loris Gazzarrini <loris.gazzarrini@for.iet.unipi.it>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#ifndef __SHARED_LIST_HPP__
#define __SHARED_LIST_HPP__

#include <atomic>
#include <memory>
#include <initializer_list>
#include <iterator>
#include <chrono>
#include <fstream>
#include <algorithm>
#include <vector>
#include <thread>
#include <iostream>

namespace more {

    /////////////////////// Default grace period:

    namespace defaults
    {
        const constexpr std::chrono::milliseconds grace_period {100};
    }

    /////////////////////// Time Policies:

    struct TimeStampCounter
    {
        typedef unsigned long long time_point;
        typedef unsigned long long duration;

        static time_point
        now()
        {
#if defined(__i386__) && !defined(__LP64__)
                time_point val;
                __asm__ __volatile__("rdtsc" : "=A" (val));
                return val;
#endif
#if defined(__LP64__)
                time_point val;
                unsigned long __a,__d;
                __asm__ __volatile__("rdtsc" : "=a" (__a), "=d" (__d));
                val = ((unsigned long)__a) | (((unsigned long long)__d)<<32);
                return val;
#endif
        }

        static duration
        grace_period()
        {
            typedef typename std::decay<decltype(defaults::grace_period)>::type duration_type;
            static duration ret = clock() * defaults::grace_period.count() / duration_type(std::chrono::seconds(1)).count();
            return ret;
        }

    private:

        static uint64_t
        clock()
        {
            std::ifstream cpu_info("/proc/cpuinfo");

            std::vector<std::string> v((std::istream_iterator<std::string>(cpu_info)), (std::istream_iterator<std::string>()));

            auto it = std::find(v.begin(), v.end(), "MHz");
            if (it == std::end(v))
                throw std::runtime_error("TimeStampCounter::clock");

            auto c = std::stod(*std::next(it, 2));
            return static_cast<uint64_t>(c) * 1000000;
        }
    };

    struct TimePoint
    {
        typedef std::chrono::high_resolution_clock::time_point   time_point;
        typedef std::chrono::high_resolution_clock::duration     duration;

        static time_point
        now()
        {
            return std::chrono::high_resolution_clock::now();
        }

        static duration
        grace_period()
        {
            return defaults::grace_period;
        }
    };

    ///////////////////// shared_list

    template <typename T, typename Time = TimeStampCounter, typename Alloc = std::allocator<T> >
    struct shared_list
    {

    public:

        typedef T                                                       value_type;
        typedef Alloc                                                   allocator_type;
        typedef std::size_t                                             size_type;
        typedef std::ptrdiff_t                                          difference_type;

        typedef value_type &                                            reference;
        typedef const value_type &                                      const_reference;

        typedef typename std::allocator_traits<Alloc>::pointer          pointer;
        typedef typename std::allocator_traits<Alloc>::const_pointer    const_pointer;


    private:

        struct node
        {
            T                                       value;
            typename Time::time_point               tp;
            std::atomic<node *>                     next;
            node *                                  prev;
        };

        typedef typename Alloc::template rebind<node>::other  AllocNode;

    public:

        struct _list_iterator : std::iterator<std::forward_iterator_tag, T>
        {
            _list_iterator()
            : node_(nullptr)
            {}

            explicit _list_iterator(node *p)
            : node_(p)
            {}

            reference
            operator*() const
            {
                return node_->value;
            }

            pointer
            operator->() const
            {
                return &node_->value;
            }

            _list_iterator &
            operator++()
            {
                node_ = node_->next.load(std::memory_order_acquire);
                return *this;
            }

            _list_iterator
            operator++(int)
            {
                auto self = *this;
                node_ = node_->next.load(std::memory_order_acquire);
                return self;
            }

            bool
            operator==(const _list_iterator &it)
            {
                return node_ == it.node_;
            }

            bool
            operator!=(const _list_iterator &it)
            {
                return node_ != it.node_;
            }

            node * node_;
        };


        struct _const_list_iterator : std::iterator<std::forward_iterator_tag, const T>
        {
            _const_list_iterator()
            : node_(nullptr)
            {}

            explicit _const_list_iterator(node *p)
            : node_(p)
            {}

            _const_list_iterator(const _list_iterator &it)
            : node_(it.node_)
            {}

            reference
            operator*() const
            {
                return node_->value;
            }

            pointer
            operator->() const
            {
                return &node_->value;
            }

            _const_list_iterator &
            operator++()
            {
                node_ = node_->next.load(std::memory_order_acquire);
                return *this;
            }

            _const_list_iterator
            operator++(int)
            {
                auto self = *this;
                node_ = node_->next.load(std::memory_order_acquire);
                return self;
            }

            bool
            operator==(const _const_list_iterator &it)
            {
                return node_ == it.node_;
            }

            bool
            operator!=(const _const_list_iterator &it)
            {
                return node_ != it.node_;
            }

            node * node_;
        };

        typedef _list_iterator           iterator;
        typedef _const_list_iterator     const_iterator;

    public:

        /* thread unsafe: to be called with no traversing visitors */

        explicit shared_list(const Alloc & alloc = Alloc())
        : head_(nullptr)
        , tail_(nullptr)
        , garbage_(this)
        , alloc_(alloc)
        {}

        /* value intialize */

        explicit shared_list(size_t n)
        : head_(nullptr)
        , tail_(nullptr)
        , garbage_(this)
        , alloc_(Alloc())
        {
            for(unsigned int i = 0; i < n; i++)
            {
                auto n = new_node_();
                insert_node_(head_.load(std::memory_order_relaxed), n);
            }
        }

        template <typename Iter>
        shared_list(Iter it, Iter end, const Alloc & alloc = Alloc())
        : head_(nullptr)
        , tail_(nullptr)
        , garbage_(this)
        , alloc_(alloc)
        {
            for(; it != end; ++it)
            {
                auto n = new_node_(*it);
                insert_node_(nullptr, n);
            }
        }

        shared_list(std::initializer_list<T> init, const Alloc & alloc = Alloc())
        : head_(nullptr)
        , tail_(nullptr)
        , garbage_(this)
        , alloc_(alloc)
        {
            for(auto const & elem : init)
            {
                auto n = new_node_(elem);
                insert_node_(nullptr, n);
            }
        }

        shared_list(const shared_list &other)
        : shared_list(std::begin(other), std::end(other), other.get_allocator())
        {
            /* using delegate constructor */
        }

        shared_list& operator=(const shared_list &other)
        {
            /* swap idiom */

            shared_list local(other);
            local.swap(*this);
            return *this;
        }

        shared_list(shared_list &&rhs)
        : head_(rhs.head_.load(std::memory_order_relaxed))
        , tail_(rhs.tail_)
        , garbage_(std::move(rhs.garbage_))
        {
            garbage_.set_ownership(this);

            rhs.head_.store(nullptr, std::memory_order_relaxed);
            rhs.tail_ = nullptr;
        }

        shared_list& operator=(shared_list &&rhs)
        {
            if (&rhs != this)
            {
                auto p = head_.load(std::memory_order_relaxed);
                head_.store(rhs.head_.load(std::memory_order_relaxed));
                tail_ = rhs.tail_;
                destroy_list_(p);
                rhs.head_.store(nullptr, std::memory_order_relaxed);
                rhs.tail_ = nullptr;

                garbage_ = std::move(rhs.garbage_);
                garbage_.set_ownership(this);
            }
            return *this;
        }

        ~shared_list()
        {
            this->clear();
        }

        /***** shared and thread-safe: only affected by data-race on T (to be handled by user-code) *****/

        void clear()
        {
            auto h = head_.exchange(nullptr, std::memory_order_relaxed);
            tail_ = nullptr;
            destroy_list_(h);
        }

        size_type shrink()
        {
            auto n = garbage_.flush();
            return n < 0 ? 0 : n;
        }

        iterator
        begin()
        {
            return _list_iterator(head_.load(std::memory_order_acquire));
        }

        const_iterator
        begin() const
        {
            return _const_list_iterator(head_.load(std::memory_order_acquire));
        }

        iterator
        end()
        {
            return _list_iterator();
        }
        const_iterator
        end() const
        {
            return _const_list_iterator();
        }

        const_iterator
        cbegin() const
        {
            return _const_list_iterator(head_.load(std::memory_order_acquire));
        }
        const_iterator
        cend() const
        {
            return _const_list_iterator();
        }

        Alloc get_allocator() const noexcept
        {
            return alloc_;
        }

        reference front()
        {
            return head_.load(std::memory_order_acquire)->value;
        }

        const_reference front() const
        {
            return head_.load(std::memory_order_acquire)->value;
        }

        reference back()
        {
            return tail_->value;
        }

        const_reference back() const
        {
            return tail_->value;
        }

        bool empty() const noexcept
        {
            return head_.load(std::memory_order_relaxed) == nullptr;
        }

        size_type max_size() const noexcept
        {
            return alloc_.max_size();
        }

        size_type size() noexcept
        {
            size_type c = 0;
            node * n = head_.load(std::memory_order_relaxed);
            while (n)
            {
                n = n->next.load(std::memory_order_relaxed);
                c++;
            }
            return c;
        }

        size_type maybe_size() const noexcept
        {
            size_type c = 0;
            node * n = head_.load(std::memory_order_relaxed);
            while (n)
            {
                n = n->next.load(std::memory_order_relaxed);
                c++;
            }
            return c;
        }

        size_type
        reverse_size() noexcept
        {
            size_type c = 0;
            node * t = tail_;
            while (t)
            {
                t = t->prev;
                c++;
            }

            return c;
        }

        void dump() const
        {
            auto n = head_.load(std::memory_order_relaxed);

            std::cout << "=> " << (void *)head_.load(std::memory_order_relaxed) << " [ ";
            while (n) {
                std::cout << "|" << (void *)n->prev << " " << n->value  << " " << n->next.load(std::memory_order_relaxed) << "|-" << n << " ";
                n = n->next.load(std::memory_order_relaxed);
            }
            std::cout << " ] <= " << tail_ << std::endl;
        }

        void push_back(const T&value)
        {
            this->insert(end(), value);
        }

        void push_back(T&& value)
        {
            this->insert(end(), std::move(value));
        }

        void push_front(const T&value)
        {
            this->insert(begin(), value);
        }

        void push_front(T&& value)
        {
            this->insert(begin(), std::move(value));
        }

        void pop_front()
        {
            this->erase(begin());
        }

        void pop_back()
        {
            this->erase(iterator(tail_));
        }

        template <typename ...Ts>
        void emplace_back(Ts && ...args)
        {
            auto n = new_node_(std::forward<Ts>(args)...);
            insert_node_(nullptr, n);
        }

        template <typename ...Ts>
        void emplace_front(Ts && ...args)
        {
            auto n = new_node_(std::forward<Ts>(args)...);
            insert_node_(head_.load(std::memory_order_relaxed), n);
        }

        void swap(shared_list &other)
        {
            auto that = head_.exchange(other.head_.load(std::memory_order_relaxed), std::memory_order_relaxed);
            other.head_.store(that, std::memory_order_relaxed);
            std::swap(tail_, other.tail_);
            std::swap(garbage_, other.garbage_);

            garbage_.set_ownership(&other);
            other.garbage_.set_ownership(this);
        }

        iterator insert(const_iterator pos, const T& value)
        {
            auto n = new_node_(value);
            insert_node_(pos.node_, n);
            return iterator(n);
        }

        iterator insert(const_iterator pos, size_type count, const T&value)
        {
            iterator p(pos.node_);
            for(size_type i = 0; i < count; i++)
            {
                p = this->insert(p, value);
            }
            return p;
        }

        iterator atomic_assign(iterator pos, const T &value)
        {
            auto n = new_node_(value);

            auto del = pos.node_;
            auto nxt = del->next.load(std::memory_order_relaxed);

            n->next.store(nxt, std::memory_order_relaxed);

            if (del->prev) {
                del->prev->next.store(n, std::memory_order_release);
            }
            else  {
                head_.store(n, std::memory_order_release);
            }

            n->prev = del->prev;

            if (nxt)
                nxt->prev = n;
            else
                tail_ = n;

            garbage_.free(del);

            return iterator(n);
        }

        iterator erase(const_iterator pos)
        {
            if (pos.node_ == tail_) {
                auto that = tail_;
                if (tail_->prev)
                    tail_->prev->next.store(nullptr, std::memory_order_release);
                else
                    head_.store(nullptr, std::memory_order_release);
                tail_ = tail_->prev;
                garbage_.free(that);
                return iterator(nullptr);
            }
            else if (pos == begin()) {
                auto that = head_.load(std::memory_order_relaxed);
                head_.store(that->next.load(std::memory_order_relaxed), std::memory_order_release);
                head_.load(std::memory_order_relaxed)->prev = nullptr;
                garbage_.free(that);
                return iterator(tail_);
            }
            else {
                auto that = pos.node_;
                auto next = that->next.load(std::memory_order_relaxed);
                that->prev->next.store(next, std::memory_order_release);
                next->prev = that->prev;
                garbage_.free(that);
                return iterator(next);
            }
        }

        static typename Time::duration
        grace_period()
        {
            return Time::grace_period();
        }

    private:

        /* value intialize */

        node * new_node_()
        {
            node * p = nullptr;
            try
            {
                p = allocnode_.allocate(1);
            }
            catch(...)
            {
                if (p) allocnode_.deallocate(p, 1);
                throw;
            }

            try
            {
                alloc_.construct(&p->value, T());
            }
            catch(...)
            {
                allocnode_.deallocate(p, 1);
                throw;
            }
            return p;
        }

        node * new_node_(const T &value)
        {
            node * p = nullptr;
            try
            {
                p = allocnode_.allocate(1);
            }
            catch(...)
            {
                if (p) allocnode_.deallocate(p, 1);
                throw;
            }

            try
            {
                alloc_.construct(&p->value, value);
            }
            catch(...)
            {
                allocnode_.deallocate(p, 1);
                throw;
            }
            return p;
        }

        template <typename ...Ts>
        node * new_node_(Ts && ...args)
        {
            node * p = nullptr;
            try
            {
                p = allocnode_.allocate(1);
            }
            catch(...)
            {
                if (p) allocnode_.deallocate(p, 1);
                throw;
            }

            try
            {
                alloc_.construct(&p->value, std::forward<Ts>(args)...);
            }
            catch(...)
            {
                allocnode_.deallocate(p, 1);
                throw;
            }
            return p;
        }

        void
        insert_node_(node *pos, node *n)
        {
            if (pos == nullptr) {
                n->next.store(nullptr, std::memory_order_relaxed);
                if (tail_)
                    tail_->next.store(n, std::memory_order_release);
                else
                    head_.store(n, std::memory_order_release);
                n->prev = tail_;
                tail_ = n;
            }
            else if (pos == head_.load(std::memory_order_relaxed)) {
                n->next.store(head_.load(std::memory_order_relaxed), std::memory_order_relaxed);
                n->prev = nullptr;
                head_.load(std::memory_order_relaxed)->prev = n;
                head_.store(n, std::memory_order_release);
            }
            else {
                n->next.store(pos, std::memory_order_relaxed);
                n->prev = pos->prev;
                pos->prev->next.store(n, std::memory_order_release);
                pos->prev = n;
            }
        }

        void destroy_node_(node *n)
        {
            alloc_.destroy(&n->value);
            allocnode_.deallocate(n, 1);
        }

        void destroy_list_(node *p)
        {
            node *q;
            for(; p != nullptr; p = q)
            {
                q = p->next.load(std::memory_order_relaxed);
                garbage_.free(p);
            }
        }

        struct garbage
        {
            garbage(shared_list *l)
            : owner_(l)
            , ptr_(nullptr)
            , tail_(nullptr)
            {}

            ~garbage()
            {
                while(this->flush() != -1)
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            garbage(const garbage &) = delete;
            garbage& operator=(const garbage &) = delete;

            garbage(garbage &&other)
            {
                ptr_  = other.ptr_, other.ptr_ = nullptr;
                tail_ = other.tail_, other.tail_ = nullptr;
                owner_ = nullptr;
            }

            garbage& operator=(garbage &&other)
            {
                while(this->flush() != -1)
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));

                ptr_ = other.ptr_, other.ptr_ = nullptr;
                tail_ = other.tail_, other.tail_ = nullptr;
                owner_ = nullptr;
                return *this;
            }

            void
            set_ownership(shared_list *l)
            {
                owner_ = l;
            }


            void free(node *p)
            {
                auto now = Time::now();

                p->tp    = now;
                p->prev  = nullptr;

                if (tail_)
                {
                    tail_->prev = p;
                }
                else
                {
                    ptr_ = p;
                }

                tail_ = p;

                if (now - ptr_->tp > Time::grace_period())
                {
                    auto q = ptr_;
                    ptr_ = ptr_->prev;
                    delete q;
                }
            }


            node *
            recycle()
            {
                auto p = ptr_;

                if (p && (Time::now() - p->tp > Time::grace_period()))
                {
                    ptr_ = ptr_->prev;
                    return p;
                }
                return nullptr;
            }

            std::ptrdiff_t
            flush()
            {
                node *q = nullptr, *p = ptr_, *n;

                if (p == nullptr)
                    return -1;

                auto now = Time::now();

                size_type ret = 0;

                for(; p != nullptr; p = n)
                {
                    n = p->prev;

                    if (now - p->tp > Time::grace_period())
                    {
                        if (q)
                            q->prev = n;
                        else
                            ptr_ = n;

                        ret++;
                        owner_->destroy_node_(p);
                    }
                    else
                        break;
                }

                return ret;
            }

        private:

            shared_list * owner_;
            node * ptr_;
            node * tail_;
        };

        std::atomic<node *>  head_;
        node * tail_;

        garbage garbage_;

        Alloc alloc_;
        AllocNode allocnode_;
    };
}

namespace std
{
    template <typename T>
    void swap(more::shared_list<T> &lsh, more::shared_list<T> &rhs)
    {
        lsh.swap(rhs);
    }
}

#endif /* __SHARED_LIST_HPP__ */
