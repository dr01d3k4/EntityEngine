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


bool addSystem(struct SystemManager* systemManager, SystemUpdate system, int priority) {
}


int addNextSystem(struct SystemManager* systemManager, SystemUpdate system) {
}


void removeSystem(struct SystemManager* systemManager, int priority) {
}


void removeSystemShiftDown(struct SystemManager* systemManager, int priority) {
}
