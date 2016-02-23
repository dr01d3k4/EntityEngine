#include <stdio.h>
#include <stdlib.h>

#include "engine/entity_manager.h"
#include "engine/entity_node.h"
#include "engine/component.h"
#include "engine/system_manager.h"


int createEntity(struct EntityManager* entityManager);
int createEntity(struct EntityManager* entityManager) {
	const int id = getNextFreeEntity(entityManager);
	printf("Id: %d\n", id);

	struct PositionComponent* positionComponent = (struct PositionComponent*) malloc(sizeof(struct PositionComponent));
	positionComponent->x = (id * 10) + 5;

	addComponent(entityManager, (void**) entityManager->positions, id, positionComponent);

	struct PositionComponent* p0 = (struct PositionComponent*) getComponent(entityManager, (void**) entityManager->positions, id);
	printf("x: %f\n", p0->x);
	printf("y: %f\n", p0->y);

	return id;
}


void printAllPositionComponents(struct EntityManager* entityManager, struct EntityPool* entityPool);
void printAllPositionComponents(struct EntityManager* entityManager, struct EntityPool* entityPool) {
	struct EntityNode* head = getEntitiesWithComponent(entityManager, entityPool, (void**) entityManager->positions);
	struct EntityNode* current = head;

	while (current != NULL) {
		printf("Looking at node\n");
		printf("\tID: %d\n", current->id);

		current = current->next;
	}

	returnEntityList(entityPool, head);
}


int main() {
	printf("Hello world!\n");

	const int ENTITY_COUNT = 10;
	const int ENTITY_NODE_COUNT = 20;
	const int SYSTEM_COUNT = 5;

	struct EntityManager* entityManager = createEntityManager(ENTITY_COUNT);
	struct EntityPool* entityPool = createEntityPool(ENTITY_NODE_COUNT);

	printf("Entity count: %d\n", entityManager->entityCount);

	const int playerId = createEntity(entityManager);
	createEntity(entityManager);
	createEntity(entityManager);

	bool playerHasPosition = entityHasComponent(entityManager, (void**) entityManager->positions, playerId);
	printf("Player has position: %d\n", playerHasPosition);

	bool playerHasMovement = entityHasComponent(entityManager, (void**) entityManager->movements, playerId);
	printf("Player has movement: %d\n", playerHasMovement);

	printf("\n");
	printf("Entity in use count: %d\n", getReservedEntityCount(entityManager));
	printf("\n");
	printAllPositionComponents(entityManager, entityPool);

	removeComponent(entityManager, (void**) entityManager->positions, playerId);

	printf("\n");
	printf("Entity in use count: %d\n", getReservedEntityCount(entityManager));
	printf("\n");
	printAllPositionComponents(entityManager, entityPool);

	printf("\n");
	const int newId = createEntity(entityManager);
	printf("New id: %d\n", newId);


	struct SystemManager* systemManager = createSystemManager(SYSTEM_COUNT);


	freeSystemManager(systemManager);
	systemManager = NULL;

	freeEntityPool(entityPool);
	entityPool = NULL;

	freeEntityManager(entityManager);
	entityManager = NULL;

	printf("\n");
	return 0;
}
