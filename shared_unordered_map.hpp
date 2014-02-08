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

#ifndef __SHARED_UNORDERED_MAP_HPP__
#define __SHARED_UNORDERED_MAP_HPP__

#include <functional>
#include <utility>
#include <memory>
#include <initializer_list>
#include <vector>
#include <atomic>
#include <tuple>

#include <shared_list.hpp>

namespace more
{
    template <typename Key,
              typename T,
              typename Time  = TimeStampCounter,
              typename Hash  = std::hash<Key>,
              typename Pred  = std::equal_to<Key>,
              typename Alloc = std::allocator<std::pair<const Key, T>>>
    class shared_unordered_map
    {

    public:
        typedef Key                         key_type;
        typedef T                           mapped_type;
        typedef std::pair<const Key, T>     value_type;

        typedef size_t                      size_type;
        typedef ptrdiff_t                   difference_type;

        typedef Hash                        hasher;
        typedef Pred                        key_equal;
        typedef Alloc                       allocator_type;

        typedef value_type&                 reference;
        typedef const value_type&           const_reference;

        typedef typename std::allocator_traits<Alloc>::pointer           pointer;
        typedef typename std::allocator_traits<Alloc>::const_pointer     const_pointer;

    private:
        typedef shared_list<value_type, Time, Alloc>    __list_type;

    public:
        typedef typename __list_type::iterator          local_iterator;
        typedef typename __list_type::const_iterator    const_local_iterator;

        struct _iterator : std::iterator<std::forward_iterator_tag, value_type>
        {
            explicit _iterator(std::vector<__list_type> *bucket)
            : bucket_(bucket)
            , lit_()
            , index_(-1)
            {}

            _iterator(std::vector<__list_type> *bucket, local_iterator it, size_type index)
            : bucket_(bucket)
            , lit_(it)
            , index_(index)
            {}

            reference
            operator*() const
            {
                return *lit_;
            }

            pointer
            operator->() const
            {
                return &(*lit_);
            }

            _iterator &
            operator++()
            {
                if (*this != _iterator(bucket_))
                    if (++lit_ != (*bucket_)[index_].end())
                        return *this;
                do {
                    index_++;
                }
                while(index_ < bucket_->size() &&
                        ((lit_ = (*bucket_)[index_].begin()), lit_ == (*bucket_)[index_].end()));

                if (index_ == bucket_->size())
                    *this = _iterator(bucket_);

                return *this;
            }

            _iterator
            operator++(int)
            {
                auto self = *this;
                ++(*this);
                return self;
            }

            bool
            operator==(const _iterator &it)
            {
                return lit_ == it.lit_;
            }

            bool
            operator!=(const _iterator &it)
            {
                return lit_ != it.lit_;
            }

            std::vector<__list_type> * bucket_;
            local_iterator lit_;
            size_type index_;
        };


        struct _const_iterator : std::iterator<std::forward_iterator_tag, const value_type>
        {
            explicit _const_iterator(std::vector<__list_type> const *bucket)
            : bucket_(bucket)
            , lit_()
            , index_(-1)
            {}

            _const_iterator(std::vector<__list_type> const *bucket, const_local_iterator it, size_type index)
            : bucket_(bucket)
            , lit_(it)
            , index_(index)
            {}

            _const_iterator(_iterator const &it)
            : bucket_(it.bucket_)
            , lit_(it.lit_)
            , index_(it.index_)
            {

            }

            const_reference
            operator*() const
            {
                return *lit_;
            }

            const_pointer
            operator->() const
            {
                return &(*lit_);
            }

            _const_iterator &
            operator++()
            {
                if (*this != _const_iterator(bucket_))
                    if (++lit_ != (*bucket_)[index_].end())
                        return *this;
                do {
                    index_++;
                }
                while(index_ < bucket_->size() &&
                        ((lit_ = (*bucket_)[index_].begin()), lit_ == (*bucket_)[index_].end()));

                if (index_ == bucket_->size())
                    *this = _const_iterator(bucket_);

                return *this;
            }

            _const_iterator
            operator++(int)
            {
                auto self = *this;
                ++(*this);
                return self;
            }

            bool
            operator==(const _const_iterator &it)
            {
                return lit_ == it.lit_;
            }

            bool
            operator!=(const _const_iterator &it)
            {
                return lit_ != it.lit_;
            }


            std::vector<__list_type> const * bucket_;
            const_local_iterator lit_;
            size_type index_;
        };

    public:

        typedef _iterator                               iterator;
        typedef _const_iterator                         const_iterator;

    public:

        explicit shared_unordered_map(size_type bucket = 1021,
                            const Hash &hash   = Hash(),
                            const Pred &pred   = Pred(),
                            const Alloc &alloc = Alloc())
        : bucket_(bucket)
        , hash_(hash)
        , equal_(pred)
        , size_(0)
        {

        }

        template <typename Input>
        explicit shared_unordered_map(Input beg, Input end,
                            size_type bucket  = 1021,
                            const Hash &hash  = Hash(),
                            const Pred &pred  = Pred(),
                            const Alloc &alloc = Alloc())
        : bucket_(bucket)
        , hash_(hash)
        , equal_(pred)
        {

            for(; beg != end; ++beg)
            {
                insert_(*beg);
            }

            size_.store(total_size_(), std::memory_order_release);
        }

        shared_unordered_map(const shared_unordered_map& other)
        : bucket_(other.bucket_.size())
        , hash_(other.hash_)
        , equal_(other.equal_)
        {
            for(auto &value : other)
                insert_(value);

            size_.store(total_size_(), std::memory_order_release);
        }

        shared_unordered_map(const shared_unordered_map& other, const Alloc &alloc)
        : bucket_(other.bucket_.size())
        , hash_(other.hash_)
        , equal_(other.equal_)
        {
            for(auto &value : other)
                insert_(value);

            size_.store(total_size_(), std::memory_order_release);
        }

        // No observers are allowed while move-constructing
        //

        shared_unordered_map(shared_unordered_map&& other)
        : bucket_(std::move(other.bucket_))
        , hash_(std::move(other.hash_))
        , equal_(std::move(other.equal_))
        {
            size_.store(total_size_(), std::memory_order_release);
        }


        shared_unordered_map(std::initializer_list<value_type> init,
                            size_type bucket  = 1021,
                            const Hash &hash  = Hash(),
                            const Pred &pred  = Pred(),
                            const Alloc &alloc = Alloc())
        : bucket_(bucket)
        , hash_(hash)
        , equal_(pred)
        {
            for(auto &value : init)
                insert_(value);

            size_.store(total_size_(), std::memory_order_release);
        }

        shared_unordered_map& operator=(shared_unordered_map const & other)
        {
            bucket_ = other.bucket_;
            hash_   = other.hash_;
            equal_   = other.equal_;
            size_.store(total_size_(), std::memory_order_release);
            return *this;
        }

        // No observers are allowed while move-assinging
        //

        shared_unordered_map& operator=(shared_unordered_map&& other)
        {
            bucket_ = std::move(other.bucket_);
            hash_   = std::move(other.hash_);
            equal_   = std::move(other.equal_);
            size_.store(total_size_(), std::memory_order_release);
            return *this;
        }

        ~shared_unordered_map()
        {
            /* enable parallel distruction */
            for(auto &buc : bucket_)
                buc.clear();
        }


        // size and capacity

        bool empty() const noexcept
        {
            return size_.load(std::memory_order_relaxed) == 0;
        }

        size_type size() noexcept
        {
            return size_.load(std::memory_order_relaxed);
        }

        size_type max_size() const noexcept
        {
            return Alloc().max_size();
        }

        size_type maybe_size() const noexcept
        {
            return size_.load(std::memory_order_relaxed);
        }

        // iterator

        iterator
        begin() noexcept
        {
            return ++iterator(&bucket_);
        }

        const_iterator
        begin() const noexcept
        {
            return ++const_iterator(&bucket_);
        }

        iterator
        end() noexcept
        {
            return iterator(&bucket_);
        }

        const_iterator
        end() const noexcept
        {
            return const_iterator(&bucket_);
        }

        const_iterator
        cbegin() const noexcept
        {
            return ++const_iterator(&bucket_);
        }

        const_iterator
        cend() const noexcept
        {
            return const_iterator(&bucket_);
        }

        // modifiers

        std::pair<iterator, bool>
        insert(const value_type &object)
        {
            auto r = insert_(object);
            if (r.second)
                size_.fetch_add(1, std::memory_order_relaxed);

            return std::make_pair(iterator(&bucket_, std::get<0>(r), std::get<1>(r)), std::get<2>(r));
        }

        template <typename Tx>
        std::pair<iterator, bool>
        insert(Tx && object)
        {
            auto r = insert_(std::move(object));
            if (std::get<2>(r))
                size_.fetch_add(1, std::memory_order_relaxed);
            return std::make_pair(iterator(&bucket_, std::get<0>(r), std::get<1>(r)), std::get<2>(r));
        }

        template <typename Iter>
        void
        insert(Iter first, Iter last)
        {
            for(; first != last; ++first) {
                auto r = insert_(*first);
                if (std::get<2>(r))
                    size_.fetch_add(1, std::memory_order_relaxed);
            }
        }

        void insert(std::initializer_list<value_type> init)
        {
            for(auto &value : init) {
                auto r = insert_(value);
                if (std::get<2>(r))
                    size_.fetch_add(1, std::memory_order_relaxed);
            }
        }

        iterator erase(const_iterator position)
        {
            size_.fetch_sub(1, std::memory_order_relaxed);

            auto buc = & const_cast<__list_type &>((*position.bucket_)[position.index_]);

            auto lit = buc->erase(position.lit_);
            if (lit != buc->end())
                return iterator(const_cast<std::vector<__list_type> *>(position.bucket_), lit, position.index_);

            auto index = position.index_;

            do {
                index++, buc++;
            }
            while(index < bucket_.size() &&
                  ((lit = buc->begin(), lit == buc->end())));

            if (index == bucket_.size())
                return iterator(&bucket_);
            else
                return iterator(&bucket_, lit, index);
        }

        size_type erase(const key_type& k)
        {
            auto p = find_(k);
            if (!std::get<2>(p))
                return 0;

            size_.fetch_sub(1, std::memory_order_relaxed);
            bucket_[std::get<1>(p)].erase(std::get<0>(p));
            return 1;
        }


        iterator erase(const_iterator first, const_iterator last)
        {
            iterator it = find(first->first);
            do
            {
                it = erase(it);
            }
            while (const_iterator(it) != last);
            return it;
        }

        void clear() noexcept
        {
            size_.store(0, std::memory_order_relaxed);
            for(auto &l : bucket_)
                l.clear();
        }

        void shrink() noexcept
        {
            for(auto &l : bucket_)
                l.shrink();
        }

        // No observers are allowed while swapping
        //

        void swap(shared_unordered_map& other)
        {
            bucket_.swap(other.bucket_);
            std::swap(hash_,other.hash_);
            std::swap(equal_,other.equal_);

            auto s = size_.load(std::memory_order_relaxed);
            size_.store(other.size_.load(std::memory_order_relaxed), std::memory_order_relaxed);
            other.size_.store(s, std::memory_order_relaxed);
        }

        // observers

        hasher hash_function() const
        {
            return hash_;
        }

        key_equal key_eq() const
        {
            return equal_;
        }

        // lookup

        iterator
        find(const key_type& k)
        {
            auto p = find_(k);
            return iterator(&bucket_, std::get<0>(p), std::get<1>(p));
        }

        const_iterator
        find(const key_type& k) const
        {
            auto p = find_(k);
            return const_iterator(&bucket_, std::get<0>(p), std::get<1>(p));
        }

        size_type count(const key_type& k) const
        {
            return std::get<2>(find_(k)) ? 1 : 0;
        }

        std::pair<iterator, iterator>
        equal_range(const key_type& k)
        {
            auto p = find_(k);
            if (!std::get<2>(p))
                return std::make_pair(iterator(&bucket_), iterator(&bucket_));

            auto it = iterator(&bucket_, std::get<0>(p), std::get<1>(p));
            return std::make_pair(it, std::next(it));
        }

        std::pair<const_iterator, const_iterator>
        equal_range(const key_type& k) const
        {
            auto p = find_(k);
            if (!std::get<2>(p))
                return std::make_pair(iterator(&bucket_), iterator(&bucket_));

            auto it = const_iterator(&bucket_, std::get<0>(p), std::get<1>(p));
            return std::make_pair(it, std::next(it));
        }

        mapped_type&
        operator[](const key_type& k)
        {
            auto p = find_(k);
            if (!std::get<2>(p))
            {
                auto &l = bucket_[std::get<1>(p)];
                l.emplace_front(k, mapped_type());
                std::get<0>(p) = l.begin();

                size_.fetch_add(1, std::memory_order_relaxed);
            }
            return std::get<0>(p)->second;
        }

        mapped_type&
        operator[](key_type&& k)
        {
            auto p = find_(k);
            if (!std::get<2>(p))
            {
                auto &l = bucket_[std::get<1>(p)];
                l.emplace_front(std::move(k), mapped_type());
                std::get<0>(p) = l.begin();

                size_.fetch_add(1, std::memory_order_relaxed);
            }
            return std::get<0>(p)->second;
        }

        mapped_type&
        at(const key_type& k)
        {
            auto p = find_(k);
            if (std::get<2>(p))
                return std::get<0>(p)->second;

            throw std::out_of_range("shared_unordered_map");
        }

        const mapped_type&
        at(const key_type& k) const
        {
            auto p = find_(k);
            if (std::get<2>(p))
                return std::get<0>(p)->second;

            throw std::out_of_range("shared_unordered_map");
        }

        // bucket interface

        size_type bucket_count() const noexcept
        {
            return bucket_.size();
        }

        size_type max_bucket_count() const noexcept
        {
            return max_size();
        }

        size_type bucket_size(size_type n) const
        {
            return std::distance(begin(n), end(n));
        }

        size_type bucket(const key_type& k) const
        {
            return hash_(k) % bucket_count();
        }

        local_iterator begin(size_type n)
        {
            return bucket_[n].begin();
        }

        const_local_iterator begin(size_type n) const
        {
            return bucket_[n].begin();
        }

        local_iterator end(size_type n)
        {
            return bucket_[n].end();
        }

        const_local_iterator end(size_type n) const
        {
            return bucket_[n].end();
        }

        const_local_iterator cbegin(size_type n) const
        {
            return bucket_[n].cbegin();
        }

        const_local_iterator cend(size_type n) const
        {
            return bucket_[n].cend();
        }

        float
        load_factor() const noexcept
        {
            return static_cast<float>(maybe_size())/ static_cast<float>(bucket_count());
        }

        void dump() const
        {
            auto n = 0;
            for(auto const &l : bucket_)
            {
                std::cout << "[" << n << "] => ";

                for(auto const &e : l)
                {
                    std::cout << "(" << e.first << "," << e.second << ") " << std::flush;
                }

                n++;

                std::cout << std::endl;
            }
        }

    private:

        template <typename Tp>
        std::tuple<local_iterator, size_type, bool>
        insert_(Tp && value)
        {
            auto index = bucket(value.first);
            auto &buc = bucket_.at(index);

            for(auto it = buc.begin(); it != buc.end(); ++it)
            {
                if (it->first == value.first)
                {
                    return std::make_tuple(it, index, false);
                }
            }

            buc.push_front(std::forward<Tp>(value));

            return std::make_tuple(buc.begin(), index, true);
        }


        std::tuple<local_iterator, size_type , bool>
        find_(const key_type &k)
        {
            auto index = bucket(k);
            auto & buc = bucket_.at(index);

            for(auto it = buc.begin(); it != buc.end(); ++it)
            {
                if (it->first == k)
                    return std::make_tuple(it, index, true);
            }

            return std::make_tuple(local_iterator(), index, false);
        }


        std::tuple<const_local_iterator, size_type, bool>
        find_(const key_type &k) const
        {
            auto index = bucket(k);
            auto & buc = bucket_.at(index);

            for(auto it = buc.begin(); it != buc.end(); ++it)
            {
                if (it->first == k)
                    return std::make_tuple(it, index, true);
            }

            return std::make_tuple(const_local_iterator(), index, false);
        }

        size_type total_size_() noexcept
        {
            size_type n = 0;
            for(auto & b : bucket_)
                n += b.size();
            return n;
        }

        std::vector<__list_type, typename Alloc::template rebind<__list_type>::other> bucket_;

        Hash hash_;
        Pred equal_;

        std::atomic<size_type>  size_;

    };

    template <typename Key,
              typename T,
              typename Time,
              typename Hash,
              typename Pred,
              typename Alloc>
    bool operator==(shared_unordered_map<Key,T,Time,Hash,Pred,Alloc> const &lhs,
                    shared_unordered_map<Key,T,Time,Hash,Pred,Alloc> const &rhs)
    {
        if (lhs.maybe_size() != rhs.maybe_size())
            return false;

        for(auto & l : lhs)
        {
            auto it = rhs.find(l.first);
            if (it == rhs.end())
                return false;
            if (it->second != l.second)
                return false;
        }

        return true;
    }

    template <typename Key,
              typename T,
              typename Time,
              typename Hash,
              typename Pred,
              typename Alloc>
    bool operator!=(shared_unordered_map<Key,T,Time,Hash,Pred,Alloc> const &lhs,
                    shared_unordered_map<Key,T,Time,Hash,Pred,Alloc> const &rhs)
    {
        return !(lhs == std::move(rhs));
    }

}

#endif /* __SHARED_MAP_HPP__ */
