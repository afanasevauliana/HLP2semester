#include <windows.h>
#include "menu/CMenu.h"
#include "menu/CMenuItem.h"
#include "Models/User.h"
#include "Models/Employee.h"
#include "Models/Client.h"
#include "Models/Service.h"
#include "Models/MyList.h"
#include <iostream>
#include <cmath>
#include <locale>
#include <codecvt>
#include <fstream>
#include <stdexcept>
#include <limits>
using namespace std;

MyList<Service> services;
MyList<Employee> employees;
MyList<Client> clients;

#pragma region функции-заглушки
int f1() {
    int value = 25;
    cout << "The root of 25 is " << sqrt(value) << endl;
    return 1;
}

int f2() {
    cout << "\n";
    cout << "   *   \n";
    cout << "  ***  \n";
    cout << " ***** \n";
    cout << "*******\n";
    cout << "*     *\n";
    cout << "*     *\n";
    cout << "*     *\n";
    cout << "*******\n\n";
    return 2;
} 

int displayEmployeeInfo() {
    try {
        Employee* emp = new Employee("Artem", "Kapralov", 30, "Artem_Kapralov", "123456789", "Photographer");
        emp->displayInfo();
        cout << endl;
        delete emp;
        return 3;
    } catch (const std::bad_alloc& e) {
        cout << "Ошибка памяти при создании сотрудника: " << e.what() << endl;
        return 3;
    }
}

int displayClientInfo() {
    try {
        Service service("Photoshoot", 50.0, "Photoshoot in the style of the nineteenth century");
        Client* client = new Client("Anastasia", "Frolova", 25, "Anastasia_Frolova", "0345244444", service);
        client->displayInfo();
        cout << endl;
        delete client;
        return 4;
    } catch (const std::bad_alloc& e) {
        cout << "Ошибка памяти при создании клиента: " << e.what() << endl;
        return 4;
    }
}

int addService() {
    try {
        Service newService;
        cin >> newService;
        if (newService.getPrice() < 0) throw std::invalid_argument("Цена не может быть отрицательной");
        services.add(new ItemList<Service>(newService));
        cout << "Услуга добавлена:\n" << newService << endl;
        return 5;
    } catch (const std::invalid_argument& e) {
        cout << "Ошибка ввода: " << e.what() << endl;
        return 5;
    } catch (const std::bad_alloc& e) {
        cout << "Ошибка памяти: " << e.what() << endl;
        return 5;
    }
}

int listServices() {
    cout << "Список услуг:\n";
    cout << services;
    cout << "listServices() завершен\n";
    return 6;
}

int sortServicesByPrice() {
    try {
        cout << "Начинаем сортировку...\n";
        services.sort();
        cout << "Сортировка завершена, проверяем список...\n";
        listServices();
        cout << "Вывод списка успешно выполнен, возвращаем управление.\n";
        return 7;
    } catch (const std::bad_alloc& e) {
        cout << "Ошибка памяти при сортировке: " << e.what() << endl;
        return 7;
    } catch (...) {
        cout << "Неизвестная ошибка при сортировке.\n";
        return 7;
    }
}

int addEmployee() {
    try {
        string firstName, lastName, login, password, position;
        int age;
        
        cout << "Введите имя: ";
        cin >> firstName;
        cout << "Введите фамилию: ";
        cin >> lastName;
        cout << "Введите возраст: ";
        cin >> age;
        if (age <= 0) throw std::invalid_argument("Возраст должен быть положительным");
        cout << "Введите логин: ";
        cin >> login;
        cout << "Введите пароль: ";
        cin >> password;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "Введите должность: ";
        getline(cin, position);
        
        Employee* emp = new Employee(firstName, lastName, age, login, password, position);
        employees.add(new ItemList<Employee>(*emp));
        cout << "Сотрудник добавлен:\n" << *emp << endl;
        return 8;
    } catch (const std::invalid_argument& e) {
        cout << "Ошибка ввода: " << e.what() << endl;
        return 8;
    } catch (const std::bad_alloc& e) {
        cout << "Ошибка памяти: " << e.what() << endl;
        return 8;
    }
}

int listEmployees() {
    cout << "Список сотрудников:\n";
    cout << employees;
    return 9;
}

int addClient() {
    try {
        if (services.isEmpty()) {
            cout << "Сначала добавьте услуги!\n";
            return 10;
        }
        
        string firstName, lastName, login, password;
        int age, serviceChoice;
        
        cout << "Введите имя: ";
        cin >> firstName;
        cout << "Введите фамилию: ";
        cin >> lastName;
        cout << "Введите возраст: ";
        cin >> age;
        if (age <= 0) throw std::invalid_argument("Возраст должен быть положительным");
        cout << "Введите логин: ";
        cin >> login;
        cout << "Введите пароль: ";
        cin >> password;
        
        cout << "Выберите услугу:\n";
        size_t idx = 1;
        for (auto it = services.begin(); it != services.end(); ++it, ++idx) {
            cout << idx << ". " << (*it).get().getName() << " (" << (*it).get().getPrice() << ")\n";
        }
        cin >> serviceChoice;
        if (serviceChoice <= 0 || static_cast<size_t>(serviceChoice) > services.len()) {
            throw std::out_of_range("Неверный выбор услуги");
        }
        
        idx = 1;
        ItemList<Service>* selectedService = nullptr;
        for (auto it = services.begin(); it != services.end(); ++it, ++idx) {
            if (idx == static_cast<size_t>(serviceChoice)) {
                selectedService = &(*it);
                break;
            }
        }
        if (selectedService) {
            Client* newClient = new Client(firstName, lastName, age, login, password, selectedService->get());
            clients.add(new ItemList<Client>(*newClient));
            cout << "Клиент добавлен:\n";
            for (auto it = clients.begin(); it != clients.end(); ++it) {
                cout << (*it).get() << endl;
            }
        }
        return 11;
    } catch (const std::invalid_argument& e) {
        cout << "Ошибка ввода: " << e.what() << endl;
        return 11;
    } catch (const std::out_of_range& e) {
        cout << "Ошибка выбора: " << e.what() << endl;
        return 11;
    } catch (const std::bad_alloc& e) {
        cout << "Ошибка памяти: " << e.what() << endl;
        return 11;
    }
}

int listClients() {
    cout << "Список клиентов:\n";
    cout << clients;
    return 12;
}

int compareServices() {
    if (services.len() < 2) {
        cout << "Добавьте хотя бы 2 услуги для сравнения!\n";
        return 13;
    }
    
    auto it = services.begin();
    Service first = (*it).get();
    ++it;
    Service second = (*it).get();
    
    cout << "Сравнение услуг:\n";
    cout << "1. " << first.getName() << "\n2. " << second.getName() << "\n";
    
    if (first == second) {
        cout << "Услуги идентичны\n";
    } else if (first < second) {
        cout << first.getName() << " дешевле чем " << second.getName() << "\n";
    } else {
        cout << second.getName() << " дешевле чем " << first.getName() << "\n";
    }
    cout << endl;
    return 14;
}

int filterServicesByPrice() {
    double minPrice;
    cout << "Введите минимальную цену для фильтрации: ";
    cin >> minPrice;
    if (cin.fail()) throw std::invalid_argument("Неверный формат цены");
    MyList<Service> filtered;
    for (auto it = services.begin(); it != services.end(); ++it) {
        if ((*it).get().getPrice() >= minPrice) {
            filtered.add(new ItemList<Service>((*it).get()));
        }
    }
    cout << "Отфильтрованные услуги:\n" << filtered;
    return 15;
}

int filterEmployeesByAge() {
    int minAge;
    cout << "Введите минимальный возраст для фильтрации: ";
    cin >> minAge;
    if (cin.fail()) throw std::invalid_argument("Неверный формат возраста");
    MyList<Employee> filtered;
    for (auto it = employees.begin(); it != employees.end(); ++it) {
        if ((*it).get().getAge() >= minAge) {
            filtered.add(new ItemList<Employee>((*it).get()));
        }
    }
    cout << "Отфильтрованные сотрудники:\n" << filtered;
    return 16;
}

int saveServicesToFile() {
    ofstream outFile("services.txt");
    if (!outFile) {
        cout << "Ошибка при открытии файла для записи!\n";
        return 17;
    }
    for (auto it = services.begin(); it != services.end(); ++it) {
        outFile << (*it).get().getName() << "\n" << (*it).get().getPrice() << "\n" << (*it).get().getDescription() << "\n";
    }
    outFile.close();
    cout << "Услуги сохранены в файл services.txt\n";
    return 17;
}

int loadServicesFromFile() {
    ifstream inFile("services.txt");
    if (!inFile) {
        cout << "Ошибка при открытии файла для чтения!\n";
        return 18;
    }
    services.erase();
    string name, description;
    double price;
    while (getline(inFile, name)) {
        inFile >> price;
        if (inFile.fail()) throw std::invalid_argument("Неверный формат цены в файле");
        inFile.ignore();
        getline(inFile, description);
        Service newService(name, price, description);
        services.add(new ItemList<Service>(newService));
    }
    inFile.close();
    cout << "Услуги загружены из файла services.txt\n";
    listServices();
    return 18;
}

#pragma endregion

const int ITEMS_NUMBER = 18;

int main() {
    #ifdef _WIN32
    system("chcp 65001");
    #endif
    setlocale(LC_ALL, "ru_RU.UTF-8");
    try {
        services.add(new ItemList<Service>(Service("Фотосессия", 6000.0, "Стандартная фотосессия в студии")));
        services.add(new ItemList<Service>(Service("Портрет", 7000.0, "Профессиональный портрет")));
        cout << "Список услуг после инициализации:\n";
        cout << services;
        Employee* emp = new Employee("Иван", "Петров", 30, "ivan", "123", "Фотограф");
        employees.add(new ItemList<Employee>(*emp));
        auto it = services.begin();
        Client* client = new Client("Анна", "Смирнова", 25, "anna", "456", (*it).get());
        clients.add(new ItemList<Client>(*client));

        CMenuItem items[ITEMS_NUMBER] {
            CMenuItem{"Root of 25", f1}, 
            CMenuItem{"Print house", f2},
            CMenuItem{"Display Employee Info", displayEmployeeInfo},
            CMenuItem{"Display Client Info", displayClientInfo},
            CMenuItem{"Добавить услугу", addService},
            CMenuItem{"Список услуг", listServices},
            CMenuItem{"Сортировать услуги по цене", sortServicesByPrice},
            CMenuItem{"Добавить сотрудника", addEmployee},
            CMenuItem{"Список сотрудников", listEmployees},
            CMenuItem{"Добавить клиента", addClient},
            CMenuItem{"Список клиентов", listClients},
            CMenuItem{"Сравнить услуги", compareServices},
            CMenuItem{"Тест оператора вывода", []() { 
                auto it = services.begin();
                cout << (*it).get() << endl; 
                return 15; 
            }},
            CMenuItem{"Тест оператора сравнения", []() { 
                auto it = services.begin();
                Service first = (*it).get();
                ++it;
                cout << "Услуги " << (first == (*it).get() ? "равны" : "разные") << endl; 
                return 16; 
            }},
            CMenuItem{"Фильтровать услуги по цене", filterServicesByPrice},
            CMenuItem{"Фильтровать сотрудников по возрасту", filterEmployeesByAge},
            CMenuItem{"Сохранить услуги в файл", saveServicesToFile},
            CMenuItem{"Загрузить услуги из файла", loadServicesFromFile}
        };
        CMenu menu("Фотостудия - Управление", items, ITEMS_NUMBER);
        while (menu.runCommand()) {
            cout << "Цикл меню продолжается\n";
        };
        cout << "Программа завершена. Нажмите Enter для выхода...\n";
        cin.get();
    } catch (const std::exception& e) {
        cout << "Общая ошибка: " << e.what() << endl;
        cout << "Нажмите Enter для выхода...\n";
        cin.get();
    }

    return 0;
}