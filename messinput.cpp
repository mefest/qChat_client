#include "messinput.h"
#include <QKeyEvent>
#include <QDebug>

messInput::messInput(QWidget *parent) :
    QTextEdit(parent)
{
}

void messInput::keyPressEvent(QKeyEvent *event)
{

    if ( event->key() == Qt::Key_Return|| event->key() == Qt::Key_Enter)
    {
        qDebug()<<"Hажата ентер";
        if((event->modifiers() & Qt::SHIFT))
        {
            this->insertPlainText("\n");
        }
        else
        {
            emit giveText(toPlainText());
        this->clear();

        }
    }
    else QTextEdit::keyPressEvent(event);

}

