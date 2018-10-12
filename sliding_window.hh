#ifndef YMD_SLIDING_WINDOW_HH
#define YMD_SLIDING_WINDOW_HH 1

#include "range_view.hh"

namespace ymd {

  namespace detail {
    template<typename T> class sliding_view {
    private:
      T w_begin;
      T w_end;
      std::size_t step;

    public:
      using iterator_category = std::input_iterator_tag;
      using difference_type = ptrdiff_t;
      using value_type = range_view<T>;
      using pointer = value_type*;
      using reference = value_type&;

      sliding_view() = default;
      sliding_view(const sliding_view&) = default;
      sliding_view(sliding_view&&) = default;
      sliding_view(T w_begin,T w_end,std::size_t step)
	: w_begin(w_begin), w_end(w_end), step(step) {}
      sliding_view& operator=(const sliding_view&) = default;
      sliding_view& operator=(sliding_view&&) = default;
      ~sliding_view() = default;

      auto& operator++(){
	std::advance(w_begin,step);
	std::advance(w_end,step);
	return *this;
      }
      auto operator++(int){ auto copy{*this}; ++this; return copy; }
      auto operator--(){
	std::advance(w_begin,-step);
	std::advance(w_end,-step);
      }
      auto operator--(int){ auto copy{*this}; --this; return copy; }
      auto operator*(){
	return ymd::range_view<T>{w_begin,w_end};
      }

      friend inline bool operator<(const sliding_view& lhs,
      				   const sliding_view& rhs){
	return lhs.w_begin < rhs.w_begin;
      }
      friend inline bool operator>(const sliding_view& lhs,
				   const sliding_view& rhs){
	return rhs < lhs;
      }
      friend inline bool operator==(const sliding_view& lhs,
				    const sliding_view& rhs){
	return lhs.w_begin == rhs.w_begin;
      }
      friend inline bool operator!=(const sliding_view& lhs,
				    const sliding_view& rhs){
	return !(lhs == rhs);
      }
      friend inline bool operator<=(const sliding_view& lhs,
				    const sliding_view& rhs){
	return (lhs < rhs) || (lhs == rhs);
      }
      friend inline bool operator>=(const sliding_view& lhs,
				    const sliding_view& rhs){
	return (lhs > rhs) || (lhs == rhs);
      }

      auto begin() const { return w_begin; }
      auto   end() const { return   w_end; }
    };

    template<typename T> class sliding_window {
    private:
      sliding_view<T> s_begin;
      sliding_view<T> s_end;

    public:
      sliding_window() = default;
      sliding_window(const sliding_window&) = default;
      sliding_window(sliding_window&&) = default;
      sliding_window(T begin, T end, std::size_t size, std::size_t step)
	: s_begin{begin,std::next(begin,size),step} {
	auto times = std::size_t(std::distance(std::next(begin,size),end)) / step;
	s_end = std::next(s_begin,times+1);
      }
      sliding_window& operator=(const sliding_window&) = default;
      sliding_window& operator=(sliding_window&&) = default;
      ~sliding_window() = default;

      auto begin(){ return s_begin; }
      auto   end(){ return   s_end; }
    };
  }

  template<typename T> inline auto sliding_window(T&& v,
						  std::size_t window_size,
						  std::size_t sliding_step){
    using std::begin;
    using std::end;

    return detail::sliding_window{begin(v),end(v),window_size,sliding_step};
  }

  namespace adaptor {
    class sliding_window {
    private:
      std::size_t window_size;
      std::size_t sliding_step;
    public:
      sliding_window() = default;
      sliding_window(const sliding_window&) = default;
      sliding_window(sliding_window&&) = default;
      sliding_window(std::size_t window_size,std::size_t sliding_step)
	: window_size(window_size), sliding_step(sliding_step) {}
      sliding_window& operator=(const sliding_window&) = default;
      sliding_window& operator=(sliding_window&&) = default;
      ~sliding_window() = default;
      template<typename T> friend inline auto operator|(T&& v,
							sliding_window&& sw){
	return ymd::sliding_window(v,sw.window_size,sw.sliding_step);
      }
    };
  }
} // namespace ymd
#endif // YMD_SLIDING_WINDOW





