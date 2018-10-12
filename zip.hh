#ifndef YMD_ZIP_HH
#define YMD_ZIP_HH 1

#include <iterator>
#include <tuple>
#include <algorithm>
#include <type_traits>

//  Requirement: c++17 (std::apply)
//  Preferable : c++2a (structured bindings)
//
//  Function   : template<typename...Types> auto ymd::zip(Types&&...v)
//               Arguments
//               ---------
//               Types&&...v: Containers and/or arrays iterated together.
//
//               Return
//               ------
//               auto: Range object whose iterator points to tuple of references of
//                     elements of v.
//
//  Usage       : for(auto&& [a,b] : ymd::zip(v1,v2)){
//                   ...
//                }
//

namespace ymd {
  namespace detail {
    template<typename...Types> class zip_iterator {
    private:
      std::tuple<Types...> iterator;

    public:
      using iterator_category = std::input_iterator_tag;
      using difference_type = ptrdiff_t;
      using value_type = decltype(std::tie((*std::declval<Types>())...));
      using pointer = std::add_pointer_t<value_type>;
      using reference = std::add_lvalue_reference_t<value_type>;

      zip_iterator() = default;
      zip_iterator(const zip_iterator&) = default;
      zip_iterator(zip_iterator&&) = default;
      zip_iterator(std::tuple<Types...> it): iterator(it) {}
      zip_iterator& operator=(const zip_iterator&) = default;
      zip_iterator& operator=(zip_iterator&&) = default;
      ~zip_iterator() = default;

      auto& operator++(){
	std::apply([](auto&&...v){ [](auto&&...v){}((++v)...); },iterator);
	return *this;
      }
      auto operator++(int){ auto copy{*this}; ++(*this); return copy; }
      auto& operator--(){
	std::apply([](auto&&...v){ [](auto&&...v){}((--v)...); },iterator);
	return *this;
      }
      auto operator--(int){ auto copy{*this}; --(*this); return copy; }

      auto operator*(){
	return std::apply([](auto&&...v){ return std::tie((*v)...); },iterator);
      }

      friend inline auto operator<(const zip_iterator<Types...>& lhs,
				   const zip_iterator<Types...>& rhs){
	return lhs.iterator < rhs.iterator;
      }
      friend inline auto operator>(const zip_iterator<Types...>& lhs,
				   const zip_iterator<Types...>& rhs){
	return rhs < lhs;
      }
      friend inline auto operator==(const zip_iterator<Types...>& lhs,
				    const zip_iterator<Types...>& rhs){
	return lhs.iterator == rhs.iterator;
      }
      friend inline auto operator!=(const zip_iterator<Types...>& lhs,
				    const zip_iterator<Types...>& rhs){
	return !(lhs == rhs);
      }
      friend inline auto operator<=(const zip_iterator<Types...>& lhs,
				    const zip_iterator<Types...>& rhs){
	return (lhs < rhs) || (lhs == rhs);
      }
      friend inline auto operator>=(const zip_iterator<Types...>& lhs,
				    const zip_iterator<Types...>& rhs){
	return (lhs > rhs) || (lhs == rhs);
      }
    };

    template<typename...Types> class zip {
    private:
      std::tuple<Types...> begins;
      std::tuple<Types...> ends;
    public:
      zip() = default;
      zip(const zip&) = default;
      zip(zip&&) = default;
      zip(std::tuple<Types...> begins,std::tuple<Types...> ends)
	: begins(begins), ends(ends) {}
      zip& operator=(const zip&) = default;
      zip& operator=(zip&&) = default;
      ~zip() = default;

      auto begin(){ return zip_iterator<Types...>{begins}; }
      auto   end(){ return zip_iterator<Types...>{  ends}; }
    };

    template<typename T> inline auto begin(T&& t){ t.begin(); }
    template<typename T> inline auto end(T&& t){ t.end(); }
  } // namespace detail

  template<typename...Types> auto zip(Types&&...v){
    using std::begin;
    using std::end;
    using zip_t = detail::zip<decltype(begin(v))...>;

    auto min = std::min({std::distance(begin(v),end(v))...});

    return zip_t{std::make_tuple(begin(v)...),std::make_tuple((begin(v)+min)...)};
  }
} // namespace ymd
#endif // YMD_ZIP_HH
