#include "src/obfuscator/CommentsCleaner.h"
#include "src/obfuscator/FilesCompiler.h"
#include "src/obfuscator/IfConditionChanger.h"
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
  CommentsCleaner commentsCleaner( *obfuscator );
  FilesCompiler filesCompiler( *obfuscator );
  obfuscator = &filesCompiler;
  if( obfuscator->initialCompilation() ) {
    obfuscator = &commentsCleaner;
    obfuscator->init();
    obfuscator->cleanComments();
  }

  // Add NOP equivalents
  // NOPInjector nopInjector( *obfuscator );
  // obfuscator = &nopInjector;
  // obfuscator->reload();
  // obfuscator->loadFileContent();
  // obfuscator->runNOPInjector();
  // // Erase spaces and new line chars (TODO)
  // // Change variable's and function's names
  NamesChanger namesChanger( *obfuscator );
  obfuscator = &namesChanger;
  obfuscator->reload();
  obfuscator->changeVariablesNames();
  // }
  // // // Add escape sequences (TODO)
  // // // Add ?: operator (TODO)
  // // // Add trigraph sequences
  // TrigraphSequencesInjector triSeqInjector( *obfuscator );
  // obfuscator = &triSeqInjector;
  // obfuscator->reload();
  // obfuscator->runTrigraphSequencesInjector();
  // // // Check code quality
  // QualityChecker qualityChecker( *obfuscator );
  // obfuscator = &qualityChecker;
  // obfuscator->reload();
  // obfuscator->loadFileContent();
  // obfuscator->rateCodeLength();

  IfConditionChanger ifConditionChanger( *obfuscator );
  obfuscator = &ifConditionChanger;
  obfuscator->reload();
  obfuscator->rebuildIfStatement();
  delete sources;
  obfuscator->close();
  return 0;
}
