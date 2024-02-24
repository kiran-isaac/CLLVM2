#pragma once

#include <memory>
#include <token.h>
#include <string>
#include <fstream>

using std::unique_ptr, std::shared_ptr, std::string;

class Lexer {
public:
  explicit Lexer(std::istream &source);
 
  unique_ptr<CToken> next();

private:
  std::istream &source;

  char buf1[4097]{};
  char buf2[4097]{};

  char *c;
  string lexerError;

  unsigned int line;
  unsigned int col;
  
  void refreshBuffer1();
  void refreshBuffer2();

  void advance();
  
  unique_ptr<CToken> parse_num();
  
  unique_ptr<CToken> parse_char();
  
  unique_ptr<CToken> parse_string();
  
  char parse_escape_character();
  
  unique_ptr<CToken> parse_word();
  
  unique_ptr<CToken> parse_keyword();
  
  bool is_keyword(const string &str);
};