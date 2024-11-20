#include "Variable.hpp"
#include "Not.hpp"
#include "And.hpp"
#include "Or.hpp"
#include "Implies.hpp"

#ifndef MODES_PONENS_HPP
#define MODES_PONENS_HPP

template<typename T1, typename T2> Formula* Modes_ponens(T1 formula1, T2 formula2);

#endif