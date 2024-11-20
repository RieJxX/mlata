#include "Variable.hpp"
#include "Not.hpp"
#include "And.hpp"
#include "Or.hpp"
#include "Implies.hpp"

#ifndef NEW_ASK_HPP
#define NEW_ASK_HPP

void take_params(Formula* formula, std::vector<Formula*>& params);

std::vector<Formula*> new_aks1(Formula* aksiome_1, const std::vector<Formula*>& param_f);

std::vector<Formula*> new_aks2(Formula* aksiome_2, std::vector<Formula*> param_f);

std::vector<Formula*> new_aks3(Formula* aksiome_3, std::vector<Formula*> param_f);

#endif