#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

#include "includes/Implies.hpp"
#include "includes/And.hpp"
#include "includes/Not.hpp"
#include "includes/Or.hpp"
#include "includes/Variable.hpp"
#include "includes/Constructor.hpp"
#include "includes/Aksiomes.hpp"
#include "includes/New_aks.hpp"
#include "includes/Formula.hpp"
#include "includes/Modes_ponens.hpp"
#include "includes/Complex_constructive_dilemma.hpp"
#include "includes/Complex_destructive_dilemma.hpp"
#include "includes/Disjunctive_syllogism.hpp"
#include "includes/Disjunctive_syllogism_xor.hpp"
#include "includes/Hypothetical_syllogism.hpp"
#include "includes/Modes_tollens.hpp"
#include "includes/Simple_constructive_dilemma.hpp"
#include "includes/Simple_destructive_dilemma.hpp"

Formula* rule_dis(Formula* form){
    Formula* res = form;
    if (auto imp = dynamic_cast<Implies*>(form)){
        if (dynamic_cast<Or*>(imp->get_left()->clone())){
            auto or_i = dynamic_cast<Or*>(imp->get_left()->clone());
            auto new_form = new Implies(nullptr, nullptr);
            auto not_left = new Not(or_i->get_left()->clone());
            new_form->set_left(not_left->clone());
            new_form->set_right(or_i->get_right()->clone());
            imp->set_left(new_form);
        }
        if (dynamic_cast<Or*>(imp->get_right()->clone())){
            auto or_i = dynamic_cast<Or*>(imp->get_right()->clone());
            auto new_form = new Implies(nullptr, nullptr);
            auto not_left = new Not(or_i->get_left()->clone());
            new_form->set_left(not_left->clone());
            new_form->set_right(or_i->get_right()->clone());
            imp->set_right(new_form->clone());
        }
        if (dynamic_cast<Implies*>(imp->get_left())) imp->set_left(rule_dis(imp->get_left()));
        if (dynamic_cast<Implies*>(imp->get_right())) imp->set_right(rule_dis(imp->get_right()));
        res = nullptr;
        res = form->clone();
    }
    return res;
}

Formula* rule_con(Formula* form){
    Formula* res = form;
    if (auto imp = dynamic_cast<Implies*>(form)){
        if (dynamic_cast<And*>(imp->get_left())){
            auto and_i = dynamic_cast<And*>(imp->get_left()->clone());
            auto imp_i = new Implies(nullptr, nullptr);
            auto not_right = new Not(and_i->get_right()->clone());
            imp_i->set_left(and_i->get_left()->clone());
            imp_i->set_right(not_right);
            auto new_form = new Not(imp_i);
            imp->set_left(new_form);
        }
        if (dynamic_cast<And*>(imp->get_right())){
            auto and_i = dynamic_cast<And*>(imp->get_right()->clone());
            auto imp_i = new Implies(nullptr, nullptr);
            auto not_right = new Not(and_i->get_right()->clone());
            imp_i->set_left(and_i->get_left()->clone());
            imp_i->set_right(not_right);
            auto new_form = new Not(imp_i);
            imp->set_right(new_form);
        }
        if (dynamic_cast<Implies*>(imp->get_left())) imp->set_left(rule_dis(imp->get_left()));
        if (dynamic_cast<Implies*>(imp->get_right())) imp->set_right(rule_dis(imp->get_right()));
        res = imp;
    }
    return res;
}


std::vector<Formula*> deduction(Formula* formula){
    std::vector<Formula*> gamma;
    auto curr_imp = dynamic_cast<Implies*>(formula);
    while(true){
        if (auto left = dynamic_cast<Implies*>(curr_imp->get_left())) gamma.push_back(left);
        if (auto left = dynamic_cast<Not*>(curr_imp->get_left())) gamma.push_back(left);
        if (auto left = dynamic_cast<Variable*>(curr_imp->get_left())) gamma.push_back(left);
        
        if (dynamic_cast<Implies*>(curr_imp->get_right())) curr_imp = dynamic_cast<Implies*>(curr_imp->get_right());
        else if (dynamic_cast<Variable*>(curr_imp->get_right())){
            gamma.push_back(dynamic_cast<Variable*>(curr_imp->get_right()));
            break;
        }
        else if (dynamic_cast<Not*>(curr_imp->get_right())){
            gamma.push_back(dynamic_cast<Not*>(curr_imp->get_right()));
            break;
        }
    }
    return gamma;
}


void task1(Formula* formula, Formula* aksiome1, Formula* aksiome2, Formula* aksiome3){
    std::vector<Formula*> param;
    take_params(formula, param);
    std::vector<Formula*> gamma;

    std::vector<Formula*> new_aks_1 = new_aks1(aksiome1, param);
    for (auto x: new_aks_1){
        gamma.push_back(x);
    }

    std::vector<Formula*> new_aks_2 = new_aks2(aksiome2, param);
    for (auto x: new_aks_2){
        gamma.push_back(x);
    }

    std::vector<Formula*> new_aks_3 = new_aks3(aksiome3, param);
    for (auto x: new_aks_3){
        gamma.push_back(x);
    }   
    std::cout << "Go to deduction..." << std::endl;
    std::vector<Formula*> ded_gamma = deduction(formula);
    std::cout << "Deduction ready." << std::endl;
    for (auto g: ded_gamma){
        gamma.push_back(g);
        g->print(std::cout);
        std::cout<<std::endl;
    }

    for (auto i = 0; i < gamma.size()-1; i++){
        for (auto j = i+1; j < gamma.size(); j++){
            if (auto f2 = dynamic_cast<Implies*>(gamma[j])){
                auto res = Modes_ponens(gamma[i], f2);
                if (res != nullptr) gamma.push_back(res);
            }
        }
    }
    for (auto i = 0; i < gamma.size()-1; i++){
        for (auto j = i+1; j < gamma.size(); j++){
            if (auto f2 =dynamic_cast<Implies*>(gamma[j])){
                auto res = Modes_ponens(gamma[i], f2);
                if (res != nullptr) gamma.push_back(res);
            }
        }
    }
    
    std::ofstream out("../result.txt");
    for (auto g: gamma){
        g->print(out);
        out<<std::endl;
    }
    out.close();
}

int main() {
    auto aksiome1 = aksiome_1();
    auto aksiome2 = aksiome_2();
    auto aksiome3 = aksiome_3();

    auto form = constr_formula("../input.txt");
    form = rule_con(form);
    form = rule_dis(form);

    form->print(std::cout);
    std::cout<<std::endl;

    task1(form, aksiome1, aksiome2, aksiome3);

    return 0;
}
