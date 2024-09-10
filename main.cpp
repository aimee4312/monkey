#include "token.h"
#include <vector>
using namespace std;

void readChar(Lexer &l) {
  l.position = l.readPosition;
  if (l.input.get(l.ch)) {
    l.readPosition++;
  } else {
    l.ch = 0;
  }
}

Token nextToken(Lexer &l) {
  Token tok;

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
  default:
    tok.setValues(EOF_TYPE, "");
    break;
  }
  readChar(l);
  return tok;
}


void testNextToken(vector<Test> test) {
  Lexer l("test.txt");
  readChar(l);
  int count = 0;
  for (const Test& t: test) {
    Token tok = nextToken(l);
    if (t.expectedType != tok.type) {
      throw runtime_error("Token type mismatch for: " + tok.type);
    } 
    if (t.exepectedLiteral != tok.literal) {
      throw runtime_error("Token literal mismatch" + tok.literal);
    }
    count++;
  }
}

int main() {
  vector<Test> test;
  test.push_back(Test(ASSIGN, "="));
  test.push_back(Test(PLUS, "+"));
  test.push_back(Test(COMMA, ","));
  test.push_back(Test(LPAREN, "("));
  test.push_back(Test(RPAREN, ")"));
  test.push_back(Test(LBRACE, "{"));
  test.push_back(Test(RBRACE, "}"));

  testNextToken(test);
  return 0;
}