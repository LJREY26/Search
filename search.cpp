#include "include/search.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

/*
PROJET #: 2
NAME: LEMUEL REYES
YEAR: 2ND YEAR
COURSE: CS_251
PROFESSOR: AYALA
UIN: 655204254
*/

using namespace std;

string cleanToken(const string& token) {
  /*

  Creates an oss type variable to keep adding into the new token.
  It cleans all punctuation and unique characters

  */
  ostringstream copy;
  int charCount = 0;
  string test = token;
  int firstAlpha = 0;  // First alpha character appearance
  int lastAlpha = 0;   // Last alpha character appearance
  for (char x : token) {
    // Counts if there are characters present
    if (isalpha(x)) {
      charCount++;
    }
  }
  for (int i = 0; i < token.size(); i++) {
    if (!ispunct(token[i])) {
      firstAlpha = i;
      break;
    }
  }

  for (int i = token.size() - 1; i >= 0; i--) {
    if (!ispunct(token[i])) {
      lastAlpha = i + 1;
      break;
    }
  }
  test = token.substr(firstAlpha, (lastAlpha - firstAlpha));

  // while (!isalpha(test[test.size() - 1]) && test.size() != 1) {
  //   test = test.substr(0, test.size() - 1);
  // }

  // for (int i = 0; i < test.size(); i++) {
  //   if (isalpha(test.at(i))) {
  //     copy << string(1, tolower(test.at(i)));
  //     charCount++;
  //   } else {
  //     copy << string(1, test.at(i));
  //   }
  // }
  for (char& x : test) {
    // This will remove all capital letters
    x = tolower(x);
  }

  if (charCount == 0) {
    return "";
  } else {
    return test;
  }
}

set<string> gatherTokens(const string& text) {
  /*

  Creates a set of unique strings or "Tokens" to be used with cleanToken().

  */

  set<string> str;
  string temporaryStr = text;  // This is constantly changing.
  while (temporaryStr.size() != 0) {
    int spaceInd = temporaryStr.find(" ");
    if (spaceInd != string::npos) {
      string word = temporaryStr.substr(0, spaceInd);
      if (cleanToken(word).size() > 0) {
        /*
        Makes sure that the token that is being cleaned is not an empty string.
        */
        str.emplace(cleanToken(word));
      }
      temporaryStr = temporaryStr.substr(spaceInd + 1);
    } else {
      string word = temporaryStr.substr(0);
      if (cleanToken(word).size() > 0) {
        str.emplace(cleanToken(word));
      }
      temporaryStr = "";
    }
  }
  return str;
}

int buildIndex(const string& filename, map<string, set<string>>& index) {
  /*
  You will update the index which will map the word to the websites "word" =
  key, and the websites are the in a set of values,
  Example:
  {"free", {"www.otherexample.com"}}
  or
  {"be", {"www.example.com", "www.otherexample.com"}}
  */
  vector<string> urlVec;
  vector<string> contentVec;
  fstream inFile(filename);
  string line;
  int lineNum = 1;
  if (inFile.is_open()) {
    while (!inFile.eof()) {
      if (lineNum % 2 == 1) {
        getline(inFile, line);
        urlVec.push_back(line);
      } else {
        getline(inFile, line);
        contentVec.push_back(line);
      }
      lineNum++;
    }
    inFile.close();
  } else {
    return 0;
  }

  for (int i = 0; i < urlVec.size() - 1; i++) {
    set<string> temp = gatherTokens(contentVec.at(i));
    for (const auto& x : temp) {
      index[x].insert(urlVec.at(i));
    }
  }

  return max(int(urlVec.size() - 1), 0);
}

set<string> findQueryMatches(const map<string, set<string>>& index,
                             const string& sentence) {
  /*
  This function returns a set that contains websites of tokens that exist
  and are searched for either by a default token or a token with a "+" prefix.
  */

  set<string> tokens = gatherTokens(sentence);
  vector<string> searchTokens;  // The set of the search items are in reverse
  set<string> websites;
  string temporaryStr = sentence;  // This will change constantly
  while (temporaryStr.size() != 0) {
    int spaceInd = temporaryStr.find(" ");
    if (spaceInd != string::npos) {
      string word = temporaryStr.substr(0, spaceInd);
      if (word.size() > 0) {
        /*
        Makes sure that the token that is being cleaned is not an empty string.
        */
        searchTokens.push_back(word);
      }
      temporaryStr = temporaryStr.substr(spaceInd + 1);
    } else {
      string word = temporaryStr.substr(0);
      if (cleanToken(word).size() > 0) {
        searchTokens.push_back(word);
      }
      temporaryStr = "";
    }
  }

  for (string x : searchTokens) {
    /*
    This will loop through each token in the set
    */
    auto sites = index.find(cleanToken(x));
    if (isalpha(x[0])) {
      if (sites != index.end()) {
        const set<string>& urls =
            sites->second;  // This grabs all the websites within that key
                            // "->second" grabs the value while sites is a key
        for (const string& url : urls) {
          websites.emplace(url);
        }
      }
    } else if (x[0] == '-') {
      if (sites != index.end()) {
        const set<string>& urls = sites->second;
        for (const string& url : urls) {
          websites.erase(url);
        }
      }
    } else if (x[0] == '+') {
      if (sites != index.end()) {
        const set<string>& urls = sites->second;
        set<string> match = {};
        for (const string& url : urls) {
          if (websites.count(url) == 1) {
            match.insert(url);
          }
        }
        websites = move(match);
      } else {
        websites.clear();
      }
    }
  }

  return websites;
}

void searchEngine(const string& filename) {
  /*
  This is the entire function that uses all the functions above.
  */

  vector<string> websitesFound = {};
  vector<string> contentVec = {};
  set<string> uniqueTerms = {};

  string userInput;
  map<string, set<string>> index = {};
  int passed = buildIndex(filename, index);

  fstream inFile(filename);
  if (passed == 0) {
    cout << "Invalid filename." << endl;
  }
  cout << "Stand by while building index..." << '\n';
  string line;
  int lineNum = 1;
  if (inFile.is_open()) {
    while (!inFile.eof()) {
      if (lineNum % 2 == 1) {
        getline(inFile, line);
        websitesFound.push_back(line);  // This will lead to printing out ##
                                        // pages containing... by using .size()
      } else {
        getline(inFile, line);
        contentVec.push_back(line);
      }
      lineNum++;
    }
    inFile.close();
  }

  for (string x : contentVec) {
    set<string> tokens = gatherTokens(x);
    for (string t : tokens) {
      uniqueTerms.insert(t);
    }
  }
  if (passed == 0) {
    websitesFound = {"nothing"};
    uniqueTerms = {};
  }
  cout << "Indexed " << websitesFound.size() - 1 << " pages containing "
       << uniqueTerms.size() << " unique terms" << '\n'
       << endl;

  cout << "Enter query sentence (press enter to quit): ";
  getline(cin, userInput);
  while (userInput.size() != 0) {
    set<string> sites = findQueryMatches(index, userInput);
    cout << "Found " << sites.size() << " matching pages" << '\n';
    for (string s : sites) {
      cout << s << '\n';
    }
    cout << endl;
    cout << "Enter query sentence (press enter to quit): ";
    getline(cin, userInput);
  }
  cout << "Thank you for searching!";
}
