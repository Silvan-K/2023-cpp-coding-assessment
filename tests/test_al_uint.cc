#include "src/al_int.h"
#include "src/al_uint.h"
#include "src/prime_field_element.h"
#include "gtest/gtest.h"

namespace {

TEST(al_uint_t, Construct_emtpy) {
  const auto result = al_uint_t("").getData();
  const auto oracle = al_uint_t::words_t{};
  EXPECT_EQ(result, oracle);
}

TEST(al_uint_t, Construct_zero) {
  const auto result = al_uint_t("0").getData();
  const auto oracle = al_uint_t::words_t{};
  EXPECT_EQ(result, oracle);
}

TEST(al_uint_t, Construct_zero_padded) {
  const auto result = al_uint_t("0123").getData();
  const auto oracle = al_uint_t("123").getData();
  EXPECT_EQ(result, oracle);
}

TEST(al_uint_t, Construct_one) {
  const auto result = al_uint_t("1").getData();
  const auto oracle = al_uint_t::words_t{1};
  EXPECT_EQ(result, oracle);
}

TEST(al_uint_t, Construct_three) {
  const auto result = al_uint_t("3").getData();
  const auto oracle = al_uint_t::words_t{3};
  EXPECT_EQ(result, oracle);
}

TEST(al_uint_t, Construct_two_words_simple) {
  const auto result = al_uint_t("4294967296").getData(); // 2^^32
  const auto oracle = al_uint_t::words_t{0, 1};
  EXPECT_EQ(result, oracle);
}

TEST(al_uint_t, Construct_two_words) {
  const auto result = al_uint_t("17596481011720").getData(); // 2^^3 + 2^^32 + 2^^44
  const auto oracle = al_uint_t::words_t{8, 4097};
  EXPECT_EQ(result, oracle);
}

TEST(al_uint_t, Construct_three_words_simple) {
  const auto result = al_uint_t("18446744073709551616").getData(); // 2^^(2^^64)
  const auto oracle = al_uint_t::words_t{0, 0, 1};
  EXPECT_EQ(result, oracle);
}

TEST(al_uint_t, Construct_large) {
  const auto result = al_uint_t("134928736532512359234162314538").getData();
  const auto oracle = al_uint_t::words_t{1030933802, 4139091843, 3019534167, 1};
  EXPECT_EQ(result, oracle);
}

TEST(al_uint_t, Construct_int_zero) {
  const auto result = al_uint_t(0).getData();
  const auto oracle = al_uint_t("0").getData();
  EXPECT_EQ(result, oracle);
}

TEST(al_uint_t, Construct_int) {
  const auto result = al_uint_t(12341234).getData();
  const auto oracle = al_uint_t("12341234").getData();
  EXPECT_EQ(result, oracle);
}

TEST(al_uint_t, divByTwo_long) {
  const auto res = al_uint_t::divByTwo("134928736532512359234162314538");
  EXPECT_EQ(res, "67464368266256179617081157269");
}

TEST(al_uint_t, divByTwo_empty) {
  const auto res = al_uint_t::divByTwo("");
  EXPECT_EQ(res, "");
}

TEST(al_uint_t, divByTwo_0) {
  const auto res = al_uint_t::divByTwo("0");
  EXPECT_EQ(res, "0");
}

TEST(al_uint_t, divByTwo_1) {
  const auto res = al_uint_t::divByTwo("1");
  EXPECT_EQ(res, "0");
}

TEST(al_uint_t, divByTwo_2) {
  const auto res = al_uint_t::divByTwo("2");
  EXPECT_EQ(res, "1");
}

TEST(al_uint_t, divByTwo_3) {
  const auto res = al_uint_t::divByTwo("3");
  EXPECT_EQ(res, "1");
}

TEST(al_uint_t, divByTwo_4) {
  const auto res = al_uint_t::divByTwo("4");
  EXPECT_EQ(res, "2");
}

TEST(al_uint_t, divByTwo_5) {
  const auto res = al_uint_t::divByTwo("5");
  EXPECT_EQ(res, "2");
}

TEST(al_uint_t, divByTwo_6) {
  const auto res = al_uint_t::divByTwo("6");
  EXPECT_EQ(res, "3");
}

TEST(al_uint_t, divByTwo_7) {
  const auto res = al_uint_t::divByTwo("7");
  EXPECT_EQ(res, "3");
}

TEST(al_uint_t, divByTwo_8) {
  const auto& res = al_uint_t::divByTwo("8");
  EXPECT_EQ(res, "4");
}

TEST(al_uint_t, divByTwo_9) {
  const auto& res = al_uint_t::divByTwo("9");
  EXPECT_EQ(res, "4");
}
  
TEST(al_uint_t, Add) {
  al_uint_t n = al_uint_t("1267650600228229401496703205376"); // 2^100
  al_uint_t two(2); 
  EXPECT_EQ(n + n, two * n);
}

TEST(al_uint_t, Add_zero) {
  const auto n = al_uint_t("1267650600228229000923");
  const auto m = al_uint_t("0");
  const auto result = (n+m).getData();
  const auto oracle = n.getData();
  EXPECT_EQ(result, oracle);
}

TEST(al_uint_t, Add_nonextending) {
  const auto n = al_uint_t("1267650600228229000923");
  const auto m = al_uint_t("1");
  const auto result = (n+m).getData();
  const auto oracle = al_uint_t("1267650600228229000924").getData();
  EXPECT_EQ(result, oracle);
}

TEST(al_uint_t, Add_extending) {
  const auto n = al_uint_t("00001267650600228229000923");
  const auto m = al_uint_t("78230000000000000000000000");
  const auto result = (n+m).getData();
  const auto oracle = al_uint_t("78231267650600228229000923").getData();
  EXPECT_EQ(result, oracle);
}

TEST(BigUnssigned, Sub_zero) {
  const auto n = al_uint_t("1267650600228229000923");
  const auto m = al_uint_t("0");
  const auto result = (n-m).getData();
  const auto oracle = n.getData();
  EXPECT_EQ(result, oracle);
}

TEST(BigUnssigned, Sub_one) {
  const auto n = al_uint_t("1267650600228229000923");
  const auto m = al_uint_t("1");
  const auto result = (n-m).getData();
  const auto oracle = al_uint_t("1267650600228229000922").getData();
  EXPECT_EQ(result, oracle);
}

TEST(BigUnssigned, Sub_nonshortening) {
  const auto n = al_uint_t("1267650600228229000923");
  const auto m = al_uint_t("1239048");
  const auto result = (n-m).getData();
  const auto oracle = al_uint_t("1267650600228227761875").getData();
  EXPECT_EQ(result, oracle);
}

TEST(BigUnssigned, Sub_shortening) {
  const auto n = al_uint_t("1267650600228229000923");
  const auto m = al_uint_t("1267650600228229000833");
  const auto result = (n-m).getData();
  const auto oracle = al_uint_t("90").getData();
  EXPECT_EQ(result, oracle);
}

TEST(al_uint_t, Mul_zero) {
  const auto n = al_uint_t("1267650600228229401496703205376"); // 2^100
  const auto m = al_uint_t("0");
  const auto result = (n*m).getData();
  const auto oracle = al_uint_t("0").getData();
  EXPECT_EQ(result, oracle);
}

TEST(al_uint_t, Mul_zero_swap) {
  const auto n = al_uint_t("0");
  const auto m = al_uint_t("1267650600228229401496703205376"); // 2^100
  const auto result = (n*m).getData();
  const auto oracle = al_uint_t("0").getData();
  EXPECT_EQ(result, oracle);
}

TEST(al_uint_t, Mul_one) {
  const auto n = al_uint_t("193485812374651982734501832475019843761");
  const auto m = al_uint_t("1");
  const auto result = (n*m).getData();
  const auto oracle = n.getData();
  EXPECT_EQ(result, oracle);
}

TEST(al_uint_t, Mul_no_carry) {
  const auto n = al_uint_t(al_uint_t::words_t{2,4,34,6,2,0,2}); // 2^100
  const auto m = al_uint_t("2");
  const auto result = (n*m).getData();
  const auto oracle = al_uint_t(al_uint_t::words_t{4,8,68,12,4,0,4}).getData();
  EXPECT_EQ(result, oracle);
}

TEST(al_uint_t, Mul_two) {
  const auto n = al_uint_t("193485812374651982734501832475019843761");
  const auto m = al_uint_t("193485812374651982734501832475019843761");
  const auto result = (al_uint_t("2")*n).getData();
  const auto oracle = (n+m).getData();
  EXPECT_EQ(result, oracle);
}

TEST(al_uint_t, Mul_three) {
  const auto n = al_uint_t("193485812374651982734501832475019843761");
  const auto m = al_uint_t("193485812374651982734501832475019843761");
  const auto l = al_uint_t("193485812374651982734501832475019843761");
  const auto result = (al_uint_t("3")*n).getData();
  const auto oracle = (n+m+l).getData();
  EXPECT_EQ(result, oracle);
}

TEST(al_uint_t, Mul_sqr) {
  const auto n = al_uint_t("193485812374651982734501832475019843761");
  const auto m = al_uint_t("193485812374651982734501832475019843761");
  const auto result = (n*m).getData();
  const auto oracle = al_uint_t("37436759590279030333954243110727827550709679681940083787260392270724850625121").getData();
  EXPECT_EQ(result, oracle);
}

TEST(al_uint_t, Mul_large) {
  const auto n = al_uint_t("193485812374651982734501832475019843761");
  const auto m = al_uint_t("12309872098376457812346512304987120493587");
  const auto result = (n*m).getData();
  const auto oracle = al_uint_t("2381785603182430810370471074024055573822238383644218710286641709364977942460707").getData();
  EXPECT_EQ(result, oracle);
}

TEST(al_uint_t, less_true) {
  al_uint_t n("2");
  al_uint_t m("1239487183294709138740912873490187234");
  EXPECT_EQ(n<m, true);
}

TEST(al_uint_t, less_false) {
  al_uint_t n("2");
  al_uint_t m("1239487183294709138740912873490187234");
  EXPECT_EQ(m<n, false);
}

TEST(al_uint_t, less_equal) {
  al_uint_t n("2");
  al_uint_t m("2");
  EXPECT_EQ(m<n, false);
}

TEST(al_uint_t, less_false_zero) {
  al_uint_t n("2");
  al_uint_t m("0");
  EXPECT_EQ(n<m, false);
}

TEST(al_uint_t, less_true_zero) {
  al_uint_t n("2");
  al_uint_t m("0");
  EXPECT_EQ(m<n, true);
}

TEST(al_uint_t, less_multi_word_0) {
  al_uint_t n(al_uint_t::words_t{19,1,83,7,46});
  al_uint_t m(al_uint_t::words_t{19,1,83,7,41});
  EXPECT_EQ(m<n, true);
}

TEST(al_uint_t, less_multi_word_1) {
  al_uint_t n(al_uint_t::words_t{19,1,83,7,46});
  al_uint_t m(al_uint_t::words_t{19,1,83,7,46});
  EXPECT_EQ(m<n, false);
}

TEST(al_uint_t, less_multi_word_2) {
  al_uint_t n(al_uint_t::words_t{19,1,83,7,46});
  al_uint_t m(al_uint_t::words_t{19,1,83,7,47});
  EXPECT_EQ(n<m, true);
}

TEST(al_uint_t, less_multi_word_3) {
  al_uint_t n(al_uint_t::words_t{19,1,83,7,46});
  al_uint_t m(al_uint_t::words_t{19,1,83,47});
  EXPECT_EQ(n<m, false);
}

TEST(al_uint_t, less_multi_word_4) {
  al_uint_t n(al_uint_t::words_t{0, 0, 0, 16});
  al_uint_t m(al_uint_t::words_t{2928915036, 430});
  EXPECT_EQ(m<n, true);
}

TEST(al_uint_t, less_multi_word_5) {
  al_uint_t n(al_uint_t::words_t{1366052260, 4294966865, 4294967295, 15});
  al_uint_t m(al_uint_t::words_t{0, 0, 0, 16});
  EXPECT_EQ(n<m, true);
}

TEST(al_uint_t, more_false) {
  al_uint_t n("2");
  al_uint_t m("1239487183294709138740912873490187234");
  EXPECT_EQ(n>m, false);
}

TEST(al_uint_t, more_true) {
  al_uint_t n("2");
  al_uint_t m("1239487183294709138740912873490187234");
  EXPECT_EQ(m>n, true);
}

TEST(al_uint_t, more_equal) {
  al_uint_t n("2");
  al_uint_t m("2");
  EXPECT_EQ(m>n, false);
}

TEST(al_uint_t, more_true_zero) {
  al_uint_t n("2");
  al_uint_t m("0");
  EXPECT_EQ(n>m, true);
}

TEST(al_uint_t, more_false_zero) {
  al_uint_t n("2");
  al_uint_t m("0");
  EXPECT_EQ(m>n, false);
}

TEST(al_uint_t, more_multi_word_0) {
  al_uint_t n(al_uint_t::words_t{19,1,83,7,46});
  al_uint_t m(al_uint_t::words_t{19,1,83,7,41});
  EXPECT_EQ(m>n, false);
}

TEST(al_uint_t, more_multi_word_1) {
  al_uint_t n(al_uint_t::words_t{19,1,83,7,46});
  al_uint_t m(al_uint_t::words_t{19,1,83,7,46});
  EXPECT_EQ(m>n, false);
}

TEST(al_uint_t, more_multi_word_2) {
  al_uint_t n(al_uint_t::words_t{19,1,83,7,46});
  al_uint_t m(al_uint_t::words_t{19,1,83,7,47});
  EXPECT_EQ(n>m, false);
}

TEST(al_uint_t, more_multi_word_3) {
  al_uint_t n(al_uint_t::words_t{19,1,83,7,46});
  al_uint_t m(al_uint_t::words_t{19,1,83,47});
  EXPECT_EQ(n>m, true);
}

TEST(al_uint_t, more_multi_word_5) {
  al_uint_t n(al_uint_t::words_t{1366052260, 4294966865, 4294967295, 15});
  al_uint_t m(al_uint_t::words_t{0, 0, 0, 16});
  EXPECT_EQ(n>m, false);
}

TEST(al_uint_t, Div_two) {
  const auto n = al_uint_t(4);
  const auto m = al_uint_t(2);
  const auto result = (n/m).first.getData();
  const auto oracle = al_uint_t(2).getData();
  EXPECT_EQ(result, oracle);
}

TEST(al_uint_t, Div_one) {
  const auto n = al_uint_t(al_uint_t::words_t{1,1});
  const auto m = al_uint_t(1);
  const auto result = (n/m).first.getData();
  const auto oracle = al_uint_t(n).getData();
  EXPECT_EQ(result, oracle);
}

TEST(al_uint_t, Div_0) {
  const auto n = al_uint_t("12980234123849");
  const auto m = al_uint_t("2");
  const auto result = (n/m);
  const auto quot = result.first;
  const auto rema = result.second;
  const auto quotOracle = al_uint_t("6490117061924");
  const auto remaOracle = al_uint_t("1");
  EXPECT_EQ(quot, quotOracle);
  EXPECT_EQ(rema, remaOracle);
}

TEST(al_uint_t, Div_same) {
  const auto n = al_uint_t("12980234123849");
  const auto m = al_uint_t("12980234123849");
  const auto result = (n/m);
  const auto quot = result.first.getData();
  const auto rema = result.second.getData();
  const auto quotOracle = al_uint_t("1").getData();
  const auto remaOracle = al_uint_t("0").getData();
  EXPECT_EQ(quot, quotOracle);
  EXPECT_EQ(rema, remaOracle);
}

TEST(al_uint_t, Div_1) {
  const auto n = al_uint_t("311489127986228");
  const auto m = al_uint_t("103829709328741");
  const auto result = (n/m);
  const auto quot = result.first.getData();
  const auto rema = result.second.getData();
  const auto quotOracle = al_uint_t("3").getData();
  const auto remaOracle = al_uint_t("5").getData();
  EXPECT_EQ(quot, quotOracle);
  EXPECT_EQ(rema, remaOracle);
}

TEST(al_uint_t, Div_long) {
  const auto n = al_uint_t("329874923475123410923784218729");
  const auto m = al_uint_t("103829709328741");
  const auto result = (n/m);
  const auto quot = result.first.getData();
  const auto rema = result.second.getData();
  const auto quotOracle = al_uint_t("3177076441875495").getData();
  const auto remaOracle = al_uint_t("64264237116934").getData();
  EXPECT_EQ(quot, quotOracle);
  EXPECT_EQ(rema, remaOracle);
}

TEST(BigPrimeField, Add) {
  al_uint_t p("1849764852316");
  BigPrimeField::set_prime(p);
  BigPrimeField n("1267650600228229401496703205376"); // 2^100
  BigPrimeField res("1037610352464");
  EXPECT_EQ(n + n, res);
}

TEST(BigPrimeField, SubtractPositive) {
  al_uint_t p("1849764852316");
  BigPrimeField::set_prime(p);
  BigPrimeField n("1267650600228229401496703205376"); // 2^100
  BigPrimeField n2("123456789123456789");
  BigPrimeField res("1114309858401");
  EXPECT_EQ(n2 - n, res);
}

TEST(BigPrimeField, SubtractNegative) {
  al_uint_t p("1849764852316");
  BigPrimeField::set_prime(p);
  BigPrimeField n("1267650600228229401496703205376"); // 2^100
  BigPrimeField n2("123456789123456789");
  BigPrimeField res("735454993915");
  EXPECT_EQ(n - n2, res);
}

TEST(BigPrimeField, Multiply) {
  al_uint_t p("1849764852316");
  BigPrimeField::set_prime(p);
  BigPrimeField n("1267650600228229401496703205376"); // 2^100
  BigPrimeField res("464726607316");
  EXPECT_EQ(n * n, res);
}

TEST(BigPrimeField, Divide) {
  al_uint_t p("1849764852316");
  BigPrimeField::set_prime(p);
  BigPrimeField n("1267650600228229401496703205376"); // 2^100
  BigPrimeField d("123456789");
  BigPrimeField res("1072048663712");
  BigPrimeField got = n / d;
  EXPECT_EQ(got, res);
}

TEST(BigPrimeField, DivideSimple) {
  al_uint_t p("37");
  BigPrimeField::set_prime(p);
  BigPrimeField n("1");
  BigPrimeField d("2");
  BigPrimeField res("19");
  BigPrimeField got = n / d;
  EXPECT_EQ(got, res);
}

TEST(BigPrimeField, Negate) {
  al_uint_t p("571203188563142862548892342071");
  BigPrimeField::set_prime(p);
  BigPrimeField n("340958134905802394");
  BigPrimeField m = -n;
  BigPrimeField result = n+m;
  BigPrimeField oracle(0);
  EXPECT_EQ(result.getData(), oracle.getData());
}

TEST(BigPrimeField, rollover) {
  al_uint_t p("571203188563142862548892342071");
  BigPrimeField::set_prime(p);
  BigPrimeField result("571203188563142862548892342071");
  BigPrimeField oracle(0);
  EXPECT_EQ(result, oracle);
}

}
