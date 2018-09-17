#ifndef YMD_TIMER_HH
#define YMD_TIMER_HH 1

#include <iostream>
#include <chrono>

// Requirement: c++17 (std::chorno::floor)
//
// Function   : template<typename F> auto time_N(F f,std::size_t N)
//              Arguments
//              ---------
//              F f          : Function object measured time
//              std::size_t N: The number of iteration
//
//              Return
//              ------
//              void
//
// Usage       : ymd::time_N([](){ return 1 + 3; },1000);
//

namespace ymd {

  template<typename F>
  inline auto time_N(F f,std::size_t N){
    using namespace std::chrono;

    auto start = system_clock::now();

    for(auto i = 0ul; i < N; ++i){
      f();
    }

    auto end = system_clock::now();

    auto elapsed = end - start;
    std::cout << floor<hours>(elapsed).count() << "h "
	      << floor<minutes>(elapsed).count() % 60 << "min "
	      << floor<seconds>(elapsed).count() % 60 << "s "
	      << floor<milliseconds>(elapsed).count() % 1000 << "ms "
	      << floor<microseconds>(elapsed).count() % 1000 << "us "
	      << floor<nanoseconds>(elapsed).count() % 1000 << "ns"
	      << std::endl;
  }
}
#endif // YMD_TIMER_HH
