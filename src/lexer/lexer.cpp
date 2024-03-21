#include "lexer.h"
#include <iostream>
#include <algorithm>

using std::unique_ptr, std::shared_ptr, std::string;

#define isIdChar(c) isalnum(*c) || *c == '_' || isdigit(*c)

void Lexer::refreshBuffer1() {
  source.read(buf1, 4096);
  if (source.eof()) {
    buf1[source.gcount()] = EOF;
  }
}

void Lexer::refreshBuffer2() {
  source.read(buf2, 4096);
  if (source.eof()) {
    buf2[source.gcount()] = EOF;
  }
}

void Lexer::advance() {
  if (c == buf1 + 4095) {
    refreshBuffer2();
    c = buf2;
    isBuffer1 = false;
  } else if (c == buf2 + 4095) {
    refreshBuffer1();
    c = buf1;
    isBuffer1 = true;
  } else {
    c++;
  }
  col++;
}

Lexer::Lexer(std::istream &source) : source(source) {
  // put eof at the end of buf1 and buf2
  std::fill_n(buf1, 4096, EOF);
  std::fill_n(buf2, 4096, EOF);
  
  buf1[4096] = EOF;
  buf2[4096] = EOF;
  refreshBuffer1();
  
  c = buf1;
  line = 1;
  col = 1;
}

std::optional<char> Lexer::lex_escape_character() {
  int val = 0;
  int i = 0; // counter for octal escape sequences
  advance();
  switch (*c) {
    case 'a':
      advance();
      return '\a';
    case 'b':
      advance();
      return '\b';
    case 'f':
      advance();
      return '\f';
    case 'n':
      advance();
      return '\n';
    case 'r':
      advance();
      return '\r';
    case 't':
      advance();
      return '\t';
    case 'v':
      advance();
      return '\v';
    case '\\':
      advance();
      return '\\';
    case '\'':
      advance();
      return '\'';
    case '"':
      advance();
      return '"';
    case '?':
      advance();
      return '?';
    case '0':
      advance();
      if (!isdigit(*c)) {
        return '\0';
      } else {
        val = 0;
        i = 0;
        while (isdigit(*c) && *c < '8') {
          val *= 8;
          val += *c - '0';
          advance();
          i++;
          
          if (i > 2) {
            lexerError = "Invalid octal escape sequence";
            return std::nullopt;
          }
        }
        
        if (val > 255) {
          lexerError = "Invalid octal escape sequence";
          return std::nullopt;
        }
        
        return static_cast<char>(val);
      }
      return '\0';
    
    case 'x':
      advance();
      if (!isxdigit(*c)) {
        lexerError = "Invalid hexadecimal escape sequence";
        return -1;
      }
      while (isxdigit(*c)) {
        val *= 16;
        if (isdigit(*c)) {
          val += *c - '0';
        } else {
          val += tolower(*c) - 'a' + 10;
        }
        advance();
        
        if (val > 255) {
          lexerError = "Invalid hexadecimal escape sequence";
          return std::nullopt;
        }
      }
      return static_cast<char>(val);
    
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
      val = 0;
      i = 0;
      while (isdigit(*c) && *c < '8') {
        val *= 8;
        val += *c - '0';
        advance();
        i++;
        
        if (val > 255 || i > 3) {
          lexerError = "Invalid octal escape sequence";
          return std::nullopt;
        }
      }
      return static_cast<char>(val);
    
    default:
      lexerError = "Invalid escape sequence";
      return std::nullopt;
  }
}

unique_ptr<CToken> Lexer::lex_char() {
  advance();
  string str;
  
  if (*c == '\\') {
    auto chr = lex_escape_character();
    if (!chr.has_value()) {
      return std::make_unique<CToken>(CTokenType::CUnknown, "", line, col);
    }
    str += chr.value();
  } else {
    str += *c;
    advance();
  }
  
  if (*c != '\'') {
    lexerError = "Expected closing single quote";
    return std::make_unique<CToken>(CTokenType::CUnknown, "", line, col);
  }
  
  advance();
  return std::make_unique<CToken>(CTokenType::CConstantChar, str.c_str(), line, col);
}

unique_ptr<CToken> Lexer::lex_string() {
  advance();
  string str;
  
  while (true) {
    if (*c == '\\') {
      auto chr = lex_escape_character();
      if (!chr.has_value()) {
        return std::make_unique<CToken>(CTokenType::CUnknown, "", line, col);
      }
      str += chr.value();
    } else if (*c == '"') {
      advance();
      str = '"' + str + '"'; // add quotes to string
      return std::make_unique<CToken>(CTokenType::CConstantString, str.c_str(), line, col);
    } else {
      str += *c;
      advance();
    }
  }
}

unique_ptr<CToken> Lexer::lex_word() {
//  auto break case char const continue default do double else
//  enum extern float for goto if inline int long register
//  restrict return short signed sizeof static struct switch typedef union
//  unsigned void volatile while _Bool _Complex _Imaginary
  
  string id;
  id += *c;
  advance();
  
  while (isIdChar(c)) {
    id += *c;
    advance();
  }
  
  switch (id[0]) {
    case 'a':
      if (id == "auto") {
        return std::make_unique<CToken>(CTokenType::CKeywordAuto, id.c_str(), line, col);
      }
      break;
    case 'b':
      if (id == "break") {
        return std::make_unique<CToken>(CTokenType::CKeywordBreak, id.c_str(), line, col);
      }
      break;
    case 'c':
      if (id == "case") {
        return std::make_unique<CToken>(CTokenType::CKeywordCase, id.c_str(), line, col);
      } else if (id == "char") {
        return std::make_unique<CToken>(CTokenType::CKeywordChar, id.c_str(), line, col);
      } else if (id == "const") {
        return std::make_unique<CToken>(CTokenType::CKeywordConst, id.c_str(), line, col);
      } else if (id == "continue") {
        return std::make_unique<CToken>(CTokenType::CKeywordContinue, id.c_str(), line, col);
      }
      break;
    case 'd':
      if (id == "default") {
        return std::make_unique<CToken>(CTokenType::CKeywordDefault, id.c_str(), line, col);
      } else if (id == "do") {
        return std::make_unique<CToken>(CTokenType::CKeywordDo, id.c_str(), line, col);
      } else if (id == "double") {
        return std::make_unique<CToken>(CTokenType::CKeywordDouble, id.c_str(), line, col);
      }
      break;
    case 'e':
      if (id == "else") {
        return std::make_unique<CToken>(CTokenType::CKeywordElse, id.c_str(), line, col);
      } else if (id == "enum") {
        return std::make_unique<CToken>(CTokenType::CKeywordEnum, id.c_str(), line, col);
      } else if (id == "extern") {
        return std::make_unique<CToken>(CTokenType::CKeywordExtern, id.c_str(), line, col);
      }
      break;
    case 'f':
      if (id == "float") {
        return std::make_unique<CToken>(CTokenType::CKeywordFloat, id.c_str(), line, col);
      } else if (id == "for") {
        return std::make_unique<CToken>(CTokenType::CKeywordFor, id.c_str(), line, col);
      }
      break;
    case 'g':
      if (id == "goto") {
        return std::make_unique<CToken>(CTokenType::CKeywordGoto, id.c_str(), line, col);
      }
      break;
    case 'i':
      if (id == "if") {
        return std::make_unique<CToken>(CTokenType::CKeywordIf, id.c_str(), line, col);
      } else if (id == "inline") {
        return std::make_unique<CToken>(CTokenType::CKeywordInline, id.c_str(), line, col);
      } else if (id == "int") {
        return std::make_unique<CToken>(CTokenType::CKeywordInt, id.c_str(), line, col);
      }
      break;
    case 'l':
      if (id == "long") {
        return std::make_unique<CToken>(CTokenType::CKeywordLong, id.c_str(), line, col);
      }
      break;
    case 'r':
      if (id == "register") {
        return std::make_unique<CToken>(CTokenType::CKeywordRegister, id.c_str(), line, col);
      } else if (id == "restrict") {
        return std::make_unique<CToken>(CTokenType::CKeywordRestrict, id.c_str(), line, col);
      } else if (id == "return") {
        return std::make_unique<CToken>(CTokenType::CKeywordReturn, id.c_str(), line, col);
      }
      break;
    case 's':
      if (id == "short") {
        return std::make_unique<CToken>(CTokenType::CKeywordShort, id.c_str(), line, col);
      } else if (id == "signed") {
        return std::make_unique<CToken>(CTokenType::CKeywordSigned, id.c_str(), line, col);
      } else if (id == "sizeof") {
        return std::make_unique<CToken>(CTokenType::CKeywordSizeof, id.c_str(), line, col);
      } else if (id == "static") {
        return std::make_unique<CToken>(CTokenType::CKeywordStatic, id.c_str(), line, col);
      } else if (id == "struct") {
        return std::make_unique<CToken>(CTokenType::CKeywordStruct, id.c_str(), line, col);
      } else if (id == "switch") {
        return std::make_unique<CToken>(CTokenType::CKeywordSwitch, id.c_str(), line, col);
      }
      break;
    case 't':
      if (id == "typedef") {
        return std::make_unique<CToken>(CTokenType::CKeywordTypedef, id.c_str(), line, col);
      }
      break;
    case 'u':
      if (id == "union") {
        return std::make_unique<CToken>(CTokenType::CKeywordUnion, id.c_str(), line, col);
      } else if (id == "unsigned") {
        return std::make_unique<CToken>(CTokenType::CKeywordUnsigned, id.c_str(), line, col);
      }
      break;
    case 'v':
      if (id == "void") {
        return std::make_unique<CToken>(CTokenType::CKeywordVoid, id.c_str(), line, col);
      } else if (id == "volatile") {
        return std::make_unique<CToken>(CTokenType::CKeywordVolatile, id.c_str(), line, col);
      }
      break;
    case 'w':
      if (id == "while") {
        return std::make_unique<CToken>(CTokenType::CKeywordWhile, id.c_str(), line, col);
      }
      break;
    case '_':
      if (id == "_Bool") {
        return std::make_unique<CToken>(CTokenType::CKeyword_Bool, id.c_str(), line, col);
      } else if (id == "_Complex") {
        return std::make_unique<CToken>(CTokenType::CKeyword_Complex, id.c_str(), line, col);
      } else if (id == "_Imaginary") {
        return std::make_unique<CToken>(CTokenType::CKeyword_Imaginary, id.c_str(), line, col);
      }
      break;
    default:
      break;
  }
  
  return std::make_unique<CToken>(CTokenType::CIdentifier, id.c_str(), line, col);
}

unique_ptr<CToken> Lexer::lex_num() {
  bool isFloat = false;
  string str;
  
  while (true) {
    switch (*c) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        str += *c;
        break;
      case '.':
        if (isFloat) {
          break;
        }
        isFloat = true;
        str += *c;
        break;
      default:
        if (isFloat) {
          return std::make_unique<CToken>(CTokenType::CConstantFloat, str.c_str(), line, col);
        } else {
          return std::make_unique<CToken>(CTokenType::CConstantInteger, str.c_str(), line, col);
        }
    }
    
    advance();
  }
}

unique_ptr<CToken> Lexer::lex_preprocessor() {
  string str;
  str += *c;
  advance();
  
  if (*c == '#') {
    advance();
    return std::make_unique<CToken>(CTokenType::CPreprocessorHashHash, "##", line, col);
  }
  
  while (*c == ' ' || *c == '\t') {
    advance();
  }
  
  while (isIdChar(c)) {
    str += *c;
    advance();
  }
  
  switch (str[1]) {
    case 'i':
      if (str == "#include") {
        return std::make_unique<CToken>(CTokenType::CPreprocessorInclude, str.c_str(), line, col);
      } else if (str == "#if") {
        return std::make_unique<CToken>(CTokenType::CPreprocessorIf, str.c_str(), line, col);
      } else if (str == "#ifdef") {
        return std::make_unique<CToken>(CTokenType::CPreprocessorIfdef, str.c_str(), line, col);
      } else if (str == "#ifndef") {
        return std::make_unique<CToken>(CTokenType::CPreprocessorIfndef, str.c_str(), line, col);
      }
      break;
    case 'd':
      if (str == "#define") {
        return std::make_unique<CToken>(CTokenType::CPreprocessorDefine, str.c_str(), line, col);
      }
      break;
    case 'u':
      if (str == "#undef") {
        return std::make_unique<CToken>(CTokenType::CPreprocessorUndef, str.c_str(), line, col);
      }
      break;
    case 'l':
      if (str == "#line") {
        return std::make_unique<CToken>(CTokenType::CPreprocessorLine, str.c_str(), line, col);
      }
      break;
    case 'e':
      if (str == "#error") {
        return std::make_unique<CToken>(CTokenType::CPreprocessorError, str.c_str(), line, col);
      } else if (str == "#elif") {
        return std::make_unique<CToken>(CTokenType::CPreprocessorElif, str.c_str(), line, col);
      } else if (str == "#else") {
        return std::make_unique<CToken>(CTokenType::CPreprocessorElse, str.c_str(), line, col);
      } else if (str == "#endif") {
        return std::make_unique<CToken>(CTokenType::CPreprocessorEndif, str.c_str(), line, col);
      }
      break;
    case 'p':
      if (str == "#pragma") {
        return std::make_unique<CToken>(CTokenType::CPreprocessorPragma, str.c_str(), line, col);
      }
      break;
    case '#' :
      return std::make_unique<CToken>(CTokenType::CPreprocessorHashHash, str.c_str(), line, col);
    
    default:
      break;
  }
  
  lexerError = "Unknown preprocessor directive";
  return std::make_unique<CToken>(CTokenType::CUnknown, "", line, col);
}

unique_ptr<CToken> Lexer::next() {
  string str;
  
  while (true) {
    switch (*c) {
      
      case EOF:
        return std::make_unique<CToken>(CTokenType::CEndOfFile, "", line, col);
      case '\n':
        line++;
        col = 1;
      case ' ':
      case '\t':
      case '\r':
        advance();
        break;
        
        // Operators
      case '+':
        advance();
        if (*c == '=') {
          advance();
          return std::make_unique<CToken>(CTokenType::COperatorPlusAssign, "+=", line, col);
        } else if (*c == '+') {
          advance();
          return std::make_unique<CToken>(CTokenType::COperatorIncrement, "++", line, col);
        }
        return std::make_unique<CToken>(CTokenType::COperatorPlus, "+", line, col);
      
      case '-':
        advance();
        if (*c == '=') {
          advance();
          return std::make_unique<CToken>(CTokenType::COperatorMinusAssign, "-=", line, col);
        } else if (*c == '-') {
          advance();
          return std::make_unique<CToken>(CTokenType::COperatorDecrement, "--", line, col);
        } else if (*c == '>') {
          advance();
          return std::make_unique<CToken>(CTokenType::CPunctuationArrow, "->", line, col);
        }
        return std::make_unique<CToken>(CTokenType::COperatorMinus, "-", line, col);
      
      case '*':
        advance();
        if (*c == '=') {
          advance();
          return std::make_unique<CToken>(CTokenType::COperatorMultiplyAssign, "*=", line, col);
        }
        return std::make_unique<CToken>(CTokenType::COperatorMultiply, "*", line, col);
      
      case '/':
        advance();
        if (*c == '=') {
          advance();
          return std::make_unique<CToken>(CTokenType::COperatorDivideAssign, "/=", line, col);
        } else if (*c == '/') {
          advance();
          while (*c != '\n' && *c != EOF) {
            advance();
          }
          break;
        } else if (*c == '*') {
          advance();
          while (true) {
            if (*c == EOF) {
              lexerError = "Unterminated comment";
              return std::make_unique<CToken>(CTokenType::CUnknown, "", line, col);
            } else if (*c == '*') {
              advance();
              if (*c == '/') {
                advance();
                break;
              }
            } else {
              advance();
            }
          }
          break;
        }
        return std::make_unique<CToken>(CTokenType::COperatorDivide, "/", line, col);
      
      case '%':
        advance();
        if (*c == '=') {
          advance();
          return std::make_unique<CToken>(CTokenType::COperatorModuloAssign, "%=", line, col);
        }
        return std::make_unique<CToken>(CTokenType::COperatorModulo, "%", line, col);
      
      case '=':
        advance();
        if (*c == '=') {
          advance();
          return std::make_unique<CToken>(CTokenType::COperatorEqual, "==", line, col);
        }
        return std::make_unique<CToken>(CTokenType::COperatorAssignment, "=", line, col);
      
      case '!':
        advance();
        if (*c == '=') {
          advance();
          return std::make_unique<CToken>(CTokenType::COperatorNotEqual, "!=", line, col);
        }
        return std::make_unique<CToken>(CTokenType::COperatorNot, "!", line, col);
      
      case '&':
        advance();
        if (*c == '=') {
          advance();
          return std::make_unique<CToken>(CTokenType::COperatorAndAssign, "&=", line, col);
        } else if (*c == '&') {
          advance();
          return std::make_unique<CToken>(CTokenType::COperatorAnd, "&&", line, col);
        }
        return std::make_unique<CToken>(CTokenType::COperatorBitwiseAnd, "&", line, col);
      
      case '|':
        advance();
        if (*c == '=') {
          advance();
          return std::make_unique<CToken>(CTokenType::COperatorOrAssign, "|=", line, col);
        } else if (*c == '|') {
          advance();
          return std::make_unique<CToken>(CTokenType::COperatorOr, "||", line, col);
        }
        return std::make_unique<CToken>(CTokenType::COperatorBitwiseOr, "|", line, col);
      
      case '^':
        advance();
        if (*c == '=') {
          advance();
          return std::make_unique<CToken>(CTokenType::COperatorXorAssign, "^=", line, col);
        }
        return std::make_unique<CToken>(CTokenType::COperatorBitwiseXor, "^", line, col);
      
      case '~':
        advance();
        return std::make_unique<CToken>(CTokenType::COperatorBitwiseNot, "~", line, col);
      
      case '>':
        advance();
        if (*c == '=') {
          advance();
          return std::make_unique<CToken>(CTokenType::COperatorGreaterEqual, ">=", line, col);
        } else if (*c == '>') {
          advance();
          if (*c == '=') {
            advance();
            return std::make_unique<CToken>(CTokenType::COperatorRightShiftAssign, ">>=", line, col);
          }
          return std::make_unique<CToken>(CTokenType::COperatorRightShift, ">>", line, col);
        }
        return std::make_unique<CToken>(CTokenType::COperatorGreater, ">", line, col);
      
      case '<':
        advance();
        if (*c == '=') {
          advance();
          return std::make_unique<CToken>(CTokenType::COperatorLessEqual, "<=", line, col);
        } else if (*c == '<') {
          advance();
          if (*c == '=') {
            advance();
            return std::make_unique<CToken>(CTokenType::COperatorLeftShiftAssign, "<<=", line, col);
          }
          return std::make_unique<CToken>(CTokenType::COperatorLeftShift, "<<", line, col);
        }
        return std::make_unique<CToken>(CTokenType::COperatorLess, "<", line, col);
        
        // Punctuation
      case '(':
        advance();
        return std::make_unique<CToken>(CTokenType::CPunctuationOpenParen, "(", line, col);
      case ')':
        advance();
        return std::make_unique<CToken>(CTokenType::CPunctuationCloseParen, ")", line, col);
      case '{':
        advance();
        return std::make_unique<CToken>(CTokenType::CPunctuationOpenBrace, "{", line, col);
      case '}':
        advance();
        return std::make_unique<CToken>(CTokenType::CPunctuationCloseBrace, "}", line, col);
      case '[':
        advance();
        return std::make_unique<CToken>(CTokenType::CPunctuationOpenBracket, "[", line, col);
      case ']':
        advance();
        return std::make_unique<CToken>(CTokenType::CPunctuationCloseBracket, "]", line, col);
      case ',':
        advance();
        return std::make_unique<CToken>(CTokenType::CPunctuationComma, ",", line, col);
      case ';':
        advance();
        return std::make_unique<CToken>(CTokenType::CPunctuationSemicolon, ";", line, col);
      case ':':
        advance();
        return std::make_unique<CToken>(CTokenType::CPunctuationColon, ":", line, col);
      case '?':
        advance();
        return std::make_unique<CToken>(CTokenType::CPunctuationQuestionMark, "?", line, col);
      case '\\':
       advance();
        if (*c == '\n') {
          line += 1;
          col = 1;
          advance();
        } else {
          return std::make_unique<CToken>(CTokenType::CPunctuationBackslash, "\\", line, col);
        }
        break;
      case '.':
        advance();
        if (*c == '.') {
          advance();
          if (*c == '.') {
            advance();
            return std::make_unique<CToken>(CTokenType::CPunctuationEllipsis, "...", line, col);
          }
        }
        return std::make_unique<CToken>(CTokenType::CPunctuationDot, ".", line, col);
      
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        return lex_num();
      
      case '\'':
        return lex_char();
      
      case '"':
        return lex_string();
      
      case '#':
        return lex_preprocessor();
      
      case 'a'...'z':
      case 'A'...'Z':
      case '_':
        return lex_word();
      
      default:
        lexerError = "Cannot parse token";
        return std::make_unique<CToken>(CTokenType::CUnknown, "", line, col);
    }
  }
}