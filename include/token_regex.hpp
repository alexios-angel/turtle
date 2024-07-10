#ifndef TOKEN_REGEX_H
#define TOKEN_REGEX_H

#include "ctre.hpp"

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
                R"((?<complete_quote>(?<quotes>"|')(\g{quotes}{2})?)(?:(?:[^\\"]|\\.|\\)*\g{complete_quote})?|)" //capture strings - check later on if string
                R"(((#|\\)[^\r\n]*)|)"                                          //capture comments
                R"((\d+)|)"
                R"(([!%&*+\-<=>@\/\\^|:])|)"                           //capture 1 caracter operators
                //   them we can combine them as a single token
                R"(([!-\/:-@\[-^{-~])|([^\s!-\/:-@\[-^{-~]+)|)"              //capture anything else
                R"((\s+))"                                                 //capture whitespace in order to keep track of position with ctre
        };
// clang-format on

#endif
