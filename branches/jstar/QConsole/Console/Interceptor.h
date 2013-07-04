
#ifndef INTERCEPTOR_H
#define INTERCEPTOR_H

#include <QString>
#include <string>
#include <boost/python.hpp>

using namespace boost::python;

class Interceptor {
private:    
    QString last_message;
    int type;
public:
    Interceptor(int type_id){        
        type = type_id;
    }
    int get_type() {
        return type;
    }
    void grab(std::string);
    void clear();
    QString output();
};

#endif // INTERCEPTOR_H
