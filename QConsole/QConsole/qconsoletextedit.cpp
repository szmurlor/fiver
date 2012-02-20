#include <QWidget>
#include <QKeyEvent>
#include <QEvent>
#include "qconsoletextedit.h"

QConsoleTextEdit::QConsoleTextEdit(QWidget *parent):
        QTextEdit(parent)
{
    setFontFamily("Courier");
    tabsize = 4;
    // It looks better when prompt and
    // waitingPrompt is the same length
    prompt = new QString(">>> ");
    waitingPrompt = new QString("... ");
    displayNormalPrompt();
}

QTextCursor QConsoleTextEdit::resetCursor() {
    QTextCursor c = textCursor();
    c.setPosition(promptParagraph);
    return c;
}

void QConsoleTextEdit::clearInput() {
    QTextCursor c = textCursor();
    QString text = getInput();
    c.setPosition(promptParagraph);
    c.setPosition(promptParagraph + text.size(),
                  QTextCursor::KeepAnchor);
    c.removeSelectedText();
}

QString QConsoleTextEdit::getText() {
    QTextCursor c = textCursor();
    int position = c.position();
    c.setPosition(promptParagraph);
    c.setPosition(position, QTextCursor::KeepAnchor);
    return c.selectedText();
}

QString QConsoleTextEdit::getInput() {
    QTextCursor c = resetCursor();
    c.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    return c.selectedText();
}

void QConsoleTextEdit::displayNormalPrompt() {
    displayPrompt(prompt);
}

void QConsoleTextEdit::displayWaitingPrompt() {
    displayPrompt(waitingPrompt);
}

void QConsoleTextEdit::displayPrompt(QString* p) {
    blockSignals(true); // We do this to avoid triggering
                        // onCursorPositionChange signal
    moveCursor(QTextCursor::EndOfLine);
    setTextColor(Qt::black);
    append(*p);
    moveCursor(QTextCursor::End);
    blockSignals(false);
    // Position just after prompt
    // Initially should be equal to p->size()
    promptParagraph = textCursor().position();
}

QConsoleTextEdit::~QConsoleTextEdit() {}

bool QConsoleTextEdit::protectPromptEvent(QEvent *event){
    // Block event if cursor is going inside prompt
    if(textCursor().position() > promptParagraph)
        return QTextEdit::event(event);
    return false;
}

void QConsoleTextEdit::protectPrompt() {
    // Move cursor to valid position
    QTextCursor c = textCursor();

    if(c.position() < promptParagraph) {
        if(c.selectionEnd() > promptParagraph ) {
            // Keep selection
            c.setPosition(promptParagraph, QTextCursor::KeepAnchor);
        } else if(c.selectionEnd() >= 0) {
            c.setPosition(promptParagraph);
        }
        setTextCursor(c);
    }
}
bool QConsoleTextEdit::event(QEvent *event)
{
    QKeyEvent *kevent = static_cast<QKeyEvent*>(event);
    if(event->type() == QEvent::KeyPress) {
        switch(kevent->key()) {
        case Qt::Key_Up:
            {
                QString text = getText();
                QString command = getInput();
                emit browseHistory(text, command, +1);
            }
            return false;
        case Qt::Key_Down:
            {
                QString text = getText();
                QString command = getInput();
                emit browseHistory(text, command, -1);
            }
            return false;
        case Qt::Key_Backspace:
            // Don't let user to delete prompt :(
            return protectPromptEvent(event);
        case Qt::Key_Tab:
            // Indent command with spaces
            insertPlainText(QString(tabsize, ' '));
            return false;
        case Qt::Key_Return: // Command accepted
            QString cmd = getInput();
            emit commandInvoked(cmd);
            return false; // Block standard action
        }
    }
    // None applies? Try default behaviour
    return QTextEdit::event(event);
}
