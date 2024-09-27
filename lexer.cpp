#include "token.h"
#include <vector>
#include <unordered_map>
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

// look up table for keywords
unordered_map<string, string> keywords = {
  {"def", DEF},
  {"return", RETURN},
  {"if", IF},
  {"else", ELSE},
  {"True", TRUE},
  {"False", FALSE}
};

// use look up table to check for keyword or IDENT
string lookupIdent (string ident) {
  if (keywords.find(ident) != keywords.end()) {
    return keywords[ident];
  }
  return IDENT;
}

// Reads the file until is not char and returns the string
string readIdent(Lexer &l) {
  string ident = "";
  ident += l.ch;
  while (isChar(l.input.peek())) { 
    readChar(l);     
    ident += l.ch;
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

// Checks if next char is =
bool checkEQ(Lexer &l){
  if (l.input.peek() == '='){
    readChar(l);
    return true;
  }
  return false;
}

// Skips white space
void skipWhiteSpace(Lexer &l) {
  while (l.ch == ' ' || l.ch == '\n' || l.ch == '\t' || l.ch == '\r') {
    readChar(l);
  }
}

// Reads digits
string readNum(Lexer &l) {
  string s = "";
  s += l.ch;
  while (isdigit(l.input.peek())) {    
    readChar(l);  
    s += l.ch;
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
    if (checkEQ(l)) {
      tok.setValues(EQ, "==");
    } else {
      tok.setValues(ASSIGN, "=");
    }
    break;

  case '/':
    if(l.input.peek() == '/') {
      readChar(l);
      tok.setValues(DOUBLE_SLASH, "//");
    } else {
      tok.setValues(SLASH, "/");
    }
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
  case ':':
    tok.setValues(COLON, ":");
    break;
  case '!':
    if (checkEQ(l)) {
      tok.setValues(NOT_EQ, "!=");
    } else {
      tok.setValues(BANG, "!");
    }
    break;
  case '-':
    tok.setValues(MINUS, "-");
    break;
  case '*':
    tok.setValues(MULTIPLY, "*");
    break;
  case '>':
    if (checkEQ(l)) {
        tok.setValues(GT_EQ, ">=");
      } else {
        tok.setValues(GT, ">");
    }
    break;
  case '<':
    if (checkEQ(l)) {
        tok.setValues(LT_EQ, "<=");
      } else {
        tok.setValues(LT, "<");
    }
    break;
  case '%':
    tok.setValues(MODULO, "%");
    break;

  default:
    if (isChar(l.ch)) {
      string str = readIdent(l);
      tok.setValues(lookupIdent(str), str);
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
  Lexer l("test4.txt");
  readChar(l);
  int count = 0;
  for (const Test& t: test) {
    Token tok = nextToken(l);
    if (t.expectedType != tok.type) {
      throw runtime_error("Token type mismatch for: " + tok.type + ", expected: " + t.expectedType + "\nToken literal: " + tok.literal + ", expected: " + t.exepectedLiteral);
    } 
    if (t.exepectedLiteral != tok.literal) {
      throw runtime_error("Token literal mismatch: " + tok.literal + ", expected: " + t.exepectedLiteral + "\nToken type: " + tok.type + ", expected: " + t.expectedType);
    }
    count++;
  }
}

int main() {
  vector<Test> test;
  // five = 5
  test.push_back(Test(IDENT, "five"));
  test.push_back(Test(ASSIGN, "="));
  test.push_back(Test(INT, "5"));

  // ten = 10
  test.push_back(Test(IDENT, "ten"));
  test.push_back(Test(ASSIGN, "="));
  test.push_back(Test(INT, "10"));

  // def add(x, y):
  test.push_back(Test(DEF, "def"));
  test.push_back(Test(IDENT, "add"));
  test.push_back(Test(LPAREN, "("));
  test.push_back(Test(IDENT, "x"));
  test.push_back(Test(COMMA, ","));
  test.push_back(Test(IDENT, "y"));
  test.push_back(Test(RPAREN, ")"));
  test.push_back(Test(COLON, ":"));

  // return x + y
  test.push_back(Test(RETURN, "return"));
  test.push_back(Test(IDENT, "x"));
  test.push_back(Test(PLUS, "+"));
  test.push_back(Test(IDENT, "y"));

  // result = add(five, ten)
  test.push_back(Test(IDENT, "result"));
  test.push_back(Test(ASSIGN, "="));
  test.push_back(Test(IDENT, "add"));
  test.push_back(Test(LPAREN, "("));
  test.push_back(Test(IDENT, "five"));
  test.push_back(Test(COMMA, ","));
  test.push_back(Test(IDENT, "ten"));
  test.push_back(Test(RPAREN, ")"));

  // !-/*5
  test.push_back(Test(BANG, "!"));
  test.push_back(Test(MINUS, "-"));
  test.push_back(Test(SLASH, "/"));
  test.push_back(Test(MULTIPLY, "*"));
  test.push_back(Test(INT, "5"));

  // 5 < 10 > 5
  test.push_back(Test(INT, "5"));
  test.push_back(Test(LT, "<"));
  test.push_back(Test(INT, "10"));
  test.push_back(Test(GT, ">"));
  test.push_back(Test(INT, "5"));

  // if 5 < 10:
  test.push_back(Test(IF, "if"));
  test.push_back(Test(INT, "5"));
  test.push_back(Test(LT, "<"));
  test.push_back(Test(INT, "10"));
  test.push_back(Test(COLON, ":"));

  // result_if = True
  test.push_back(Test(IDENT, "result_if"));
  test.push_back(Test(ASSIGN, "="));
  test.push_back(Test(TRUE, "True"));

  // else:
  test.push_back(Test(ELSE, "else"));
  test.push_back(Test(COLON, ":"));

  // result_if = False
  test.push_back(Test(IDENT, "result_if"));
  test.push_back(Test(ASSIGN, "="));
  test.push_back(Test(FALSE, "False"));

  // 10 == 10
  test.push_back(Test(INT, "10"));
  test.push_back(Test(EQ, "=="));
  test.push_back(Test(INT, "10"));

  // 10 != 9
  test.push_back(Test(INT, "10"));
  test.push_back(Test(NOT_EQ, "!="));
  test.push_back(Test(INT, "9"));

  // 10 // 3
  test.push_back(Test(INT, "10"));
  test.push_back(Test(DOUBLE_SLASH, "//"));
  test.push_back(Test(INT, "3"));

  // 10 / 3
  test.push_back(Test(INT, "10"));
  test.push_back(Test(SLASH, "/"));
  test.push_back(Test(INT, "3"));

  // 10 % 3
  test.push_back(Test(INT, "10"));
  test.push_back(Test(MODULO, "%"));
  test.push_back(Test(INT, "3"));

  // 10 <= 3
  test.push_back(Test(INT, "10"));
  test.push_back(Test(LT_EQ, "<="));
  test.push_back(Test(INT, "3"));

  // 10 >= 3
  test.push_back(Test(INT, "10"));
  test.push_back(Test(GT_EQ, ">="));
  test.push_back(Test(INT, "3"));

  testNextToken(test);
  return 0;
}