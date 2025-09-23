#include <catch2/catch_all.hpp>
#include <iomanip>
#include <iostream>

#define private public

#include "../src/AVL.hpp"

using namespace std;

TEST_CASE("Test traversals", "[tree behavior]") {
    AVLTree tree;
    tree.insert("Alice", "00000002");
    tree.insert("Bob", "00000001");
    tree.insert("Charlie", "00000003");

    SECTION("Inorder traversal") {
        vector<Student> result = tree.getInorder();
        REQUIRE(result.size() == 3);
        REQUIRE(result[0].name == "Bob");
        REQUIRE(result[1].name == "Alice");
        REQUIRE(result[2].name == "Charlie");
    };

    SECTION("Preorder traversal") {
        vector<Student> result = tree.getPreorder();
        REQUIRE(result.size() == 3);
        REQUIRE(result[0].name == "Alice");
        REQUIRE(result[1].name == "Bob");
        REQUIRE(result[2].name == "Charlie");
    };

    SECTION("Postorder traversal") {
        vector<Student> result = tree.getPostorder();
        REQUIRE(result.size() == 3);
        REQUIRE(result[0].name == "Bob");
        REQUIRE(result[1].name == "Charlie");
        REQUIRE(result[2].name == "Alice");
    };
}

TEST_CASE("Test empty tree", "[tree behavior]") {
    AVLTree tree;

    SECTION("Check if the root is null") { REQUIRE(tree.root == nullptr); };

    SECTION("Check if traversals return empty vectors") {
        vector<Student> result = tree.getInorder();
        REQUIRE(result.empty());

        result = tree.getPreorder();
        REQUIRE(result.empty());

        result = tree.getPostorder();
        REQUIRE(result.empty());
    };
}

TEST_CASE("Catch incorrect insertions", "[input validation]") {
    AVLTree tree;

    SECTION("Insert a student with a negative ID") {
        REQUIRE(tree.insert("Test Student", "-0000000") == false);
    };

    SECTION("Insert a student with an ID bigger than 8 characters") {
        REQUIRE(tree.insert("Test Student", "123456789") == false);
    };

    SECTION("Insert a student with an ID smaller than 8 characters") {
        REQUIRE(tree.insert("Test Student", "1234567") == false);
    };

    SECTION("Insert a student with an invalid name") {
        REQUIRE(tree.insert("A11y", "12345678") == false);
    };

    SECTION("Insert a student with an empty name") {
        REQUIRE(tree.insert("", "12345678") == false);
    };
}

TEST_CASE("Simple insertions", "[tree behavior]") {
    AVLTree tree;

    REQUIRE(tree.insert("Test Student", "12345678") == true);
    REQUIRE(tree.root != nullptr);
    REQUIRE(tree.root->val.name == "Test Student");
    REQUIRE(tree.root->val.id == "12345678");

    REQUIRE(tree.insert("Second Student", "87654321") == true);
    REQUIRE(tree.root->right != nullptr);
    REQUIRE(tree.root->right->val.name == "Second Student");
    REQUIRE(tree.root->right->val.id == "87654321");

    REQUIRE(tree.insert("Third Student", "00000001") == true);
    REQUIRE(tree.root->left != nullptr);
    REQUIRE(tree.root->left->val.name == "Third Student");
    REQUIRE(tree.root->left->val.id == "00000001");
}

TEST_CASE("Correct rotations", "[tree behavior]") {
    AVLTree tree;

    SECTION("Right-Right case") {
        tree.insert("A", "10000000");
        tree.insert("B", "20000000");
        tree.insert("C", "30000000");
        REQUIRE(tree.root->val.id == "20000000");
        REQUIRE(tree.root->left->val.id == "10000000");
        REQUIRE(tree.root->right->val.id == "30000000");
    };

    SECTION("Left-Left case") {
        tree.insert("Test Student", "30000000");
        tree.insert("Test Student", "20000000");
        tree.insert("Test Student", "10000000");
        REQUIRE(tree.root->val.id == "20000000");
        REQUIRE(tree.root->left->val.id == "10000000");
        REQUIRE(tree.root->right->val.id == "30000000");
    };

    SECTION("Left-Right case") {
        tree.insert("Test Student", "30000000");
        tree.insert("Test Student", "10000000");
        tree.insert("Test Student", "20000000");
        REQUIRE(tree.root->val.id == "20000000");
        REQUIRE(tree.root->left->val.id == "10000000");
        REQUIRE(tree.root->right->val.id == "30000000");
    };

    SECTION("Right-Left case") {
        tree.insert("Test Student", "10000000");
        tree.insert("Test Student", "30000000");
        tree.insert("Test Student", "20000000");
        REQUIRE(tree.root->val.id == "20000000");
        REQUIRE(tree.root->left->val.id == "10000000");
        REQUIRE(tree.root->right->val.id == "30000000");
    };
}

TEST_CASE("Large insert and delete", "[tree behavior]") {
    AVLTree inputTree;
    std::vector<Student> expectedOutput, actualOutput;

    for (int i = 0; i < 100; i++) {
        // Generate a random 8-digit number as a string
        int randomNumber = rand() % 100000000;
        std::stringstream ss;
        ss << std::setw(8) << std::setfill('0') << randomNumber;
        std::string randomInput = std::to_string(randomNumber);

        if (std::count(expectedOutput.begin(), expectedOutput.end(),
                       Student("Test Student", randomInput)) == 0) {
            expectedOutput.push_back(Student("Test Student", randomInput));
            inputTree.insert("Test Student", randomInput);
        }
    }

    // Check insertions
    actualOutput = inputTree.getInorder();
    REQUIRE(expectedOutput.size() == actualOutput.size());
    std::sort(expectedOutput.begin(), expectedOutput.end());
    REQUIRE(expectedOutput == actualOutput);

    for (int i = 0; i < 10; i++) {
        int randomInput = rand() % 100;
        inputTree.remove(std::stoi(expectedOutput[randomInput].id));
        expectedOutput.erase(expectedOutput.begin() + randomInput);
    }

    // Check deletions
    actualOutput = inputTree.getInorder();
    REQUIRE(expectedOutput.size() == actualOutput.size());
    std::sort(expectedOutput.begin(), expectedOutput.end());
    REQUIRE(expectedOutput == actualOutput);
}

// TODO You must write 5 unique, meaningful tests for credit on the testing
// portion of this project!
