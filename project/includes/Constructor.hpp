#include "Variable.hpp"
#include "Not.hpp"
#include "And.hpp"
#include "Or.hpp"
#include "Implies.hpp"

#ifndef CONSTRUCTOR_HPP
#define CONSTRUCTOR_HPP

std::vector<char> from_file(std::string file_name);

void constr(std::string file_name, std::vector<int>& scob);

Formula* tree_arch(std::string file_name);

Formula* constr_formula(std::string file_name);

#endif
