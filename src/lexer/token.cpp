#include <token.h>

string CToken::getTypeAsString() const {
  switch (type) {
    case CTokenType::CKeywordAuto:
      return "auto";
    case CTokenType::CKeywordBreak:
      return "break";
    case CTokenType::CKeywordCase:
      return "case";
    case CTokenType::CKeywordChar:
      return "char";
    case CTokenType::CKeywordConst:
      return "const";
    case CTokenType::CKeywordContinue:
      return "continue";
    case CTokenType::CKeywordDefault:
      return "default";
    case CTokenType::CKeywordDo:
      return "do";
    case CTokenType::CKeywordDouble:
      return "double";
    case CTokenType::CKeywordElse:
      return "else";
    case CTokenType::CKeywordEnum:
      return "enum";
    case CTokenType::CKeywordExtern:
      return "extern";
    case CTokenType::CKeywordFloat:
      return "float";
    case CTokenType::CKeywordFor:
      return "for";
    case CTokenType::CKeywordGoto:
      return "goto";
    case CTokenType::CKeywordIf:
      return "if";
    case CTokenType::CKeywordInline:
      return "inline";
    case CTokenType::CKeywordInt:
      return "int";
    case CTokenType::CKeywordLong:
      return "long";
    case CTokenType::CKeywordRegister:
      return "register";
    case CTokenType::CKeywordRestrict:
      return "restrict";
    case CTokenType::CKeywordReturn:
      return "return";
    case CTokenType::CKeywordShort:
      return "short";
    case CTokenType::CKeywordSigned:
      return "signed";
    case CTokenType::CKeywordSizeof:
      return "sizeof";
    case CTokenType::CKeywordStatic:
      return "static";
    case CTokenType::CKeywordStruct:
      return "struct";
    case CTokenType::CKeywordSwitch:
      return "switch";
    case CTokenType::CKeywordTypedef:
      return "typedef";
    case CTokenType::CKeywordUnion:
      return "union";
    case CTokenType::CKeywordUnsigned:
      return "unsigned";
    case CTokenType::CKeywordVoid:
      return "void";
    case CTokenType::CKeywordVolatile:
      return "volatile";
    case CTokenType::CKeywordWhile:
      return "while";
    case CTokenType::CKeyword_Bool:
      return "_Bool";
    case CTokenType::CKeyword_Complex:
      return "_Complex";
    case CTokenType::CKeyword_Imaginary:
      return "_Imaginary";
      
    case CTokenType::CIdentifier:
      return "identifier";
    
    case CTokenType::CPreprocessorInclude:
      return "#include";
    case CTokenType::CPreprocessorDefine:
      return "#define";
    case CTokenType::CPreprocessorUndef:
      return "#undef";
    case CTokenType::CPreprocessorLine:
      return "#line";
    case CTokenType::CPreprocessorError:
      return "#error";
    case CTokenType::CPreprocessorPragma:
      return "#pragma";
    case CTokenType::CPreprocessorIf:
      return "#if";
    case CTokenType::CPreprocessorIfdef:
      return "#ifdef";
    case CTokenType::CPreprocessorIfndef:
      return "#ifndef";
    case CTokenType::CPreprocessorElif:
      return "#elif";
    case CTokenType::CPreprocessorElse:
      return "#else";
    case CTokenType::CPreprocessorEndif:
      return "#endif";
    case CTokenType::CPreprocessorHashHash:
      return "##";
    
    case CTokenType::CConstantInteger:
      return "integer constant";
    case CTokenType::CConstantFloat:
      return "float constant";
    case CTokenType::CConstantChar:
      return "char constant";
    case CTokenType::CConstantString:
      return "string constant";
    
    case CTokenType::COperatorPlus:
      return "+";
    case CTokenType::COperatorMinus:
      return "-";
    case CTokenType::COperatorMultiply:
      return "*";
    case CTokenType::COperatorDivide:
      return "/";
    case CTokenType::COperatorModulo:
      return "%";
    case CTokenType::COperatorIncrement:
      return "++";
    case CTokenType::COperatorDecrement:
      return "--";
    case CTokenType::COperatorEqual:
      return "==";
    case CTokenType::COperatorNotEqual:
      return "!=";
    case CTokenType::COperatorGreater:
      return ">";
    case CTokenType::COperatorLess:
      return "<";
    case CTokenType::COperatorGreaterEqual:
      return ">=";
    case CTokenType::COperatorLessEqual:
      return "<=";
    case CTokenType::COperatorAnd:
      return "&&";
    case CTokenType::COperatorOr:
      return "||";
    case CTokenType::COperatorNot:
      return "!";
    case CTokenType::COperatorBitwiseAnd:
      return "&";
    case CTokenType::COperatorBitwiseOr:
      return "|";
    case CTokenType::COperatorBitwiseXor:
      return "^";
    case CTokenType::COperatorBitwiseNot:
      return "~";
    case CTokenType::COperatorLeftShift:
      return "<<";
    case CTokenType::COperatorRightShift:
      return ">>";
    case CTokenType::COperatorAssignment:
      return "=";
    case CTokenType::COperatorPlusAssign:
      return "+=";
    case CTokenType::COperatorMinusAssign:
      return "-=";
    case CTokenType::COperatorMultiplyAssign:
      return "*=";
    case CTokenType::COperatorDivideAssign:
      return "/=";
    case CTokenType::COperatorModuloAssign:
      return "%=";
    case CTokenType::COperatorAndAssign:
      return "&=";
    case CTokenType::COperatorOrAssign:
      return "|=";
    case CTokenType::COperatorXorAssign:
      return "^=";
    case CTokenType::COperatorLeftShiftAssign:
      return "<<=";
    case CTokenType::COperatorRightShiftAssign:
      return ">>=";
    
    case CTokenType::CPunctuationOpenParen:
      return "(";
    case CTokenType::CPunctuationCloseParen:
      return ")";
    case CTokenType::CPunctuationOpenBrace:
      return "{";
    case CTokenType::CPunctuationCloseBrace:
      return "}";
    case CTokenType::CPunctuationOpenBracket:
      return "[";
    case CTokenType::CPunctuationCloseBracket:
      return "]";
    case CTokenType::CPunctuationComma:
      return ",";
    case CTokenType::CPunctuationSemicolon:
      return ";";
    case CTokenType::CPunctuationColon:
      return ":";
    case CTokenType::CPunctuationQuestionMark:
      return "?";
    case CTokenType::CPunctuationBackslash:
      return "\\";
    case CTokenType::CPunctuationDot:
      return ".";
    case CTokenType::CPunctuationHash:
      return "#";
    
    case CTokenType::CEndOfFile:
      return "end of file";
    case CTokenType::CUnknown:
      return "unknown";
  }
}

string CToken::toString() const {
  string str = "<Token: " + getTypeAsString();
  if (value) {
    str += " value: " + *value;
  }
  str += " line: " + std::to_string(line);
  str += " col: " + std::to_string(col);
  return str + ">";
}

CToken::CToken(CTokenType type, const char *value, unsigned int line, unsigned int col) {
  this->type = type;
  this->value = std::make_unique<string>(value);
  this->line = line;
  this->col = col;
}