#include <stdbool.h>
#pragma once

// Systems are just functions
typedef void (*SystemUpdate)(float deltaTime);


struct SystemManager {
	const int systemCount;
	SystemUpdate* systems;
};


struct SystemManager* createSystemManager(int systemCount);


void freeSystemManager(struct SystemManager* systemManager);


// Returns true if priority is between 0 and systemCount
bool priorityInRange(struct SystemManager* systemManager, int priority);


// If there already is a system with this priority, returns false
// Else puts the system into the manager and returns true
bool addSystem(struct SystemManager* systemManager, SystemUpdate system, int priority);


// Adds a system in the next available priority slot
// Returns the priority it was placed in, or -1 if there are no free slots
int addNextSystem(struct SystemManager* systemManager, SystemUpdate system);


// Removes a system at the given priority
void removeSystem(struct SystemManager* systemManager, int priority);


// Removes a system and shifts down all systems after it to leave no gaps
void removeSystemShiftDown(struct SystemManager* systemManager, int priority);
