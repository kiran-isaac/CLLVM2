#pragma once

#include <memory>
#include <token.h>
#include <string>
#include <fstream>

using std::unique_ptr, std::shared_ptr, std::string;

class Lexer {
public:
 Lexer(std::ifstream &source);
 ~Lexer();

private:
  std::ifstream &source;

  char buf1[4097];
  char buf2[4097];

  char *c;

  unsigned int line;
  unsigned int col;

  unique_ptr<Token> next();

  void refreshBuffer1();
  void refreshBuffer2();

  void advance();
};