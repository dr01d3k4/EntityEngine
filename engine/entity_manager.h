#include <stdbool.h>
#pragma once


struct EntityNode;
struct EntityPool;
struct PositionComponent;
struct MovementComponent;


struct EntityManager {
	const int entityCount;
	// Array of entities
	// Index is true if entity exists, else false
	bool* entities;
	struct PositionComponent** positions;
	struct MovementComponent** movements;
};


// Allocates space for all the arrays
struct EntityManager* createEntityManager(const int entityCount);


// Frees all the components, the arrays and the entity manager itself
void freeEntityManager(struct EntityManager* entityManager);


// Returns the number of entities currently in use
int getReservedEntityCount(struct EntityManager* entityManager);


// Returns the number of free entities in the entity manager
int getFreeEntityCount(struct EntityManager* entityManager);


// Returns true if entity is between 0 and entityCount, else false
bool entityIdInRange(struct EntityManager* entityManager, int entity);


// Marks an entity as being in use
void reserveEntity(struct EntityManager* entityManager, int entity);


// Returns true if an entity is not in use according to the entities array
bool isEntityFree(struct EntityManager* entityManager, int entity);


// Marks an entity as free if it has no components
// Does nothing if the entity is already marked as free (or out of bounds)
void unreserveEntityIfNoComponents(struct EntityManager* entityManager, int entity);


// Returns the index for the next free entity
// If reserve is true, it calls reserveEntity()
// Returns -1 if there is no more free entities remaining
int peekNextFreeEntity(struct EntityManager* entityManager, bool reserve);


// Calls peekNextFreeEntity() with reserve = true
int getNextFreeEntity(struct EntityManager* entityManager);


// Attaches a component to an entity in the given list
// If there already is a component, it frees it and replaces it with the new one
// Also calls reserveEntity to ensure that this entity is now no longer free
// Returns true if the component was added, else false
// If it returns false, you should free the component because it is not being managed by entity manager
bool addComponent(struct EntityManager* entityManager, void** componentList, int entity, void* component);


// Returns the component for the entity from the given list
// If there is no component or the entity index is out of bounds, returns null
void* getComponent(struct EntityManager* entityManager, void** componentList, int entity);


// Removes and frees the component for the entity from the given list
void removeComponent(struct EntityManager* entityManager, void** componentList, int entity);


// Removes and frees all of an entity's components and marks it as unused
void removeEntity(struct EntityManager* entityManager, int entity);


// Returns true if the entity has a component in the given list
bool entityHasComponent(struct EntityManager* entityManager, void** componentList, int entity);


// Returns a linked list of all entities that have a component in the given list
// The list is ordered so that lower integer entities appear first
struct EntityNode* getEntitiesWithComponent(struct EntityManager* entityManager, struct EntityPool* entityPool, void** componentList);


// See getEntitiesWithComponent
// Does the same but checks for 2 component types per entity
struct EntityNode* getEntitiesWithComponents2(struct EntityManager* entityManager, struct EntityPool* entityPool, void** componentList0, void** componentList1);
