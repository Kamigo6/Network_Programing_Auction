#include <stdio.h>
#include <stdlib.h>
#include "model.h"

// RoomList functions
void initRoomList(struct RoomList *roomList) {
    roomList->rooms = NULL;
    roomList->size = 0;
}

void freeRoomList(struct RoomList *roomList) {
    if (roomList->rooms != NULL) {
        free(roomList->rooms);
    }
    roomList->size = 0;
}

// ItemList functions
void initItemList(struct ItemList *itemList) {
    itemList->items = NULL;
    itemList->size = 0;
}

void freeItemList(struct ItemList *itemList) {
    if (itemList->items != NULL) {
        free(itemList->items);
    }
    itemList->size = 0;
}

// LogList functions
void initLogList(struct LogList *logList) {
    logList->logs = NULL;
    logList->size = 0;
}

void freeLogList(struct LogList *logList) {
    if (logList->logs != NULL) {
        free(logList->logs);
    }
    logList->size = 0;
}

// RoomLogList functions
void initRoomLogList(struct RoomLogList *roomLogList) {
    roomLogList->roomLogs = NULL;
    roomLogList->size = 0;
}

void freeRoomLogList(struct RoomLogList *roomLogList) {
    if (roomLogList->roomLogs != NULL) {
        free(roomLogList->roomLogs);
    }
    roomLogList->size = 0;
}
