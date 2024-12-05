#include "mysql_connector.h"
#include "../model.h"
#include <string.h>

MySQLOperations::MySQLOperations()
{
    driver = get_driver_instance();
    conn = nullptr;
    stmt = nullptr;
}

MySQLOperations::~MySQLOperations()
{
    delete stmt;
    delete conn;
}

bool MySQLOperations::connect(const string &ipAddress, const string &username, const string &password, const string &database)
{
    try
    {
        conn = driver->connect(ipAddress, username, password);
        if (conn)
        {
            conn->setSchema(database);
            stmt = conn->createStatement();
            return true;
        }
    }
    catch (SQLException &e)
    {
        cerr << "Error connecting: " << e.what() << endl;
    }
    return false;
}

int MySQLOperations::checkRecord(const string &countQuery)
{
    try
    {
        sql::ResultSet *res = stmt->executeQuery(countQuery);
        if (res->next()) {
            return 1;
        }
        else return 0;
    }
    catch (SQLException &e)
    {
        cerr << "Error: " << e.what() << endl;
        return 0;
    }
}

bool MySQLOperations::insertRecords(const string &insertQuery)
{
    try
    {
        stmt->execute(insertQuery);
        return true;
    }
    catch (SQLException &e)
    {
        cerr << "Error inserting records: " << e.what() << endl;
    }
    return false;
}

sql::ResultSet* MySQLOperations::executeQuery(const string &query)
{
    try
    {
        return stmt->executeQuery(query);
    }
    catch (SQLException &e)
    {
        cerr << "Error executing query: " << e.what() << endl;
        return nullptr;
    }
}

void MySQLOperations::getRoomList(struct RoomList *roomList, const string &query)
{
    try
    {
        sql::ResultSet *res = stmt->executeQuery(query);
        int roomListSize = roomList->size;
        while (res->next())
        {
            if (roomList->rooms == NULL)
            {
                roomList->rooms = (struct Room *)malloc(sizeof(struct Room));
            }
            else
            {
                roomList->rooms = (struct Room *)realloc(roomList->rooms, (roomListSize + 1) * sizeof(struct Room));
            }
            if (roomList->rooms == NULL)
            {
                fprintf(stderr, "Memory allocation failed.\n");
                exit(1);
            }
            roomList->rooms[roomListSize].room_id = res->getInt("room_id");
            roomList->rooms[roomListSize].owner_id = res->getInt("owner_id");
            strncpy(roomList->rooms[roomListSize].name, res->getString("name").c_str(), sizeof(roomList->rooms[roomListSize].name) - 1);
            roomList->rooms[roomListSize].name[sizeof(roomList->rooms[roomListSize].name) - 1] = '\0';
            roomList->size = ++roomListSize;
        }
        delete res;
    }
    catch (SQLException &e)
    {
        cerr << "Error selecting rooms: " << e.what() << endl;
    }
}

void MySQLOperations::getItemList(struct ItemList *itemList, const string &query)
{
    try
    {
        sql::ResultSet *res = stmt->executeQuery(query);
        int itemListSize = itemList->size;
        while (res->next())
        {
            if (itemList->items == NULL)
            {
                itemList->items = (struct Item *)malloc(sizeof(struct Item));
            }
            else
            {
                itemList->items = (struct Item *)realloc(itemList->items, (itemListSize + 1) * sizeof(struct Item));
            }
            if (itemList->items == NULL)
            {
                fprintf(stderr, "Memory allocation failed.\n");
                exit(1);
            }
            itemList->items[itemListSize].item_id = res->getInt("item_id");
            strncpy(itemList->items[itemListSize].name, res->getString("name").c_str(), sizeof(itemList->items[itemListSize].name) - 1);
            itemList->items[itemListSize].name[sizeof(itemList->items[itemListSize].name) - 1] = '\0';
            itemList->items[itemListSize].currentPrice = res->getDouble("startingPrice");
            itemList->items[itemListSize].buyNowPrice = res->getDouble("buyNowPrice");
            itemList->items[itemListSize].room_id = res->getInt("room_id");
            itemList->size = ++itemListSize;
        }
        delete res;
    }
    catch (SQLException &e)
    {
        cerr << "Error selecting items: " << e.what() << endl;
    }
}

void MySQLOperations::disconnect()
{
    delete stmt;
    delete conn;
}
