#ifndef Obfuscator_H
#define Obfuscator_H

#include <string>
#include <fstream>
#include "SourcesController.h"
#include <iostream>
class Obfuscator{
    public:
        Obfuscator(std::string oFP = "../testFile.cc", std::string tFP = "../obfuscatedFile.cc"){src = new SourcesController(oFP,tFP);}
        Obfuscator(SourcesController* source){src=source;}
        ~Obfuscator(){delete src;}
        virtual void changeVariablesNames(){}
        virtual void changeFunctionNames(){}
        virtual void fileCompilation(){}
        virtual bool initialCompilation(){return false;}
        virtual void init();
        void close();
    protected:
        SourcesController* src;
};

#endif