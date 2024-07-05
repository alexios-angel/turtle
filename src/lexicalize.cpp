//
// Created by alex on 11/11/21.
//
#include <iostream>
#include "boost/foreach.hpp"
#include "global.hpp"
#include "node.hpp"
#include <ctre.hpp>
#include <string>
#include <string_view>
#include <vector>
#include "lexicalize.hpp"
#include <stdint.h>
#include <type_traits>


// Primary template
template <typename... Args>
struct RegexResultsNumberOfTemplateArgs;

// Specialization for variadic template
template <typename... Args>
struct RegexResultsNumberOfTemplateArgs<const ctre::regex_results<Args...>> {
    static constexpr std::size_t value = sizeof...(Args);
};

template <size_t MaxDepth, size_t N = 1, typename Match>
int get_matching_group(const Match& match) {
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

void lexicalize(std::string &filedata, std::vector<turtle::lexeme_t> &lexemes) {
  // clang-format off
/*
Paste into regex101.com
Replace regex comments with )"${2}R"( by using (\(\?#([^)]*)\))|^

[a-zA-Z]{0,2}?(("|')(\2{2})?)((?:[^\\"]|\\.|\\)*\1)?|(?#   //capture strings - check later on if string
                                                           //                  prefix is valid and the string terminates
)(#[^\r\n]*)|(?#                                           //capture comments
)([\n\r][ \t]*)|(?#                                        //capture newlines
)(\\[^\r\n]*)|(?#                                          //capture \TheBackslashAndAnythingAfterIt
)((?#
    )(\.{3})|(?#                                           //capture ...
    )(->)|(?#                                              //capture ->
                                                           //fucking floating point numbers
    )(\d[\d_]*\.[\d_]*\d[\d_]*[eE]-?[\d_]*)|(?#            //capture exponential floating point literals
    )(\d[\d_]*\.[\d_]*\d[\d_]*[\w]*)|(?#                   //capture floating point literals -> \d.\d [suffix]
    )(\d[\d_]*\.[eE]-?[\d_]*)|(?#                          //capture exponential floating point literals
    )(\d[\d_]*\.\w*)|(?#                                   //capture floating point literals -> \d.   [suffix]
    )(\.\d[\d_]*[eE]-?[\d_]*)|(?#                          //capture exponential floating point literals
    )(\.\d[\d_]*\w*)|(?#                                   //capture floating point literals ->   .\d [suffix]
    )(\d[\d_]*[eE]-?[\d_]*)|(?#                            //capture exponential literals
    )([<>*\/]{2})=?|(?#                                    //capture 2-3 character operators
    )([!%&*+\-<=>@\/\\^|:]=)(?#                            //capture 2 caracter operators
))|(?#
)([!-\/:-@\[-^{-~]|[^\s!-\/:-@\[-^{-~]+)|(?#               //capture anything else
)(\s+)                                                     //capture whitespace in order to keep track of position with ctre
*/
        // clang-format off
        //when rEgEX is A LaNGUAgE
        static constexpr ctll::fixed_string TokenRegex {
                R"((?<complete_quote>(?<quotes>"|')(\g{quotes}{2})?)(?:(?:[^\\"]|\\.|\\)*\g{complete_quote})?|)" //capture strings - check later on if string
                R"((#[^\r\n]*)|)"                                          //capture comments
                R"((\\[^\r\n]*)|)"                                         //capture \TheBackslashAndAnythingAfterIt
                R"((\.{3})|)"                                          //capture ...
                R"((->)|)"                                             //capture ->
                R"((\d+)|)"
                R"(([<>*\/]{2})=?|)"                                   //capture 2-3 character operators
                R"(([!%&*+\-<=>@\/\\^|:]=)|)"                           //capture 2 caracter operators
                R"((\{\}|\(\)|\[\])|)"                                     //capture empty braces. Due to the fact that theres nothing in
                //   them we can combine them as a single token
                R"(([!-\/:-@\[-^{-~])|)"
                R"(([^\s!-\/:-@\[-^{-~]+)|)"                            //capture anything else
                R"((\s+))"                                                 //capture whitespace in order to keep track of position with ctre
        };
  // clang-format on
  const auto &matches =
      ctre::tokenize<TokenRegex>(filedata);
  // std::distance is not constexpr thus it does not work with ctre
  auto distance = [](const auto &first, const auto &last) {
    size_t i = 0;
    for (auto it = first; it != last; ++it) {
      ++i;
    }
    return i;
  };

  lexemes.reserve(distance(matches.begin(), matches.end()));

  unsigned int col = 0, row = 0;
  for (const auto &match : matches) {
      const auto &str = match.to_view();
      constexpr size_t num_of_vars = RegexResultsNumberOfTemplateArgs<typeof(match)>::value;
      size_t group = get_matching_group<num_of_vars, 1>(match);
      std::cout << group << " [" << match.to_view()<<  "]\n";

    lexemes.push_back(
        {.str = str,
         .group = group});
  }
}
