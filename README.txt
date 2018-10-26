README

included:
All source files (.cpp, .h)
this README
Makefile
Data (.txt)
Some early versions(.cpp)

Proj3 ECS60

	I adopted the hash table from the last project, and use Djb hash. At the very beginning, I  used two hash tables store the sensitive and insensitive cases, and used a string split function from internet, But it's rather slow. Almost 900ms for the largest text. Yesterday  afternoon after 5 hours I finally came up with an idea that just store the sensitive words and deal with it later. I actually thought of that before but I didn't realize it can actually simplify the program. Then it took about 700ms for the largest. 
	This morning I rewrote the string split function(Addline) and the time was about 600ms. The professor's program was 500ms for the same text, but this was the best I can do. But when I copy it to CSIF and ran it, the time was 232ms for the largest!!
	I used the radix sort for the count, and my partner finished quick sort for the ascii sorting.


Partner: Quanzhou Chen

Reference: 
	hash structure: last project
	hash function: lecture notes
	Earlier version of split strings: https://www.geeksforgeeks.org/how-to-split-a-string-in-cc-python-and-java/

Yiran Wu (Kevin)  Aug.31, 2018
	