#ifndef DISJUNCTIVE_SYLLOGISM_XOR_HPP
#define DISJUNCTIVE_SYLLOGISM_XOR_HPP

#include "Formula.hpp"
#include "Xor.hpp"
#include "Not.hpp"

template<typename T1, typename T2> Formula* Disjunctive_syllogism_xor(T1 formula1, T2 formula2);

#endif