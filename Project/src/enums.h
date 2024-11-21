#ifndef ENUMS_H
#define ENUMS_H

const char * database = "auctionDb";
const char * username = "root";
const char * password = "123";
const char * ipAddress = "tcp://127.0.0.1:3309";

enum UserRequest {
    LOGIN_REQ = 1,
    REGISTER_REQ = 2,
    VIEW_ALL_ROOMS_REQ = 3,
    CREATE_ROOM_REQ = 4,
    VIEW_ROOM_LOG_REQ = 5,
    PLACE_ITEM_IN_ROOM_REQ = 6,
    ACCEPT_REJECT_ITEM_REQ = 7,
    VIEW_ITEMS_IN_ROOM_REQ = 8,
    DELETE_ITEM_IN_ROOM_REQ = 9,
    JOIN_ROOM_REQ = 10,
    LEAVE_ROOM_REQ = 11,
    VIEW_ALL_ITEMS_REQ = 12,
    SEARCH_ITEM_BY_NAME_REQ = 13,
    SEARCH_ITEM_BY_DURATION_REQ = 14,
    PLACE_BID_REQ = 15,
    BUY_NOW_REQ = 16,
    DELETE_ITEM_FROM_ROOM_REQ = 17,
    DELETE_ITEM_REQ = 18,
    UPDATE_ITEM_REQ = 19,
    VIEW_USER_LOG_REQ = 20
};

enum ServerResponse {
    LOGIN_RES = 1,
    REGISTER_RES = 2,
    VIEW_ALL_ROOMS_RES = 3,
    CREATE_ROOM_RES = 4,
    VIEW_ROOM_LOG_RES = 5,
    PLACE_ITEM_IN_ROOM_RES = 6,
    ACCEPT_REJECT_ITEM_RES = 7,
    VIEW_ITEMS_IN_ROOM_RES = 8,
    DELETE_ITEM_IN_ROOM_RES = 9,
    JOIN_ROOM_RES = 10,
    LEAVE_ROOM_RES = 11,
    VIEW_ALL_ITEMS_RES = 12,
    SEARCH_ITEM_BY_NAME_RES = 13,
    SEARCH_ITEM_BY_DURATION_RES = 14,
    PLACE_BID_RES = 15,
    BUY_NOW_RES = 16,
    DELETE_ITEM_FROM_ROOM_RES = 17,
    DELETE_ITEM_RES = 18,
    UPDATE_ITEM_RES = 19,
    VIEW_USER_LOG_RES = 20
};

enum StatusCode {
    FAIL = 0,
    SUCCESS = 1
};

#endif // ENUMS_H
