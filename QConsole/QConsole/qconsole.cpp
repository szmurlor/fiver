#include <QList>
#include <QTextStream>
#include <QString>
#include <Console.h>
#include <Interceptor.h>
#include <InterceptorInjector.h>
#include <Injector.h>
#include <Interpreter.h>
#include <Executer.h>
#include "qconsole.h"
#include "ui_qconsole.h"
/**
  * TODO: Think about interpreted commands such as:
  *         load <filename> - loading script file
  */

QConsole::QConsole(QWidget *parent)
    : QWidget(parent), ui(new Ui::QConsole)
{

    console = new Console(new Executer,new Interpreter);
    ui->setupUi(this);
}

QConsole::~QConsole()
{
    delete console;
    delete ui;
}

void QConsole::inject(void *obj, QString name, Injector *injector) {
    // Pass args to Console class
    console->inject(obj, name, injector);
}

void QConsole::introduce(Injector *injector) {
    // Pass args to Console class
    console->introduce(injector);
}

void QConsole::on_consoleView_cursorPositionChanged()
{
    // FIXME: add try-catch?
    QConsoleTextEdit *w = static_cast<QConsoleTextEdit*>(ui->consoleView);
    w->protectPrompt();
}

void QConsole::on_consoleView_commandInvoked(QString& command) {
    QString output = console->evaluate(command);

    if(!output.isEmpty()) {
        // Get type of Interceptor that grabbed this output
        int output_type = console->get_output_type();

        // Change color of output, base on type of
        // source Interceptor
        if(output_type == Console::STDOUT)
            ui->consoleView->setTextColor(Qt::black);
        else if(output_type == Console::STDERR)
            ui->consoleView->setTextColor(Qt::red);

        ui->consoleView->append(output);
    }

    QConsoleTextEdit *w = static_cast<QConsoleTextEdit*>(ui->consoleView);
    if(console->is_waiting()) // Console is waiting for more
                              // input before execution f.eg.
                              // loops, conditions
        w->displayWaitingPrompt();
    else
        w->displayNormalPrompt();
}

void QConsole::on_consoleView_browseHistory(QString& text, QString& command,
                                            int direction) {
    //QTextStream out(stdout);
    QList<QString> *history = console->get_history();
    QList<QString> filtered;
    // Filter history
    for(QList<QString>::iterator i = history->begin();
        i != history->end();
        i++)
    {
        QString element = (*i);
        if(element.startsWith(text)){
            filtered.push_front(element);
            //out << "+ " << element << "\n";
        } else {
            //out << "- " << element << "\n";
        }
    }
    // Find if we can find command in filtered list
    int index = filtered.indexOf(command);

    //out << "Found " << command << " in list at #" << index << "\n\n";

    if(index + direction < filtered.size() &&
       index + direction >= 0) {
        QConsoleTextEdit *w = static_cast<QConsoleTextEdit*>(ui->consoleView);
        int position = w->textCursor().position();
        QString text = filtered.at(index + direction);
        w->clearInput();
        w->insertPlainText(text);
        QTextCursor c = w->textCursor();
        c.setPosition(position);
        w->setTextCursor(c);
    }
}
