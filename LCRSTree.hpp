//
// Created by irvin on 3/27/2025.
//

#ifndef ASSIGNMENT1_LCRSTREE_HPP
#define ASSIGNMENT1_LCRSTREE_HPP

#include "TreeNode.hpp"
#include <iostream>
#include <fstream>


class LCRSTree {
public:
    LCRSTree(TreeNode* Treeroot){
        root = Treeroot;
        currNode = root;
        printNode = root;
        prevNode = nullptr;
    }

    void addSibling(TreeNode* newNode){
        currNode->set_sibling(newNode);
        prevNode = currNode;
        currNode = newNode;
    }

    void addChild(TreeNode* newNode) {
        currNode->set_child(newNode);
        prevNode = currNode;
        currNode = newNode;
    }

    TreeNode* get_root(LCRSTree curr_tree){
        return curr_tree.root;
    }

    int getNextNode() {
        if (printNode->get_child() != nullptr) {
            printNode = printNode->get_child();
            return 2;

        }
        else if (printNode->get_sibling() != nullptr) {
            printNode = printNode->get_sibling();
            return 1;
        }
        else {
            printNode = nullptr;
            return 0;
        }
    }

    void printTree() {
        ofstream ff("tree.txt");
        if (!ff) {
            cerr << "outpy file tree.txt could not be opened";
        }

        int tab = 1;
        while(printNode != nullptr){
            ff << printNode->get_NodeName();
            int tmp = getNextNode();
            if (tmp == 0) {
                continue;
            }
            else if (tmp == 1) {
                ff << "   ";
            }
            else {
                ff << " ";
                ff << endl;
                //for (int i = 0; i < tab; i++) {
                //    cout << "\t";
                //}
                tab += 1;
            }
        }
    }
private:
    TreeNode* root;
    TreeNode* currNode;
    TreeNode* printNode;
    TreeNode* prevNode;
};


#endif //ASSIGNMENT1_LCRSTREE_HPP
