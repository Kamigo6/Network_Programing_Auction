#ifndef ENUMS_H
#define ENUMS_H

const char * database = "auctionDb";
const char * username = "root";
const char * password = "123";
const char * ipAddress = "tcp://127.0.0.1:3309";

enum UserRequest {
    LOGIN_REQ = 1,
    LOGOUT_REQ = 2,
    REGISTER_REQ = 3,
    VIEW_ROOMS_REQ = 4,
    CREATE_ROOM_REQ = 5,
    VIEW_ROOM_OWNED_REQ = 6,
    VIEW_ROOM_LOG_REQ = 7,
    PLACE_ITEM_IN_ROOM_REQ = 8,
    ACCEPT_REJECT_ITEM_REQ = 9,
    VIEW_ITEMS_IN_ROOM_REQ = 10,
    DELETE_ITEM_IN_ROOM_REQ = 11,
    JOIN_ROOM_REQ = 12,
    LEAVE_ROOM_REQ = 13,
    VIEW_ALL_ITEMS_REQ = 14,
    SEARCH_ITEM_BY_NAME_REQ = 15,
    SEARCH_ITEM_BY_DURATION_REQ = 16,
    PLACE_BID_REQ = 17,
    BUY_NOW_REQ = 18,
    DELETE_ITEM_FROM_ROOM_REQ = 19,
    DELETE_ITEM_REQ = 20,
    UPDATE_ITEM_REQ = 21,
    VIEW_USER_LOG_REQ = 22
};

enum ServerResponse {
    LOGIN_RES = 1,
    LOGOUT_RES = 2,
    REGISTER_RES = 3,
    VIEW_ROOMS_RES = 4,
    CREATE_ROOM_RES = 5,
    VIEW_ROOM_OWNED_RES = 6,
    VIEW_ROOM_LOG_RES = 7,
    PLACE_ITEM_IN_ROOM_RES = 8,
    ACCEPT_REJECT_ITEM_RES = 9,
    VIEW_ITEMS_IN_ROOM_RES = 10,
    DELETE_ITEM_IN_ROOM_RES = 11,
    JOIN_ROOM_RES = 12,
    LEAVE_ROOM_RES = 13,
    VIEW_ALL_ITEMS_RES = 14,
    SEARCH_ITEM_BY_NAME_RES = 15,
    SEARCH_ITEM_BY_DURATION_RES = 16,
    PLACE_BID_RES = 17,
    BUY_NOW_RES = 18,
    DELETE_ITEM_FROM_ROOM_RES = 19,
    DELETE_ITEM_RES = 20,
    UPDATE_ITEM_RES = 21,
    VIEW_USER_LOG_RES = 22
};

enum StatusCode {
    FAIL = 0,
    SUCCESS = 1
};

const char* UserRequestToString(UserRequest req) {
    switch (req) {
        case LOGIN_REQ: return "LOGIN_REQ";
        case LOGOUT_REQ: return "LOGOUT_REQ";
        case REGISTER_REQ: return "REGISTER_REQ";
        case VIEW_ROOMS_REQ: return "VIEW_ROOMS_REQ";
        case CREATE_ROOM_REQ: return "CREATE_ROOM_REQ";
        case VIEW_ROOM_OWNED_REQ: return "VIEW_ROOM_OWNED_REQ";
        case VIEW_ROOM_LOG_REQ: return "VIEW_ROOM_LOG_REQ";
        case PLACE_ITEM_IN_ROOM_REQ: return "PLACE_ITEM_IN_ROOM_REQ";
        case ACCEPT_REJECT_ITEM_REQ: return "ACCEPT_REJECT_ITEM_REQ";
        case VIEW_ITEMS_IN_ROOM_REQ: return "VIEW_ITEMS_IN_ROOM_REQ";
        case DELETE_ITEM_IN_ROOM_REQ: return "DELETE_ITEM_IN_ROOM_REQ";
        case JOIN_ROOM_REQ: return "JOIN_ROOM_REQ";
        case LEAVE_ROOM_REQ: return "LEAVE_ROOM_REQ";
        case VIEW_ALL_ITEMS_REQ: return "VIEW_ALL_ITEMS_REQ";
        case SEARCH_ITEM_BY_NAME_REQ: return "SEARCH_ITEM_BY_NAME_REQ";
        case SEARCH_ITEM_BY_DURATION_REQ: return "SEARCH_ITEM_BY_DURATION_REQ";
        case PLACE_BID_REQ: return "PLACE_BID_REQ";
        case BUY_NOW_REQ: return "BUY_NOW_REQ";
        case DELETE_ITEM_FROM_ROOM_REQ: return "DELETE_ITEM_FROM_ROOM_REQ";
        case DELETE_ITEM_REQ: return "DELETE_ITEM_REQ";
        case UPDATE_ITEM_REQ: return "UPDATE_ITEM_REQ";
        case VIEW_USER_LOG_REQ: return "VIEW_USER_LOG_REQ";
        default: return "UNKNOWN_REQ";
    }
}

const char* ServerResponseToString(ServerResponse res) {
    switch (res) {
        case LOGIN_RES: return "LOGIN_RES";
        case LOGOUT_RES: return "LOGOUT_RES";
        case REGISTER_RES: return "REGISTER_RES";
        case VIEW_ROOMS_RES: return "VIEW_ROOMS_RES";
        case CREATE_ROOM_RES: return "CREATE_ROOM_RES";
        case VIEW_ROOM_OWNED_RES: return "VIEW_ROOM_OWNED_RES";
        case VIEW_ROOM_LOG_RES: return "VIEW_ROOM_LOG_RES";
        case PLACE_ITEM_IN_ROOM_RES: return "PLACE_ITEM_IN_ROOM_RES";
        case ACCEPT_REJECT_ITEM_RES: return "ACCEPT_REJECT_ITEM_RES";
        case VIEW_ITEMS_IN_ROOM_RES: return "VIEW_ITEMS_IN_ROOM_RES";
        case DELETE_ITEM_IN_ROOM_RES: return "DELETE_ITEM_IN_ROOM_RES";
        case JOIN_ROOM_RES: return "JOIN_ROOM_RES";
        case LEAVE_ROOM_RES: return "LEAVE_ROOM_RES";
        case VIEW_ALL_ITEMS_RES: return "VIEW_ALL_ITEMS_RES";
        case SEARCH_ITEM_BY_NAME_RES: return "SEARCH_ITEM_BY_NAME_RES";
        case SEARCH_ITEM_BY_DURATION_RES: return "SEARCH_ITEM_BY_DURATION_RES";
        case PLACE_BID_RES: return "PLACE_BID_RES";
        case BUY_NOW_RES: return "BUY_NOW_RES";
        case DELETE_ITEM_FROM_ROOM_RES: return "DELETE_ITEM_FROM_ROOM_RES";
        case DELETE_ITEM_RES: return "DELETE_ITEM_RES";
        case UPDATE_ITEM_RES: return "UPDATE_ITEM_RES";
        case VIEW_USER_LOG_RES: return "VIEW_USER_LOG_RES";
        default: return "UNKNOWN_RES";
    }
}

#endif // ENUMS_H
