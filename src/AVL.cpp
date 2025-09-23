#include "AVL.hpp"

#include <iostream>
#include <stack>
#include <stdexcept>

AVLTree::~AVLTree() {
    if (!root) {
        return;
    }

    std::stack<Node*> nodeStack;
    nodeStack.push(root);

    while (!nodeStack.empty()) {
        Node* currentNode = nodeStack.top();
        nodeStack.pop();

        if (currentNode->left) nodeStack.push(currentNode->left);
        if (currentNode->right) nodeStack.push(currentNode->right);
        delete currentNode;
    }
}
AVLTree::AVLTree(const AVLTree& other) {
    if (!other.root) {
        root = nullptr;
        return;
    }

    std::stack<Node*> otherStack;
    otherStack.push(other.root);

    std::stack<Node*> thisStack;
    root = new Node(other.root->val);
    thisStack.push(root);

    Node* currentThis = root;
    Node* currentOther = other.root;

    while (!otherStack.empty()) {
        currentOther = otherStack.top();
        otherStack.pop();
        currentThis = thisStack.top();
        thisStack.pop();

        if (currentOther->right) {
            currentThis->right = new Node(currentOther->right->val);
            otherStack.push(currentOther->right);
            thisStack.push(currentThis->right);
        }
        if (currentOther->left) {
            currentThis->left = new Node(currentOther->left->val);
            otherStack.push(currentOther->left);
            thisStack.push(currentThis->left);
        }
    }
}
AVLTree& AVLTree::operator=(const AVLTree& other) {
    if (this == &other) {
        return *this;
    }

    // Clean up current tree
    this->~AVLTree();

    if (!other.root) {
        root = nullptr;
        return *this;
    }

    std::stack<Node*> otherStack;
    otherStack.push(other.root);

    std::stack<Node*> thisStack;
    root = new Node(other.root->val);
    thisStack.push(root);

    Node* currentThis = root;
    Node* currentOther = other.root;

    while (!otherStack.empty()) {
        currentOther = otherStack.top();
        otherStack.pop();
        currentThis = thisStack.top();
        thisStack.pop();

        if (currentOther->right) {
            currentThis->right = new Node(currentOther->right->val);
            otherStack.push(currentOther->right);
            thisStack.push(currentThis->right);
        }
        if (currentOther->left) {
            currentThis->left = new Node(currentOther->left->val);
            otherStack.push(currentOther->left);
            thisStack.push(currentThis->left);
        }
    }

    return *this;
}

Node* AVLTree::rotateRightRight(Node*& node) {
    Node* child = node->right;
    Node* grandchild = child->left;
    child->left = node;
    node->right = grandchild;

    node->height = 1 + std::max(node->left ? node->left->height : 0,
                                node->right ? node->right->height : 0);
    child->height = 1 + std::max(child->left ? child->left->height : 0,
                                 child->right ? child->right->height : 0);

    return child;
}
Node* AVLTree::rotateLeftLeft(Node*& node) {
    Node* child = node->left;
    Node* grandchild = child->right;
    child->right = node;
    node->left = grandchild;
    return child;

    node->height = 1 + std::max(node->left ? node->left->height : 0,
                                node->right ? node->right->height : 0);
    child->height = 1 + std::max(child->left ? child->left->height : 0,
                                 child->right ? child->right->height : 0);
}
Node* AVLTree::rotateRightLeft(Node*& node) {
    node->right = rotateLeftLeft(node->right);
    return rotateRightRight(node);
}
Node* AVLTree::rotateLeftRight(Node*& node) {
    node->left = rotateRightRight(node->left);
    return rotateLeftLeft(node);
}

int AVLTree::balanceFactor(Node* node) {
    if (!node) return 0;
    int leftHeight = node->left ? node->left->height : 0;
    int rightHeight = node->right ? node->right->height : 0;
    return leftHeight - rightHeight;
}

Node* AVLTree::insert(Node* node, Student student) {
    if (!node) return new Node(student);

    if (std::stoi(student.id) < std::stoi(node->val.id))
        node->left = insert(node->left, student);
    else
        node->right = insert(node->right, student);

    node->height = 1 + std::max(node->left ? node->left->height : 0,
                                node->right ? node->right->height : 0);

    int balance = balanceFactor(node);

    if (balance < -1) {
        int rightBalance = balanceFactor(node->right);
        if (rightBalance > 0)
            node = rotateRightLeft(node);
        else
            node = rotateRightRight(node);
    } else if (balance > 1) {
        int leftBalance = balanceFactor(node->left);
        if (leftBalance < 0)
            node = rotateLeftRight(node);
        else
            node = rotateLeftLeft(node);
    }

    return node;
}
bool AVLTree::insert(std::string name, std::string id) {
    Student newStudent;
    try {
        newStudent = Student(name, id);
    } catch (const std::invalid_argument& e) {
        std::cout << "unsuccessful" << std::endl;
        return false;
    }

    // Unique IDs TODO
    // if (search(id)) {
    //     std::cout << "unsuccessful" << std::endl;
    //     return false;
    // }

    root = insert(root, newStudent);

    std::cout << "successful" << std::endl;
    return true;
}
bool AVLTree::remove(int id) { throw std::runtime_error("Not implemented"); }
bool AVLTree::search(int id) { throw std::runtime_error("Not implemented"); }
bool AVLTree::search(std::string name) {
    throw std::runtime_error("Not implemented");
}

// Traversals
void AVLTree::getInorder(Node* node, std::vector<Student>& inorder) {
    if (!node) return;
    getInorder(node->left, inorder);
    inorder.push_back(node->val);
    getInorder(node->right, inorder);
}
std::vector<Student> AVLTree::getInorder() {
    if (!root) return {};
    std::vector<Student> inorder;
    getInorder(root, inorder);
    return inorder;
}
void AVLTree::getPreorder(Node* node, std::vector<Student>& preorder) {
    if (!node) return;
    preorder.push_back(node->val);
    getPreorder(node->left, preorder);
    getPreorder(node->right, preorder);
}
std::vector<Student> AVLTree::getPreorder() {
    if (!root) return {};
    std::vector<Student> preorder;
    getPreorder(root, preorder);
    return preorder;
}
void AVLTree::getPostorder(Node* node, std::vector<Student>& postorder) {
    if (!node) return;
    getPostorder(node->left, postorder);
    getPostorder(node->right, postorder);
    postorder.push_back(node->val);
}
std::vector<Student> AVLTree::getPostorder() {
    if (!root) return {};
    std::vector<Student> postorder;
    getPostorder(root, postorder);
    return postorder;
}
void AVLTree::printVector(const std::vector<Student>& vec) {
    for (size_t i = 0; i < vec.size(); i++) {
        std::cout << vec[i].name;
        if (i != vec.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
}
void AVLTree::printInorder() {
    std::vector<Student> inorder = getInorder();
    printVector(inorder);
}
void AVLTree::printPreorder() {
    std::vector<Student> preorder = getPreorder();
    printVector(preorder);
}
void AVLTree::printPostorder() {
    std::vector<Student> postorder = getPostorder();
    printVector(postorder);
}

void AVLTree::printLevelCount() { throw std::runtime_error("Not implemented"); }
bool AVLTree::removeInorder(int n) {
    throw std::runtime_error("Not implemented");
}
