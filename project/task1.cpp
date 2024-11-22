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

Formula* rule_dis(Or* form){
    auto new_form = new Implies(nullptr, nullptr);
    auto not_left = new Not(form->get_left());
    new_form->set_left(not_left);
    new_form->set_right(form->get_right());
    return new_form;
}

Not* rule_con(Formula* form_i) {
    auto and_i = dynamic_cast<And*>(form_i);
    auto form = dynamic_cast<And*>(form_i->clone());

    auto imp = new Implies(nullptr, nullptr);
    auto not_right = new Not(form->get_right()->clone());

    imp->set_left(form->get_left()->clone());

    imp->set_right(not_right);

    auto new_form = new Not(imp);

    return new_form;
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

    for (auto i: new_aks_1){
        for (auto j: new_aks_2){
            auto i_d = dynamic_cast<Implies*>(i);
            auto j_d = dynamic_cast<Implies*>(j);
            if (Modes_ponens(i_d,j_d) != nullptr){
                i_d->print(std::cout);
                std::cout<<"\t";
                j_d->print(std::cout);
                std::cout<<std::endl;
                gamma.push_back(Modes_ponens(i_d,j_d));
            }
        }
    }
    std::cout<<std::endl;
    for (auto g: gamma){
        g->print(std::cout);
        std::cout<<std::endl;
    }

     for (int i = 0; i < gamma.size()-1; i++){
        for (int j = i+1; j < gamma.size(); j++){
            auto i_d = dynamic_cast<Implies*>(gamma[i]);
            auto j_d = dynamic_cast<Implies*>(gamma[j]);
            if (Modes_ponens(i_d,j_d) != nullptr){
                i_d->print(std::cout);
                std::cout<<"\t";
                j_d->print(std::cout);
                std::cout<<std::endl;
                gamma.push_back(Modes_ponens(i_d,j_d));
            }
        }
    }
    std::cout<<std::endl;
    std::ofstream out("result.txt");

    for (auto g: gamma){
        g->print(out);
        out<<std::endl;
    }
    out.close();

    for (int i = 0; i < gamma.size()-1; i++){
        for (int j = i+1; j < gamma.size(); j++){
            auto i_d = dynamic_cast<Implies*>(gamma[i]);
            auto j_d = dynamic_cast<Implies*>(gamma[j]);
            if (Modes_ponens(i_d,j_d) != nullptr){
                i_d->print(std::cout);
                std::cout<<"\t";
                j_d->print(std::cout);
                std::cout<<std::endl;
                gamma.push_back(Modes_ponens(i_d,j_d));
            }
        }
    }
}


int main() {

    auto A = new Variable('P');
    auto B = new Variable('Q');
    auto C = new Variable('R');
    auto D = new Variable('T');

    Modes_ponens(A , new Implies(A , B))->print(std::cout);
    std::cout << " <-- Modes ponens" << std::endl;

    Modes_tollens(new Implies(A , B) , new Not(B))->print(std::cout);
    std::cout << " <-- Modes tollens" << std::endl;

    Disjunctive_syllogism(new Not(A) , new Or(A , B))->print(std::cout);
    std::cout << " <-- Дизъюнктивный силлогизм" << std::endl;

    Hypothetical_syllogism(new Implies(A , B) , new Implies(B , C))->print(std::cout);
    std::cout << " <-- Гипотетический силлогизм" << std::endl;

    Disjunctive_syllogism_xor(A , new Xor(A , B))->print(std::cout);
    std::cout << " <-- Разделительный силлогизм	" << std::endl;

    Simple_constructive_dilemma(new Implies(A , C) , new Implies(B , C) , new Or(A , B))->print(std::cout);
    std::cout << " <-- Простая конструктивная дилемма" << std::endl;

    Complex_constructive_dilemma(new Implies(A , C) , new Implies(B , D) , new Or(A , B))->print(std::cout);
    std::cout << " <-- Сложная конструктивная дилемма" << std::endl;

    Simple_destructive_dilemma(new Implies(A , C) , new Implies(A , B) , new Or(new Not(C) , new Not(B)))->print(std::cout);
    std::cout << " <-- Простая деструктивная дилемма" << std::endl;

    Complex_destructive_dilemma(new Implies(A , C) , new Implies(B , D) , new Or(new Not(C) , new Not(D)))->print(std::cout);
    std::cout << " <-- Сложная деструктивная дилемма" << std::endl;

    //task1(form, aksiome1, aksiome2, aksiome3);

    // std::vector<Formula*> param;
    // take_params(form, param);

    // for (auto i: param){
    //     i->print(std::cout);
    //     std::cout<<std::endl;
    // }

    // std::vector<Formula*> new_aks_1 = new_aks1(aksiome1, param);

    // for (auto i: new_aks_1){
    //     i->print(std::cout);
    //     std::cout<<std::endl;
    // }
    // std::cout<<std::endl;
    // std::vector<Formula*> new_aks_2 = new_aks2(aksiome2, param);

    // for (auto i: new_aks_2){
    //     i->print(std::cout);
    //     std::cout<<std::endl;
    // }

    // std::cout<<std::endl;
    
    // auto r = dynamic_cast<Implies*>(form);
    // auto new_input = rule_con(r->get_left());
    // auto new_form = r->clone();
    // new_form->set_left(new_input);
    // std::vector<Formula*> new_param;
    // take_params(new_form, new_param);
    // for (auto i: new_param){
    //     i->print(std::cout);
    //     std::cout<<std::endl;
    // }
    // std::cout<<std::endl;

    // std::vector<Formula*> new_aks_1_1 = new_aks1(aksiome1, new_param);

    // for (auto i: new_aks_1_1){
    //     i->print(std::cout);
    //     std::cout<<std::endl;
    // }
    // std::cout<<std::endl;
    // std::vector<Formula*> new_aks_2_2 = new_aks2(aksiome2, new_param);

    // for (auto i: new_aks_2_2){
    //     i->print(std::cout);
    //     std::cout<<std::endl;
    // }

    // std::cout<<std::endl;
    
    // std::vector<Formula*> new_aks_3 = new_aks3(aksiome3, param);

    // for (auto i: new_aks_3){
    //     i->print(std::cout);
    //     std::cout<<std::endl;
    // }
    // std::vector<Formula*> aks;
    // aks.push_back(new Variable('A'));
    // aks.push_back(new Variable('B'));
    // for (int l = 0; l < aks.size(); l++){
    //     auto j = aks[l];
    //     for (auto i: new_aks_1){
    //         //const char A = 'A';
    //         if (auto j_d = dynamic_cast<Variable*>(j)){
    //             auto i_d = dynamic_cast<Implies*>(i);
    //             if (Modes_ponens(j_d, i_d) != nullptr){
    //                     aks.push_back(Modes_ponens(j_d, i_d));
    //                     std::cout<<std::endl;
    //                 }
    //                 else std::cout<<"null\n";
    //         }

    //         if (auto j_d = dynamic_cast<Implies*>(j)){
    //             auto i_d = dynamic_cast<Implies*>(i);
    //             if (Modes_ponens(j_d, i_d) != nullptr){
    //                     aks.push_back(Modes_ponens(j_d, i_d));
    //                     std::cout<<std::endl;
    //                 }
    //             else std::cout<<"null\n";
    //         }
    //     }
    //     for (auto x: aks){
    //     x->print(std::cout);
    //     std::cout<<std::endl;
    // }
    // }
    // for (auto x: aks){
    //     x->print(std::cout);
    //     std::cout<<std::endl;
 //   }
    // for (auto i: new_aks_1){
    //     for (auto j: new_aks_2){
    //         auto i_d = dynamic_cast<Implies*>(i);
    //         auto j_d = dynamic_cast<Implies*>(j);
    //         if (Modes_ponens(i_d,j_d) != nullptr){
    //             Modes_ponens(i_d,j_d)->print(std::cout);
    //         }
    //         else std::cout<<"null\n";
    //     }
    // }

    // for (auto i: new_aks_1){
    //     for (auto j: new_aks_3){
    //         auto i_d = dynamic_cast<Implies*>(i);
    //         auto j_d = dynamic_cast<Implies*>(j);
    //         if (Modes_ponens(i_d,j_d) != nullptr){
    //             Modes_ponens(i_d,j_d)->print(std::cout);
    //         }
    //         else std::cout<<"null\n";
    //     }
    // }

    // for (auto i: new_aks_2){
    //     for (auto j: new_aks_3){
    //         auto i_d = dynamic_cast<Implies*>(i);
    //         auto j_d = dynamic_cast<Implies*>(j);
    //         if (Modes_ponens(i_d,j_d) != nullptr){
    //             Modes_ponens(i_d,j_d)->print(std::cout);
    //         }
    //         else std::cout<<"null\n";
    //     }
    // }

    // for (auto i: new_aks_3){
    //     for (auto j: new_aks_2){
    //         auto i_d = dynamic_cast<Implies*>(i);
    //         auto j_d = dynamic_cast<Implies*>(j);
    //         if (Modes_ponens(i_d,j_d) != nullptr){
    //             Modes_ponens(i_d,j_d)->print(std::cout);
    //         }
    //         else std::cout<<"null\n";
    //     }
    // }


    // delete aksiome1;
    // delete aksiome2;
    return 0;
}
