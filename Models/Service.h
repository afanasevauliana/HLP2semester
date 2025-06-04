#pragma once
#include <string>
#include <iostream>

class Service {
private:
    std::string name;
    double price;
    std::string description;

public:
    Service(const std::string& name = "", double price = 0.0, const std::string& description = "");
    std::string getName() const;
    double getPrice() const;
    std::string getDescription() const;

    friend std::ostream& operator<<(std::ostream& os, const Service& service);
    friend std::istream& operator>>(std::istream& is, Service& service);
    bool operator==(const Service& other) const;
    bool operator<(const Service& other) const;
    bool operator>(const Service& other) const;
};