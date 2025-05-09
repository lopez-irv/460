#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <tuple>
#include "TreeNode.hpp"
#include "LCRSTree.hpp"
#include <list>
#include "symbolTable.hpp"
#include <map>
#include <unordered_map>
#include <stack>
using namespace std;

vector<string> reserved_word = {"function", "procedure", "class", "struct", "namespace", "void","printf"  };
vector<string> data_types = {"int", "char", "bool",  };

//list <symbolTable> symTable;
vector<unordered_map<string , symbolTable*>> variables(3);

void assignPrint(vector<pair<string, int>> tokenStack) {
    ofstream tokenList("tokenlist.txt");
    if (!tokenList) {
        cerr << "outpy file tokenlist could not be opened";
    }

    bool inQuote = false;


    for (int i = 0; i < tokenStack.size(); ++i) {
        if (isdigit(tokenStack.at(i).first.at(0)) && !all_of(tokenStack.at(i).first.begin() + 1, tokenStack.at(i).first.end(), ::isdigit)) {
            tokenList.close();
            tokenList.open("tokenlist.txt", ios::trunc);
            tokenList << "Syntax error on line " << tokenStack.at(i).second << ": invalid integer" << endl;
            exit(5);
        }
        if (tokenStack.at(i).first == "(") {
            tokenList << "L_PAREN " << endl;
            tokenList << "(" << endl;
        }
        else if (tokenStack.at(i).first == ")") {
            tokenList << "R_PAREN "  << endl;
            tokenList << ")" << endl;
        }
        else if (tokenStack.at(i).first == "{") {
            tokenList << "L_BRACE "  << endl;
            tokenList << "{" << endl;
        }
        else if (tokenStack.at(i).first == "}") {
            tokenList << "R_BRACE "  << endl;
            tokenList << "}" << endl;
        }
        else if (tokenStack.at(i).first == "[") {
            tokenList << "L_BRACKET "  << endl;
            tokenList << "[" << endl;
            if (tokenStack.at(i+1).first.at(0) == '-'){
                cout << "Syntax error on line "<< tokenStack.at(i).second << " array declaration size must be a positive integer. " << endl;
                exit(33);
            }
        }
        else if (tokenStack.at(i).first == "]") {
            tokenList << "R_BRACKET " << endl;
            tokenList << "]" << endl;
        }
        else if (tokenStack.at(i).first == "\"") {
            if (inQuote) {
                inQuote = false;
            }
            else {
                inQuote = true;
            }
            tokenList << "DOUBLE_QUOTE " << endl;
            tokenList << "\"" << endl;
        }
        else if (tokenStack.at(i).first == "\'") {
            if (inQuote) {
                inQuote = false;
            }
            else {
                inQuote = true;
            }
            tokenList << "SINGLE_QUOTE " << endl;
            tokenList << "\'" << endl;
        }
        else if (tokenStack.at(i).first == "=") {
            if (tokenStack.at(i+1).first == "=") {
                tokenList << "BOOLEAN_EQUAL " << endl;
                tokenList << "==" << endl;
                i++;
            }
            else {
                tokenList << "ASSIGNMENT_OPERATOR " << endl;
                tokenList << "=" << endl;
            }
        }
        else if (tokenStack.at(i).first == ";") {
            tokenList << "SEMICOLON " << endl;
            tokenList << ";" << endl;
        }
        else if (tokenStack.at(i).first == ",") {
            tokenList << "COMMA " << endl;
            tokenList << "," << endl;
        }
        else if (tokenStack.at(i).first == "=") {
            tokenList << "ASSIGNMENT_OPERATOR " << endl;
            tokenList << "=" << endl;
        }
        else if (tokenStack.at(i).first == "!") {
            tokenList << "NOT " << endl;
            tokenList << "!" << endl;
        }
        else if (tokenStack.at(i).first == "%") {
            tokenList << "MODULO " << endl;
            tokenList << "%" << endl;
        }
        else if (tokenStack.at(i).first == "+") {
            tokenList << "PLUS " << endl;
            tokenList << "+" << endl;
        }
        else if (tokenStack.at(i).first == "*") {
            tokenList << "ASTERISK " << endl;
            tokenList << "*" << endl;
        }

        else if (tokenStack.at(i).first == "<") {
            if (tokenStack.at(i+1).first != "=") {
                tokenList << "LT " << endl;
                tokenList << "<" << endl;
            }
            else {
                tokenList << "LT_EQUAL" << endl;
                tokenList << "<=" << endl;
                i++;
            }
        }
        else if (tokenStack.at(i).first == ">") {
            if (tokenStack.at(i+1).first != "=") {
                tokenList << "GT " << endl;
                tokenList << ">" << endl;
            }
            else {
                tokenList << "GT_EQUAL " << endl;
                tokenList << ">=" << endl;
                i++;
            }
        }
        else if (tokenStack.at(i).first == "&" && tokenStack.at(i + 1).first == "&") {
            tokenList << "BOOLEAN_AND " << endl;
            tokenList << "&&" << endl;
            i++;
        }
        else if (tokenStack.at(i).first == "|" && tokenStack.at(i + 1).first == "|") {
            tokenList << "BOOLEAN_OR " << endl;
            tokenList << "||" << endl;
            i++;
        }
        else if (inQuote) {
            tokenList << "STRING " << endl;
            tokenList << tokenStack.at(i).first << endl;
            size_t pos = tokenStack.at(i).first.find("\\");
            if (pos != std::string::npos) {
                if (tokenStack.at(i).first.size() == pos +1) {
                    if (tokenStack.at(i+1).first == "\"" || tokenStack.at(i+1).first == "\'") {
                        cout << "Syntax error on line " << tokenStack.at(i).second << " : unterminated string quote." << endl;
                        exit(44);
                    }
                }
            }
        }
        //tokenStack.at(i).first.at(pos + 1) == '\'' || tokenStack.at(i).first.at(pos + 1) == '\"'
        else if (tokenStack.at(i).first == "-" ) {
            tokenList << "MINUS " << endl;
            tokenList << "-" << endl;
        }
        else if (tokenStack.at(i).first == "/" ) {
            tokenList << "DIVIDE "  << endl;
            tokenList << "/" << endl;
        }

        else if (all_of(tokenStack.at(i).first.begin(), tokenStack.at(i).first.end(), ::isdigit) || ((tokenStack.at(i).first.at(0) == '-') && all_of(tokenStack.at(i).first.begin() + 1, tokenStack.at(i).first.end(), ::isdigit))) {
            tokenList << "INTEGER " << endl ;
            tokenList << tokenStack.at(i).first << endl;
        }
        else {
            tokenList << "IDENTIFIER "  << endl;
            tokenList << tokenStack.at(i).first  << endl;
            auto isit2 = std::find(data_types.begin(), data_types.end(), tokenStack.at(i).first);
            if (tokenStack.at(i).first == "function") {
                auto isit = std::find(reserved_word.begin(), reserved_word.end(), tokenStack.at(i+1).first);
                if (isit != reserved_word.end()) {
                    cout << "Syntax error on line "<< tokenStack.at(i).second << " : reserved word " << tokenStack.at(i+1).first <<  " cannot be used for the name of a function." << endl;
                    exit(31);

                }
            }
            if (isit2 != data_types.end()) {
                auto isit1 = std::find(data_types.begin(), data_types.end(), tokenStack.at(i+1).first);
                isit2 = std::find(reserved_word.begin(), reserved_word.end(), tokenStack.at(i+1).first);
                if (isit1 != data_types.end() || isit2 != reserved_word.end()){
                    cout << "Syntax error on line "<< tokenStack.at(i).second << " : reserved word " << tokenStack.at(i+1).first <<  " cannot be used for the name of a variable." << endl;
                    exit(32);
                }

            }
        }
    }
    tokenList << endl;
}


bool allBlank(string line1){
    for (int i = 0; i < line1.size(); ++i) {
        if (!isblank(line1.at(i))) {
            return false;
        }
    }
    return true;
}

vector<pair<string, int>> tokenize(string readfile) {

    vector<char> singleTokens = {'(', ')', '{', '}', ';', '=', '\"', '\'', '%', '*', '+', '<', '>', ',', '&', '[', ']', '/', '!'};

    ifstream readFrom(readfile);
    if (!readFrom) {
        std::cerr << "Error: Unable to open file 'example.txt' for reading." << endl;
        exit(4);
    }

    char c;
    vector<pair<string, int>> tokenStack;
    string line;
    string token;

    bool inQuote = false;

    int lineNumber = 0;
    while (getline(readFrom, line)) {
        lineNumber++;
        //check if the line gathered is just empty lines
        //is so skip
        if (allBlank(line))
            continue;
        //read the individuall characters
        istringstream ss(line);
        while (ss.get(c)) {
            //what to do if in a quote
            if (inQuote){
                if (c == '\'' || c == '\"') {
                    tokenStack.emplace_back(token, lineNumber);
                    token = "";
                    token.push_back(c);
                    tokenStack.emplace_back(token, lineNumber);
                    token = "";
                    inQuote = false;
                }
                else{
                    token.push_back(c);
                }

            }
            else if (token.empty()){
                if (c == ' ' || c == '\n' || c == '\t'){
                    continue;
                }
                else if (find(singleTokens.begin(), singleTokens.end(), c) != singleTokens.end()) {
                    if (c == '\'' || c == '\"') {
                        token.push_back(c);
                        tokenStack.emplace_back(token, lineNumber);
                        token = "";
                        inQuote = true;
                    }
                    else {
                        token.push_back(c);
                        tokenStack.emplace_back(token, lineNumber);
                        token = "";
                    }
                }
                else {
                    token.push_back(c);
                }
            }
            else {
                if (c == ' ' || c == '\n' || c == '\t') {
                    tokenStack.emplace_back(token, lineNumber);
                    token = "";
                }
                else if (find(singleTokens.begin(), singleTokens.end(), c) != singleTokens.end()) {
                    if (c == '\'' || c == '\"') {
                        tokenStack.emplace_back(token, lineNumber);
                        token = "";
                        token.push_back(c);
                        tokenStack.emplace_back(token, lineNumber);
                        token = "";
                        inQuote = true;
                    }
                    else {
                        tokenStack.emplace_back(token, lineNumber);
                        token = "";
                        token.push_back(c);
                        tokenStack.emplace_back(token, lineNumber);
                        token = "";
                    }
                }
                else {
                    token.push_back(c);
                }
            }
        }
    }
    /*
    for (int i = 0; i < tokenStack.size(); ++i) {
        cout << tokenStack.at(i).first << endl;
    }
    */

    assignPrint(tokenStack);
    return tokenStack;
}

/*void recursive_parse(ifstream &ifs, LCRSTree* tree) {
    string tokenType, tokenName;
    if (!getline(ifs, tokenType) && !getline(ifs, tokenName)) {
        return;
    }
    TreeNode* tmpNode = new TreeNode(tokenName, tokenType);
    if (tokenName == "{" || tokenName == "}") {
        tree->addChild(tmpNode);
        recursive_parse(ifs, tree);
    }
    else if (tokenName == ";") {
        tree->addChild(tmpNode);
        recursive_parse(ifs, tree);
    }
    else {
        tree->addSibling(tmpNode);
        recursive_parse(ifs, tree);
    }

}
 */
bool checkIssues(vector<vector<string>> scope_list, string id_name, int scope) {
    if (scope_list.at(scope).empty()){
        return false;
    }
    for (int i = 0; i < scope_list.at(0).size(); ++i) {
        if (scope_list.at(0).at(i) == id_name) {
            return true;
        }
    }
    for (int i = 0; i < scope_list.at(scope).size(); ++i) {
        if (scope_list.at(scope).at(i) == id_name) {
            return true;
        }
    }
    return false;
}

//create the symbol table linked list
//function takes a token Stack as a paramater
//goes through the stack using getline
//when a token of interst is incountered, perform approprite action
//tokens of interest include procedure, function and a variable type (int, char, bool) fixme
list<symbolTable> createSymbolTable(vector<pair<string, int>> const &tokenStack) {
    ofstream currentStack("currStack.txt");
    for (int i = 0; i < tokenStack.size(); ++i) {
         currentStack << tokenStack.at(i).first << " " << tokenStack.at(i).second << endl;
    }

    ofstream ass4("output4.txt");
    ofstream ass5("part5.txt");

    vector<vector<string>> scope_variable_list(5);

    //data used for linked list
    string id_name;
    string id_type;
    string dataType;
    int scope = 0;
    int array_size = 0;
    bool isArray = false;

    // keep track of scope
    int highestScope = 0;
    int current_scope = 0;
    //to keep track if we are inside a function
    int bracketTracker = 0;

    //vector<string> scopeStack;


    list <symbolTable> symTable;

    for (int i = 0; i < tokenStack.size(); ++i) {
        if (tokenStack.at(i).first == "function") {
            current_scope = highestScope + 1;
            highestScope = current_scope;
            id_type = "function";
            i+=1;
            dataType = tokenStack.at(i).first;
            i+=1;
            id_name = tokenStack.at(i).first;
            ass5 << tokenStack.at(i).first << endl;
            i+=1;
            isArray = false;
            array_size = 0;
            symTable.emplace_back(id_name, id_type, dataType, current_scope, array_size,  isArray);
            while (tokenStack.at(i).first != ")") {
                i+=1;
                if (tokenStack.at(i).first == "void") {
                    i+=1;
                    continue;
                }
                dataType = tokenStack.at(i).first;
                i+=1;
                id_name = tokenStack.at(i).first;
                i+=1;
                if (tokenStack.at(i).first == "[") {
                    isArray = true;
                    i += 1;
                    array_size = stoi(tokenStack.at(i).first);
                    i+=2;
                }
                if (checkIssues(scope_variable_list, id_name, current_scope)) {
                    cout << "Error on line " << tokenStack.at(i).second << " variable " << "\"" << id_name << "\"" << " is already declared" << endl;
                    exit(42);
                }
                else {
                    scope_variable_list.at(current_scope).push_back(id_name);
                }
                symTable.back().paramater_list.emplace_back(id_name, id_type, dataType, current_scope, array_size, isArray);
                array_size = 0;
                isArray = false;
            }
        }
        else if (tokenStack.at(i).first == "procedure") {
            current_scope = highestScope + 1;
            highestScope = current_scope;
            id_type = "procedure";
            dataType = "NOT APPLICABLE";
            i+=1;
            id_name = tokenStack.at(i).first;
            ass5 << tokenStack.at(i).first << endl;
            i+=1;
            isArray = false;
            array_size = 0;
            symTable.emplace_back(id_name, id_type, dataType, current_scope, array_size, isArray);
            while (tokenStack.at(i).first != ")") {
                i+=1;
                if (tokenStack.at(i).first == "void") {
                    i+=1;
                    continue;
                }
                dataType = tokenStack.at(i).first;
                i+=1;
                id_name = tokenStack.at(i).first;
                i+=1;
                if (tokenStack.at(i).first == "[") {
                    isArray = true;
                    i += 1;
                    array_size = stoi(tokenStack.at(i).first);
                    i+=2;
                }
                if (checkIssues(scope_variable_list, id_name, current_scope)) {
                    cout << "Error on line " << tokenStack.at(i).second << " variable " << "\"" << id_name << "\"" << " is already declared" << endl;
                    exit(43);
                }
                else {
                    scope_variable_list.at(current_scope).push_back(id_name);
                }
                symTable.back().paramater_list.emplace_back(id_name, id_type, dataType, current_scope, array_size, isArray);
                array_size = 0;
                isArray = false;
            }
        }
        else if (tokenStack.at(i).first == "int" || tokenStack.at(i).first == "char" || tokenStack.at(i).first == "bool") {
            dataType = tokenStack.at(i).first;

            while(tokenStack.at(i).first != ";") {
                i+=1;
                id_name = tokenStack.at(i).first;
                id_type = "datatype";
                i += 1;
                if (tokenStack.at(i).first == "[") {
                    isArray = true;
                    i += 1;
                    array_size = stoi(tokenStack.at(i).first);
                    i+=2;
                }
                if (checkIssues(scope_variable_list, id_name, current_scope)) {
                    cout << "Error on line " << tokenStack.at(i).second << " variable " << "\"" << id_name << "\"" << " is already declared" << endl;
                    exit(44);
                }
                else {
                    scope_variable_list.at(current_scope).push_back(id_name);
                }
                symTable.emplace_back(id_name, id_type, dataType, current_scope, array_size, isArray);
                array_size = 0;
                isArray = false;
            }
        }
        else if (tokenStack.at(i).first == "{") {
            bracketTracker +=1;

        }
        else if (tokenStack.at(i).first == "}") {
            bracketTracker -=1;
            if (bracketTracker == 0)
                current_scope = 0;
            if (bracketTracker < 0) {
                cout << "there is an error with the number of currly braces: more closing braces than opening braces" << endl;
                cout <<"on line: " << tokenStack.at(i).second << endl;
                exit(41);
            }
        }
        else {
            continue;
        }
    }
    for (auto value: symTable) {
        ass4 << "IDENTIFIER_NAME: " << value.indentifier_name << endl;
        ass4 << "IDENTIFIER_TYPE: " << value.identifier_type << endl;
        ass4 << "DATATYPE: " << value.datatype << endl;
        if (value.isArray)
            ass4 << "DATATYPE_IS_ARRAY: " << "yes" << endl;
        else
            ass4 << "DATATYPE_IS_ARRAY: " << "no" << endl;
        ass4 << "DATATYPE_ARRAY_SIZE: " << value.array_size << endl;
        ass4 << "SCOPE: " << value.scope << endl;
        ass4 << "VALUE OF VAR: " << value.datavalue <<  endl << endl;
    }
    for(auto value: symTable) {
        if (value.paramater_list.empty()) {
            continue;
        }
        ass4 << "PARAMETER LIST FOR: " << value.indentifier_name << endl;
        for (int i = 0; i < value.paramater_list.size(); ++i) {
            ass4 << "IDENTIFIER_NAME: " << value.paramater_list.at(i).indentifier_name << endl;
            ass4 << "DATATYPE: " <<  value.paramater_list.at(i).datatype << endl;
            if (value.paramater_list.at(i).isArray)
                ass4 << "DATATYPE_IS_ARRAY: "  "yes" << endl;
            else
                ass4 << "DATATYPE_IS_ARRAY: "  "no" << endl;
            ass4 << "DATATYPE_ARRAY_SIZE: " << value.paramater_list.at(i).array_size << endl;
            ass4 << "SCOPE:" << value.paramater_list.at(i).scope << endl << endl;
        }
    }

    //cout << "gear" << endl;
    return symTable;
}

int getPrecedence(const string& token) {
    if (token == "||") return 0;
    if (token == "&&") return 1;
    if (token == "==" || token == "!=") return 2;
    if (token == "<" || token == "<=" || token == ">" || token == ">=") return 3;
    if (token == "+" || token == "-") return 4;
    if (token == "*" || token == "/" || token == "%") return 5;
    if (token == "!") return 6; // Unary NOT - highest precedence
    if (token == "=") return -1;
    if (token == "(" || token == ")") return 99; // special handling
    return -99;
}
bool isOperator (string Operator) {
    vector <string> operators {"+", "-", "*", "/", "%", "(", ")", "=", "!=", "==", "<=", "<", ">", ">=", "&&", "||", "!"}; //all the stuff that will go to the stack
    for(int i =0; i < operators.size(); ++i) {
        if(Operator == operators.at(i)) {
            return true;
        }
    }
    return false;
}
//sum = sum_of_first_n_squares ( n );        START
//sum sum_of_n_first_squares ( n ) =         WANT
//sum sum_of_first_n_squares n ) ( =          HAVE
void infixToPostfix(vector<string>& infix) {
    vector<string> postFix;
    vector<string> operatorStack;

    for (const string& token : infix) {
        if (!isOperator(token)) {
            postFix.push_back(token);
        }
        else if (token == "(") {
            operatorStack.push_back(token);
        }
        else if (token == ")") {
            while (!operatorStack.empty() && operatorStack.back() != "(") {
                postFix.push_back(operatorStack.back());
                operatorStack.pop_back();
            }
            if (!operatorStack.empty() && operatorStack.back() == "(") {
                operatorStack.pop_back(); // remove the '('
            }
        }
        else { // token is an operator
            while (!operatorStack.empty() &&
                   getPrecedence(token) <= getPrecedence(operatorStack.back()) &&
                   operatorStack.back() != "(") {
                postFix.push_back(operatorStack.back());
                operatorStack.pop_back();
            }
            operatorStack.push_back(token);
        }
    }

    while (!operatorStack.empty()) {
        postFix.push_back(operatorStack.back());
        operatorStack.pop_back();
    }

    infix = postFix;

}

bool isAFunction(const vector<string>& funList, const string& curFunction) {
    for (int i = 0; i < funList.size(); ++i) {
        if (curFunction == funList.at(i))
            return true;
    }
    return false;
}

// this is where the new token list function starts
//function to create the new token list
//takes the token list that is outputed form part 2, reads through it and creates another accordingly
void secondTokenList(string originalList, string newList) {
    //read from to get the function and procedure names
    ifstream flist("part5.txt");
    string curFunctionName = "";
    vector<string> functionList;

    while (getline(flist, curFunctionName)) {
        functionList.push_back(curFunctionName);
    }


    ifstream inputfile(originalList);
    ofstream outputfile(newList);

    string tokenType;
    string tokenName;
    vector<string> postFixAssignment;
    vector<string> pf1;
    vector<string> pf2;
    vector<string> pf3;

    while (getline(inputfile, tokenType)){
        getline(inputfile, tokenName);
        if(tokenName == "")
            break;
        if (isAFunction(functionList, tokenName)) {
            outputfile << "CALL" << endl;
            outputfile << tokenName << endl;
            getline(inputfile, tokenType);
            getline(inputfile, tokenName);
            while (tokenName != ";") {
                outputfile << tokenName << endl;
                getline(inputfile, tokenType);
                getline(inputfile, tokenName);
            }
            outputfile << ";" << endl;

        }

        else if (tokenName == "function" || tokenName == "procedure") {
            outputfile << "DECLARATION" << endl;
            while (tokenName != ")") {
                getline(inputfile, tokenType);
                getline(inputfile, tokenName);
            }
            outputfile << ";" << endl;
        }
        //int i,j,k;
        else if (tokenName == "int" || tokenName == "char" || tokenName == "bool") {
            getline(inputfile, tokenType);
            getline(inputfile, tokenName);
            outputfile << "DECLARATION" << endl;
            getline(inputfile, tokenType);
            getline(inputfile, tokenName);
            if (tokenName == "[") {
                getline(inputfile, tokenType);
                getline(inputfile, tokenName);
                getline(inputfile, tokenType);
                getline(inputfile, tokenName);
                getline(inputfile, tokenType);
                getline(inputfile, tokenName);
            }
            while(tokenName != ";") {
                getline(inputfile, tokenType);
                getline(inputfile, tokenName);
                outputfile << ";" << endl;
                outputfile << "DECLARATION" << endl;
                getline(inputfile, tokenType);
                getline(inputfile, tokenName);
            }
            outputfile << ";" << endl;
        }
        else if (tokenName == "{" || tokenName == "}") {
            if (tokenName == "{")
                outputfile << "BEGIN BLOCK" << endl;
            else
                outputfile << "END BLOCK" << endl;
            //outputfile << ";" << endl;
        }
        else if (tokenName == "return") {
            outputfile << tokenName << endl;
            getline(inputfile, tokenType);
            getline(inputfile, tokenName);
            while (tokenName != ";") {
                postFixAssignment.push_back(tokenName);
                getline(inputfile, tokenType);
                getline(inputfile, tokenName);
            }
            infixToPostfix(postFixAssignment);
            for (const string& token : postFixAssignment) {
                outputfile << token << "\n";
            }
            postFixAssignment.clear();
            outputfile << ";" << endl;
        }
        else if (tokenName == "while") {
            outputfile << "while" << "\n";
            getline(inputfile, tokenType);
            getline(inputfile, tokenName);
            postFixAssignment.push_back(tokenName);
            getline(inputfile, tokenType);
            getline(inputfile, tokenName);
            while (tokenName != "{") {
                postFixAssignment.push_back(tokenName);
                getline(inputfile, tokenType);
                getline(inputfile, tokenName);
            }
            infixToPostfix(postFixAssignment); //converts assignment to postfix, add logic to
            //add parenthesis around parameters.
            for (const string& token : postFixAssignment) {
                outputfile << token << "\n";
            }
            postFixAssignment.clear();
            outputfile << ";" << endl;
            outputfile << "BEGIN BLOCK\n";
        }
        else if (tokenName == "for") {

            //outputfile << "for" << endl;
            //outputfile << "statement 1" << endl;
            getline(inputfile, tokenType);
            getline(inputfile, tokenName);
            getline(inputfile, tokenType);
            getline(inputfile, tokenName);
            while (tokenName != ";") {
                //outputfile << tokenName << endl;
                pf1.push_back(tokenName);
                getline(inputfile, tokenType);
                getline(inputfile, tokenName);

            }
            getline(inputfile, tokenType);
            getline(inputfile, tokenName);
            while (tokenName != ";") {
                //outputfile << tokenName << endl;
                pf2.push_back(tokenName);
                getline(inputfile, tokenType);
                getline(inputfile, tokenName);
            }
            getline(inputfile, tokenType);
            getline(inputfile, tokenName);
            while (tokenName != ")") {
                //outputfile << tokenName << endl;
                pf3.push_back(tokenName);
                getline(inputfile, tokenType);
                getline(inputfile, tokenName);
            }
            //three for loops to output for loop tokens in postfix.
            infixToPostfix(pf1), infixToPostfix(pf2), infixToPostfix(pf3);
            outputfile << "For Expression 1\n";
            for (const string& token : pf1) {
                outputfile << token << "\n";
            }
            outputfile << ";" << endl;
            outputfile << "For Expression 2\n";
            for (const string& token : pf2) {
                outputfile << token << "\n";
            }
            outputfile << ";"  << endl;
            outputfile<< "For Expression 3\n";
            for (const string& token : pf3) {
                outputfile << token << "\n";
            }
            outputfile << ";" << endl;
            pf1.clear(), pf2.clear(), pf3.clear();
        }
        else if (tokenName == "if") {
            outputfile << "if" << "\n";
            getline(inputfile, tokenType);
            getline(inputfile, tokenName);
            //outputfile << tokenName << endl;
            postFixAssignment.push_back(tokenName);
            getline(inputfile, tokenType);
            getline(inputfile, tokenName);
            while (tokenName != "{") {
                postFixAssignment.push_back(tokenName);
                getline(inputfile, tokenType);
                getline(inputfile, tokenName);
            }
            infixToPostfix(postFixAssignment); //converts assignment to postfix, add logic to
            //add parenthesis around parameters.
            string functionName;
            for(int i = 0; i <postFixAssignment.size(); ++i) {
                functionName = postFixAssignment.at(i);
                if((isAFunction(functionList, functionName))) {
                    postFixAssignment.insert(postFixAssignment.begin() + i + 1, "(");
                    postFixAssignment.insert(postFixAssignment.begin() + postFixAssignment.size() -1, ")");
                }
            }
            for (const string& token : postFixAssignment) {
                outputfile << token << "\n";
            }
            postFixAssignment.clear();
            outputfile << ";" << endl;
            outputfile << "BEGIN BLOCK" << endl;
        }
        else if (tokenName == "else") {
            outputfile << tokenName << endl;
            getline(inputfile, tokenType);
            getline(inputfile, tokenName);
            outputfile << "BEGIN BLOCK" << endl;
            outputfile << ";" << endl;
        }
        else if (tokenName == "printf") {
            outputfile << "printf" << endl;
            getline(inputfile, tokenType);
            getline(inputfile, tokenName);
            while (tokenName != ";") {
                if (tokenName == "\"" || tokenName == "," || tokenName == "(" || tokenName == ")") {
                    getline(inputfile, tokenType);
                    getline(inputfile, tokenName);
                } else {
                    outputfile << tokenName << endl;
                    getline(inputfile, tokenType);
                    getline(inputfile, tokenName);
                }
            }
            outputfile << ";" << endl;
        }
        else{
            outputfile << "assignment" << "\n"; // CRISTIANS LOGIC WILL GO HERE....
            //postFixAssignment.push_back(tokenName);
            while (tokenName != ";"){
                //outputfile << tokenName << endl;
                postFixAssignment.push_back(tokenName);
                if (getline(inputfile, tokenType)) {
                    getline(inputfile, tokenName);
                }
                else
                    break;
            }
            infixToPostfix(postFixAssignment); //converts assignment to postfix, add logic to
            //add parenthesis around parameters.
            //new logic to add parenthesis if needed to an assignment if theres a function call.
            string functionName;
            for(int i = 0; i <postFixAssignment.size(); ++i) {
                functionName = postFixAssignment.at(i);
                if((isAFunction(functionList, functionName))) {
                    postFixAssignment.insert(postFixAssignment.begin() + i + 1, "(");
                    postFixAssignment.insert(postFixAssignment.begin() + postFixAssignment.size() -1, ")");
                }
            }
            for (const string& token : postFixAssignment) {
                outputfile << token << "\n";
            }
            postFixAssignment.clear();

            outputfile << ";" << endl;
        }
    }
}

//create new trees for each function
//This function goes through the AST and the Symbol table at the same time  to determine
//which declarations in the AST are functions and procedures to separate them and create
//individual trees for each
//each tree will be stored in an unorderd_map
// map: <string(function name), tree>

//takes no paramaters read from tree.txt and output4.txt
void separate_trees(unordered_map<string, LCRSTree*> *mapy, LCRSTree* myTree) {
    int bCount = 0;
    TreeNode *curNode = myTree->root;
    while (curNode != NULL) {
        if (curNode->sTable == NULL) {
            if (curNode->get_sibling() == NULL){
                curNode = curNode->get_child();
            }
            else {
                curNode = curNode->get_sibling();
            }
                continue;
        }
        if (curNode->sTable->identifier_type == "function" || curNode->sTable->identifier_type == "procedure" ) {
            TreeNode *tmpNode = new TreeNode(curNode->sTable->indentifier_name);
            tmpNode->sTable = curNode->sTable;
            LCRSTree* newTree = new LCRSTree(tmpNode);
            TreeNode* curNewNode = newTree->root;

            curNode = curNode->get_child();
            curNewNode->child = curNode;
            curNewNode = curNewNode->get_child();

            while (curNode != NULL) {
                if (curNode->get_NodeName() == "DECLARATION") {
                    int varScope = curNode->sTable->scope;
                    variables.at(varScope).emplace(curNode->sTable->indentifier_name, curNode->sTable);
                }
                if (curNode->get_NodeName() == "BEGIN BLOCK") {
                    bCount++;
                }
                else if (curNode->get_NodeName() == "END BLOCK") {
                    bCount--;
                }
                if (bCount == 0) {
                    curNode = curNewNode->get_child();
                    curNewNode->child = NULL;
                    curNewNode->sibling = NULL;
                    string funcName = newTree->root->sTable->indentifier_name;
                    mapy->insert({funcName, newTree});
                    //cout << "size at time of creation " << mapy->at(funcName)->treeSize() << endl;
                    //curNode = curNewNode->get_child();
                    break;
                }
                if (curNode->get_sibling() == NULL) {
                    curNode = curNode->get_child();
                    curNewNode->child = curNode;
                    curNewNode = curNewNode->get_child();
                } else{
                    curNode = curNode->get_sibling();
                    curNewNode->set_sibling(curNode);
                    curNewNode = curNewNode->get_sibling();
                }
            }
        }
        else {
            int varScope = curNode->sTable->scope;
            variables.at(varScope).emplace(curNode->sTable->indentifier_name, curNode->sTable);
        }

    }
    //for (int i = 0; i < variables.size(); ++i) {
    //    cout << "number of variables:" << endl;
    //    cout << variables.at(i).size() << endl;
    //}
}

// input: sum = n * (n + 1) * (2 * n + 1) / 6;
//postfix output:  sum n n 1 + * 2 n * 1 + * 6 / =
int postfixEvaluator(const vector<string>& postfix) {
    stack<int> z;


    for (const string& token : postfix) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            int right = z.top(); z.pop();
            int left = z.top(); z.pop();


            int result = 0;
            if (token == "+") result = left + right;
            else if (token == "-") result = left - right;
            else if (token == "*") result = left * right;
            else if (token == "/") result = left / right;


            z.push(result);
        } else {
            // Assume it's a number
            z.push(stoi(token));
        }
    }


    return z.top();
}

void printfunction(vector<string> Strings) {
    if (Strings.size() == 1) {
        cout << Strings.at(0);
    }
    else {
        string nextString;
        string curString = Strings.at(0);
        int StringsPoint = 1;

        for (int i = 0; i < curString.size(); ++i) {
            if (curString.at(i) == '%'){
                //cout <<
            }
        }
    }
}


void evaluateTree(vector<pair<LCRSTree*, TreeNode*>> &runningStack) {
    if (runningStack.empty()) {
        return;
    }
    TreeNode* currentNode = runningStack.back().second;
    while(currentNode != NULL) {
        //cout << currentNode->get_NodeName() << endl;
        if (currentNode->get_NodeName() == "DECLARATION") {
            currentNode = currentNode->get_child();
            continue;
        }
        else if (currentNode->get_NodeName() == "BEGIN BLOCK") {
            currentNode = currentNode->get_child();
            continue;
        }
        else if (currentNode->get_NodeName() == "END BLOCK") {
            currentNode = currentNode->get_child();
            continue;
        }
        else if (currentNode->get_NodeName() == "printf") {
            vector<string> printStrings;
            string tmp;
            while (currentNode->get_sibling() != NULL) {
                currentNode = currentNode->get_sibling();
                tmp = currentNode->get_NodeName();
                printStrings.push_back(tmp);
            }
            //tmp = currentNode->get_NodeName();
            //printStrings.push_back(tmp);
            printfunction(printStrings);

        }

        if (currentNode->get_child() == NULL) {
            currentNode = currentNode->get_sibling();
        }
        else {
            currentNode = currentNode->get_child();
        }
    }

}

int main() {

    string testFile;
    //cout << "enter the name of the test file" << endl;
    //cin >> testFile;
    //change the file you want to test here
    string num;
    cout << "Welcome to programming assignment 6!\n";
    cout << "please enter any number from 1 - 3 to test a file: ";
    cin >> num;
    testFile = "programming_assignment_6-test_file_" + num + ".c";

    ifstream inputfile(testFile);
    if (!inputfile){
        cout << "Error file could not be opened!" << endl;
        exit(1);
    }

    ofstream output("outputfile.txt");
    if (!output.is_open())
        cout << "output file could not open" << endl;

    //simulates being in a certain state
    bool inSingleComment = false;
    bool inBlockComment = false;
    bool inLitteral = false;
    //charecter in get
    char ch;
    char nextCH;

    //count that keeps track of the line number
    int lineNumber = 1;
    vector<int> blockCommentsStack;

    //while loop to read each charecter
    while (inputfile.get(ch)) {
        // hadels if already in a literal like in quotes
        if (inLitteral) {
            if (ch == '\'')
                continue;
            if ((ch) == '\"') {
                output << ch;
                inLitteral = false;
            }
            else{
                output << ch;
            }
        }
        // handeles if already in a block comment
        else if (inBlockComment) {
            if (ch == '*'){
                inputfile.get(nextCH);
                if (nextCH == '/') {
                    output << "  ";
                    inBlockComment = false;
                    blockCommentsStack.pop_back();
                }
                else
                    inputfile.putback(nextCH);
                    output << " ";// << nextCH; fixme
            }
            else if (ch == '\n'){
                output << ch;
                lineNumber++;
            }
            else {
                output << " ";
            }
        }
        //handels when in a sinle line comment state
        else if (inSingleComment) {
            if (ch == '\n') {
                output << ch;
                inSingleComment = false;
                lineNumber++;
            }
            else
                output << " ";
        }

        //handles when not in the other three states
        else  {
            if (ch == '\"') {
                output << "\"";
                inLitteral = true;
            }
            else if (ch == '/'){
                inputfile.get(nextCH);
                if (nextCH == '/'){
                    output << "  ";
                    inSingleComment = true;
                }
                else if (nextCH == '*') {
                    output << "  ";
                    inBlockComment = true;
                    blockCommentsStack.push_back(lineNumber);
                }
                else {
                    inputfile.putback(nextCH);
                    output << ch << nextCH;
                }
            }
            else if (ch == '\n') {
                output << ch;
                lineNumber++;
            }
            else if (ch == '*') {
                inputfile.get(nextCH);
                if (nextCH == '/'){
                    cout <<  "ERROR: Program contains C-style, unterminated comment on line " << lineNumber;
                    exit(2);
                }
                else {
                    inputfile.putback(nextCH);
                    output << ch;
                }
            }
            else
                output << ch;
        }

    }
    if (!blockCommentsStack.empty()) {
        cout << "ERROR: Program contains C-style, unterminated comment on line " << blockCommentsStack.at(0);
        exit(3);
    }

    output.close();
    //start to tokenize

    auto tokenStack = tokenize("outputfile.txt");




    //start of assinment 4
    //auto it = symTable.begin();

    auto symbol = createSymbolTable(tokenStack);
    auto it = symbol.begin();
    string originalList = "tokenlist.txt";
    string newList = "newtokenlist.txt";

    secondTokenList(originalList, newList);

    //start of assignment 5
    ifstream ffs ("newtokenlist.txt");

    if (!ffs) {
        cerr << "outpy file newtokenlist could not be opened";
    }

    //string tokenType;
    string tokenName;
    bool prevWasBrace = false;

    //getline(ffs, tokenType); //fixme
    getline(ffs, tokenName);
    TreeNode *tmpNode = new TreeNode(tokenName);
    tmpNode->sTable = &*it;
    ++it;
    LCRSTree *MyTree = new LCRSTree(tmpNode);


    while (getline(ffs, tokenName)) {
        tmpNode = new TreeNode(tokenName);
        if (prevWasBrace) {
            if (tokenName == ";")
                continue;
            else if (tokenName == "BEGIN BLOCK" || tokenName == "END BLOCK") {
                MyTree->addChild(tmpNode);
                prevWasBrace = true;
                continue;
            }
            else {
                if (tokenName == "DECLARATION") {
                    tmpNode->sTable = &*it;
                    if (it != symbol.end())
                        ++it;
                }
                MyTree->addChild(tmpNode);
                prevWasBrace = false;
                continue;
            }
        }
        if (tokenName == "BEGIN BLOCK" || tokenName == "END BLOCK") {
            MyTree->addChild(tmpNode);
            prevWasBrace = true;
        }
        else if (tokenName == ";") {
            prevWasBrace = true;
        }
        else {
            if (tokenName == "DECLARATION") {
                tmpNode->sTable = &*it;
                if (it != symbol.end())
                    ++it;
            }
            MyTree->addSibling(tmpNode);
            prevWasBrace = false;
        }
    }

    MyTree->printTree();


    //start of part 6

    //we have a full tree with symbol tables
    //plan:
    //separate trees and fill out a map with said trees
    //we need
    //a map
    unordered_map <string, LCRSTree*> treeMap;
    separate_trees(&treeMap, MyTree);

    //cout << "original tree size: " << MyTree->treeSize() << endl;
    //cout << treeMap.size() << endl;
    //for (const auto& pair : treeMap) {
    //    cout << pair.first << " " << pair.second->treeSize() << endl ;  // prints only the keys
    //}

    //we now have the map of trees that are correct
    // we need to call them

    vector<pair<LCRSTree*, TreeNode*>> runningStackOG;
    TreeNode* inTreePosition = treeMap.at("main")->root;
    //cout << inTreePosition << " here";
    runningStackOG.emplace_back(treeMap.at("main"), inTreePosition);

    //calling the evaluator with just main on there to start
    evaluateTree(runningStackOG);
    //cout << runningStackOG.back().second << endl;
    //cout << runningStackOG.back().second->get_child() << endl;
    //cout << "done";
    //cout << treeMap.at("sum_of_first_n_squares")->root->sTable->paramater_list.at(0).datavalue;

    return 0;
}
