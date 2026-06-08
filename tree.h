#ifndef TREE_H
#define TREE_H
#include <list>
using namespace std;

class Tree
{
private:
    char data;
    Tree* left;
    Tree* right;
    Tree* parent;
public:
    Tree(char);
    Tree();
    ~Tree();
    char get_data();
    Tree* get_left();
    Tree* get_right();
    void insert(char);
    void insert_left(char);
    void insert_right(char);
    void print_tree(int);
    void parse(Tree*, list<char>&);
    Tree* find(char);
    void delete_left();
    void delete_right();
    void insert_strict_binary(char);
    void collect_nodes(list<char>&);
    static Tree* build_balanced_BST(list<char>::iterator, list<char>::iterator);
    bool remove(char value);
};

#endif // TREE_H
