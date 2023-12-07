#include "io.hpp"
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using namespace io;

// std::stringstream ss{"5\n10\nhello\nthis is a sentence\n1 2 "
//                      "3 4 5\n1.0 2.0 3.0 4.0 5.0\nthis is "
//                      "a sentence\n"};

template <typename T>
  requires IOReadable<T>
[[nodiscard]] T testInput(const std::string &&message, std::istream &istream) {
  return input<T>(std::string(message), ' ', '\n', istream);
}

static std::stringstream ss;

class SetupListener : public Catch::EventListenerBase {
public:
  using Catch::EventListenerBase::EventListenerBase;

  void testRunStarting(Catch::TestRunInfo const &) override {
    {
      ss = readFile("/home/box/source/cpp/lib/test.txt", []() { exit(1); });
    }
  }
};

CATCH_REGISTER_LISTENER(SetupListener)

TEST_CASE("Simple IO") {
  SECTION("Generic stdin int input parses successfully", "[input<int>]") {
    // As int
    const auto num = testInput<int>("Input a number: ", ss);
    REQUIRE(num == 5);
    REQUIRE(std::is_same_v<decltype(num), const int>);
  }

  SECTION("Generic stdin double input parses successfully", "[input<double>]") {
    // As double
    const auto num = testInput<double>("Input a double: ", ss);
    REQUIRE(num == 10);
    REQUIRE(std::is_same_v<decltype(num), const double>);
  }

  SECTION("Generic stdin string input parses until ' ' successfully",
          "[input<std::string>]") {
    // As string
    const auto str = testInput<string>("Input a word: ", ss);
    REQUIRE(str == "hello");
    REQUIRE(std::is_same_v<decltype(str), const string>);
  }

  SECTION("Generic stdin string input parses until '\\n' successfully",
          "[input<std::string>]") {
    // As string
    const auto str = input<string>("Input a sentence: ", '\n', '\n', ss);
    REQUIRE(str == "this is a sentence");
    REQUIRE(std::is_same_v<decltype(str), const string>);
  }

  SECTION("Generic stdin int inputs parses successfully", "[tokenize<int>]") {
    //  As a vector<int>
    const auto values =
        tokenize<int>("Input some integers [delimeted by space]: ", '\n', ss);
    REQUIRE(values == vector<int>{1, 2, 3, 4, 5});
    REQUIRE(std::is_same_v<decltype(values), const vector<int>>);
  }

  SECTION("Generic stdin double inputs parses successfully",
          "[tokenize<double>]") {
    // As a vector<double>
    const auto values = tokenize<double>("Input some doubles: ", '\n', ss);
    REQUIRE(values == vector<double>{1.0, 2.0, 3.0, 4.0, 5.0});
    REQUIRE(std::is_same_v<decltype(values), const vector<double>>);
  }

  SECTION("Generic stdin string input parses into vector<string> successfully",
          "[tokenize<string>]") {
    // As a vector<string>
    const auto str = tokenize<string>("Input a sentence: ", '\n', ss);
    REQUIRE(str == vector<string>{"this", "is", "a", "sentence"});
    REQUIRE(std::is_same_v<decltype(str), const vector<string>>);
  }

  SECTION("Generic map over vector<int>", "[map<int>]") {
    const auto values = vector<int>{1, 2, 3, 4, 5};
    const auto f = [](const int &x) { return x * 2; };

    const auto mappedValues = map(values, f);
    REQUIRE(values == vector<int>{1, 2, 3, 4, 5});
    REQUIRE(mappedValues == vector<int>{2, 4, 6, 8, 10});
    REQUIRE(std::is_same_v<decltype(mappedValues), const std::vector<int>>);
  }

  SECTION("Generic map over vector<double>", "[map<double>]") {
    const auto values = vector<double>{1.0, 2.0, 3.0, 4.0, 5.0};
    const auto f = [](const double &x) { return x * 2; };

    const auto mapped = map(values, f);
    REQUIRE(values == vector<double>{1.0, 2.0, 3.0, 4.0, 5.0});
    REQUIRE(mapped == vector<double>{2, 4, 6, 8, 10});
    REQUIRE(std::is_same_v<decltype(mapped), const std::vector<double>>);
  }

  SECTION("Generic map from vector<int> to vector<double>",
          "[map_from_into<int, std::string>]") {
    const auto values = vector<int>{1, 2, 3, 4, 5};
    const auto f = [](const int x) { return (double)x * 2; };

    const auto doubles = map_from_into<int, double>(values, f);
    REQUIRE(doubles == vector<double>{2.0, 4.0, 6.0, 8.0, 10.0});
    REQUIRE(std::is_same_v<decltype(doubles), const std::vector<double>>);
  }

  SECTION("Generic map from vector<int> to vector<string>",
          "[map_from_into<int, std::string>]") {
    const auto values = vector<int>{1, 2, 3, 4, 5};
    const auto f = [](const int x) { return std::format("Value!: {}", x * 2); };

    const auto strings = map_from_into<int, std::string>(values, f);
    REQUIRE(strings == vector<string>{"Value!: 2", "Value!: 4", "Value!: 6",
                                      "Value!: 8", "Value!: 10"});
    REQUIRE(std::is_same_v<decltype(strings), const std::vector<string>>);
  }
}