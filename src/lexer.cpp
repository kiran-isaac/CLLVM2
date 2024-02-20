#include <lexer.h>
#include <iostream>

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

Lexer::~Lexer() {
  source.close();
}

Lexer::Lexer(std::ifstream &source) : source(source) {
  // put eof at the end of buf1 and buf2
  buf1[4096] = EOF;
  buf2[4096] = EOF;
  refreshBuffer1();
  refreshBuffer2();

  c = buf1;
  line = 1;
  col = 1;
}