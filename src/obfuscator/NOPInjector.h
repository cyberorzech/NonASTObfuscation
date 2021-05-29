#ifndef NOPInjector_H
#define NOPInjector_H

#include <stdlib.h>
#include <time.h>
#include <regex>

#include "Obfuscator.h"

class NOPInjector : public Obfuscator {
  int randomX, randomY;
  std::string foundFunctions;

public:
  NOPInjector( Obfuscator obf ) : Obfuscator( obf ) {
    srand( time( NULL ) ); 
    randomX = rand() % 10 + 1;
    randomY = rand() % 50 + 1;
  }
  void findRegexMatches(std::string str, std::regex reg);
  bool findFuncDefinitions();          // returns string with definitions
  int getRandomValues( bool );
  bool findFunctionsPos();             // return positions of found functions
  bool injectForLoops( const char * );

  bool findKeywords();
  bool injectZeros();
  bool injectOR();
};

#endif