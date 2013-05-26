#include "unit/TestRunner.h"
#include "base/StringUtilities.h"

static int _testIsLetter(void) {
  assert(isLetter('a'));
  assert(isLetter('A'));
  return 0;
}

static int _testIsNotLetter(void) {
  assertFalse(isLetter('0'));
  return 0;
}

static int _testIsNumber(void) {
  assert(isNumber('0'));
  return 0;
}

static int _testIsNotNumber(void) {
  assertFalse(isNumber('a'));
  return 0;
}

static int _testWrapNullSourceString(void) {
  assertFalse(wrapString(NULL, 0));
  return 0;
}


static int _testConvertIntIdToString(void) {
  CharString test = newCharStringWithCString("abcd");
  unsigned long id = convertStringIdToInt(test);
  assertIntEquals(id, 0x61626364);
  freeCharString(test);
  return 0;
}

static int _testConvertZeroIntIdToString(void) {
  CharString c = convertIntIdToString(0);
  assert(charStringIsEmpty(c));
  freeCharString(c);
  return 0;
}

static int _testConvertStringIdToInt(void) {
  CharString c = convertIntIdToString(0x61626364);
  assertCharStringEquals(c, "abcd");
  freeCharString(c);
  return 0;
}

static int _testConvertEmptyStringIdToInt(void) {
  CharString c = newCharString();
  unsigned long id = convertStringIdToInt(c);
  assertIntEquals(id, 0);
  freeCharString(c);
  return 0;
}

static int _testConvertNullStringIdToInt(void) {
  unsigned long id = convertStringIdToInt(NULL);
  assertIntEquals(id, 0);
  return 0;
}

static int _testConvertInvalidStringIdToInt(void) {
  CharString c = newCharStringWithCString("a");
  unsigned long id = convertStringIdToInt(c);
  assertIntEquals(id, 0);
  freeCharString(c);
  return 0;
}

TestSuite addStringUtilitiesTests(void);
TestSuite addStringUtilitiesTests(void) {
  TestSuite testSuite = newTestSuite("StringUtilities", NULL, NULL);
  addTest(testSuite, "IsLetter", _testIsLetter);
  addTest(testSuite, "IsNotLetter", _testIsNotLetter);
  addTest(testSuite, "IsNumber", _testIsNumber);
  addTest(testSuite, "IsNotNumber", _testIsNotNumber);

  addTest(testSuite, "ConvertIntIdToString", _testConvertIntIdToString);
  addTest(testSuite, "ConvertZeroIntIdToString", _testConvertZeroIntIdToString);
  addTest(testSuite, "ConvertStringIdToInt", _testConvertStringIdToInt);
  addTest(testSuite, "ConvertNullStringIdToInt", _testConvertNullStringIdToInt);
  addTest(testSuite, "ConvertEmptyStringIdToInt", _testConvertEmptyStringIdToInt);
  addTest(testSuite, "ConvertInvalidStringIdToInt", _testConvertInvalidStringIdToInt);

  return testSuite;
}
