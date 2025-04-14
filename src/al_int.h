#ifndef AL_INT_H
#define AL_INT_H

#include <utility>
#include <cmath>

#include "al_uint.h"

template<typename uint_t>
class al_int_t_  {

  using accum_t = typename uint_t::accum_t_;

public:

  al_int_t_(const uint_t& val) :
    m_uint{val}, m_sign(1) {}
  al_int_t_(const uint_t& val, int sign) :
    m_uint{val}, m_sign(sign) {}

  al_int_t_(int value) :
    m_uint(accum_t(std::abs(value))),
    m_sign(value < 0 ? -1 : 1) {}

  bool zero() const {return m_uint == 0;}
  const uint_t& abs() const {return m_uint;}
  int sign() const {return m_sign;}
  
private:
  
  uint_t m_uint;
  int m_sign;

  template<typename uint_t_>
  friend al_int_t_<uint_t_>
  operator+(const al_int_t_<uint_t_>&,
            const al_int_t_<uint_t_>&);
  template<typename uint_t_>
  friend al_int_t_<uint_t_>
  operator-(const al_int_t_<uint_t_>&,
            const al_int_t_<uint_t_>&);
  template<typename uint_t_>
  friend al_int_t_<uint_t_>
  operator*(const al_int_t_<uint_t_>&,
            const al_int_t_<uint_t_>&);
  template<typename uint_t_>
  friend std::pair<al_int_t_<uint_t_>,
                   al_int_t_<uint_t_> >
  operator/(const al_int_t_<uint_t_>&,
            const al_int_t_<uint_t_>&);

  template<typename uint_t_> friend bool
  operator==(const al_int_t_<uint_t_>&,
             const al_int_t_<uint_t_>&);

};

template<typename uint_t>
al_int_t_<uint_t>
operator+(const al_int_t_<uint_t>& lhs,
          const al_int_t_<uint_t>& rhs) {
  if(lhs.m_sign == rhs.m_sign)
    return al_int_t_<uint_t>(lhs.m_uint+
                              rhs.m_uint,
                              lhs.m_sign);
  if(lhs.m_sign == -1) {
    if(lhs.m_uint < rhs.m_uint) {
      const auto absDiff = rhs.m_uint-lhs.m_uint;
      return al_int_t_<uint_t>(absDiff, +1);
    }
    else {
      const auto absDiff = lhs.m_uint - rhs.m_uint;
      return al_int_t_<uint_t>(absDiff, -1);
    }
  }
  else {
    if(rhs.m_uint < lhs.m_uint) {
      const auto absDiff = lhs.m_uint-rhs.m_uint;
      return al_int_t_<uint_t>(absDiff, +1);
    }
    else {
      const auto absDiff = rhs.m_uint - lhs.m_uint;
      return al_int_t_<uint_t>(absDiff, -1);
    }
  }
}

template<typename uint_t>
std::pair<al_int_t_<uint_t>,
          al_int_t_<uint_t> >
operator/(const al_int_t_<uint_t>& lhs,
          const al_int_t_<uint_t>& rhs) {
  const auto divRes = lhs.abs()/ rhs.abs();
  if(lhs.m_sign == rhs.m_sign) {
    return std::make_pair(al_int_t_<uint_t>(divRes.first,  +1),
                          al_int_t_<uint_t>(divRes.second, +1));
  }
  if(lhs.m_sign == -1) {
    return std::make_pair(al_int_t_<uint_t>(divRes.first,  -1),
                          al_int_t_<uint_t>(divRes.second, -1));
  }
  else {
    return std::make_pair(al_int_t_<uint_t>(divRes.first,  -1),
                          al_int_t_<uint_t>(divRes.second, +1));
  }
}

template<typename uint_t>
al_int_t_<uint_t>
operator-(const al_int_t_<uint_t>& lhs,
          const al_int_t_<uint_t>& rhs) {
  return lhs+al_int_t_<uint_t>(rhs.m_uint,
                                -rhs.m_sign);
}

template<typename uint_t>
al_int_t_<uint_t>
operator*(const al_int_t_<uint_t>& lhs,
          const al_int_t_<uint_t>& rhs) {
  return al_int_t_<uint_t>(lhs.m_uint*rhs.m_uint,
                            lhs.m_sign*rhs.m_sign);
}

template<typename uint_t> bool
operator==(const al_int_t_<uint_t>& lhs,
           const al_int_t_<uint_t>& rhs) {
  // Discard sign if zero
  if(lhs.abs()==0 && rhs.abs()==0)
    return true;
  return (lhs.sign() == rhs.sign() && lhs.abs() == rhs.abs());
}

typedef al_int_t_<al_uint_t> al_int_t;

#endif
