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

TEST(Lexer, AllKeywords) {
  string source = "auto break case char const continue default do double else enum extern float for goto if inline int long register restrict return short signed sizeof static struct switch typedef union unsigned void volatile while _Bool _Complex _Imaginary";
  std::istringstream sourceStream(source);
  
  Lexer lexer(sourceStream);
  unique_ptr<CToken> token;
  
  for (int i = 0; i < 32; i++) {
    token = lexer.next();
    ASSERT_EQ(token->type, CTokenType(i));
  }
}

// These need to all be recognized as identifiers
TEST(Lexer, AllKeywordsWithExtraChars) {
  string source = "autoo breakk casee charr constt continuee defaultt doo doublee elsee enumm externn floatt forr gotoo iff inlinee intt longg registerr restricct returnn shortt signedd sizeoff staticc structt switchh typedeff unionn unsignedd voidd volatilee whilee _Booll _Complexx _Imaginaryy";
  std::istringstream sourceStream(source);
  
  Lexer lexer(sourceStream);
  unique_ptr<CToken> token;
  
  for (int i = 0; i < 32; i++) {
    token = lexer.next();
    ASSERT_EQ(token->type, CTokenType::CIdentifier);
  }
}

// These need to all be recognized as identifiers
TEST(Lexer, AllKeywordsWithCharsMissing) {
  string source = "aut brak cas cha cons cont defaul d doubl els enu exter floa fo got i inlin in lo registe restric retur shor signe sizeo stat struc switc typede unio unsigne voi volatil whil _Boo _Compl _Imaginar";
  std::istringstream sourceStream(source);
  
  Lexer lexer(sourceStream);
  unique_ptr<CToken> token;
  
  for (int i = 0; i < 32; i++) {
    token = lexer.next();
    ASSERT_EQ(token->type, CTokenType::CIdentifier);
  }
}