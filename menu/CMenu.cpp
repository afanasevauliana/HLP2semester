#include "./CMenu.h"
#include <limits>

CMenu::CMenu(string title, CMenuItem *items, size_t count) : title(title), items(items), count(count) {}

int CMenu::getSelect() const {
    return select;
}

bool CMenu::isRun() const {
    return running;
}

size_t CMenu::getCount() const {
    return count;
}

string CMenu::getTitle() {
    return title;
}

CMenuItem *CMenu::getItems() {
    return items;
}

void CMenu::print() {
    for (size_t i{}; i < count; ++i) {
        std::cout << i + 1 << ". ";
        items[i].print();
        std::cout << std::endl;
    }
    std::cout << "0. Exit" << std::endl;
}

int CMenu::runCommand() {
    print();
    std::cout << "\n   Select >> ";
    if (!(std::cin >> select)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::invalid_argument("Некорректный выбор пункта меню");
    }
    std::cout << "Выбран пункт: " << select << "\n";
    if (select > 0 && static_cast<size_t>(select) <= count) {
        int result = items[select - 1].run();
        std::cout << "Функция вернула: " << result << "\n";
        return result;
    } else {
        return 0;
    }
}