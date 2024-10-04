#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/search.h"

using namespace std;
using namespace testing;

/*
CLEAN TOKENS TEST || TOP
*/
TEST(CleanTokens, Punctutations) {
  EXPECT_THAT(cleanToken(".!TEST"), StrEq("test"));
  EXPECT_THAT(cleanToken("TEST!."), StrEq("test"));
  EXPECT_THAT(cleanToken(".!TEST!."), StrEq("test"));
  EXPECT_THAT(cleanToken("!TE-ST!"), StrEq("te-st"));
}

TEST(CleanTokens, firstAndLastNumber) {
  string test = "26TEST26";
  string expected = "26test26";

  EXPECT_THAT(cleanToken(test), StrEq(expected)) << "text=\"" << test << "\"";
}

TEST(CleanTokens, punctuation_extended) {
  EXPECT_THAT(cleanToken("Te.st"), StrEq("te.st"));
  EXPECT_THAT(cleanToken("tE.ST@"), StrEq("te.st"));
  EXPECT_THAT(cleanToken("@tE.St"), StrEq("te.st"));
}

/*
CLEAN TOKENS TEST || BOTTOM
*/

/*
GATHER TOKENS TEST || TOP
*/

TEST(GatherTokens, NoWords) {
  string test = "";
  set<string> expected = {};

  EXPECT_THAT(gatherTokens(test), ContainerEq(expected))
      << "text=\"" << test << "\"";
}
TEST(GatherTokens, SingleWordOne) {
  string test = "Test";
  set<string> expected = {"test"};

  EXPECT_THAT(gatherTokens(test), ContainerEq(expected))
      << "text=\"" << test << "\"";
}
TEST(GatherTokens, SingleWordTwo) {
  string test = "Apples";
  set<string> expected = {"apples"};

  EXPECT_THAT(gatherTokens(test), ContainerEq(expected))
      << "text=\"" << test << "\"";
}
TEST(GatherTokens, SingleWordThree) {
  string test = "flamEs";
  set<string> expected = {"flames"};

  EXPECT_THAT(gatherTokens(test), ContainerEq(expected))
      << "text=\"" << test << "\"";
}
TEST(GatherTokens, MultiWordsOne) {
  string test = "gO flamEs";
  set<string> expected = {"go", "flames"};

  EXPECT_THAT(gatherTokens(test), ContainerEq(expected))
      << "text=\"" << test << "\"";
}
TEST(GatherTokens, MultiWordsTwo) {
  string test = "This here is a TEST";
  set<string> expected = {"this", "here", "is", "a", "test"};

  EXPECT_THAT(gatherTokens(test), ContainerEq(expected))
      << "text=\"" << test << "\"";
}

TEST(GatherTokens, Spaces1) {
  string test = "  This is a test";
  set<string> expected = {"this", "is", "a", "test"};

  EXPECT_THAT(gatherTokens(test), ContainerEq(expected))
      << "text=\"" << test << "\"";
}
TEST(GatherTokens, Spaces2) {
  string test = " This is a test  ";
  set<string> expected = {"this", "is", "a", "test"};

  EXPECT_THAT(gatherTokens(test), ContainerEq(expected))
      << "text=\"" << test << "\"";
}
TEST(GatherTokens, SpacesFinal) {
  string test = " This    is  a test";
  set<string> expected = {"this", "is", "a", "test"};

  EXPECT_THAT(gatherTokens(test), ContainerEq(expected))
      << "text=\"" << test << "\"";
}
/*
GATHER TOKENS TEST || BOTTOM
*/

/*
BUILD INDEX TEST || TOP
*/

TEST(BuildIndex, TinyTxt) {
  string filename = "data/tiny.txt";
  map<string, set<string>> expectedIndex = {
      {"eggs", {"www.shoppinglist.com"}},
      {"milk", {"www.shoppinglist.com"}},
      {"fish", {"www.shoppinglist.com", "www.dr.seuss.net"}},
      {"bread", {"www.shoppinglist.com"}},
      {"cheese", {"www.shoppinglist.com"}},
      {"red", {"www.rainbow.org", "www.dr.seuss.net"}},
      {"gre-en", {"www.rainbow.org"}},
      {"orange", {"www.rainbow.org"}},
      {"yellow", {"www.rainbow.org"}},
      {"blue", {"www.rainbow.org", "www.dr.seuss.net"}},
      {"indigo", {"www.rainbow.org"}},
      {"violet", {"www.rainbow.org"}},
      {"one", {"www.dr.seuss.net"}},
      {"two", {"www.dr.seuss.net"}},
      {"i'm", {"www.bigbadwolf.com"}},
      {"not", {"www.bigbadwolf.com"}},
      {"trying", {"www.bigbadwolf.com"}},
      {"to", {"www.bigbadwolf.com"}},
      {"eat", {"www.bigbadwolf.com"}},
      {"you", {"www.bigbadwolf.com"}}};
  map<string, set<string>> studentIndex;
  int studentNumProcesed = buildIndex(filename, studentIndex);

  string indexTestFeedback =
      "buildIndex(\"" + filename + "\", ...) index incorrect\n";
  EXPECT_THAT(studentIndex, ContainerEq(expectedIndex)) << indexTestFeedback;

  string retTestFeedback =
      "buildIndex(\"" + filename + "\", ...) return value incorrect\n";
  EXPECT_THAT(studentNumProcesed, Eq(4)) << retTestFeedback;
}

TEST(BuildIndex, Invalid_Filename) {
  string filename = "data/invalid.txt";
  map<string, set<string>> expectedIndex = {};
  map<string, set<string>> studentIndex;
  int studentNumProcesed = buildIndex(filename, studentIndex);
  string indexTestFeedback =
      "buildIndex(\"" + filename + "\", ...) index incorrect\n";
  EXPECT_THAT(studentIndex, ContainerEq(expectedIndex)) << indexTestFeedback;

  string retTestFeedback =
      "buildIndex(\"" + filename + "\", ...) return value incorrect\n";
  EXPECT_THAT(studentNumProcesed, Eq(0)) << retTestFeedback;
}
/*
BUILD INDEX TEST || BOTTOM
*/

/*
FIND QUERY TEST || TOP
*/
map<string, set<string>> INDEX = {
    {"hello", {"example.com", "uic.edu"}},
    {"there", {"example.com"}},
    {"according", {"uic.edu"}},
    {"to", {"uic.edu"}},
    {"all", {"example.com", "uic.edu", "random.org"}},
    {"known", {"uic.edu"}},
    {"laws", {"random.org"}},
    {"of", {"random.org"}},
    {"aviation", {"random.org"}},
    {"a", {"uic.edu", "random.org"}},
};

TEST(FindQueryMatches, noSearch) {
  set<string> expected;

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "theres"), ContainerEq(expected));

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "welcome!"), ContainerEq(expected));
  EXPECT_THAT(findQueryMatches(INDEX, "Greetings!"), ContainerEq(expected));
}

TEST(FindQueryMatches, noPlus) {
  set<string> expected;

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "hello +all +random"),
              ContainerEq(expected));

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "hello +aviations"),
              ContainerEq(expected));
}

TEST(FindQueryMatches, noMinus) {
  set<string> expected;

  expected = {"example.com", "uic.edu"};
  EXPECT_THAT(findQueryMatches(INDEX, "hello -theres"), ContainerEq(expected));

  expected = {"example.com", "uic.edu"};
  EXPECT_THAT(findQueryMatches(INDEX, "hello -welcome!"),
              ContainerEq(expected));
  EXPECT_THAT(findQueryMatches(INDEX, "hello -Greetings!"),
              ContainerEq(expected));
}

TEST(FindQueryMatches, unModified) {
  set<string> expected;

  expected = {"example.com", "uic.edu"};
  EXPECT_THAT(findQueryMatches(INDEX, "hello theres"), ContainerEq(expected));

  expected = {"example.com", "uic.edu"};
  EXPECT_THAT(findQueryMatches(INDEX, "hello welcome!"), ContainerEq(expected));
  EXPECT_THAT(findQueryMatches(INDEX, "hello Greetings!"),
              ContainerEq(expected));
}