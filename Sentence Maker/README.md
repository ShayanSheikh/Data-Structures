THE INPUT AND OUTPUT CASES WERE ALL PROVIDED BY PROF. HOLZBAUER RPI ALONG WITH MTRAND.CPP AND MTRAND.H. HGCUSTOM IS MY OWN TEST CASE AND ALL WORK IN SENTENCE_MAKER.CPP IS MINE

HOMEWORK 7: WORD FREQUENCY MAPS


NAME:  Shayan Sheikh


COLLABORATORS AND OTHER RESOURCES:

Anton Nekhai

ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  20

ANALYSIS OF PERFORMANCE OF YOUR ALGORITHM:
n = total number of words in the sample text file
m = number of unique words in the file
w = width of the sequencing window
p = average number of words observed to follow a particular word

How much memory will the map data structure require, in terms of n, m,
w, and p (order notation for memory use)?
To start we have m keys for each word, each with p^(w-1) words following them. 
A window of 2 is a map within a map so we have just the one layer of p words for 
top level word. So O(m * p^(w-1))


What is the order notation for performance (running time) of each of
the commands?
LoadSampleText: n for looping through all words. m^w number of keys.
So O(n + m*w)

Print_Window: Finding word is O(log(m)). Have to loop through twice, once to
find total, once to print. So O(2*m^(w-1))=>O(m^(w-1)). Final is O(log(m) + m^(w-1)).

Print_Sentence: For most common O is equivalent to above since same operations carried 
out so O(log(m) + m^(w-1)).

For random it's the same except for extra loop through words vector so 
O(log(m) + m^(w-1) + p).
