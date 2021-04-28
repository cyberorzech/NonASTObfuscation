#include <gtest/gtest.h>
#include "../obfuscator/Obfuscator.h"
#include "../obfuscator/SourcesController.h"
#include "../obfuscator/FilesCompiler.h"
#include "../obfuscator/NamesChanger.h"

using ::testing::Test;

class obfuscationTests : public Test{
    protected: 
        void SetUp() override {
            testFile = "../../../src/test/testFile.cc";
            targetFile = "../../../src/test/dumpTestFile.cc";
            obf = new Obfuscator(testFile,targetFile);
            }
        Obfuscator* obf;
        std::string testFile;
        std::string targetFile;

};

TEST_F(obfuscationTests, compile_source_file){
    FilesCompiler compiler(testFile,targetFile);
    obf = &compiler;
    ASSERT_TRUE(obf->initialCompilation());
}

// Very last test (deletes Obfuscator)
TEST_F(obfuscationTests, compile_target_file){
    NamesChanger changer(testFile,targetFile);
    obf = &changer;
    obf->init();
    obf->changeVariablesNames();
    obf->close();
    ASSERT_TRUE(FilesCompiler::fileCompilation(targetFile));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}