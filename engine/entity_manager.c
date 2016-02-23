#include <stdlib.h>
#include "entity_manager.h"
#include "entity_node.h"
#include "component.h"


struct EntityManager* createEntityManager(const int entityCount) {
	struct EntityManager* entityManager = (struct EntityManager*) malloc(sizeof(struct EntityManager));
	*((int*) &entityManager->entityCount) = entityCount;
	entityManager->entities = (bool*) malloc(sizeof(bool) * entityCount);

	entityManager->positions = (struct PositionComponent**) calloc(entityCount, sizeof(struct PositionComponent));
	entityManager->movements = (struct MovementComponent**) calloc(entityCount, sizeof(struct MovementComponent));

	return entityManager;
}


void freeEntityManager(struct EntityManager* entityManager) {
	free(entityManager->entities);
	for (int i = 0; i < entityManager->entityCount; i++) {
		free(entityManager->positions[i]);
		free(entityManager->movements[i]);
	}
	free(entityManager->positions);
	free(entityManager->movements);
	free(entityManager);
}


int getReservedEntityCount(struct EntityManager* entityManager) {
	int count = 0;
	for (int i = 0; i < entityManager->entityCount; i++) {
		if (entityManager->entities[i]) {
			count += 1;
		}
	}
	return count;
}


int getFreeEntityCount(struct EntityManager* entityManager) {
	return entityManager->entityCount - getReservedEntityCount(entityManager);
}


bool entityIdInRange(struct EntityManager* entityManager, int entity) {
	return ((entity >= 0) && (entity < entityManager->entityCount));
}


void reserveEntity(struct EntityManager* entityManager, int entity) {
	if (entityIdInRange(entityManager, entity)) {
		entityManager->entities[entity] = true;
	}
}


bool isEntityFree(struct EntityManager* entityManager, int entity) {
	if (entityIdInRange(entityManager, entity)) {
		return !entityManager->entities[entity];
	} else {
		return false;
	}
}


void unreserveEntityIfNoComponents(struct EntityManager* entityManager, int entity) {
	if (!entityIdInRange(entityManager, entity) || !entityManager->entities[entity]) {
		return;
	}

	if ((entityManager->positions[entity] == NULL) && (entityManager->movements[entity] == NULL)) {
		entityManager->entities[entity] = false;
	}
}


int peekNextFreeEntity(struct EntityManager* entityManager, bool reserve) {
	int freeIndex = -1;

	for (int i = 0; i < entityManager->entityCount; i++) {
		if (!entityManager->entities[i]) {
			freeIndex = i;
			break;
		}
	}

	if (reserve && (freeIndex > -1)) {
		reserveEntity(entityManager, freeIndex);
	}

	return freeIndex;
}


int getNextFreeEntity(struct EntityManager* entityManager) {
	return peekNextFreeEntity(entityManager, true);
}


bool addComponent(struct EntityManager* entityManager, void** componentList, int entity, void* component) {
	if (!entityIdInRange(entityManager, entity)) {
		return false;
	}

	reserveEntity(entityManager, entity);

	if (componentList[entity] != NULL) {
		free(componentList[entity]);
	}

	componentList[entity] = component;

	return true;
}


void* getComponent(struct EntityManager* entityManager, void** componentList, int entity) {
	if (!entityIdInRange(entityManager, entity)) {
		return NULL;
	} else {
		return componentList[entity];
	}
}


void removeComponent(struct EntityManager* entityManager, void** componentList, int entity) {
	if (!entityIdInRange(entityManager, entity)) {
		return;
	}

	if (componentList[entity] != NULL) {
		free(componentList[entity]);
		componentList[entity] = NULL;
	}

	unreserveEntityIfNoComponents(entityManager, entity);
}


void removeEntity(struct EntityManager* entityManager, int entity) {
	removeComponent(entityManager, (void**) entityManager->positions, entity);
	removeComponent(entityManager, (void**) entityManager->movements, entity);
	unreserveEntityIfNoComponents(entityManager, entity);
}


bool entityHasComponent(struct EntityManager* entityManager, void** componentList, int entity) {
	if (!entityIdInRange(entityManager, entity)) {
		return false;
	}

	return (componentList[entity] != NULL);
}


struct EntityNode* getEntitiesWithComponent(struct EntityManager* entityManager, struct EntityPool* entityPool, void** componentList) {
	struct EntityNode* head = NULL;
	struct EntityNode* current = head;

	for (int i = 0; i < entityManager->entityCount; i++) {
		if (componentList[i] != NULL) {
			struct EntityNode* node = getEntityNode(entityPool);
			node->id = i;

			if (head == NULL) {
				head = node;
				current = head;
			} else {
				current->next = node;
				current = node;
			}
		}
	}

	// Ensure the last node in the list points to null
	// (So when traversing, you don't end up looking at free nodes in the pool)
	if (current != NULL) {
		current->next = NULL;
	}

	return head;
}


struct EntityNode* getEntitiesWithComponents2(struct EntityManager* entityManager, struct EntityPool* entityPool, void** componentList0, void** componentList1) {
	struct EntityNode* head = NULL;
	struct EntityNode* current = head;

	for (int i = 0; i < entityManager->entityCount; i++) {
		if ((componentList0[i] != NULL) && (componentList1[i] != NULL)) {
			struct EntityNode* node = getEntityNode(entityPool);
			node->id = i;

			if (head == NULL) {
				head = node;
				current = head;
			} else {
				current->next = node;
				current = node;
			}
		}
	}

	// Ensure the last node in the list points to null
	// (So when traversing, you don't end up looking at free nodes in the pool)
	if (current != NULL) {
		current->next = NULL;
	}

	return head;
}
