#include "tree.h"
#include <iostream>
#include <queue>
using namespace std;

Tree::Tree(char data)
{
    this->data = data;
    left = right = parent = nullptr;
}

Tree::Tree()
{
    left = right = parent = nullptr;
}

Tree::~Tree()
{
    delete left;
    delete right;
}

char Tree::get_data()
{
    return data;
}

Tree* Tree::get_left()
{
    return left;
}

Tree* Tree::get_right()
{
    return right;
}

void Tree::insert_left(char data)
{
    Tree* new_node = new Tree(data);
    if (this->left != nullptr)
    {
        this->left->parent = new_node;
        new_node->left = this->left;
    }
    this->left = new_node;
    new_node->parent = this;
}

void Tree::insert_right(char data)
{
    Tree* new_node = new Tree(data);
    if (this->right != nullptr)
    {
        this->right->parent = new_node;
        new_node->right = this->right;
    }
    this->right = new_node;
    new_node->parent = this;
}

void Tree::insert(char data)
{
    Tree* current = this;
    while (current != nullptr)
    {
        if (data > current->data)
        {
            if (current->right != nullptr)
                current = current->right;
            else
            {
                current->insert_right(data);
                return;
            }
        }
        else if (data < current->data)
        {
            if (current->left != nullptr)
                current = current->left;
            else
            {
                current->insert_left(data);
                return;
            }
        }
        else return;
    }
}

Tree* Tree::find(char data)
{
    if (this == nullptr || this->data == data) return this;
    else if (data > this->data) return this->right ? this->right->find(data) : nullptr;
    return this->left ? this->left->find(data) : nullptr;
}

void Tree::parse(Tree* current, list<char>& list)
{
    if (current == nullptr) return;
    list.push_back(current->get_data());
    parse(current->get_left(), list);
    parse(current->get_right(), list);
}

void Tree::print_tree(int level)
{
    if (this != nullptr)
    {
        if (right) right->print_tree(level + 1);
        for (int i = 0; i < level; i++) cout << "   ";
        cout << data << endl;
        if (left) left->print_tree(level + 1);
    }
}

void Tree::delete_left()
{
    if (left)
    {
        delete left;
        left = nullptr;
    }
}

void Tree::delete_right()
{
    if (right)
    {
        delete right;
        right = nullptr;
    }
}

void Tree::insert_strict_binary(char value)
{
    queue<Tree*> q;
    q.push(this);
    while (!q.empty())
    {
        Tree* current = q.front();
        q.pop();
        if (!current->left)
        {
            current->insert_left(value);
            return;
        }
        else q.push(current->left);
        if (!current->right)
        {
            current->insert_right(value);
            return;
        }
        else q.push(current->right);
    }
}

void Tree::collect_nodes(list<char>& nodes)
{
    if (this == nullptr) return;
    if (left) left->collect_nodes(nodes);
    nodes.push_back(data);
    if (right) right->collect_nodes(nodes);
}

Tree* Tree::build_balanced_BST(list<char>::iterator begin, list<char>::iterator end)
{
    int len = distance(begin, end);
    if (len <= 0) return nullptr;
    auto mid = begin;
    advance(mid, len / 2);
    Tree* node = new Tree(*mid);
    node->left = build_balanced_BST(begin, mid);
    node->right = build_balanced_BST(next(mid), end);
    return node;
}

bool Tree::remove(char value)
{
    Tree* node = find(value);
    if (!node) return false;

    // Случай 1: Узел - лист
    if (!node->left && !node->right) {
        if (node->parent) {
            if (node->parent->left == node)
                node->parent->left = nullptr;
            else
                node->parent->right = nullptr;
        }
        delete node;
        return true;
    }

    // Случай 2: У узла только один потомок
    Tree* child = node->left ? node->left : node->right;
    if (!node->left || !node->right) {
        if (node->parent) {
            if (node->parent->left == node)
                node->parent->left = child;
            else
                node->parent->right = child;
        }
        if (child) child->parent = node->parent;
        if (node == this) {
            // Особый случай: удаляем корень
            this->data = child->data;
            this->left = child->left;
            this->right = child->right;
            child->left = child->right = nullptr;
            delete child;
        } else {
            delete node;
        }
        return true;
    }

    // Случай 3: У узла два потомка
    Tree* successor = node->right;
    while (successor->left)
        successor = successor->left;

    char temp = successor->data;
    remove(temp);
    node->data = temp;

    return true;
}
