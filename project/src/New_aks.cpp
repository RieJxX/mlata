#include "../includes/New_aks.hpp"

void take_params(Formula* formula, std::vector<Formula*>& params){
    if (auto imp = dynamic_cast<Implies*>(formula)){
        if (!dynamic_cast<Implies*>(imp->get_left())) params.push_back(imp->get_left());
        if (!dynamic_cast<Implies*>(imp->get_right())) params.push_back(imp->get_right());
        take_params(imp->get_left(), params);
        take_params(imp->get_right(), params);
    }
    else if (auto andi = dynamic_cast<And*>(formula)){
        params.push_back(andi->get_left());
        params.push_back(andi->get_right());
        take_params(andi->get_left(), params);
        take_params(andi->get_right(), params);
    }
    else if (auto ori = dynamic_cast<Or*>(formula)){
        params.push_back(ori->get_left());
        params.push_back(ori->get_right());
        take_params(ori->get_left(), params);
        take_params(ori->get_right(), params);
    }  
    for (int i = 0; i < params.size()-1; i++){
        for (int j = i+1; j < params.size(); j++){
            if (params[i]->equals(*params[j])){
                params.erase(params.begin()+j, params.begin()+j+1);
            }
        }
    }
}

std::vector<Formula*> new_aks1(Formula* aksiome_1, const std::vector<Formula*>& param_f){
    //заготовка для других аксиом
    // std::vector<Formula*> param_a;
    // take_params(aksiome_1, param_a);
    // param_a.erase(std::unique(param_a.begin(), param_a.end(), [](const Formula* lhs, const Formula* rhs) {
    //     return lhs->equals(*rhs);
    // }), param_a.end());
    std::vector<Formula*> new_aks;
    for (auto f1: param_f){
        //f1->print(std::cout);
        for (auto f2: param_f){
            auto aks1 = dynamic_cast<Implies*>(aksiome_1);
            auto imp = dynamic_cast<Implies*>(aks1->clone());
            imp->set_left(f1->clone());

            auto imp2 = dynamic_cast<Implies*>(imp->get_right());
            imp2->set_left(f2->clone());
            imp2->set_right(f1->clone());
            imp->set_right(imp2->clone());

            new_aks.push_back(imp);
        }
    }
    return new_aks;
}

std::vector<Formula*> new_aks2(Formula* aksiome_2, std::vector<Formula*> param_f){
    std::vector<Formula*> new_aks;
    for (auto f1: param_f){
        for (auto f2: param_f){
            for (auto f3: param_f){
                auto aks2 = dynamic_cast<Implies*>(aksiome_2);
                auto imp = dynamic_cast<Implies*>(aks2->clone());

                auto imp1 = dynamic_cast<Implies*>(imp->get_left());
                imp1->set_left(f1->clone());

                auto imp3 = dynamic_cast<Implies*>(imp1->get_right());
                imp3->set_left(f2->clone());
                imp3->set_right(f3->clone());
                imp1->set_right(imp3->clone());

                imp->set_left(imp1->clone());

                auto imp2 = dynamic_cast<Implies*>(imp->get_right());
                auto imp4 = dynamic_cast<Implies*>(imp2->get_left());   
                imp4->set_left(f1->clone());
                imp4->set_right(f2->clone());
                imp2->set_left(imp4->clone());

                auto imp5 = dynamic_cast<Implies*>(imp2->get_right());
                imp5->set_left(f1->clone());
                imp5->set_right(f3->clone());
                imp2->set_right(imp5->clone());

                imp->set_right(imp2->clone());

                new_aks.push_back(imp);
            }
        }
    }
    return new_aks;
}

std::vector<Formula*> new_aks3(Formula* aksiome_3, std::vector<Formula*> param_f){
    std::vector<Formula*> new_aks;
    for (auto f1: param_f){
        for (auto f2: param_f){
            auto aks3 = dynamic_cast<Implies*>(aksiome_3);
            auto imp = dynamic_cast<Implies*>(aks3->clone());

            auto imp1 = dynamic_cast<Implies*>(imp->get_left());
            imp1->set_left(new Not(f2->clone()));
            imp1->set_right(new Not(f1->clone()));


            auto imp2 = dynamic_cast<Implies*>(imp->get_right());
            auto imp4 = dynamic_cast<Implies*>(imp2->get_left());   
            imp4->set_left(new Not(f2->clone()));
            imp4->set_right(f1->clone());

            imp2->set_right(f2->clone());
            imp2->set_left(imp4->clone());

            imp->set_left(imp1->clone());   
            imp->set_right(imp2->clone());

            new_aks.push_back(imp);
        }
    }
    return new_aks;
}