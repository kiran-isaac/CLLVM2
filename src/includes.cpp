//
// Created by kiran on 2/28/24.
//
#include <fstream>
#include "includes.h"

std::string find_include(const std::string &filename) {
  if (filename[0] == '/')
    return filename;
  
  string fname = "";
  for (const auto &include : system_includes) {
    fname = include + "/" + filename;
    if (std::ifstream f(fname); f.good()) {
      return fname;
    } else if (std::ifstream(fname + ".h"); f.good()) {
      return fname + ".h";
    }
  }
  
  return "";
}

