#ifndef YMD_ADAM_HH
#define YMD_ADAM_HH 1

#include <cmath>

namespace ymd {
  template<typename ValueType> class Adam {
  public:
    using value_type = ValueType;

  private:
    value_type alpha;
    value_type beta1;
    value_type beta2;
    value_type beta1_t;
    value_type beta2_t;
    value_type eps;
    value_type m;
    value_type v;

  public:
    Adam()
      : alpha{0.001}, beta1{0.9}, beta2{0.999},
	beta1_t{0}, beta2_t{0}, eps{1e-8}, m{0}, v{0} {}
    Adam(const Adam&) = default;
    Adam(Adam&&) = default;
    Adam(value_type alpha,value_type beta1,value_type beta2,value_type eps)
      :alpha{alpha}, beta1{beta1}, beta2{beta2},
	beta1_t{0}, beta2_t{0}, eps{eps}, m{0}, v{0} {}
    Adam& operator=(const Adam&) = default;
    Adam& operator=(Adam&&) = default;
    ~Adam() = default;

    inline auto operator()(value_type diff){
      m = beta1 * m + (1-beta1) * diff;
      v = beta2 * v + (1-beta2) * diff * diff;

      beta1_t *= beta1;
      beta2_t *= beta2;

      auto m_hat = m / (1-beta1_t);
      auto v_hat = v / (1-beta2_t);

      return alpha * m_hat / (std::sqrt(v_hat) + eps);
    }
  };

} // namespace ymd
#endif // YMD_ADAM
