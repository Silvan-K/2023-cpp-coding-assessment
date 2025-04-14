#include "src/al_uint.h"
#include <iostream>
#include <cassert>
#include <climits>

namespace {

int charToInt(const char digit) {
  static const int offset = int('0');
  return int(digit) - offset;
};

template<typename words_t>
void trim(words_t& input) {
  if(input.empty()) return;
  auto endit=input.end();
  for(auto it = input.end()-1;
      it>=input.begin(); it--) {
    if (*it != 0) break;
    endit=it;
  }
  if (endit!=input.end())
    input.erase(endit, input.end());
  return;
}

template<typename words_t, typename accum_t>
void mul(const typename words_t::value_type& a,
         const typename words_t::value_type& b,
         words_t& resultBuffer) {
  static const auto wordMax =
    std::numeric_limits<typename words_t::value_type>::max();
  static const auto wordLen =
    sizeof(typename words_t::value_type)*CHAR_BIT;
  accum_t c = accum_t(a)*accum_t(b);
  resultBuffer.push_back(c) ;
  if(c > wordMax) resultBuffer.push_back(c>>wordLen) ;
  return;
}

template<typename word_t>
word_t sub(const word_t& a,
           const word_t& b,
           word_t& carry) {
  word_t partial = a - carry;
  carry = partial > a;
  word_t res = partial - b;
  carry = carry ? carry : res > partial;
  return res;
}

template<typename word_t>
word_t add(const word_t& a,
           const word_t& b,
           word_t& carry) {
  word_t partial = a + carry;
  carry = partial < a;
  word_t res = partial + b;
  carry = carry ? carry : res < partial;
  return res;
}

template<typename words_t>
words_t sub(const words_t& a,
            const words_t& b) {
  using word_t = typename words_t::value_type;
  words_t res; res.reserve(a.size()); word_t carry(0);
  for(std::size_t i=0; i<a.size(); i++)
    res.push_back(sub<word_t>(a[i],i<b.size()?b[i]:0, carry));
  assert(!carry);
  trim(res);
  return res;
}

template<typename words_t>
words_t add(const words_t& a,
            const words_t& b) {
  using word_t = typename words_t::value_type;
  const auto& sizeA = a.size();
  const auto& sizeB = b.size();
  const auto& largeX = sizeA > sizeB ? a : b;
  const auto& smallX = sizeA > sizeB ? b : a;
  const auto outSize = largeX.size();
  const auto smallSize = smallX.size();
  words_t res(outSize); word_t carry(0);
  for(std::size_t i=0; i<outSize; i++)
    res[i] = add<word_t>(largeX[i],
                         i<smallSize?smallX[i]:0,
                         carry);
  if (carry) res.push_back(carry);
  return res;
}

} // Anonymous namespace

template<typename word_t, typename accum_t>
al_uint_t_<word_t, accum_t>::al_uint_t_(const words_t& data)
  : m_words{data} { trim(m_words); }

template<typename word_t, typename accum_t>
al_uint_t_<word_t, accum_t>::al_uint_t_(accum_t value){
  while(value) {
    m_words.push_back(word_t{0} | value);
    value = value >> m_words.size()*wordSize;
  }
}

// TODO: pass reference and allow modification if significant for performance
template<typename word_t, typename accum_t>
typename al_uint_t_<word_t, accum_t>::words_t
al_uint_t_<word_t, accum_t>::decimalToBinary(std::string input) {
  std::vector<bool> bits;
  while(input!="0" && input != "") {
    // TODO: omit conversion to int and instead check for odd/even directly on
    // string if relevant for performance
    bits.push_back(charToInt(input.back())%2);
    input = divByTwo(input);
  }
  words_t ret;
  const auto wordlen = al_uint_t_<word_t, accum_t>::wordSize;
  for(size_t i(0); i<bits.size(); i++){
    const auto wordIdx = i/wordlen;
    if(wordIdx>=ret.size()) ret.push_back(word_t{0});
    if(bits[i]) ret[wordIdx] |= word_t{1} << i%(wordlen);
  }
  trim(ret);
  return ret;
}

// TODO: modify argument in place if significant for performance
template<typename word_t, typename accum_t>
std::string al_uint_t_<word_t, accum_t>::
divByTwo(const std::string& input) {
  std::string ret = ""; ret.reserve(input.size());
  int carry = 0;
  for(const auto& s: input) {
    const int digit = charToInt(s);
    ret.append(std::to_string(digit/2 + carry));
    carry = (digit%2)*5;
  }
  if(ret.size()>1 && *ret.begin()=='0') 
    ret.erase(ret.begin());
  return ret;
}

//////////////////////////
// Arithmetic operators //
//////////////////////////

template<typename word_t, typename accum_t>
bool al_uint_t_<word_t, accum_t>::operator<=
(const al_uint_t_<word_t, accum_t>& rhs) const {
  if (*this==rhs)
    return true;
  return *this < rhs;
}                

template<typename word_t, typename accum_t>
bool al_uint_t_<word_t, accum_t>::operator>=
(const al_uint_t_<word_t, accum_t>& rhs) const {
  if (*this==rhs)
    return true;
  return *this > rhs;
}                

template<typename word_t, typename accum_t>
bool al_uint_t_<word_t, accum_t>::operator<
(const al_uint_t_<word_t, accum_t>& rhs) const {
  const auto& lhsData = getData();
  const auto& rhsData = rhs.getData();
  if(rhsData.size() != lhsData.size())
    return lhsData.size() < rhsData.size();
  auto rit = rhsData.rbegin();
  auto lit = lhsData.rbegin();
  for(; rit!=rhsData.rend(); rit++, lit++) {
    if(*rit == *lit) continue;
    if(*lit < *rit) return true;
    return false;
  }
  assert(lhsData == rhsData);
  return false;
}

template<typename word_t, typename accum_t>
bool al_uint_t_<word_t, accum_t>::operator>
(const al_uint_t_<word_t, accum_t>& rhs) const {
  const auto& lhsData = getData();
  const auto& rhsData = rhs.getData();
  if(rhsData.size() != lhsData.size())
    return lhsData.size() > rhsData.size();
  auto rit = rhsData.rbegin();
  auto lit = lhsData.rbegin();
  for(; rit!=rhsData.rend(); rit++, lit++) {
    if(*rit == *lit) continue;
    if(*lit > *rit) return true;
    return false;
  }
  assert(lhsData == rhsData);
  return false;
}

template<typename word_t, typename accum_t>
bool al_uint_t_<word_t, accum_t>::operator==
(const al_uint_t_<word_t, accum_t>& rhs) const {
  return getData() == rhs.getData();
}

template<typename word_t, typename accum_t>
bool al_uint_t_<word_t, accum_t>::operator==
(const accum_t& rhs) const {
  if(rhs==0) return m_words.empty();
  throw("Not Yet Implemented");
  return false;
}

template<typename word_t, typename accum_t>
bool al_uint_t_<word_t, accum_t>::operator!=
(const accum_t& rhs) const {
  if(rhs==0) return !m_words.empty();
  throw("Not Yet Implemented");
  return false;
}

template<typename word_t, typename accum_t>
bool al_uint_t_<word_t, accum_t>::operator==
(const int& rhs) const {
  if(rhs==0) return m_words.empty();
  throw("Not Yet Implemented");
  return false;
}

template<typename word_t, typename accum_t>
bool al_uint_t_<word_t, accum_t>::operator!=
(const int& rhs) const {
  if(rhs==0) return !m_words.empty();
  throw("Not Yet Implemented");
  return false;
}

template<typename word_t, typename accum_t>
al_uint_t_<word_t, accum_t>
operator+(const al_uint_t_<word_t, accum_t>& lhs,
          const al_uint_t_<word_t, accum_t>& rhs){
  return al_uint_t_<word_t, accum_t>(add(lhs.getData(),
                                           rhs.getData())); 
}

template<typename word_t, typename accum_t>
al_uint_t_<word_t, accum_t>
operator-(const al_uint_t_<word_t, accum_t>& lhs,
          const al_uint_t_<word_t, accum_t>& rhs){
  return al_uint_t_<word_t, accum_t>(sub(lhs.getData(),
                                           rhs.getData())); 
}

// TODO: do addition truly in-place
template<typename word_t, typename accum_t>
al_uint_t_<word_t, accum_t>&
operator+=(al_uint_t_<word_t, accum_t>& lhs,
           const al_uint_t_<word_t, accum_t>& rhs) {
  lhs.m_words = add(lhs.getData(), rhs.getData());
  return lhs;
}

// TODO: do addition truly in-place
template<typename word_t, typename accum_t>
al_uint_t_<word_t, accum_t>&
operator-=(al_uint_t_<word_t, accum_t>& lhs,
           const al_uint_t_<word_t, accum_t>& rhs) {
  lhs.m_words = sub(lhs.getData(), rhs.getData());
  return lhs;
}

template<typename word_t, typename accum_t>
al_uint_t_<word_t, accum_t>
operator*(const al_uint_t_<word_t, accum_t>& lhs,
          const al_uint_t_<word_t, accum_t>& rhs){
  using words_t = typename al_uint_t_<word_t, accum_t>::words_t;
  const auto& a = lhs.getData();
  const auto& b = rhs.getData();
  auto ret = words_t{0};
  for(size_t i(0); i<a.size(); i++) {
    for(size_t j(0); j<b.size(); j++) {
      auto partialBuffer = words_t(i+j, 0);
      mul<words_t, accum_t>(a[i], b[j], partialBuffer);
      // TODO: in-place addition
      ret = add(ret, partialBuffer);
    }
  }
  trim(ret);
  return al_uint_t_<word_t, accum_t>(ret);
}

template<typename word_t, typename accum_t>
al_uint_t_<word_t, accum_t>
quickDiv(const al_uint_t_<word_t, accum_t>& lhs,
         const al_uint_t_<word_t, accum_t>& rhs) {
  assert(lhs.getData().size()); assert(lhs>rhs);
  using RetType = al_uint_t_<word_t, accum_t>;
  const word_t& divisor = rhs.getData().back();
  const auto lhsSize = lhs.getData().size();
  const auto rhsSize = rhs.getData().size();
  static const auto wordLen = sizeof(word_t)*CHAR_BIT;
  if(lhsSize==1) return RetType(lhs.getData().back()/divisor);
  // TODO: avoid repeated inserts
  const int quotSize = 1+lhsSize-rhsSize;
  auto quot = RetType(); quot.m_words.resize(quotSize);
  accum_t carry(0);
  for(int i(0); i<quotSize; i++ ){
    accum_t thisDiv = accum_t(lhs.getData()[lhsSize-1-i]) + (carry<<wordLen);
    assert(thisDiv/divisor < std::numeric_limits<word_t>::max());
    carry = thisDiv%divisor;
    quot.m_words[quotSize-1-i] = word_t(thisDiv/divisor);
  }
  trim(quot.m_words);
  return quot;
}

// template<typename word_t, typename accum_t>
// al_uint_t_<word_t, accum_t>::operator int() const {
//   accum_t res = accum_t(getData().front());
//   if(size>1) res += getData()[size-2];
// }

template<typename word_t, typename accum_t>
std::pair<al_uint_t_<word_t, accum_t>,
          al_uint_t_<word_t, accum_t>>
operator/(const al_uint_t_<word_t, accum_t>& lhs,
          const al_uint_t_<word_t, accum_t>& rhs) {
  using RetType = al_uint_t_<word_t, accum_t>;
  if(lhs<rhs) return std::make_pair(RetType({0}), lhs);
  if(rhs.getData().size()==1 && rhs.getData().front()==1)
    return std::make_pair(lhs, RetType({0}));
  if(rhs==lhs) return std::make_pair(RetType({1}),
                                     RetType({0}));
  if(rhs.getData().size()==1 && lhs.getData().size()==1){
    word_t quot = lhs.getData()[0]/rhs.getData()[0];
    word_t rema = lhs.getData()[0]%rhs.getData()[0];
    return std::make_pair(RetType({quot}), RetType({rema}));
  }
  auto quot = quickDiv(lhs, rhs);
  auto QD = quot*rhs; bool qTooSmall = lhs > QD;
  auto rem = qTooSmall ? lhs-QD : QD-lhs;
  while(rem>rhs || !qTooSmall) {
    const auto delta = rem>rhs ? quickDiv(rem, rhs) : RetType(1);
    quot = qTooSmall ? (quot+delta) : quot-delta;
    QD = quot*rhs; qTooSmall = lhs > QD;
    rem = qTooSmall ? lhs-QD : QD-lhs;
  }
  return std::make_pair(quot, rem);
}

//////////////////////////////////////
// Explicit template instantiations //
//////////////////////////////////////

template al_uint_t_<uint8_t, uint16_t>
operator*(const al_uint_t_<uint8_t, uint16_t>& lhs,
          const al_uint_t_<uint8_t, uint16_t>& rhs);

template al_uint_t_<uint32_t, uint64_t>
operator*(const al_uint_t_<uint32_t, uint64_t>& lhs,
          const al_uint_t_<uint32_t, uint64_t>& rhs);

template std::pair<al_uint_t_<uint8_t, uint16_t>,
                   al_uint_t_<uint8_t, uint16_t> >
operator/(const al_uint_t_<uint8_t, uint16_t>& lhs,
          const al_uint_t_<uint8_t, uint16_t>& rhs);

template std::pair<al_uint_t_<uint32_t, uint64_t>,
                   al_uint_t_<uint32_t, uint64_t> >
operator/(const al_uint_t_<uint32_t, uint64_t>& lhs,
          const al_uint_t_<uint32_t, uint64_t>& rhs);

template al_uint_t_<uint8_t, uint16_t>
operator+(const al_uint_t_<uint8_t, uint16_t>& lhs,
          const al_uint_t_<uint8_t, uint16_t>& rhs);

template al_uint_t_<uint32_t, uint64_t>
operator+(const al_uint_t_<uint32_t, uint64_t>& lhs,
          const al_uint_t_<uint32_t, uint64_t>& rhs);

template al_uint_t_<uint32_t, uint64_t>&
operator+=(al_uint_t_<uint32_t, uint64_t>& lhs,
           const al_uint_t_<uint32_t, uint64_t>& rhs);
template al_uint_t_<uint32_t, uint64_t>&
operator-=(al_uint_t_<uint32_t, uint64_t>& lhs,
           const al_uint_t_<uint32_t, uint64_t>& rhs);

template al_uint_t_<uint8_t, uint16_t>
operator-(const al_uint_t_<uint8_t, uint16_t>& lhs,
          const al_uint_t_<uint8_t, uint16_t>& rhs);

template al_uint_t_<uint32_t, uint64_t>
operator-(const al_uint_t_<uint32_t, uint64_t>& lhs,
          const al_uint_t_<uint32_t, uint64_t>& rhs);

template class al_uint_t_<uint8_t, uint16_t>;
template class al_uint_t_<uint32_t, uint64_t>;
