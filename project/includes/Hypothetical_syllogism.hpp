#ifndef HYPOTHETICAL_SYLLOGISM_HPP
#define HYPOTHETICAL_SYLLOGISM_HPP

#include "Formula.hpp"
#include "Implies.hpp"

template<typename T1, typename T2> Formula* Hypothetical_syllogism(T1 formula1, T2 formula2);

#endif