#ifndef TURTLE_SYNTAX_H
#define TURTLE_SYNTAX_H

#include <array>

#define DEDENT turtle::token::flags::Control::DEDENT
#define ENDMARKER turtle::token::flags::Control::ENDMARKER
#define INDENT turtle::token::flags::Control::INDENT
#define NEWLINE turtle::token::flags::Control::NEWLINE
#define NAME turtle::token::flags::Type::IDENTIFIER
#define NEWLINE turtle::token::flags::Control::NEWLINE
#define NUMBER turtle::token::flags::Data::DATA_TYPE_NUMBER
#define STRING turtle::token::flags::Data::DATA_TYPE_STRING

#define OR '|',
#define TO '-',
#define ANY_OF(...) '[', __VA_ARGS__, ']',
#define POSITIVE_LOOKAHEAD(x) '(', '?', '=', x, ')',
#define NEGATIVE_LOOKAHEAD(x) '(', '?', '!', x, ')',
#define OPTIONAL(...) '(', '?', ':', __VA_ARGS__, ')', '?',

#define annotated_rhs turtle::token::flags::Group::annotated_rhs,
#define annotation turtle::token::flags::Group::annotation,
#define args turtle::token::flags::Group::args,
#define arguments turtle::token::flags::Group::arguments,
#define as_pattern turtle::token::flags::Group::as_pattern,
#define assert_stmt turtle::token::flags::Group::assert_stmt,
#define assignment turtle::token::flags::Group::assignment,
#define assignment_expression turtle::token::flags::Group::assignment_expression,
#define atom turtle::token::flags::Group::atom,
#define attr turtle::token::flags::Group::attr,
#define augassign turtle::token::flags::Group::augassign,
#define await_primary turtle::token::flags::Group::await_primary,
#define bitwise_and turtle::token::flags::Group::bitwise_and,
#define bitwise_or turtle::token::flags::Group::bitwise_or,
#define bitwise_xor turtle::token::flags::Group::bitwise_xor,
#define block turtle::token::flags::Group::block,
#define capture_pattern turtle::token::flags::Group::capture_pattern,
#define case_block turtle::token::flags::Group::case_block,
#define class_def turtle::token::flags::Group::class_def,
#define class_def_raw turtle::token::flags::Group::class_def_raw,
#define class_pattern turtle::token::flags::Group::class_pattern,
#define closed_pattern turtle::token::flags::Group::closed_pattern,
#define compare_op_bitwise_or_pair turtle::token::flags::Group::compare_op_bitwise_or_pair,
#define comparison turtle::token::flags::Group::comparison,
#define complex_number turtle::token::flags::Group::complex_number,
#define compound_stmt turtle::token::flags::Group::compound_stmt,
#define conjunction turtle::token::flags::Group::conjunction,
#define decorators turtle::token::flags::Group::decorators,
#define default_t turtle::token::flags::Group::default_t,
#define del_stmt turtle::token::flags::Group::del_stmt,
#define del_t_atom turtle::token::flags::Group::del_t_atom,
#define del_target turtle::token::flags::Group::del_target,
#define del_targets turtle::token::flags::Group::del_targets,
#define dict turtle::token::flags::Group::dict,
#define dictcomp turtle::token::flags::Group::dictcomp,
#define disjunction turtle::token::flags::Group::disjunction,
#define dotted_as_name turtle::token::flags::Group::dotted_as_name,
#define dotted_as_names turtle::token::flags::Group::dotted_as_names,
#define dotted_name turtle::token::flags::Group::dotted_name,
#define double_star_pattern turtle::token::flags::Group::double_star_pattern,
#define double_starred_kvpair turtle::token::flags::Group::double_starred_kvpair,
#define double_starred_kvpairs turtle::token::flags::Group::double_starred_kvpairs,
#define elif_stmt turtle::token::flags::Group::elif_stmt,
#define else_block turtle::token::flags::Group::else_block,
#define eq_bitwise_or turtle::token::flags::Group::eq_bitwise_or,
#define eval turtle::token::flags::Group::eval,
#define except_block turtle::token::flags::Group::except_block,
#define except_star_block turtle::token::flags::Group::except_star_block,
#define expression turtle::token::flags::Group::expression,
#define expressions turtle::token::flags::Group::expressions,
#define factor turtle::token::flags::Group::factor,
#define file turtle::token::flags::Group::file,
#define finally_block turtle::token::flags::Group::finally_block,
#define for_if_clause turtle::token::flags::Group::for_if_clause,
#define for_if_clauses turtle::token::flags::Group::for_if_clauses,
#define for_stmt turtle::token::flags::Group::for_stmt,
#define fstring turtle::token::flags::Group::fstring,
#define fstring_conversion turtle::token::flags::Group::fstring_conversion,
#define fstring_format_spec turtle::token::flags::Group::fstring_format_spec,
#define fstring_full_format_spec turtle::token::flags::Group::fstring_full_format_spec,
#define fstring_middle turtle::token::flags::Group::fstring_middle,
#define fstring_replacement_field turtle::token::flags::Group::fstring_replacement_field,
#define fstringstring turtle::token::flags::Group::fstringstring,
#define func_type turtle::token::flags::Group::func_type,
#define func_type_comment turtle::token::flags::Group::func_type_comment,
#define function_def turtle::token::flags::Group::function_def,
#define function_def_raw turtle::token::flags::Group::function_def_raw,
#define genexp turtle::token::flags::Group::genexp,
#define global_stmt turtle::token::flags::Group::global_stmt,
#define group turtle::token::flags::Group::group,
#define group_pattern turtle::token::flags::Group::group_pattern,
#define gt_bitwise_or turtle::token::flags::Group::gt_bitwise_or,
#define gte_bitwise_or turtle::token::flags::Group::gte_bitwise_or,
#define guard turtle::token::flags::Group::guard,
#define if_stmt turtle::token::flags::Group::if_stmt,
#define imaginary_number turtle::token::flags::Group::imaginary_number,
#define import_from turtle::token::flags::Group::import_from,
#define import_from_as_name turtle::token::flags::Group::import_from_as_name,
#define import_from_as_names turtle::token::flags::Group::import_from_as_names,
#define import_from_targets turtle::token::flags::Group::import_from_targets,
#define import_name turtle::token::flags::Group::import_name,
#define import_stmt turtle::token::flags::Group::import_stmt,
#define in_bitwise_or turtle::token::flags::Group::in_bitwise_or,
#define interactive turtle::token::flags::Group::interactive,
#define invalid_default turtle::token::flags::Group::invalid_default,
#define inversion turtle::token::flags::Group::inversion,
#define is_bitwise_or turtle::token::flags::Group::is_bitwise_or,
#define isnot_bitwise_or turtle::token::flags::Group::isnot_bitwise_or,
#define items_pattern turtle::token::flags::Group::items_pattern,
#define key_value_pattern turtle::token::flags::Group::key_value_pattern,
#define keyword_pattern turtle::token::flags::Group::keyword_pattern,
#define keyword_patterns turtle::token::flags::Group::keyword_patterns,
#define kvpair turtle::token::flags::Group::kvpair,
#define kwarg_or_double_starred turtle::token::flags::Group::kwarg_or_double_starred,
#define kwarg_or_starred turtle::token::flags::Group::kwarg_or_starred,
#define kwargs turtle::token::flags::Group::kwargs,
#define kwds turtle::token::flags::Group::kwds,
#define lambda_kwds turtle::token::flags::Group::lambda_kwds,
#define lambda_param turtle::token::flags::Group::lambda_param,
#define lambda_param_maybe_default turtle::token::flags::Group::lambda_param_maybe_default,
#define lambda_param_no_default turtle::token::flags::Group::lambda_param_no_default,
#define lambda_param_with_default turtle::token::flags::Group::lambda_param_with_default,
#define lambda_parameters turtle::token::flags::Group::lambda_parameters,
#define lambda_params turtle::token::flags::Group::lambda_params,
#define lambda_slash_no_default turtle::token::flags::Group::lambda_slash_no_default,
#define lambda_slash_with_default turtle::token::flags::Group::lambda_slash_with_default,
#define lambda_star_etc turtle::token::flags::Group::lambda_star_etc,
#define lambdef turtle::token::flags::Group::lambdef,
#define list turtle::token::flags::Group::list,
#define listcomp turtle::token::flags::Group::listcomp,
#define literal_expr turtle::token::flags::Group::literal_expr,
#define literal_pattern turtle::token::flags::Group::literal_pattern,
#define lt_bitwise_or turtle::token::flags::Group::lt_bitwise_or,
#define lte_bitwise_or turtle::token::flags::Group::lte_bitwise_or,
#define mapping_pattern turtle::token::flags::Group::mapping_pattern,
#define match_stmt turtle::token::flags::Group::match_stmt,
#define maybe_sequence_pattern turtle::token::flags::Group::maybe_sequence_pattern,
#define maybe_star_pattern turtle::token::flags::Group::maybe_star_pattern,
#define name_or_attr turtle::token::flags::Group::name_or_attr,
#define named_expression turtle::token::flags::Group::named_expression,
#define nonlocal_stmt turtle::token::flags::Group::nonlocal_stmt,
#define noteq_bitwise_or turtle::token::flags::Group::noteq_bitwise_or,
#define notin_bitwise_or turtle::token::flags::Group::notin_bitwise_or,
#define open_sequence_pattern turtle::token::flags::Group::open_sequence_pattern,
#define or_pattern turtle::token::flags::Group::or_pattern,
#define param turtle::token::flags::Group::param,
#define param_maybe_default turtle::token::flags::Group::param_maybe_default,
#define param_no_default turtle::token::flags::Group::param_no_default,
#define param_no_default_star_annotation turtle::token::flags::Group::param_no_default_star_annotation,
#define param_star_annotation turtle::token::flags::Group::param_star_annotation,
#define param_with_default turtle::token::flags::Group::param_with_default,
#define parameters turtle::token::flags::Group::parameters,
#define params turtle::token::flags::Group::params,
#define pattern turtle::token::flags::Group::pattern,
#define pattern_capture_target turtle::token::flags::Group::pattern_capture_target,
#define patterns turtle::token::flags::Group::patterns,
#define positional_patterns turtle::token::flags::Group::positional_patterns,
#define power turtle::token::flags::Group::power,
#define primary turtle::token::flags::Group::primary,
#define raise_stmt turtle::token::flags::Group::raise_stmt,
#define real_number turtle::token::flags::Group::real_number,
#define return_stmt turtle::token::flags::Group::return_stmt,
#define sequence_pattern turtle::token::flags::Group::sequence_pattern,
#define set turtle::token::flags::Group::set,
#define setcomp turtle::token::flags::Group::setcomp,
#define shift_expr turtle::token::flags::Group::shift_expr,
#define signed_number turtle::token::flags::Group::signed_number,
#define signed_real_number turtle::token::flags::Group::signed_real_number,
#define simple_stmt turtle::token::flags::Group::simple_stmt,
#define simple_stmts turtle::token::flags::Group::simple_stmts,
#define single_subscript_attribute_target turtle::token::flags::Group::single_subscript_attribute_target,
#define single_target turtle::token::flags::Group::single_target,
#define slash_no_default turtle::token::flags::Group::slash_no_default,
#define slash_with_default turtle::token::flags::Group::slash_with_default,
#define slice turtle::token::flags::Group::slice,
#define slices turtle::token::flags::Group::slices,
#define star_annotation turtle::token::flags::Group::star_annotation,
#define star_atom turtle::token::flags::Group::star_atom,
#define star_etc turtle::token::flags::Group::star_etc,
#define star_expression turtle::token::flags::Group::star_expression,
#define star_expressions turtle::token::flags::Group::star_expressions,
#define star_named_expression turtle::token::flags::Group::star_named_expression,
#define star_named_expressions turtle::token::flags::Group::star_named_expressions,
#define star_pattern turtle::token::flags::Group::star_pattern,
#define star_target turtle::token::flags::Group::star_target,
#define star_targets turtle::token::flags::Group::star_targets,
#define star_targets_list_seq turtle::token::flags::Group::star_targets_list_seq,
#define star_targets_tuple_seq turtle::token::flags::Group::star_targets_tuple_seq,
#define starred_expression turtle::token::flags::Group::starred_expression,
#define statement turtle::token::flags::Group::statement,
#define statement_newline turtle::token::flags::Group::statement_newline,
#define statements turtle::token::flags::Group::statements,
#define string turtle::token::flags::Group::string,
#define strings turtle::token::flags::Group::strings,
#define subject_expr turtle::token::flags::Group::subject_expr,
#define sum turtle::token::flags::Group::sum,
#define t_lookahead turtle::token::flags::Group::t_lookahead,
#define t_primary turtle::token::flags::Group::t_primary,
#define target_with_star_atom turtle::token::flags::Group::target_with_star_atom,
#define term turtle::token::flags::Group::term,
#define try_stmt turtle::token::flags::Group::try_stmt,
#define tuple turtle::token::flags::Group::tuple,
#define type_alias turtle::token::flags::Group::type_alias,
#define type_expressions turtle::token::flags::Group::type_expressions,
#define type_param turtle::token::flags::Group::type_param,
#define type_param_bound turtle::token::flags::Group::type_param_bound,
#define type_param_seq turtle::token::flags::Group::type_param_seq,
#define type_params turtle::token::flags::Group::type_params,
#define value_pattern turtle::token::flags::Group::value_pattern,
#define while_stmt turtle::token::flags::Group::while_stmt,
#define wildcard_pattern turtle::token::flags::Group::wildcard_pattern,
#define with_item turtle::token::flags::Group::with_item,
#define with_stmt turtle::token::flags::Group::with_stmt,
#define yield_expr turtle::token::flags::Group::yield_expr,
#define yield_stmt turtle::token::flags::Group::yield_stmt,

namespace turtle {

void syntax() {
  constexpr auto syntaxRegex = std::to_array<turtle::turtle_flag_t>({

    1

  });
}

} // namespace turtle

#undef annotated_rhs
#undef annotation
#undef args
#undef arguments
#undef as_pattern
#undef assert_stmt
#undef assignment
#undef assignment_expression
#undef atom
#undef attr
#undef augassign
#undef await_primary
#undef bitwise_and
#undef bitwise_or
#undef bitwise_xor
#undef block
#undef capture_pattern
#undef case_block
#undef class_def
#undef class_def_raw
#undef class_pattern
#undef closed_pattern
#undef compare_op_bitwise_or_pair
#undef comparison
#undef complex_number
#undef compound_stmt
#undef conjunction
#undef decorators
#undef default_t
#undef del_stmt
#undef del_t_atom
#undef del_target
#undef del_targets
#undef dict
#undef dictcomp
#undef disjunction
#undef dotted_as_name
#undef dotted_as_names
#undef dotted_name
#undef double_star_pattern
#undef double_starred_kvpair
#undef double_starred_kvpairs
#undef elif_stmt
#undef else_block
#undef eq_bitwise_or
#undef eval
#undef except_block
#undef except_star_block
#undef expression
#undef expressions
#undef factor
#undef file
#undef finally_block
#undef for_if_clause
#undef for_if_clauses
#undef for_stmt
#undef fstring
#undef fstring_conversion
#undef fstring_format_spec
#undef fstring_full_format_spec
#undef fstring_middle
#undef fstring_replacement_field
#undef fstringstring
#undef func_type
#undef func_type_comment
#undef function_def
#undef function_def_raw
#undef genexp
#undef global_stmt
#undef group
#undef group_pattern
#undef gt_bitwise_or
#undef gte_bitwise_or
#undef guard
#undef if_stmt
#undef imaginary_number
#undef import_from
#undef import_from_as_name
#undef import_from_as_names
#undef import_from_targets
#undef import_name
#undef import_stmt
#undef in_bitwise_or
#undef interactive
#undef invalid_default
#undef inversion
#undef is_bitwise_or
#undef isnot_bitwise_or
#undef items_pattern
#undef key_value_pattern
#undef keyword_pattern
#undef keyword_patterns
#undef kvpair
#undef kwarg_or_double_starred
#undef kwarg_or_starred
#undef kwargs
#undef kwds
#undef lambda_kwds
#undef lambda_param
#undef lambda_param_maybe_default
#undef lambda_param_no_default
#undef lambda_param_with_default
#undef lambda_parameters
#undef lambda_params
#undef lambda_slash_no_default
#undef lambda_slash_with_default
#undef lambda_star_etc
#undef lambdef
#undef list
#undef listcomp
#undef literal_expr
#undef literal_pattern
#undef lt_bitwise_or
#undef lte_bitwise_or
#undef mapping_pattern
#undef match_stmt
#undef maybe_sequence_pattern
#undef maybe_star_pattern
#undef name_or_attr
#undef named_expression
#undef nonlocal_stmt
#undef noteq_bitwise_or
#undef notin_bitwise_or
#undef open_sequence_pattern
#undef or_pattern
#undef param
#undef param_maybe_default
#undef param_no_default
#undef param_no_default_star_annotation
#undef param_star_annotation
#undef param_with_default
#undef parameters
#undef params
#undef pattern
#undef pattern_capture_target
#undef patterns
#undef positional_patterns
#undef power
#undef primary
#undef raise_stmt
#undef real_number
#undef return_stmt
#undef sequence_pattern
#undef set
#undef setcomp
#undef shift_expr
#undef signed_number
#undef signed_real_number
#undef simple_stmt
#undef simple_stmts
#undef single_subscript_attribute_target
#undef single_target
#undef slash_no_default
#undef slash_with_default
#undef slice
#undef slices
#undef star_annotation
#undef star_atom
#undef star_etc
#undef star_expression
#undef star_expressions
#undef star_named_expression
#undef star_named_expressions
#undef star_pattern
#undef star_target
#undef star_targets
#undef star_targets_list_seq
#undef star_targets_tuple_seq
#undef starred_expression
#undef statement
#undef statement_newline
#undef statements
#undef string
#undef strings
#undef subject_expr
#undef sum
#undef t_lookahead
#undef t_primary
#undef target_with_star_atom
#undef term
#undef try_stmt
#undef tuple
#undef type_alias
#undef type_expressions
#undef type_param
#undef type_param_bound
#undef type_param_seq
#undef type_params
#undef value_pattern
#undef while_stmt
#undef wildcard_pattern
#undef with_item
#undef with_stmt
#undef yield_expr
#undef yield_stmt

#undef OPTIONAL
#undef NEGATIVE_LOOKAHEAD
#undef POSITIVE_LOOKAHEAD
#undef ANY_OF
#undef OR

#undef DEDENT
#undef ENDMARKER
#undef INDENT
#undef NAME
#undef NEWLINE
#undef NUMBER
#undef STRING

#endif // TURTLE_SYNTAX_H
