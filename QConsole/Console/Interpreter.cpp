#include <QString>
#include <QFile>
#include <QTextStream>
#include <Interpreter.h>

QString Interpreter::interprete(QString command) {
    QString interpreted_command;
    QTextStream out(stdout);
    //LOG4CXX_INFO(logger,
    //    "Interpreting \"" << command << "\"");

    QString trimmed = command.trimmed();
    if(trimmed.startsWith("load "))
    {
        QString path = trimmed.remove(0,5); // trimmed[0,5] => 'load '
        path = path.trimmed();
        QFile f(path);
        QTextStream fout(&f);
        if(f.open(QFile::ReadOnly)) {
            return fout.readAll();
        } else {
            QString error(
                    "import sys\n"
                    "sys.stderr.write('Could not read file')"
                    );
            return error;
        }
    }

    if(command.endsWith(':')  ||
       command.endsWith('\\') ||
       command.startsWith(' '))
    {
        //LOG4CXX_INFO(logger,
        //"Expecting more.");
        if(buffer.isEmpty()) {
            buffer = command;
        } else {
            buffer += "\n" + command;
        }
        //LOG4CXX_DEBUG(logger,
        //    "Buffer: \"" << buffer << "\"");
        flush(out);
        waiting = true;
    } else {
        if(buffer.isEmpty()) {
            interpreted_command = command;
        } else {
            buffer += "\n" + command;
            interpreted_command = QString(buffer);
            buffer.clear();
        }
        //LOG4CXX_INFO(logger,
        //"Command to execute: \"" << interpreted_command << "\"");
        flush(out);
        waiting = false;
    }
    return interpreted_command;
}
