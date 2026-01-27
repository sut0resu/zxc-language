#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <regex>
#include <cctype>
#include <algorithm>

std::map<std::string, int> int_vars;
std::map<std::string, std::string> str_vars;
std::map<std::string, bool> bool_vars;

enum class Types {
    Number,
    Operation,
    Identifier
};

struct Token {
    Types type;

    std::string name;
    std::string oper;

    int num;
};

struct Node {
    Types type;

    std::string oper;
    std::string name;

    int value;

    Node* right;
    Node* left;
};

bool is_var(std::string var, std::string type) {
    //std::cout << "is_var: " << var << "пробел" << "\n";

    if (type == "int") {
        for (auto i : int_vars) {
            //std::cout << "цикл: " << i.first << "\n";

            if (var == i.first) return true;
        }
    }
    if (type == "string") {
        for (auto i : str_vars)
            if (var == i.first) return true;
    }
    if (type == "bool") {
        for (auto i : bool_vars)
            if (var == i.first) return true;
    }

    return false;
}

bool is_forbidden_sign(char c) {
    std::string signs = "!@#$%^&*()_+-={}[]:;\"'<>,.?/\\|`~";
    for (auto& i : signs) {
        if (c == i) return true;
    }

    return false;
}

bool is_identifier(std::string ident) {
    //std::cout << "is_identifier " << ident[0] << "\n";
    
    if ((ident[0] == '_' || isalpha(ident[0]) || !is_forbidden_sign(ident[0])) && !isdigit(ident[0])) {
        for (auto& i : ident) {
            if (!is_forbidden_sign(i)) return true;
        }

        return true;
    }
    else {
        return false;
    }
}

std::string get_def_var(std::string var, std::string type) {
    if (type == "int") {
        for (auto i : int_vars)
            if (var == i.first) return std::to_string(i.second);
    }
    if (type == "string") {
        for (auto i : str_vars)
            if (var == i.first) return i.second;
    }
    if (type == "bool") {
        for (auto i : bool_vars)
            if (var == i.first) {
                if (i.second == 0) return "false";
                if (i.second == 1) return "true";
            }
    }

    return "none";
}

bool is_valid_name(const std::string & name_var) {
    std::string specials = "!@#$%^&*()_+-={}[]:;\"'<>,.?/\\|`~";
    for (char i : specials) {
        if (name_var.find(i) != std::string::npos || name_var == "zxc") {
            return false;
        }
    }

    return true;
}

size_t pos_operation(std::string str_defenition) {
    std::string operations = "=+-*/";

    for (auto& i : operations) {
        if (str_defenition.find(i) != std::string::npos) return str_defenition.find(i);
    }

    return 0;
}

bool is_oper(std::string str_defenition) {
    std::string operations = "+-*/()";

    for (auto& i : operations) {
        if (str_defenition.find(i) != std::string::npos) return true;
    }

    return false;
}

bool is_oper(char char_def) {
    std::string operations = "+-*/()";

    for (auto& i : operations) {
        if (char_def == i) return true;
    }

    return false;
}

bool is_number(std::string str_defenition) {
    int count = 0;

    for (auto& i : str_defenition) {
        if (!isdigit(i)) return false;
    }

    return true;
}

std::vector<char> parse_string_to_vector_char(std::string str) {
    std::vector<char> str_into_char;

    for (auto& i : str) {
        str_into_char.push_back(i);
    }

    return str_into_char;
}

std::string which_func(std::vector<char> str) {
    std::string func;

    for (auto& i : str) {
        func += i;
        if (func == "zxc") return "zxc";
        if (func == "print(") return "print";
        if (func == "int ") return "int";
        if (func == "string ") return "string";
        if (func == "bool ") return "bool";
        if (func == "if(" || func == "if (") return "if";
    }

    return "none";
}

bool is_end(std::vector<char> str) {
    if (which_func(str) == "print") {
        return str.back() - 1 == ')' && str.back() == ';';
    } 
    else if (which_func(str) == "int" || which_func(str) == "bool" || which_func(str) == "string") {
        return str.back() == ';';
    }
    else {
        return false;
    }
}

int get_index_first(std::vector<char> str) {
    int count = 0;
    for (auto& i : str) {
        count++;
        if (which_func(str) == "print") {
            return 6;
        } 
        else if (which_func(str) == "int" || which_func(str) == "bool" || which_func(str) == "string") {
            if (i == '=') {
                if (str[count] == ' ') {
                    return count + 1;
                }
                else {
                    return count;
                } 
            }
        }
        else if (which_func(str) == "if") {
            if (i == 'f') {
                if (str[count] == '(') {
                    return count + 1;
                }
                else {
                    return count;
                }
            }
        }
    }
}

int get_index_last(std::vector<char> str) {
    int count = 0;
    for (auto& i : str) {
        count++;
        if (which_func(str) == "print") {
            if (i == ')' && str[count] == ';') {
                return count - 1;
            }
        } 
        else if (which_func(str) == "int" || which_func(str) == "bool" || which_func(str) == "string") {
            if (i == ';') {
                return count - 1;
            }
        }
    }
}

int get_index_first_name_var(std::vector<char> str) {
    if (which_func(str) == "int") {
        return 4;
    }
    else if (which_func(str) == "bool") {
        return 5;
    }
    else if (which_func(str) == "string") {
        return 7;
    }
}

std::string get_var_name(std::vector<char> str) {
    std::string var_name = "";

    for (int i = get_index_first_name_var(str); i < str.size(); i++) {
        if (str[i] == '=') break;
        var_name += str[i];
    }

    if (var_name.back() == ' ') var_name.pop_back();

    return var_name;
}

std::string get_def(std::vector<char> str) {
    std::string def = "";

    for (int i = get_index_first(str); i < get_index_last(str); i++) {
        if (str[get_index_first(str)] == '"' && str[get_index_last(str) - 1] == '"')
            std::erase(str, '"');
        if(str[i] != ' ')
            def += str[i];
    }

    return def;
}

std::string get_def_without_spaces(std::vector<char> str) {
    std::string def = "";

    for (int i = get_index_first(str); i < get_index_last(str); i++) {
        if (str[get_index_first(str)] == '"' && str[get_index_last(str) - 1] == '"')
            std::erase(str, '"');

        def += str[i];
    }

    return def;
}

bool if_has_quotes(std::vector<char> str) {
    return (str[get_index_first(str)] == '"' && str[get_index_last(str) - 1] == '"')
        || (str[get_index_first(str)] == '"' && str[get_index_last(str)] == '"');
}

inline bool check_semicolon(std::vector<char> str) {
    return str.back() == ';';
}

std::vector<std::string> semi_tokenization(std::vector<char> str) {
    std::string value = "";

    std::vector<std::string> tokens;

    for (int i = 0; i < str.size(); i++) {
        if (is_oper(str[i])) {
            //std::cout << value << "\n";
            if (str[i] == '-' && (i == 0 || (is_oper(str[i - 1]) && str[i - 1] != ')') && i + 1 < str.size()
                && isdigit(str[i + 1] && !isdigit(str[i - 1])))) {
                value += str[i];
            }

            if (!value.empty()) {
                tokens.push_back(value);
                value.clear();
            }
            if(value.empty()) {
                value += str[i];
                tokens.push_back(value);
                value.clear();
            }
        }
        else {
            value += str[i];
        }
   
    //(str[i] == '-' && (i == 0 || (is_oper(str[i - 1]) && str[i - 1] != ')') && i + 1 < str.size()
        //    && isdigit(str[i + 1] && !isdigit(str[i - 1])))) {
    }
    
    if (!value.empty())
    tokens.push_back(value);

    std::erase(tokens, " ");

    return tokens;
}

int to_int(std::string num) {
    int i = 0;
    int sign = 1;
    if (num[0] == '-') {
        sign = -1;
        i++;
    }

    int value = 0;
    for (; i < num.size(); i++) {
        value = value * 10 + (num[i] - '0');
    }

    return value * sign;
}

std::vector<Token*> tokenization(std::vector<char> str) {
    std::vector<Token*> t;

    for (auto i : semi_tokenization(str)) {  
        Token* T = new Token;

        if (is_identifier(i)) {
            //std::cout << "tokenz " << i << "\n";
            T->type = Types::Identifier;
            T->name = i;
        } 
        else if (isdigit(i[0])) {

            if (is_number(i)) {

                T->type = Types::Number;
                T->num = to_int(i);

            }
        }
        else if(is_oper(i)) {
            T->type = Types::Operation;
            T->oper = i;
        }
        else {
            std::cout << "ошибка" << "\n";
            break;
        }

        t.push_back(T);

    }

    return t;
}

bool validate_tokens(std::vector<Token*> tokens) {
    if (tokens.empty()) return false;

    int balance = 0;

    for (int i = 0; i < tokens.size(); i++) {
        Token* cur = tokens[i];
        Token* prev = (i > 0) ? tokens[i - 1] : nullptr;

        if (cur->type == Types::Number) {
            if (prev) {
                if (prev->type == Types::Number) return false;
                if (prev->type == Types::Operation && prev->oper == ")") return false;
            }
        }

        if (cur->type == Types::Operation) {
            std::string op = cur->oper;

            if (op == "(") {
                balance++;
                if (prev && prev->type == Types::Number) return false;
                if (prev && prev->oper == ")") return false;
            }

            if (op == ")") {
                balance--;
                if (balance < 0) return false;
                if (!prev) return false;
                if (prev->type == Types::Operation && prev->oper != ")") return false;
            }
            if (op == "+" || op == "-" || op == "*" || op == "/") {
                if (!prev) return false;
                if (prev->type == Types::Operation && prev->oper != ")") return false;
            }
        }
    }

    Token* last = tokens.back();
    if (last->type == Types::Operation && last->oper != ")") return false;

    return balance == 0;
}

int priority(std::string op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;

    return 0;
}

double eval(Node* node) {
    if (!node) return 0;

    if (node->type == Types::Number) {
        //std::cout << "eval operation " << node->value;
        return node->value;
    }

    if (node->type == Types::Identifier) {
        if (!is_var(node->name, "int")) {
            std::cout << "Не найдена переменная " << node->name << "\n";
            return 0;
        }

        //std::cout << int_vars[node->name] << "\n";
        return int_vars[node->name];
    }

    double right = eval(node->right);
    double left = eval(node->left);

    if (node->oper == "+") return left + right;
    if (node->oper == "-") return left - right;
    if (node->oper == "/") return left / right;
    if (node->oper == "*") return left * right;

    return 0;
}

Node* create_node(std::vector<Node*>& operands, std::vector<std::string>& operations) {
    if (operands.size() < 2 || operations.empty()) {
        return nullptr;
    }

    Node* node_oper = new Node;

    node_oper->type = Types::Operation;

    node_oper->right = operands.back();
    //std::cout << static_cast<int>(operands.back()->type) << " right create_node" << "\n";
    operands.pop_back();

    node_oper->left = operands.back();
    //std::cout << static_cast<int>(operands.back()->type) << " left create_node" << "\n";
    operands.pop_back();

    node_oper->oper = operations.back();

    operations.pop_back();


    return node_oper;
}

Node* AST(std::vector<Token*> tokens) {
    std::vector<Node*> operands;
    std::vector<std::string> operations;

    int count_tokens = 0;

    for (; count_tokens < tokens.size(); count_tokens++) {
        if (tokens[count_tokens]->type == Types::Identifier) {
            //std::cout << "AST " << "\n";

            Node* node = new Node;
            node->type = Types::Identifier;
            node->name = tokens[count_tokens]->name;

            operands.push_back(node);

            //std::cout << "Operands: " << operands.back()->name << "\n";
        }
        if (tokens[count_tokens]->type == Types::Number) {
            Node* node = new Node;
            node->type = Types::Number;
            node->value = tokens[count_tokens]->num;

            operands.push_back(node);

            //std::cout << "Operands: " << operands.back()->value << "\n";
        } 
        else if (tokens[count_tokens]->type == Types::Operation) {
            //std::cout << "Operations: " << tokens[count_tokens]->oper << "\n";
            if (tokens[count_tokens]->oper != "(" && tokens[count_tokens]->oper != ")") {
                while (!operations.empty() && operations.back() != "(" && priority(operations.back()) >= priority(tokens[count_tokens]->oper)) {

                    operands.push_back(create_node(operands, operations));            
                }

                operations.push_back(tokens[count_tokens]->oper);
            }
            else if (tokens[count_tokens]->oper == ")") {
                while (operations.back() != "(") {
                    if (!operations.empty() && operands.size() >= 2) {
                        operands.push_back(create_node(operands, operations));
                        
                    }
                }

                operations.pop_back();
            }
            else if (tokens[count_tokens]->oper == "(") {
                operations.push_back("(");
            }
        }
    }


    while (!operations.empty()) {
        operands.push_back(create_node(operands, operations));               
    }


    return operands.back();
}

int main()
{ 
    setlocale(LC_ALL, "ru");

    std::ifstream in("main.zxc");

    if (!in.is_open()) {
        std::cout << "Не удалось открыть файл";
    }

    std::string text;

    //variables
    

    int count_zxc = 0;
    int count_str = 1;

    std::string str_zxc; 
    std::getline(in, text);

    
    if (text == "zxc") {
        while (std::getline(in, text)) {
            count_str++;

            std::vector<char> str = parse_string_to_vector_char(text);

            if (which_func(str) != "zxc") {
                count_zxc++;

                if (!check_semicolon(str)) {
                    std::cout << "Не хватает ; или после ; есть символы на " << count_str << " строке";
                    break;
                }

                if (which_func(str) == "print") {
                    if (text.find("(") == std::string::npos) {
                        std::cout << "Не найдено '('";
                        break;
                    }
                    if (text.find(')') == std::string::npos) {
                        std::cout << "Не найдено ')' на " << count_str << " строке";
                        break;
                    }

                    std::string defenition = get_def_without_spaces(str);
                    
                    std::vector<Token*> tokens = tokenization(parse_string_to_vector_char(get_def_without_spaces(str)));

                    int vars = 0;

                    if (1) {
                        // print int_var
                        for (auto& i : int_vars) {
                            for (auto& j : tokens) {
                                if (j->type == Types::Identifier) {
                                    if (j->name == i.first) {
                                        std::cout << i.second;
                                    }
                                }
                            }
                        }
                        // print string_var
                        for (auto& i : str_vars) {
                            if (!i.first.empty()) {
                                if (i.first == defenition) {
                                    vars++;
                                    std::cout << i.second;
                                }
                            }
                        }
                        // print bool_var
                        for (auto& i : bool_vars) {
                            if (!i.first.empty()) {
                                if (i.first == defenition) {
                                    vars++;
                                    std::cout << i.second;
                                }
                            }
                        }
                    }

                    if (!if_has_quotes(str)) {
                        if (validate_tokens(tokenization(parse_string_to_vector_char(defenition)))) {
                            if (is_oper(defenition)) {
                                defenition = get_def(str);
                                std::cout << eval(AST(tokenization(parse_string_to_vector_char(defenition))));
                            }
                            else {
                                std::cout << defenition;
                            }
                        }
                        else {
                            if (defenition == "true")
                                std::cout << 1;
                            else if (defenition == "false")
                                std::cout << 0;
                            else
                                std::cout << "неправильное название переменной на " << count_str << " строке";
                        }
                    }
                    else if (if_has_quotes(str)) {
                        if (text[get_index_first(str)] == '"' && text[get_index_last(str) - 1] == '"') {
                            defenition = get_def_without_spaces(str);
                            std::cout << defenition;
                        }
                        else {
                            std::cout << "Требуется закрыть кавычки на " << count_str << " строке";
                        }
                    }

                }

                /*
                    VAR TYPES
                */

                // INT
                if (which_func(str) == "int") {

                    if (text.find("=") == std::string::npos) {
                        std::cout << "Не найдено '='";
                        break;
                    }

                    std::string name_var = get_var_name(str);

                    for (auto& i : int_vars) {
                        if (i.first == name_var) {
                            std::cout << "такая переменная уже существует. " << count_str << " строка";
                        }
                    }

                    if (!is_valid_name(name_var)) {
                        std::cout << "Не правильное имя переменной на " << count_str << " строке";
                        break;
                    }

                    if (text.find('"') != std::string::npos) {
                        std::cout << "Не правильное значение переменной на " << count_str << " строке";
                        break;
                    }

                    std::string str_defenition = get_def(str);


                    if ((str_defenition.find("+") != std::string::npos || str_defenition.find("-") != std::string::npos ||
                        str_defenition.find("*") != std::string::npos || str_defenition.find("/") != std::string::npos)) {
                        int_vars[name_var] = eval(AST(tokenization(parse_string_to_vector_char(str_defenition))));

                    }
                    else {
                        int_vars[name_var] = to_int(str_defenition);
                    }

                }
                // STRING
                if (which_func(str) == "string") {
                    if (text.find("=") == std::string::npos) {
                        std::cout << "Не найдено '=' на " << count_str << " строке";
                        break;
                    } if (text.find('"') == std::string::npos) {
                        std::cout << "Не найдено двойных кавычек на " << count_str << " строке";
                        break;
                    } if (text.find('"', text.find('"') + 1) == std::string::npos) {
                        std::cout << "Не найдено двойных кавычек на " << count_str << " строке";
                        break;
                    }

                    std::string name_var = get_var_name(str);

                    if (!is_valid_name(name_var)) {
                        std::cout << "Не правильное имя переменной на " << count_str << " строке";
                        break;
                    }

                    auto pos_def_start = text.find('"') + 1;
                    auto pos_def_end = text.find('"', pos_def_start);


                    std::string defenition = get_def(str);

                    str_vars[name_var] = defenition;

                }
                // BOOL
                if (which_func(str) == "bool") {
                    auto pos = text.find("bool ");
                    auto pos_to = text.find("=");

                    if (text.find("=") == std::string::npos) {
                        std::cout << "Не найдено '=' на " << count_str << " строке";
                        break;
                    }
                    if (text.find('"') != std::string::npos) {
                        std::cout << "Не правильное значение переменной на " << count_str << " строке";
                        break;
                    }

                    std::string name_var = get_var_name(str);
                    if (!is_valid_name(name_var)) {
                        std::cout << "Не правильное имя переменной на " << count_str << " строке";
                        break;
                    }

                    if (get_def(str).find('"') == std::string::npos) {
                        if (get_def(str) == "false" || get_def(str) == "0") {
                            bool_vars[name_var] = 0;
                        }
                        else if (get_def(str) == "true" || get_def(str) == "1") {
                            bool_vars[name_var] = 1;
                        }
                        else {
                            std::cout << "не правильное значение переменной " << name_var << " на " << count_str << " строке";
                        }
                    }
                    else {
                        std::cout << "не правильное значение переменной " << name_var << " на " << count_str << " строке";
                        break;
                    }
                }
            }
            else {
                break;
            }
        }
    }
}
