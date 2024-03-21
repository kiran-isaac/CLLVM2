//
// Created by kiran on 2/28/24.
//

#ifndef CLLVM_INCLUDES_H
#define CLLVM_INCLUDES_H

#include <vector>
#include <memory>
#include <string>

using std::string;

static const std::vector<std::string> system_includes = {
  "/usr/local/include",
  "/usr/include/x86_64-linux-gnu",
  "/usr/include",
};

std::string find_include(const std::string &filename);

#endif //CLLVM_INCLUDES_H
