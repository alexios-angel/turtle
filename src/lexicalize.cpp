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

// Define a macro for the list of names
#define NAMES(X) \
    X(zero) \
    X(one) \
    X(two) \
    X(three) \
    X(four) \
    X(five) \
    X(six) \
    X(seven) \
    X(eight) \
    X(nine) \
    X(ten) \
    X(eleven) \
    X(twelve) \
    X(thirteen) \
    X(fourteen) \
    X(fifteen)

// Generate the structured binding with the macro
#define COMMA_SEPARATE(name) name,
#define CONST_LOCAL_REFERENCE(value) auto && value,
#define BINDINGS NAMES
#undef X

#define GENERATE_ENUM_LIST(MACRO, NAME) \
   enum NAME : int                      \
   {                                    \
      MACRO(COMMA_SEPARATE)              \
     LAST_ENUM_NAME_ITEM                \
   };

GENERATE_ENUM_LIST(NAMES, name_values)

// Generate the lambda function with the macro
// "name && name_values::name" is a hacky way to filter out zero
#define LAMBDA_BODY NAMES return -1;
#undef X

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
                R"((("|')(\2{2})?)((?:[^\\"]|\\.|\\)*\1)?|)" //capture strings - check later on if string
                //                  prefix is valid and the string terminates
                R"((#[^\r\n]*)|)"                                          //capture comments
                R"(([\n\r]?[ \t]+)|)"                                       //capture newlines
                R"((\\[^\r\n]*)|)"                                         //capture \TheBackslashAndAnythingAfterIt
                R"((\.{3})|)"                                          //capture ...
                R"((->)|)"                                             //capture ->
                R"((\d+)|)"
                R"(([<>*\/]{2})=?|)"                                   //capture 2-3 character operators
                R"(([!%&*+\-<=>@\/\\^|:]=)|)"                           //capture 2 caracter operators
                R"((\{\}|\(\)|\[\])|)"                                     //capture empty braces. Due to the fact that theres nothing in
                //   them we can combine them as a single token
                R"(([!-\/:-@\[-^{-~])|([^\s!-\/:-@\[-^{-~]+)|)"              //capture anything else
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
      auto && [NAMES(COMMA_SEPARATE) LASTGROUP] = match;

        // Lambda function
        #define GENERATE_MATCH_IF(name) if (name) { i = name_values::name; }
        auto check_match = [](NAMES(CONST_LOCAL_REFERENCE)/*, */ auto && LASTGROUP) -> uint {
          uint8_t i = 0;
          NAMES(GENERATE_MATCH_IF)
          if (LASTGROUP) { i = LAST_ENUM_NAME_ITEM; }
          return i;
        };

        int result = check_match(NAMES(COMMA_SEPARATE) LASTGROUP);

        std::cout << result << " [" << match.to_string() << "]\n";

    lexemes.push_back(
        {.str = str,
         .x = col,
         .y = row});
    str[0] == '\n' || str[0] == '\r' ? ++row, col = 0 : col += str.length();
  }
}
