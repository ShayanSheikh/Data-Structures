#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include <string>
#include "assert.h"

//used in alphabet populator
const std::string Alphabet = "abcdefghijklmnopqrstuvwxyz";

//function prototype since I have two functions that call each other in turn
void find_board(std::vector<std::vector<char> > &board, 
	std::list<std::vector<std::vector<char> > > &solns, 
	std::list<std::string> &plus, const std::list<std::string> &minus, 
	bool &one_soln, bool &done);

//simple check to see if word is palindrome
bool is_palindrome(std::string word) {
	std::string tmp = "";
	for(int i = word.length() - 1; i >= 0; --i)
		tmp += word[i]; //add to empty string from back of word
	//compare and return appropriate bool
	if(tmp == word)
		return true;
	else
		return false;
}

//check solns vector to see if current board is already in there
//necessary for puzzles like 7.
bool duplicate(const std::vector<std::vector<char> > &board, 
		std::list<std::vector<std::vector<char> > > &solns) {
	int length = board.size();
	int width = board[0].size();
	std::list<std::vector<std::vector<char> > >::const_iterator it = solns.begin();
	//loop through solns
	for(; it != solns.end(); it++) {
		//store current board
		std::vector<std::vector<char> > tmp = *it;
		bool same = true;
		//compare each element in both boards
		for(int i = 0; i < length; ++i) {
			for(int j = 0; j < width; ++j) {
				//if an element doesn't match set same to false break loop
				if(board[i][j] != tmp[i][j]) {
					same = false;
					break;
				}
			}
			//break this level as well so we can move on to the next board
			if(!same)
				break;
		}
		//if above double for loop ends without same being set to false then
		//boards are equal return true
		if(same)
			return true;
	}
	//if we loop through entire solns without returning true, board is not duplicate
	return false;
}

//linear word search, very similar to check of emptiness when adding word
bool word_search(const std::vector<std::vector<char> > &board, std::string word) {
	int w_size = word.length();
	int x, y, end_x, end_y;
	int length = board.size();
	int width = board[0].size();
	bool found = true;
	for(int i = 0; i < length; ++i) {
		for(int j = 0; j < width; ++j) {
			x = i, y = j;
			//check in row
			//go to right
			end_y = j+w_size-1;
			//make sure within bounds
			if(end_y < width) {
				found = true; //set found to true before each loop
				for(int q = y, k = 0; q <= end_y; ++q, ++k) {
					//if does not match we set found to false and break out of loop
					if(board[x][q] != word[k]) {
						found = false;
						break;
					}
				}
				//if found not set to false, word in there, return true
				if(found)
					return true;
			}
			//go to left if word not palindrome
			end_y = j-w_size+1;
			if(end_y >= 0) {
				found = true;
				for(int q = y, k = 0; q >= end_y; --q, ++k) {
					if(board[x][q] != word[k]){
						found = false;
						break;
					}
				}
				if(found)
					return true;
			}
			//check in col
			//go down
			end_x = i+w_size-1;
			if(end_x < length) {
				found = true;
				for(int p = x, k = 0; p <= end_x; ++p, ++k) {
					if(board[p][y] != word[k]){
						found = false;
						break;
					}
				}
				if(found)
					return true;
			}
			//go up
			end_x = i-w_size+1;
			if(end_x >= 0) {
				found = true;
				for(int p = x, k = 0; p >= end_x; --p, ++k) {
					if(board[p][y] != word[k]){
						found = false;
						break;
					}
				}
				if(found)
					return true;
			}
			//down to the right
			end_x = i+w_size-1;
			end_y = j+w_size-1;
			if(end_x < length && end_y < width) {
				found = true;
				for(int p = x, q = y, k = 0; p <= end_x  && q <= end_y; ++p, ++q, ++k) {
					if(board[p][q] != word[k]){
						found = false;
						break;
					}
				}
				if(found)
					return true;
			}
			//up to the left
			end_x = i-w_size+1;
			end_y = j-w_size+1;
			if(end_x >= 0 && end_y >= 0) {
				found = true;
				for(int p = x, q = y, k = 0; p >= end_x && q >= end_y; --p, --q, ++k) {
					if(board[p][q] != word[k]){
						found = false;
						break;
					}
				}
				if(found)
					return true;
			}
			//up to the right
			end_x = i-w_size+1;
			end_y = j+w_size-1;
			if(end_x >= 0 && end_y < width) {
				found = true;
				for(int p = x, q = y, k = 0; p >= end_x && q <= end_y; --p, ++q, ++k) {
					if(board[p][q] != word[k]){
						found = false;
						break;
					}
				}
				if(found)
					return true;
			}
			//down to the left
			end_x = i+w_size-1;
			end_y = j-w_size+1;
			if(end_x < length && end_y >= 0) {
				found = true;
				for(int p = x, q = y, k = 0; p <= end_x && q >= end_y; ++p, --q, k++) {
					if(board[p][q] != word[k]){
						found = false;
						break;
					}
				}
				if(found)
					return true;
			}
		}
	}
	//if we loop through whole board and don't return anything, word not found
	return false;
}

//check if board has any minus words in it
bool board_is_valid(const std::vector<std::vector<char> > &board, 
		const std::list<std::string> &minus) {
	//loop through minus words vectors and check for each word
	std::list<std::string>::const_iterator it = minus.begin();
	for(; it != minus.end(); it++) {
		std::string word = *it;
		//if any words found return false
		if(word_search(board, word))
			return false;
	}
	//if loop ends, no minus words in board
	return true;
}

//make a vector of all empty spaces in board so populate function doesn't have
//to keep looping through board
std::list<std::vector<int> > empty_spaces(const std::vector<std::vector<char> > &board) {
	int length = board.size();
	int width = board[0].size();
	std::list<std::vector<int> > spaces;
	std::vector<int> loc;
	//loop through board
	for(int i = 0; i < length; ++i) {
		for(int j = 0; j < width; ++j) {
			//if spot empty, push i and j to a vector and store that in another
			//vector
			if(board[i][j] == '.') {
				loc.push_back(i);
				loc.push_back(j);
				spaces.push_back(loc);
				loc.clear();
			}
		}
	}
	return spaces;
}

//populate board with alphabet 
void populate(std::vector<std::vector<char> > board, 
		std::list<std::vector<std::vector<char> > > &solns, 
		std::list<std::vector<int> > &spaces,const std::list<std::string> &minus,
		bool &one_soln, bool &done) {
	//done is a bool for one_soln
	//if it's true just return, passed in by reference so this pretty much stops all
	//recursive functions
	if(done)
		return;
	//if spaces vector is empty check that board is not duplicate and is valid
	if(spaces.empty()) {
		if(!duplicate(board, solns) && board_is_valid(board, minus)) {
			//if one_soln bool is true we set done to true
			if(one_soln)
				done = true;
			//push board to solns list
			solns.push_back(board);
		}
		return;
	}
	//loop through alphabet
	for(unsigned int i = 0; i < Alphabet.size(); ++i) {
		std::list<std::vector<int> >::iterator it = spaces.begin();
		std::vector<int> tmp = *it;
		//first space in vector is set to current letter
		board[tmp[0]][tmp[1]] = Alphabet[i];
		//create copy of spaces vector and remove first element
		std::list<std::vector<int> > new_spaces = spaces;
		new_spaces.pop_front();
		//pass to populate with new spaces vector
		populate(board, solns, new_spaces, minus, one_soln, done);
	}
}

//check if space is either empty or contains the letters that constitute the word
//used before add_word function
//I know this seems like wasted effort but the add_word makes a copy of the board
//to add the word in and this saves a lot of unnecesary copying
bool is_valid(const std::vector<std::vector<char> > &board, std::string word,
		int x, int y, int end_x, int end_y) {
	int max_x = std::max(x,end_x);
	int max_y = std::max(y,end_y);
	int min_x = std::min(x,end_x);
	int min_y = std::min(y,end_y);
	//check passed in indices for being in bounds
	if(min_x < 0 || max_x >= (int)board.size() || min_y < 0 || max_y >= (int)board[0].size())
		return false;
	//check start and end indices to figure out direction of iteration
	//i is x, j is y and k is word
	//row
	if(x == end_x) {
		//go to right
		if(y < end_y) {
			for(int j = y, k = 0; j <= end_y; ++j, ++k) {
				//check space for empty or matching letter in word
				if(board[x][j] != '.' && board[x][j] != word[k])
					return false;
			}
		}
		//go to left
		else {
			for(int j = y, k = 0; j >= end_y; --j, ++k) {
				if(board[x][j] != '.' && board[x][j] != word[k])
					return false;
			}
		}
	}
	//column
	else if(y == end_y) {
		//down
		if(x < end_x) {
			for(int i = x, k = 0; i <= end_x; ++i, ++k) {
				if(board[i][y] != '.' && board[i][y] != word[k])
					return false;
			}
		}
		//up
		else {
			for(int i = x, k = 0; i >= end_x; --i, ++k) {
				if(board[i][y] != '.' && board[i][y] != word[k])
					return false;
			}
		}
	}
	//down to the right
	else if(x < end_x && y < end_y) {
		for(int i = x, j = y, k = 0; i <= end_x  && j <= end_y; ++i, ++j, ++k) {
			if(board[i][j] != '.' && board[i][j] != word[k])
				return false;
		}
	}
	//up to the left
	else if (x > end_x && y > end_y) {
		for(int i = x, j = y, k = 0; i >= end_x && j >= end_y; --i, --j, ++k) {
			if(board[i][j] != '.' && board[i][j] != word[k])
				return false;
		}
	}
	//up to the right
	else if(x > end_x && y < end_y) {
		for(int i = x, j = y, k = 0; i >= end_x && j <= end_y; --i, ++j, ++k) {
			if(board[i][j] != '.' && board[i][j] != word[k])
				return false;
		}
	}
	//down to the left
	else if(x < end_x && y > end_y) {
		for(int i = x, j = y, k = 0; i <= end_x && j >= end_y; ++i, --j, ++k) {
			if(board[i][j] != '.' && board[i][j] != word[k])
				return false;
		}
	}
	//if relevant loop ends, word can be added
	return true;
}

//identical to above in logic but simply sets corresponding space to word
//copy of board is passed in so new board is made for each possibility
void add_word(std::vector<std::vector<char> > board, 
		std::list<std::vector<std::vector<char> > > &solns,
		std::string word, std::list<std::string> plus,const std::list<std::string> &minus,
		int x, int y, int end_x, int end_y, bool &one_soln, bool &done) {
	//identical to above, adds in word
	if(x == end_x) {
		if(y < end_y) {
			//set space in board to word
			for(int j = y, k = 0; j <= end_y; ++j, ++k)
				board[x][j] = word[k];
		}
		else {
			for(int j = y, k = 0; j >= end_y; --j, ++k)
				board[x][j] = word[k];
		}
	}
	else if(y == end_y) {
		if(x < end_x) {
			for(int i = x, k = 0; i <= end_x; ++i, ++k)
				board[i][y] = word[k];
		}
		else {
			for(int i = x, k = 0; i >= end_x; --i, ++k)
				board[i][y] = word[k];
		}
	}
	else if(x < end_x && y < end_y) {
		for(int i = x, j = y, k = 0; i <= end_x  && j <= end_y; ++i, ++j, ++k)
			board[i][j] = word[k];
	}
	else if (x > end_x && y > end_y) {
		for(int i = x, j = y, k = 0; i >= end_x && j >= end_y; --i, --j, ++k)
			board[i][j] = word[k];
	}
	else if(x > end_x && y < end_y) {
		for(int i = x, j = y, k = 0; i >= end_x && j <= end_y; --i, ++j, ++k)
			board[i][j] = word[k];
	}
	else if(x < end_x && y > end_y) {
		for(int i = x, j = y, k = 0; i <= end_x && j >= end_y; ++i, --j, k++)
			board[i][j] = word[k];
	}
	//remove added word from plus words vector
	plus.pop_front();
	//pass new vector back to find_board
	find_board(board, solns, plus, minus, one_soln, done);
}

//find_board function, takes in board as reference since necessary copying is done in
//pass to add_word. solns container(taken in by this, add_word and populate), plus and
//minus vectors and a bool that tells us if we're only looking for one_soln
void find_board(std::vector<std::vector<char> > &board, 
	std::list<std::vector<std::vector<char> > > &solns, 
	std::list<std::string> &plus, const std::list<std::string> &minus,
	bool &one_soln, bool &done) {
	//same bool as explained above, if true all recursive calls are ended at start
	if(done)
		return;
	if(plus.empty()) {
		//if plus words vector is empty first check for empty spaces
		std::list<std::vector<int> > spaces = empty_spaces(board);
		//if empty spaces present and board valid pass to populate
		if(!spaces.empty() && board_is_valid(board, minus)) {
			//if many more spaces than words, check validity before passing
			if(spaces.size() > minus.size()) {
				if(board_is_valid(board, minus)) {
					populate(board, solns, spaces, minus, one_soln, done);
				}
			}
			else //just pass
				populate(board, solns, spaces, minus, one_soln, done);
		}
		else {
			//else check that board is not duplicate and contains no minus words
			if(board_is_valid(board, minus) & !duplicate(board, solns)) {
				//if one_soln is true set done to true
				if(one_soln)
					done = true;
				//push board to solns
				solns.push_back(board);
			}
		}
		//end function
		return;
	}
	int length = board.size();
	int width = board[0].size();
	std::list<std::string>::iterator it = plus.begin();
	//first word in plus vector
	std::string word = *it;
	int w_size = word.length();
	//loop through board
	for(int i = 0; i < length; ++i) {
		for(int j = 0; j < width; ++j) {
			//if word is not a palindrome we do all 8 directions otherwise only
			//do 4 plus directions to prevent duplicates
			if(!is_palindrome(word)) {
				if(is_valid(board, word, i, j, i-w_size+1, j))
					add_word(board, solns, word, plus, minus, i, j, i-w_size+1, j, one_soln, done);
				if(is_valid(board, word, i, j, i, j-w_size+1))
					add_word(board, solns, word, plus, minus, i, j, i, j-w_size+1, one_soln, done);
				if(is_valid(board, word, i, j, i-w_size+1, j-w_size+1))
					add_word(board, solns, word, plus, minus, i, j, i-w_size+1, j-w_size+1, one_soln, done);
				if(is_valid(board, word, i, j, i+w_size-1, j-w_size+1))
					add_word(board, solns, word, plus, minus, i, j, i+w_size-1, j-w_size+1, one_soln, done);
			}
			if(is_valid(board, word, i, j, i+w_size-1, j))
				add_word(board, solns, word, plus, minus, i, j, i+w_size-1, j, one_soln, done);
			if(is_valid(board, word, i, j, i, j+w_size-1))
				add_word(board, solns, word, plus, minus, i, j, i, j+w_size-1, one_soln, done);
			if(is_valid(board, word, i, j, i+w_size-1, j+w_size-1))
				add_word(board, solns, word, plus, minus, i, j, i+w_size-1, j+w_size-1, one_soln, done);
			if(is_valid(board, word, i, j, i-w_size+1, j+w_size-1))
				add_word(board, solns, word, plus, minus, i, j, i-w_size+1, j+w_size-1, one_soln, done);
		}
	}
}

int main(int argc, char const *argv[]) {
	if(argc != 4) 
		std::cerr << "Usage " << argv[0] << " puzzle file, out file, no of solns" << std::endl;

	std::ifstream istr(argv[1]);
	if(!istr) 
		std::cerr << "Could not open " << argv[1] << std::endl;

	//read in grid size
	int x, y;
	istr >> y >> x;
	if(x == 0 or y == 0)
		std::cerr << "Invalid grid size." << std::endl;

	std::list<std::string> plus;
	std::list<std::string> minus;
	std::string word;
	//read in plus and minus words
	while(istr >> word) {
		if(word == "+") {
			istr >> word;
			plus.push_back(word);
		}
		else {
			istr >> word;
			minus.push_back(word);
		}
	}
	istr.close();

	//create board, vector of vectors of characters
	std::vector<std::vector<char> > board;
	std::vector<char> tmp;
	//make row vector with length equal to number of cols
	for (int i = 0; i < y; ++i)
		tmp.push_back('.');

	//push in for number of rows
	for (int i = 0; i < x; i++)
		board.push_back(tmp);

	//solns container, list of boards
	std::list<std::vector<std::vector<char> > > solns;
	bool one_soln = false;
	bool done = false;
	//if one_solution set that bool to true
	if(std::string(argv[3]) == "one_solution")
		one_soln = true;
	//call find_board
	find_board(board, solns, plus, minus, one_soln, done);

	std::ofstream ostr(argv[2]);
	if(!ostr) 
		std::cerr << "Could not open " << argv[2] << std::endl;
	
	//no solns found case
	if(solns.empty())
		ostr << "No solutions found\n";
	else if(!one_soln) //if not one_soln output no of solutions
		ostr << solns.size() << " solution(s)\n";
	std::list<std::vector<std::vector<char> > >::iterator itr = solns.begin();
	//print all boards
	for( ; itr != solns.end(); ++itr) {
		std::vector<std::vector<char> > temp = *itr;
		ostr << "Board:\n";
		for(int i = 0; i < x; ++i) {
			ostr << "  ";
			for(int j = 0; j < y; ++j) {
				ostr << temp[i][j];
			}
			ostr << std::endl;
		}
	}
	ostr.close();

	return 0;
}