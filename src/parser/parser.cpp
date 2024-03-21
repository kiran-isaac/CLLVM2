//
// Created by kiran on 2/24/24.
//

#include <iostream>
#include "parser.h"

void Parser::check_for_circular_include(const std::string &filename) {
  for (const auto &include : lexers) {
    if (include->filename == filename) {
      throw std::runtime_error("Circular include detected: " + filename);
    }
  }
}

void Parser::next() {
  auto newToken = lexers.back()->next();

  if (newToken->type == CTokenType::CEndOfFile) {
    lexers.pop_back();
    if (lexers.empty()) {
      token = std::move(newToken);
    } else {
      next();
    }
    
    if (!lexers.empty()) {
      current_lexer = lexers.back().get();
    }
    std::cout << "Ascending to depth " << lexers.size() - 1 << ". Current lexer: " << current_lexer->filename << std::endl;
  } else {
    token = std::move(newToken);
  }
}
  
unique_ptr<AST> Parser::parse_preprocessor() {
  switch (token->type) {
    case CTokenType::CPreprocessorInclude:
      parse_include();
      break;
    case CTokenType::CPreprocessorDefine:
      // handle define
      next();
      break;
    case CTokenType::CPreprocessorUndef:
      // handle undef
      next();
      break;
    case CTokenType::CPreprocessorLine:
      // handle line
      next();
      break;
    case CTokenType::CPreprocessorError:
      // handle error
      next();
      break;
    case CTokenType::CPreprocessorPragma:
      // handle pragma
      next();
      break;
    case CTokenType::CPreprocessorIf:
      // handle if
      next();
      break;
    case CTokenType::CPreprocessorIfdef:
      // handle ifdef
      next();
      break;
    case CTokenType::CPreprocessorIfndef:
      // handle ifndef
      next();
      break;
    case CTokenType::CPreprocessorElif:
      // handle elif
      next();
      break;
    case CTokenType::CPreprocessorElse:
      // handle else
      next();
      break;
    case CTokenType::CPreprocessorEndif:
      // handle endif
      next();
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
        next();
        break;
        
    }
  }
}
