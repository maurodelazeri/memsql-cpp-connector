# memsql-cpp-connector
Simple MemSQL connector with pool of connections for c++

### Dependencies

http://download.memsql.com/clients/mariadb-connector-c-3.0.9-linux-x86_64.tar.gz

I'm using boost, but you can easily removed if wanted


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

It's simple, ready to go...

You can for example check the affected rows using 

```c++
std::shared_ptr<my_ulonglong> affectRowsPtr = std::make_shared<my_ulonglong>();
con->execute(sqlQuery.str(), false, true, affectRowsPtr.get());
if (*affectRowsPtr == 0) {
  cout << "No changes" << endl;
}
```

or you can simple select data:

```c++
#include <iostream>
#include <sstream>
#include "connection_pool.h"

using namespace std;

int main(int argc, char **argv) {

    std::ostringstream sqlQuery;
    auto product_id = 2;
    sqlQuery << boost::format(
            "select * venue from products where product_id=%1%")
                % product_id;

    shared_ptr<MysqlConnection> con = m_pool->get_connection();
    if (con) {
        auto data = con->open(sqlQuery.str());
        if (!data->is_valid()) {
            return false;
        }

        auto row = data->next();
        unsigned int row_idx = 0;
        while (row) {
            row_idx++;

            cout << row->get_value(0) << endl;

            row = data->next();
        }
        
        m_pool->release_connection(*con);
    }
    
    return 0;
}    
```
