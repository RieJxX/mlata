#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <vector>

//определение классов
//Базовый класс
class Formula{
    public:
        virtual ~Formula() = default;
        virtual void print(std::ostream& os) const=0;
};


class Variable : public Formula{
    public:
        Variable(const char name): name(name){}
        void print(std::ostream& os) const override{
            os << name;
        }

        char get_name(){
            return name;
        }

    private:
        const char name;
};

class And : public Formula {
    public:
        And(Formula* left, Formula* right)
            : left(std::move(left)), right(std::move(right)) {}

        void print(std::ostream& os) const override {
            os << "(";
            if (left == nullptr) os<<" ";
            else left->print(os);
            os << " * ";
            if (right == nullptr) os<<" ";
            else right->print(os);
            os << ")";
        }

        void set_left(Formula* left_in){
            left = left_in;
        }

        void set_right(Formula* right_in){
            right = right_in;
        }

        Formula* get_left(){
            return left;
        }

        Formula* get_right(){
            return right;
        }

        ~And() {
            delete left;
            delete right;
        }

    private:
        Formula* left;
        Formula* right;
};

class Or : public Formula {
    public:
        Or(Formula* left, Formula* right)
            : left(std::move(left)), right(std::move(right)) {}

        void print(std::ostream& os) const override {
            os << "(";
            if (left == nullptr) os<<" ";
            else left->print(os);
            os << " | ";
            if (right == nullptr) os<<" ";
            else right->print(os);
            os << ")";
        }

        void set_left(Formula* left_in){
            left = left_in;
        }

        void set_right(Formula* right_in){
            right = right_in;
        }

        Formula* get_left(){
            return left;
        }

        Formula* get_right(){
            return right;
        }

        ~Or() {
            delete left;
            delete right;
        }

    private:
        Formula* left;
        Formula* right;
    };


class Not : public Formula {
    public:
        Not(Formula* operand) : operand(std::move(operand)) {}

        void print(std::ostream& os) const override {
            os << "!";
            if (operand == nullptr) os << " ";
            else operand->print(os);
        }

        void set_operand(Formula* new_oper){
            operand = new_oper;
        }

        Formula* get_operand(){
            return operand;
        }

        ~Not() {
            delete operand;
        }
    private:
        Formula* operand;
};

class Implies : public Formula {
    public:
        Implies(Formula* left, Formula* right)
            : left(std::move(left)), right(std::move(right)) {}

        void print(std::ostream& os) const override {
            os << "(";
            if (left == nullptr) os<<" ";
            else left->print(os);
            os << " > ";
            if (right == nullptr) os<<" ";
            else right->print(os);
            os << ")";
        }

        ~Implies() {
            delete left;
            delete right;
        }

        void set_left(Formula* left_in){
            left = left_in;
        }

        void set_right(Formula* right_in){
            right = right_in;
        }

        Formula* get_left(){
            return left;
        }

        Formula* get_right(){
            return right;
        }
        
    private:
        Formula* left;
        Formula* right;
};

//сбор формулы
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
        //std::cout<<parce_str[i]<<"\n";
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
    for (auto r: scob){
        std::cout<<r<<" ";
    }
    std::cout<<std::endl;
}

void tree_arch(std::string file_name){
    std::vector<int> scob;
    constr(file_name, scob);
    std::vector<char> parce_str = from_file(file_name);
    std::vector<std::vector<Formula*>> tree;
    //int i_forms = 1;
    int curr_index = -1;
    for (int i = scob.size()-1; i>=0; i--){
        std::cout<<"scob "<<scob[i]<<"\t";
        std::cout<<"curr "<<curr_index<<"\n";
        if (scob[i] == 2){
            std::vector<Formula*> in;
            tree.insert(tree.begin(), in);
            if (scob[i+1] == 5 ) curr_index--;
            curr_index++;
        }

        else if(scob[i] == 1){
            //cbor
            int j = tree[curr_index].size()-1;
            int flag = 0;
            if (tree[curr_index][j] != nullptr){
                if (tree[curr_index].size() > 1){
                    while(true){
                        if (auto var = dynamic_cast<Variable*>(tree[curr_index][j])){
                            j--;
                        }

                        if (auto imp = dynamic_cast<Implies*>(tree[curr_index][j])){
                            //tree[curr_index][j]->print(std::cout);
                            if (imp->get_right() == nullptr){
                                if ((j + 1) < tree[curr_index].size()){
                                    //std::cout<<"me"<<j<<"\n";
                                    tree[curr_index][j]->print(std::cout);
                                    imp->set_right(tree[curr_index][j+1]);
                                    tree[curr_index].erase(tree[curr_index].begin()+j+1, tree[curr_index].begin()+j+2);
                                }
                                else{
                                    imp->set_right(tree[curr_index+1][0]);
                                    tree.erase(tree.begin()+curr_index+1, tree.begin()+curr_index+2);
                                }
                            }
                            tree[curr_index][j]->print(std::cout);
                                std::cout<<std::endl;
                            if (imp->get_left() == nullptr){
                                if (j - 1 > -1){
                                    imp->set_left(tree[curr_index][j-1]);
                                    tree[curr_index].erase(tree[curr_index].begin()+j-1, tree[curr_index].begin()+j);
                                }
                                else{
                                    imp->set_left(tree[curr_index-1][0]);
                                    // std::cout<<"me2\n";
                                    // tree[curr_index][0]->print(std::cout);
                                    // std::cout<<std::endl;
                                    tree.erase(tree.begin()+curr_index-1, tree.begin()+curr_index);
                                    curr_index--;
                                }
                            }
                        }

                        if (auto and_i = dynamic_cast<And*>(tree[curr_index][j])){
                            //tree[curr_index][j]->print(std::cout);
                            if (and_i->get_right() == nullptr){
                                if ((j + 1) < tree[curr_index].size()){
                                    //std::cout<<"me"<<j<<"\n";
                                    tree[curr_index][j]->print(std::cout);
                                    and_i->set_right(tree[curr_index][j+1]);
                                    tree[curr_index].erase(tree[curr_index].begin()+j+1, tree[curr_index].begin()+j+2);
                                }
                                else{
                                    and_i->set_right(tree[curr_index+1][0]);
                                    tree.erase(tree.begin()+curr_index+1, tree.begin()+curr_index+2);
                                }
                            }
                            tree[curr_index][j]->print(std::cout);
                                std::cout<<std::endl;
                            if (and_i->get_left() == nullptr){
                                if (j - 1 > -1){
                                    and_i->set_left(tree[curr_index][j-1]);
                                    tree[curr_index].erase(tree[curr_index].begin()+j-1, tree[curr_index].begin()+j);
                                }
                                else{
                                    and_i->set_left(tree[curr_index-1][0]);
                                    // std::cout<<"me2\n";
                                    // tree[curr_index][0]->print(std::cout);
                                    // std::cout<<std::endl;
                                    tree.erase(tree.begin()+curr_index-1, tree.begin()+curr_index);
                                    curr_index--;
                                }
                            }
                        }
                        
                        if (auto or_i = dynamic_cast<Or*>(tree[curr_index][j])){
                            //tree[curr_index][j]->print(std::cout);
                            if (or_i->get_right() == nullptr){
                                if ((j + 1) < tree[curr_index].size()){
                                    //std::cout<<"me"<<j<<"\n";
                                    tree[curr_index][j]->print(std::cout);
                                    or_i->set_right(tree[curr_index][j+1]);
                                    tree[curr_index].erase(tree[curr_index].begin()+j+1, tree[curr_index].begin()+j+2);
                                }
                                else{
                                    or_i->set_right(tree[curr_index+1][0]);
                                    tree.erase(tree.begin()+curr_index+1, tree.begin()+curr_index+2);
                                }
                            }
                            tree[curr_index][j]->print(std::cout);
                                std::cout<<std::endl;
                            if (or_i->get_left() == nullptr){
                                if (j - 1 > -1){
                                    or_i->set_left(tree[curr_index][j-1]);
                                    tree[curr_index].erase(tree[curr_index].begin()+j-1, tree[curr_index].begin()+j);
                                }
                                else{
                                    or_i->set_left(tree[curr_index-1][0]);
                                    // std::cout<<"me2\n";
                                    // tree[curr_index][0]->print(std::cout);
                                    // std::cout<<std::endl;
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
                    flag = 1;
                    std::cout<<"tyt\n";
                    if (auto imp = dynamic_cast<Implies*>(tree[curr_index][j])){
                            tree[curr_index][j]->print(std::cout);
                        if (imp->get_right() == nullptr){
                            imp->set_right(tree[curr_index+1][0]);
                            std::cout<<"tyt\n";
                            tree.erase(tree.begin()+curr_index+1, tree.begin()+curr_index+2);
                        }
                        if (imp->get_left() == nullptr){
                            imp->set_left(tree[curr_index-1][0]);
                            tree.erase(tree.begin()+curr_index-1, tree.begin()+curr_index);
                            //curr_index--;
                        }
                    }
                }
            }

            if (i != 0 && scob[i-1] == 1 && !flag && (curr_index+1 < tree.size())){
                std::cout<<"hel\n";
                for (auto k: tree){
            if (k.size() == 0) std::cout<<"null";
            for (auto l: k){
                if (k.size() != 0) l->print(std::cout);
            }
            std::cout<<"\t->\t";
        }
        std::cout<<"\n";
                curr_index++;
            }
            else if (flag) curr_index = tree.size()-2;
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
        std::cout<<"curr "<<curr_index<<"\n";
        std::cout<<"new size "<<tree.size()<<"\n";
        for (auto k: tree){
            if (k.size() == 0) std::cout<<"null";
            for (auto l: k){
                if (k.size() != 0) l->print(std::cout);
            }
            std::cout<<"\t->\t";
        }
        std::cout<<"\n";
    }
    if (tree.size() > 1){}
    for (auto i: tree){
        for (auto j: i){
            j->print(std::cout);
        }
        std::cout<<"\t";
    }   
}

void insert_forms(std::vector<Formula*>& forms, int& j, const auto& obj){
    int i = 0;
    if (obj->get_right() == nullptr){
            obj->set_right(forms[forms.size() - j+1]);
            forms.erase(forms.begin()+(forms.size() - j+1), forms.begin()+(forms.size() - j+2));
            j = 0;
    }
    else if (obj->get_left() == nullptr){
        i = j + 1;
        if (auto prev = dynamic_cast<Implies*>(forms[forms.size() - i])){
            if (prev->get_right() != nullptr && prev->get_left() != nullptr){
                obj->set_left(prev);
            }
            forms[forms.size() - j] = obj;
            forms.erase(forms.begin()+(forms.size() - i), forms.begin()+(forms.size() - i+1));
        }
        else if (auto prev = dynamic_cast<And*>(forms[forms.size() - i])){
            if (prev->get_right() != nullptr && prev->get_left() != nullptr){
                obj->set_left(prev);
            }
            forms[forms.size() - j] = obj;
            forms.erase(forms.begin()+(forms.size() - i), forms.begin()+(forms.size() - i+1));
        }
        else if (auto prev = dynamic_cast<Or*>(forms[forms.size() - i])){
            if (prev->get_right() != nullptr && prev->get_left() != nullptr){
                obj->set_left(prev);
            }
            forms[forms.size() - j] = obj;
            forms.erase(forms.begin()+(forms.size() - i), forms.begin()+(forms.size() - i+1));
        }
        else if (auto prev = dynamic_cast<Not*>(forms[forms.size() - i])){
            obj->set_left(prev);
            forms[forms.size() - j] = obj;
            forms.erase(forms.begin()+(forms.size() - i), forms.begin()+(forms.size() - i+1));
        }
    }
    j++;
}

// убрать дублир по возможности
Formula* tree_formula(std::vector<Formula*> forms, std::vector<int> scob){
        for (auto r: forms){
        r->print(std::cout);

    }
            std::cout<<"\n";
            //std::cout<<std::endl;
    int j = 1, i;
    while (true){
        if (auto variable = dynamic_cast<Variable*>(forms[forms.size() - j])){
            i = j + 1;
            if (auto prev = dynamic_cast<Implies*>(forms[forms.size() - i])){
                //std::cout<<"hello\n";
                if (prev->get_right() == nullptr){
                    prev->set_right(variable);
                }
                forms[forms.size() - i] = prev;
            }
            else if (auto prev = dynamic_cast<And*>(forms[forms.size() - i])){
                if (prev->get_right() == nullptr){
                    prev->set_right(variable);
                }
                forms[forms.size() - i] = prev;
            }
            else if (auto prev = dynamic_cast<Or*>(forms[forms.size() - i])){
                if (prev->get_right() == nullptr){
                    prev->set_right(variable);
                }
                forms[forms.size() - i] = prev;
            }
            else if (auto prev = dynamic_cast<Not*>(forms[forms.size() - i])){
                prev->set_operand(variable);
                forms[forms.size() - i] = prev;
            }
            forms.erase(forms.begin()+(forms.size() - j), forms.begin()+(forms.size() - j+1));
        }
        else {
            j++;
        }
        //i++;
        if (j >= forms.size()) break;

    }

        for (auto r: forms){
        r->print(std::cout);

        }
            std::cout<<"\n";
    j = 1;
    i = 0;
    int index_scob = 1;
    while (true){
        std::cout<<j<<"\n";

        if(auto implies = dynamic_cast<Implies*>(forms[forms.size() - j])){
           
            insert_forms(forms, j, implies);
        }
        else if(auto andi = dynamic_cast<And*>(forms[forms.size() - j])){
           
            insert_forms(forms, j, andi);
        }
        else if(auto ori = dynamic_cast<Or*>(forms[forms.size() - j])){
            
            insert_forms(forms, j, ori);
        }
        else{
            // auto not_i = dynamic_cast<Not*>(forms[forms.size()-j]);
            // if (not_i->get_operand() != nullptr){

            // }
            j++;
        }

        if (j > forms.size()) break;

        for (auto r: forms){
        r->print(std::cout);
        }
        std::cout<<"\n";

    }
    

    return forms[0];
}

Formula* constr_formula(std::string file_name){
    std::vector<Formula*> forms;
    std::vector<int> scob;
    //constr(file_name, forms, scob);
    Formula* formula = tree_formula(forms, scob);
    return formula;
}

//aksioms
Formula* aksiome_1(){
    Formula* aksiome = constr_formula("aks1.txt");
    return aksiome;
}

Formula* aksiome_2(){
    Formula* aksiome = constr_formula("aks2.txt");
    return aksiome;
}

Formula* aksiome_3(){
    Formula* aksiome = constr_formula("aks3.txt");
    return aksiome;
}


//Modes_ponens
template<typename T1, typename T2> Formula* Modes_ponens(T1* formula1, T2* formula2){
    if ((*formula1) == (*formula2)->get_left()){
        return (*formula2)->get_right();
    }
    else{
        //обработка??
        return nullptr;
    }
    //return nullptr;
}


int main() {

    std::ofstream outFile("result.txt");

    //auto aksiome1 = aksiome_1();
    //auto aksiome2 = aksiome_2();
    //auto aksiome3 = aksiome_3();
    std::vector<Formula*> forms;
    std::vector<int> scob;
    //constr("aks2.txt", forms, scob);
    tree_arch("input.txt");
    //aksiome2->print(std::cout);
    //aksiome2->print(std::cout);

    // if (outFile.is_open()){
    //     ak1->print(outFile);
    //     outFile << std::endl;
    //     outFile.close();
    //     std::cout << "Формула успешно записана в файл result.txt" << std::endl;
    // } else {
    //     std::cerr << "Не удалось открыть файл для записи" << std::endl;
    // }
    //if (auto variable = dynamic_cast<Implies*>(formula)) std::cout<<"es\n";
    // delete aksiome1;
    // delete aksiome2;
    //delete aksiome3;
    
    return 0;
}
// void insert_forms(std::vector<Formula*>& forms, int& j, const auto& obj){
//     int i = 0;
//     if (obj->get_right() == nullptr){
//             obj->set_right(forms[forms.size() - j+1]);
//             forms.erase(forms.begin()+(forms.size() - j+1), forms.begin()+(forms.size() - j+2));
//             j = 0;
//     }
//     else if (obj->get_left() == nullptr){
//         i = j + 1;
//         if (auto prev = dynamic_cast<Implies*>(forms[forms.size() - i])){
//             if (prev->get_right() != nullptr && prev->get_left() != nullptr){
//                 obj->set_left(prev);
//             }
//             forms[forms.size() - j] = obj;
//             forms.erase(forms.begin()+(forms.size() - i), forms.begin()+(forms.size() - i+1));
//         }
//         else if (auto prev = dynamic_cast<And*>(forms[forms.size() - i])){
//             if (prev->get_right() != nullptr && prev->get_left() != nullptr){
//                 obj->set_left(prev);
//             }
//             forms[forms.size() - j] = obj;
//             forms.erase(forms.begin()+(forms.size() - i), forms.begin()+(forms.size() - i+1));
//         }
//         else if (auto prev = dynamic_cast<Or*>(forms[forms.size() - i])){
//             if (prev->get_right() != nullptr && prev->get_left() != nullptr){
//                 obj->set_left(prev);
//             }
//             forms[forms.size() - j] = obj;
//             forms.erase(forms.begin()+(forms.size() - i), forms.begin()+(forms.size() - i+1));
//         }
//         else if (auto prev = dynamic_cast<Not*>(forms[forms.size() - i])){
//             obj->set_left(prev);
//             forms[forms.size() - j] = obj;
//             forms.erase(forms.begin()+(forms.size() - i), forms.begin()+(forms.size() - i+1));
//         }
//     }
//     j++;
// }

// Formula* tree_formula(std::vector<Formula*> forms, std::vector<int> scob){
//         for (auto r: forms){
//         r->print(std::cout);

//     }
//             std::cout<<"\n";
//             //std::cout<<std::endl;
//     int j = 1, i;
//     while (true){
//         if (auto variable = dynamic_cast<Variable*>(forms[forms.size() - j])){
//             i = j + 1;
//             if (auto prev = dynamic_cast<Implies*>(forms[forms.size() - i])){
//                 //std::cout<<"hello\n";
//                 if (prev->get_right() == nullptr){
//                     prev->set_right(variable);
//                 }
//                 forms[forms.size() - i] = prev;
//             }
//             else if (auto prev = dynamic_cast<And*>(forms[forms.size() - i])){
//                 if (prev->get_right() == nullptr){
//                     prev->set_right(variable);
//                 }
//                 forms[forms.size() - i] = prev;
//             }
//             else if (auto prev = dynamic_cast<Or*>(forms[forms.size() - i])){
//                 if (prev->get_right() == nullptr){
//                     prev->set_right(variable);
//                 }
//                 forms[forms.size() - i] = prev;
//             }
//             else if (auto prev = dynamic_cast<Not*>(forms[forms.size() - i])){
//                 prev->set_operand(variable);
//                 forms[forms.size() - i] = prev;
//             }
//             forms.erase(forms.begin()+(forms.size() - j), forms.begin()+(forms.size() - j+1));
//         }
//         else {
//             j++;
//         }
//         //i++;
//         if (j >= forms.size()) break;

//     }

//         for (auto r: forms){
//         r->print(std::cout);

//         }
//             std::cout<<"\n";
//     j = 1;
//     i = 0;
//     int index_scob = 1;
//     while (true){
//         std::cout<<j<<"\n";

//         if(auto implies = dynamic_cast<Implies*>(forms[forms.size() - j])){
           
//             insert_forms(forms, j, implies);
//         }
//         else if(auto andi = dynamic_cast<And*>(forms[forms.size() - j])){
           
//             insert_forms(forms, j, andi);
//         }
//         else if(auto ori = dynamic_cast<Or*>(forms[forms.size() - j])){
            
//             insert_forms(forms, j, ori);
//         }
//         else{
//             // auto not_i = dynamic_cast<Not*>(forms[forms.size()-j]);
//             // if (not_i->get_operand() != nullptr){

//             // }
//             j++;
//         }

//         if (j > forms.size()) break;

//         for (auto r: forms){
//         r->print(std::cout);
//         }
//         std::cout<<"\n";

//     }
    

//     return forms[0];