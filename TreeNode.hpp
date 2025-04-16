//
// Created by irvin on 3/27/2025.
//

#ifndef ASSIGNMENT1_TREENODE_HPP
#define ASSIGNMENT1_TREENODE_HPP
using namespace std;
#include <string>


class TreeNode {
public:
    TreeNode(string name, string type){
        child = nullptr;
        sibling = nullptr;
        TokenName = name;
        TokenType = type;

    }

    string get_NodeName(){
        return TokenName;
    }

    string get_NodeType(){
        return TokenType;
    }

    void set_sibling(TreeNode* newSib){
        sibling = newSib;
    }

    void set_child(TreeNode* newChild) {
        child = newChild;
    }

    TreeNode* get_child(){
        return child;
    }

    TreeNode* get_sibling() {
        return sibling;
    }

private:
    TreeNode* child;
    TreeNode* sibling;
    string TokenName;
    string TokenType;

};


#endif //ASSIGNMENT1_TREENODE_HPP
