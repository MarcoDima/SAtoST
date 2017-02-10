/* Author: Di Martino Marco 
   Last update: 08/02/2017
*/

#include<stdio.h>   
#include <stdlib.h>
#include <string.h>

/*************MACRO**********/
#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#define NL printf("\n");
#define STAMPA_ARRAY(array, l) for(int i=0; i<l; i++) { printf("%d ", array[i]); }

/*******************************/

/* Boolean type definition */
typedef int boolean;

/* Tree type definition */
typedef struct node {
	char * path_label;
	int string_depth;
	struct node * parent;
	struct node * child;
	struct node * sibiling;
} node_tree;

/* Recursively assigns value of string_depth to each node */
void string_depth(node_tree * node, int strdpt){
	node->string_depth=strdpt;
	if (node->child != NULL){
		string_depth(node->child, strdpt+1);
	}
	if (node->sibiling != NULL){
		string_depth(node->sibiling, strdpt);		
	}
}

/* initialization of a node*/
node_tree * crea_node(node_tree * node, char * label){
	node = malloc(sizeof(node_tree));
	if (node != NULL){
		node->path_label = label;
		node->parent = NULL;
		node->child=NULL;
		node->sibiling=NULL;
	}
	return node;
}

/* Add a node to the Tree */
void add_node(node_tree * parent, node_tree * node){
	if (parent->child == NULL){
		parent->child=node;	
	}else{
		node_tree * next = parent->child;
		while (next->sibiling != NULL){
			next = next->sibiling;
		}
		next->sibiling = node;
	}
	node->parent = parent;
}

/* Given string T, find its substring from i with length l 
T = [1,2,....,n]
subT=[i,i+1,...,i+l-1]
*/
char * substring(char * T, int i, int l){
	char * str = (char *)malloc(sizeof(char)*l+1); // terminator of a string is '\0'
	strncpy(str, &T[i], l);
	str[l]='\0';
	return str;
}

/* Given Text T and its suffix array and lcp, create recursively the 
	suffix tree of the text T.
 */
void generate_suffix_tree(char * T, node_tree * root, int * sa, int * lcp, int i, int j, int l) {
	if (i==j){
		node_tree * f = crea_node(f, &T[sa[i]]);
		add_node(root, f);	
	}else{	
		int min=lcp[i];		
		int in=i+1;
		int pos_min=i;
		for(int k=in; k<j; k++){ 
			if (lcp[k] < min){
				min = lcp[k];
				pos_min=k;
			}
		}
		node_tree * s;
		if (!(i==0 && j==l-1)){
			s = crea_node(s, substring(T, sa[pos_min], lcp[pos_min]));
			add_node(root, s);
		}else{
			s = root;
		}
		for (int h=i; h<=j; h++){
			if(h==j){
				generate_suffix_tree(T, s, sa, lcp, i, j, l);	
			} else if (lcp[h] == min){
				generate_suffix_tree(T, s, sa, lcp, i, h, l);
				i=h+1;
			}	
		}
	}
}

/* Determine whether a node is a leaf or not */
boolean is_leaf(node_tree * node){
	return (node->child == NULL);
}

/* Recursively visit of the tree, printing on stdo the path label 
	and the string depth of each node. 
	--format: path_label [strdpt] 
*/
void tree_visit(node_tree * node){
	if (node != NULL){
		if (!(is_leaf(node))){
			printf("%s [%d] \n", node->path_label, node->string_depth);	
			tree_visit(node->child);
			tree_visit(node->sibiling);
		}else{
			printf("%s [%d] \n", node->path_label, node->string_depth);
			tree_visit(node->sibiling);
		}
	}
}

/* Print the tree on the file. See void print_tree_on_file(..) */
void print(node_tree * node, FILE * file){
	if (node != NULL){
		if (!(is_leaf(node))){
			fprintf(file, "\t\"%s [%d]\"->\"%s [%d]\" \n", node->parent->path_label, node->parent->string_depth, node->path_label, node->string_depth);
			print(node->child, file);
			print(node->sibiling, file);
		}else{
			fprintf(file, "\t\"%s [%d]\"->\"%s [%d]\" \n", node->parent->path_label, node->parent->string_depth, node->path_label, node->string_depth);
			print(node->sibiling, file);
		}
	}
}

/* Recursively print all nodes of the tree with every path label and string depth
   Used to display the tree structure using Webgraphviz. 
   --format: digraph G { 
			"node_p" -> "node_c"
			"node_p1" -> "node_c1
			...
			} 
*/
void print_tree_on_file(node_tree * node, FILE * file){
	fprintf(file, "%s \n", "digraph G {");
	print(node->child, file);
	fprintf(file, "%s \n", "}");
}

/* free memory when tree does not occur anymore */
void destroy_tree(node_tree * node){
	if (node != NULL){
		destroy_tree(node->child);
		destroy_tree(node->sibiling);
		free(node);
	}
}

/***** STARTING MAIN *****/
int main(int argc, char ** argv){

char * T;
FILE * f;

if (argc != 2){
	printf("Wrong number of args!\n");
	printf("Format => ./Suffix_array 'file_name'");

}

if ((f = fopen(argv[1], "r")) == NULL){
	printf("%s \n", "An error occurred while reading access to a file.");
	exit(EXIT_FAILURE);
}

T = (char *)malloc((sizeof(char *)));
if (T == NULL){
	printf("ERROR: Memory couldn't be allocated");
	exit(EXIT_FAILURE);
}
fscanf(f, "%s", T);
int n = strlen(T);

int suffix_array[n];
int lcp_array[n];

int i=0;
while (i<n){
	fscanf(f, "%d", &suffix_array[i]);
	i++;
}

i=0;
while (i<n){
	fscanf(f, "%d", &lcp_array[i]);
	i++;
}
fclose(f);

node_tree * root = crea_node(root, "ROOT");
NL;
printf("Testo T: %s\n", T);
NL
printf("SA = [ " ); STAMPA_ARRAY(suffix_array, n); printf(" ]"); // SA = [ val1, ..., valn ]
NL
printf("LCP = [ " ); STAMPA_ARRAY(lcp_array, n); printf(" ]"); // LCP = [ val1, ..., valn ]
NL NL

generate_suffix_tree(T, root, suffix_array, lcp_array, 0, n-1, n);

string_depth(root, 0);

FILE * file;
if ((file = fopen("tree.txt", "w")) == NULL){
	printf("%s \n", "An error occurred while writing access to a file.");
	exit(EXIT_FAILURE);
}else{
	print_tree_on_file(root, file);
}
fclose(file);

tree_visit(root);

destroy_tree(root);
free(T);
}
