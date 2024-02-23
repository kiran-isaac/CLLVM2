#pragma once

#include <memory>
#include <string>
#include <map>

using std::unique_ptr, std::string, std::map;

enum class CTokenType {
  CKeywordAuto,
  CKeywordBreak,
  CKeywordCase,
  CKeywordChar,
  CKeywordConst,
  CKeywordContinue,
  CKeywordDefault,
  CKeywordDo,
  CKeywordDouble,
  CKeywordElse,
  CKeywordEnum,
  CKeywordExtern,
  CKeywordFloat,
  CKeywordFor,
  CKeywordGoto,
  CKeywordIf,
  CKeywordInline,
  CKeywordInt,
  CKeywordLong,
  CKeywordRegister,
  CKeywordRestrict,
  CKeywordReturn,
  CKeywordShort,
  CKeywordSigned,
  CKeywordSizeof,
  CKeywordStatic,
  CKeywordStruct,
  CKeywordSwitch,
  CKeywordTypedef,
  CKeywordUnion,
  CKeywordUnsigned,
  CKeywordVoid,
  CKeywordVolatile,
  CKeywordWhile,
  CKeyword_Bool,
  CKeyword_Complex,
  CKeyword_Imaginary,

  CIdentifier,

  CPreprocessorInclude,      // #include
  CPreprocessorDefine,       // #define
  CPreprocessorUndef,        // #undef
  CPreprocessorLine,         // #line
  CPreprocessorError,        // #error
  CPreprocessorPragma,       // #pragma
  CPreprocessorIf,           // #if
  CPreprocessorIfdef,        // #ifdef
  CPreprocessorIfndef,       // #ifndef
  CPreprocessorElif,         // #elif
  CPreprocessorElse,         // #else
  CPreprocessorEndif,        // #endif
  CPreprocessorHashHash,     // ##
  
  CConstantInteger,
  CConstantFloat,
  CConstantChar,
  CConstantString,

  COperatorPlus,             // +
  COperatorMinus,            // -
  COperatorMultiply,         // *
  COperatorDivide,           // /
  COperatorModulo,           // %
  COperatorIncrement,        // ++
  COperatorDecrement,        // --
  COperatorEqual,            // ==
  COperatorNotEqual,         // !=
  COperatorGreater,          // >
  COperatorLess,             // <
  COperatorGreaterEqual,     // >=
  COperatorLessEqual,        // <=
  COperatorAnd,              // &&
  COperatorOr,               // ||
  COperatorNot,              // !
  COperatorBitwiseAnd,       // &
  COperatorBitwiseOr,        // |
  COperatorBitwiseXor,       // ^
  COperatorBitwiseNot,       // ~
  COperatorLeftShift,        // <<
  COperatorRightShift,       // >>
  COperatorAssignment,       // =
  COperatorPlusAssign,       // +=
  COperatorMinusAssign,      // -=
  COperatorMultiplyAssign,   // *=
  COperatorDivideAssign,     // /=
  COperatorModuloAssign,     // %=
  COperatorAndAssign,        // &=
  COperatorOrAssign,         // |=
  COperatorXorAssign,        // ^=
  COperatorLeftShiftAssign,  // <<=
  COperatorRightShiftAssign, // >>=
  
  CPunctuationOpenParen,     // (
  CPunctuationCloseParen,    // )
  CPunctuationOpenBrace,     // {
  CPunctuationCloseBrace,    // }
  CPunctuationOpenBracket,   // [
  CPunctuationCloseBracket,  // ]
  CPunctuationComma,         // ,
  CPunctuationSemicolon,     // ;
  CPunctuationColon,         // :
  CPunctuationQuestionMark,  // ?
  CPunctuationBackslash,     // \.
  CPunctuationHash,          // #
  CPunctuationDot,           // .
  CPunctuationArrow,         // ->
  CPunctuationEllipsis,      // ...
  
  CEndOfFile,
  CUnknown,
};

class CToken {
private:
  
public:
  CTokenType type;
  
  // All associated values are strings to avoid having to deal with the different integer sizes at this stage of compilation
  unique_ptr<string> value;
  unsigned int line;
  unsigned int col;
  
  CToken(CTokenType type, const char *value, unsigned int line, unsigned int col);
  
  [[nodiscard]] string getTypeAsString() const;
  
  string toString() const;
};