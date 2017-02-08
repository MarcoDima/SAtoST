																			<p>SAtoST<p>

-- From Suffix Array and Lcp to Suffix Tree Algorithm Implementation

Author: Marco Di Martino

<p>Description<p>

Given a text T, with its suffix array and array lcp, the algorithm creates the suffix tree of the text T.
Text and the two arrays are read from the attached files 
- banana.txt
- mississippi.txt
- ab.txt

The main() reads one of these files, generates the suffix tree and then prints it to a file called tree.txt.
The print of the tree structure is made using a special format that easly makes it possible to copy-and-paste
it to the site http://www.webgraphviz.com/ in order to be able to display the tree.
Once the tree has been created, the text and the arrays are printed to the standard output together with
all nodes of the tree, but for a clearer view of the tree I reccomend the use of webgraphviz.

<p>Tree Structure<p>

Since the length of the text (and so also the number of nodes of the tree) is not known, I decided to use 
a child-sibiling tree representation. So the tree I create has the following structure:
- each node is identyfied by its path label and string depth (easy implementation to identify a single node without giving a 'name')
- for each node is also known (if they are):
		- node parent
		- node child
		- node sibiling
Creation and addition are separeted: first creation of the node is made, and second it is added to tree structure.
Once I got the tree, the string depth of each node is calculated.

<p>Methods implemented<p>

	void string_depth(node_tree * node, int strdpt);
	node_tree * create_node(node_tree * node, char * label);
	void add_node(node_tree * parent, node_tree * node);
	char * substring(char * T, int i, int l);
	void generate_suffix_tree(char * T, node_tree * root, int * sa, int * lcp, 	int i, int j, int l);
	boolean is_leaf(node_tree * node);
	void tree_visit(node_tree * node);
	void print(node_tree * node, FILE * file);
	void print_tree_on_file(node_tree * node, FILE * file);
	void destroy_tree(node_tree * node);
	
<p>Creation of the Suffix Tree<p>

This is done by the procedure "generate_suffix_tree".
This sub gets the text, the arrays, a pointer to the root of the tree and the dimension of the arrays.
The generation is done recursively taking every time the minimum value of the current interval of the array and calling recursively a subinterval (i,j).
	- first check wheter actual interval contains just one element (i==j), if 	so, a leaf is created.
	- if not, calculate min value e the position of the min.
	- Taking the position of the min, a new node is created and added to the 	tree, but this won't be a leaf because if it was, it would have already 	been created after the check i==j.
	With help of the substring method, the path label of this node can be 	calculated:
	it will have a path label starting from the text in position suffix array 	in position pos_min and will have a length given from lcp[pos_min] value.	
	- every time the min is found in the current interval of the array lcp, a 	recursive call is done to the portion (i,h), where i is the current 	starting index and h is the index where the min has been found. Once the 	call ends, index i is updated with the value i=h+1; this is done because 	the recursive call will have generated all nodes belonging to its interval 	and so I can just look at the remaining part.






