// #include <iostream>
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

std::ofstream log_file("../log.txt");

Formula* rule_dis(Formula* form){
    log_file << "Entering rule_dis with formula: ";
    form->print(log_file);
    log_file << std::endl;

    Formula* res = form;
    if (auto imp = dynamic_cast<Implies*>(form)){
        if (dynamic_cast<Or*>(imp->get_left()->clone())){
            auto or_i = dynamic_cast<Or*>(imp->get_left()->clone());
            auto new_form = new Implies(nullptr, nullptr);
            auto not_left = new Not(or_i->get_left()->clone());
            new_form->set_left(not_left->clone());
            new_form->set_right(or_i->get_right()->clone());
            imp->set_left(new_form);

            log_file << "Applied rule_dis to left part: ";
            imp->get_left()->print(log_file);
            log_file << std::endl;
        }
        if (dynamic_cast<Or*>(imp->get_right()->clone())){
            auto or_i = dynamic_cast<Or*>(imp->get_right()->clone());
            auto new_form = new Implies(nullptr, nullptr);
            auto not_left = new Not(or_i->get_left()->clone());
            new_form->set_left(not_left->clone());
            new_form->set_right(or_i->get_right()->clone());
            imp->set_right(new_form->clone());

            log_file << "Applied rule_dis to right part: ";
            imp->get_right()->print(log_file);
            log_file << std::endl;
        }
        if (dynamic_cast<Implies*>(imp->get_left())) imp->set_left(rule_dis(imp->get_left()));
        if (dynamic_cast<Implies*>(imp->get_right())) imp->set_right(rule_dis(imp->get_right()));
        res = nullptr;
        res = form->clone();
    }

    if (auto ori = dynamic_cast<Or*>(form->clone())){
        auto new_form = new Implies(nullptr, nullptr);
        auto not_left = new Not(ori->get_left()->clone());
        new_form->set_left(not_left->clone());
        new_form->set_right(ori->get_right()->clone());
        res = new_form->clone();

        log_file << "Applied rule_dis: ";
        res->print(log_file);
        log_file << std::endl;
    }

    log_file << "Exiting rule_dis with result: ";
    res->print(log_file);
    log_file << std::endl;

    return res;
}

Formula* rule_con(Formula* form){
    log_file << "Entering rule_con with formula: ";
    form->print(log_file);
    log_file << std::endl;

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

            log_file << "Applied rule_con to left part: ";
            imp->get_left()->print(log_file);
            log_file << std::endl;
        }
        if (dynamic_cast<And*>(imp->get_right())){
            auto and_i = dynamic_cast<And*>(imp->get_right()->clone());
            auto imp_i = new Implies(nullptr, nullptr);
            auto not_right = new Not(and_i->get_right()->clone());
            imp_i->set_left(and_i->get_left()->clone());
            imp_i->set_right(not_right);
            auto new_form = new Not(imp_i);
            imp->set_right(new_form);

            log_file << "Applied rule_con to right part: ";
            imp->get_right()->print(log_file);
            log_file << std::endl;
        }
        if (dynamic_cast<Implies*>(imp->get_left())) imp->set_left(rule_dis(imp->get_left()));
        if (dynamic_cast<Implies*>(imp->get_right())) imp->set_right(rule_dis(imp->get_right()));
        res = imp;
    }
    if (dynamic_cast<And*>(form)){
        auto and_i = dynamic_cast<And*>(form->clone());
        auto imp_i = new Implies(nullptr, nullptr);
        auto not_right = new Not(and_i->get_right()->clone());
        imp_i->set_left(and_i->get_left()->clone());
        imp_i->set_right(not_right);
        auto new_form = new Not(imp_i);
        res = new_form;

        log_file << "Applied rule_con: ";
        res->print(log_file);
        log_file << std::endl;
    }
    log_file << "Exiting rule_con with result: ";
    res->print(log_file);
    log_file << std::endl;

    return res;
}

std::vector<Formula*> deduction(Formula* formula){
    log_file << "Entering deduction with formula: ";
    formula->print(log_file);
    log_file << std::endl;

    std::vector<Formula*> gamma;
    std::ofstream out("../deduction.txt");
    auto curr_imp = dynamic_cast<Implies*>(formula);
    while(true){
        out<<"curr formula: ";
        curr_imp->print(out);
        out<<"\t";
        if (auto left = dynamic_cast<Implies*>(curr_imp->get_left())){
            gamma.push_back(left);
            out<<"{";
            for (auto g: gamma){
                g->print(out);
                out<<" ";
            }
            out<<"}";
            out<<std::endl;
        }
        if (auto left = dynamic_cast<Not*>(curr_imp->get_left())){
            gamma.push_back(left);
            out<<"{";
            for (auto g: gamma){
                g->print(out);
                out<<" ";
            }
            out<<"}";
            out<<std::endl;
        }
        if (auto left = dynamic_cast<Variable*>(curr_imp->get_left())){
            gamma.push_back(left);
            out<<"{";
            for (auto g: gamma){
                g->print(out);
                out<<" ";
            }
            out<<"}";
            out<<std::endl;
        }
        
        if (dynamic_cast<Implies*>(curr_imp->get_right())) curr_imp = dynamic_cast<Implies*>(curr_imp->get_right());
        else if (dynamic_cast<Variable*>(curr_imp->get_right())){
            gamma.push_back(dynamic_cast<Variable*>(curr_imp->get_right()));
                out<<"{";
                for (auto g: gamma){
                    g->print(out);
                    out<<" ";
                }
                out<<"}";
                out<<std::endl;
            break;
        }
        else if (dynamic_cast<Not*>(curr_imp->get_right())){
            gamma.push_back(dynamic_cast<Not*>(curr_imp->get_right()));
            out<<"{";
            for (auto g: gamma){
                g->print(out);
                out<<" ";
            }
            out<<"}";
            out<<std::endl;
            break;
        }
    }
    out.close();

    log_file << "Exiting deduction with result: ";
    for (auto g: gamma){
        g->print(log_file);
        log_file << " ";
    }
    log_file << std::endl;

    return gamma;
}

// Функция для замены подформулы old_sub на new_sub в формуле form
Formula* replace_subformula(Formula* form, Formula* old_sub, Formula* new_sub) {
    if (form == nullptr) return nullptr;

    // Если текущая формула совпадает с old_sub, возвращаем new_sub
    if (form->equals(*old_sub)) {
        return new_sub->clone();
    }

    // Рекурсивно заменяем в подформулах
    if (auto imp = dynamic_cast<Implies*>(form)) {
        Formula* new_left = replace_subformula(imp->get_left(), old_sub, new_sub);
        Formula* new_right = replace_subformula(imp->get_right(), old_sub, new_sub);
        return new Implies(new_left, new_right);
    } else if (auto or_form = dynamic_cast<Or*>(form)) {
        Formula* new_left = replace_subformula(or_form->get_left(), old_sub, new_sub);
        Formula* new_right = replace_subformula(or_form->get_right(), old_sub, new_sub);
        return new Or(new_left, new_right);
    } else if (auto and_form = dynamic_cast<And*>(form)) {
        Formula* new_left = replace_subformula(and_form->get_left(), old_sub, new_sub);
        Formula* new_right = replace_subformula(and_form->get_right(), old_sub, new_sub);
        return new And(new_left, new_right);
    } else if (auto not_form = dynamic_cast<Not*>(form)) {
        Formula* new_operand = replace_subformula(not_form->get_operand(), old_sub, new_sub);
        return new Not(new_operand);
    }

    // Если формула не является составной, возвращаем её без изменений
    return form->clone();
}

void task1(Formula* formula, Formula* aksiome1, Formula* aksiome2, Formula* aksiome3) {
    log_file << "Entering task1 with formula: ";
    formula->print(log_file);
    log_file << std::endl;

    std::vector<Formula*> param;
    take_params(formula, param);
    std::vector<Formula*> gamma;

    // Добавляем новые аксиомы в gamma
    std::vector<Formula*> new_aks_1 = new_aks1(aksiome1, param);
    for (auto x : new_aks_1) {
        gamma.push_back(x);
    }

    std::vector<Formula*> new_aks_2 = new_aks2(aksiome2, param);
    for (auto x : new_aks_2) {
        gamma.push_back(x);
    }

    std::vector<Formula*> new_aks_3 = new_aks3(aksiome3, param);
    for (auto x : new_aks_3) {
        gamma.push_back(x);
    }

    std::cout << "Go to deduction..." << std::endl;
    std::vector<Formula*> ded_gamma = deduction(formula);
    std::cout << "Deduction ready." << std::endl;

    // Добавляем результаты дедукции в gamma
    for (int i = 0; i < ded_gamma.size() - 1; i++) {
        gamma.push_back(ded_gamma[i]);
    }

    Formula* new_form = ded_gamma[ded_gamma.size() - 1];
    int flag = 1;

    // Логируем зависимости
    log_file << "Dependencies log:" << std::endl;

    while (true) {
        int size = gamma.size();
        std::vector<Formula*> new_gamma;

        for (int i = 0; i < size; i++) {
            if (flag) {
                for (int j = 0; j < size; j++) {
                    if (flag) {
                        // Проверяем Modus Ponens
                        if (auto f2 = dynamic_cast<Implies*>(gamma[j])) {
                            auto res = Modes_ponens(gamma[i], f2);
                            if (res != nullptr) {
                                log_file << "From: ";
                                gamma[i]->print(log_file);
                                log_file << " and ";
                                gamma[j]->print(log_file);
                                log_file << " derived (Modus Ponens): ";
                                res->print(log_file);
                                log_file << std::endl;
                                new_gamma.push_back(res);

                                if (res->equals(*new_form)) {
                                    flag = 0;
                                    std::cout << "Done!\n";
                                    break;
                                }
                            }
                        }

                        // Проверяем Modus Tollens
                        if (auto f1 = dynamic_cast<Implies*>(gamma[i])) {
                            if (auto neg_right = dynamic_cast<Not*>(gamma[j])) {
                                auto res = Modes_tollens(f1, neg_right);
                                if (res != nullptr) {
                                    log_file << "From: ";
                                    gamma[i]->print(log_file);
                                    log_file << " and ";
                                    gamma[j]->print(log_file);
                                    log_file << " derived (Modus Tollens): ";
                                    res->print(log_file);
                                    log_file << std::endl;
                                    new_gamma.push_back(res);

                                    if (res->equals(*new_form)) {
                                        flag = 0;
                                        std::cout << "Done!\n";
                                        break;
                                    }
                                }
                            }
                        }

                        // Проверяем Disjunctive Syllogism
                        if (auto f2 = dynamic_cast<Or*>(gamma[j])) {
                            if (auto not_left = dynamic_cast<Not*>(gamma[i])) {
                                auto res = Disjunctive_syllogism(not_left, f2);
                                if (res != nullptr) {
                                    log_file << "From: ";
                                    gamma[i]->print(log_file);
                                    log_file << " and ";
                                    gamma[j]->print(log_file);
                                    log_file << " derived (Disjunctive Syllogism): ";
                                    res->print(log_file);
                                    log_file << std::endl;
                                    new_gamma.push_back(res);

                                    if (res->equals(*new_form)) {
                                        flag = 0;
                                        std::cout << "Done!\n";
                                        break;
                                    }
                                }
                            }
                        }

                        // Проверяем Hypothetical Syllogism
                        if (auto f1 = dynamic_cast<Implies*>(gamma[i])) {
                            if (auto f2 = dynamic_cast<Implies*>(gamma[j])) {
                                auto res = Hypothetical_syllogism(f1, f2);
                                if (res != nullptr) {
                                    log_file << "From: ";
                                    gamma[i]->print(log_file);
                                    log_file << " and ";
                                    gamma[j]->print(log_file);
                                    log_file << " derived (Hypothetical Syllogism): ";
                                    res->print(log_file);
                                    log_file << std::endl;
                                    new_gamma.push_back(res);

                                    if (res->equals(*new_form)) {
                                        flag = 0;
                                        std::cout << "Done!\n";
                                        break;
                                    }
                                }
                            }
                        }

                        // Проверяем Simple Constructive Dilemma
                        if (auto f1 = dynamic_cast<Implies*>(gamma[i])) {
                            for (int k = 0; k < size; k++) {
                                if (auto f2 = dynamic_cast<Implies*>(gamma[k])) {
                                    if (auto f3 = dynamic_cast<Or*>(gamma[j])) {
                                        auto res = Simple_constructive_dilemma(f1, f2, f3);
                                        if (res != nullptr) {
                                            log_file << "From: ";
                                            gamma[i]->print(log_file);
                                            log_file << " and ";
                                            gamma[k]->print(log_file);
                                            log_file << " and ";
                                            gamma[j]->print(log_file);
                                            log_file << " derived (Simple Constructive Dilemma): ";
                                            res->print(log_file);
                                            log_file << std::endl;
                                            new_gamma.push_back(res);

                                            if (res->equals(*new_form)) {
                                                flag = 0;
                                                std::cout << "Done!\n";
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        // Проверяем Simple Destructive Dilemma
                        if (auto f1 = dynamic_cast<Implies*>(gamma[i])) {
                            for (int k = 0; k < size; k++) {
                                if (auto f2 = dynamic_cast<Implies*>(gamma[k])) {
                                    if (auto f3 = dynamic_cast<Or*>(gamma[j])) {
                                        auto res = Simple_destructive_dilemma(f1, f2, f3);
                                        if (res != nullptr) {
                                            log_file << "From: ";
                                            gamma[i]->print(log_file);
                                            log_file << " and ";
                                            gamma[k]->print(log_file);
                                            log_file << " and ";
                                            gamma[j]->print(log_file);
                                            log_file << " derived (Simple Destructive Dilemma): ";
                                            res->print(log_file);
                                            log_file << std::endl;
                                            new_gamma.push_back(res);

                                            if (res->equals(*new_form)) {
                                                flag = 0;
                                                std::cout << "Done!\n";
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        // Проверяем Complex Constructive Dilemma
                        if (auto f1 = dynamic_cast<Implies*>(gamma[i])) {
                            for (int k = 0; k < size; k++) {
                                if (auto f2 = dynamic_cast<Implies*>(gamma[k])) {
                                    if (auto f3 = dynamic_cast<Or*>(gamma[j])) {
                                        auto res = Complex_constructive_dilemma(f1, f2, f3);
                                        if (res != nullptr) {
                                            log_file << "From: ";
                                            gamma[i]->print(log_file);
                                            log_file << " and ";
                                            gamma[k]->print(log_file);
                                            log_file << " and ";
                                            gamma[j]->print(log_file);
                                            log_file << " derived (Complex Constructive Dilemma): ";
                                            res->print(log_file);
                                            log_file << std::endl;
                                            new_gamma.push_back(res);

                                            if (res->equals(*new_form)) {
                                                flag = 0;
                                                std::cout << "Done!\n";
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        // Проверяем Complex Destructive Dilemma
                        if (auto f1 = dynamic_cast<Implies*>(gamma[i])) {
                            for (int k = 0; k < size; k++) {
                                if (auto f2 = dynamic_cast<Implies*>(gamma[k])) {
                                    if (auto f3 = dynamic_cast<Or*>(gamma[j])) {
                                        auto res = Complex_destructive_dilemma(f1, f2, f3);
                                        if (res != nullptr) {
                                            log_file << "From: ";
                                            gamma[i]->print(log_file);
                                            log_file << " and ";
                                            gamma[k]->print(log_file);
                                            log_file << " and ";
                                            gamma[j]->print(log_file);
                                            log_file << " derived (Complex Destructive Dilemma): ";
                                            res->print(log_file);
                                            log_file << std::endl;
                                            new_gamma.push_back(res);

                                            if (res->equals(*new_form)) {
                                                flag = 0;
                                                std::cout << "Done!\n";
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    } else {
                        break;
                    }
                }
            }
        }

        // Добавляем новые формулы в gamma
        for (auto g : new_gamma) {
            gamma.push_back(g);
        }

        if (flag == 0) {
            break;
        }
    }

    // Записываем результаты в файл
    std::ofstream out("../result.txt");
    for (auto g : gamma) {
        g->print(out);
        out << std::endl;
    }
    out.close();

    log_file << "Exiting task1 with result: ";
    for (auto g : gamma) {
        g->print(log_file);
        log_file << " ";
    }
    log_file << std::endl;
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

    log_file.close();

    return 0;
}