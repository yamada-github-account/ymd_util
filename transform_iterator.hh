#ifndef YMD_TRANSFORM_ITERATOR_HH
#define YMD_TRANSFORM_ITERATOR_HH 1

#include <iterator>
#include <type_traits>
#include <functional>

namespace ymd {
  template<typename Iterator,typename F> class transform_iterator {
  private:
    Iterator it;
    F f;
  public:
    using iterator_category = std::input_iterator_tag;
    using difference_type = ptrdiff_t;
    using value_type = decltype(f(std::declval<typename Iterator::value_type>()));
    using pointer = value_type*;
    using reference = value_type&;

    transform_iterator() = default;
    transform_iterator(const transform_iterator&) = default;
    transform_iterator(transform_iterator&&) = default;
    transform_iterator(Iterator it,F f) : it(it), f(f) {}
    transform_iterator& operator=(const transform_iterator&) = default;
    transform_iterator& operator=(transform_iterator&&) = default;
    ~transform_iterator() = default;

    auto& operator++(){ ++it; return *this; }
    auto operator++(int){ auto copy{*this}; ++(*this); return copy; }
    auto& operator--(){ --it; return *this; }
    auto operator--(int){ auto copy{*this}; --(*this); return copy; }

    auto operator*(){ return f(*it); }

    friend inline bool operator<(const transform_iterator<Iterator,F>& lhs,
				 const transform_iterator<Iterator,F>& rhs){
      return lhs.it < rhs.it;
    }
    friend inline bool operator>(const transform_iterator<Iterator,F>& lhs,
				 const transform_iterator<Iterator,F>& rhs){
      return rhs < lhs;
    }
    friend inline bool operator==(const transform_iterator<Iterator,F>& lhs,
				  const transform_iterator<Iterator,F>& rhs){
      return lhs.it == rhs.it;
    }
    friend inline bool operator!=(const transform_iterator<Iterator,F>& lhs,
				  const transform_iterator<Iterator,F>& rhs){
      return !(lhs == rhs);
    }
    friend inline bool operator<=(const transform_iterator<Iterator,F>& lhs,
				  const transform_iterator<Iterator,F>& rhs){
      return (lhs < rhs) || (lhs == rhs);
    }
    friend inline bool operator>=(const transform_iterator<Iterator,F>& lhs,
				  const transform_iterator<Iterator,F>& rhs){
      return (lhs > rhs) || (lhs == rhs);
    }
  };

  namespace detail {
    template<typename Iterator,typename F> class transform {
    private:
      Iterator v_begin;
      Iterator v_end;
      F f;
    public:
      transform() = default;
      transform(const transform&) = default;
      transform(transform&&) = default;
      transform(Iterator begin,Iterator end,F f)
	: v_begin(begin),v_end(end), f(f) {}
      transform& operator=(const transform&) = default;
      transform& operator=(transform&&) = default;
      ~transform() = default;

      auto begin() const { return ymd::transform_iterator{v_begin,f}; }
      auto end() const { return ymd::transform_iterator{v_end,f}; }
      std::size_t size() const { return std::distance(v_begin,v_end); }
    };
  } // namespace detail

  template<typename Container,typename F>
  inline auto transform(Container&& container,F&& f){
    using std::begin;
    using std::end;
    return detail::transform{begin(container),end(container),std::forward<F>(f)};
  }

  namespace adaptor {
    template<typename F> class transform {
    private:
      F f;
    public:
      transform() = default;
      transform(const transform&) = default;
      transform(transform&&) = default;
      transform(F f) : f(f) {}
      transform& operator=(const transform&) = default;
      transform& operator=(transform&&) = default;
      ~transform() = default;
      template<typename Container> friend inline auto operator|(Container&& container,
								transform<F>&& t){
	return ymd::transform(std::forward<Container>(container),t.f);
      }
    }; // class transform
  } // namespace adaptor
} // namespace ymd
#endif // YMD_TRANSFORM_ITERATOR_HH
