#include "src/obfuscator/EscapeSequencesInjector.h"
#include "src/obfuscator/FilesCompiler.h"
#include "src/obfuscator/NOPInjector.h"
#include "src/obfuscator/NamesChanger.h"
#include "src/obfuscator/Obfuscator.h"
#include "src/obfuscator/QualityChecker.h"
#include "src/obfuscator/SourcesController.h"
#include "src/obfuscator/TrigraphSequencesInjector.h"
#include <iostream>
#include <string>

using namespace std;

int main( int argc, char **argv ) {
  // Prepare files
  SourcesController *sources = new SourcesController();
  if( argc > 1 ) {
    string sourceName = argv[1];
    sources->setOriginalFilePath( argv[1] );
    cout << sources->getOriginalFilePath();
    if( argc > 2 ) {
      string targetName = argv[2];
      sources->setTargetFilePath( argv[2] );
    }
  }
  Obfuscator *obfuscator = new Obfuscator( sources );
  // Delete comments (TODO)
  // Add NOP equivalents
  NOPInjector nopInjector( *obfuscator );
  obfuscator = &nopInjector;
  obfuscator->reload();
  obfuscator->loadFileContent();
  if( nopInjector.findFuncDefinitions() ) {
    cerr << "No functions definitions found" << endl;
    exit( 1 );
  }
  nopInjector.findPositions( 1 );
  nopInjector.injectForLoops();
  nopInjector.findVarDefinitions();
  nopInjector.findPositions( 0 );
  nopInjector.injectZeros();
  // Erase spaces and new line chars (TODO)
  // Change variable's and function's names
  NamesChanger namesChanger( *obfuscator );
  FilesCompiler filesCompiler( *obfuscator );
  obfuscator = &filesCompiler;
  if( obfuscator->initialCompilation() ) {
    obfuscator = &namesChanger;
    obfuscator->init();
    obfuscator->changeVariablesNames();
  }
  // Add escape sequences
  EscapeSequencesInjector escSeqInjector( *obfuscator );
  escSeqInjector.findStrings();
  escSeqInjector.findPositions();
  string asd = "abcdefg";
  string asdd = "61";
  string asddd = "62";
  //escSeqInjector.convertCharsToHex(asd);
  cout << "UWAGA UWAGA UWAGA" << endl;
  string literaA = escSeqInjector.getEscapeSequence(asdd);
  cout << literaA << endl;
  // Add ?: operator (TODO)
  // Add trigraph sequences
  TrigraphSequencesInjector triSeqInjector( *obfuscator );
  triSeqInjector.findReplacements();
  triSeqInjector.findPositions();
  triSeqInjector.injectTrigraphSequences();
  // Check code quality
  QualityChecker qualityChecker( *obfuscator );
  obfuscator = &qualityChecker;
  obfuscator->reload();
  obfuscator->loadFileContent();
  string x = "1234567890";
  string y = "12345678901";
  const char *source = x.c_str();
  const char *target = y.c_str();
  obfuscator->rateCodeLength( source, target );
  return 0;
}
