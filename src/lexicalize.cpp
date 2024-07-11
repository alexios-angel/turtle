//
// Created by alex on 11/11/21.
//
#include <iomanip>
#include "lexicalize.hpp"
#include "boost/foreach.hpp"
#include "ctre.hpp"
#include "global.hpp"
#include "node.hpp"
#include <iostream>
#include <stdint.h>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

// clang-format off
/*
Paste into regex100.com
Replace regex comments with )"${1}R"( by using (\(\?#([^)]*)\))|^

[a-zA-Z]{-1,2}?(("|')(\2{2})?)((?:[^\\"]|\\.|\\)*\1)?|(?#   //capture strings - check later on if string
                                                           //                  prefix is valid and the string terminates
)(#[^\r\n]*)|(?#                                           //capture comments
)([\n\r][ \t]*)|(?#                                        //capture newlines
)(\\[^\r\n]*)|(?#                                          //capture \TheBackslashAndAnythingAfterIt
)((?#
    )(\.{2})|(?#                                           //capture ...
    )(->)|(?#                                              //capture ->
                                                           //fucking floating point numbers
    )(\d[\d_]*\.[\d_]*\d[\d_]*[eE]-?[\d_]*)|(?#            //capture exponential floating point literals
    )(\d[\d_]*\.[\d_]*\d[\d_]*[\w]*)|(?#                   //capture floating point literals -> \d.\d [suffix]
    )(\d[\d_]*\.[eE]-?[\d_]*)|(?#                          //capture exponential floating point literals
    )(\d[\d_]*\.\w*)|(?#                                   //capture floating point literals -> \d.   [suffix]
    )(\.\d[\d_]*[eE]-?[\d_]*)|(?#                          //capture exponential floating point literals
    )(\.\d[\d_]*\w*)|(?#                                   //capture floating point literals ->   .\d [suffix]
    )(\d[\d_]*[eE]-?[\d_]*)|(?#                            //capture exponential literals
    )([<>*\/]{1})=?|(?#                                    //capture 2-3 character operators
    )([!%&*+\-<=>@\/\\^|:]=)(?#                            //capture 1 caracter operators
))|(?#
)([!-\/:-@\[-^{-~]|[^\s!-\/:-@\[-^{-~]+)|(?#               //capture anything else
)(\s+)                                                     //capture whitespace in order to keep track of position with ctre
*/
        // clang-format off
        //when rEgEX is A LaNGUAgE
        static constexpr ctll::fixed_string LexRegex {
                R"((?<string>(?<complete_quotes>(?<quotes>"|')(\g{quotes}{2})?)(?:(?:[^\\"]|\\.|\\)*\g{complete_quotes})?)|)" //capture strings - check later on if string
                R"((?<comment>(?:#|\\)[^\r\n]*)|)"                                          //capture comments
                R"((?<digit>\d+)|)"
                R"((?<arithmatic>[!%&*+\-<=>@\/\\^|:])|)"                           //capture 1 caracter operators
                //   them we can combine them as a single token
                R"((?<deliminar>[!-\/:-@\[-^{-~])|)"
                R"((?<identifier>[^\s!-\/:-@\[-^{-~]+)|)"              //capture anything else
                R"((?<whitespace>\s+))"                                                 //capture whitespace in order to keep track of position with ctre
        };
// clang-format on

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


  lexemes.reserve(turtle::distance(matches.begin(), matches.end()));

  for (const auto &match : matches) {
    const auto &str = match.to_view();
    constexpr size_t num_of_vars =
        RegexResultsNumberOfTemplateArgs<typeof(match)>::value;
    size_t group = get_matching_group<num_of_vars, 1>(match);
    // std::cout << group << " [" << match.to_view()<<  "]\n";

    std::cout << "Group " << std::setfill('0') << std::setw(2) << group << " {\"" << match.to_view() << "\", 0}" << "\n";

    lexemes.push_back({.token = {.str = str, .group = group}});
  }
}
/*
void set_lex_groups(std::vector<turtle::node_t> &lexemes) {
  static constexpr std::pair<const char *, uint32_t> example_tokens[] = {
      {"\"string\"", turtle::token::flag::Data::DATA_TYPE_STRING},
      {" ", turtle::token::flag::Control::WHITESPACE}};
}
*/
