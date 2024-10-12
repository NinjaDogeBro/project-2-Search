#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/search.h"

using namespace std;
using namespace testing;

TEST(CleanToken, noPunctuationOrSpecialChar) {
  EXPECT_THAT(cleanToken("Gizmo"), StrEq("gizmo"));
}

// Tokens with punctuation at the beginning

TEST(CleanToken, punctuationInFront) {
  EXPECT_THAT(cleanToken(".Gizmo"), StrEq("gizmo"));
}

// Tokens with punctuation at the end
TEST(CleanToken, punctuationAtEnd) {
  EXPECT_THAT(cleanToken("Gizmo."), StrEq("gizmo"));
}

// Tokens without any letters
TEST(CleanToken, noLetters) {
  EXPECT_THAT(cleanToken("12344"), StrEq(""));
}

// Tokens with uppercase letters, and possibly punctuation
TEST(CleanToken, upperCaseAndPunctuation) {
  EXPECT_THAT(cleanToken(".OHMYGOD!!"), StrEq("ohmygod"));
}

// Missing test for: punctuation in middle, and not at either end
TEST(CleanToken, punctuationInMiddle) {
  EXPECT_THAT(cleanToken("hello!!!bro"), StrEq("hello!!!bro"));
}

// Missing test for: punctuation in middle, and at start (but not end)
TEST(CleanToken, punctuationInMiddleAndStart) {
  EXPECT_THAT(cleanToken("!!OHMY!!GOD"), StrEq("ohmy!!god"));
}

// Missing test for: punctuation in middle, and at end (but not start)
TEST(CleanToken, punctuationInMiddleAndEnd) {
  EXPECT_THAT(cleanToken("OHMY!!GOD!!"), StrEq("ohmy!!god"));
}

// _________________________________________________________________________________________
// _________________________________________________________________________________________

// Missing test for: space(s) at beginning of text
TEST(GatherTokens, spacesAtBeginningOfText) {
  string text = " this has a space";
  set<string> expected = {"this", "has", "a", "space"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected)) << "text=\"" << text << "\"";

}

// Missing test for: space(s) at end of text
TEST(GatherTokens, spacesAtEndOfText) {
  string text = "this has a space ";
  set<string> expected = {"this", "has", "a", "space"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected)) << "text=\"" << text << "\"";
}

// Missing test for: multiple spaces between tokens
TEST(GatherTokens, multipleSpacesBetweenTokens) {
  string text = "this  has  a   space";
  set<string> expected = {"this", "has", "a", "space"};

  EXPECT_THAT(gatherTokens(text), ContainerEq(expected)) << "text=\"" << text << "\"";
}

// _________________________________________________________________________________________
// _________________________________________________________________________________________

// That you run against data/tiny.txt
TEST(BuildIndex, TinyTxt) {
  string filename = "data/tiny.txt";
  map<string, set<string>> expectedIndex = {

      {"blue", { "www.dr.seuss.net", "www.rainbow.org" }}, 
      {"bread", { "www.shoppinglist.com" }}, 
      {"cheese", { "www.shoppinglist.com" }}, 
      {"eat", { "www.bigbadwolf.com" }}, 
      {"eggs", { "www.shoppinglist.com" }}, 
      {"fish", { "www.dr.seuss.net", "www.shoppinglist.com" }}, 
      {"gre-en", { "www.rainbow.org" }}, 
      {"i'm", { "www.bigbadwolf.com" }}, 
      {"indigo", { "www.rainbow.org" }}, 
      {"milk", { "www.shoppinglist.com" }}, 
      {"not", { "www.bigbadwolf.com" }}, 
      {"one", { "www.dr.seuss.net" }}, 
      {"orange", { "www.rainbow.org" }}, 
      {"red", { "www.dr.seuss.net", "www.rainbow.org" }}, 
      {"to", { "www.bigbadwolf.com" }}, 
      {"trying", { "www.bigbadwolf.com" }}, 
      {"two", { "www.dr.seuss.net" }}, 
      {"violet", { "www.rainbow.org" }}, 
      {"yellow", { "www.rainbow.org" }}, 
      {"you", { "www.bigbadwolf.com" }},
  };
  map<string, set<string>> studentIndex;
  int studentNumProcesed = buildIndex(filename, studentIndex);

  string indexTestFeedback =
      "buildIndex(\"" + filename + "\", ...) index incorrect\n";
  EXPECT_THAT(studentIndex, ContainerEq(expectedIndex)) << indexTestFeedback;

  string retTestFeedback =
      "buildIndex(\"" + filename + "\", ...) return value incorrect\n";
  EXPECT_THAT(studentNumProcesed, Eq(4)) << retTestFeedback;
}
// When the file is not found.
TEST(BuildIndex, fileNotFound) {
  string filename = "data/lololol.txt";
  map<string, set<string>> expectedIndex = {

  };

  map<string, set<string>> studentIndex;
  int studentNumProcesed = buildIndex(filename, studentIndex);

  string indexTestFeedback =
      "buildIndex(\"" + filename + "\", ...) index incorrect\n";
  EXPECT_THAT(studentIndex, ContainerEq(expectedIndex)) << indexTestFeedback;

  string retTestFeedback =
      "buildIndex(\"" + filename + "\", ...) return value incorrect\n";
  EXPECT_THAT(studentNumProcesed, Eq(0)) << retTestFeedback;
}

// _________________________________________________________________________________________
// _________________________________________________________________________________________

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

// Missing test for: first search term not in map
TEST(FindQueryMatches, wordNotInMap) {
  set<string> expected;

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "eggs"),
              ContainerEq(expected));

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "cheese"), ContainerEq(expected));

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "fomage"),
              ContainerEq(expected));
}

// Missing test for: later search term modified with '+' not in map
TEST(FindQueryMatches, modifiedWithPlus) {
  set<string> expected;

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "laws of +eggs"),
              ContainerEq(expected));

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "laws of +eggs"), ContainerEq(expected));
}

// Missing test for: later search term modified with '-' not in map
TEST(FindQueryMatches, modifiedWithMinus) {
  set<string> expected;

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "c -eggs"), ContainerEq(expected));
}

// Missing test for: later search term (unmodified) not in map
TEST(FindQueryMatches, laterSearchTermNotInMap) {
  set<string> expected;

  expected = {};
  EXPECT_THAT(findQueryMatches(INDEX, "c cows"), ContainerEq(expected));
}
