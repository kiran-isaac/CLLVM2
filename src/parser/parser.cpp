//
// Created by kiran on 2/24/24.
//

#include "parser.h"

void Parser::next() {
  auto newToken = lexers.top()->next();

  if (newToken->type == CTokenType::CEndOfFile) {
    lexers.pop();
    if (lexers.empty()) {
      token = std::move(newToken);
    } else {
      next();
    }
  } else {
    token = std::move(newToken);
  }
}

void Parser::parse_include() {
  next();
  
  string filename;
  switch (token->type) {
    case CTokenType::CConstantString:
      // parse user include
      filename = *token->value;
      // remove quotes
      filename = filename.substr(1, filename.size() - 2);
    case CTokenType::COperatorLess:
      // parse system include
      filename = "";
      while (true) {
        next();
        if (token->type == CTokenType::COperatorGreater) {
          break;
        }
        filename += *token->value;
      }
      
      break;
    default:
      throw std::runtime_error("Invalid include directive");
  }
  
  filename = find_include(filename);
  if (filename.empty()) {
    throw std::runtime_error("Could not find include file: " + filename);
  }
  
  auto stream = std::make_unique<std::ifstream>(filename);
  if (!stream->good()) {
    throw std::runtime_error("Could not open include file: " + filename);
  }
  
  lexers.push(std::make_unique<Lexer>(*stream));
}
  
unique_ptr<AST> Parser::parse_preprocessor() {
  switch (token->type) {
    case CTokenType::CPreprocessorInclude:
      parse_include();
      break;
    case CTokenType::CPreprocessorDefine:
      // handle define
      break;
    case CTokenType::CPreprocessorUndef:
      // handle undef
      break;
    case CTokenType::CPreprocessorLine:
      // handle line
      break;
    case CTokenType::CPreprocessorError:
      // handle error
      break;
    case CTokenType::CPreprocessorPragma:
      // handle pragma
      break;
    case CTokenType::CPreprocessorIf:
      // handle if
      break;
    case CTokenType::CPreprocessorIfdef:
      // handle ifdef
      break;
    case CTokenType::CPreprocessorIfndef:
      // handle ifndef
      break;
    case CTokenType::CPreprocessorElif:
      // handle elif
      break;
    case CTokenType::CPreprocessorElse:
      // handle else
      break;
    case CTokenType::CPreprocessorEndif:
      // handle endif
      break;
    default:  // should never happen
      throw std::runtime_error("Invalid preprocessor directive");
  }
}

unique_ptr<AST> Parser::parse() {
  unique_ptr<AST> ast = std::make_unique<AST>();
  
  while (true) {
    switch (token->type) {
      case CTokenType::CEndOfFile:
        return ast;
      
      case CTokenType::CPreprocessorInclude:
      case CTokenType::CPreprocessorDefine:
      case CTokenType::CPreprocessorUndef:
      case CTokenType::CPreprocessorLine:
      case CTokenType::CPreprocessorError:
      case CTokenType::CPreprocessorPragma:
      case CTokenType::CPreprocessorIf:
      case CTokenType::CPreprocessorIfdef:
      case CTokenType::CPreprocessorIfndef:
      case CTokenType::CPreprocessorElif:
      case CTokenType::CPreprocessorElse:
      case CTokenType::CPreprocessorEndif:
        ast->children.push_back(parse_preprocessor());
        break;
      default :
        // handle code
        break;
        
    }
  }
}
