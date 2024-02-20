#pragma once

enum class CToken {
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
  CKeywordInt,
  CKeywordLong,
  CKeywordRegister,
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

  CIdentifier,

  CPreprocessorInclude,
  CPreprocessorDefine,
  CPreprocessorUndef,
  CPreprocessorLine,
  CPreprocessorError,
  CPreprocessorPragma,
  CPreprocessorIf,
  CPreprocessorIfdef,
  CPreprocessorIfndef,
  CPreprocessorElif,
  CPreprocessorElse,
  CPreprocessorEndif,
  CPreprocessorHash,
  CPreprocessorHashHash,
};

union TokenAssociatedValue {
  char *identifier;
  char *preprocessor;
};

typedef struct Token {
  CToken type;
  TokenAssociatedValue value;
  unsigned int line;
  unsigned int col;
};