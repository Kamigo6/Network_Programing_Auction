#ifndef MODEL_H
#define MODEL_H

// Structure for User
struct User {
    int user_id;
    char name[255];
    char password[255];
    int room_id;  // Foreign key to Room
};

// Structure for Room
struct Room {
    int room_id;
    char name[255];
    int owner_id;  // Foreign key to User
};

struct RoomList {
    struct Room *rooms;
    int size;
};

void initRoomList(struct RoomList *roomList);
void freeRoomList(struct RoomList *roomList);

// Structure for Item
struct Item {
    int item_id;
    char name[255];
    char startTime[20];
    char endTime[20];
    double currentPrice;
    char state[20];
    double buyNowPrice;
    int owner_id;  // Foreign key to User
    int room_id;   // Foreign key to Room
};

struct ItemList {
    struct Item *items;
    int size;
};

void initItemList(struct ItemList *itemList);
void freeItemList(struct ItemList *itemList);

// Structure for Log (bids)
struct Log {
    int log_id;
    int user_id;    // Foreign key to User
    int item_id;    // Foreign key to Item
    int room_id;    // Foreign key to Room
    double bidPrice;
    char time[20];
    char status[20];
};

struct LogList {
    struct Log *logs;
    int size;
};

void initLogList(struct LogList *logList);
void freeLogList(struct LogList *logList);

// Structure for Room Log
struct RoomLog {
    int roomLog_id;
    int item_id;    // Foreign key to Item
    int room_id;    // Foreign key to Room
    char status[20];
};

struct RoomLogList {
    struct RoomLog *roomLogs;
    int size;
};

void initRoomLogList(struct RoomLogList *roomLogList);
void freeRoomLogList(struct RoomLogList *roomLogList);

#endif
