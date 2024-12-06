**DOCKER:**
Install docker

Create docker container for database:
```
docker compose up -d
```

Shutdown docker container:
```
docker compose down -v
```
**DATABASE CONNECTION:**
Install mysql connector/C++:
```shell
sudo apt-get install libmysqlcppconn-dev
```

Run sample code to verify whether mysql connector lib is installed:
```
g++ -o test_mysql test_mysql.cpp -lmysqlcppconn
./test_mysql
```

Run sample code to verify whether mysql connector can connect with database:
```
g++ -o testConnectDB testConnectDB.cpp -lmysqlcppconn
./testConnectDB
```

**RUN INSTRUCTION:**

#### server

```bash
g++ -o server server.cpp model.cpp database/mysql_connector.cpp -lmysqlcppconn
./server
```

#### client
```bash
g++ -o client client.cpp
./client 127.0.0.1
```