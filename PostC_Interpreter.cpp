/*                            
                          ,ad8888ba,   
    aa          aa       d8"'    `"8b  
    88          88      d8'            
aaaa88aaaa  aaaa88aaaa  88             
""""88""""  """"88""""  88             
    88          88      Y8,            
    ""          ""       Y8a.    .a8P  
                          `"Y8888Y"'   
                                       */
// ++C (AKA PostC)
// A basic Postfix Scripting Language Lexer + Interpreter
// Hudson Liu

#include <iostream>
#include <stack>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <map>
#include <string>

using namespace std;

class Interpreter{
    public:
        //Constructors
        Interpreter();
        Interpreter(string fileContents);

        //Remove comments
        void removeComments();

        //Parses the string into a set of identifiers
        void parseFile();

        //Main loop, loops through each line of file and translates + runs each line
        void identifierIterator();
    private:
        string fileContents;
        vector<string> parsed;

        vector<int> integerVars; //i
        vector<string> stringVars; //s
        vector<bool> boolVars; //b

        stack<string> operands;
        map<string, pair<char, int>> variableKey; /// string is variable name, char is datatype, int is index

        //A bunch of switch cases that determines what function the identifier should be handled by
        void identifierHandoff(string identifier);

        //Detects whether an identifier is a variable or not
        bool isVariableName(string identifier);

        //Access variable by identifier, and return the proper type; throws error if variable is not correct type 
        int getInt(string variableName);
        string getString(string variableName);
        bool getBool(string variableName);

        //Quick conversion from char abbreviation to full datatype name, used for getInt, getString, and getBool error messages
        string charToDatatype(char datatype);

        //Convert \n's to actual newline characters; directly edits original string
        string formatString(string unformatted);

        //If a "string" variable declaration is detected, the last operand will be added to the variableKey list as a string 
        void stringDeclarationHandler();

        //If an "int" variable declaration is detected, the last operand will be added to the variableKey list as an int
        void intDeclarationHandler();

        //If a "bool" variable declaration is detected, the last operand will be added to the variableKey list as a bool
        void boolDeclarationHandler();

        //If a stream insertion operator << is detected, the second-to-last operand on the stack will be inserted into the last operand 
        void streamInsertionHandler();

        //If a stream extraction operator >> is detected, the second-to-last operand on the stack will be given the input of the last operand
        void streamExtractionHandler();

        //If an assignment operator = is detected, the rvalue will be assigned to the lvalue
        void assignmentHandler();

        //If an arithmetic operator is detected, perform the operation
        void arithmeticHandler(string identifier);

        //If a semicolon is detected, clear the stack
        void semicolonHandler();
};

int main(){

    //Open file
    string filename;
    cout << "Name of file: ";
    cin >> filename;
    ifstream fin;
    fin.open(filename);
    if (fin.fail()){
        cout << "Input file opening failed";
        exit(1);
    }

    //Get all lines and store it in a big string
    string program;
    while(!fin.eof()){
        char character = fin.get();
        if (character != '\n'){
            program = program + character;
        }
    }

    //Runs the Interpreter
    Interpreter interpret(program);
    interpret.parseFile();
    interpret.identifierIterator();

    return 0;
}

Interpreter::Interpreter(){
    cout << "invalid Argument to Interpreter Class; constructor arguments must not be empty" << endl;
    exit(1);
}

Interpreter::Interpreter(string fileContents){
    Interpreter::fileContents = fileContents;
}

void Interpreter::removeComments(){
    //Loops until the end of the program
    int cursor = 1, size = 1;
    char currentChar = ' ', prevChar = 'L';
    bool commentClearance = false;
    string cleaned;
    while (cursor < fileContents.length()){
        //Updates currentChar
        currentChar = fileContents[cursor];
        prevChar = fileContents[cursor - 1];

        //Detect comments
        if (currentChar != '/' || prevChar != '/'){
            cleaned += prevChar;
            cursor++;
        }
        else{
            //Clear cursor from any additional /'s attached to the string
            cursor++;
            //Move past comment
            do{
                cursor++;
                currentChar = fileContents[cursor];
                prevChar = fileContents[cursor - 1];
            } while(currentChar != '/' || prevChar != '/');
            //Clear the two end //'ss
            cursor+=2;
        }
    }
    //Gets last character only if the last characters weren't a comment
    if (cursor == fileContents.length() + 1){
        currentChar = fileContents[cursor];
        cleaned += currentChar;
    }
    fileContents = cleaned;
}

void Interpreter::parseFile(){
    //Removes Comments
    removeComments();
    
    //Loops until the end of a statement
    int cursor = 0, size = 1;
    char currentChar = ' ', prevChar = 'L';
    while (cursor < fileContents.length()){
        //Updates currentChar
        currentChar = fileContents[cursor];
        if (cursor > 0){
            prevChar = fileContents[cursor - 1];
        }

        //If the current character is a quote, skip to the end of it, then make that whole quote one big identifier
        if(currentChar == '\"'){
            int starter = cursor;
            int sizeStr = 1;
            do {
                cursor++;
                sizeStr++;
                currentChar = fileContents[cursor];
            } while (currentChar != '\"');
            cursor+=2;
            string substring = fileContents.substr(starter, sizeStr);
            parsed.push_back(substring);
        }
        //If the current character is a whitespace, add the past word to the list as a substring, unless there was an accidental double space
        else if (currentChar == ' ' && prevChar != ' '){
            //Add the parsed substring to the vector, and reset size and update the cursor position
            string substring = fileContents.substr((cursor - size) + 1, size - 1);
            parsed.push_back(substring);
            size = 1;
            
            //Ensure the cursor position is past any additional spaces
            currentChar = fileContents[cursor];
            while (currentChar == ' '){
                cursor++;
                currentChar = fileContents[cursor];
            }
        }
        //If the current character is a semicolon, adds the current identifier and the semicolon to parsed, then skip past the semicolon
        else if (currentChar == ';'){
            string substring = fileContents.substr((cursor - size) + 1, size - 1);
            parsed.push_back(substring);
            string semicolon = fileContents.substr(cursor, 1);
            parsed.push_back(semicolon);
            cursor++;
            size = 1;
        }
        //If it isn't any special case, then iterate it as normal
        else{
            cursor++;
            size++;
        }
    }
}

void Interpreter::identifierIterator(){
    //Iterates over all the identifiers in the "parsed" list
    for (string identifier : parsed){
        identifierHandoff(identifier);
    }
}

void Interpreter::identifierHandoff(string identifier){
    //Detects all operators and performs relevant operations; if not operator it will just append the value to operands
    if (identifier == "<<"){
        streamInsertionHandler();
    }
    else if (identifier == "int"){
        intDeclarationHandler();
    }
    else if (identifier == "string"){
        stringDeclarationHandler();
    }
    else if (identifier == "bool"){
        boolDeclarationHandler();
    }
    else if (identifier == ">>"){
        streamExtractionHandler();
    }
    else if (identifier == "="){
        assignmentHandler();
    }
    else if (identifier == "+" || identifier == "-" || identifier == "*" || identifier == "/"){
        arithmeticHandler(identifier);
    }
    else if (identifier == ";"){
        semicolonHandler();
    }
    else {
        operands.push(identifier);
    }
}
void Interpreter::streamInsertionHandler(){
    //Pull the two relevant operators off the stack
    string outputStream = operands.top();
    operands.pop();
    string text = operands.top();
    operands.pop();
    //Check for a valid rvalue
    if (outputStream == "cout"){
        //Find the appropriate action to take based off of the lvalue
        if (text[0] == '\"' && text[text.length() - 1] == '\"'){
            string noQuotes = text.substr(1, text.length() - 2);
            cout << formatString(noQuotes);
        }
        else if (isVariableName(text)){
            pair<char, int> pairing = variableKey.at(text);
            string returnText;
            switch(pairing.first){
                case 'i':
                    returnText = to_string(getInt(text));
                    break;
                case 's':
                    returnText = getString(text);
                    break;
                case 'b':
                    returnText = to_string(getBool(text));
                    break;
            }
            cout << returnText;
        }
        else if (text == "endl"){
            cout << "\n";
        }
        else{
            cout << "++C ERROR: Stream insertion operator lvalue expected string or modifier, received: \"" << text << "\"\n";
            exit(1);
        }
        //"<<" operator returns cout
        operands.push("cout");
    }
    else{
        cout << "++C ERROR: Stream insertion operator rvalue expected \"cout\", received: \"" << outputStream << "\"\n";
        exit(1);
    }
}

void Interpreter::streamExtractionHandler(){
    //Pull the two relevant operators off the stack
    string inputStream = operands.top();
    operands.pop();
    string varName = operands.top();
    operands.pop();
    //Check for a valid rvalue
    if (inputStream == "cin"){
        //Find the appropriate action to take based off of the lvalue
        if (isVariableName(varName)){
            pair<char, int> pairing = variableKey.at(varName);
            char datatype = pairing.first;
            switch (datatype){
                case 'i':
                    cin >> integerVars[pairing.second];
                    break;
                case 's':
                    cin >> stringVars[pairing.second];
                    break;
                default:
                    cout << "++C ERROR: Stream extraction operator lvalue expected datatype string or int, received: \"" 
                        << charToDatatype(datatype) << "\"\n";
                    exit(1);
            }
        }
        else{
            cout << "++C ERROR: Stream extraction operator lvalue expected variable, received: \"" << varName << "\"\n";
            exit(1);
        }
        // ">>" operator returns cin
        operands.push("cin");
    }
    else{
        cout << "++C ERROR: Stream extraction operator rvalue expected \"cin\", received: \"" << inputStream << "\"\n";
        exit(1);
    }
}

void Interpreter::assignmentHandler(){
    //Finds the name of the variable and the corresponding variable type
    string valStr = operands.top();
    operands.pop();
    string varName = operands.top();
    operands.pop();
    pair<char, int> pairing = variableKey.at(varName);
    char datatype = pairing.first;
    //Reads the assignment value appropriately
    switch (datatype){
        case 'i':
            integerVars[pairing.second] = stoi(valStr);
            break;
        case 's':
            stringVars[pairing.second] = formatString(valStr.substr(1, valStr.length() - 2));
            break;
        case 'b':
            boolVars[pairing.second] = (valStr == "true");
            break;
    }
    //Adds back the rvalue, since "=" returns the value that was assigned
    operands.push(valStr);
}

void Interpreter::intDeclarationHandler(){
    //Doesn't pop the stack since variable declaration returns the variable
    string varName = operands.top();
    pair<char, int> keyPair = pair<char,int>('i', integerVars.size());
    variableKey.insert({varName, keyPair});
    integerVars.push_back(0);
}

void Interpreter::stringDeclarationHandler(){
    //Doesn't pop the stack since variable declaration returns the variable
    string varName = operands.top();
    pair<char, int> keyPair = pair<char,int>('s', stringVars.size());
    variableKey.insert({varName, keyPair});
    stringVars.push_back("");
}

void Interpreter::boolDeclarationHandler(){
    //Doesn't pop the stack since variable declaration returns the variable
    string varName = operands.top();
    pair<char, int> keyPair = pair<char,int>('b', boolVars.size());
    variableKey.insert({varName, keyPair});
    boolVars.push_back(false);
}

int Interpreter::getInt(string variableName){
    pair<char, int> pairing = variableKey.at(variableName);
    char datatype = pairing.first;
    if (datatype != 'i'){
        cout << "++C ERROR: Operator expected int, received: \"" << charToDatatype(datatype) << "\"\n";
        exit(1);
    }
    int index = pairing.second;
    return integerVars[index];
}

string Interpreter::getString(string variableName){
    pair<char, int> pairing = variableKey.at(variableName);
    char datatype = pairing.first;
    if (datatype != 's'){
        cout << "++C ERROR: Operator expected string, received: \"" << charToDatatype(datatype) << "\"\n";
        exit(1);
    }
    int index = pairing.second;
    return stringVars[index];
}

bool Interpreter::getBool(string variableName){
    pair<char, int> pairing = variableKey.at(variableName);
    char datatype = pairing.first;
    if (datatype != 'b'){
        cout << "++C ERROR: Operator expected bool, received: \"" << charToDatatype(datatype) << "\"\n";
        exit(1);
    }
    int index = pairing.second;
    return integerVars[index];
}

bool Interpreter::isVariableName(string identifier){
    return variableKey.find(identifier) != variableKey.end();
}

string Interpreter::charToDatatype(char datatype){
    switch (datatype){
        case 'i':
            return "int";
        case 's':
            return "string";
        case 'b':
            return "bool";
        default:
            cout << "++C ERROR: Internal error; \"charToDatatype()\" expected datatype, received: \"" << datatype << "\"\n";
            exit(1);
    }
}

string Interpreter::formatString(string unformatted){
    for (int cursor = 0; cursor < unformatted.length() - 1; cursor++){
        string substring = unformatted.substr(cursor, 2);
        if (substring == "\\n"){
            unformatted.replace(cursor, 2, "\n");
        }
        else if (substring == "\\t"){
            unformatted.replace(cursor, 2, "\t");
        }
    }
    return unformatted;
}

void Interpreter::arithmeticHandler(string identifier){
    //Get operands from stack
    string val1 = operands.top();
    operands.pop();
    string val2 = operands.top();
    operands.pop();
    //Find the variable values of val1 & val2 if they are variables
    int num1, num2;
    if (isVariableName(val1)){
        num1 = getInt(val1);
    }
    else{
        num1 = stoi(val1);
    }
    if (isVariableName(val2)){
        num2 = getInt(val2);
    }
    else{
        num2 = stoi(val2);
    }
    //Perform the correct operation for the operator
    switch (identifier[0]){
        case '+':
            operands.push(to_string(num1 + num2));
            break;
        case '-':
            operands.push(to_string(num1 - num2));
            break;
        case '*':
            operands.push(to_string(num1 * num2));
            break;
        case '/':
            operands.push(to_string(num1 / num2));
            break;
    }
}

void Interpreter::semicolonHandler(){
    //Clear stack
    while (!operands.empty()){
        operands.pop();
    }
}