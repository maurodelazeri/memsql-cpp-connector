#pragma once

#include "mysqlconn.h"
#include <memory>
#include <boost/format.hpp>

class connection_pool {
public:
    explicit connection_pool(
            const std::string& host, const std::string& user, const std::string& passwd, const std::string& database,
            uint16_t port, uint16_t max_conn, bool do_closeconn_on_unlock);

    ~connection_pool();

    shared_ptr<MysqlConnection> get_connection();

    void release_connection(MysqlConnection &con);

private:
    MysqlConnPool m_pool;

    bool _do_closeconn_on_unlock;
};