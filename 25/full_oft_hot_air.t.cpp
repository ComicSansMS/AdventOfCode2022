#include <full_oft_hot_air.hpp>

#include <catch.hpp>

TEST_CASE("Full of Hot Air")
{
    char const sample_input[] =
        "1=-0-2" "\n"
        "12111"  "\n"
        "2=0="   "\n"
        "21"     "\n"
        "2=01"   "\n"
        "111"    "\n"
        "20012"  "\n"
        "112"    "\n"
        "1=-1="  "\n"
        "1-12"   "\n"
        "12"     "\n"
        "1="     "\n"
        "122"    "\n";

    auto const numbers = parseInput(sample_input);

    SECTION("Parse Input")
    {
        REQUIRE(numbers.size() == 13);
        // 1=-0-2
        REQUIRE(numbers[0].digits.size() == 6);
        CHECK(numbers[0].digits[0] == SnafuDigit::Two);
        CHECK(numbers[0].digits[1] == SnafuDigit::MinusOne);
        CHECK(numbers[0].digits[2] == SnafuDigit::Zero);
        CHECK(numbers[0].digits[3] == SnafuDigit::MinusOne);
        CHECK(numbers[0].digits[4] == SnafuDigit::MinusTwo);
        CHECK(numbers[0].digits[5] == SnafuDigit::One);
        // 122
        REQUIRE(numbers[12].digits.size() == 3);
        CHECK(numbers[12].digits[0] == SnafuDigit::Two);
        CHECK(numbers[12].digits[1] == SnafuDigit::Two);
        CHECK(numbers[12].digits[2] == SnafuDigit::One);
    }

    SECTION("To Decimal")
    {
        CHECK(snafuToDecimal(parseNumber("1")) == 1);
        CHECK(snafuToDecimal(parseNumber("2")) == 2);
        CHECK(snafuToDecimal(parseNumber("1=")) == 3);
        CHECK(snafuToDecimal(parseNumber("1-")) == 4);
        CHECK(snafuToDecimal(parseNumber("10")) == 5);
        CHECK(snafuToDecimal(parseNumber("11")) == 6);
        CHECK(snafuToDecimal(parseNumber("12")) == 7);
        CHECK(snafuToDecimal(parseNumber("2=")) == 8);
        CHECK(snafuToDecimal(parseNumber("2-")) == 9);
        CHECK(snafuToDecimal(parseNumber("20")) == 10);
        CHECK(snafuToDecimal(parseNumber("1=0")) == 15);
        CHECK(snafuToDecimal(parseNumber("1-0")) == 20);
        CHECK(snafuToDecimal(parseNumber("1=11-2")) == 2022);
        CHECK(snafuToDecimal(parseNumber("1-0---0")) == 12345);
        CHECK(snafuToDecimal(parseNumber("1121-1110-1=0")) == 314159265);

        CHECK(snafuToDecimal(numbers[0]) == 1747);
        CHECK(snafuToDecimal(numbers[1]) == 906);
        CHECK(snafuToDecimal(numbers[2]) == 198);
        CHECK(snafuToDecimal(numbers[3]) == 11);
        CHECK(snafuToDecimal(numbers[4]) == 201);
        CHECK(snafuToDecimal(numbers[5]) == 31);
        CHECK(snafuToDecimal(numbers[6]) == 1257);
        CHECK(snafuToDecimal(numbers[7]) == 32);
        CHECK(snafuToDecimal(numbers[8]) == 353);
        CHECK(snafuToDecimal(numbers[9]) == 107);
        CHECK(snafuToDecimal(numbers[10]) == 7);
        CHECK(snafuToDecimal(numbers[11]) == 3);
        CHECK(snafuToDecimal(numbers[12]) == 37);
    }

    SECTION("Decimal To Snafu")
    {
        CHECK(fmt::format("{}", decimalToSnafu(0)) == "0");
        CHECK(fmt::format("{}", decimalToSnafu(1)) == "1");
        CHECK(fmt::format("{}", decimalToSnafu(2)) == "2");
        CHECK(fmt::format("{}", decimalToSnafu(3)) == "1=");
        CHECK(fmt::format("{}", decimalToSnafu(4)) == "1-");
        CHECK(fmt::format("{}", decimalToSnafu(5)) == "10");
        CHECK(fmt::format("{}", decimalToSnafu(6)) == "11");
        CHECK(fmt::format("{}", decimalToSnafu(7)) == "12");
        CHECK(fmt::format("{}", decimalToSnafu(8)) == "2=");
        CHECK(fmt::format("{}", decimalToSnafu(9)) == "2-");
        CHECK(fmt::format("{}", decimalToSnafu(10)) == "20");
        CHECK(fmt::format("{}", decimalToSnafu(15)) == "1=0");
        CHECK(fmt::format("{}", decimalToSnafu(20)) == "1-0");
        CHECK(fmt::format("{}", decimalToSnafu(2022)) == "1=11-2");
        CHECK(fmt::format("{}", decimalToSnafu(12345)) == "1-0---0");
        CHECK(fmt::format("{}", decimalToSnafu(314159265)) == "1121-1110-1=0");
        
        CHECK(decimalToSnafu(1747) == numbers[0]);
        CHECK(decimalToSnafu(906) == numbers[1]);
        CHECK(decimalToSnafu(198) == numbers[2]);
        CHECK(decimalToSnafu(11) == numbers[3]);
        CHECK(decimalToSnafu(201) == numbers[4]);
        CHECK(decimalToSnafu(31) == numbers[5]);
        CHECK(decimalToSnafu(1257) == numbers[6]);
        CHECK(decimalToSnafu(32) == numbers[7]);
        CHECK(decimalToSnafu(353) == numbers[8]);
        CHECK(decimalToSnafu(107) == numbers[9]);
        CHECK(decimalToSnafu(7) == numbers[10]);
        CHECK(decimalToSnafu(3) == numbers[11]);
        CHECK(decimalToSnafu(37) == numbers[12]);
    }

    SECTION("Answer")
    {
        CHECK(fmt::format("{}", answer(numbers)) == "2=-1=0");
    }
}
