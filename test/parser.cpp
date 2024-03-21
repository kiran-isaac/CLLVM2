//
// Created by kiran on 2/28/24.
//

#include <gtest/gtest.h>
#include <options.h>
#include <parser.h>
#include <filesystem>
#include <memory>

using std::string, std::make_unique, std::istringstream, std::unique_ptr;

string cwd = std::filesystem::current_path().parent_path().string() + "/test/test_files";

TEST(Parser, Include) {
  auto options = std::make_shared<Options>();
  
  options->include_dirs.push_back(cwd);
  
  std::string source = "#include \"include_test.h\"";

  std::istringstream sourceStream(source);

  std::unique_ptr<std::istream> stream = std::make_unique<std::istringstream>(source);
  
  Parser parser(stream, options);
  auto ast = parser.parse();
}