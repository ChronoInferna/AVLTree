#include "Student.hpp"

#include <regex>
#include <stdexcept>

Student::Student(std::string name, std::string id) {
    // Validate name
    if (name.empty()) {
        throw std::invalid_argument("Name cannot be empty");
    }
    std::regex name_regex("^[a-zA-Z\\s]+$");
    if (!std::regex_match(name, name_regex)) {
        throw std::invalid_argument(
            "Name must contain only alphabetic characters and spaces");
    }
    this->name = name;

    // Validate id
    if (id.length() != 8) {
        throw std::invalid_argument("ID must be 8 characters long");
    }
    std::regex id_regex("^[0-9]{8}$");
    if (!std::regex_match(id, id_regex)) {
        throw std::invalid_argument("ID must be numeric");
    }
    this->id = id;
}
