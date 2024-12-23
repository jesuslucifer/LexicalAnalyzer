#include <iostream>

#include <fstream>
#include <string>

template <typename T>
class List
{
private:
    template<typename U>
    class Node
    {
    public:
        T value;
        T value2;
        Node *next;

        Node(T value, T value2, Node* next = nullptr)
        {
            this->value = value;
            this->value2 = value2;
            this->next = next;
        }

        Node(char value, Node* next = nullptr)
        {
            this->value = value;
            this->next = next;
        }


    };

    Node<T>* head;

public:

    List()
    {
        head = nullptr;
    }

    void writeInFile(std::ofstream &out)
    {
        auto current = head;

        while (current != nullptr)
        {
            out << current->value << " " << current->value2 << "\n";
            current = current->next;
        }
    }

    void insert(char value)
    {
        if (head == nullptr)
        {
            head = new Node<T>(value);
        }
        else
        {
            auto current = head;

            while (current->next != nullptr)
            {
                current = current->next;
            }

            current->next = new Node<T>(value);
        }
    }

    void insert(std::string value, std::string value2)
    {
        if (head == nullptr)
        {
            head = new Node<T>(value, value2);
        }
        else
        {
            auto current = head;

            while (current->next != nullptr)
            {
                current = current->next;
            }

            current->next = new Node<T>(value, value2);
        }
    }

    void print()
    {
        auto current = head;

        std::cout << "List: " << "\n";
        while (current != nullptr)
        {
            std::cout << current->value << " : " << current->value2 << "\n";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

const std::string KEYWORDS[] = {"program", "Program", "var", "integer", "if", "else", "end", "begin", "then", "const", "and", "or", "not"};
const char OPERATORS[] = {'+', '-', '*', '/', '=', '<', '>', '(', ')', ':'};
const std::string DOUBLE_OPERATORS[] = {":=", "<>", "<=", ">="};
const char SEPARATORS[] = {';', ',', '.'};
const char DELIMITERS[] = {'\n', '\t', ' '};

bool isKeyword(std::string word) {
    for (const std::string keyword : KEYWORDS) {
        if (keyword == word) {
            return true;
        }
    }
    return false;
}

bool isOperator(char op) {
    for (char opIter : OPERATORS) {
        if (opIter == op) {
            return true;
        }
    }
    return false;
}

bool isDoubleOperator(char firstOp, char secondOp) {
    std::string doubleOperator;
    doubleOperator += firstOp;
    doubleOperator += secondOp;
    for (std::string doubleOperatorIter : DOUBLE_OPERATORS) {
        if (doubleOperatorIter == doubleOperator) {
            return true;
        }
    }
    return false;
}

bool isSeparator(char sep) {
    for (char sepIter : SEPARATORS) {
        if (sepIter == sep) {
            return true;
        }
    }
    return false;
}

bool isDelimiter(char delimiter) {
    for (char delimiterIter : DELIMITERS) {
        if (delimiterIter == delimiter) {
            return true;
        }
    }
    return false;
}

int lexer(std::ifstream &in, List<std::string> list) {
    char ch;
    std::string buffer;
    std::string errorStr;
    bool errorFlag = false;

    while (in.get(ch)) {
        if (isalpha(ch) || ch == '_') {
            do {
                buffer += ch;
                in.get(ch);
            } while ((isalpha(ch) || ch == '_' || isdigit(ch)) && !in.eof());

            if (!isDelimiter(ch) && !isOperator(ch) && !isSeparator(ch)) {
                errorStr += "Invalid identifier: ";
                errorStr += ch;
                errorFlag = true;
            }

            if (isKeyword(buffer)) {
                if (buffer == "and" || buffer == "or" || buffer == "not") {
                    list.insert(buffer, "OPERATOR");
                    buffer.clear();
                } else {
                    list.insert(buffer, "KEYWORD");
                    buffer.clear();
                }
            } else {
                list.insert(buffer, "IDENTIFIER");
                buffer.clear();
            }
        }

        if (isdigit(ch)) {
            do {
                buffer += ch;
                in.get(ch);
            } while (isdigit(ch) && !in.eof());

            if (!isDelimiter(ch) && !isOperator(ch) && !isSeparator(ch)) {
                errorStr += "Invalid digit: ";
                errorStr += ch;
                errorFlag = true;
            }

            list.insert(buffer, "DIGIT");
            buffer.clear();
        }

        if (isDoubleOperator(ch, in.peek())) {
            buffer += ch;
            buffer += in.get();
            list.insert(buffer, "OPERATOR");
            buffer.clear();
        }

        if (isOperator(ch)) {
            buffer += ch;
            list.insert(buffer, "OPERATOR");
            buffer.clear();
        }

        if (isSeparator(ch)) {
            buffer += ch;
            list.insert(buffer, "SEPARATOR");
            buffer.clear();
        }

        if (!isDelimiter(ch) && !isOperator(ch) && !isSeparator(ch) && errorStr.empty()) {
            errorStr += "Invalid symbol: ";
            errorStr += ch;
            errorFlag = true;
        }

        if (!errorStr.empty()) {
            std::cout << "ERROR " << errorStr << std::endl;
            errorStr.clear();
        }
    }

    std::ofstream out("D:\\LexicalAnalyzer\\output.txt");

    if (errorFlag) {
        out << "error";
        return 1;
    }

    list.writeInFile(out);
    list.print();
    return 0;
}

int main() {
    List<std::string> list;
    std::ifstream in("D:\\LexicalAnalyzer\\test.txt");

    if (!in.is_open()) {
        std::cerr << "Error in opening file" << std::endl;
    }

    lexer(in, list);
    in.close();
    return 0;
}
