#ifndef DISJUNCTIVE_SYLLOGISM_HPP
#define DISJUNCTIVE_SYLLOGISM_HPP

#include "Formula.hpp"
#include "Not.hpp"
#include "Or.hpp"

template<typename T1, typename T2> Formula* Disjunctive_syllogism(T1 formula1, T2 formula2);

#endif