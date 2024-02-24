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

TEST(Lexer, Punctuation) {
  string source = "(){}[],;:?\\.->...";
  std::istringstream sourceStream(source);
  
  Lexer lexer(sourceStream);
  unique_ptr<CToken> token;
  
  ASSERT_EQ(lexer.next()->type, CTokenType::CPunctuationOpenParen);
  ASSERT_EQ(lexer.next()->type, CTokenType::CPunctuationCloseParen);
  ASSERT_EQ(lexer.next()->type, CTokenType::CPunctuationOpenBrace);
  ASSERT_EQ(lexer.next()->type, CTokenType::CPunctuationCloseBrace);
  ASSERT_EQ(lexer.next()->type, CTokenType::CPunctuationOpenBracket);
  ASSERT_EQ(lexer.next()->type, CTokenType::CPunctuationCloseBracket);
  ASSERT_EQ(lexer.next()->type, CTokenType::CPunctuationComma);
  ASSERT_EQ(lexer.next()->type, CTokenType::CPunctuationSemicolon);
  ASSERT_EQ(lexer.next()->type, CTokenType::CPunctuationColon);
  ASSERT_EQ(lexer.next()->type, CTokenType::CPunctuationQuestionMark);
  ASSERT_EQ(lexer.next()->type, CTokenType::CPunctuationBackslash);
  ASSERT_EQ(lexer.next()->type, CTokenType::CPunctuationDot);
  ASSERT_EQ(lexer.next()->type, CTokenType::CPunctuationArrow);
  ASSERT_EQ(lexer.next()->type, CTokenType::CPunctuationEllipsis);
}

TEST(Lexer, Operators) {
  string source = ">=<=&&||!&|^~<<>> =+=-=*=/=%=&=|=^=<<=>>=";
  std::istringstream sourceStream(source);
  
  Lexer lexer(sourceStream);
  unique_ptr<CToken> token;
  
  ASSERT_EQ(lexer.next()->type, CTokenType::COperatorGreaterEqual);
  ASSERT_EQ(lexer.next()->type, CTokenType::COperatorLessEqual);
  ASSERT_EQ(lexer.next()->type, CTokenType::COperatorAnd);
  ASSERT_EQ(lexer.next()->type, CTokenType::COperatorOr);
  ASSERT_EQ(lexer.next()->type, CTokenType::COperatorNot);
  ASSERT_EQ(lexer.next()->type, CTokenType::COperatorBitwiseAnd);
  ASSERT_EQ(lexer.next()->type, CTokenType::COperatorBitwiseOr);
  ASSERT_EQ(lexer.next()->type, CTokenType::COperatorBitwiseXor);
  ASSERT_EQ(lexer.next()->type, CTokenType::COperatorBitwiseNot);
  ASSERT_EQ(lexer.next()->type, CTokenType::COperatorLeftShift);
  ASSERT_EQ(lexer.next()->type, CTokenType::COperatorRightShift);
  ASSERT_EQ(lexer.next()->type, CTokenType::COperatorAssignment);
  ASSERT_EQ(lexer.next()->type, CTokenType::COperatorPlusAssign);
  ASSERT_EQ(lexer.next()->type, CTokenType::COperatorMinusAssign);
  ASSERT_EQ(lexer.next()->type, CTokenType::COperatorMultiplyAssign);
  ASSERT_EQ(lexer.next()->type, CTokenType::COperatorDivideAssign);
  ASSERT_EQ(lexer.next()->type, CTokenType::COperatorModuloAssign);
  ASSERT_EQ(lexer.next()->type, CTokenType::COperatorAndAssign);
  ASSERT_EQ(lexer.next()->type, CTokenType::COperatorOrAssign);
  ASSERT_EQ(lexer.next()->type, CTokenType::COperatorXorAssign);
  ASSERT_EQ(lexer.next()->type, CTokenType::COperatorLeftShiftAssign);
  ASSERT_EQ(lexer.next()->type, CTokenType::COperatorRightShiftAssign);
}

TEST(Lexer, Preprocessor) {
  string source = "#define #include #if #ifdef #ifndef #else #elif #endif #undef #line #error #pragma ##";
  std::istringstream sourceStream(source);
  
  Lexer lexer(sourceStream);
  unique_ptr<CToken> token;
  
  ASSERT_EQ(lexer.next()->type, CTokenType::CPreprocessorDefine);
  ASSERT_EQ(lexer.next()->type, CTokenType::CPreprocessorInclude);
  ASSERT_EQ(lexer.next()->type, CTokenType::CPreprocessorIf);
  ASSERT_EQ(lexer.next()->type, CTokenType::CPreprocessorIfdef);
  ASSERT_EQ(lexer.next()->type, CTokenType::CPreprocessorIfndef);
  ASSERT_EQ(lexer.next()->type, CTokenType::CPreprocessorElse);
  ASSERT_EQ(lexer.next()->type, CTokenType::CPreprocessorElif);
  ASSERT_EQ(lexer.next()->type, CTokenType::CPreprocessorEndif);
  ASSERT_EQ(lexer.next()->type, CTokenType::CPreprocessorUndef);
  ASSERT_EQ(lexer.next()->type, CTokenType::CPreprocessorLine);
  ASSERT_EQ(lexer.next()->type, CTokenType::CPreprocessorError);
  ASSERT_EQ(lexer.next()->type, CTokenType::CPreprocessorPragma);
  ASSERT_EQ(lexer.next()->type, CTokenType::CPreprocessorHashHash);
}

TEST(Lexer, Integers) {
  string source = "0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15";
  std::istringstream sourceStream(source);
  
  Lexer lexer(sourceStream);
  unique_ptr<CToken> token;
  
  for (int i = 0; i < 16; i++) {
    token = lexer.next();
    ASSERT_EQ(token->type, CTokenType::CConstantInteger);
  }
}

TEST(Lexer, Floats) {
  string source = "0.0 1.0 2.0 3.0 4.0 5.0 6.0 7.0 8.0 9.0 10.0 11.0 12.0 13.0 14.0 15.0";
  std::istringstream sourceStream(source);
  
  Lexer lexer(sourceStream);
  unique_ptr<CToken> token;
  
  for (int i = 0; i < 16; i++) {
    token = lexer.next();
    ASSERT_EQ(token->type, CTokenType::CConstantFloat);
  }
}

TEST(Lexer, Character) {
  string source = "'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r' 's' 't' 'u' 'v' 'w' 'x' 'y' 'z' 'A' 'B' 'C' 'D' 'E' 'F' 'G' 'H' 'I' 'J' 'K' 'L' 'M' 'N' 'O' 'P' 'Q' 'R' 'S' 'T' 'U' 'V' 'W' 'X' 'Y' 'Z' '0' '1' '2' '3' '4' '5' '6' '7' '8' '9' ' '";
  std::istringstream sourceStream(source);
  
  Lexer lexer(sourceStream);
  unique_ptr<CToken> token;
  
  for (int i = 0; i < 62; i++) {
    token = lexer.next();
    ASSERT_EQ(token->type, CTokenType::CConstantChar);
  }
}

TEST(Lexer, EscapeChar) {
  string source = R"('\xff' '\xf' '\021' '\377' '\7' '\n' '\t' '\r' '\0' '\'' '\"' '\\' )";
  std::istringstream sourceStream(source);
  
  Lexer lexer(sourceStream);
  unique_ptr<CToken> token;
  
  for (int i = 0; i < 12; i++) {
    token = lexer.next();
    ASSERT_EQ(token->type, CTokenType::CConstantChar);
  }
}

TEST(Lexer, EscapeCharInString) {
  string source = R"("\xff" "\xf" "\021" "\377" "\7" "\n" "\t" "\r" "\0" "\'" "\"")";
  std::istringstream sourceStream(source);
  
  Lexer lexer(sourceStream);
  unique_ptr<CToken> token;
  
  for (int i = 0; i < 10; i++) {
    token = lexer.next();
    ASSERT_EQ(token->type, CTokenType::CConstantString);
  }
}