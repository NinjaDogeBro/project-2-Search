#include "include/search.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

string cleanToken(const string& token) {
  int firstIndex = 0;
  int lastIndex = token.length() - 1;

  for (int i = 0; i < token.length() - 1; i++) {
    if (ispunct(token[i])) {
      firstIndex++;
    } else {
      break;
    }
  }

  for (int i = token.length() - 1; i >= 0; i--) {
    if (ispunct(token[i])) {
      lastIndex--;
    } else {
      break;
    }
  }

  int countLetters = 0;
  string str = token.substr(firstIndex, lastIndex - firstIndex + 1);

  for (int i = 0; i < str.length(); i++) {
    str[i] = tolower(str[i]);

    if (isalpha(str[i])) {
      countLetters++;
    }
  }

  if (countLetters == 0) {
    str = "";
  }
  return str;
}

set<string> gatherTokens(const string& text) {
  set<string> tokenSet = {};
  string word; 
  string newWord;

  stringstream input(text); 
  
    // To store individual words
  while (input >> word){
    newWord = cleanToken(word);
    if(newWord != ""){
      tokenSet.insert(tokenSet.end(), newWord);
    }
  }
  return tokenSet;
}

int buildIndex(const string& filename, map<string, set<string>>& index) {
  ifstream file;
  file.open (filename);
  string website;
  string sentence;
  int countWebsites = 0;

  //If statement to check if filename is valid
  if(!file.is_open()){
    cout << "Invalid filename." << endl;
    return 0;
  }

  //Goes through each line of file that has website URL's
  while(getline(file, website)){
    //Goes through the next line of file that has the sentence 
    //(USING IF STATEMENT TO ONLY CHECK NEXT LINE AND NOT USE WHILE BECAUSE WHILE GOES THROUGH EVERYTHING)
    countWebsites++;
    if(getline(file, sentence)){
      //Make temp set to seperate each word in the sentence into a set
      set<string> tempSet = gatherTokens(sentence);

      //Now that it's a set, we go through each word and 
      //make it into a key that has the same value of the website URL
      for(string word : tempSet){
        index[word].insert(website);
      }

    }
  }
  file.close();
  
  return countWebsites;
}

set<string> findQueryMatches(const map<string, set<string>>& index, const string& sentence) {
  
  set<string> websiteSet = {}; //Set that I will return that is the output based on user input (Union, difference, or intersection)
  set<string> tempSet = {};
  set<string> resultTempSet = {};

  string word;
  string currWord;
  stringstream input(sentence); 
  bool foundWord = false;
  
    // To store individual words
  while (input >> word){
    tempSet = {};
    currWord = cleanToken(word);
    foundWord = false;

    for (auto it = index.begin(); it!=index.end(); ++it){ //Goes through map of strings as keys of words and set of strings as website values
      if(it->first == currWord){
        foundWord = true;
        for(string website : it->second){
          tempSet.insert(website);
        }
      }
    }

    if(!foundWord){
      return websiteSet = {};
    }

    if(word[0] == '+'){ //WRONG?
      resultTempSet = {};
      set_intersection(websiteSet.begin(), websiteSet.end(),
          tempSet.begin(), tempSet.end(),
          inserter(resultTempSet, resultTempSet.begin()));
          websiteSet = resultTempSet;
    }
    if(word[0] == '-'){ //GOOD
      resultTempSet = {};
      set_difference(websiteSet.begin(), websiteSet.end(),
          tempSet.begin(), tempSet.end(),
          inserter(resultTempSet, resultTempSet.begin()));
          websiteSet = resultTempSet;
    }
    else if (isalpha(word[0])){ //GOOD
      resultTempSet = {};
      set_union(websiteSet.begin(), websiteSet.end(),
          tempSet.begin(), tempSet.end(),
          inserter(resultTempSet, resultTempSet.begin()));
          websiteSet = resultTempSet;
    }  
  }

  return websiteSet;
}

void searchEngine(const string& filename) {
  string website;
  string sentence;
  map<string, set<string>> index;
  int matchingPagesNum = buildIndex(filename, index);

  cout << "Stand by while building index..." << endl;

  int wordCount = index.size();
  cout << "Indexed " << matchingPagesNum << " pages containing " << wordCount << " unique terms" << endl << endl;


  while(1){
    string querySentence;
    
    cout << "Enter query sentence (press enter to quit): ";
    getline(cin, querySentence);

    if(querySentence.empty()){
      cout << "Thank you for searching!" << endl;
      break;
    }
    
    set<string> matchWebsites = findQueryMatches(index, querySentence);
    
    cout << "Found " << matchWebsites.size() << " matching pages" << endl;

    for(string website : matchWebsites){
      cout << website << endl;
    }
  }
}
