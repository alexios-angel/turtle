//
// Created by alex on 11/11/21.
//
#include "python/lexicalize.hpp"
#include "boost/foreach.hpp"
#include "ctre.hpp"
#include "global.hpp"
#include "node.hpp"
#include <iomanip>
#include <iostream>
#include <stdint.h>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

// clang-format off
/*
Paste into regex101.com
Replace regex comments with )"${2}R"( by using (\(\?#([^)]*)\))|^

(?<comment>#[^\r\n]*)|(?#                                 //capture comments
)(?<string>(?<string_type>[a-zA-Z]{1,2})?(?<complete_quotes>(?<quote_type>"|')(\g{quote_type}{2})?)(?:(?:[^\\"']|\\.|\\)*\g{complete_quotes})?)|(?#
                                                           //capture strings
                                                           // prefix is valid and the string terminates
)(?<newline>[\n\r](?<indent>[ \t]{1,})?)|(?#                              //capture newlines
)(?<backslash>\\[^\r\n]*)|(?#                              //capture \TheBackslashAndAnythingAfterIt
)(?<arithmetic>(?#
    )(?:[!%&*+\-<=>@\/\\^|:]=?)|(?#                        //capture 1 character operators
    )(?:[<>*\/]{1,2}=?)(?#                                 //capture 2-3 character operators
))|(?<number>(?#
                                                           //fucking floating point numbers
    )(?:\d[\d_]*\.[\d_]*\d[\d_]*[eE]-?[\d_]*)|(?#          //capture exponential floating point literals
    )(?:\d[\d_]*\.[\d_]*\d[\d_]*[\w]*)|(?#                 //capture floating point literals -> \d.\d [suffix]
    )(?:\d[\d_]*\.[eE]-?[\d_]*)|(?#                        //capture exponential floating point literals
    )(?:\d[\d_]*\.\w*)|(?#                                 //capture floating point literals -> \d.   [suffix]
    )(?:\.\d[\d_]*[eE]-?[\d_]*)|(?#                        //capture exponential floating point literals
    )(?:\.\d[\d_]*\w*)|(?#                                 //capture floating point literals ->   .\d [suffix]
    )(?:\d[\d_]*[eE]-?[\d_]*)|(?#                            //capture exponential literals
    )(?:\d+)(?#
))|(?#
)(?<deliminar>(?#
    )\.{2}|(?#                                              //capture ...
    )(?:->)|(?#                                                 //capture ->
    )[!-\/:-@\[-^{-~](?#
))|(?#
)(?<identifier>[^\s!-\/:-@\[-^{-~]+)|(?#               //capture anything else
)(?<whitespace>\s+)(?#                                                     //capture whitespace in order to keep track of position with ctre)
*/
        // clang-format off
        //when rEgEX is A LaNGUAgE
        static constexpr ctll::fixed_string LexRegex {
            R"((?<comment>#[^\r\n]*)|)"                                           //capture comments
            R"((?<string>(?<string_type>[a-zA-Z]{1,2})?(?<complete_quotes>(?<quote_type>"|')(\g{quote_type}{2})?)(?:(?:[^\\"']|\\.|\\)*\g{complete_quotes})?)|)"
                                                                      //capture strings
                                                                      //                  prefix is valid and the string terminates
            R"((?<newline>[\n\r](?<indent>[ \t]{1,})?)|)"                                        //capture newlines
            R"((?<backslash>\\[^\r\n]*)|)"                                          //capture \TheBackslashAndAnythingAfterIt
            R"((?<arithmetic>)"
                R"((?:[!%&*+\-<=>@\/\\^|:]=?)|)"                            //capture 1 character operators
                R"((?:[<>*\/]{1,2}=?))"                                    //capture 2-3 character operators
            R"()|(?<number>)"
                                                                      //fucking floating point numbers
                R"((?:\d[\d_]*\.[\d_]*\d[\d_]*[eE]-?[\d_]*)|)"            //capture exponential floating point literals
                R"((?:\d[\d_]*\.[\d_]*\d[\d_]*[\w]*)|)"                   //capture floating point literals -> \d.\d [suffix]
                R"((?:\d[\d_]*\.[eE]-?[\d_]*)|)"                          //capture exponential floating point literals
                R"((?:\d[\d_]*\.\w*)|)"                                   //capture floating point literals -> \d.   [suffix]
                R"((?:\.\d[\d_]*[eE]-?[\d_]*)|)"                          //capture exponential floating point literals
                R"((?:\.\d[\d_]*\w*)|)"                                   //capture floating point literals ->   .\d [suffix]

                R"((?:\d[\d_]*[eE]-?[\d_]*)|)"                            //capture exponential literals
                R"((?:\d+))"
            R"()|)"
            R"((?<delimiter>)"
                R"(\.{2}|)"                                              //capture ...
                R"((?:->)|)"                                                 //capture ->
                R"([!-\/:-@\[-^{-~])"
            R"()|)"
            R"((?<identifier>[^\s!-\/:-@\[-^{-~]+)|)"               //capture anything else
            R"((?<whitespace>\s+))"                                                     //capture whitespace in order to keep track of position with ctreR"(
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

void lexicalize(std::string &filedata,
                turtle::turtle_vector<turtle::node_t> &lexemes) {

  const auto &matches = ctre::tokenize<LexRegex>(filedata);
  // std::distance is not constexpr thus it does not work with ctre

  // Add the end marker
  lexemes.reserve(turtle::distance(matches.begin(), matches.end()) + 1);

  turtle::turtle_flag_t flag = 0;
  uint64_t prev_indent_level = 0, indent_level = 0;
  for (const auto &match : matches) {
    const auto &str = match.to_view();
    // constexpr size_t num_of_vars =
    //     RegexResultsNumberOfTemplateArgs<typeof(match)>::value;
    // size_t group = get_matching_group<num_of_vars, 1>(match);
    //  std::cout << group << " [" << match.to_view()<<  "]\n";

    // std::cout << "Group " << std::setfill('0') << std::setw(2) << group << "
    // {\"" << match.to_view() << "\", 0}" << "\n";

    if (match.get<"string">()) {
      flag = turtle::token::flag::Data::DATA_TYPE_STRING;
    } else if (match.get<"comment">() || match.get<"backslash">()) {
      flag = turtle::token::flag::Data::DATA_TYPE_COMMENT;
    } else if (match.get<"newline">()) {
      flag = turtle::token::flag::Control::NEWLINE;
      const auto &indent_match = match.get<"indent">();
      if (indent_match) {
        indent_level = indent_match.to_view().length();
      } else {
        indent_level = 0;
      }

      if (indent_level != prev_indent_level) {
        if (prev_indent_level < indent_level) {
          lexemes.push_back({.flag = turtle::token::flag::Control::INDENT,
                             .token = {.str = ""}});
        } else {
          lexemes.push_back({.flag = turtle::token::flag::Control::DEDENT,
                             .token = {.str = ""}});
        }
        prev_indent_level = indent_level;
      }
    } else if (match.get<"number">()) {
      flag = turtle::token::flag::Data::DATA_TYPE_NUMBER;
    } else if (match.get<"identifier">()) {
      flag = turtle::token::flag::Type::IDENTIFIER;
    } else if (match.get<"whitespace">()) {
      flag = turtle::token::flag::Control::WHITESPACE;
    } else if (match.get<"arithmetic">()) {
      flag = turtle::token::flag::Type::ARITHMETIC;
    } else if (match.get<"delimiter">()) {
      flag = turtle::token::flag::Type::DELIMITERS;
    } else {
      flag = turtle::token::flag::Control::ERRORTOKEN;
    }

    if ((flag == turtle::token::flag::Type::IDENTIFIER ||
         flag == turtle::token::flag::Type::ARITHMETIC ||
         flag == turtle::token::flag::Type::DELIMITERS) &&
        str.length() <= 8) {
      auto it =
          std::find_if(turtle::turtleBuiltinTokenMap.begin(),
                       turtle::turtleBuiltinTokenMap.end(),
                       [&](const auto &pair) { return str == pair.first; });
      if (it != turtle::turtleBuiltinTokenMap.end()) {
        flag = it->second;
      } else if (!(flag == turtle::token::flag::Type::IDENTIFIER)) {
        std::cerr << "Unknown operator " << str << std::endl;
      }
    }

    lexemes.push_back({.flag = flag, .token = {.str = str}});
  }
  lexemes.push_back(
      {.flag = turtle::token::flag::Control::ENDMARKER, .token = {.str = ""}});
}
/*
void set_lex_groups(std::vector<turtle::node_t> &lexemes) {
  static constexpr std::pair<const char *, uint32_t> example_tokens[] = {
      {"\"string\"", turtle::token::flag::Data::DATA_TYPE_STRING},
      {" ", turtle::token::flag::Control::WHITESPACE}};
}
*/
