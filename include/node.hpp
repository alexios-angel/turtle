#ifndef TURTLE_TOKEN_H
#define TURTLE_TOKEN_H

/*
 * This file contains the magic numbers and data structure
 * definitions nessessary to parse Python. Cython does exist,
 * but it does not generate readable code that is a literal
 * translation of a file. With the introduction of c++2a,
 * almost all constructs used in python can be used in C++,
 * it's just the translation needs to be automated.
 * This is more of my try at reinventing the wheel.
 *
 * My motivations to do this to translate Python3 code
 * to C++20 in order to create C++ bash scripts that run fast.
 *
 * Throughout this file I make long comments -- these are not
 * ment for you, but me, as I have a terrible memory.
 *
 * The use of enums are really great as they can be
 * used for pre-calculated magic numbers.
 * What we need to do is take the "magic" out of magic numbers.
 *
 * Currently the the token flag is represented by a 32 bit unsigned integer.
 *
 * Different Types of tokens are separated into classes;
 * Separating tokens into classes makes it possible to compresses
 * the information and give each types of token its unique "fingerprint"
 * or id.
 *
 * Once a specific token has been identified as some sort of predefined or
 * identifier it's token string is deleted from program memory, as
 * it is more efficent to just store the identifying "fingerprint" of a token
 * then a multi character string.
 *      Example:
 *          The '(' left brace token is predefined, give it the numeric id
 *          DELIMITER_CURVED_LEFT_BRACE, delete token string and go to
 *          the next token
 *
 * At the parse tree stage, each deliminar          is a     terminal,
 *                          each identifier or data is a non-terminal
 *
 */

#include <cmath>
#include <cstdint>
#include <cstring>
#include <string>
#include <uchar.h>

#include "global.hpp"

//#include <boost/multiprecision/cpp_int.hpp>
//#include <boost/multiprecision/cpp_dec_float.hpp>
#include <any>

namespace turtle {
// In case I want to change it to something bigger in the future
typedef char32_t turtle_flag_t;

template <class T> using turtle_vector = std::basic_string<T>;

typedef long turtle_int;
typedef turtle_int turtle_float;
typedef std::any turtle_any;
using std::any_cast;

typedef turtle_int t_int;
typedef turtle_float t_float;
typedef turtle_any t_any;
/*struct turtle_num {
    turtle_int sig_digits = 0;
    turtle_int num        = 0;
    constexpr turtle_num(const std::string_view& _n){
        num = static_cast<turtle_int>(_n);
    }
};*/
} // namespace turtle

//      M_turtle_flag(N) (0000'0000 0000'0000 0000'0000 0000'0001 <<  N )
#define M_turtle_flag(N)                                                       \
  (static_cast<turtle::turtle_flag_t>(1u)                                      \
   << static_cast<turtle::turtle_flag_t>(N))

consteval turtle::turtle_flag_t constexpr_ceil(const float num) {
  return (static_cast<float>(static_cast<turtle::turtle_flag_t>(num)) == num)
             ? static_cast<turtle::turtle_flag_t>(num)
             : static_cast<turtle::turtle_flag_t>(num) + ((num > 0) ? 1 : 0);
}

consteval size_t Log2(const size_t n) {
  return ((n < 2) ? 0 : 1 + Log2(n / 2));
}
consteval size_t num_of_bits_required(const size_t max) {
  return constexpr_ceil(Log2(max) + 1);
}
#define bit_range_will_not_overflow(r1, r2)                                    \
  (((r1) + (r2)) < bits_in_turtle_flag)
#define bit_range_will_not_underflow(r1) ((r1) >= 0)
#define CHECK_BIT_RANGE(BITS_NEEDED, MESSAGE)                                  \
  static_assert(bit_range_will_not_overflow(                                   \
                    num_of_bits_required_for_token_type, (BITS_NEEDED)) &&     \
                    bit_range_will_not_underflow(BITS_NEEDED),                 \
                MESSAGE)

#if 0 // def DEBUG_CPP
#include "enum.h"
#define TURTLE_CLASS(ENAME, ...)                                               \
  BETTER_ENUM(ENAME, turtle_flag, __VA_ARGS__, NUMBER_OF_ENUMS)
#else
#define TURTLE_CLASS(ENAME, ...)                                               \
  struct ENAME {                                                               \
    enum : turtle_flag_t { __VA_ARGS__, NUMBER_OF_ENUMS };                     \
  };
#endif

namespace turtle::token {
#define ENUM_NAME Type
TURTLE_CLASS(ENUM_NAME, ASCII, CONTROL,
             DELIMITERS, // such as '(' or ')' or '.' or '[' or ']' ','
             ARITHMETIC,
             KEYWORD,    // any builtin type
             DATA,       // such as a number or string
             IDENTIFIER, // any label
             GROUP       // such as a stmt
)
#undef ENUM_NAME
} // namespace turtle::token

constexpr auto num_of_bits_required_for_token_type =
    num_of_bits_required(turtle::token::Type::NUMBER_OF_ENUMS);
constexpr auto bits_in_turtle_flag = (sizeof(turtle::turtle_flag_t) * 8);
// make constexpr in order to reduce compile time
constexpr auto tokenTypeOffset =
    (bits_in_turtle_flag - num_of_bits_required_for_token_type);

consteval turtle::turtle_flag_t maxOfTokenType(turtle::turtle_flag_t typeflag) {
  turtle::turtle_flag_t i = 0;
  i = ~i;
  i <<= num_of_bits_required_for_token_type;
  i >>= num_of_bits_required_for_token_type;
  i |= typeflag;
  return i;
}

CHECK_BIT_RANGE(num_of_bits_required(turtle::token::Type::NUMBER_OF_ENUMS),
                "Too many token types");

namespace turtle {
// std::vector<struct Node> SyntaxGroups
//          ||
//          \/
// std::vector<struct Node> SemanticGroups

#if 0 // DEBUG_CPP
template <class T> constexpr const char *flagstr(T values, turtle_flag flag) {
  for (const auto &v : values) {
    if (v._to_integral() == flag) {
      return v._to_string();
    }
  }
  return "";
}
#endif

namespace token {

#define ENUM_NAME Control
TURTLE_CLASS(ENUM_NAME, NULL_TOKEN, HAS_VALUE = NULL_TOKEN, NEWLINE, WHITESPACE,
             INDENT, DEDENT, ENDMARKER, ERROR, ERRORTOKEN,
             TokenError, UNSUPPORTED)
#undef ENUM_NAME

#define ENUM_NAME Data
TURTLE_CLASS(ENUM_NAME, DATA_TYPE_STRING,
             DATA_TYPE_RAW, // Used in combination with the string flag
             DATA_TYPE_FORMATTED,
             DATA_TYPE_FORMAT_TYPE, // "%s" printf style or "{}".format style?
             DATA_TYPE_UNICODE,

             DATA_TYPE_NUMBER, DATA_TYPE_INT, DATA_TYPE_FLOAT,
             DATA_TYPE_COMPLEX, DATA_TYPE_EXPONENTIAL, DATA_TYPE_HEX,
             DATA_TYPE_OCTAL, DATA_TYPE_BINARY,
             DATA_TYPE_, // unknown -- make effort to determine type
             DATA_TYPE_COMMENT)
#undef /*cuz its*/ ENUM_NAME // sun

#define ENUM_NAME Operator
TURTLE_CLASS(ENUM_NAME,
             DELIMITER_AT_SIGN, // '@' Python decorator, akin to passing a
                                // function pointer
             DELIMITER_COLON,   // ':' symbol
             DELIMITER_SEMICOLON, DELIMITER_COMMA,
             DELIMITER_PERIOD,   // access token '.'
             DELIMITER_ELLIPSIS, // ...
             DELIMINAR_RARROW, DELIMITER_BRACE,
             // DELIMITER_ACCESS,

             DELIMITER_LEFT_OR_RIGHT_BRACE = DELIMITER_BRACE +
                                             1u, // 0 = Left, 1 = Right
             DELIMITER_CURVED_BRACE,             // '(' or ')'
             DELIMITER_SQUARE_BRACE,             // '[' or ']'
             DELIMITER_CURLY_BRACE,              // '{' or '}'
             DELIMITER_ASSIGN                    // '=' symbol
)
#undef ENUM_NAME

// like identifiers these will be represented as integers
#define ENUM_NAME Keyword
TURTLE_CLASS(ENUM_NAME, KEYWORD_FALSE, KEYWORD_CLASS, KEYWORD_FINALLY,
             KEYWORD_IS, KEYWORD_RETURN, KEYWORD_NONE, KEYWORD_CONTINUE,
             KEYWORD_FOR, KEYWORD_LAMBDA, KEYWORD_TRY, KEYWORD_TRUE,
             KEYWORD_DEF, KEYWORD_FROM, KEYWORD_NONLOCAL, KEYWORD_WHILE,
             KEYWORD_AND, KEYWORD_DEL, KEYWORD_GLOBAL, KEYWORD_NOT,
             KEYWORD_WITH, KEYWORD_AS, KEYWORD_ELIF, KEYWORD_IF, KEYWORD_OR,
             KEYWORD_YIELD, KEYWORD_ASSERT, KEYWORD_ELSE, KEYWORD_IMPORT,
             KEYWORD_PASS, KEYWORD_BREAK, KEYWORD_EXCEPT, KEYWORD_IN,
             KEYWORD_RAISE)
#undef ENUM_NAME

#define ENUM_NAME Arithmetic
TURTLE_CLASS(ENUM_NAME,
             ARITHMETIC_OPERATION = 0u, // 1 = ARITHMETIC, 0 = LOGICAL (greater
                                        // than, less than, equal to, not)

             ARITHMETIC_ADD, ARITHMETIC_SUB, ARITHMETIC_MULL, ARITHMETIC_DIV,
             ARITHMETIC_MOD, ARITHMETIC_FLOOR, ARITHMETIC_EXPONENTIAL,
             ARITHMETIC_BIT_AND, ARITHMETIC_BIT_OR, ARITHMETIC_BIT_XOR,
             ARITHMETIC_BIT_NOT, ARITHMETIC_BIT_LEFT_SHIFT,
             ARITHMETIC_BIT_RIGHT_SHIFT,

             ARITHMETIC_EQUAL_TO = 1, // LOGICAL CLASS
             ARITHMETIC_GREATER_THAN, ARITHMETIC_LESS_THAN,
             ARITHMETIC_NOT //,

             // ARITHMETIC_NOT_EQUAL_TO,

             // ARITHMETIC_GREATER_THAN_EQUAL_TO,

             // ARITHMETIC_LESS_THAN_EQUAL_TO,
)
#undef ENUM_NAME

#define ENUM_NAME Group
TURTLE_CLASS(
    ENUM_NAME, annotated_rhs, annotation, args, arguments, as_pattern,
    assert_stmt, assignment, assignment_expression, atom, attr, augassign,
    await_primary, bitwise_and, bitwise_or, bitwise_xor, block, capture_pattern,
    case_block, class_def, class_def_raw, class_pattern, closed_pattern,
    compare_op_bitwise_or_pair, comparison, complex_number, compound_stmt,
    conjunction, decorators, default_t, del_stmt, del_t_atom, del_target,
    del_targets, dict, dictcomp, disjunction, dotted_as_name, dotted_as_names,
    dotted_name, double_star_pattern, double_starred_kvpair,
    double_starred_kvpairs, elif_stmt, else_block, eq_bitwise_or, eval,
    except_block, except_star_block, expression, expressions, factor, file,
    finally_block, for_if_clause, for_if_clauses, for_stmt, fstring,
    fstring_conversion, fstring_format_spec, fstring_full_format_spec,
    fstring_middle, fstring_replacement_field, fstringstring, func_type,
    func_type_comment, function_def, function_def_raw, genexp, global_stmt,
    group, group_pattern, gt_bitwise_or, gte_bitwise_or, guard, if_stmt,
    imaginary_number, import_from, import_from_as_name, import_from_as_names,
    import_from_targets, import_name, import_stmt, in_bitwise_or, interactive,
    invalid_default, inversion, is_bitwise_or, isnot_bitwise_or, items_pattern,
    key_value_pattern, keyword_pattern, keyword_patterns, kvpair,
    kwarg_or_double_starred, kwarg_or_starred, kwargs, kwds, lambda_kwds,
    lambda_param, lambda_param_maybe_default, lambda_param_no_default,
    lambda_param_with_default, lambda_parameters, lambda_params,
    lambda_slash_no_default, lambda_slash_with_default, lambda_star_etc,
    lambdef, list, listcomp, literal_expr, literal_pattern, lt_bitwise_or,
    lte_bitwise_or, mapping_pattern, match_stmt, maybe_sequence_pattern,
    maybe_star_pattern, name_or_attr, named_expression, nonlocal_stmt,
    noteq_bitwise_or, notin_bitwise_or, open_sequence_pattern, or_pattern,
    param, param_maybe_default, param_no_default,
    param_no_default_star_annotation, param_star_annotation, param_with_default,
    parameters, params, pattern, pattern_capture_target, patterns,
    positional_patterns, power, primary, raise_stmt, real_number, return_stmt,
    sequence_pattern, set, setcomp, shift_expr, signed_number,
    signed_real_number, simple_stmt, simple_stmts,
    single_subscript_attribute_target, single_target, slash_no_default,
    slash_with_default, slice, slices, star_annotation, star_atom, star_etc,
    star_expression, star_expressions, star_named_expression,
    star_named_expressions, star_pattern, star_target, star_targets,
    star_targets_list_seq, star_targets_tuple_seq, starred_expression,
    statement, statement_newline, statements, string, strings, subject_expr,
    sum, t_lookahead, t_primary, target_with_star_atom, term, try_stmt, tuple,
    type_alias, type_expressions, type_param, type_param_bound, type_param_seq,
    type_params, value_pattern, while_stmt, wildcard_pattern, with_item,
    with_stmt, yield_expr, yield_stmt )
#undef ENUM_NAME

namespace flag {

/*
 * Below are the precomputed flags
 *
 * This program strives for low memeory use at the cost of speed
 *
 *  TokenFlags
 *
 *    MSB    LSB
 *    |      |
 * #0 0000...[Ascii character] - > ASCII
 * #1 0001...[Type of Control] - > CONTROL
 * #2 0010...[Type of Delim]   - > DELIMITERS
 * #3 0011...[Type of Arith]   - > ARITHMETIC
 * #4 0100...[Type of Keyword] - > KEYWORD
 * #5 0101...[Type of Data]    - > DATA
 * #6 0111...[Numeric id]      - > IDENTIFIER
 * #7 1000
 *
 * The MSB in the token flags segment will be the IDENTIFIER flag.
 * If the token is not an IDENTIFIER, The token
 * types 2-6 will have a numeric id that can be extracted via
 *      (TokenFlag >> ( ( sizeof(TURTLES_FLAG_DATA_TYPE) * 8 ) - 4 ) )
 */

// Refer to the huge comment in the flag namespace on wtf this is & does
#define M_typeFlagMacro(N) (N << tokenTypeOffset)
#define ENUM_NAME Type
TURTLE_CLASS(ENUM_NAME,
            // ASCII missing on purpose
            CONTROL = M_typeFlagMacro(token::ENUM_NAME::CONTROL),
             DELIMITERS = M_typeFlagMacro(token::ENUM_NAME::DELIMITERS),
             ARITHMETIC = M_typeFlagMacro(token::ENUM_NAME::ARITHMETIC),
             KEYWORD = M_typeFlagMacro(token::ENUM_NAME::KEYWORD),
             DATA = M_typeFlagMacro(token::ENUM_NAME::DATA),
             IDENTIFIER = M_typeFlagMacro(token::ENUM_NAME::IDENTIFIER),
             GROUP = M_typeFlagMacro(token::ENUM_NAME::GROUP))
#undef ENUM_NAME
/*
 *
 *   ┌──> Flag Type - Control Class Id
 *   │
 * ┌─┤┌───> Is null token
 * 00100000  00000000 00000000 00000000
 *
 *
 *
 *
 *   ┌──> Flag Type - Control Class Id
 *   │
 * ┌─┤┌───> Is not null
 * 00111111  11111111 11111111 11111111
 *     │└─────────────────────────────┴──> Amount Of whitespace
 *     └────> Is newline
 */

#define ENUM_NAME Control
TURTLE_CLASS(ENUM_NAME, NULL_TOKEN = 0u | flag::Type::CONTROL,
             NEWLINE = M_turtle_flag(token::ENUM_NAME::NEWLINE) |
                       M_turtle_flag(token::ENUM_NAME::HAS_VALUE) |
                       flag::Type::CONTROL,
             WHITESPACE = M_turtle_flag(token::ENUM_NAME::WHITESPACE) |
                          M_turtle_flag(token::ENUM_NAME::HAS_VALUE) |
                          flag::Type::CONTROL,
             INDENT = M_turtle_flag(token::ENUM_NAME::INDENT) |
                          M_turtle_flag(token::ENUM_NAME::HAS_VALUE) |
                          flag::Type::CONTROL,
             DEDENT = M_turtle_flag(token::ENUM_NAME::DEDENT) |
                          M_turtle_flag(token::ENUM_NAME::HAS_VALUE) |
                          flag::Type::CONTROL,
             ENDMARKER = M_turtle_flag(token::ENUM_NAME::ENDMARKER) |
                         M_turtle_flag(token::ENUM_NAME::HAS_VALUE) |
                         flag::Type::CONTROL,
             ERRORTOKEN, TokenError, UNSUPPORTED,
             TOKENMAX = maxOfTokenType(flag::Type::CONTROL))
#undef offset
#undef ENUM_NAME

/*
    Any identifier Tokens are represented by a numeric id,
    To get the tokens numeric id perform
        ( NOT(flag::IDENTIFIER) AND Node.NodeFlag )


      ┌──> Flag Type - IDENTIFIER Class Id
      │
    ┌─┤
    11100000  00000000 00000000 00000000
       └───────────────────────────────┴──> Numeric Id
*/

#define ENUM_NAME Identifier
TURTLE_CLASS(ENUM_NAME, TOKENMAX = maxOfTokenType(flag::Type::IDENTIFIER))
#undef ENUM_NAME

/*

   //Strings
      ┌──> Flag Type - DATA Class Id
      │
    ┌─┤
    10100001  00000000 00000000 00000000
       ││││└──> Is string
       │││└───> Is raw string
       ││└────> Is formated string
       │└─────> Format Type
       └──────> Is unicode string
 */
#define ENUM_NAME Data
TURTLE_CLASS(
    ENUM_NAME,
    DATA_TYPE_STRING = M_turtle_flag(token::ENUM_NAME::DATA_TYPE_STRING) |
                       flag::Type::DATA,
    DATA_TYPE_RAW = M_turtle_flag(token::ENUM_NAME::DATA_TYPE_RAW) |
                    flag::Type::DATA,
    DATA_TYPE_FORMATTED = M_turtle_flag(token::ENUM_NAME::DATA_TYPE_FORMATTED) |
                          flag::Type::DATA,
    PRINTF_STYLE_FORMAT =
        M_turtle_flag(token::ENUM_NAME::DATA_TYPE_FORMAT_TYPE) |
        flag::Type::DATA,
    DATA_TYPE_UNICODE = M_turtle_flag(token::ENUM_NAME::DATA_TYPE_UNICODE) |
                        flag::Type::DATA,

    DATA_TYPE_RAW_STRING = (flag::ENUM_NAME::DATA_TYPE_RAW |
                            flag::ENUM_NAME::DATA_TYPE_STRING),
    DATA_TYPE_FORMATED_STRING = (flag::ENUM_NAME::DATA_TYPE_FORMATTED |
                                 flag::ENUM_NAME::DATA_TYPE_STRING),
    DATA_TYPE_FORMATED_PRINTF_STRING =
        (flag::ENUM_NAME::PRINTF_STYLE_FORMAT |
         flag::ENUM_NAME::DATA_TYPE_FORMATED_STRING),
    DATA_TYPE_FORMATED_PYTHON_STRING = DATA_TYPE_FORMATED_STRING,
    DATA_TYPE_UNICODE_STRING = (flag::ENUM_NAME::DATA_TYPE_UNICODE |
                                flag::ENUM_NAME::DATA_TYPE_STRING),
    DATA_TYPE_FORMATED_UNICODE_STRING =
        (flag::ENUM_NAME::DATA_TYPE_FORMATED_STRING |
         flag::ENUM_NAME::DATA_TYPE_UNICODE_STRING),

    DATA_TYPE_COMMENT = M_turtle_flag(token::ENUM_NAME::DATA_TYPE_COMMENT) |
                        flag::Type::DATA,
    DATA_TYPE_NUMBER = M_turtle_flag(token::ENUM_NAME::DATA_TYPE_NUMBER) |
                       flag::Type::DATA,
    DATA_TYPE_INT = M_turtle_flag(token::ENUM_NAME::DATA_TYPE_INT) |
                    flag::ENUM_NAME::DATA_TYPE_NUMBER,
    DATA_TYPE_FLOAT = M_turtle_flag(token::ENUM_NAME::DATA_TYPE_FLOAT) |
                      flag::ENUM_NAME::DATA_TYPE_NUMBER,
    DATA_TYPE_COMPLEX = M_turtle_flag(token::ENUM_NAME::DATA_TYPE_COMPLEX) |
                        flag::ENUM_NAME::DATA_TYPE_NUMBER,
    DATA_TYPE_EXPONENTIAL =
        M_turtle_flag(token::ENUM_NAME::DATA_TYPE_EXPONENTIAL) |
        flag::ENUM_NAME::DATA_TYPE_NUMBER,
    DATA_TYPE_HEX = M_turtle_flag(token::ENUM_NAME::DATA_TYPE_HEX) |
                    flag::ENUM_NAME::DATA_TYPE_INT,
    DATA_TYPE_OCTAL = M_turtle_flag(token::ENUM_NAME::DATA_TYPE_OCTAL) |
                      flag::ENUM_NAME::DATA_TYPE_INT,
    DATA_TYPE_BINARY = M_turtle_flag(token::ENUM_NAME::DATA_TYPE_BINARY) |
                       flag::ENUM_NAME::DATA_TYPE_INT,
    TOKENMAX = maxOfTokenType(flag::Type::DATA))
#undef ENUM_NAME
#undef DataShiftToMargin

/*
 * The DELIMITER_ASSIGN and DELIMITER_BRACE group is marked by their LSB being
 * set
 *
 *
 *   ┌──> Flag Type - Deliminar Class Id
 *   │                            ┌──> Other Deliminar tokens
 *   │                            │  ┌──> DELIMITER_BRACE token class
 * ┌─┤                          ┌─┴─┐│┌───> DELIMITER_ASSIGN operator class
 * 01000000  00000000 00000000 01111111
 *
 * DELIMITER_ASSIGN operator class:
 * Any Deliminar that has the 1st LSB flag set is a ASSIGN-ment
 *
 * DELIMITER_BRACE token class:
 * Any Deliminar that has the 2nd LSB flag set is a brace
 */

// The below may look cryptic,
// But heres whats happening:
//
// Shift an arithmetic integer to go next to the DELIMITER_ASSIGN flag
// (DELIMITER_ASSIGN + 1 )
//
// Then give it the DELIMITER_ASSIGN flag

#define ENUM_NAME Operator
// offset the arithmetic tokens to be next to the DELIMITER_ASSIGN token
#define DeliminarAssignOffset_M(x)                                             \
  (x << (token::ENUM_NAME::DELIMITER_ASSIGN + 1))
TURTLE_CLASS(
    ENUM_NAME,
    DELIMITER_ASSIGN = M_turtle_flag(token::ENUM_NAME::DELIMITER_ASSIGN) |
                       flag::Type::DELIMITERS,

    ARITHMETIC_ADD_ASSIGN =
        DeliminarAssignOffset_M(token::Arithmetic::ARITHMETIC_ADD) |
        flag::ENUM_NAME::DELIMITER_ASSIGN,
    ARITHMETIC_SUB_ASSIGN =
        DeliminarAssignOffset_M(token::Arithmetic::ARITHMETIC_SUB) |
        flag::ENUM_NAME::DELIMITER_ASSIGN,
    ARITHMETIC_MULL_ASSIGN =
        DeliminarAssignOffset_M(token::Arithmetic::ARITHMETIC_MULL) |
        flag::ENUM_NAME::DELIMITER_ASSIGN,
    ARITHMETIC_DIV_ASSIGN =
        DeliminarAssignOffset_M(token::Arithmetic::ARITHMETIC_DIV) |
        flag::ENUM_NAME::DELIMITER_ASSIGN,
    ARITHMETIC_MOD_ASSIGN =
        DeliminarAssignOffset_M(token::Arithmetic::ARITHMETIC_MOD) |
        flag::ENUM_NAME::DELIMITER_ASSIGN,
    ARITHMETIC_FLOOR_ASSIGN =
        DeliminarAssignOffset_M(token::Arithmetic::ARITHMETIC_FLOOR) |
        flag::ENUM_NAME::DELIMITER_ASSIGN,
    ARITHMETIC_EXPONENTIAL_ASSIGN =
        DeliminarAssignOffset_M(token::Arithmetic::ARITHMETIC_EXPONENTIAL) |
        flag::ENUM_NAME::DELIMITER_ASSIGN,
    ARITHMETIC_BIT_AND_ASSIGN =
        DeliminarAssignOffset_M(token::Arithmetic::ARITHMETIC_BIT_AND) |
        flag::ENUM_NAME::DELIMITER_ASSIGN,
    ARITHMETIC_BIT_OR_ASSIGN =
        DeliminarAssignOffset_M(token::Arithmetic::ARITHMETIC_BIT_OR) |
        flag::ENUM_NAME::DELIMITER_ASSIGN,
    ARITHMETIC_BIT_XOR_ASSIGN =
        DeliminarAssignOffset_M(token::Arithmetic::ARITHMETIC_BIT_XOR) |
        flag::ENUM_NAME::DELIMITER_ASSIGN,
    ARITHMETIC_BIT_LEFT_SHIFT_ASSIGN =
        DeliminarAssignOffset_M(token::Arithmetic::ARITHMETIC_BIT_LEFT_SHIFT) |
        flag::ENUM_NAME::DELIMITER_ASSIGN,
    ARITHMETIC_BIT_RIGHT_SHIFT_ASSIGN =
        DeliminarAssignOffset_M(token::Arithmetic::ARITHMETIC_BIT_RIGHT_SHIFT) |
        flag::ENUM_NAME::DELIMITER_ASSIGN,

    DELIMITER_AT_SIGN = M_turtle_flag(token::ENUM_NAME::DELIMITER_AT_SIGN) |
                        flag::Type::DELIMITERS,
    //@=
    ARITHMETIC_AT_ASSIGN = flag::ENUM_NAME::DELIMITER_AT_SIGN |
                           flag::ENUM_NAME::DELIMITER_ASSIGN,
    DELIMINAR_RARROW = M_turtle_flag(token::ENUM_NAME::DELIMINAR_RARROW) |
                       flag::Type::DELIMITERS,
    DELIMITER_COLON = M_turtle_flag(token::ENUM_NAME::DELIMITER_COLON) |
                      flag::Type::DELIMITERS,
    DELIMITER_SEMICOLON = M_turtle_flag(token::ENUM_NAME::DELIMITER_SEMICOLON) |
                          flag::Type::DELIMITERS,
    DELIMITER_COMMA = M_turtle_flag(token::ENUM_NAME::DELIMITER_COMMA) |
                      flag::Type::DELIMITERS,
    DELIMITER_PERIOD = M_turtle_flag(token::ENUM_NAME::DELIMITER_PERIOD) |
                       flag::Type::DELIMITERS,
    DELIMITER_ELLIPSIS = M_turtle_flag(token::ENUM_NAME::DELIMITER_ELLIPSIS) |
                         flag::Type::DELIMITERS,
    DELIMITER_ACCESS = DELIMITER_PERIOD,

    DELIMITER_BRACE = M_turtle_flag(token::ENUM_NAME::DELIMITER_BRACE) |
                      flag::Type::DELIMITERS,
    DELIMITER_LEFT_BRACE =
        M_turtle_flag(token::ENUM_NAME::DELIMITER_LEFT_OR_RIGHT_BRACE) |
        flag::Type::DELIMITERS, // 1 = Left, 0 = Right

    DELIMITER_CURVED_BRACE =
        M_turtle_flag(token::ENUM_NAME::DELIMITER_CURVED_BRACE) |
        flag::Type::DELIMITERS,
    DELIMITER_SQUARE_BRACE =
        M_turtle_flag(token::ENUM_NAME::DELIMITER_SQUARE_BRACE) |
        flag::Type::DELIMITERS,
    DELIMITER_CURLY_BRACE =
        M_turtle_flag(token::ENUM_NAME::DELIMITER_CURLY_BRACE) |
        flag::Type::DELIMITERS,

    DELIMITER_CURVED_RIGHT_BRACE = (DELIMITER_CURVED_BRACE | DELIMITER_BRACE),
    DELIMITER_CURVED_LEFT_BRACE = (DELIMITER_CURVED_BRACE |
                                   DELIMITER_LEFT_BRACE | DELIMITER_BRACE),
    DELIMITER_SQUARE_RIGHT_BRACE = (DELIMITER_SQUARE_BRACE | DELIMITER_BRACE),
    DELIMITER_SQUARE_LEFT_BRACE = (DELIMITER_SQUARE_BRACE |
                                   DELIMITER_LEFT_BRACE | DELIMITER_BRACE),
    DELIMITER_CURLY_RIGHT_BRACE = (DELIMITER_CURLY_BRACE | DELIMITER_BRACE),
    DELIMITER_CURLY_LEFT_BRACE = (DELIMITER_CURLY_BRACE | DELIMITER_LEFT_BRACE |
                                  DELIMITER_BRACE),
    TOKENMAX = maxOfTokenType(flag::Type::DELIMITERS))

#undef DeliminarAssignOffset_M
#undef ENUM_NAME

#define ENUM_NAME Keyword
TURTLE_CLASS(
    ENUM_NAME,
    KEYWORD_FALSE = token::ENUM_NAME::KEYWORD_FALSE | flag::Type::KEYWORD,
    KEYWORD_CLASS = token::ENUM_NAME::KEYWORD_CLASS | flag::Type::KEYWORD,
    KEYWORD_FINALLY = token::ENUM_NAME::KEYWORD_FINALLY | flag::Type::KEYWORD,
    KEYWORD_IS = token::ENUM_NAME::KEYWORD_IS | flag::Type::KEYWORD,
    KEYWORD_RETURN = token::ENUM_NAME::KEYWORD_RETURN | flag::Type::KEYWORD,
    KEYWORD_NONE = token::ENUM_NAME::KEYWORD_NONE | flag::Type::KEYWORD,
    KEYWORD_CONTINUE = token::ENUM_NAME::KEYWORD_CONTINUE | flag::Type::KEYWORD,
    KEYWORD_FOR = token::ENUM_NAME::KEYWORD_FOR | flag::Type::KEYWORD,
    KEYWORD_LAMBDA = token::ENUM_NAME::KEYWORD_LAMBDA | flag::Type::KEYWORD,
    KEYWORD_TRY = token::ENUM_NAME::KEYWORD_TRY | flag::Type::KEYWORD,
    KEYWORD_TRUE = token::ENUM_NAME::KEYWORD_TRUE | flag::Type::KEYWORD,
    KEYWORD_DEF = token::ENUM_NAME::KEYWORD_DEF | flag::Type::KEYWORD,
    KEYWORD_FROM = token::ENUM_NAME::KEYWORD_FROM | flag::Type::KEYWORD,
    KEYWORD_NONLOCAL = token::ENUM_NAME::KEYWORD_NONLOCAL | flag::Type::KEYWORD,
    KEYWORD_WHILE = token::ENUM_NAME::KEYWORD_WHILE | flag::Type::KEYWORD,
    KEYWORD_AND = token::ENUM_NAME::KEYWORD_AND | flag::Type::KEYWORD,
    KEYWORD_DEL = token::ENUM_NAME::KEYWORD_DEL | flag::Type::KEYWORD,
    KEYWORD_GLOBAL = token::ENUM_NAME::KEYWORD_GLOBAL | flag::Type::KEYWORD,
    KEYWORD_NOT = token::ENUM_NAME::KEYWORD_NOT | flag::Type::KEYWORD,
    KEYWORD_WITH = token::ENUM_NAME::KEYWORD_WITH | flag::Type::KEYWORD,
    KEYWORD_AS = token::ENUM_NAME::KEYWORD_AS | flag::Type::KEYWORD,
    KEYWORD_ELIF = token::ENUM_NAME::KEYWORD_ELIF | flag::Type::KEYWORD,
    KEYWORD_IF = token::ENUM_NAME::KEYWORD_IF | flag::Type::KEYWORD,
    KEYWORD_OR = token::ENUM_NAME::KEYWORD_OR | flag::Type::KEYWORD,
    KEYWORD_YIELD = token::ENUM_NAME::KEYWORD_YIELD | flag::Type::KEYWORD,
    KEYWORD_ASSERT = token::ENUM_NAME::KEYWORD_ASSERT | flag::Type::KEYWORD,
    KEYWORD_ELSE = token::ENUM_NAME::KEYWORD_ELSE | flag::Type::KEYWORD,
    KEYWORD_IMPORT = token::ENUM_NAME::KEYWORD_IMPORT | flag::Type::KEYWORD,
    KEYWORD_PASS = token::ENUM_NAME::KEYWORD_PASS | flag::Type::KEYWORD,
    KEYWORD_BREAK = token::ENUM_NAME::KEYWORD_BREAK | flag::Type::KEYWORD,
    KEYWORD_EXCEPT = token::ENUM_NAME::KEYWORD_EXCEPT | flag::Type::KEYWORD,
    KEYWORD_IN = token::ENUM_NAME::KEYWORD_IN | flag::Type::KEYWORD,
    KEYWORD_RAISE = token::ENUM_NAME::KEYWORD_RAISE | flag::Type::KEYWORD,
    TOKENMAX = maxOfTokenType(flag::Type::KEYWORD))
#undef ENUM_NAME

/*
 * The '=' token is classified as a deliminar;
 * It is NOT an ARITHMETIC token
 */

/*
 * The type of ARITHMETIC_OPERATION is set by the LSB
 * When the LSB is 1, it is of the ARITHMETIC_OPERATION class
 * When the LSB is 0, it is of the LOGICAL_OPERATION class
 *
 *   ┌─ Flag Type - ARITHMETIC Class Id
 * ┌─┤                               ┌───> ARITHMETIC_OPERATION class
 * 01100000 00000000 00000000 00000001
 *
 *
 *   ┌─ Flag Type - ARITHMETIC Class Id
 * ┌─┤                               ┌───> LOGICAL_OPERATION class
 * 01100000 00000000 00000000 00000000
 */
#define ENUM_NAME Arithmetic
TURTLE_CLASS(
    ENUM_NAME,
    ARITHMETIC_OPERATION =
        M_turtle_flag(token::ENUM_NAME::ARITHMETIC_OPERATION) |
        flag::Type::ARITHMETIC,

    ARITHMETIC_ADD = M_turtle_flag(token::ENUM_NAME::ARITHMETIC_ADD) |
                     flag::ENUM_NAME::ARITHMETIC_OPERATION,
    ARITHMETIC_SUB = M_turtle_flag(token::ENUM_NAME::ARITHMETIC_SUB) |
                     flag::ENUM_NAME::ARITHMETIC_OPERATION,
    ARITHMETIC_MULL = M_turtle_flag(token::ENUM_NAME::ARITHMETIC_MULL) |
                      flag::ENUM_NAME::ARITHMETIC_OPERATION,
    ARITHMETIC_DIV = M_turtle_flag(token::ENUM_NAME::ARITHMETIC_DIV) |
                     flag::ENUM_NAME::ARITHMETIC_OPERATION,
    ARITHMETIC_MOD = M_turtle_flag(token::ENUM_NAME::ARITHMETIC_MOD) |
                     flag::ENUM_NAME::ARITHMETIC_OPERATION,
    ARITHMETIC_FLOOR = M_turtle_flag(token::ENUM_NAME::ARITHMETIC_FLOOR) |
                       flag::ENUM_NAME::ARITHMETIC_OPERATION,
    ARITHMETIC_EXPONENTIAL =
        M_turtle_flag(token::ENUM_NAME::ARITHMETIC_EXPONENTIAL) |
        flag::ENUM_NAME::ARITHMETIC_OPERATION,
    ARITHMETIC_BIT_AND = M_turtle_flag(token::ENUM_NAME::ARITHMETIC_BIT_AND) |
                         flag::ENUM_NAME::ARITHMETIC_OPERATION,
    ARITHMETIC_BIT_OR = M_turtle_flag(token::ENUM_NAME::ARITHMETIC_BIT_OR) |
                        flag::ENUM_NAME::ARITHMETIC_OPERATION,
    ARITHMETIC_BIT_XOR = M_turtle_flag(token::ENUM_NAME::ARITHMETIC_BIT_XOR) |
                         flag::ENUM_NAME::ARITHMETIC_OPERATION,
    ARITHMETIC_BIT_NOT = M_turtle_flag(token::ENUM_NAME::ARITHMETIC_BIT_NOT) |
                         flag::ENUM_NAME::ARITHMETIC_OPERATION,
    ARITHMETIC_BIT_LEFT_SHIFT =
        M_turtle_flag(token::ENUM_NAME::ARITHMETIC_BIT_LEFT_SHIFT) |
        flag::ENUM_NAME::ARITHMETIC_OPERATION,
    ARITHMETIC_BIT_RIGHT_SHIFT =
        M_turtle_flag(token::ENUM_NAME::ARITHMETIC_BIT_RIGHT_SHIFT) |
        flag::ENUM_NAME::ARITHMETIC_OPERATION,

    ARITHMETIC_EQUAL_TO = M_turtle_flag(token::ENUM_NAME::ARITHMETIC_EQUAL_TO) |
                          flag::Type::ARITHMETIC,

    ARITHMETIC_NOT_EQUAL = M_turtle_flag(token::ENUM_NAME::ARITHMETIC_NOT) |
                           flag::ENUM_NAME::ARITHMETIC_EQUAL_TO,

    ARITHMETIC_GREATER_THAN =
        M_turtle_flag(token::ENUM_NAME::ARITHMETIC_GREATER_THAN) |
        flag::Type::ARITHMETIC,

    ARITHMETIC_LESS_THAN =
        M_turtle_flag(token::ENUM_NAME::ARITHMETIC_LESS_THAN) |
        flag::Type::ARITHMETIC,

    ARITHMETIC_NOT = M_turtle_flag(token::ENUM_NAME::ARITHMETIC_NOT) |
                     flag::Type::ARITHMETIC,

    ARITHMETIC_GREATER_THAN_EQUAL_TO =
        (flag::ENUM_NAME::ARITHMETIC_EQUAL_TO |
         flag::ENUM_NAME::ARITHMETIC_GREATER_THAN),

    ARITHMETIC_LESS_THAN_EQUAL_TO = (flag::ENUM_NAME::ARITHMETIC_EQUAL_TO |
                                     flag::ENUM_NAME::ARITHMETIC_LESS_THAN),

    TOKENMAX = maxOfTokenType(flag::Type::ARITHMETIC))
#undef ENUM_NAME

#define ENUM_NAME Group
TURTLE_CLASS(ENUM_NAME,
annotated_rhs = token::ENUM_NAME::annotated_rhs | flag::Type::GROUP, //
annotation = token::ENUM_NAME::annotation | flag::Type::GROUP, //
args = token::ENUM_NAME::args | flag::Type::GROUP, //
arguments = token::ENUM_NAME::arguments | flag::Type::GROUP, //
as_pattern = token::ENUM_NAME::as_pattern | flag::Type::GROUP, //
assert_stmt = token::ENUM_NAME::assert_stmt | flag::Type::GROUP, //
assignment = token::ENUM_NAME::assignment | flag::Type::GROUP, //
assignment_expression = token::ENUM_NAME::assignment_expression | flag::Type::GROUP, //
atom = token::ENUM_NAME::atom | flag::Type::GROUP, //
attr = token::ENUM_NAME::attr | flag::Type::GROUP, //
augassign = token::ENUM_NAME::augassign | flag::Type::GROUP, //
await_primary = token::ENUM_NAME::await_primary | flag::Type::GROUP, //
bitwise_and = token::ENUM_NAME::bitwise_and | flag::Type::GROUP, //
bitwise_or = token::ENUM_NAME::bitwise_or | flag::Type::GROUP, //
bitwise_xor = token::ENUM_NAME::bitwise_xor | flag::Type::GROUP, //
block = token::ENUM_NAME::block | flag::Type::GROUP, //
capture_pattern = token::ENUM_NAME::capture_pattern | flag::Type::GROUP, //
case_block = token::ENUM_NAME::case_block | flag::Type::GROUP, //
class_def = token::ENUM_NAME::class_def | flag::Type::GROUP, //
class_def_raw = token::ENUM_NAME::class_def_raw | flag::Type::GROUP, //
class_pattern = token::ENUM_NAME::class_pattern | flag::Type::GROUP, //
closed_pattern = token::ENUM_NAME::closed_pattern | flag::Type::GROUP, //
compare_op_bitwise_or_pair = token::ENUM_NAME::compare_op_bitwise_or_pair | flag::Type::GROUP, //
comparison = token::ENUM_NAME::comparison | flag::Type::GROUP, //
complex_number = token::ENUM_NAME::complex_number | flag::Type::GROUP, //
compound_stmt = token::ENUM_NAME::compound_stmt | flag::Type::GROUP, //
conjunction = token::ENUM_NAME::conjunction | flag::Type::GROUP, //
decorators = token::ENUM_NAME::decorators | flag::Type::GROUP, //
default_t = token::ENUM_NAME::default_t | flag::Type::GROUP, //
del_stmt = token::ENUM_NAME::del_stmt | flag::Type::GROUP, //
del_t_atom = token::ENUM_NAME::del_t_atom | flag::Type::GROUP, //
del_target = token::ENUM_NAME::del_target | flag::Type::GROUP, //
del_targets = token::ENUM_NAME::del_targets | flag::Type::GROUP, //
dict = token::ENUM_NAME::dict | flag::Type::GROUP, //
dictcomp = token::ENUM_NAME::dictcomp | flag::Type::GROUP, //
disjunction = token::ENUM_NAME::disjunction | flag::Type::GROUP, //
dotted_as_name = token::ENUM_NAME::dotted_as_name | flag::Type::GROUP, //
dotted_as_names = token::ENUM_NAME::dotted_as_names | flag::Type::GROUP, //
dotted_name = token::ENUM_NAME::dotted_name | flag::Type::GROUP, //
double_star_pattern = token::ENUM_NAME::double_star_pattern | flag::Type::GROUP, //
double_starred_kvpair = token::ENUM_NAME::double_starred_kvpair | flag::Type::GROUP, //
double_starred_kvpairs = token::ENUM_NAME::double_starred_kvpairs | flag::Type::GROUP, //
elif_stmt = token::ENUM_NAME::elif_stmt | flag::Type::GROUP, //
else_block = token::ENUM_NAME::else_block | flag::Type::GROUP, //
eq_bitwise_or = token::ENUM_NAME::eq_bitwise_or | flag::Type::GROUP, //
eval = token::ENUM_NAME::eval | flag::Type::GROUP, //
except_block = token::ENUM_NAME::except_block | flag::Type::GROUP, //
except_star_block = token::ENUM_NAME::except_star_block | flag::Type::GROUP, //
expression = token::ENUM_NAME::expression | flag::Type::GROUP, //
expressions = token::ENUM_NAME::expressions | flag::Type::GROUP, //
factor = token::ENUM_NAME::factor | flag::Type::GROUP, //
file = token::ENUM_NAME::file | flag::Type::GROUP, //
finally_block = token::ENUM_NAME::finally_block | flag::Type::GROUP, //
for_if_clause = token::ENUM_NAME::for_if_clause | flag::Type::GROUP, //
for_if_clauses = token::ENUM_NAME::for_if_clauses | flag::Type::GROUP, //
for_stmt = token::ENUM_NAME::for_stmt | flag::Type::GROUP, //
fstring = token::ENUM_NAME::fstring | flag::Type::GROUP, //
fstring_conversion = token::ENUM_NAME::fstring_conversion | flag::Type::GROUP, //
fstring_format_spec = token::ENUM_NAME::fstring_format_spec | flag::Type::GROUP, //
fstring_full_format_spec = token::ENUM_NAME::fstring_full_format_spec | flag::Type::GROUP, //
fstring_middle = token::ENUM_NAME::fstring_middle | flag::Type::GROUP, //
fstring_replacement_field = token::ENUM_NAME::fstring_replacement_field | flag::Type::GROUP, //
fstringstring = token::ENUM_NAME::fstringstring | flag::Type::GROUP, //
func_type = token::ENUM_NAME::func_type | flag::Type::GROUP, //
func_type_comment = token::ENUM_NAME::func_type_comment | flag::Type::GROUP, //
function_def = token::ENUM_NAME::function_def | flag::Type::GROUP, //
function_def_raw = token::ENUM_NAME::function_def_raw | flag::Type::GROUP, //
genexp = token::ENUM_NAME::genexp | flag::Type::GROUP, //
global_stmt = token::ENUM_NAME::global_stmt | flag::Type::GROUP, //
group = token::ENUM_NAME::group | flag::Type::GROUP, //
group_pattern = token::ENUM_NAME::group_pattern | flag::Type::GROUP, //
gt_bitwise_or = token::ENUM_NAME::gt_bitwise_or | flag::Type::GROUP, //
gte_bitwise_or = token::ENUM_NAME::gte_bitwise_or | flag::Type::GROUP, //
guard = token::ENUM_NAME::guard | flag::Type::GROUP, //
if_stmt = token::ENUM_NAME::if_stmt | flag::Type::GROUP, //
imaginary_number = token::ENUM_NAME::imaginary_number | flag::Type::GROUP, //
import_from = token::ENUM_NAME::import_from | flag::Type::GROUP, //
import_from_as_name = token::ENUM_NAME::import_from_as_name | flag::Type::GROUP, //
import_from_as_names = token::ENUM_NAME::import_from_as_names | flag::Type::GROUP, //
import_from_targets = token::ENUM_NAME::import_from_targets | flag::Type::GROUP, //
import_name = token::ENUM_NAME::import_name | flag::Type::GROUP, //
import_stmt = token::ENUM_NAME::import_stmt | flag::Type::GROUP, //
in_bitwise_or = token::ENUM_NAME::in_bitwise_or | flag::Type::GROUP, //
interactive = token::ENUM_NAME::interactive | flag::Type::GROUP, //
invalid_default = token::ENUM_NAME::invalid_default | flag::Type::GROUP, //
inversion = token::ENUM_NAME::inversion | flag::Type::GROUP, //
is_bitwise_or = token::ENUM_NAME::is_bitwise_or | flag::Type::GROUP, //
isnot_bitwise_or = token::ENUM_NAME::isnot_bitwise_or | flag::Type::GROUP, //
items_pattern = token::ENUM_NAME::items_pattern | flag::Type::GROUP, //
key_value_pattern = token::ENUM_NAME::key_value_pattern | flag::Type::GROUP, //
keyword_pattern = token::ENUM_NAME::keyword_pattern | flag::Type::GROUP, //
keyword_patterns = token::ENUM_NAME::keyword_patterns | flag::Type::GROUP, //
kvpair = token::ENUM_NAME::kvpair | flag::Type::GROUP, //
kwarg_or_double_starred = token::ENUM_NAME::kwarg_or_double_starred | flag::Type::GROUP, //
kwarg_or_starred = token::ENUM_NAME::kwarg_or_starred | flag::Type::GROUP, //
kwargs = token::ENUM_NAME::kwargs | flag::Type::GROUP, //
kwds = token::ENUM_NAME::kwds | flag::Type::GROUP, //
lambda_kwds = token::ENUM_NAME::lambda_kwds | flag::Type::GROUP, //
lambda_param = token::ENUM_NAME::lambda_param | flag::Type::GROUP, //
lambda_param_maybe_default = token::ENUM_NAME::lambda_param_maybe_default | flag::Type::GROUP, //
lambda_param_no_default = token::ENUM_NAME::lambda_param_no_default | flag::Type::GROUP, //
lambda_param_with_default = token::ENUM_NAME::lambda_param_with_default | flag::Type::GROUP, //
lambda_parameters = token::ENUM_NAME::lambda_parameters | flag::Type::GROUP, //
lambda_params = token::ENUM_NAME::lambda_params | flag::Type::GROUP, //
lambda_slash_no_default = token::ENUM_NAME::lambda_slash_no_default | flag::Type::GROUP, //
lambda_slash_with_default = token::ENUM_NAME::lambda_slash_with_default | flag::Type::GROUP, //
lambda_star_etc = token::ENUM_NAME::lambda_star_etc | flag::Type::GROUP, //
lambdef = token::ENUM_NAME::lambdef | flag::Type::GROUP, //
list = token::ENUM_NAME::list | flag::Type::GROUP, //
listcomp = token::ENUM_NAME::listcomp | flag::Type::GROUP, //
literal_expr = token::ENUM_NAME::literal_expr | flag::Type::GROUP, //
literal_pattern = token::ENUM_NAME::literal_pattern | flag::Type::GROUP, //
lt_bitwise_or = token::ENUM_NAME::lt_bitwise_or | flag::Type::GROUP, //
lte_bitwise_or = token::ENUM_NAME::lte_bitwise_or | flag::Type::GROUP, //
mapping_pattern = token::ENUM_NAME::mapping_pattern | flag::Type::GROUP, //
match_stmt = token::ENUM_NAME::match_stmt | flag::Type::GROUP, //
maybe_sequence_pattern = token::ENUM_NAME::maybe_sequence_pattern | flag::Type::GROUP, //
maybe_star_pattern = token::ENUM_NAME::maybe_star_pattern | flag::Type::GROUP, //
name_or_attr = token::ENUM_NAME::name_or_attr | flag::Type::GROUP, //
named_expression = token::ENUM_NAME::named_expression | flag::Type::GROUP, //
nonlocal_stmt = token::ENUM_NAME::nonlocal_stmt | flag::Type::GROUP, //
noteq_bitwise_or = token::ENUM_NAME::noteq_bitwise_or | flag::Type::GROUP, //
notin_bitwise_or = token::ENUM_NAME::notin_bitwise_or | flag::Type::GROUP, //
open_sequence_pattern = token::ENUM_NAME::open_sequence_pattern | flag::Type::GROUP, //
or_pattern = token::ENUM_NAME::or_pattern | flag::Type::GROUP, //
param = token::ENUM_NAME::param | flag::Type::GROUP, //
param_maybe_default = token::ENUM_NAME::param_maybe_default | flag::Type::GROUP, //
param_no_default = token::ENUM_NAME::param_no_default | flag::Type::GROUP, //
param_no_default_star_annotation = token::ENUM_NAME::param_no_default_star_annotation | flag::Type::GROUP, //
param_star_annotation = token::ENUM_NAME::param_star_annotation | flag::Type::GROUP, //
param_with_default = token::ENUM_NAME::param_with_default | flag::Type::GROUP, //
parameters = token::ENUM_NAME::parameters | flag::Type::GROUP, //
params = token::ENUM_NAME::params | flag::Type::GROUP, //
pattern = token::ENUM_NAME::pattern | flag::Type::GROUP, //
pattern_capture_target = token::ENUM_NAME::pattern_capture_target | flag::Type::GROUP, //
patterns = token::ENUM_NAME::patterns | flag::Type::GROUP, //
positional_patterns = token::ENUM_NAME::positional_patterns | flag::Type::GROUP, //
power = token::ENUM_NAME::power | flag::Type::GROUP, //
primary = token::ENUM_NAME::primary | flag::Type::GROUP, //
raise_stmt = token::ENUM_NAME::raise_stmt | flag::Type::GROUP, //
real_number = token::ENUM_NAME::real_number | flag::Type::GROUP, //
return_stmt = token::ENUM_NAME::return_stmt | flag::Type::GROUP, //
sequence_pattern = token::ENUM_NAME::sequence_pattern | flag::Type::GROUP, //
set = token::ENUM_NAME::set | flag::Type::GROUP, //
setcomp = token::ENUM_NAME::setcomp | flag::Type::GROUP, //
shift_expr = token::ENUM_NAME::shift_expr | flag::Type::GROUP, //
signed_number = token::ENUM_NAME::signed_number | flag::Type::GROUP, //
signed_real_number = token::ENUM_NAME::signed_real_number | flag::Type::GROUP, //
simple_stmt = token::ENUM_NAME::simple_stmt | flag::Type::GROUP, //
simple_stmts = token::ENUM_NAME::simple_stmts | flag::Type::GROUP, //
single_subscript_attribute_target = token::ENUM_NAME::single_subscript_attribute_target | flag::Type::GROUP, //
single_target = token::ENUM_NAME::single_target | flag::Type::GROUP, //
slash_no_default = token::ENUM_NAME::slash_no_default | flag::Type::GROUP, //
slash_with_default = token::ENUM_NAME::slash_with_default | flag::Type::GROUP, //
slice = token::ENUM_NAME::slice | flag::Type::GROUP, //
slices = token::ENUM_NAME::slices | flag::Type::GROUP, //
star_annotation = token::ENUM_NAME::star_annotation | flag::Type::GROUP, //
star_atom = token::ENUM_NAME::star_atom | flag::Type::GROUP, //
star_etc = token::ENUM_NAME::star_etc | flag::Type::GROUP, //
star_expression = token::ENUM_NAME::star_expression | flag::Type::GROUP, //
star_expressions = token::ENUM_NAME::star_expressions | flag::Type::GROUP, //
star_named_expression = token::ENUM_NAME::star_named_expression | flag::Type::GROUP, //
star_named_expressions = token::ENUM_NAME::star_named_expressions | flag::Type::GROUP, //
star_pattern = token::ENUM_NAME::star_pattern | flag::Type::GROUP, //
star_target = token::ENUM_NAME::star_target | flag::Type::GROUP, //
star_targets = token::ENUM_NAME::star_targets | flag::Type::GROUP, //
star_targets_list_seq = token::ENUM_NAME::star_targets_list_seq | flag::Type::GROUP, //
star_targets_tuple_seq = token::ENUM_NAME::star_targets_tuple_seq | flag::Type::GROUP, //
starred_expression = token::ENUM_NAME::starred_expression | flag::Type::GROUP, //
statement = token::ENUM_NAME::statement | flag::Type::GROUP, //
statement_newline = token::ENUM_NAME::statement_newline | flag::Type::GROUP, //
statements = token::ENUM_NAME::statements | flag::Type::GROUP, //
string = token::ENUM_NAME::string | flag::Type::GROUP, //
strings = token::ENUM_NAME::strings | flag::Type::GROUP, //
subject_expr = token::ENUM_NAME::subject_expr | flag::Type::GROUP, //
sum = token::ENUM_NAME::sum | flag::Type::GROUP, //
t_lookahead = token::ENUM_NAME::t_lookahead | flag::Type::GROUP, //
t_primary = token::ENUM_NAME::t_primary | flag::Type::GROUP, //
target_with_star_atom = token::ENUM_NAME::target_with_star_atom | flag::Type::GROUP, //
term = token::ENUM_NAME::term | flag::Type::GROUP, //
try_stmt = token::ENUM_NAME::try_stmt | flag::Type::GROUP, //
tuple = token::ENUM_NAME::tuple | flag::Type::GROUP, //
type_alias = token::ENUM_NAME::type_alias | flag::Type::GROUP, //
type_expressions = token::ENUM_NAME::type_expressions | flag::Type::GROUP, //
type_param = token::ENUM_NAME::type_param | flag::Type::GROUP, //
type_param_bound = token::ENUM_NAME::type_param_bound | flag::Type::GROUP, //
type_param_seq = token::ENUM_NAME::type_param_seq | flag::Type::GROUP, //
type_params = token::ENUM_NAME::type_params | flag::Type::GROUP, //
value_pattern = token::ENUM_NAME::value_pattern | flag::Type::GROUP, //
while_stmt = token::ENUM_NAME::while_stmt | flag::Type::GROUP, //
wildcard_pattern = token::ENUM_NAME::wildcard_pattern | flag::Type::GROUP, //
with_item = token::ENUM_NAME::with_item | flag::Type::GROUP, //
with_stmt = token::ENUM_NAME::with_stmt | flag::Type::GROUP, //
yield_expr = token::ENUM_NAME::yield_expr | flag::Type::GROUP, //
yield_stmt = token::ENUM_NAME::yield_stmt | flag::Type::GROUP
)
#undef ENUM_NAME

} // namespace flag

} // namespace token

// convert 8 byte string to 64 bit intager
constexpr uint_fast64_t sti(const char *str) {
  uint_fast64_t res = 0;
  for (uint_fast8_t i = 0; str[i]; ++i) {
    res <<= 8;
    res |= static_cast<uint_fast64_t>(str[i]);
  }
  return res;
}

struct lexeme_t {
  std::string_view str;
  uint_fast16_t x = 0, y = 0;
};
struct node_t {
  turtle::turtle_flag_t flag = 0;
  turtle::lexeme_t token;
  union {
    node_t *child[2] = {nullptr, nullptr};
    struct {
      node_t *left;
      node_t *right;
    };
  };
  /*
  [[nodiscard]] constexpr turtle::turtle_flag_t type() const;
  [[nodiscard]] constexpr turtle::turtle_flag_t type_of(
      turtle::turtle_flag_t _f);
  [[nodiscard]] constexpr turtle::turtle_flag_t test_bit(unsigned char i) const;
  [[nodiscard]] constexpr turtle::turtle_flag_t hasType(
      turtle::turtle_flag_t _f) const;
  [[nodiscard]] constexpr turtle::turtle_flag_t hasFlag(
      const turtle::turtle_flag_t _f);*/
};

struct document_t {
  // Document Nodes
  std::vector<struct lexeme_t> lexemes;
  std::vector<struct node_t> nodes;
  std::vector<std::any> data;
};
} // namespace turtle

#endif // TURTLE_TOKEN_H
