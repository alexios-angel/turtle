//
// Created by alex on 11/11/21.
//

#ifndef TURTLE_LANG_ANTLR_LEXICALIZE_H
#define TURTLE_LANG_ANTLR_LEXICALIZE_H
#include "node.hpp"

namespace turtle::langs::antlr {


/*
 * Unfortunately std::map can not be constexpr-ed
 * Although memory is cheap, the less used the better
 * And it is my opinion to allocations on the stack make
 * for a safer garentee that the program will run effectively,
 * efficently, and fast
 */
constexpr auto turtleBuiltinTokenMap =
    std::to_array<std::pair<std::string_view, uint64_t>>(
        {{",", token::flag::Operator::DELIMITER_COMMA},
         {";", token::flag::Operator::DELIMITER_SEMICOLON},
         {":", token::flag::Operator::DELIMITER_COLON},
         {"(", token::flag::Operator::DELIMITER_CURVED_LEFT_BRACE},
         {")", token::flag::Operator::DELIMITER_CURVED_RIGHT_BRACE},
         {"{", token::flag::Operator::DELIMITER_CURLY_LEFT_BRACE},
         {"}", token::flag::Operator::DELIMITER_CURLY_RIGHT_BRACE},
         {"[", token::flag::Operator::DELIMITER_SQUARE_LEFT_BRACE},
         {"]", token::flag::Operator::DELIMITER_SQUARE_RIGHT_BRACE},
         {"...", token::flag::Operator::DELIMITER_ELLIPSIS},
         {".", token::flag::Operator::DELIMITER_PERIOD},
         {"=", token::flag::Operator::DELIMITER_ASSIGN},
         {"->", token::flag::Operator::DELIMINAR_RARROW},
         {":=", token::flag::Control::UNSUPPORTED}, // NOTE -- walrus
         {"@", token::flag::Operator::DELIMITER_AT_SIGN},
         {"@=", token::flag::Operator::ARITHMETIC_AT_ASSIGN},
         {"+", token::flag::Arithmetic::ARITHMETIC_ADD},
         {"-", token::flag::Arithmetic::ARITHMETIC_SUB},
         {"*", token::flag::Arithmetic::ARITHMETIC_MULL},
         {"/", token::flag::Arithmetic::ARITHMETIC_DIV},
         {"%", token::flag::Arithmetic::ARITHMETIC_MOD},
         {">", token::flag::Arithmetic::ARITHMETIC_GREATER_THAN},
         {"<", token::flag::Arithmetic::ARITHMETIC_LESS_THAN},
         {"&", token::flag::Arithmetic::ARITHMETIC_BIT_AND},
         {"|", token::flag::Arithmetic::ARITHMETIC_BIT_OR},
         {"^", token::flag::Arithmetic::ARITHMETIC_BIT_XOR},
         {"~", token::flag::Arithmetic::ARITHMETIC_BIT_NOT},
         {"!", token::flag::Arithmetic::ARITHMETIC_NOT},
         {"==", token::flag::Arithmetic::ARITHMETIC_EQUAL_TO},
         {"!=", token::flag::Arithmetic::ARITHMETIC_NOT_EQUAL},
         {"//", token::flag::Arithmetic::ARITHMETIC_FLOOR},
         {"**", token::flag::Arithmetic::ARITHMETIC_EXPONENTIAL},
         {"<<", token::flag::Arithmetic::ARITHMETIC_BIT_LEFT_SHIFT},
         {">>", token::flag::Arithmetic::ARITHMETIC_BIT_LEFT_SHIFT},
         {"+=", token::flag::Operator::ARITHMETIC_ADD_ASSIGN},
         {"-=", token::flag::Operator::ARITHMETIC_SUB_ASSIGN},
         {"*=", token::flag::Operator::ARITHMETIC_MULL_ASSIGN},
         {"/=", token::flag::Operator::ARITHMETIC_DIV_ASSIGN},
         {"%=", token::flag::Operator::ARITHMETIC_MOD_ASSIGN},
         {">=", token::flag::Arithmetic::ARITHMETIC_GREATER_THAN_EQUAL_TO},
         {"<=", token::flag::Arithmetic::ARITHMETIC_LESS_THAN_EQUAL_TO},
         {"//=", token::flag::Operator::ARITHMETIC_FLOOR_ASSIGN},
         {"**=", token::flag::Operator::ARITHMETIC_EXPONENTIAL_ASSIGN},
         {"&=", token::flag::Operator::ARITHMETIC_BIT_AND_ASSIGN},
         {"|=", token::flag::Operator::ARITHMETIC_BIT_OR_ASSIGN},
         {"^=", token::flag::Operator::ARITHMETIC_BIT_XOR_ASSIGN},

         //"~=" operator does not exist

         {"<<=", token::flag::Operator::ARITHMETIC_BIT_LEFT_SHIFT_ASSIGN},
         {">>=", token::flag::Operator::ARITHMETIC_BIT_RIGHT_SHIFT_ASSIGN},
         {"async", token::flag::Control::UNSUPPORTED},
         {"await", token::flag::Control::UNSUPPORTED},
         {"False", token::flag::Keyword::KEYWORD_FALSE},
         {"True", token::flag::Keyword::KEYWORD_TRUE},
         {"class", token::flag::Keyword::KEYWORD_CLASS},
         {"finally", token::flag::Keyword::KEYWORD_FINALLY},
         {"is", token::flag::Keyword::KEYWORD_IS},
         {"return", token::flag::Keyword::KEYWORD_RETURN},
         {"None", token::flag::Keyword::KEYWORD_NONE},
         {"continue", token::flag::Keyword::KEYWORD_CONTINUE},
         {"for", token::flag::Keyword::KEYWORD_FOR},
         {"lambda", token::flag::Keyword::KEYWORD_LAMBDA},
         {"try", token::flag::Keyword::KEYWORD_TRY},
         {"def", token::flag::Keyword::KEYWORD_DEF},
         {"from", token::flag::Keyword::KEYWORD_FROM},
         {"nonlocal", token::flag::Keyword::KEYWORD_NONLOCAL},
         {"while", token::flag::Keyword::KEYWORD_WHILE},
         {"and", token::flag::Keyword::KEYWORD_AND},
         {"del", token::flag::Keyword::KEYWORD_DEL},
         {"global", token::flag::Keyword::KEYWORD_GLOBAL},
         {"not", token::flag::Keyword::KEYWORD_NOT},
         {"with", token::flag::Keyword::KEYWORD_WITH},
         {"as", token::flag::Keyword::KEYWORD_AS},
         {"elif", token::flag::Keyword::KEYWORD_ELIF},
         {"if", token::flag::Keyword::KEYWORD_IF},
         {"or", token::flag::Keyword::KEYWORD_OR},
         {"yield", token::flag::Keyword::KEYWORD_YIELD},
         {"assert", token::flag::Keyword::KEYWORD_ASSERT},
         {"else", token::flag::Keyword::KEYWORD_ELSE},
         {"import", token::flag::Keyword::KEYWORD_IMPORT},
         {"pass", token::flag::Keyword::KEYWORD_PASS},
         {"break", token::flag::Keyword::KEYWORD_BREAK},
         {"except", token::flag::Keyword::KEYWORD_EXCEPT},
         {"in", token::flag::Keyword::KEYWORD_IN},
         {"raise", token::flag::Keyword::KEYWORD_RAISE}
});

void lexicalize(std::string &filedata,
                turtle::turtle_vector<turtle::node_t> &lexemes);
} // namespace turtle::lang::antlr

#endif // TURTLE_LEXICALIZE_H
