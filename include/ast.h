//
// Created by kiran on 2/24/24.
//

#ifndef CLLVM_AST_H
#define CLLVM_AST_H

#include "token.h"
#include <vector>

class AST {
public:
  virtual ~AST() = default;
  std::vector<unique_ptr<AST>> children;
};

#endif //CLLVM_AST_H
