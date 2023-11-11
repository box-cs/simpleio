#ifndef SIMPLE_IO_H
#define SIMPLE_IO_H

#include <algorithm>
#include <concepts>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace io {

template <typename T>
concept IOWriteable = requires(T a) {
  { std::cout << a };
};

template <typename T>
concept IOReadable = requires(T a) {
  { std::cin >> a };
};

template <typename T>
concept IterableContainer = requires(T a) {
  { a.begin() };
  { a.end() };
};

template <typename T>
concept IOStreamableContainer = requires(T a) {
  {
    std::for_each(a.begin(), a.end(), [](const auto &value) {
      static_assert(IOWriteable<decltype(value)>);
      static_assert(IOReadable<decltype(value)>);
    })
  };
};

/// @brief Generic input function that takes a message and a delimeter
/// @tparam T return type
/// @param message optional string message
/// @param delimeter optional char delimeter
template <typename T>
  requires IOReadable<T>
T input(std::string message, char delimeter = '\n') {
  std::cout << message;
  T result;
  if (std::cin.peek() == '\n') {
    std::cin.ignore();
  }
  if constexpr (std::is_same_v<T, std::string>) {
    std::getline(std::cin >> std::ws, result, delimeter);
    result = result.substr(0, result.find(' '));
  } else {
    std::cin >> result;
  }
  return result;
}

/// @brief Generic function that tokenizes from stdin
/// @tparam T return type std::vector<T>
/// @param message optional string message
/// @param delimeter optional char delimeter
template <typename T>
  requires IOReadable<T>
std::vector<T> tokenize(std::string message = "", char delimeter = '\n') {
  std::cout << message;
  std::vector<T> results = {};
  std::string token = "";
  if (std::cin.peek() == '\n') {
    std::cin.ignore();
  }
  std::getline(std::cin >> std::ws, token, delimeter);
  std::istringstream iss(token);
  T value;
  while (iss >> value) {
    results.push_back(value);
    if constexpr (std::is_same_v<T, std::string>) {
      token.clear();
    }
  }

  return results;
}

/// @brief This function applies a transformation to each element in a given
/// container, and then returns a new container that contains the transformed
/// elements. The original container remains unchanged.
/// @tparam T type of the elements in the source container
/// @tparam K type of the elements in the destination container
/// @param iterable container to be mapped over
/// @param lambda function that takes a T and returns a K
/// @return
template <typename T, typename K>
  requires IterableContainer<std::vector<T>>
std::vector<K> map_from_into(const std::vector<T> &iterable,
                             std::function<K(const T &)> lambda) {
  std::vector<K> tokens = {};
  for (const T &value : iterable) {
    tokens.push_back(lambda(value));
  }
  return tokens;
}

/// @brief `std::for_each` with less boilerplate
/// @param iterable container to be iterated over
/// @param lambda function that takes a T and returns void
void inline for_each(auto &iterable, auto &&lambda) {
  std::for_each(iterable.cbegin(), iterable.cend(), lambda);
}

/// @brief `std::transform` with less boilerplate
/// @param iterable container to be transformed
/// @param lambda function that takes a T and returns a transformed T
auto inline transform(auto &iterable, auto &&lambda) {
  std::transform(iterable.cbegin(), iterable.cend(), iterable.begin(), lambda);
}

/// @brief  Maps over a container and returns a new container with transformed
/// elements. The original container remains unchanged.
/// @param iterable container to be mapped over
/// @param lambda function that takes a T and returns a transformed T
/// @return a new container with transformed elements
auto inline map(auto &iterable, auto &&lambda) {
  auto copy = std::vector(iterable);
  transform(copy, lambda);
  return copy;
}

/// @brief Prints a container to stdout
void inline print(auto &iterable, char delimeter = ' ') {
  static_assert(IOStreamableContainer<decltype(iterable)>);
  for_each(iterable,
           [&](const auto &value) { std::cout << value << delimeter; });
  std::cout << '\n';
}
#endif /* SIMPLE_IO_H */
}
