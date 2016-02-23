#include <stdlib.h>
#include "system_manager.h"


struct SystemManager* createSystemManager(int systemCount) {
	struct SystemManager* systemManager = (struct SystemManager*) malloc(sizeof(struct SystemManager));
	*((int*) &systemManager->systemCount) = systemCount;

	systemManager->systems = (SystemUpdate*) calloc(systemCount, sizeof(SystemUpdate));

	return systemManager;
}


void freeSystemManager(struct SystemManager* systemManager) {
	free(systemManager->systems);
	free(systemManager);
}


bool priorityInRange(struct SystemManager* systemManager, int priority) {
	return ((priority >= 0) && (priority < systemManager->systemCount));
}


int getNextFreeSlot(struct SystemManager* systemManager, int start) {
	int nextFree = start;
	while ((nextFree < systemManager->systemCount) && (systemManager->systems[nextFree] != NULL)) {
		nextFree++;
	}
	return nextFree;
}


int getFirstFreeSlot(struct SystemManager* systemManager) {
	return getNextFreeSlot(systemManager, 0);
}


bool addSystem(struct SystemManager* systemManager, SystemUpdate system, int priority) {
	if (!priorityInRange(systemManager, priority)) {
		return false;
	}

	if (systemManager->systems[priority] != NULL) {
		return false;
	}

	systemManager->systems[priority] = system;
	return true;
}


bool addSystemShiftUp(struct SystemManager* systemManager, SystemUpdate system, int priority) {
	if (!priorityInRange(systemManager, priority)) {
		return false;
	}

	bool allFit = true;

	if (systemManager->systems[priority] != NULL) {
		// Get the next free slot
		int nextFree = getNextFreeSlot(systemManager, priority);

		// If there are no more free slots
		if (nextFree == systemManager->systemCount) {
			allFit = false;
			nextFree--;
		}

		// Point to slot before the free slot
		nextFree--;

		for (int i = nextFree; i >= priority; i--) {
			systemManager->systems[i + 1] = systemManager->systems[i];
		}
	}

	systemManager->systems[priority] = system;
	return allFit;
}



int addNextSystem(struct SystemManager* systemManager, SystemUpdate system) {
	int firstFree = getFirstFreeSlot(systemManager);

	if (firstFree == systemManager->systemCount) {
		return -1;
	}

	systemManager->systems[firstFree] = system;
	return firstFree;
}


void removeSystem(struct SystemManager* systemManager, int priority) {
	if (!priorityInRange(systemManager, priority)) {
		return;
	}

	systemManager->systems[priority] = NULL;
}


void removeSystemShiftDown(struct SystemManager* systemManager, int priority) {
	if (!priorityInRange(systemManager, priority)) {
		return;
	}

	// If system count is 10, iterate to index 8
	// So index 8 gets set to value of index 9 (the last)
	for (int i = priority; i < systemManager->systemCount - 1; i++) {
		systemManager->systems[i] = systemManager->systems[i + 1];
	}

	// Set the last system to null so that it's not duplicated (i.e. in last and last-1 indices)
	systemManager->systems[systemManager->systemCount - 1] = NULL;
}
