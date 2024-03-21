//
// Created by kiran on 2/24/24.
//

#ifndef CLLVM_PARSER_H
#define CLLVM_PARSER_H

#include "token.h"
#include "lexer.h"
#include "includes.h"

#include <stack>
#include <memory>
#include <ast.h>

using std::unique_ptr;

class Parser {
private:
  // stack of lexers to handle nested includes
  std::stack<unique_ptr<Lexer>> lexers;
  
  void next();
  
  unique_ptr<CToken> token;
  
public:
  explicit Parser(std::istream &source) {
    lexers.push(std::make_unique<Lexer>(source));
    next();
  }
  
  unique_ptr<AST> parse();
  
  unique_ptr<AST> parse_preprocessor();
  
  void parse_include();
};


#endif //CLLVM_PARSER_H
