#ifndef QCONSOLE_H
#define QCONSOLE_H

#include <QtGui/QWidget>
#include <QString>
#include <Console.h>
#include <Injector.h>

namespace Ui
{
    class QConsole;
}

class QConsole : public QWidget
{
    Q_OBJECT
public:
    QConsole(QWidget *parent = 0);
    virtual ~QConsole();

    void introduce(Injector *);
    void inject(void*, QString, Injector *);

private:
    Console *console;

    Ui::QConsole *ui;

private slots:
    void on_consoleView_cursorPositionChanged();
    void on_consoleView_commandInvoked(QString&);
    void on_consoleView_browseHistory(QString&, QString&, int);
};

#endif // QCONSOLE_H
