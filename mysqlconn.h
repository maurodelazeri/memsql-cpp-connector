#pragma once

#include <cassert>
#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>
#include <mysql/mysql.h>

using std::string;
using std::shared_ptr;

class MysqlLock {
public:
    MysqlLock();
    virtual ~MysqlLock();

    void unlock();
protected:
    virtual void lock();
    virtual bool trylock();

private:
    std::mutex _cts;
};


class MysqlRow {
public:
    [[nodiscard]] string get_value(size_t index) const;
    void set_value(size_t index, const string& value);
    void add_value(const string& value);
private:
    std::vector <string> _values;
};

class MysqlData {
public:
    MysqlData();
    virtual ~MysqlData();

    bool store(MYSQL* conn);
    [[nodiscard]] bool is_valid() const;

    [[nodiscard]] size_t get_columnCount() const;
    string get_columnName(size_t index);

    shared_ptr<MysqlRow> next();
    bool more();

private:
    MYSQL* _conn;
    MYSQL_RES* _sqlResult;

    std::vector<string> _columns;
};


class MysqlConnection: public MysqlLock {
public:
    MysqlConnection();
    ~MysqlConnection() override;

    bool connect(
            const string& host,
            const string& user,
            const string& passwd,
            const string& database,
            unsigned int port = 0 );
    bool disconnect();
    [[nodiscard]] bool is_connected() const;

    [[nodiscard]] shared_ptr<MysqlData> open(const string& query) const;
    bool exec(const string& query, bool multiline = false, my_ulonglong* affectRowsPtr = nullptr) const;
    bool execute(const string& query, bool multiline = false, bool transactionOnExecute = true, my_ulonglong* affectRowsPtr = nullptr) const;

    [[nodiscard]] bool ping() const;
    [[nodiscard]] my_ulonglong affectrows() const;
    [[nodiscard]] const char * lastError() const;
    [[nodiscard]] long long lastInsertID() const;

    void transactionStart() const;
    void transactionCommit() const;
    void transactionRollback() const;

    void print() const;
public:
    void unlock(bool do_disconnect = false);
public:
    void lock() override;
    bool trylock() override;

private:
    MYSQL* _mysql;
    MYSQL* _conn;
};

class MysqlConnPool: public MysqlLock {
public:
    MysqlConnPool(
            unsigned int poolCount,
            string  host,
            string  user,
            string  passwd,
            string  database,
            unsigned int port = 0 );
    ~MysqlConnPool() override;

    [[nodiscard]] bool checkConnection() const;
    shared_ptr<MysqlConnection> lockConnection();

private:
    string _host;
    string _user;
    string _passwd;
    string _database;
    unsigned int _port;

    size_t _connIndex;
    std::vector<shared_ptr<MysqlConnection>> _connList;

};