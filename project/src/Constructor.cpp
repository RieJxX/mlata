#include "../includes/Constructor.hpp"

std::vector<char> from_file(std::string file_name){
    std::string line;
    std::ifstream in(file_name);
    std::vector<char> parce_str;
    if (in.is_open()){
        while (std::getline(in, line))
        {
            for (char sim :line){
                if (sim != ' ') parce_str.push_back(sim);
            }
        }
        
    }
    in.close();
    return parce_str;
}

void constr(std::string file_name, std::vector<int>& scob){
    std::vector<Formula*> forms;
    std::vector<char> parce_str = from_file(file_name);
    int i = 0;
    for (int i = 0; i < parce_str.size(); i++){
        if (parce_str[i] == '('){
            scob.push_back(1);
        }
        else if (65 <= parce_str[i] and parce_str[i] <= 90){
            scob.push_back(0);
            auto curr = new Variable(parce_str[i]);
            forms.push_back(curr);
        }
        else if (parce_str[i] == '|' || parce_str[i] == '*' || parce_str[i] == '>'){
            switch (parce_str[i])
            {
            case '|':
                forms.push_back(new Or(nullptr, nullptr));
                scob.push_back(3);
                break;
            case '*':
                forms.push_back(new And(nullptr, nullptr));
                scob.push_back(4);

                break;
            case '>':
                forms.push_back(new Implies(nullptr, nullptr));
                scob.push_back(5);

                break;
            default:
                break;
            }
        } 
        else if (parce_str[i] == '!'){
            scob.push_back(6);
            forms.push_back(new Not(nullptr));
        }
        else{
            scob.push_back(2);
        }

    }
}

Formula* tree_arch(std::string file_name){
    std::vector<int> scob;
    constr(file_name, scob);
    std::vector<char> parce_str = from_file(file_name);
    std::vector<std::vector<Formula*>> tree;
    int curr_index = -1;

    for (int i = scob.size()-1; i>=0; i--){

        if (scob[i] == 2){
            std::vector<Formula*> in;
            tree.insert(tree.begin(), in);
            if (scob[i+1] == 5 ) curr_index--;
            curr_index++;
        }

        else if(scob[i] == 1){
            //cbor
            int j = tree[curr_index].size()-1;
            if (tree[curr_index][j] != nullptr){
                if (tree[curr_index].size() > 1){
                    while(true){
                        if (auto var = dynamic_cast<Variable*>(tree[curr_index][j])){
                            j--;
                        }

                        if (auto imp = dynamic_cast<Implies*>(tree[curr_index][j])){
                            if (imp->get_right() == nullptr){
                                if ((j + 1) < tree[curr_index].size()){
                                    imp->set_right(tree[curr_index][j+1]);
                                    tree[curr_index].erase(tree[curr_index].begin()+j+1, tree[curr_index].begin()+j+2);
                                }
                                else{
                                    imp->set_right(tree[curr_index+1][0]);
                                    tree.erase(tree.begin()+curr_index+1, tree.begin()+curr_index+2);
                                }
                            }

                            if (imp->get_left() == nullptr){
                                if (j - 1 > -1){
                                    imp->set_left(tree[curr_index][j-1]);
                                    tree[curr_index].erase(tree[curr_index].begin()+j-1, tree[curr_index].begin()+j);
                                }
                                else{
                                    imp->set_left(tree[curr_index-1][0]);
                                    tree.erase(tree.begin()+curr_index-1, tree.begin()+curr_index);
                                    curr_index--;
                                }
                            }
                        }

                        if (auto and_i = dynamic_cast<And*>(tree[curr_index][j])){
                            if (and_i->get_right() == nullptr){
                                if ((j + 1) < tree[curr_index].size()){
                                    and_i->set_right(tree[curr_index][j+1]);
                                    tree[curr_index].erase(tree[curr_index].begin()+j+1, tree[curr_index].begin()+j+2);
                                }
                                else{
                                    and_i->set_right(tree[curr_index+1][0]);
                                    tree.erase(tree.begin()+curr_index+1, tree.begin()+curr_index+2);
                                }
                            }
                            if (and_i->get_left() == nullptr){
                                if (j - 1 > -1){
                                    and_i->set_left(tree[curr_index][j-1]);
                                    tree[curr_index].erase(tree[curr_index].begin()+j-1, tree[curr_index].begin()+j);
                                }
                                else{
                                    and_i->set_left(tree[curr_index-1][0]);
                                    tree.erase(tree.begin()+curr_index-1, tree.begin()+curr_index);
                                    curr_index--;
                                }
                            }
                        }
                        
                        if (auto or_i = dynamic_cast<Or*>(tree[curr_index][j])){
                            if (or_i->get_right() == nullptr){
                                if ((j + 1) < tree[curr_index].size()){
                                    or_i->set_right(tree[curr_index][j+1]);
                                    tree[curr_index].erase(tree[curr_index].begin()+j+1, tree[curr_index].begin()+j+2);
                                }
                                else{
                                    or_i->set_right(tree[curr_index+1][0]);
                                    tree.erase(tree.begin()+curr_index+1, tree.begin()+curr_index+2);
                                }
                            }
                            if (or_i->get_left() == nullptr){
                                if (j - 1 > -1){
                                    or_i->set_left(tree[curr_index][j-1]);
                                    tree[curr_index].erase(tree[curr_index].begin()+j-1, tree[curr_index].begin()+j);
                                }
                                else{
                                    or_i->set_left(tree[curr_index-1][0]);
                                    tree.erase(tree.begin()+curr_index-1, tree.begin()+curr_index);
                                    curr_index--;
                                }
                            }
                        }

                        else j--;
                        if (tree[curr_index].size() == 1) break;
                    }
                }

                else{
                    if (auto imp = dynamic_cast<Implies*>(tree[curr_index][j])){
                        if (imp->get_right() == nullptr){
                            imp->set_right(tree[curr_index+1][0]);
                            tree.erase(tree.begin()+curr_index+1, tree.begin()+curr_index+2);
                        }
                        if (imp->get_left() == nullptr){
                            imp->set_left(tree[curr_index-1][0]);
                            tree.erase(tree.begin()+curr_index-1, tree.begin()+curr_index);
                            curr_index--;
                        }
                    }
                    else if (auto and_i = dynamic_cast<And*>(tree[curr_index][j])){
                        if (and_i->get_right() == nullptr){
                            and_i->set_right(tree[curr_index+1][0]);
                            tree.erase(tree.begin()+curr_index+1, tree.begin()+curr_index+2);
                        }
                        if (and_i->get_left() == nullptr){
                            and_i->set_left(tree[curr_index-1][0]);
                            tree.erase(tree.begin()+curr_index-1, tree.begin()+curr_index);
                            curr_index--;
                        }
                    }
                    else if (auto or_i = dynamic_cast<Or*>(tree[curr_index][j])){
                        if (or_i->get_right() == nullptr){
                            or_i->set_right(tree[curr_index+1][0]);
                            tree.erase(tree.begin()+curr_index+1, tree.begin()+curr_index+2);
                        }
                        if (or_i->get_left() == nullptr){
                            or_i->set_left(tree[curr_index-1][0]);
                            tree.erase(tree.begin()+curr_index-1, tree.begin()+curr_index);
                            curr_index--;
                        }
                    }
                    
                }
            }

            if (i != 0 && scob[i-1] == 1 && (curr_index+1 < tree.size())){
                curr_index++;
            }
            else curr_index--;

            
        }
        else if (scob[i] == 0){
            tree[curr_index].insert(tree[curr_index].begin(), new Variable(parce_str[i]));
        }
        else if (scob[i] == 5){
            tree[curr_index].insert(tree[curr_index].begin(), new Implies(nullptr, nullptr));
        }
        else if (scob[i] == 3){
            tree[curr_index].insert(tree[curr_index].begin(), new Or(nullptr, nullptr));
        }
        else if (scob[i] == 4){
            tree[curr_index].insert(tree[curr_index].begin(), new And(nullptr, nullptr));
        }
        else if (scob[i] == 6){
            tree[curr_index].insert(tree[curr_index].begin(), new Not(tree[curr_index][0]));
            tree[curr_index].erase(tree[curr_index].begin()+1, tree[curr_index].begin()+2);

        }

    }
    return tree[0][0];
}

Formula* constr_formula(std::string file_name){
    Formula* formula = tree_arch(file_name);
    return formula;
}


