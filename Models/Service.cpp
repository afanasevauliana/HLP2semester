#include "Service.h"
#include <limits>

Service::Service(const std::string& name, double price, const std::string& description)
    : name(name), price(price), description(description) {}

std::string Service::getName() const { return name; }
double Service::getPrice() const { return price; }
std::string Service::getDescription() const { return description; }

std::ostream& operator<<(std::ostream& os, const Service& service) {
    os << "Услуга: " << service.name << "\nЦена: " << service.price  << " рублей" << "\nОписание: " << service.description;
    return os;
}

std::istream& operator>>(std::istream& is, Service& service) {
    bool firstAttempt = true;
    std::cout << "Название услуги: ";
    do {
        if (!std::getline(is, service.name)) {
            is.clear();
            continue;
        }
        
        service.name.erase(0, service.name.find_first_not_of(" \t"));
        service.name.erase(service.name.find_last_not_of(" \t") + 1);
        
        if (service.name.empty()) {
            if (!firstAttempt) {
                std::cout << "Название не может быть пустым. Введите снова.\n";
            }
        } else {
            break;
        }
        firstAttempt = false;
    } while (true);

    while (true) {
        std::cout << "Цена услуги (в рублях): ";
        if (is >> service.price) {
            if (service.price >= 0) {
                break;
            }
            std::cout << "Цена не может быть отрицательной. Введите снова.\n";
        } else {
            std::cout << "Неверный формат цены. Введите число.\n";
            is.clear();
            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    is.ignore();

    while (true) {
        std::cout << "Описание услуги: ";
        if (!std::getline(is, service.description)) {
            is.clear();
            continue;
        }
        
        service.description.erase(0, service.description.find_first_not_of(" \t"));
        service.description.erase(service.description.find_last_not_of(" \t") + 1);
        
        if (!service.description.empty()) {
            break;
        }
        std::cout << "Описание не может быть пустым. Введите снова.\n";
    }

    return is;
}

bool Service::operator==(const Service& other) const {
    return name == other.name && price == other.price;
}

bool Service::operator<(const Service& other) const {
    return price < other.price;
}

bool Service::operator>(const Service& other) const {
    return price > other.price;
}