/**
 \file flattening_iterator.hpp
 \memo 参考: http://stackoverflow.com/questions/3623082/flattening-iterator
 */

#pragma once
#include <boost/iterator/iterator_facade.hpp>

namespace utility 
{
    namespace detail
    {
        template<typename OuterIterator, typename InnerIteraor, bool IsOuterIteratorReference>
        class flattening_iterator_base;

        // OuterIterator の指す要素の参照が取れる場合
        template<
            typename OuterIterator,
            typename InnerIterator>
        class flattening_iterator_base<OuterIterator, InnerIterator, true> : public boost::iterator_facade<
            flattening_iterator_base<OuterIterator, InnerIterator, true>,
            typename InnerIterator::value_type,
            boost::forward_traversal_tag,
            typename InnerIterator::reference> 
        {
            using mytype = flattening_iterator_base<OuterIterator, InnerIterator, true>;
            using inner_iterator_reference = typename InnerIterator::reference;
        public:
            flattening_iterator_base() {}
            explicit flattening_iterator_base(OuterIterator it_end): _outer_it(it_end), _outer_end(it_end) {}
            flattening_iterator_base(OuterIterator it, OuterIterator it_end) : _outer_it(it), _outer_end(it_end)
            {
                using std::begin;
                using std::end;

                if (_outer_it == _outer_end) return;

                _inner_it = begin(*_outer_it);
                _inner_end = end(*_outer_it);
                advance_past_empty_inner_containers();
            }

        private:
            friend class boost::iterator_core_access;

            void increment() 
            {
                ++_inner_it;
                if (_inner_it == _inner_end) advance_past_empty_inner_containers();
            }

            inner_iterator_reference dereference() const { return *_inner_it; }

            bool equal(const mytype& other) const
            {
                // this と other が非終端で一致している
                // 1. 外側のイテレータが一致 (_outer_it == other._outer_it)
                // 2. 外側のイテレータが終端でない (_outer_it != _outer_end, other._outer_it != other._outer_end)
                // 3. 内側のイテレータが一致 (_inner_it == other._inner_it)
                // this と oterh が終端で一致している
                // a. 外側のイテレータが一致 (_outer_it == other._outer_it)
                // b. 外側のイテレータが終端 (_outer_it == _outer_end, other._outer_it == other._outer_it)
                if (_outer_it != other._outer_it)  // 1. a.
                    return false;  

                if (_outer_it != _outer_end &&  // 2.
                    other._outer_it != other._outer_end &&  // 2.
                    _inner_it == other._inner_it) // 3.
                    return true;

                if (_outer_it == _outer_end && other._outer_it == other._outer_end) // b.
                    return true;

                return false;
            }

            void advance_past_empty_inner_containers()
            {
                using std::begin;
                using std::end;

                while (_outer_it != _outer_end && _inner_it == _inner_end)
                {
                    ++_outer_it;
                    if (_outer_it != _outer_end)
                    {
                        _inner_it = begin(*_outer_it);
                        _inner_end = end(*_outer_it);
                    }
                }
            }

            OuterIterator _outer_it;
            OuterIterator _outer_end;
            InnerIterator _inner_it;
            InnerIterator _inner_end;
        };

        // OuterIterator の指す要素の参照が取れない場合 (transformed adaptor などでネストした range を返す場合など)
        template<
            typename OuterIterator,
            typename InnerIterator>
        class flattening_iterator_base<OuterIterator, InnerIterator, false> : public boost::iterator_facade<
            flattening_iterator_base<OuterIterator, InnerIterator, false>,
            typename InnerIterator::value_type,
            boost::forward_traversal_tag,
            typename InnerIterator::reference>
        {
            using mytype = flattening_iterator_base<OuterIterator, InnerIterator, false>;
            using inner_iterator_reference = typename InnerIterator::reference;
            using inner_value_table = std::map<OuterIterator, typename OuterIterator::value_type>;
        public:
            flattening_iterator_base() {}
            flattening_iterator_base(OuterIterator it_end) : _outer_it(it_end), _outer_end(it_end) {}
            flattening_iterator_base(OuterIterator it, OuterIterator it_end) : 
                _outer_it(it), _outer_end(it_end), _table(std::make_shared<inner_value_table>())
            {
                using std::begin;
                using std::end;

                if (_outer_it == _outer_end) return;

                if (_table->find(_outer_it) == _table->end())
                {
                    (*_table)[_outer_it] = *_outer_it;
                }

                _inner_it = begin((*_table)[_outer_it]);
                _inner_end = end((*_table)[_outer_it]);                
                advance_past_empty_inner_containers();
            }

        private:
            friend class boost::iterator_core_access;

            void increment()
            {
                ++_inner_it;
                if (_inner_it == _inner_end) advance_past_empty_inner_containers();
            }

            inner_iterator_reference dereference() const { return *_inner_it; }

            bool equal(const mytype& other) const
            {
                // this と other が非終端で一致している
                // 1. 外側のイテレータが一致 (_outer_it == other._outer_it)
                // 2. 外側のイテレータが終端でない (_outer_it != _outer_end, other._outer_it != other._outer_end)
                // 3. 内側のイテレータが一致 (_inner_it == other._inner_it)
                // this と other が終端で一致している
                // a. 外側のイテレータが一致 (_outer_it == other._outer_it)
                // b. 外側のイテレータが終端 (_outer_it == _outer_end, other._outer_it == other._outer_it)
                if (_outer_it != other._outer_it)  // 1. a.
                    return false;

                if (_outer_it != _outer_end &&  // 2.
                    other._outer_it != other._outer_end &&  // 2.
                    _inner_it == other._inner_it) // 3.
                    return true;

                if (_outer_it == _outer_end && other._outer_it == other._outer_end) // b.
                    return true;

                return false;
            }

            void advance_past_empty_inner_containers()
            {
                using std::begin;
                using std::end;

                while (_outer_it != _outer_end && _inner_it == _inner_end)
                {
                    ++_outer_it;
                    if (_outer_it != _outer_end)
                    {
                        if (_table->find(_outer_it) == _table->end())
                        {
                            (*_table)[_outer_it] = *_outer_it;
                        }

                        _inner_it = begin((*_table)[_outer_it]);
                        _inner_end = end((*_table)[_outer_it]);
                    }
                }
            }

            OuterIterator _outer_it;
            OuterIterator _outer_end;            
            std::shared_ptr<inner_value_table> _table;
            InnerIterator _inner_it;
            InnerIterator _inner_end;
        };
    }

    template<typename ForwardIterator>
    using flattening_iterator = detail::flattening_iterator_base<
        ForwardIterator,
        typename ForwardIterator::value_type::iterator,
        std::is_reference<typename ForwardIterator::reference>::value>;
    template<typename ForwardIterator>
    using const_flattening_iterator = detail::flattening_iterator_base<
        ForwardIterator, 
        typename ForwardIterator::value_type::const_iterator,
        std::is_reference<typename ForwardIterator::reference>::value>;

    template<typename ForwardIterator>
    flattening_iterator<ForwardIterator> make_flattening_iterator(ForwardIterator it)
    {
        return flattening_iterator<ForwardIterator>(it);
    }

    template<typename ForwardIterator>
    flattening_iterator<ForwardIterator> make_flattening_iterator(ForwardIterator it, ForwardIterator it_end)
    {
        return flattening_iterator<ForwardIterator>(it, it_end);
    }

    template<typename ForwardIterator>
    const_flattening_iterator<ForwardIterator> make_const_flattening_iterator(ForwardIterator it)
    {
        return const_flattening_iterator<ForwardIterator>(it);
    }

    template<typename ForwardIterator>
    const_flattening_iterator<ForwardIterator> make_const_flattening_iterator(ForwardIterator it, ForwardIterator it_end)
    {
        return const_flattening_iterator<ForwardIterator>(it, it_end);
    }
}