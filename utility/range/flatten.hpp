#pragma once

#include <boost/range/iterator_range.hpp>
#include <boost/range/concepts.hpp>

#include "../iterator/flattening_iterator.hpp"

namespace utility
{
    namespace range_detail
    {
        struct flatten_forwarder { };

        template< class R, class I >
        struct flattened_range : boost::iterator_range<utility::const_flattening_iterator<I>>
        {
        private:
            using base = boost::iterator_range<utility::const_flattening_iterator<I>>;

        public:
            flattened_range(R& r)
                : base(
                utility::make_const_flattening_iterator(std::begin(r), std::end(r)),
                utility::make_const_flattening_iterator(std::end(r), std::end(r)))
            {}
        };

        template< class SinglePassRange >
        inline flattened_range<SinglePassRange, typename SinglePassRange::iterator> 
            operator|(SinglePassRange& r, flatten_forwarder)
        {
            BOOST_RANGE_CONCEPT_ASSERT((boost::SinglePassRangeConcept<SinglePassRange>));
            return flattened_range<SinglePassRange, typename SinglePassRange::iterator>(r);
        }

        template< class SinglePassRange >
        inline flattened_range<const SinglePassRange, typename SinglePassRange::const_iterator> 
            operator|(const SinglePassRange& r, flatten_forwarder)
        {
            BOOST_RANGE_CONCEPT_ASSERT((boost::SinglePassRangeConcept<const SinglePassRange>));
            return flattened_range<const SinglePassRange, typename SinglePassRange::const_iterator>(r);
        }
    }

    using range_detail::flattened_range;

    namespace adaptors
    {
        namespace
        {
            const range_detail::flatten_forwarder flattened = range_detail::flatten_forwarder();
        }

        template<class SinglePassRange>
        inline flattened_range<SinglePassRange, typename SinglePassRange::iterator>
            flatten(SinglePassRange& rng)
        {
            BOOST_RANGE_CONCEPT_ASSERT((boost::SinglePassRangeConcept<SinglePassRange>));
            return flattened_range<SinglePassRange, typename SinglePassRange::iterator>(rng);
        }

        template<class SinglePassRange>
        inline flattened_range<const SinglePassRange, typename SinglePassRange::const_iterator>
            flatten(const SinglePassRange& rng)
        {
            BOOST_RANGE_CONCEPT_ASSERT((boost::SinglePassRangeConcept<const SinglePassRange>));
            return flattened_range<const SinglePassRange, typename SinglePassRange::const_iterator>(rng);
        }
    }
}