#ifndef CONSOLE_H
#define CONSOLE_H

#include <QString>
#include <QList>

#include <Injector.h>
#include <Interpreter.h>
#include <Interceptor.h>
#include <Executer.h>

class Console
{
public:
    static const int NONE = -1;
    static const int STDERR = 0;
    static const int STDOUT = 1;

    Console (Executer *, Interpreter *);
    ~Console ();

    QString evaluate (QString);
    void inject(void*, QString, Injector*);
    void introduce(Injector*);

    bool is_waiting() {
        return interpreter->is_waiting();
    }

    int get_output_type() {
        return last_output_type;
    }

    QList<QString> *get_history() {
        return history;
    }

private:
    QList<QString> *history;
    QList<Interceptor*> *interceptor;
    Executer *executer;
    Interpreter *interpreter;
    int last_output_type;
};

#endif // CONSOLE_H
