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


// Returns then next free slot available after start (or start if start is free)
// If there are no free slots, returns systemCount
int getNextFreeSlot(struct SystemManager* systemManager, int start);


// Calls getNextFreeSlot with start of 0
int getFirstFreeSlot(struct SystemManager* systemManager);


// If there already is a system with this priority, returns false
// Else puts the system into the manager and returns true
bool addSystem(struct SystemManager* systemManager, SystemUpdate system, int priority);


// If there already is a system with this priority, shifts all the following systems up to fit this one in
// If there isn't enough space for all systems (i.e. the last one gets cut off), returns false
// Else returns true
bool addSystemShiftUp(struct SystemManager* systemManager, SystemUpdate system, int priority);


// Adds a system in the first available priority slot
// Returns the priority it was placed in, or -1 if there are no free slots
int addNextSystem(struct SystemManager* systemManager, SystemUpdate system);


// Removes a system at the given priority
void removeSystem(struct SystemManager* systemManager, int priority);


// Removes a system and shifts down all systems after it to leave no gaps
void removeSystemShiftDown(struct SystemManager* systemManager, int priority);
