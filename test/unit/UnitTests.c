//
// UnitTests.c - MrsWatson
// Copyright (c) 2016 Teragon Audio. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#include "base/File.h"
#include "base/LinkedList.h"
#include "unit/TestRunner.h"

#include <stdio.h>
#include <stdlib.h>

extern TestSuite addAudioClockTests(void);
extern TestSuite addAudioSettingsTests(void);
extern TestSuite addCharStringTests(void);
extern TestSuite addEndianTests(void);
extern TestSuite addFileTests(void);
extern TestSuite addLinkedListTests(void);
extern TestSuite addMidiSequenceTests(void);
extern TestSuite addMidiSourceTests(void);
extern TestSuite addPcmSampleBufferTests(void);
extern TestSuite addPlatformInfoTests(void);
extern TestSuite addPluginTests(void);
extern TestSuite addPluginChainTests(void);
extern TestSuite addPluginPresetTests(void);
extern TestSuite addPluginVst2xIdTests(void);
extern TestSuite addProgramOptionTests(void);
extern TestSuite addSampleBufferTests(void);
extern TestSuite addSampleSourceTests(void);
extern TestSuite addTaskTimerTests(void);

extern TestSuite addAnalysisClippingTests(void);
extern TestSuite addAnalysisDistortionTests(void);
extern TestSuite addAnalysisSilenceTests(void);
extern TestSuite addIntegrationTests(File mrsWatsonExePath, File resourcesPath);

extern void _printTestSummary(int testsRun, int testsPassed, int testsFailed,
                              int testsSkipped);

static void _sumTestSuiteResults(void *item, void *extraData) {
  TestSuite testSuite = (TestSuite)item;
  TestSuite result = (TestSuite)extraData;
  result->numSuccess += testSuite->numSuccess;
  result->numFail += testSuite->numFail;
  result->numSkips += testSuite->numSkips;
}

LinkedList getTestSuites(File mrsWatsonExePath, File resourcesPath);
LinkedList getTestSuites(File mrsWatsonExePath, File resourcesPath) {
  LinkedList unitTestSuites = newLinkedList();

  linkedListAppend(unitTestSuites, addAudioClockTests());
  linkedListAppend(unitTestSuites, addAudioSettingsTests());
  linkedListAppend(unitTestSuites, addCharStringTests());
  linkedListAppend(unitTestSuites, addEndianTests());
  linkedListAppend(unitTestSuites, addFileTests());
  linkedListAppend(unitTestSuites, addLinkedListTests());
  linkedListAppend(unitTestSuites, addMidiSequenceTests());
  linkedListAppend(unitTestSuites, addMidiSourceTests());
  linkedListAppend(unitTestSuites, addPcmSampleBufferTests());
  linkedListAppend(unitTestSuites, addPlatformInfoTests());
  linkedListAppend(unitTestSuites, addPluginTests());
  linkedListAppend(unitTestSuites, addPluginChainTests());
  linkedListAppend(unitTestSuites, addPluginPresetTests());
  linkedListAppend(unitTestSuites, addPluginVst2xIdTests());
  linkedListAppend(unitTestSuites, addProgramOptionTests());
  linkedListAppend(unitTestSuites, addSampleBufferTests());
  linkedListAppend(unitTestSuites, addSampleSourceTests());
  linkedListAppend(unitTestSuites, addTaskTimerTests());

  linkedListAppend(unitTestSuites, addAnalysisClippingTests());
  linkedListAppend(unitTestSuites, addAnalysisDistortionTests());
  linkedListAppend(unitTestSuites, addAnalysisSilenceTests());

  const boolByte mrsWatsonExists = fileExists(mrsWatsonExePath);
  if (!mrsWatsonExists) {
    printf("WARNING: mrswatson executable not found, skipping integration "
           "tests\n");
  }

  const boolByte resourcesExist = fileExists(resourcesPath);
  if (!resourcesExist) {
    printf("WARNING: Test resources not found, skipping integration tests\n");
  }

  if (mrsWatsonExists && resourcesExist) {
    linkedListAppend(unitTestSuites,
                     addIntegrationTests(mrsWatsonExePath, resourcesPath));
  }

  return unitTestSuites;
}

static void _setTestSuiteOnlyPrintFailing(void *item, void *userData) {
  TestSuite testSuite = (TestSuite)item;
  testSuite->onlyPrintFailing = true;
}

TestSuite runUnitTests(LinkedList testSuites, boolByte onlyPrintFailing);
TestSuite runUnitTests(LinkedList testSuites, boolByte onlyPrintFailing) {
  TestSuite suiteResults;

  if (onlyPrintFailing) {
    linkedListForeach(testSuites, _setTestSuiteOnlyPrintFailing, NULL);
  }

  linkedListForeach(testSuites, runTestSuite, NULL);
  // Create a new test suite to be used as the userData passed to the foreach
  // loop
  suiteResults = newTestSuite("Suite results", NULL, NULL);
  linkedListForeach(testSuites, _sumTestSuiteResults, suiteResults);

  _printTestSummary(
      suiteResults->numSuccess + suiteResults->numFail + suiteResults->numSkips,
      suiteResults->numSuccess, suiteResults->numFail, suiteResults->numSkips);

  return suiteResults;
}

TestCase findTestCase(TestSuite testSuite, char *testName);
TestCase findTestCase(TestSuite testSuite, char *testName) {
  LinkedList iterator = testSuite->testCases;
  TestCase currentTestCase = NULL;

  while (iterator != NULL) {
    if (iterator->item != NULL) {
      currentTestCase = (TestCase)iterator->item;

      if (!strncasecmp(currentTestCase->name, testName,
                       strlen(currentTestCase->name))) {
        return currentTestCase;
      }
    }

    iterator = iterator->nextItem;
  }

  return NULL;
}

TestSuite findTestSuite(LinkedList testSuites, const CharString testSuiteName);
TestSuite findTestSuite(LinkedList testSuites, const CharString testSuiteName) {
  LinkedList iterator = testSuites;
  TestSuite testSuite = NULL;

  if (testSuiteName == NULL || charStringIsEmpty(testSuiteName)) {
    return NULL;
  }

  while (iterator != NULL) {
    if (iterator->item != NULL) {
      testSuite = (TestSuite)iterator->item;

      if (charStringIsEqualToCString(testSuiteName, testSuite->name, true)) {
        break;
      } else {
        testSuite = NULL;
      }
    }

    iterator = iterator->nextItem;
  }

  return testSuite;
}

static void _printTestCases(void *item, void *userData) {
  TestCase testCase = (TestCase)item;
  char *testSuiteName = (char *)userData;
  printf("%s:%s\n", testSuiteName, testCase->name);
}

static void _printTestsInSuite(void *item, void *userData) {
  TestSuite testSuite = (TestSuite)item;
  linkedListForeach(testSuite->testCases, _printTestCases, testSuite->name);
}

void printUnitTestSuites(void);
void printUnitTestSuites(void) {
  File dummy = newFile();
  LinkedList unitTestSuites = getTestSuites(dummy, dummy);
  linkedListForeach(unitTestSuites, _printTestsInSuite, NULL);
  freeLinkedListAndItems(unitTestSuites, (LinkedListFreeItemFunc)freeTestSuite);
  freeFile(dummy);
}
