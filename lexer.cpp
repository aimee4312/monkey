#include "token.h"
#include <vector>
using namespace std;

// Reads the next char in the file
void readChar(Lexer &l) {
  l.position = l.readPosition;
  if (l.input.get(l.ch)) {
    l.readPosition++;
  } else {
    l.ch = 0;
  }
}

// Checks if char is char
bool isChar(char c) {
  return isalpha(c) || c == '_';
}

// Checks if char is digit
bool isDigit(char c) {
  return isdigit(c);
}

// Reads the file until is not char and returns the string
string readIdent(Lexer &l) {
  string ident = "";
  while (isChar(l.ch)) {
    ident += l.ch;
    readChar(l);
  }
  return ident;
}

// Reads strings
string readStr(Lexer &l) {
  string s = "\"";
  readChar(l);
  while (l.ch != '"' && l.ch != 0) {
    s += l.ch;
    readChar(l);
  }
  if (l.ch == '"') {
    s += l.ch;
    readChar(l);
  }
  return s;
}

// Skips white space
void skipWhiteSpace(Lexer &l) {
  while (l.ch == ' ' || l.ch == '\n' || l.ch == '\t' || l.ch == '\r') {
    readChar(l);
  }
}

string readNum(Lexer &l) {
  string s = "";
  while (isdigit(l.ch)) {
    s += l.ch;
    readChar(l);
  }
  return s;
} 

// Reads next token and tokenizes it
Token nextToken(Lexer &l) {
  Token tok;
  skipWhiteSpace(l);

  switch (l.ch)
  {
  case '=':
    tok.setValues(ASSIGN, "=");
    break;
  case '+':
    tok.setValues(PLUS, "+");
    break;
  case ',':
    tok.setValues(COMMA, ",");
    break;
  case '(':
    tok.setValues(LPAREN, "(");
    break;
  case ')':
    tok.setValues(RPAREN, ")");
    break;
  case '{':
    tok.setValues(LBRACE, "{");
    break;
  case '}':
    tok.setValues(RBRACE, "}");
    break;
  case '"':
    tok.setValues(STRING, readStr(l));
    break;
  default:
    if (isChar(l.ch)) {
      tok.setValues(IDENT, readIdent(l));
    } else if (isdigit(l.ch)) {
      tok.setValues(INT, readNum(l));
    } else {
      tok.setValues(EOF_TYPE, "");
    }
    break;
  }
  readChar(l);
  return tok;
}

// Runs the tokenizer with a initialized lexer and test file
void testNextToken(vector<Test> test) {
  Lexer l("test2.txt");
  readChar(l);
  int count = 0;
  for (const Test& t: test) {
    Token tok = nextToken(l);
    if (t.expectedType != tok.type) {
      throw runtime_error("Token type mismatch for: " + tok.type);
    } 
    if (t.exepectedLiteral != tok.literal) {
      throw runtime_error("Token literal mismatch: " + tok.literal);
    }
    count++;
  }
}

int main() {
  vector<Test> test;
  test.push_back(Test(IDENT, "five"));
  test.push_back(Test(ASSIGN, "="));
  test.push_back(Test(INT, "5"));

  test.push_back(Test(IDENT, "ten"));
  test.push_back(Test(ASSIGN, "="));
  test.push_back(Test(INT, "10"));

  test.push_back(Test(IDENT, "hello"));
  test.push_back(Test(ASSIGN, "="));
  test.push_back(Test(STRING, "\"hello\""));

// test.push_back(Test(DEF, "def"));
// test.push_back(Test(IDENT, "add"));
// test.push_back(Test(LPAREN, "("));
// test.push_back(Test(IDENT, "x"));
// test.push_back(Test(COMMA, ","));
// test.push_back(Test(IDENT, "y"));
// test.push_back(Test(RPAREN, ")"));
// test.push_back(Test(COLON, ":"));
// test.push_back(Test(RETURN, "return"));
// test.push_back(Test(IDENT, "x"));
// test.push_back(Test(PLUS, "+"));
// test.push_back(Test(IDENT, "y"));

// test.push_back(Test(IDENT, "result"));
// test.push_back(Test(ASSIGN, "="));
// test.push_back(Test(IDENT, "add"));
// test.push_back(Test(LPAREN, "("));
// test.push_back(Test(IDENT, "five"));
// test.push_back(Test(COMMA, ","));
// test.push_back(Test(IDENT, "ten"));
// test.push_back(Test(RPAREN, ")"));

  testNextToken(test);
  return 0;
}