//
// Created by alex on 11/11/21.
//

#ifndef TURTLE_LEXICALIZE_H
#define TURTLE_LEXICALIZE_H
#include "node.hpp"

namespace turtle::lang::python {
void lexicalize(std::string &filedata,
                turtle::turtle_vector<turtle::node_t> &lexemes);
} // namespace turtle::lang::python

#endif // TURTLE_LEXICALIZE_H
