#include <catch2/catch_all.hpp>
#include <iomanip>
#include <random>
#include <string>

#define private public

#include "../src/AVL.hpp"

using namespace std;

TEST_CASE("Incorrect insertions", "[required]") {
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

TEST_CASE("Test empty tree", "[required]") {
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

    SECTION("Check if search returns false") {
        REQUIRE(tree.search("Non-existent Student") == false);
    };

    SECTION("Check if remove returns false") {
        REQUIRE(tree.remove(12345678) == false);
    };
}

TEST_CASE("Correct rotations", "[required]") {
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

TEST_CASE("Deletion cases", "[required]") {
    AVLTree tree;

    SECTION("Delete leaf node") {
        tree.insert("A", "20000000");
        tree.insert("B", "10000000");
        tree.insert("C", "30000000");
        REQUIRE(tree.remove(10000000) == true);
        vector<Student> result = tree.getInorder();
        REQUIRE(result.size() == 2);
        REQUIRE(result[0].id == "20000000");
        REQUIRE(result[1].id == "30000000");
    };

    SECTION("Delete node with one child") {
        tree.insert("A", "20000000");
        tree.insert("B", "10000000");
        tree.insert("C", "30000000");
        tree.insert("D", "25000000");
        REQUIRE(tree.remove(30000000) == true);
        vector<Student> result = tree.getInorder();
        REQUIRE(result.size() == 3);
        REQUIRE(result[0].id == "10000000");
        REQUIRE(result[1].id == "20000000");
        REQUIRE(result[2].id == "25000000");
    };

    SECTION("Delete node with two children") {
        tree.insert("A", "20000000");
        tree.insert("B", "10000000");
        tree.insert("C", "30000000");
        tree.insert("D", "25000000");
        tree.insert("E", "35000000");
        REQUIRE(tree.remove(20000000) == true);
        vector<Student> result = tree.getInorder();
        REQUIRE(result.size() == 4);
        REQUIRE(result[0].id == "10000000");
        REQUIRE(result[1].id == "25000000");
        REQUIRE(result[2].id == "30000000");
        REQUIRE(result[3].id == "35000000");
    };
}

TEST_CASE("Large insert and delete", "[required]") {
    AVLTree inputTree;
    std::vector<Student> expectedOutput, actualOutput;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 99999999);

    for (int i = 0; i < 100; i++) {
        // Generate a random 8-digit number as a string
        int randomNumber = dist(gen);
        std::stringstream ss;
        ss << std::setw(8) << std::setfill('0') << randomNumber;
        std::string randomInput = ss.str();

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

    // Delete
    for (int i = 0; i < 10; i++) {
        dist = std::uniform_int_distribution<int>(0, expectedOutput.size() - 1);
        int randomInput = dist(gen);
        inputTree.remove(std::stoi(expectedOutput[randomInput].id));
        expectedOutput.erase(expectedOutput.begin() + randomInput);
    }

    // Check deletions
    actualOutput = inputTree.getInorder();
    REQUIRE(expectedOutput.size() == actualOutput.size());
    std::sort(expectedOutput.begin(), expectedOutput.end());
    REQUIRE(expectedOutput == actualOutput);
}

TEST_CASE("Test traversals", "[optional]") {
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

TEST_CASE("Simple insertions", "[optional]") {
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

TEST_CASE("Duplicate ID", "[optional]") {
    AVLTree tree;

    REQUIRE(tree.insert("Test Student", "12345678") == true);
    REQUIRE(tree.insert("Another Student", "12345678") == false);
    REQUIRE(tree.root != nullptr);
    REQUIRE(tree.root->val.name == "Test Student");
    REQUIRE(tree.root->val.id == "12345678");
}

TEST_CASE("Remove existing ID", "[optional]") {
    AVLTree tree;

    REQUIRE(tree.insert("Test Student", "12345678") == true);
    REQUIRE(tree.remove(12345678) == true);
    REQUIRE(tree.root == nullptr);
}

TEST_CASE("Remove non-existent ID", "[optional]") {
    AVLTree tree;

    REQUIRE(tree.insert("Test Student", "12345678") == true);
    REQUIRE(tree.remove(87654321) == false);
    REQUIRE(tree.root != nullptr);
    REQUIRE(tree.root->val.name == "Test Student");
    REQUIRE(tree.root->val.id == "12345678");
}

TEST_CASE("Remove from empty tree", "[optional]") {
    AVLTree tree;

    REQUIRE(tree.remove(12345678) == false);
    REQUIRE(tree.root == nullptr);
}

TEST_CASE("Search by name", "[optional]") {
    AVLTree tree;

    REQUIRE(tree.insert("Test Student", "12345678") == true);
    REQUIRE(tree.insert("Test Student", "87654321") == true);
    REQUIRE(tree.search("Test Student") == true);
    REQUIRE(tree.search("Non-existent Student") == false);
}

TEST_CASE("Get level count", "[optional]") {
    AVLTree tree;

    REQUIRE(tree.getLevelCount() == 0);

    tree.insert("A", "10000000");
    REQUIRE(tree.getLevelCount() == 1);

    tree.insert("B", "20000000");
    REQUIRE(tree.getLevelCount() == 2);

    tree.insert("C", "30000000");
    REQUIRE(tree.getLevelCount() == 2);  // After balancing

    tree.insert("D", "05000000");
    REQUIRE(tree.getLevelCount() == 3);
}

TEST_CASE("removeInorder function", "[optional]") {
    AVLTree tree;

    tree.insert("A", "10000000");
    tree.insert("B", "20000000");
    tree.insert("C", "30000000");
    tree.insert("D", "40000000");
    tree.insert("E", "50000000");

    // Current inorder: A, B, C, D, E

    REQUIRE(tree.removeInorder(2) == true);  // Remove C
    vector<Student> result = tree.getInorder();
    REQUIRE(result.size() == 4);
    REQUIRE(result[0].id == "10000000");
    REQUIRE(result[1].id == "20000000");
    REQUIRE(result[2].id == "40000000");
    REQUIRE(result[3].id == "50000000");

    REQUIRE(tree.removeInorder(0) == true);  // Remove A
    result = tree.getInorder();
    REQUIRE(result.size() == 3);
    REQUIRE(result[0].id == "20000000");
    REQUIRE(result[1].id == "40000000");
    REQUIRE(result[2].id == "50000000");

    REQUIRE(tree.removeInorder(5) == false);  // Invalid position
}

TEST_CASE("Correct level count after multiple operations", "[optional]") {
    AVLTree tree;

    tree.insert("A", "10000000");
    tree.insert("B", "20000000");
    tree.insert("C", "30000000");
    REQUIRE(tree.getLevelCount() == 2);  // After balancing

    tree.remove(20000000);
    REQUIRE(tree.getLevelCount() == 2);

    tree.insert("D", "05000000");
    REQUIRE(tree.getLevelCount() == 2);

    tree.insert("E", "02500000");
    REQUIRE(tree.getLevelCount() == 3);

    tree.remove(10000000);
    REQUIRE(tree.getLevelCount() == 2);
}
