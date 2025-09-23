#pragma once

#include <string>

struct Student {
    std::string name;
    std::string id;

    Student() : name(""), id("") {}
    Student(std::string name, std::string);

    bool operator==(const Student& other) const { return id == other.id; }
    bool operator<(const Student& other) const { return id < other.id; }
    bool operator>(const Student& other) const { return id > other.id; }
};
