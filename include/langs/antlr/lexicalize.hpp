//
// Created by alex on 11/11/21.
//

#ifndef TURTLE_LANG_ANTLR_LEXICALIZE_H
#define TURTLE_LANG_ANTLR_LEXICALIZE_H
#include "node.hpp"

namespace turtle::langs::antlr {
void lexicalize(std::string &filedata,
                turtle::turtle_vector<turtle::node_t> &lexemes);
} // namespace turtle::lang::antlr

#endif // TURTLE_LEXICALIZE_H
