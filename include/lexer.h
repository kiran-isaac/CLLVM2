#pragma once

#include <memory>
#include <token.h>
#include <string>
#include <fstream>
#include <optional>


using std::unique_ptr, std::shared_ptr, std::string;

class Lexer {
public:
  explicit Lexer(std::istream &source, string filename = "");
  
  unique_ptr<CToken> next();

  string filename;
private:
  std::istream *source;

  char buf1[4097]{};
  char buf2[4097]{};

  char *c;
  string lexerError;

  unsigned int line;
  unsigned int col;
  
  bool isBuffer1;
  
  void refreshBuffer1();
  void refreshBuffer2();

  void advance();
  
  unique_ptr<CToken> lex_num();
  
  unique_ptr<CToken> lex_char();
  
  unique_ptr<CToken> lex_string();
  
  std::optional<char> lex_escape_character();
  
  unique_ptr<CToken> lex_word();
  
  unique_ptr<CToken> lex_preprocessor();
};