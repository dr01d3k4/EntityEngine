#include <stdlib.h>
#include "entity_node.h"


struct EntityNode* createEntityNodes(struct EntityNode* head, const int amount) {
	for (int i = 0; i < amount; i++) {
		struct EntityNode* node = (struct EntityNode*) malloc(sizeof(struct EntityNode));
		node->next = head;
		head = node;
	}
	return head;
}


struct EntityPool* createEntityPool(const int amount) {
	struct EntityPool* entityPool = (struct EntityPool*) malloc(sizeof(struct EntityPool));
	entityPool->head = createEntityNodes(NULL, amount);
	return entityPool;
}


void freeEntityNodes(struct EntityNode* freeEntityNodes) {
	struct EntityNode* temp;
	while (freeEntityNodes != NULL) {
		temp = freeEntityNodes;
		freeEntityNodes = freeEntityNodes->next;
		free(temp);
	}
}


void freeEntityPool(struct EntityPool* entityPool) {
	freeEntityNodes(entityPool->head);
	free(entityPool);
}


struct EntityNode* getEntityNode(struct EntityPool* entityPool) {
	struct EntityNode* node = entityPool->head;
	if (node == NULL) {
		return NULL;
	}
	entityPool->head = node->next;
	return node;
}


void returnEntityNode(struct EntityPool* entityPool, struct EntityNode* node) {
	if (node == NULL) {
		return;
	}

	node->next = entityPool->head;
	entityPool->head = node;
}


void returnEntityList(struct EntityPool* entityPool, struct EntityNode* node) {
	struct EntityNode* next;

	while (node != NULL) {
		next = node->next;
		returnEntityNode(entityPool, node);
		node = next;
	}
}
