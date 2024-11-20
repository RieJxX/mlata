#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

//определение классов
//Базовый класс
class Formula{
    public:
        virtual ~Formula() = default;
        virtual void print(std::ostream& os) const=0;
        virtual bool equals(const Formula& other) const = 0;
        virtual Formula* clone() const = 0;
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

        bool equals(const Formula& other) const override {
            const Variable* var = dynamic_cast<const Variable*>(&other);
            return var && name == var->name;
        }

        Formula* clone() const override{
            return new Variable(name);
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

        bool equals(const Formula& other) const override {
            const And* derived = dynamic_cast<const And*>(&other);
            return derived && ((left && derived->left && left->equals(*derived->left)) || (!left && !derived->left))
                && ((right && derived->right && right->equals(*derived->right)) || (!right && !derived->right));
        }

        And* clone() const override{
            return new And(left ? left->clone() : nullptr, right ? right->clone() : nullptr);
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
        bool equals(const Formula& other) const override {
            const Or* derived = dynamic_cast<const Or*>(&other);
            return derived && ((left && derived->left && left->equals(*derived->left)) || (!left && !derived->left))
                && ((right && derived->right && right->equals(*derived->right)) || (!right && !derived->right));
        }

        Or* clone() const override{
            return new Or(left ? left->clone() : nullptr, right ? right->clone() : nullptr);
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

        bool equals(const Formula& other) const override {
            const Not* derived = dynamic_cast<const Not*>(&other);
            return derived && ((operand && derived->operand && operand->equals(*derived->operand)) || (!operand && !derived->operand));
        }

        Not* clone() const override{
            return new Not(operand? operand->clone() : nullptr);
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

        bool equals(const Formula& other) const override {
            const Implies* derived = dynamic_cast<const Implies*>(&other);
            return derived && ((left && derived->left && left->equals(*derived->left)) || (!left && !derived->left))
                && ((right && derived->right && right->equals(*derived->right)) || (!right && !derived->right));
        }

        Implies* clone() const override{
            return new Implies(left ? left->clone() : nullptr, right ? right->clone() : nullptr);
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

void take_params(Formula* formula, std::vector<Formula*>& params){
    if (auto imp = dynamic_cast<Implies*>(formula)){
        params.push_back(imp->get_left());
        params.push_back(imp->get_right());
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
    params.erase(std::unique(params.begin(), params.end(), [](const Formula* lhs, const Formula* rhs) {
        return lhs->equals(*rhs);
    }), params.end());
}

std::vector<Formula*> new_aks1(Formula* aksiome_1, std::vector<Formula*> param_f){
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
            if (f1 == f2) continue;
            else{
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
    }
    return new_aks;
}

std::vector<Formula*> new_aks2(Formula* aksiome_2, std::vector<Formula*> param_f){
    std::vector<Formula*> new_aks;
    for (auto f1: param_f){
        for (auto f2: param_f){
            for (auto f3: param_f){
                if (f1 == f2 || f1 == f3 || f2 == f3) continue;
                else{
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
    }
    return new_aks;
}


int main() {

    std::ofstream outFile("result.txt");

    auto aksiome1 = aksiome_1();
    auto aksiome2 = aksiome_2();
    auto aksiome3 = aksiome_3();

    auto form = tree_arch("input.txt");
    std::vector<Formula*> param;
    take_params(form, param);
    //std::set<Formula*> new_param(param.begin(), param.end());

    for (auto i: param){
        i->print(std::cout);
        std::cout<<std::endl;
    }

    std::vector<Formula*> new_aks = new_aks1(aksiome1, param);

    for (auto i: new_aks){
        i->print(std::cout);
        std::cout<<std::endl;
    }

    
    //aksiome1->print(std::cout);
    return 0;
}
