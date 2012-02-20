#include <QString>
#include <string>
#include <Interceptor.h>

void Interceptor::grab(std::string message) {
    QString msg(message.c_str());
    if (!msg.isEmpty())
        last_message += msg + "\n";

    //LOG4CXX_DEBUG(logger, "Grabbed: '" << message << "'");
}

void Interceptor::clear() {
    //LOG4CXX_DEBUG(logger, "Cleared last message: " << last_message);
    last_message.clear();
}

QString Interceptor::output() {
    int len = last_message.length();
    if(len == 0)
        return QString();
    else if(last_message.endsWith('\n'))
        return last_message.left(len-1);
    else
        return last_message;
}
