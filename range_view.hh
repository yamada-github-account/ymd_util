#ifndef YMD_RANGE_VIEW_HH
#define YMD_RANGE_VIEW_HH 1

#include <iterator>
#include <utility>

namespace ymd {

  template<typename Iterator> class range_view {
  private:
    Iterator v_begin;
    Iterator v_end;

  public:
    range_view() = default;
    range_view(const range_view&) = default;
    range_view(range_view&&) = default;
    range_view(Iterator begin,Iterator end)
      : v_begin(std::forward<Iterator>(begin)), v_end(std::forward<Iterator>(end)){}
    range_view& operator=(const range_view&) = default;
    range_view& operator=(range_view&&) = default;
    ~range_view() = default;

    auto begin() const { return v_begin; }
    auto   end() const { return v_end; }
  };

} // namespace ymd
#endif // YMD_RANGE_VIEW
