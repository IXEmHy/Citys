#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <cctype>
#include <algorithm>
#include <Windows.h>

#define ConsoleOutIn SetConsoleCP(1251); SetConsoleOutputCP(1251);


std::string blocked_symbols = "-ЫыЬьЪъЙй1234567890+*/!`=)(&~;:.,'\\\"][><?%$#№@";
size_t npos = std::string::npos;
std::string find_block(std::string strf) {
    while (strf.size() != 0) {
        if (blocked_symbols.find(strf.at(strf.size() - 1)) == npos)
            if (strf.erase(0, strf.size() - 1)[0] >= 'а' && strf.erase(0, strf.size() - 1)[0] <= 'я')
                return std::to_string(char(strf.erase(0, strf.size() - 1)[0] - 32));
            else return std::to_string(strf.erase(0, strf.size() - 1)[0]);
        else return find_block(strf.erase(strf.size() - 1));
    }
    return " неправильно введнная строка!\n";
}

std::string readFile(const std::string& fileName) {
    std::ifstream f(fileName);
    size_t size = f.seekg(0, std::ios::end).tellg();
    std::string s(size, ' ');
    f.seekg(0);
    f.read(&s[0], size);
    std::cout << s << std::endl;
    return s;
}

size_t count_s(std::ifstream& f, size_t count) {
    while (count) {
        std::string temp;
        f >> temp;
        count--;
    }
    size_t tellg = f.tellg();
    f.seekg(0);
    return tellg;
}

int main()
{
    using std::this_thread::sleep_for; using std::chrono::milliseconds;
    using std::fstream; using std::ofstream; using std::ifstream; using std::ios;
    using std::string;
    using std::cout; using std::endl; using std::cin;

    ConsoleOutIn
    srand(time(0));
    string scin, citys = readFile("all_citys.txt"), buffer, hidden_city, cities_already_used;

    ofstream out("file.txt");
    out << citys;
    out.close();

    ifstream fin("file.txt");
    fin.seekg(count_s(fin, rand() % (std::count(citys.begin(), citys.end(), ' ') + 1)));
    fin >> buffer;
    hidden_city = buffer;
    fin.close();

    out.open("file-было.txt", ios::app);
    cities_already_used = "БОТ : " + buffer +'\n';
    out << cities_already_used;
    out.close();
    out.open("file.txt");
    size_t index = citys.find(buffer);
    citys.erase(index ? index-1 : 0, buffer.size() + 1);
    out << citys;
    out.close();

    bool res = false;
    int attempts = 4;

    cout << "Привет! Эта игра \"Города\"\n"
        "Правила игры \"Города\"\n"
        "Тебе нужно (как и мне) после того как кто-то из нас назовет город\n"
        "другому нужно ответить назвав город, на последнию букву тогого города, который задал оппонент\n"
        "Информация:\n"
        "Исключенные буквы \"'Ь', 'Ъ', 'Ы' и 'Й'\"\n"
        "Для выхода достаточно прописать одну букву 'q', но тогда ты проиграешь!\n"
        "Кол-во городов: " << std::count(citys.begin(), citys.end(), ' ') + 2
        << "\nЯ начну! Первый город - " + buffer + ", тебе на букву \"" << char(find_block(buffer)[0]-32) << "\":\n";

    fin.open("file.txt");
    while (attempts != 0 && cin >> scin) {
        if (scin == "q") {
            cout << "\nУра! Я выиграл!\nВот список всех сыгранных городов :\n"
                + readFile("file-было.txt") + "Попыток осталось : " + char(attempts + '0') + "\nБот выиграл!"
                + "\nПриходи еще, буду рад поиграть снова!\n";
            cin >> scin;
            return 0;
        }

        if (!citys.size()) {
            cout << "Блин... я проиграл! Молодец!\n\nВот список всех сыгранных городов :\n" + readFile("file-было.txt")
                + "Попыток осталось : " + char(attempts + '0') + "\nТы выиграл!\n" + "Перезапусти игру, чтобы начать заново!";
            cin >> scin;
            return 0;
        }

        if (scin[0] != char(find_block(hidden_city)[0] - 32))  {
            cout << "Не-е-е-т! Не на ту букву!\n";
            continue;
        }

        while (fin >> buffer)
        {
            if (res = (buffer == scin)) {
                cout << "ДА! " << buffer << " есть в моем списке\n";
                fin.close();

                out.open("file-было.txt", ios::app);
                cities_already_used = "ТЫ  : " + buffer + " \n";
                out << cities_already_used;
                out.close();

                out.open("file.txt");
                index = citys.find(buffer);
                out << citys.erase(index != 0 ? index - 1 : 0, buffer.size() + 1);
                out.close();

                break;
            }
        }
        fin.close();
        fin.open("file.txt");

        if (res) {
            string back = find_block(scin);
            cout << "так . . . мне на \"" << char(back[0] - 32) << "\"\n";
            sleep_for(milliseconds(600));
            size_t find = citys.find(back[0] >= 'а' && back[0] <= 'я' ? char(back[0] - 32) : back[0]);
            if (find != npos)
                while (find != npos)
                {
                    if (citys[find != 0 ? find - 1 : 0] != '-') {
                        fin.seekg(find);
                        fin >> hidden_city;
                        fin.close();

                        out.open("file-было.txt", ios::app);
                        cities_already_used = "БОТ : " + hidden_city + " \n";
                        out.close();

                        cout << "Хм . . . " + hidden_city + "!\n" + "Тебе на букву \"" << char(find_block(hidden_city)[0] - 32) << "\": ";

                        out.open("file.txt");
                        out << citys.erase(find != 0 ? find - 1 : 0, hidden_city.size() + 1);
                        out.close();

                        break;
                    }
                    else
                        find = citys.find(find, char(back[0] - 32));
                }
            else {
                cout << "Блин... я проиграл! Молодец!\n\nВот список всех сыгранных городов :\n" + cities_already_used 
                    + "Попыток осталось : " + char(attempts + '0') + "\nТы выиграл!\n" + "Перезапусти игру, чтобы начать заново!";
                cin >> scin;
                return 0;
            }
        }
        else {
            if(--attempts != 0)
                cout << (cities_already_used.find(scin + ' ') != npos ?
                    "Этот город уже был использован!\nПопыток : " + std::to_string(attempts) + '\n'
                    : "Такого города нет в моем списке!\nПопыток : " + std::to_string(attempts) + '\n');
        }
    }
    return 0;
}