#include <iostream>
#include <memory>
#include "Grammar.h"

int InputNum()
{
    string s;
    while (true)
    {
        cin >> s;
        if (s.find_first_not_of("123") != string::npos )
        {
            cout << "Некорректное значение!\nПовторите ввод: ";
        }
        else
            return atoi(s.c_str());
    }
}



bool checkStr(const std::string& str) {
    std::string simbols = "abc$";
    for( auto e: str){
        if(simbols.find(e) == string::npos){
            return false;
        }
    }
    return true;

}



int main() {
    unique_ptr<Grammar> generator = make_unique<Grammar>();
    int act = 0;
    cout << "Грамматика:\n"
            "S'-> S$    \n"
            "S -> AaS \n"
            "S -> b        \n"
            "A -> CAb \n"
            "A -> B\n"
            "B -> cSA\t\n"
            "B -> empty \n"
            "C -> c\n"
            "C -> ab" << endl<<endl<<endl;
    bool flagProgramm=1;


    while (flagProgramm) {
        cout << "Выберите действие:\n1: Проверить строку\n2: Сгенерировать новую строку \n3: Завершить программу\n\n ";
        act = InputNum();
        switch (act) {
            case 1: {
                string inputStr;
                cout << "Введите слово:" << endl;

                // Очистка буфера ввода перед использованием getline
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                // Использование getline для считывания строки с пробелами
                std::getline(cin, inputStr);

                // Проверка на пустой ввод и замена на один пробел
                if (inputStr.empty()) {
                    cout << "Строка не соотвествует грамматике." << endl << endl;
                    break;
                }

                if (checkStr(inputStr)) {
                    generator->GlobalCheck(inputStr);
                } else {
                    cout << "Символы не из алфавита. Попробуйте снова" << endl << endl;
                }
                break;
            }

            case 2: {
                cout<<generator->generateRandom();
                cout << endl;
                break;
            }

            case 3:{
                flagProgramm = false;
                break;
            }

            default: {
                break;
            }
        }
    }

    return 0;
}
