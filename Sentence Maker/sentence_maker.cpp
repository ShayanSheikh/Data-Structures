#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include "assert.h"
#include "mtrand.h"

//innermost map
typedef std::map<std::string, int> inner;
//nested map for window = 2
typedef std::map<std::string, inner >  MY_MAP;
//window = 3
typedef std::map<std::string, MY_MAP> MY_MAP1;


// Custom helper function that reads the input stream looking for
// double quotes (a special case delimiter needed below), and white
// space.  Contiguous blocks of alphabetic characters are lowercased &
// packed into the word.
bool ReadNextWord(std::istream &istr, std::string &word) {
	char c;
	word.clear();
	while (istr) {
		// just "peek" at the next character in the stream
		c = istr.peek();
		if (isspace(c)) {
			// skip whitespace before a word starts
			istr.get(c);
			if (word != "") {
	// break words at whitespace
	return true;
			}
		} else if (c == '"') {
			// double quotes are a delimiter and a special "word"
			if (word == "") {
	istr.get(c);
	word.push_back(c);
			}
			return true;
		} else if (isalpha(c)) {
			// this a an alphabetic word character
			istr.get(c);
			word.push_back(tolower(c));
		} else {
			// ignore this character (probably punctuation)
			istr.get(c);
		}
	}
	return false;
}


// Custom helper function that reads the input stream looking a
// sequence of words inside a pair of double quotes.  The words are
// separated by white space, but the double quotes might not have
// space between them and the neighboring word.  Punctuation is
// ignored and words are lowercased.
std::vector<std::string> ReadQuotedWords(std::istream &istr) {
	// returns a vector of strings of the different words
	std::vector<std::string> answer;
	std::string word;
	bool open_quote = false;
	while (ReadNextWord(istr,word)) {
		if (word == "\"") {
			if (open_quote == false) { open_quote=true; }
			else { break; }
		} else {
			// add each word to the vector
			answer.push_back(word);
		}
	}
	return answer;
}

// Loads the sample text from the file, storing it in the map data
// structure Window specifies the width of the context (>= 2) of the
// sequencing stored in the map.
void LoadSampleText(MY_MAP &data, const std::string &filename) {
	// open the file stream
	std::ifstream istr(filename.c_str());
	if (!istr) { 
		std::cerr << "ERROR cannot open file: " << filename << std::endl; 
		exit(1);
	}
	std::string word;
	std::string p_word = "";
	while (ReadNextWord(istr,word)) {
		// skip the quotation marks (not used for this part)
		if (word == "\"") continue;
		if(p_word.empty()) {
			//if p_word is empty we are at start
			//create entry for first word
			inner tmp;
			data[word] = tmp;
		}
		else {
			//find p_word
			MY_MAP::iterator itr = data.find(p_word);
			if(itr == data.end()) {
				//if not found create entry with current word
				data[p_word].insert(std::pair<std::string, int>(word,1));
			}
			else {
				//find inner word
				inner::iterator itr = data[p_word].find(word);
				if(itr == data[p_word].end()) {
					//if not found create entry with current word
					data[p_word].insert(std::pair<std::string, int>(word,1));
				}
				else {
					//if found increment entry
					data[p_word][word]++;
				}
			}
		}
		//set p_word to current word
		p_word = word;
	}
	//create dummy entry for correct count of occurrences
	data[p_word].insert(std::pair<std::string, int>("",1));
}

void LoadSampleText1(MY_MAP1 &data, const std::string &filename) {
	// open the file stream
	std::ifstream istr(filename.c_str());
	if (!istr) { 
		std::cerr << "ERROR cannot open file: " << filename << std::endl; 
		exit(1);
	}

	std::string word;
	std::string p_word = "";
	std::string pp_word = "";
	while (ReadNextWord(istr,word)) {
		// skip the quotation marks (not used for this part)
		if (word == "\"") continue;
		if(pp_word.empty()) {
			//if most previous word empty and p_word empty, we're at start, create
			//entry for current word
			if(p_word.empty()) {
				MY_MAP tmp;
				data[word] = tmp;
			}
			else {
				//create entry for p_word
				inner tmp;
				data[p_word][word] = tmp;
				pp_word = p_word;
			}
			p_word = word;
		}
		else {
			MY_MAP1::iterator itr = data.find(pp_word);
			if(itr == data.end()) {
				//if pp_word not in data create entry for phrase
				data[pp_word][p_word][word] = 1;
			}
			else {
				MY_MAP::iterator itr1 = data[pp_word].find(p_word);
				if(itr1 == data[pp_word].end()) {
					//if p_word not found create entry for phrase
					data[pp_word][p_word][word] = 1;
				}
				else {
					inner::iterator itr2 = data[pp_word][p_word].find(word);
					//if word not found create entry for phrase
					if(itr2 == data[pp_word][p_word].end())
						data[pp_word][p_word][word] = 1;
					else
						data[pp_word][p_word][word]++; //increment existing entry
				}
			}
			//set words appropriately
			pp_word = p_word;
			p_word = word;
		}
	}
	//create dummy entry for correct count of occurrences;
	data[pp_word][p_word].insert(std::pair<std::string, int>("",1));
}

//print word stat functions
void Print_Window_2(const MY_MAP &data, const std::vector<std::string> &sentence) {
	MY_MAP::const_iterator itr1;
	inner::const_iterator itr2;
	std::string word = sentence[0];
	//find word
	itr1 = data.find(word);
	int sum = 0;
	itr2 = itr1->second.begin();
	//find total occurences
	for(; itr2 != itr1->second.end(); itr2++) {
		sum += itr2->second;
	}
	std::cout << word << " (" << sum << ")" << std::endl; 
	itr2 = itr1->second.begin();
	for(; itr2 != itr1->second.end(); itr2++) {
		//go through and print each word along with occurences while skipping 
		//dummy entry
		if(itr2->first != "")
			std::cout << word << " " << itr2->first << " (" << 
			itr2->second << ")" << std::endl;
	}
	std::cout << std::endl;
}

void Print_Window_3(const MY_MAP1 &data, const std::vector<std::string> &sentence) {
	std::string word = sentence[0];
	//one word passed in
	if(sentence.size() == 1) {
		MY_MAP1::const_iterator itr1;
		MY_MAP::const_iterator itr2;
		inner::const_iterator itr3;
		//find word
		itr1 = data.find(word);
		int tot_sum = 0;
		std::vector<int> sums;
		int sum;
		itr2 = itr1->second.begin();
		//go through and find total occurences of word and also total occurences
		//of each word following it
		for(; itr2 != itr1->second.end(); itr2++) {
			itr3 = itr2->second.begin();
			sum = 0;
			for(; itr3 != itr2->second.end(); itr3++) {
				tot_sum += itr3->second;
				sum += itr3->second;
			}
			//store next word totals for later
			sums.push_back(sum);
		}
		std::cout << word << " (" << tot_sum << ")" << std::endl; 
		itr2 = itr1->second.begin();
		sum = 0;
		//output total next word occurences
		for(; itr2 != itr1->second.end(); itr2++) {
			if(itr2->first != "")
				std::cout << word << " " << itr2->first << " (" << 
				sums[sum] << ")" << std::endl;
			++sum;
		}
		std::cout << std::endl;
	}
	else {
		std::string next_word = sentence[1];
		MY_MAP1::const_iterator itr1;
		MY_MAP::const_iterator itr2;
		inner::const_iterator itr3;
		//find correct spot
		itr1 = data.find(word);
		itr2 = itr1->second.find(next_word);
		int sum = 0;
		itr3 = itr2->second.begin();
		//find total occurences of phrase
		for(; itr3 != itr2->second.end(); itr3++) {
			sum += itr3->second;
		}
		std::cout << word << " " << next_word << " (" << sum << ")" << std::endl; 
		itr3 = itr2->second.begin();
		for(; itr3 != itr2->second.end(); itr3++) {
			//go through and print each word along with occurences while skipping 
			//dummy entry
			if(itr3->first != "")
				std::cout << word << " " << next_word << " " << 
				itr3->first << " (" << itr3->second << ")" << std::endl;
		}
		std::cout << std::endl;
	}
	return;
}

//random and most_common sentence printer window = 2
void Print_Sentence_Window_2(const MY_MAP &data, 
		const std::vector<std::string> &sentence, int length, bool random_flag) {
	std::string word = sentence[0];
	MY_MAP::const_iterator itr1;
	inner::const_iterator itr2;
	std::cout << word;
	static MTRand mtrand(time(NULL));
	if(random_flag) {
		//vector to store cumulative distribution function
		std::vector<float> percents;
		float t_percent, percent, total;
		std::vector<std::string> words;
		for(int i = 0; i < length; ++i) {
			itr1 = data.find(word);
			if(itr1 == data.end())
				return;
			itr2 = itr1->second.begin();
			total = 0;
			for(; itr2 != itr1->second.end(); itr2++) {
				//find total occurences of word and store all following words 
				//for later
				if(itr2->first != "") {
					total += (float)itr2->second;
					words.push_back(itr2->first);
				}
			}
			//if only one word, print, set words appropriately and do next loop
			if(words.size() == 1) {
				word =	words[0];
				std::cout << " " <<  word;
				words.clear();
				continue;
			}
			//loop through words again
			itr2 = itr1->second.begin();
			t_percent = 0; //used for cdf
			for(; itr2 != itr1->second.end(); itr2++) {
				if(itr2->first != "") {
					//store cumulative percentage of word 
					percent = (float)itr2->second/total;
					percents.push_back(percent+t_percent);
					t_percent += percent;
				}
			}
			float random_no = mtrand();
			for(unsigned int j = 0; j < percents.size(); ++j) {
				//loop through and print as soon as random number is less than 
				//current cumulative percentage
				if(random_no < percents[j]) {
					word =	words[j];
					std::cout << " " << word;
					break;
				}
			}
			words.clear();
		}
	}
	else {
		for(int i = 0; i < length; ++i) {
			itr1 = data.find(word);
			int count = 0;
			itr2 = itr1->second.begin();
			//find most occuring word
			for(; itr2 != itr1->second.end(); itr2++) {
				if(itr2->first != "" && itr2->second > count)
					count = itr2->second;
			}
			//print current and set word to current
			itr2 = itr1->second.begin();
			for(; itr2 != itr1->second.end(); itr2++) {
				if(itr2->first != "" && count == itr2->second) {
					std::cout << " " << itr2->first;
					word = itr2->first;
					break;
				}
			}
		}
	}
	std::cout << std::endl << std::endl;
	return;
}

//similar to above function
void Print_Sentence_Window_3(const MY_MAP1 &data, 
		const std::vector<std::string> &sentence, int length, bool random_flag) {
	std::string word = sentence[0];
	MY_MAP1::const_iterator itr1;
	MY_MAP::const_iterator itr2;
	inner::const_iterator itr3;
	static MTRand mtrand(time(NULL));
	std::vector<std::string> words;
	std::cout << word;
	std::string next_word;
	//only one word given
	if(sentence.size() == 1) {
		if(random_flag) {
			std::vector<float> percents;
			float t_percent, percent, total;
			itr1 = data.find(word);

			total = 0;
			itr2 = itr1->second.begin();
			//find total occurrences and store words
			for(; itr2 != itr1->second.end(); itr2++) {
				itr3 = itr2->second.begin();
				for(; itr3 != itr2->second.end(); itr3++) {
					if(itr3->first != "") {
						total += (float)itr3->second;
						words.push_back(itr2->first);
					}
				}
			}
			itr2 = itr1->second.begin();
			t_percent = 0;
			for(; itr2 != itr1->second.end(); itr2++) {
				itr3 = itr2->second.begin();
				percent = 0;
				//find occurrences of each following wod
				for(; itr3 != itr2->second.end(); itr3++) {
					if(itr3->first != "") {
						percent += (float)itr3->second;
					}
				}
				//store in cumulative fashion
				percents.push_back(percent/total+t_percent);
				t_percent += percent/total;
			}
			//set next_word to appropriate word based on random_no
			float random_no = mtrand();
			for(unsigned int j = 0; j < percents.size(); ++j) {
				if(random_no < percents[j]) {
					next_word =	words[j];
					break;
				}
			}
			words.clear();
			percents.clear();
		}
		else {
			//just count up occurrences and set next_word to most common
			itr1 = data.find(word);

			int count = 0;
			int curr_count = 0;
			itr2 = itr1->second.begin();
			for(; itr2 != itr1->second.end(); itr2++) {
				itr3 = itr2->second.begin();
				curr_count = 0;
				for(; itr3 != itr2->second.end(); itr3++) {
					if(itr3->first != "")
						curr_count += itr3->second;
				}
				if(curr_count > count)
					count = curr_count;
			}
			itr2 = itr1->second.begin();
			for(; itr2 != itr1->second.end(); itr2++) {
				itr3 = itr2->second.begin();
				curr_count = 0;
				for(; itr3 != itr2->second.end(); itr3++) {
					if(itr3->first != "") {
						curr_count += itr3->second;
					}
				}
				if(count == curr_count) {
					next_word = itr2->first;
					break;
				}
			}
		}
		//we'll be printing out next word so we can stop one before we would for
		//a sentence size of 2
		--length;
	}
	else {
		//two words given
		next_word = sentence[1];
	}
	std::cout << " " << next_word;
	//pretty much identical to corresponding section in 
	//Print_Sentence_Window_2
	//only difference is we go one level deeper due to having
	//two words to go off
	if(random_flag) {
		std::vector<float> percents;
		float t_percent, percent, total;
		for(int i = 0; i < length; ++i) {
			itr1 = data.find(word);
			//make sure phrase actually exists
			if(itr1 == data.end())
				break;
			itr2 = itr1->second.find(next_word);
			if(itr2 == itr1->second.end())
				break;
			itr3 = itr2->second.begin();
			total = 0;
			for(; itr3 != itr2->second.end(); itr3++) {
				if(itr3->first != "") {
					total += (float)itr3->second;
					words.push_back(itr3->first);
				}
			}
			if(words.empty()) {
				break;
			}
			itr3 = itr2->second.begin();
			t_percent = 0;
			for(; itr3 != itr2->second.end(); itr3++) {
				if(itr3->first != "") {
					percent = (float)itr3->second/total;
					percents.push_back(percent+t_percent);
					t_percent += percent;
				}
			}
			float random_no = mtrand();
			for(unsigned int j = 0; j < percents.size(); ++j) {
				if(random_no < percents[j]) {
					word =	next_word;
					next_word = words[j];
					std::cout  << " " << next_word;
					break;
				}
			}
			percents.clear();
			words.clear();
		}
	}
	else {
		int count;
		for(int i = 0; i < length; ++i) {
			count = 0;
			itr1 = data.find(word);
			if(itr1 == data.end()) 
				break;
			itr2 = itr1->second.find(next_word);
			if(itr2 == itr1->second.end()) 
				break;

			itr3 = itr2->second.begin();
			for(; itr3 != itr2->second.end(); itr3++) {
				if(itr3->first != "" && itr3->second > count)
					count = itr3->second;
			}
			itr3 = itr2->second.begin();
			for(; itr3 != itr2->second.end(); itr3++) {
				if(itr3->first != "" && count == itr3->second) {
					word = next_word;
					next_word = itr3->first;
					std::cout  << " " << next_word;
					break;
				}
			}
		}
	}
	std::cout << std::endl << std::endl;
	return;
}

int main () {

	// ASSIGNMENT: THE MAIN DATA STRUCTURE
	MY_MAP data;
	MY_MAP1 data1;
	int window;
	// Parse each command
	std::string command;    
	while (std::cin >> command) {
		// load the sample text file
		if (command == "load") {
			std::string filename;
			std::string parse_method;
			std::cin >> filename >> window >> parse_method;      
			// verify the window parameter is appropriate
			if (window < 2) {
				std::cerr << "ERROR window size must be >= 2:" << window << std::endl;
			}
			if(window == 2)
				LoadSampleText(data, filename);
			if(window == 3)
				LoadSampleText1(data1, filename);
			std::cout << "Loaded " << filename << " with window = " << window << 
			" and parse method = " << parse_method << std::endl << std::endl;
		}
		// print the portion of the map structure with the choices for the
		// next word given a particular sequence.
		else if (command == "print") {
			std::vector<std::string> sentence = ReadQuotedWords(std::cin);
			if(window == 2) {
				Print_Window_2(data, sentence);
			}
			else {
				Print_Window_3(data1, sentence);
			}
		}

		// generate the specified number of words 
		else if (command == "generate") {
			std::vector<std::string> sentence = ReadQuotedWords(std::cin);
			// how many additional words to generate
			int length;
			std::cin >> length;
			std::string selection_method;
			std::cin >> selection_method;
			bool random_flag;
			if (selection_method == "random") {
				random_flag = true;
			} else {
				assert (selection_method == "most_common");
				random_flag = false;
			}

			if(window == 2)
				Print_Sentence_Window_2(data, sentence, length, random_flag);
			else
				Print_Sentence_Window_3(data1, sentence, length, random_flag);

		} else if (command == "quit") {
			break;
		} else {
			std::cout << "WARNING: Unknown command: " << command << std::endl;
		}
	}
	std::cout << std::endl;

	return 0;
}
