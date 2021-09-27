/* ------------------------------------------------
 * main.cpp
 *
 * Class: Program #3 for CS 141, Fall 2019
 * Syroot: Codio, Repl.it, Mac OS X
 * Author: Amaan Hussain & William Kopec
 * 
 *
*/
#include <iostream>
#include <string>
#include <fstream>      // For file input
#include <vector>		// For dictionary vector
#include <iomanip>		// For setw()
#include <cctype>       // For tolower()
using namespace std;

// Display ID info
void displayIdInfo()
{
    cout << "Program #3: Work Play  \n"
         << "Author: Dale Reed      \n"
         << "Lab: Tues 8am          \n"
         << "System:  Codio         \n"
         << " \n";
}


//Populates dictionary & statistics from words present in "dictionary.txt" file
void populateDictionary(vector<string> &dictionary, int &dictionaryCount, int dictionaryLengthCount[25]){
 
	//initializes byRef variables (dictionary, dictionaryLengthCount, dictionaryCount) to 0
	dictionary.clear();						
    for (int i=0; i<25; i++){
		dictionaryLengthCount[i]=0;
	}
	dictionaryCount = 0; 
	
	ifstream inputFileStream;
	string line;	// stores each line as it is read
    string fileName = "dictionary.txt"; // C string (array of char) to store filename

	// Open input file
	inputFileStream.open( fileName.c_str() );
	    
    // Verify that the open worked
    if( !inputFileStream.is_open()) {
        cout << "Could not find input file " << fileName << "  Exiting..." << endl;
        exit( -1);
    }

	// Read all the words from the file populating dictionary vector, total word count, and word count by length
    while( getline( inputFileStream, line) ) {
		if (line.length() > 0 && line.length() < 25){	
			for (int i = 0; i < line.length(); i++){	//all words stored in dictionary are lowercase
				line[i] = tolower(line[i]);
			}
			dictionary.push_back(line);
			dictionaryLengthCount[line.length()]++;
			dictionaryCount++;
		}
    }
    
    inputFileStream.close();    // Close the input file.
	
}


//Populates dictionary subset of words with specific length trimmed from the entire dictionary
void populateDictionarySubset(vector<string> dictionary, int wordLength, vector<string> &dictionarySubset) {
	dictionarySubset.clear();			//initializes byRef variable (dictionarySubset) to 0  
	for (int i=0; i<dictionary.size(); i++){		//loops through entire dictionary vector and only stores words of specific length in dictionarySubset
		if (dictionary[i].length() == wordLength){
			dictionarySubset.push_back(dictionary[i]);
		}
	}
}


// Use binary search to look up the search word in the dictionary vector, returning
// the word's index if found, -1 otherwise.
long binarySearch(
            string searchWord,            // word to be looked up
            vector<string> dictionary)   // the dictionary of words
{
    long low, mid, high;     // array indices for binary search
    long searchResult = -1;  // Stores index of word if search succeeded, else -1
    
    // Binary search for word
    low = 0;
    high = dictionary.size() - 1;
    while ( low <= high)  {
        mid = (low + high) / 2;
        // SearchResult negative value means word is to the left, positive value means
        // word is to the right, value of 0 means word was found
        searchResult = searchWord.compare( dictionary[mid] );
        if ( searchResult == 0)  {
            // Word IS in dictionary, so return the index where the word was found
            return mid;
        }
        else if (searchResult < 0)  {
            high = mid - 1; // word should be located before the mid location
        }
        else  {
            low = mid + 1; // word should be located after the mid location
        }
    }
    
    // Word was not found
    return -1;
}


//Is able to set passed word (byRef) into random word present in dictionary 
//If random word feature is not requested it will do error checking on user input
//  returns true if user word is of correct length and in dictionary
//  returns false if word does not meet requirements
bool setWord(vector<string> dictionary, int wordLength, string &word){
	if(word=="r"){
		word=dictionary[ rand() % dictionary.size() ];
		return true;
	}
    if(word == "exit"){
        cout << "Exiting program.";
        exit(-1);
    }
	else{
		if(word.length() != wordLength){
		  cout << "\n\t*** '" << word << "' is not of length " << wordLength << ". Please retry.\n \n" ;
		  return false;
		}
		else if (binarySearch(word, dictionary) == -1){			
		  cout << "\n\t*** '" << word << "' is not in the dictionary. Please retry.\n \n";
		  return false;
		}
		return true;
	}
}


//Word change game 
//user has to change input word to reach output word 1 charecter at a time
void wordChangeGame(vector<string> dictionary, int wordLength, string startWord, string endWord){
  string prevWord = startWord;
  string userInput = "";
  int count = 0;
  while( prevWord != endWord ){
      count++;
      while (true){
		  cout << count << ". Previous word is '" << prevWord << "'. Next word:\n";
		  cin >> userInput;
		  int diffCount = 0;
          for (int i = 0; i < wordLength; i++){
            if(prevWord[i] != userInput[i]){
          	diffCount++;
            }
          }
          if(userInput == "exit"){
              return;
          }
		  //check for user input errors
          else if(userInput.length() != wordLength ){
			cout << "*** '" << userInput << "' is not of length " << wordLength <<".  Please retry.\n";
          }
		  else if (binarySearch(userInput, dictionary) == -1){
            cout << "\t*** '" << userInput << "' is not in the dictionary. Please retry.\n";
		  }
          else if(diffCount != 1){
            cout << "\t*** '" << userInput << "' must be exactly 1 character different. Please retry.\n";
          }else{
			//if no error on user input then break out of inner while loop
			//to continue with next round of the game
			break;
		  }
      }
	  prevWord = userInput;
  }
  cout << "Congratulations, you did it!" << endl << endl;
}

bool isInVector(string word, vector<string> list){
	for (int i=0; i<list.size(); i++){
		if (list[i] == word){
			return true;
		}
	}
	return false;
}


//prints to screen the sequence of generated words attempting to get from startWord to endWord
//populates a passed vector (byRef) with the word sequence trying to get from startWord -> endWord
//populates a passed vector (byRef) that maps the index position of the root word that generates each individual word present in sequence
//returns TRUE if possible to get from startWord to endWord
//returns FALSE if not possible
bool findEndWordWithDebug(vector<string> dictionary, int wordLength, string startWord, string endWord, vector<string> &sequence, vector<int> &map){
  sequence.clear();
  sequence.push_back(startWord);
  map.clear();
  map.push_back(0);
  
  int totalCount = 0;
  for(int i =0; i < sequence.size(); i++){		//selects word to perform permutations on
	cout << i << ". " << sequence[i] << ":   ";
	for(int j = 0; j<wordLength; j++){			//selects individual letter within word
		string testWord = sequence[i];
		for(char c='a'; c<='z'; c++){			//varies individual letters from 'a' -> 'z'
			//testWord variable will become all permutations of the currently selected sequence word
			testWord[j] = c;	
			//if the  current word permutation is a word present in the dictionary
			if(binarySearch(testWord, dictionary) >= 0){
				//and the current word permutation does not already exist in "sequence" vector
				if( isInVector(testWord, sequence) == false){							//did not use binary search here since sequence vector is not sorted
					//then append it back onto the "sequence" vector
					sequence.push_back(testWord);
					map.push_back(i);
					totalCount++;
					cout << totalCount << ":" << testWord << " ";
					//terminate permutations if endWord was found
					if (testWord == endWord){
					   cout << endl;
					   return true;
					}
				}
			}
		}
	}
	cout << endl;
  }
  return false;
}

//Same as findEndWordWithDebug 
//but it will NOT print the sequence onto the screen
bool findEndWord(vector<string> dictionary, int wordLength, string startWord, string endWord, 
                 vector<string> &sequence, vector<int> &map){
  sequence.clear();
  sequence.push_back(startWord);
  map.clear();
  map.push_back(0);
    
  for(int i =0; i < sequence.size(); i++){
	for(int j = 0; j<wordLength; j++){
		string testWord = sequence[i];
		for(char c='a'; c<='z'; c++){
			testWord[j] = c;	
			if(binarySearch(testWord, dictionary) >= 0){
				if( isInVector(testWord, sequence) == false){
					sequence.push_back(testWord);
					map.push_back(i);
					if (testWord == endWord){
					   return true;
					}
				}
			}
		}
	}
  }
  return false;  
}


void printDictionaryStats(int &dictionaryCount, int dictionaryLengthCount[25]){
    cout << "Total number of words in dictionary file: " << dictionaryCount << "\n\n";
    cout << "Word lengths where there are more than 1400 words: \n";
    cout << "Length\t" << "How Many\n";
    cout << "------\t" << "--------\n";
	for (int i=0; i<25; i++){
		if (dictionaryLengthCount[i] > 1400){
			cout << setw(5) << right << i << setw(10) << right << dictionaryLengthCount[i] << "\n";
		}
	}
	cout << endl;
}

int main(){
	vector<string> dictionary;			// vector that will store all words present in dictionary file
	int dictionaryCount;				// counter for total words in dictionary
	int dictionaryLengthCount[25];  	// counter of words of specific length (between 0 & 24 chars)
	int wordLength = 3;         		// default length of word for dictionary subset
	vector<string> dictionarySubset;	// vector that will only store only words of specific length
    string startWord = "dog";           // The start word for transformation
    string endWord = "cat";             // The end word for transformation
    int menuOption = -1;                // User menu option selection
	vector<string> wordSequence;		// vector that will store the word sequence generated from the start word to end word
	vector<int> map;					// vector that will store the index position of the root word that generated each word in wordSequence
	bool solutionValid = false;			// flag that represents if wordSequence holds a solution for current parameters
	
	// Display ID info
    displayIdInfo();
    
    // Seed the random number generator
    srand(1);  // Change 1 to time(0) for non-predictable results, but leave as 1 for testing

	populateDictionary(dictionary, dictionaryCount, dictionaryLengthCount);  			//Populates dictionary vector & relevant stats
	printDictionaryStats(dictionaryCount, dictionaryLengthCount);					//Prints dictionary statistics to screen
	populateDictionarySubset(dictionary, wordLength, dictionarySubset );				//Populates dictionary subset vector at default word lenght
	
    // Menu display and handling infinite loop
    while(true) {
        cout << "Currently we have " << dictionarySubset.size() << " words of length "
             << wordLength << " in the dictionary.  \n"
             << "Changing from '" << startWord << "' to '" << endWord << "'" << endl
             << endl;
        
        cout << "Choose from the following options:  \n"
             << "   1. Change the word length        \n"
             << "   2. Display some dictionary words \n"
             << "   3. Get start and end words       \n"
             << "   4. Play the word change game     \n"
             << "   5. Find the end word with debug  \n"
             << "   6. Find the end word             \n"
             << "   7. Display an answer sequence    \n"
             << "   8. Exit the program              \n"
             << "Your choice -> ";
        cin >> menuOption;
		cout << endl;
        
		// Menu option handling should go here
		switch (menuOption){
			case 1:
			{
				cout << "What length words do you want to use? ";
				cin >> wordLength;
				populateDictionarySubset(dictionary, wordLength, dictionarySubset );		//this function sets/populates dictionarySubset (byRef) with words of user specified length
				startWord = "";
				endWord = "";
				solutionValid = false;
				break;
			}
			case 2:
			{
				int startIndex = 0;
				int endIndex = 0;
				
				cout << "Enter the start and end index values of words to display: ";
				cin >> startIndex;
				cin >> endIndex; 
				
				if( endIndex >= dictionarySubset.size() ){
					endIndex = dictionarySubset.size() - 1;
					cout << "Invalid end value.  Resetting it to the maximum which is " << endIndex << endl << endl;
				}

				cout << "About to display dictionary words from " << startIndex << " to " << endIndex << endl;
				for(int i = startIndex; i <= endIndex; i++){
					cout << setw(5) << right << i << " " << dictionarySubset[i] << endl;
				}
				break;
			}
			case 3:
			{
				do{
					cout << "Enter starting word, or 'r' for a random word: ";
					cin >> startWord;
				}while (setWord(dictionarySubset, wordLength, startWord) == false);		//this function sets startWord (byRef)
				cout << endl;
				do{
					cout << "Enter ending word, or 'r' for a random word: ";
					cin >> endWord;
				}while (setWord(dictionarySubset, wordLength, endWord) == false);		//this function sets endWord (byRef)
				solutionValid = false;
				break;
			}
			case 4:
			{
				wordChangeGame(dictionarySubset, wordLength, startWord, endWord);
				break;
			}
			case 5:
			{
				if(findEndWordWithDebug(dictionarySubset, wordLength, startWord, endWord, wordSequence, map) == true){	//prints to screen the sequence of words from startWord -> endWord
					cout << endl << "Winning sequence was found!" << endl << endl;
					solutionValid = true;
				}else{
					cout << "It is not possible to go from " << startWord << " to " << endWord << " using the current dictionary." << endl << endl;
					solutionValid = false;
				}
				break;
			}
			case 6:
			{
				if (findEndWord(dictionarySubset, wordLength, startWord, endWord, wordSequence, map) == true){			//prints if its possible or not to get from startWord -> endWord
					cout << endl << endl << "Winning sequence was found!" << endl << endl;
					solutionValid = true;
				}
				else{
					cout << "It is not possible to go from " << startWord << " to " << endWord << " using the current dictionary." << endl << endl;
					solutionValid = false;
				}	
				break;
			}
			case 7:
			{
				if (solutionValid == false){
					cout << "   *** You must find the solution before displaying it.\n \n";
				}else
				{
					cout << "Winning sequence in reverse order is:" << endl;
					int i = wordSequence.size()-1;
					while (true){
						cout << setw(5) << right << i << ". " << wordSequence[i] << endl;
						if ( i == 0 ){
							break;
						}
						i = map[i];
					}
					cout << endl << endl;
				}
				break;
			}
			case 8:
			{
				cout << "Exiting the program";
				exit(0);
				break;
			}
		}
    }
	return 0;
}