
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QString>

class Interpreter
{
private:
    QString buffer;
    bool waiting;
public:
    Interpreter() {
        waiting = false;
    }
    bool is_waiting() {
        return waiting;
    }
    QString interprete (QString command);

};

#endif // INTERPRETER_H
