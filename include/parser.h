//
// Created by kiran on 2/24/24.
//

#ifndef CLLVM_PARSER_H
#define CLLVM_PARSER_H

#include "token.h"
#include "lexer.h"
#include "options.h"

#include <stack>
#include <memory>
#include <utility>
#include <ast.h>

using std::unique_ptr, std::shared_ptr;

class Parser {
private:
  shared_ptr<Options> options;
  // stack of lexers to handle nested includes
  std::vector<unique_ptr<Lexer>> lexers;
  
  Lexer *current_lexer;
  
  void next();
  
  unique_ptr<CToken> token;
  
public:
  explicit Parser(std::istream &source, shared_ptr<Options> options) : options(std::move(options)){
    lexers.push_back(std::make_unique<Lexer>(source));
    current_lexer = lexers.back().get();
    next();
  }
  
  unique_ptr<AST> parse();
  
  unique_ptr<AST> parse_preprocessor();
  
  void parse_include();
  
  std::string find_include(const std::string &filename);
  
  void check_for_circular_include(const string &filename);
};

#endif //CLLVM_PARSER_H
