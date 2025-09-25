#include <iostream>

#include "AVL.hpp"

int main() {
    AVLTree tree;

    // Take input
    std::string commandAmount;
    std::getline(std::cin, commandAmount);
    int commands;
    try {
        commands = std::stoi(commandAmount);
    } catch (std::invalid_argument&) {
        std::cout << "unsuccessful" << std::endl;
        return 0;
    }

    for (int i = 0; i < commands; i++) {
        std::string line;
        std::getline(std::cin, line);
        std::string command = line.substr(0, line.find(' '));
        if (command == "insert") {
            std::string name =
                line.substr(line.find('"') + 1,
                            line.find_last_of('"') - line.find('"') - 1);
            std::string id = line.substr(line.find_last_of(' ') + 1);
            if (!tree.insert(name, id))
                std::cout << "unsuccessful" << std::endl;
            else
                std::cout << "successful" << std::endl;
        } else if (command == "remove") {
            int id;
            try {
                id = std::stoi(line.substr(line.find(' ') + 1));
            } catch (std::invalid_argument&) {
                std::cout << "unsuccessful" << std::endl;
                continue;
            }
            if (!tree.remove(id))
                std::cout << "unsuccessful" << std::endl;
            else
                std::cout << "successful" << std::endl;
        } else if (command == "search") {
            std::string query = line.substr(line.find(' ') + 1);
            std::string name;
            if (line.find('"') == std::string::npos)
                name = "";
            else
                name = line.substr(line.find('"') + 1,
                                   line.find_last_of('"') - line.find('"') - 1);

            if (name != "") {
                if (!tree.search(name))
                    std::cout << "unsuccessful" << std::endl;
            } else {
                int id;
                try {
                    id = std::stoi(query);
                } catch (std::invalid_argument&) {
                    std::cout << "unsuccessful" << std::endl;
                    continue;
                }
                if (!tree.search(id)) std::cout << "unsuccessful" << std::endl;
            }
        } else if (command == "printInorder") {
            tree.printInorder();
        } else if (command == "printPreorder") {
            tree.printPreorder();
        } else if (command == "printPostorder") {
            tree.printPostorder();
        } else if (command == "printLevelCount") {
            tree.printLevelCount();
        } else if (command == "removeInorder") {
            int n;
            try {
                n = std::stoi(line.substr(line.find(' ') + 1));
            } catch (std::invalid_argument&) {
                std::cout << "unsuccessful" << std::endl;
                continue;
            }
            if (!tree.removeInorder(n))
                std::cout << "unsuccessful" << std::endl;
            else
                std::cout << "successful" << std::endl;
        } else {
            std::cout << "unsuccessful" << std::endl;
        }
    }

    return 0;
}
