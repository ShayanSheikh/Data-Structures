HOMEWORK 6: INVERSE WORD SEARCH


NAME: Shayan Sheikh


COLLABORATORS AND OTHER RESOURCES: 

Anton Nekhai
Ryan Smith

ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  20

ALGORITHM ANALYSIS:
What's the order notation of your algorithm?
find_board loops through the board for each plus word so to start we have
O(w*h*r). Inserting a word in every possible place is O((w-l)(h-l)*l*r). 
Searching for each minus word is, similarly O((w-l)(h-l)*l*f). Finally, 
searching for duplicates is O(s*w*h). Assuming e empty spaces we have 
O(26^e) boards being made as well. 
So total = O(w*h*r + (w-l)(h-l)*l*r + (w-l)(h-l)*l*f + s*w*h + 26^e)


TEST CASE SUMMARY:
How did your program perform on the different test cases?  Summarize
the running times.  (It's ok if it didn't finish the harder examples.)
What new test cases did you create and how did it perform on those
tests?
Number of solutions matched given output files
Puzzle No.		Time(s)
1				0.064
2  				0.064
3				0.171
4 				0.109	
5				0.124
6 				0.171
7 				3.515
8				40.624

The only other test case I ran it on was one with no solutions. Small grid
of 2x3 with cat and car as + words and aa as a minus one. Runs in 0.109s. 