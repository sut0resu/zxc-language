#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <regex>

bool is_valid_name(const std::string& name_var) {
    std::string specials = "!@#$%^&*()_+-={}[]:;\"'<>,.?/\\|`~";
    for (char i : specials) {
        if (name_var.find(i) != std::string::npos) {
            return false;
        }
    }

    return true;
}

std::string maths(std::string str_defenition, int defenition) {
    size_t plus_pos = str_defenition.find('+');
    size_t minus_pos = str_defenition.find('-');
    size_t multiply_pos = str_defenition.find('*');
    size_t divide_pos = str_defenition.find('/');

    if (str_defenition.find('(') != std::string::npos || str_defenition.find(')') != std::string::npos) {
        str_defenition.erase(str_defenition.find('('), 1);
        str_defenition.erase(str_defenition.find(')'), 1);
    }

    /*
       УМНОЖЕНИЕ
   */

link_multiply:
    while (multiply_pos != std::string::npos) {
        multiply_pos = str_defenition.find('*');
        if (multiply_pos == std::string::npos) break;

        if (divide_pos != std::string::npos) {
            divide_pos = str_defenition.find('/');
            if (multiply_pos > divide_pos) goto link_divide;
        }

        std::regex pattern_for_first_num(R"((\d+)\*)");
        std::regex pattern_for_second_num(R"(\*(\d+)(?=[+\-*/]|$))");
        std::regex pattern_for_replace(R"(([A-Za-z0-9]+)\*([A-Za-z0-9]+))");
        std::smatch match;

        int first_num;
        int second_num;

        bool is_first_ok = false;
        bool is_second_ok = false;

        if (std::regex_search(str_defenition, match, pattern_for_first_num)) {
            first_num = std::stoi(match[1]);
            is_first_ok = true;
        }

        if (std::regex_search(str_defenition, match, pattern_for_second_num)) {
            second_num = std::stoi(match[1]);
            is_second_ok = true;
        }

        if (is_first_ok && is_second_ok) {
            defenition = first_num * second_num;
        }
        else {
            std::cout << "Ошибка инициализации";
            std::abort();
        }

        std::string result = std::regex_replace(str_defenition, pattern_for_replace, std::to_string(defenition), std::regex_constants::format_first_only);

        str_defenition = result;

    }

    /*
       ДЕЛЕНИЕ
   */

link_divide:
    while (divide_pos != std::string::npos) {
        divide_pos = str_defenition.find('/');
        if (divide_pos == std::string::npos) break;

        if (multiply_pos != std::string::npos) {
            multiply_pos = str_defenition.find('*');
            if (divide_pos > multiply_pos) goto link_multiply;
        }

        std::regex pattern_for_first_num(R"((\d+)\/)");
        std::regex pattern_for_second_num(R"(\/(\d+)(?=[+\-*/]|$))");
        std::regex pattern_for_replace(R"(([A-Za-z0-9]+)\/([A-Za-z0-9]+))");
        std::smatch match;

        int first_num;
        int second_num;

        bool is_first_ok = false;
        bool is_second_ok = false;

        if (std::regex_search(str_defenition, match, pattern_for_first_num)) {
            first_num = std::stoi(match[1]);
            is_first_ok = true;
        }

        if (std::regex_search(str_defenition, match, pattern_for_second_num)) {
            second_num = std::stoi(match[1]);
            is_second_ok = true;
        }

        if (is_first_ok && is_second_ok) {
            defenition = first_num / second_num;
        }
        else {
            std::cout << "Ошибка инициализации";
            std::abort();
        }

        std::string result = std::regex_replace(str_defenition, pattern_for_replace, std::to_string(defenition), std::regex_constants::format_first_only);

        str_defenition = result;
    }

    /*
       ПЛЮС
   */

link_plus:
    while (plus_pos != std::string::npos) {
        plus_pos = str_defenition.find('+');
        if (plus_pos == std::string::npos) break;

        if (minus_pos != std::string::npos) {
            minus_pos = str_defenition.find('-');
            if (plus_pos > minus_pos) goto link_minus;
        }

        std::regex pattern_for_first_num(R"((\d+)\+)");
        std::regex pattern_for_second_num(R"(\+(\d+)(?=[+\-*/]|$))");
        std::regex pattern_for_replace(R"(([A-Za-z0-9]+)\+([A-Za-z0-9]+))");
        std::smatch match;

        int first_num;
        int second_num;

        bool is_first_ok = false;
        bool is_second_ok = false;

        if (std::regex_search(str_defenition, match, pattern_for_first_num)) {
            first_num = std::stoi(match[1]);
            is_first_ok = true;
        }

        if (std::regex_search(str_defenition, match, pattern_for_second_num)) {
            second_num = std::stoi(match[1]);
            is_second_ok = true;
        }

        if (is_first_ok && is_second_ok) {
            defenition = first_num + second_num;
        }
        else {
            std::cout << "Ошибка инициализации";
            std::abort();
        }

        std::string result = std::regex_replace(str_defenition, pattern_for_replace, std::to_string(defenition), std::regex_constants::format_first_only);

        str_defenition = result;
    }

    /*
        МИНУС
    */

link_minus:
    while (minus_pos != std::string::npos) {
        minus_pos = str_defenition.find('-');
        if (minus_pos == std::string::npos) break;

        if (plus_pos != std::string::npos) {
            plus_pos = str_defenition.find('+');
            if (minus_pos > plus_pos) goto link_plus;
        }

        std::regex pattern_for_first_num(R"((\d+)\-)");
        std::regex pattern_for_second_num(R"(\-(\d+)(?=[+\-*/]|$))");
        std::regex pattern_for_replace(R"(([A-Za-z0-9]+)\-([A-Za-z0-9]+))");
        std::smatch match;

        int first_num;
        int second_num;

        bool is_first_ok = false;
        bool is_second_ok = false;

        if (std::regex_search(str_defenition, match, pattern_for_first_num)) {
            first_num = std::stoi(match[1]);
            is_first_ok = true;
        }

        if (std::regex_search(str_defenition, match, pattern_for_second_num)) {
            second_num = std::stoi(match[1]);
            is_second_ok = true;
        }

        if (str_defenition[0] == '-') break;

        if (is_first_ok && is_second_ok) {
            defenition = first_num - second_num;
        }
        else {
            std::cout << "Ошибка инициализации";
            std::abort();
        }

        std::string result = std::regex_replace(str_defenition, pattern_for_replace, std::to_string(defenition), std::regex_constants::format_first_only);

        str_defenition = result;


    }

    return str_defenition;
}

size_t pos_operation(std::string str_defenition) {
    std::string operations = "=+-*/";

    for (auto& i : operations) {
        if (str_defenition.find(i) != std::string::npos) return str_defenition.find(i);
    }

    return 0;
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
    std::map<std::string, int> int_vars;
    std::map<std::string, std::string> str_vars;
    std::map<std::string, bool> bool_vars;


    while (std::getline(in, text)) {
        if (text.find("print(") != std::string::npos) {
            auto pos = text.find("(") + 1;
            if (text.find("(") == std::string::npos) {
                std::cout << "Не найдено '('";
                break;
            }
            if (text.find(')') == std::string::npos) {
                std::cout << "Не найдено ')'";
                break;
            }

            auto pos_to = text.find(')');

            auto pos_def_start = text.find('"') + 1;
            auto pos_def_end = text.find('"', pos_def_start);

            std::string defenition = text.substr(pos, pos_to - pos);


            int vars = 0;

            // print int_var
            for (auto& i : int_vars) {
                if (!i.first.empty()) {
                    if (i.first == defenition) {
                        vars++;
                        std::cout << i.second;
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

            if (vars == 0) {
                if (text.find('"') + 1 != std::string::npos && text.find('"', text.find('"') + 1) != std::string::npos) {
                    defenition = text.substr(pos_def_start, pos_def_end - pos_def_start);
                    std::cout << defenition;
                }
                else {
                    std::cout << "Переменная не найдена";
                    break;
                }
            }

        }

        /*
            VAR TYPES
        */

        // INT
        if (text.find("int ") != std::string::npos) { 
            auto pos = text.find("int ");
            auto pos_to = text.find("=");

            if (text.find("=") == std::string::npos) {
                std::cout << "Не найдено '='";
                break;
            }
            
            std::string name_var = text.substr(pos + 4, pos_to - (pos + 5));

            if (!is_valid_name(name_var)) {
                std::cout << "Не правильное имя переменной";
                break;
            }

            if (text.find('"') != std::string::npos) {
                std::cout << "Не правильное значение переменной";
                break;
            }       
                                 

            std::string str_defenition = text.substr(pos_to + 1);

            std::regex pattern(R"(([A-Za-z_][A-Za-z0-9_]*))");
            std::smatch match;
            std::string::const_iterator search_start(str_defenition.cbegin());

            while (std::regex_search(search_start, str_defenition.cend(), match, pattern)) {
                std::string var_name = match[1];

                if (int_vars.find(var_name) != int_vars.end()) {
                    auto pos_var = match.position(1) + std::distance(str_defenition.cbegin(), search_start);

                    if (pos_var != std::string::npos) {
                        str_defenition.replace(pos_var, var_name.size(), std::to_string(int_vars[var_name]));
                        search_start = str_defenition.cbegin() + pos_var + std::to_string(int_vars[var_name]).size();
                    }
                }
                else {
                    search_start = match.suffix().first;
                }
            }

            if (str_defenition[0] == ' ') {
                str_defenition.erase(0, 1);
            }


            int defenition = 0;

            while(str_defenition.find("+") != std::string::npos || str_defenition.find("-") != std::string::npos || 
                str_defenition.find("*") != std::string::npos || str_defenition.find("/") != std::string::npos) {
                
                int index = -1;
                for (auto& i : str_defenition) {
                    index++;
                    if (i == ' ') {
                        str_defenition.erase(index, 1);
                    }
                }

                if (str_defenition.find('(') != std::string::npos || str_defenition.find(')') != std::string::npos) {
                    if (str_defenition.find('(') == std::string::npos || str_defenition.find(')') == std::string::npos) {
                        std::cout << "Не найдено ( или )";
                        std::abort();
                    }
                    else {
                        auto pos_first = str_defenition.find('(');
                        auto pos_second = str_defenition.find(')');

                        //std::cout << str_defenition.substr(pos_first + 1, pos_second - (pos_first + 1));

                        str_defenition.replace(pos_first, pos_second - pos_first + 1, maths(str_defenition.substr(pos_first + 1, pos_second - pos_first - 1), defenition));
                    }
                }

                str_defenition = maths(str_defenition, defenition);
            }

            try {
                int a = std::stoi(str_defenition);
            }
            catch (const std::exception&) {
                std::cout << "Не правильно значение переменной";
                break;
            }

            defenition = std::stoi(str_defenition);

            int_vars[name_var] = defenition;
        }
        // STRING
        if (text.find("string ") != std::string::npos) { 
            auto pos = text.find("string ");


            if (text.find("=") == std::string::npos) {
                std::cout << "Не найдено '='";
                break;
            } if (text.find('"') == std::string::npos) {
                std::cout << "Не найдено двойных кавычек ";
                break;
            } if (text.find('"', text.find('"') + 1) == std::string::npos) {
                std::cout << "Не найдено двойных кавычек ";
                break;
            }
            auto pos_to = text.find("=");

            std::string name_var = text.substr(pos + 7, pos_to - (pos + 8));

            if (!is_valid_name(name_var)) {
                std::cout << "Не правильное имя переменной";
                break;
            }
            
            auto pos_def_start = text.find('"') + 1;
            auto pos_def_end = text.find('"', pos_def_start);

            
            std::string defenition = text.substr(pos_def_start, pos_def_end - pos_def_start);

            str_vars[name_var] = defenition;

        }
        // BOOL
        if (text.find("bool ") != std::string::npos) { 
            auto pos = text.find("bool ");
            auto pos_to = text.find("=");

            if (text.find("=") == std::string::npos) {
                std::cout << "Не найдено '='";
                break;
            }
            if (text.find('"') != std::string::npos) {
                std::cout << "Не правильное значение переменной";
                break;
            }

            std::string name_var = text.substr(pos + 5, pos_to - (pos + 6));
            if (!is_valid_name(name_var)) {
                std::cout << "Не правильное имя переменной";
                break;
            }

            
            if (isdigit(text.substr(pos_to + 1)[0]) || isdigit(text.substr(pos_to + 1)[1])) {
                int defenition = std::stoi(text.substr(pos_to + 1));
                bool_vars[name_var] = defenition;
            }
            else if (text.substr(pos_to + 2) == "true" || text.substr(pos_to + 1) == "false") {
                if (text.substr(pos_to + 1) == "true") {
                    int defenition = true;
                    bool_vars[name_var] = defenition;
                }
                if (text.substr(pos_to + 1) == "false") {
                    int defenition = false;
                    bool_vars[name_var] = defenition;
                }
            }
            else {
                std::cout << "не правильное значение переменной " << name_var;
                break;
            }
        }

        
    }
}
