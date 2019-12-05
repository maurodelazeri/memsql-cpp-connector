# memsql-cpp-connector
MemSQL connector wrapping mariadb for C++ 

```c++
#include <iostream>
#include <sstream>
#include "connection_pool.h"

using namespace std;

int main(int argc, char **argv) {

    std::ostringstream sqlQuery;
    std::shared_ptr<connection_pool> m_pool;
    m_pool = std::make_shared<connection_pool>("x.x.x.x", "root", "pwd", "database", 3306, 1, false);

    sqlQuery << boost::format("INSERT INTO xxxx (x,x,x, x) VALUES ('%1%', '%2%', '%3%', '%4%')")
                % "a"
                % "b"
                % "c";
    shared_ptr<MysqlConnection> con = m_pool->get_connection();
    if (con) {
        try {
            con->execute(sqlQuery.str(), false, true);
            m_pool->release_connection(*con);
        } catch (...) {
            m_pool->release_connection(*con);
        }
    }
    return 0;
}
```
