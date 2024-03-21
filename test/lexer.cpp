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

TEST(Lexer, Stdio) {
  std::ifstream source("/usr/include/stdio.h");
  
  Lexer lexer(source);
  unique_ptr<CToken> token;

  int count = 0;
  
  // stdio.h without comments or whitespace
  string expected_source = R"(#ifndef_STDIO_H#define_STDIO_H1#define__GLIBC_INTERNAL_STARTING_HEADER_IMPLEMENTATION#include<bits/libc-header-start.h>__BEGIN_DECLS#define__need_size_t#define__need_NULL#include<stddef.h>#define__need___va_list#include<stdarg.h>#include<bits/types.h>#include<bits/types/__fpos_t.h>#include<bits/types/__fpos64_t.h>#include<bits/types/__FILE.h>#include<bits/types/FILE.h>#include<bits/types/struct_FILE.h>#ifdef__USE_GNU#include<bits/types/cookie_io_functions_t.h>#endif#ifdefined__USE_XOPEN||defined__USE_XOPEN2K8#ifdef__GNUC__#ifndef_VA_LIST_DEFINEDtypedef__gnuc_va_listva_list;#define_VA_LIST_DEFINED#endif#else#include<stdarg.h>#endif#endif#ifdefined__USE_UNIX98||defined__USE_XOPEN2K#ifndef__off_t_defined#ifndef__USE_FILE_OFFSET64typedef__off_toff_t;#elsetypedef__off64_toff_t;#endif#define__off_t_defined#endif#ifdefined__USE_LARGEFILE64&&!defined__off64_t_definedtypedef__off64_toff64_t;#define__off64_t_defined#endif#endif#ifdef__USE_XOPEN2K8#ifndef__ssize_t_definedtypedef__ssize_tssize_t;#define__ssize_t_defined#endif#endif#ifndef__USE_FILE_OFFSET64typedef__fpos_tfpos_t;#elsetypedef__fpos64_tfpos_t;#endif#ifdef__USE_LARGEFILE64typedef__fpos64_tfpos64_t;#endif#define_IOFBF0#define_IOLBF1#define_IONBF2#defineBUFSIZ8192#defineEOF(-1)#defineSEEK_SET0#defineSEEK_CUR1#defineSEEK_END2#ifdef__USE_GNU#defineSEEK_DATA3#defineSEEK_HOLE4#endif#ifdefined__USE_MISC||defined__USE_XOPEN#defineP_tmpdir"/tmp"#endif#include<bits/stdio_lim.h>#if__GLIBC_USE(ISOC2X)#define_PRINTF_NAN_LEN_MAX4#endifexternFILE*stdin;externFILE*stdout;externFILE*stderr;#definestdinstdin#definestdoutstdout#definestderrstderrexternintremove(constchar*__filename)__THROW;externintrename(constchar*__old,constchar*__new)__THROW;#ifdef__USE_ATFILEexternintrenameat(int__oldfd,constchar*__old,int__newfd,constchar*__new)__THROW;#endif#ifdef__USE_GNU#defineRENAME_NOREPLACE(1<<0)#defineRENAME_EXCHANGE(1<<1)#defineRENAME_WHITEOUT(1<<2)externintrenameat2(int__oldfd,constchar*__old,int__newfd,constchar*__new,unsignedint__flags)__THROW;#endifexternintfclose(FILE*__stream);#undef__attr_dealloc_fclose#define__attr_dealloc_fclose__attr_dealloc(fclose,1)#ifndef__USE_FILE_OFFSET64externFILE*tmpfile(void)__attribute_malloc____attr_dealloc_fclose__wur;#else#ifdef__REDIRECTexternFILE*__REDIRECT(tmpfile,(void),tmpfile64)__attribute_malloc____attr_dealloc_fclose__wur;#else#definetmpfiletmpfile64#endif#endif#ifdef__USE_LARGEFILE64externFILE*tmpfile64(void)__attribute_malloc____attr_dealloc_fclose__wur;#endifexternchar*tmpnam(char[L_tmpnam])__THROW__wur;#ifdef__USE_MISCexternchar*tmpnam_r(char__s[L_tmpnam])__THROW__wur;#endif#ifdefined__USE_MISC||defined__USE_XOPENexternchar*tempnam(constchar*__dir,constchar*__pfx)__THROW__attribute_malloc____wur__attr_dealloc_free;#endifexternintfflush(FILE*__stream);#ifdef__USE_MISCexternintfflush_unlocked(FILE*__stream);#endif#ifdef__USE_GNUexternintfcloseall(void);#endif#ifndef__USE_FILE_OFFSET64externFILE*fopen(constchar*__restrict__filename,constchar*__restrict__modes)__attribute_malloc____attr_dealloc_fclose__wur;externFILE*freopen(constchar*__restrict__filename,constchar*__restrict__modes,FILE*__restrict__stream)__wur;#else#ifdef__REDIRECTexternFILE*__REDIRECT(fopen,(constchar*__restrict__filename,constchar*__restrict__modes),fopen64)__attribute_malloc____attr_dealloc_fclose__wur;externFILE*__REDIRECT(freopen,(constchar*__restrict__filename,constchar*__restrict__modes,FILE*__restrict__stream),freopen64)__wur;#else#definefopenfopen64#definefreopenfreopen64#endif#endif#ifdef__USE_LARGEFILE64externFILE*fopen64(constchar*__restrict__filename,constchar*__restrict__modes)__attribute_malloc____attr_dealloc_fclose__wur;externFILE*freopen64(constchar*__restrict__filename,constchar*__restrict__modes,FILE*__restrict__stream)__wur;#endif#ifdef__USE_POSIXexternFILE*fdopen(int__fd,constchar*__modes)__THROW__attribute_malloc____attr_dealloc_fclose__wur;#endif#ifdef__USE_GNUexternFILE*fopencookie(void*__restrict__magic_cookie,constchar*__restrict__modes,cookie_io_functions_t__io_funcs)__THROW__attribute_malloc____attr_dealloc_fclose__wur;#endif#ifdefined__USE_XOPEN2K8||__GLIBC_USE(LIB_EXT2)externFILE*fmemopen(void*__s,size_t__len,constchar*__modes)__THROW__attribute_malloc____attr_dealloc_fclose__wur;externFILE*open_memstream(char**__bufloc,size_t*__sizeloc)__THROW__attribute_malloc____attr_dealloc_fclose__wur;#ifdef_WCHAR_Hextern__FILE*open_wmemstream(wchar_t**__bufloc,size_t*__sizeloc)__THROW__attribute_malloc____attr_dealloc_fclose;#endif#endifexternvoidsetbuf(FILE*__restrict__stream,char*__restrict__buf)__THROW;externintsetvbuf(FILE*__restrict__stream,char*__restrict__buf,int__modes,size_t__n)__THROW;#ifdef__USE_MISCexternvoidsetbuffer(FILE*__restrict__stream,char*__restrict__buf,size_t__size)__THROW;externvoidsetlinebuf(FILE*__stream)__THROW;#endifexternintfprintf(FILE*__restrict__stream,constchar*__restrict__format,...);externintprintf(constchar*__restrict__format,...);externintsprintf(char*__restrict__s,constchar*__restrict__format,...)__THROWNL;externintvfprintf(FILE*__restrict__s,constchar*__restrict__format,__gnuc_va_list__arg);externintvprintf(constchar*__restrict__format,__gnuc_va_list__arg);externintvsprintf(char*__restrict__s,constchar*__restrict__format,__gnuc_va_list__arg)__THROWNL;#ifdefined__USE_ISOC99||defined__USE_UNIX98externintsnprintf(char*__restrict__s,size_t__maxlen,constchar*__restrict__format,...)__THROWNL__attribute__((__format__(__printf__,3,4)));externintvsnprintf(char*__restrict__s,size_t__maxlen,constchar*__restrict__format,__gnuc_va_list__arg)__THROWNL__attribute__((__format__(__printf__,3,0)));#endif#if__GLIBC_USE(LIB_EXT2)externintvasprintf(char**__restrict__ptr,constchar*__restrict__f,__gnuc_va_list__arg)__THROWNL__attribute__((__format__(__printf__,2,0)))__wur;externint__asprintf(char**__restrict__ptr,constchar*__restrict__fmt,...)__THROWNL__attribute__((__format__(__printf__,2,3)))__wur;externintasprintf(char**__restrict__ptr,constchar*__restrict__fmt,...)__THROWNL__attribute__((__format__(__printf__,2,3)))__wur;#endif#ifdef__USE_XOPEN2K8externintvdprintf(int__fd,constchar*__restrict__fmt,__gnuc_va_list__arg)__attribute__((__format__(__printf__,2,0)));externintdprintf(int__fd,constchar*__restrict__fmt,...)__attribute__((__format__(__printf__,2,3)));#endifexternintfscanf(FILE*__restrict__stream,constchar*__restrict__format,...)__wur;externintscanf(constchar*__restrict__format,...)__wur;externintsscanf(constchar*__restrict__s,constchar*__restrict__format,...)__THROW;#include<bits/floatn.h>#if!__GLIBC_USE(DEPRECATED_SCANF)&&!defined__LDBL_COMPAT\&&__LDOUBLE_REDIRECTS_TO_FLOAT128_ABI==0#ifdef__REDIRECTexternint__REDIRECT(fscanf,(FILE*__restrict__stream,constchar*__restrict__format,...),__isoc99_fscanf)__wur;externint__REDIRECT(scanf,(constchar*__restrict__format,...),__isoc99_scanf)__wur;externint__REDIRECT_NTH(sscanf,(constchar*__restrict__s,constchar*__restrict__format,...),__isoc99_sscanf);#elseexternint__isoc99_fscanf(FILE*__restrict__stream,constchar*__restrict__format,...)__wur;externint__isoc99_scanf(constchar*__restrict__format,...)__wur;externint__isoc99_sscanf(constchar*__restrict__s,constchar*__restrict__format,...)__THROW;#definefscanf__isoc99_fscanf#definescanf__isoc99_scanf#definesscanf__isoc99_sscanf#endif#endif#ifdef__USE_ISOC99externintvfscanf(FILE*__restrict__s,constchar*__restrict__format,__gnuc_va_list__arg)__attribute__((__format__(__scanf__,2,0)))__wur;externintvscanf(constchar*__restrict__format,__gnuc_va_list__arg)__attribute__((__format__(__scanf__,1,0)))__wur;externintvsscanf(constchar*__restrict__s,constchar*__restrict__format,__gnuc_va_list__arg)__THROW__attribute__((__format__(__scanf__,2,0)));#if!__GLIBC_USE(DEPRECATED_SCANF)#ifdefined__REDIRECT&&!defined__LDBL_COMPAT\&&__LDOUBLE_REDIRECTS_TO_FLOAT128_ABI==0externint__REDIRECT(vfscanf,(FILE*__restrict__s,constchar*__restrict__format,__gnuc_va_list__arg),__isoc99_vfscanf)__attribute__((__format__(__scanf__,2,0)))__wur;externint__REDIRECT(vscanf,(constchar*__restrict__format,__gnuc_va_list__arg),__isoc99_vscanf)__attribute__((__format__(__scanf__,1,0)))__wur;externint__REDIRECT_NTH(vsscanf,(constchar*__restrict__s,constchar*__restrict__format,__gnuc_va_list__arg),__isoc99_vsscanf)__attribute__((__format__(__scanf__,2,0)));#elif!defined__REDIRECTexternint__isoc99_vfscanf(FILE*__restrict__s,constchar*__restrict__format,__gnuc_va_list__arg)__wur;externint__isoc99_vscanf(constchar*__restrict__format,__gnuc_va_list__arg)__wur;externint__isoc99_vsscanf(constchar*__restrict__s,constchar*__restrict__format,__gnuc_va_list__arg)__THROW;#definevfscanf__isoc99_vfscanf#definevscanf__isoc99_vscanf#definevsscanf__isoc99_vsscanf#endif#endif#endifexternintfgetc(FILE*__stream);externintgetc(FILE*__stream);externintgetchar(void);#ifdef__USE_POSIX199506externintgetc_unlocked(FILE*__stream);externintgetchar_unlocked(void);#endif#ifdef__USE_MISCexternintfgetc_unlocked(FILE*__stream);#endifexternintfputc(int__c,FILE*__stream);externintputc(int__c,FILE*__stream);externintputchar(int__c);#ifdef__USE_MISCexternintfputc_unlocked(int__c,FILE*__stream);#endif#ifdef__USE_POSIX199506externintputc_unlocked(int__c,FILE*__stream);externintputchar_unlocked(int__c);#endif#ifdefined__USE_MISC\||(defined__USE_XOPEN&&!defined__USE_XOPEN2K)externintgetw(FILE*__stream);externintputw(int__w,FILE*__stream);#endifexternchar*fgets(char*__restrict__s,int__n,FILE*__restrict__stream)__wur__fortified_attr_access(__write_only__,1,2);#if__GLIBC_USE(DEPRECATED_GETS)externchar*gets(char*__s)__wur__attribute_deprecated__;#endif#ifdef__USE_GNUexternchar*fgets_unlocked(char*__restrict__s,int__n,FILE*__restrict__stream)__wur__fortified_attr_access(__write_only__,1,2);#endif#ifdefined__USE_XOPEN2K8||__GLIBC_USE(LIB_EXT2)extern__ssize_t__getdelim(char**__restrict__lineptr,size_t*__restrict__n,int__delimiter,FILE*__restrict__stream)__wur;extern__ssize_tgetdelim(char**__restrict__lineptr,size_t*__restrict__n,int__delimiter,FILE*__restrict__stream)__wur;extern__ssize_tgetline(char**__restrict__lineptr,size_t*__restrict__n,FILE*__restrict__stream)__wur;#endifexternintfputs(constchar*__restrict__s,FILE*__restrict__stream);externintputs(constchar*__s);externintungetc(int__c,FILE*__stream);externsize_tfread(void*__restrict__ptr,size_t__size,size_t__n,FILE*__restrict__stream)__wur;externsize_tfwrite(constvoid*__restrict__ptr,size_t__size,size_t__n,FILE*__restrict__s);#ifdef__USE_GNUexternintfputs_unlocked(constchar*__restrict__s,FILE*__restrict__stream);#endif#ifdef__USE_MISCexternsize_tfread_unlocked(void*__restrict__ptr,size_t__size,size_t__n,FILE*__restrict__stream)__wur;externsize_tfwrite_unlocked(constvoid*__restrict__ptr,size_t__size,size_t__n,FILE*__restrict__stream);#endifexternintfseek(FILE*__stream,longint__off,int__whence);externlongintftell(FILE*__stream)__wur;externvoidrewind(FILE*__stream);#ifdefined__USE_LARGEFILE||defined__USE_XOPEN2K#ifndef__USE_FILE_OFFSET64externintfseeko(FILE*__stream,__off_t__off,int__whence);extern__off_tftello(FILE*__stream)__wur;#else#ifdef__REDIRECTexternint__REDIRECT(fseeko,(FILE*__stream,__off64_t__off,int__whence),fseeko64);extern__off64_t__REDIRECT(ftello,(FILE*__stream),ftello64);#else#definefseekofseeko64#defineftelloftello64#endif#endif#endif#ifndef__USE_FILE_OFFSET64externintfgetpos(FILE*__restrict__stream,fpos_t*__restrict__pos);externintfsetpos(FILE*__stream,constfpos_t*__pos);#else#ifdef__REDIRECTexternint__REDIRECT(fgetpos,(FILE*__restrict__stream,fpos_t*__restrict__pos),fgetpos64);externint__REDIRECT(fsetpos,(FILE*__stream,constfpos_t*__pos),fsetpos64);#else#definefgetposfgetpos64#definefsetposfsetpos64#endif#endif#ifdef__USE_LARGEFILE64externintfseeko64(FILE*__stream,__off64_t__off,int__whence);extern__off64_tftello64(FILE*__stream)__wur;externintfgetpos64(FILE*__restrict__stream,fpos64_t*__restrict__pos);externintfsetpos64(FILE*__stream,constfpos64_t*__pos);#endifexternvoidclearerr(FILE*__stream)__THROW;externintfeof(FILE*__stream)__THROW__wur;externintferror(FILE*__stream)__THROW__wur;#ifdef__USE_MISCexternvoidclearerr_unlocked(FILE*__stream)__THROW;externintfeof_unlocked(FILE*__stream)__THROW__wur;externintferror_unlocked(FILE*__stream)__THROW__wur;#endifexternvoidperror(constchar*__s);#ifdef__USE_POSIXexternintfileno(FILE*__stream)__THROW__wur;#endif#ifdef__USE_MISCexternintfileno_unlocked(FILE*__stream)__THROW__wur;#endif#ifdef__USE_POSIX2externintpclose(FILE*__stream);externFILE*popen(constchar*__command,constchar*__modes)__attribute_malloc____attr_dealloc(pclose,1)__wur;#endif#ifdef__USE_POSIXexternchar*ctermid(char*__s)__THROW__attr_access((__write_only__,1));#endif#if(defined__USE_XOPEN&&!defined__USE_XOPEN2K)||defined__USE_GNUexternchar*cuserid(char*__s)__attr_access((__write_only__,1));#endif#ifdef__USE_GNUstructobstack;externintobstack_printf(structobstack*__restrict__obstack,constchar*__restrict__format,...)__THROWNL__attribute__((__format__(__printf__,2,3)));externintobstack_vprintf(structobstack*__restrict__obstack,constchar*__restrict__format,__gnuc_va_list__args)__THROWNL__attribute__((__format__(__printf__,2,0)));#endif#ifdef__USE_POSIX199506externvoidflockfile(FILE*__stream)__THROW;externintftrylockfile(FILE*__stream)__THROW__wur;externvoidfunlockfile(FILE*__stream)__THROW;#endif#ifdefined__USE_XOPEN&&!defined__USE_XOPEN2K&&!defined__USE_GNU#include<bits/getopt_posix.h>#endifexternint__uflow(FILE*);externint__overflow(FILE*,int);#ifdef__USE_EXTERN_INLINES#include<bits/stdio.h>#endif#if__USE_FORTIFY_LEVEL>0&&defined__fortify_function#include<bits/stdio2.h>#endif#include<bits/floatn.h>#ifdefined__LDBL_COMPAT||__LDOUBLE_REDIRECTS_TO_FLOAT128_ABI==1#include<bits/stdio-ldbl.h>#endif__END_DECLS#endif)";
  string actual_source;
  
  while (true) {
    token = lexer.next();
    count ++;
    
    actual_source += *token->value;
    
    if (token->type == CTokenType::CEndOfFile) {
      ASSERT_EQ(expected_source, actual_source);
      break;
    }
  }
}