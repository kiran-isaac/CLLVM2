#include "lexer.h"
#include <iostream>
#include <algorithm>

using std::unique_ptr, std::shared_ptr, std::string;

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
    if (source.eof()) {
      c = nullptr;
      return;
    }
    refreshBuffer1();
    c = buf1;
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
  refreshBuffer2();
  
  c = buf1;
  line = 1;
  col = 1;
}

char Lexer::parse_escape_character() {
  int val = 0;
  int i = 0; // counter for octal escape sequences
  advance();
  switch (*c) {
    case 'a':
      return '\a';
    case 'b':
      return '\b';
    case 'f':
      return '\f';
    case 'n':
      return '\n';
    case 'r':
      return '\r';
    case 't':
      return '\t';
    case 'v':
      return '\v';
    case '\\':
      return '\\';
    case '\'':
      return '\'';
    case '"':
      return '"';
    case '?':
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
          
          if (i >= 2) {
            lexerError = "Invalid octal escape sequence";
            return -1;
          }
        }
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
          return -1;
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
      while (isdigit(*c) && *c < '8') {
        val *= 8;
        val += *c - '0';
        advance();
        
        if (val > 255) {
          lexerError = "Invalid octal escape sequence";
          return -1;
        }
      }
      return static_cast<char>(val);
      
    default:
      lexerError = "Invalid escape sequence";
      return -1;
  }
}

unique_ptr<CToken> Lexer::parse_char() {
  advance();
  string str;
  
  if (*c == '\\') {
    str += parse_escape_character();
    if (str.back() == -1) {
      return std::make_unique<CToken>(CTokenType::CUnknown, "", line, col);
    }
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

unique_ptr<CToken> Lexer::parse_string() {
  advance();
  string str;
  
  while (true) {
    if (*c == '\\') {
      str += parse_escape_character();
      if (str.back() == -1) {
        return std::make_unique<CToken>(CTokenType::CUnknown, "", line, col);
      }
    } else if (*c == '"') {
      advance();
      return std::make_unique<CToken>(CTokenType::CConstantString, str.c_str(), line, col);
    } else {
      str += *c;
      advance();
    }
  }
}

// TODO make this work when checked multiple times
bool Lexer::is_keyword(const string &kw) {
  advance();
  
  for (int i = 1; i < kw.size(); i++) {
    id += *c;
    if (*c != kw[i]) {
      return false;
    }
    advance();
  }
  
  if (isalnum(*c) || *c == '_') {
    return false;
  }
  
  return true;
}

unique_ptr<CToken> Lexer::parse_word() {
//  auto break case char const continue default do double else
//  enum extern float for goto if inline int long register
//  restrict return short signed sizeof static struct switch typedef union
//  unsigned void volatile while _Bool _Complex _Imaginary
  id += *c;
  
  // Switch on first char. Do not have to KW check on future chars
  switch (*c) {
    case 'a':
      if (is_keyword("auto")) {
        return std::make_unique<CToken>(CTokenType::CKeywordAuto, "auto", line, col);
      }
      break;
      
    case 'b':
      if (is_keyword("break")) {
        return std::make_unique<CToken>(CTokenType::CKeywordBreak, "break", line, col);
      }
      break;
      case 'c':
      if (is_keyword("case")) {
        return std::make_unique<CToken>(CTokenType::CKeywordCase, "case", line, col);
      } else if (is_keyword("char")) {
        return std::make_unique<CToken>(CTokenType::CKeywordChar, "char", line, col);
      } else if (is_keyword("const")) {
        return std::make_unique<CToken>(CTokenType::CKeywordConst, "const", line, col);
      } else if (is_keyword("continue")) {
        return std::make_unique<CToken>(CTokenType::CKeywordContinue, "continue", line, col);
      }
      break;
      
    case 'd':
      if (is_keyword("default")) {
        return std::make_unique<CToken>(CTokenType::CKeywordDefault, "default", line, col);
      } else if (is_keyword("do")) {
        return std::make_unique<CToken>(CTokenType::CKeywordDo, "do", line, col);
      } else if (is_keyword("double")) {
        return std::make_unique<CToken>(CTokenType::CKeywordDouble, "double", line, col);
      }
      break;
      
    case 'e':
      if (is_keyword("else")) {
        return std::make_unique<CToken>(CTokenType::CKeywordElse, "else", line, col);
      } else if (is_keyword("enum")) {
        return std::make_unique<CToken>(CTokenType::CKeywordEnum, "enum", line, col);
      } else if (is_keyword("extern")) {
        return std::make_unique<CToken>(CTokenType::CKeywordExtern, "extern", line, col);
      }
      break;
      
    case 'f':
      if (is_keyword("float")) {
        return std::make_unique<CToken>(CTokenType::CKeywordFloat, "float", line, col);
      } else if (is_keyword("for")) {
        return std::make_unique<CToken>(CTokenType::CKeywordFor, "for", line, col);
      }
      break;
      
    case 'g':
      if (is_keyword("goto")) {
        return std::make_unique<CToken>(CTokenType::CKeywordGoto, "goto", line, col);
      }
      break;
      
    case 'i':
      if (is_keyword("if")) {
        return std::make_unique<CToken>(CTokenType::CKeywordIf, "if", line, col);
      } else if (is_keyword("inline")) {
        return std::make_unique<CToken>(CTokenType::CKeywordInline, "inline", line, col);
      } else if (is_keyword("int")) {
        return std::make_unique<CToken>(CTokenType::CKeywordInt, "int", line, col);
      }
      break;
      
    case 'l':
      if (is_keyword("long")) {
        return std::make_unique<CToken>(CTokenType::CKeywordLong, "long", line, col);
      }
      break;
      
    case 'r':
      if (is_keyword("register")) {
        return std::make_unique<CToken>(CTokenType::CKeywordRegister, "register", line, col);
      } else if (is_keyword("restrict")) {
        return std::make_unique<CToken>(CTokenType::CKeywordRestrict, "restrict", line, col);
      } else if (is_keyword("return")) {
        return std::make_unique<CToken>(CTokenType::CKeywordReturn, "return", line, col);
      }
      break;
      
    case 's':
      if (is_keyword("short")) {
        return std::make_unique<CToken>(CTokenType::CKeywordShort, "short", line, col);
      } else if (is_keyword("signed")) {
        return std::make_unique<CToken>(CTokenType::CKeywordSigned, "signed", line, col);
      } else if (is_keyword("sizeof")) {
        return std::make_unique<CToken>(CTokenType::CKeywordSizeof, "sizeof", line, col);
      } else if (is_keyword("static")) {
        return std::make_unique<CToken>(CTokenType::CKeywordStatic, "static", line, col);
      } else if (is_keyword("struct")) {
        return std::make_unique<CToken>(CTokenType::CKeywordStruct, "struct", line, col);
      } else if (is_keyword("switch")) {
        return std::make_unique<CToken>(CTokenType::CKeywordSwitch, "switch", line, col);
      }
      break;
      
    case 't':
      if (is_keyword("typedef")) {
        return std::make_unique<CToken>(CTokenType::CKeywordTypedef, "typedef", line, col);
      }
      break;
      
    case 'u':
      if (is_keyword("union")) {
        return std::make_unique<CToken>(CTokenType::CKeywordUnion, "union", line, col);
      } else if (is_keyword("unsigned")) {
        return std::make_unique<CToken>(CTokenType::CKeywordUnsigned, "unsigned", line, col);
      }
      break;
      
    case 'v':
      if (is_keyword("void")) {
        return std::make_unique<CToken>(CTokenType::CKeywordVoid, "void", line, col);
      } else if (is_keyword("volatile")) {
        return std::make_unique<CToken>(CTokenType::CKeywordVolatile, "volatile", line, col);
      }
      break;
      
    case 'w':
      if (is_keyword("while")) {
        return std::make_unique<CToken>(CTokenType::CKeywordWhile, "while", line, col);
      }
      break;
      
    case '_':
      if (is_keyword("_Bool")) {
        return std::make_unique<CToken>(CTokenType::CKeyword_Bool, "_Bool", line, col);
      } else if (is_keyword("_Complex")) {
        return std::make_unique<CToken>(CTokenType::CKeyword_Complex, "_Complex", line, col);
      } else if (is_keyword("_Imaginary")) {
        return std::make_unique<CToken>(CTokenType::CKeyword_Imaginary, "_Imaginary", line, col);
      }
      break;
      
    default:
      break;
  }
  
  while (isalnum(*c) || *c == '_') {
    id += *c;
    advance();
  }
  
  // copy id to a new string
  string idCopy = id;
  id.clear();
  return std::make_unique<CToken>(CTokenType::CIdentifier, idCopy.c_str(), line, col);
}

unique_ptr<CToken> Lexer::parse_num() {
  bool isFloat = false;
  string str;
  
  while (true) {
    advance();
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
  }
}


unique_ptr<CToken> Lexer::next() {
  string str;
  
  while (true) {
    switch (*c) {
      case EOF:
        return std::make_unique<CToken>(CTokenType::CEndOfFile, "", line, col);
        
        // Whitespace
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
        return std::make_unique<CToken>(CTokenType::CPunctuationBackslash, "\\", line, col);
      case '#':
        advance();
        return std::make_unique<CToken>(CTokenType::CPunctuationHash, "#", line, col);
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
        return parse_num();
        
      case '\'':
        return parse_char();
        
      case '"':
        return parse_string();
        
        
      case 'a'...'z':
      case 'A'...'Z':
      case '_':
        return parse_word();
        
      default:
        lexerError = "Cannot parse token";
        return std::make_unique<CToken>(CTokenType::CUnknown, "", line, col);
    }
  }
}