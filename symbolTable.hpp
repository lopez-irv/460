//
// Created by irvin on 4/8/2025.
//

#ifndef ASSIGNMENT1_SYMBOLTABLE_HPP
#define ASSIGNMENT1_SYMBOLTABLE_HPP

#include <string>
#include <vector>

using namespace std;

class symbolTable {
public:
    symbolTable(string name, string type, string data, int where) {
        indentifier_name = name;
        identifier_type = type;
        datatype = data;
        scope = where;
        isArray = false;
        array_size = 0;
        if (data == "int")
            datavalue = "0";
        else if (data == "char")
            datavalue = "";

    }

    symbolTable(string name, string type, string data, int where, int size, bool arrayStatus) {
        indentifier_name = name;
        identifier_type = type;
        datatype = data;
        scope = where;
        isArray = arrayStatus;
        array_size = size;
        if (data == "int")
            datavalue = "0";
        else if (data == "char")
            datavalue = "";
    }

    string indentifier_name;
    string identifier_type;
    string datatype;
    bool isArray;
    int array_size;
    int scope;
    string datavalue;


    vector <symbolTable> paramater_list;
};


#endif //ASSIGNMENT1_SYMBOLTABLE_HPP
