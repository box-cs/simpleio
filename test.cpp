#include "io.hpp"
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <format>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using namespace io;

TEST_CASE("Generic stdin int input parses successfully", "[input<int>]") {
  // As int
  const auto num = input<int>("Input a number: ");
  REQUIRE(num == 5);
  REQUIRE(std::is_same_v<decltype(num), const int>);
}

TEST_CASE("Generic stdin double input parses successfully", "[input<double>]") {
  // As double
  const auto num = input<double>("Input a double: ");
  REQUIRE(num == 10);
  REQUIRE(std::is_same_v<decltype(num), const double>);
}

TEST_CASE("Generic stdin string input parses successfully",
          "[input<std::string>]") {
  // As string
  const auto str = input<string>("Input a word: ");
  REQUIRE(str == "hello");
  REQUIRE(std::is_same_v<decltype(str), const string>);
}

TEST_CASE("Generic stdin int inputs parses successfully", "[tokenize<int>]") {
  //  As a vector<int>
  const auto values =
      tokenize<int>("Input some integers [delimeted by space]: ");
  REQUIRE(values == vector<int>{1, 2, 3, 4, 5});
  REQUIRE(std::is_same_v<decltype(values), const vector<int>>);
}

TEST_CASE("Generic stdin double inputs parses successfully",
          "[tokenize<double>]") {
  // As a vector<double>
  const auto values = tokenize<double>("Input some doubles: ");
  REQUIRE(values == vector<double>{1.0, 2.0, 3.0, 4.0, 5.0});
  REQUIRE(std::is_same_v<decltype(values), const vector<double>>);
}

TEST_CASE("Generic stdin string input parses successfully",
          "[tokenize<string>]") {
  // As a vector<string>
  const auto str = tokenize<string>("Input a sentence: ");
  REQUIRE(str == vector<string>{"this", "is", "a", "sentence"});
  REQUIRE(std::is_same_v<decltype(str), const vector<string>>);
}

TEST_CASE("Generic map over vector<int>", "[map<int>]") {
  const auto values = vector<int>{1, 2, 3, 4, 5};
  const auto f = [](const int &x) { return x * 2; };

  const auto mappedValues = map(values, f);
  REQUIRE(values == vector<int>{1, 2, 3, 4, 5});
  REQUIRE(mappedValues == vector<int>{2, 4, 6, 8, 10});
  REQUIRE(std::is_same_v<decltype(mappedValues), const std::vector<int>>);
}

TEST_CASE("Generic map over vector<double>", "[map<double>]") {
  const auto values = vector<double>{1.0, 2.0, 3.0, 4.0, 5.0};
  const auto f = [](const double &x) { return x * 2; };

  const auto mapped = map(values, f);
  REQUIRE(values == vector<double>{1.0, 2.0, 3.0, 4.0, 5.0});
  REQUIRE(mapped == vector<double>{2, 4, 6, 8, 10});
  REQUIRE(std::is_same_v<decltype(mapped), const std::vector<double>>);
}

TEST_CASE("Generic map from vector<int> to vector<string>",
          "[map_from_into<int, std::string>]") {
  const auto values = vector<int>{1, 2, 3, 4, 5};
  const auto f = [](const int x) { return std::format("Value!: {}", x * 2); };

  const auto strings = map_from_into<int, std::string>(values, f);
  REQUIRE(strings == vector<string>{"Value!: 2", "Value!: 4", "Value!: 6",
                                    "Value!: 8", "Value!: 10"});
  REQUIRE(std::is_same_v<decltype(strings), const std::vector<string>>);
}