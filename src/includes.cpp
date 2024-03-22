//
// Created by kiran on 2/28/24.
//
#include <fstream>
#include <iostream>
#include "parser.h"

std::string Parser::find_include(const std::string &filename) {
  if (filename[0] == '/')
    return filename;
  
  string fname = "";
  for (const auto &include : this->options->include_dirs) {
    fname = include + "/" + filename;
    if (std::ifstream f(fname); f.good()) {
      return fname;
    } else if (std::ifstream(fname + ".h"); f.good()) {
      return fname + ".h";
    }
  }
  
  return "";
}

void Parser::parse_include() {
  next();
  
  string filename;
  switch (token->type) {
    case CTokenType::CConstantString:
      // parse user include
      filename = *token->value;
      // remove quotes
      filename = filename.substr(1, filename.size() - 2);
      break;
    case CTokenType::COperatorLess:
      // parse system include
      filename = "";
      while (true) {
        next();
        if (token->type == CTokenType::COperatorGreater) {
          break;
        }
        filename += *token->value;
      }
      
      break;
    default:
      throw std::runtime_error("Invalid include directive");
  }
  
  filename = find_include(filename);
  
  if (filename.empty()) {
    throw std::runtime_error("Could not find include file: " + filename);
  }
  
  check_for_circular_include(filename);
  
  std::cout << "Decending to depth " << lexers.size()  << " with " << filename << std::endl;
  
  std::ifstream stream(filename);
  
  lexers.push_back(std::make_unique<Lexer>((std::istream &) stream, filename));
  next();
}