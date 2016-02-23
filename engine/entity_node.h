#pragma once


struct EntityNode {
	int id;
	struct EntityNode* next;
};


struct EntityPool {
	struct EntityNode* head;
};


// Creates an amount of entity nodes before the head and returns the first node in this list
// I.e. walking the list from the return value will read amount of nodes, then the previous head, etc.
struct EntityNode* createEntityNodes(struct EntityNode* head, const int amount);


// Creates a new pool and initialises an amount of nodes
struct EntityPool* createEntityPool(const int amount);


// Walks the list of nodes, freeing all of them
void freeEntityNodes(struct EntityNode* freeEntityNodes);


// Frees an entity pool and all its nodes
void freeEntityPool(struct EntityPool* entityPool);


// Returns the next free node in the pool (or null if there are no nodes)
// Updates the head pointer in the pool to point to the next free node
// Doesn't change data in the node itself
struct EntityNode* getEntityNode(struct EntityPool* entityPool);


// Returns an entity node to the pool
// Updates the node next pointer to the previous head
// Updates pool head pointer to point to the node
// Does nothing if node is null
void returnEntityNode(struct EntityPool* entityPool, struct EntityNode* node);


// Returns all nodes in a list to the pool
void returnEntityList(struct EntityPool* entityPool, struct EntityNode* node);
