#include <iostream>
#include <string>
#include <cctype>

using namespace std;

class Parser {
private:
    string expr;
    int pos;

    char peek() {
        return pos < expr.size() ? expr[pos] : '\0';
    }

    char get() {
        return pos < expr.size() ? expr[pos++] : '\0';
    }

    void skipSpaces() {
        while (isspace(peek()))
            get();
    }

    int number() {
        skipSpaces();
        int result = 0;
        while (isdigit(peek())) {
            result = result * 10 + (get() - '0');
        }
        return result;
    }

    int factor() {
        skipSpaces();
        if (peek() == '(') {
            get(); // skip '('
            int result = expression();
            if (get() != ')') {
                throw runtime_error("Expected closing parenthesis");
            }
            return result;
        }
        return number();
    }

    int term() {
        int result = factor();
        while (true) {
            skipSpaces();
            char op = peek();
            if (op == '*' || op == '/') {
                get();
                int rhs = factor();
                if (op == '*') result *= rhs;
                else {
                    if (rhs == 0)
                        throw runtime_error("Division by zero");
                    result /= rhs;
                }
            } else {
                break;
            }
        }
        return result;
    }

    int expression() {
        int result = term();
        while (true) {
            skipSpaces();
            char op = peek();
            if (op == '+' || op == '-') {
                get();
                int rhs = term();
                if (op == '+') result += rhs;
                else result -= rhs;
            } else {
                break;
            }
        }
        return result;
    }

public:
    int evaluate(const string& input) {
        expr = input;
        pos = 0;
        int result = expression();
        skipSpaces();
        if (pos != expr.length()) {
            throw runtime_error("Unexpected characters in expression");
        }
        return result;
    }
};

int main() {
    Parser parser;
    string input;

    cout << "=== SIMPLE COMPILER: EXPRESSION EVALUATOR ===" << endl;
    cout << "Enter an arithmetic expression (e.g. 3 + 4 * (2 - 1)):\n> ";

    getline(cin, input);

    try {
        int result = parser.evaluate(input);
        cout << "Result: " << result << endl;
    } catch (const exception &e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}