#pragma once
  #include "User.h"

  class Employee : public User {
  private:
      std::string position;

  public:
      Employee(const std::string& firstName, const std::string& lastName, int age, 
               const std::string& login, const std::string& password, const std::string& position);
      void displayInfo() const override;
      void authenticate() const override;
      bool operator<(const User& other) const override;
      bool operator>(const User& other) const override;
      int getAge() const { return age; }

      friend std::ostream& operator<<(std::ostream& os, const Employee& employee);
  };