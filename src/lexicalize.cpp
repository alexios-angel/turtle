//
// Created by alex on 11/11/21.
//
#include "lexicalize.hpp"
#include "boost/foreach.hpp"
#include "ctre.hpp"
#include "global.hpp"
#include "node.hpp"
#include "token_regex.hpp"
#include <iostream>
#include <stdint.h>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

// Primary template
template <typename... Args> struct RegexResultsNumberOfTemplateArgs;

// Specialization for variadic template
template <typename... Args>
struct RegexResultsNumberOfTemplateArgs<const ctre::regex_results<Args...>> {
  static constexpr std::size_t value = sizeof...(Args);
};

template <size_t MaxDepth, size_t N = 1, typename Match>
int get_matching_group(const Match &match) {
  if constexpr (N >= MaxDepth) {
    return -1; // No matching group found within MaxDepth
  } else {
    if (match.template get<N>()) {
      return N;
    } else {
      return get_matching_group<MaxDepth, N + 1>(match);
    }
  }
}

void lexicalize(std::string &filedata, std::vector<turtle::node_t> &lexemes) {

  const auto &matches = ctre::tokenize<LexRegex>(filedata);
  // std::distance is not constexpr thus it does not work with ctre
  auto distance = [&](const auto &first, const auto &last) {
    size_t i = 0;
    for (auto it = first; it != last; ++it) {
      ++i;
    }
    return i;
  };

  lexemes.reserve(distance(matches.begin(), matches.end()));

  for (const auto &match : matches) {
    const auto &str = match.to_view();
    constexpr size_t num_of_vars =
        RegexResultsNumberOfTemplateArgs<typeof(match)>::value;
    size_t group = get_matching_group<num_of_vars, 1>(match);
    // std::cout << group << " [" << match.to_view()<<  "]\n";

    lexemes.push_back({.token = {.str = str, .group = group}});
  }
}

void set_lex_groups(std::vector<turtle::node_t> &lexemes) {
  static constexpr std::pair<const char *, uint32_t> example_tokens[] = {
      {"\"string\"", turtle::token::flag::Data::DATA_TYPE_STRING}};
}
