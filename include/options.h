//
// Created by kiran on 3/21/24.
//

#ifndef CLLVM_OPTIONS_H
#define CLLVM_OPTIONS_H

#include <string>
#include <vector>
#include <memory>
#include <filesystem>

static const std::vector<std::string> system_includes = {
  "/usr/local/include",
  "/usr/include/x86_64-linux-gnu",
  "/usr/include"
};

class Options {
public:
  std::vector<std::string> include_dirs = system_includes;
  
  std::string output = "a.out";
  
  std::string input;
  
  bool debug = false;
  
  bool verbose = false;
  
  bool help = false;
  
  bool version = false;
  
  bool emit_ast = false;
};

#endif //CLLVM_OPTIONS_H
