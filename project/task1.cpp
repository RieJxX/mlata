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

//Modes_ponens
template<typename T1, typename T2> Formula* Modes_ponens(T1 formula1, T2 formula2) {
    if (formula1->equals(*formula2->get_left())) {
        //std::cout << "hello\n";
        return formula2->get_right();
    } else {
        // обработка??
        return nullptr;
    }
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

    // std::ofstream outFile("result.txt");

    auto aksiome1 = aksiome_1();
    std::cout<<"AKS1"<<std::endl;
    auto aksiome2 = aksiome_2();
    std::cout<<"AKS2"<<std::endl;
    auto aksiome3 = aksiome_3();
    std::cout<<"AKS3"<<std::endl;

    auto form = tree_arch("../input.txt");
    form->print(std::cout);

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


    delete aksiome1;
    delete aksiome2;
    return 0;
}
