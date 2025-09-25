#pragma once
#include <string>
#include <vector>

#include "Student.hpp"

class Node {
   public:
    Student val;
    int height;
    Node* left;
    Node* right;
    Node(Student value)
        : val(value), height(1), left(nullptr), right(nullptr) {}
};

class AVLTree {
   private:
    Node* root;

    // Rotation helper functions - named after the imbalance they fix, not
    // necessarily the rotation itself. This is probably confusing, but I am
    // deeming it too late to fix.
    Node* rotateRightRight(Node*& node);
    Node* rotateLeftLeft(Node*& node);
    Node* rotateRightLeft(Node*& node);
    Node* rotateLeftRight(Node*& node);

    // Recursive helper functions called by public functions
    Node* insert(Node* node, Student value);
    Node* remove(Node* node, int id);
    void getInorder(Node* node, std::vector<Student>& inorder);
    void getPreorder(Node* node, std::vector<Student>& preorder);
    void getPostorder(Node* node, std::vector<Student>& postorder);

    // Utility implementation of search function for use in insert and remove
    bool inTree(int id);

    int balanceFactor(Node* node);

   public:
    AVLTree() : root(nullptr) {}
    AVLTree(Student value) { root = new Node(value); }
    // Rule of 3 (didn't implement move constructor/assignment)
    ~AVLTree();
    AVLTree(const AVLTree& other);
    AVLTree& operator=(const AVLTree& other);

    // Required functions
    // Returns true if successful, false otherwise
    bool insert(std::string name, std::string id);
    bool remove(int id);
    bool search(int id);
    bool search(std::string name);
    std::vector<Student> getInorder();
    std::vector<Student> getPreorder();
    std::vector<Student> getPostorder();
    void printVector(const std::vector<Student>& vec);
    void printInorder();
    void printPreorder();
    void printPostorder();
    int getLevelCount();
    void printLevelCount();
    bool removeInorder(int n);
};
