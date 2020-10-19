# Dancing Links Simulation

## Description

Dancing Links is a term coined by Donald Knuth describing a certain set of properties usually correlated with a circular doubly linked list. The idea of dancing links is based on the observation of the following properties. Properties 1 & 2 are typically well known as these deal with the removal of a node from the circular doubly linked list:

	(1) RLINK(LLINK(X)) <- RLINK(X)
	(2) LLINK(RLINK(X)) <- LLINK(X)

These 2 properties basically adjust the surrounding nodes of X to adjust their pointers to each other. Please note these properties do not change the links of the X node at all. Typically after these 2 properties are performed the developer is inclined to clean up memory and release X (after releasing the links X holds). However, in DLX the final 2 properties rely on backtracking and leaving X in memory:

(3) RLINK(LLINK(X) <- X
(4) LLINK(RLINK(X) <- X
      
This application showcases these properties working in a recursive function and how they allow certain algorithms (such as DLX/Algorithm X) to search through the data structure and if necessary backtrack to a later point to retry another "path".

#### Building

gcc -o DANCE dancingex.c