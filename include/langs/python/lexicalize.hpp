//
// Created by alex on 11/11/21.
//

#ifndef TURTLE_LANG_PYTHON_LEXICALIZE_H
#define TURTLE_LANG_PYTHON_LEXICALIZE_H
#include "node.hpp"

namespace turtle::langs::python {
void lexicalize(std::string &filedata,
                turtle::turtle_vector<turtle::node_t> &lexemes);
} // namespace turtle::lang::python

#endif // TURTLE_LEXICALIZE_H
