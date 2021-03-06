#include "../obfuscator/CommentsCleaner.h"
#include "../obfuscator/FilesCompiler.h"
#include "../obfuscator/NOPInjector.h"
#include "../obfuscator/NamesChanger.h"
#include "../obfuscator/Obfuscator.h"
#include "../obfuscator/QualityChecker.h"
#include "../obfuscator/SourcesController.h"
#include "../obfuscator/TrigraphSequencesInjector.h"
#include <gtest/gtest.h>

using ::testing::Test;

class obfuscationTests : public Test {
protected:
  void SetUp() override {
    testFile = "../../../src/test/testFiles/testFile.cc";
    targetFile = "../../../src/test/testFiles/dumpTestFile.cc";
    obf = new Obfuscator( testFile, targetFile );
  }
  Obfuscator *obf;
  std::string testFile;
  std::string targetFile;
};

TEST_F( obfuscationTests, compile_source_file ) {
  FilesCompiler compiler( testFile, targetFile );
  obf = &compiler;
  ASSERT_TRUE( FilesCompiler::fileCompilation( testFile ) );
  ASSERT_TRUE( obf->initialCompilation() );
}

TEST_F( obfuscationTests, quality_check ) {
  QualityChecker qualityChecker( *obf );
  std::string x = "10digitlen";
  std::string y = "11digitlenx";
  const char *source = x.c_str();
  const char *target = y.c_str();
}

TEST_F( obfuscationTests, nop_injector ) {
  // Random values tests
  NOPInjector nopInjector( *obf );
  nopInjector.randomNoGen();
  ASSERT_GE( nopInjector.getRandomValues( true ), 1 );
  ASSERT_LE( nopInjector.getRandomValues( true ), 10 );
  ASSERT_GE( nopInjector.getRandomValues( false ), 1 );
  ASSERT_LE( nopInjector.getRandomValues( false ), 50 );
  // Regexp tests
  std::string variablesText =
  "#@#@^&$SF DGSint x = 2;!@DFY$float y = 2.22222;*";
  std::string functionsText = "#include <iostream>\nint main() {\nreturn 0;\n}";

  const std::regex variablesReg(
  "(bool||char||double||float||int||long||short||void)[ ][a-zA-Z]+[ ](=)[ "
  "][0-9]+(.)?[0-9]*" );
  const std::regex functionsReg(
  "(bool||char||double||float||int||long||short||void)[ "
  "][a-zA-Z]+(\\(((\\w)+[ ]+(\\w)+(,)*[ ]*)*\\))+[ ](\\{)+" );

  const std::string variablesCorrectResult = "int x = 2;\nfloat y = 2.22222";
  const std::string functionsCorrectResult = "int main() {";

  std::vector<std::string> foundFunc =
  nopInjector.findRegexMatches( functionsText, functionsReg );
  for( int i = 0; i < foundFunc.size(); ++i ) {
    std::string currentString = foundFunc[i];
    ASSERT_STREQ( currentString.c_str(), functionsCorrectResult.c_str() );
  }
  // findFuncDefinitions tests
  ASSERT_FALSE( nopInjector.findFuncDefinitions() );

  ASSERT_FALSE( nopInjector.findPositions( 1 ) );
  const std::vector<std::size_t> foundFuncPositions =
  nopInjector.getFuncPositions();
  ASSERT_GE( foundFuncPositions[0], 0 );
  for( int i = 1; i < foundFuncPositions.size(); ++i ) {
    ASSERT_GT( foundFuncPositions[i], foundFuncPositions[i - 1] );
  }

  ASSERT_FALSE( nopInjector.findVarDefinitions() );
  ASSERT_FALSE( nopInjector.findPositions( 0 ) );
  const std::vector<std::size_t> foundVarPositions =
  nopInjector.getVarPositions();
  ASSERT_GE( foundVarPositions[0], 0 );
  for( int i = 1; i < foundVarPositions.size(); ++i ) {
    ASSERT_GT( foundVarPositions[i], foundVarPositions[i - 1] );
  }
}

TEST_F( obfuscationTests, trigraph_sequences_injector ) {
  ASSERT_TRUE( true );
}
TEST_F( obfuscationTests, comments_cleaner ) {

  CommentsCleaner cleaner(
  "../../../src/test/testFiles/commentsTest.cc",
  "../../../src/test/testFiles/commentsTestOutput.cc" );
  cleaner.init();
  cleaner.cleanComments();
  cleaner.close();
  ASSERT_EQ( "int main() {} ", cleaner.getOutput() );

}          // Very last test (deletes Obfuscator)
TEST_F( obfuscationTests, compile_target_file ) {
  NamesChanger changer( testFile, targetFile );
  obf = &changer;
  obf->init();
  obf->changeVariablesNames();
  obf->close();
  ASSERT_TRUE( FilesCompiler::fileCompilation( targetFile ) );
}

int main( int argc, char **argv ) {
  testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}