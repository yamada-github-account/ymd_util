#ifndef YMD_INDEX_ITERATOR_HH
#define YMD_INDEX_ITERATOR_HH 1

#include <iterator>
#include <random>
#include <algorithm>
#include <utility>
#include <vector>
#include <cmath>

namespace ymd {

  namespace detail {
    template<typename ValueIterator,typename IndexIterator> class index_iterator {
    private:
      ValueIterator v_it;
      IndexIterator i_it;
    public:
      using iterator_category = std::input_iterator_tag;
      using difference_type = ptrdiff_t;
      using value_type = typename ValueIterator::value_type;
      using pointer = typename ValueIterator::pointer;
      using reference = typename ValueIterator::reference;

      index_iterator() = default;
      index_iterator(const index_iterator&) = default;
      index_iterator(index_iterator&&) = default;
      index_iterator(ValueIterator v_begin,IndexIterator i_begin)
	: v_it(v_begin), i_it(i_begin) {}
      index_iterator& operator=(const index_iterator&) = default;
      index_iterator& operator=(index_iterator&&) = default;
      ~index_iterator() = default;

      auto& operator++(){ ++i_it; return *this; }
      auto operator++(int){ auto copy{*this}; ++(*this); return copy; }
      auto& operator--(){ --i_it; return *this; }
      auto operator--(int){ auto copy{*this}; --(*this); return copy; }

      auto operator*(){ return *std::next(v_it,*i_it); }

      friend inline
      auto operator<(const index_iterator<ValueIterator,IndexIterator>& lhs,
		     const index_iterator<ValueIterator,IndexIterator>& rhs){
	return lhs.i_it < rhs.i_it;
      }
      friend inline
      auto operator>(const index_iterator<ValueIterator,IndexIterator>& lhs,
		     const index_iterator<ValueIterator,IndexIterator>& rhs){
	return rhs < lhs;
      }
      friend inline
      auto operator==(const index_iterator<ValueIterator,IndexIterator>& lhs,
		      const index_iterator<ValueIterator,IndexIterator>& rhs){
	return lhs.i_it == rhs.i_it;
      }
      friend inline
      auto operator!=(const index_iterator<ValueIterator,IndexIterator>& lhs,
		      const index_iterator<ValueIterator,IndexIterator>& rhs){
	return !(lhs == rhs);
      }
      friend inline
      auto operator<=(const index_iterator<ValueIterator,IndexIterator>& lhs,
		      const index_iterator<ValueIterator,IndexIterator>& rhs){
	return (lhs < rhs) || (lhs == rhs);
      }
      friend inline
      auto operator>=(const index_iterator<ValueIterator,IndexIterator>& lhs,
		      const index_iterator<ValueIterator,IndexIterator>& rhs){
	return (lhs > rhs) || (lhs == rhs);
      }

    }; // index_iterator

    template<typename Iterator,typename Indexes> class index_view {
    private:
      Iterator v_begin;
      Indexes indexes;
    public:
      index_view() = default;
      index_view(const index_view&) = default;
      index_view(index_view&&) = default;
      index_view(Iterator begin,Indexes indexes): v_begin(begin),indexes(indexes) {}
      index_view& operator=(const index_view&) = default;
      index_view& operator=(index_view&&) = default;
      ~index_view() = default;

      auto begin(){
	using std::begin;
	return index_iterator{v_begin,begin(indexes)};
      }
      auto end(){
	using std::end;
	return index_iterator{v_begin,end(indexes)};
      }
    }; // index_view
  } // namespace detail

  template<typename Container,typename Indexes>
  inline auto index_view(Container&& container,const Indexes& indexes){
    using std::begin;

    return detail::index_view{begin(container),indexes};
  }

  template<typename Container,typename URNG = decltype(std::mt19937{})>
  inline auto shuffle_view(Container&& container,
			   URNG g = std::mt19937{std::random_device{}()}){
    using std::begin;
    using std::end;

    std::size_t size = std::distance(begin(container),end(container));

    std::vector<std::size_t> indexes{};
    indexes.reserve(size);

    std::generate_n(std::back_inserter(indexes),size,
		    [v = 0ul]()mutable{ return v++; });

    std::shuffle(indexes.begin(),indexes.end(),g);
    return index_view(std::forward<Container>(container),indexes);
  }

  template<typename Container>
  inline auto slice_view(Container&& container,std::size_t i_begin,std::size_t i_end,
			 std::size_t i_step = 1ul){
    using std::begin;
    using std::end;

    i_end = std::min(i_end,std::size_t(begin(container),end(container)));

    std::vector<std::size_t> indexes{};
    indexes.reserve((i_end - i_begin)/(i_step));

    for(auto i_it  = i_begin; i_it < i_end; i_it += i_step){
      indexes.push_back(i_it);
    }

    return index_view(std::forward<Container>(container),indexes);
  }

  namespace adaptor {
    template<typename Indexes> class index_view {
    private:
      Indexes indexes;
    public:
      index_view() = default;
      index_view(const index_view&) = default;
      index_view(index_view&&) = default;
      index_view(Indexes indexes): indexes(indexes) {}
      index_view& operator=(const index_view&) = default;
      index_view& operator=(index_view&&) = default;
      ~index_view() = default;
      template<typename Container>
      friend inline auto operator|(Container&& container,
				   index_view<Indexes>&& iv){
	return ymd::index_view(container,iv.indexes);
      }
    };

    template<typename URNG = decltype(std::mt19937{})> class shuffle_view {
    private:
      URNG g;
    public:
      shuffle_view(const shuffle_view&) = default;
      shuffle_view(shuffle_view&&) = default;
      shuffle_view(URNG g = std::mt19937{std::random_device{}()}): g(g) {}
      shuffle_view& operator=(const shuffle_view&) = default;
      shuffle_view& operator=(shuffle_view&&) = default;
      ~shuffle_view() = default;
      template<typename Container>
      friend inline auto operator|(Container&& container,
				   shuffle_view<URNG>&& sv){
	return ymd::shuffle_view(container,sv.g);
      }
    };

    class slice_view {
    private:
      std::size_t i_begin;
      std::size_t i_end;
      std::size_t i_step;
    public:
      slice_view() = default;
      slice_view(const slice_view&) = default;
      slice_view(slice_view&&) = default;
      slice_view(std::size_t begin,std::size_t end,std::size_t step = 1ul)
	: i_begin(begin), i_end(end), i_step(step) {}
      slice_view& operator=(const slice_view&) = default;
      slice_view& operator=(slice_view&&) = default;
      ~slice_view() = default;
      template<typename Container> friend inline auto operator|(Container&& container,
								slice_view&& sv){
	return ymd::slice_view(container,sv.i_begin,sv.i_end,sv.i_step);
      }
    };
  }
} // namespace ymd
#endif // YMD_INDEX_ITERATOR
