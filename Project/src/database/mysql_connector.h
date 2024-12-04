#ifndef MYSQL_AUCTION_OPERATIONS_H
#define MYSQL_AUCTION_OPERATIONS_H

#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

using namespace std;
using namespace sql;

class MySQLOperations {
private:
    sql::Driver *driver;
    sql::Connection *conn;
    sql::Statement *stmt;

public:
    MySQLOperations();
    ~MySQLOperations();

    // Connect to the database
    bool connect(const string& ipAddress, const string& username, const string& password, const string& database);
    int checkRecord(const string &countQuery);

    // Generic functions
    bool createTableIfNotExists(const string& createTableQuery);
    bool insertRecords(const string& insertQuery);
    void disconnect();

    // Specific operations for Auction Database
    void getRoomList(struct RoomList *roomList, const string& query);
    void getItemList(struct ItemList *itemList, const string& query);
    void getLogList(struct LogList *logList, const string& query);
    void getRoomLogList(struct RoomLogList *roomLogList, const string& query);

    // Auction-specific queries
    bool addNewItem(const string& query);  // Insert new items
    bool placeBid(const string& query);   // Insert new bids into the log table
    bool updateItemPrice(int itemId, const string& query);  // Update current price of an item
    bool updateRoomLogStatus(int roomLogId, const string& query); // Update the room_log status
};

#endif // MYSQL_AUCTION_OPERATIONS_H
