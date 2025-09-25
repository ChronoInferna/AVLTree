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

// TODO generally for all in main: print unsuccessful or successful as needed
Node* AVLTree::insert(Node* node, Student student) {
    if (!node) return new Node(student);

    if (std::stoi(student.id) < std::stoi(node->val.id))
        node->left = insert(node->left, student);
    else
        node->right = insert(node->right, student);

    // Update height
    node->height = 1 + std::max(node->left ? node->left->height : 0,
                                node->right ? node->right->height : 0);

    // Balance tree
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
        return false;
    }

    // Unique IDs
    if (inTree(std::stoi(id))) return false;

    root = insert(root, newStudent);

    return true;
}
Node* AVLTree::remove(Node* node, int id) {
    if (!node) return node;

    if (id < std::stoi(node->val.id))
        node->left = remove(node->left, id);
    else if (id > std::stoi(node->val.id))
        node->right = remove(node->right, id);
    else {
        // Node with only one child or no child
        if (!node->left || !node->right) {
            Node* temp = node->left ? node->left : node->right;

            // No child case
            if (!temp) {
                temp = node;
                node = nullptr;
                delete temp;
                return node;
            }

            // One child case
            node->val = temp->val;
            Node* tempLeft = temp->left;
            Node* tempRight = temp->right;

            node->left = tempLeft;
            node->right = tempRight;

            temp->left = nullptr;
            temp->right = nullptr;
            delete temp;
        } else {
            // 2 child case
            Node* temp = node->right;

            while (temp->left) temp = temp->left;

            node->val = temp->val;
            node->right = remove(node->right, std::stoi(temp->val.id));
        }
    }

    // Update height
    node->height = 1 + std::max(node->left ? node->left->height : 0,
                                node->right ? node->right->height : 0);

    // Balance tree
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
bool AVLTree::remove(int id) {
    // Search for the id first
    if (!inTree(id)) return false;

    root = remove(root, id);

    return true;
}
bool AVLTree::inTree(int id) {
    Node* current = root;
    while (current)
        if (std::stoi(current->val.id) == id)
            return true;
        else if (id < std::stoi(current->val.id))
            current = current->left;
        else
            current = current->right;
    return false;
}
bool AVLTree::search(int id) {
    Node* current = root;
    while (current) {
        if (std::stoi(current->val.id) == id) {
            std::cout << current->val.name << std::endl;
            return true;
        } else if (id < std::stoi(current->val.id)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return false;
}
bool AVLTree::search(std::string name) {
    bool found = false;

    // Lambda function for inorder DFS! Cool trick I learned
    auto inorderDFS = [&](Node* node, const std::string& targetName,
                          auto& inorderDFSRef) -> void {
        if (!node) return;

        inorderDFSRef(node->left, targetName, inorderDFSRef);

        if (node->val.name == targetName) {
            std::cout << node->val.id << std::endl;
            found = true;
        }

        inorderDFSRef(node->right, targetName, inorderDFSRef);
    };

    inorderDFS(root, name, inorderDFS);

    return found;
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

int AVLTree::getLevelCount() {
    if (!root) return 0;
    return root->height;
}
void AVLTree::printLevelCount() { std::cout << getLevelCount() << std::endl; }
bool AVLTree::removeInorder(int n) {
    if (!root) return false;
    std::vector<Student> inorder;
    inorder = getInorder();
    if (n >= int(inorder.size()) || n < 0) return false;
    return remove(std::stoi(inorder[n].id));
}
