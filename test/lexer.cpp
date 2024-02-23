#include <gtest/gtest.h>
#include <lexer.h>
#include <filesystem>

static string CWD = std::filesystem::path(__FILE__).parent_path().string();
static string TEST_DIR = CWD + "/test_files/";

TEST(Lexer, TestFileOpenAndRead) {
  std::ifstream source(TEST_DIR + "lex_test.c");

  if (!source.is_open()) {
    std::cerr << "Could not open file" << std::endl;
    FAIL();
  }

  Lexer lexer(source);
}

TEST(Lexer, Keyword) {
  std::string simulatedFileContent = "auto aut autoo";
  std::stringstream simulatedFile(simulatedFileContent);
  
  Lexer lexer(simulatedFile);
  
  ASSERT_EQ(lexer.next()->type, CTokenType::CKeywordAuto);
  ASSERT_EQ(lexer.next()->type, CTokenType::CIdentifier);
  ASSERT_EQ(lexer.next()->type, CTokenType::CIdentifier);
  ASSERT_EQ(lexer.next()->type, CTokenType::CEndOfFile);
}