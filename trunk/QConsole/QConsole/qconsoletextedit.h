#ifndef QCONSOLETEXTEDIT_H
#define QCONSOLETEXTEDIT_H

#include <QTextEdit>

class QConsoleTextEdit : public QTextEdit
{
    Q_OBJECT
private:
    int tabsize;
    int promptParagraph; // position of prompt's last character
    QString getInput(); // Returns text (after prompt) entered by user
    QString getText(); // Returns text between prompt and cursor
    QTextCursor resetCursor(); // Moves cursor just after prompt
    bool protectPromptEvent(QEvent *); // don't let anybody modify prompt
    void displayPrompt(QString *); // display various types of prompt
                                   // waitingPrompt, normalPrompt
    QString *prompt;               // default prompt '>>> '
    QString *waitingPrompt;        // "waiting" prompt '... '
public:
    QConsoleTextEdit(QWidget *parent = 0);
    virtual ~QConsoleTextEdit();
    virtual bool event(QEvent *);

    void displayNormalPrompt();
    void displayWaitingPrompt();
    void protectPrompt();
    void clearInput();
signals:
    void commandInvoked(QString&);
    void browseHistory(QString&, QString&, int);
};

#endif // QCONSOLETEXTEDIT_H
