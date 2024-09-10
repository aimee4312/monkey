#include <iostream>
#include <string>
#include <fstream>

using namespace std;

typedef string TokenType;

struct Token {
  TokenType type;
  string literal;

  Token () {}

  void setValues(TokenType t, string l) {
    type = t;
    literal = l;
  }
};

struct Test {
    TokenType expectedType;
    string exepectedLiteral;

    Test (TokenType t, string l) : expectedType(t), exepectedLiteral(l) {}
};

struct Lexer {
    ifstream input;
    int position;
    int readPosition;
    char ch;

    Lexer (string filename) : position(0), readPosition(0), ch(0) {
        input.open(filename);
        if (!input.is_open()) {
            cerr << "Error opening file" << endl;
        }
    }

    ~Lexer() {
        if (input.is_open()) {
            input.close();
        }
    }
};

const string ASSIGN = "=";
const string PLUS = "+";
const string COMMA = ",";
const string LPAREN = "(";
const string RPAREN = ")";
const string LBRACE = "{";
const string RBRACE = "}";
const string EOF_TYPE = "";