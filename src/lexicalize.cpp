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

// Define a macro for the list of names
#define NAMES \
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
    X(fifteen) \
    X(sixteen) \
    X(seventeen) \
    X(eighteen) \
    X(nineteen) \
    X(twenty) \
    X(twenty_one) \
    X(twenty_two)

// Generate the structured binding with the macro
#define X(name) name,
#define BINDINGS NAMES
#undef X

// Generate the lambda function with the macro
#define X(name) if (name) { return __COUNTER__ - 1; } else
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
                R"([a-zA-Z]{0,2}?(("|')(\2{2})?)((?:[^\\"]|\\.|\\)*\1)?|)" //capture strings - check later on if string
                //                  prefix is valid and the string terminates
                R"((#[^\r\n]*)|)"                                          //capture comments
                R"(([\n\r][ \t]*)|)"                                       //capture newlines
                R"((\\[^\r\n]*)|)"                                         //capture \TheBackslashAndAnythingAfterIt
                R"(()"
                R"((\.{3})|)"                                          //capture ...
                R"((->)|)"                                             //capture ->
                //fucking floating point numbers
                R"((\d[\d_]*\.[\d_]*\d[\d_]*[eE]-?[\d_]*)|)"           //capture exponential floating point literals
                R"((\d[\d_]*\.[\d_]*\d[\d_]*[\w]*)|)"                  //capture floating point literals -> \d.\d [suffix]
                R"((\d[\d_]*\.[eE]-?[\d_]*)|)"                         //capture exponential floating point literals
                R"((\d[\d_]*\.\w*)|)"                                  //capture floating point literals -> \d.   [suffix]
                R"((\.\d[\d_]*[eE]-?[\d_]*)|)"                         //capture exponential floating point literals
                R"((\.\d[\d_]*\w*)|)"                                  //capture floating point literals ->   .\d [suffix]
                R"((\d[\d_]*[eE]-?[\d_]*)|)"                           //capture exponential literals
                R"(([<>*\/]{2})=?|)"                                   //capture 2-3 character operators
                R"(([!%&*+\-<=>@\/\\^|:]=))"                           //capture 2 caracter operators
                R"()|)"
                R"((\{\}|\(\)|\[\])|)"                                     //capture empty braces. Due to the fact that theres nothing in
                //   them we can combine them as a single token
                R"(([!-\/:-@\[-^{-~])|([^\s!-\/:-@\[-^{-~]+)|)"              //capture anything else
                R"((\s+))"                                                 //capture whitespace in order to keep track of position with ctre
        };
  // clang-format on
  const auto &matches =
      ctre::search_all<TokenRegex>(filedata);
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
      auto && [zero, one, two, three, four, five,
           six, seven, eight, nine, ten, eleven, twelve,
           thirteen, fourteen, fifteen, sixteen, seventeen,
           eighteen, nineteen, twenty, twenty_one,
           twenty_two,twenty_three] = match;

   // Lambda function
        auto check_match = [](const auto &zero, const auto &one, const auto &two, const auto &three, const auto &four,
                              const auto &five, const auto &six, const auto &seven, const auto &eight, const auto &nine,
                              const auto &ten, const auto &eleven, const auto &twelve, const auto &thirteen, const auto &fourteen,
                              const auto &fifteen, const auto &sixteen, const auto &seventeen, const auto &eighteen, const auto &nineteen,
                              const auto &twenty, const auto &twenty_one, const auto &twenty_two, const auto &twenty_three) {
            if (one) { return 1; }
            else if (two) { return 2; }
            else if (three) { return 3; }
            else if (four) { return 4; }
            else if (five) { return 5; }
            else if (six) { return 6; }
            else if (seven) { return 7; }
            else if (eight) { return 8; }
            else if (nine) { return 9; }
            else if (ten) { return 10; }
            else if (eleven) { return 11; }
            else if (twelve) { return 12; }
            else if (thirteen) { return 13; }
            else if (fourteen) { return 14; }
            else if (fifteen) { return 15; }
            else if (sixteen) { return 16; }
            else if (seventeen) { return 17; }
            else if (eighteen) { return 18; }
            else if (nineteen) { return 19; }
            else if (twenty) { return 20; }
            else if (twenty_one) { return 21; }
            else if (twenty_two) { return 22; }
            else if (twenty_three) {return 23;}
            return -1; // or some default value indicating no match
        };

        int result = check_match(zero, one, two, three, four, five, six, seven, eight, nine,
                                 ten, eleven, twelve, thirteen, fourteen, fifteen, sixteen, seventeen,
                                 eighteen, nineteen, twenty, twenty_one, twenty_two, twenty_three);

        std::cout << result << " [" << match.to_string() << "]\n";

    lexemes.push_back(
        {.str = str,
         .x = col,
         .y = row});
    str[0] == '\n' || str[0] == '\r' ? ++row, col = 0 : col += str.length();
  }
}
