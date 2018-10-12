#ifndef YMD_TUPLE_ZIP_HH
#define YMD_TUPLE_ZIP_HH 1

#include <tuple>
#include <algorithm>
#include <utility>

namespace ymd {
  template<std::size_t N,typename F,typename...Tuples>
  inline auto apply_at_N(F&& f,Tuples&&...tuples){
    return f(std::get<N>(tuples)...);
  }

  template<std::size_t...Ns>
  inline auto get(const std::index_sequence<Ns...>&){
    return [=](auto&& f,auto&...t){
	     return std::make_tuple(apply_at_N<Ns>(f,t...)...);
	   };

  }

  template<typename F,typename...Tuples> inline auto
  zip_for_each(F&& f,Tuples&...tuples){
    using indices_t =
      std::make_index_sequence<std::min({std::tuple_size<Tuples>::value...})>;

    return get(indices_t{})(f,tuples...);
  }

} // namespace ymd
#endif // YMD_TUPLE_ZIP_HH
