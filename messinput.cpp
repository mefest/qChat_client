#include "messinput.h"
#include <QKeyEvent>
#include <QDebug>
#include <QMimeData>


messInput::messInput(QWidget *parent) :
    QTextEdit(parent)
{
    //connect(this,SIGNAL(textChanged()),this,SLOT(paste()));
}

void messInput::paste()
{
//    qDebug()<<"paste";
//    QString text=toPlainText();
//    if(text.length()<256)
//        setText(text);
//    else setText(text.mid(0,255));
}

void messInput::keyPressEvent(QKeyEvent *event)
{

    if ( event->key() == Qt::Key_Return|| event->key() == Qt::Key_Enter)
    {
        qDebug()<<"Hажата ентер";
        this->setStyleSheet("");
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
    else
    {
        if(toPlainText().length()>256)
        {
            if(event->key() == Qt::Key_Backspace||event->key() == Qt::Key_Delete)
                QTextEdit::keyPressEvent(event);
            else
                this->setStyleSheet("background-color: red;");
        }
        else
        {
            this->setStyleSheet("");
            QTextEdit::keyPressEvent(event);
        }
    }

}

bool messInput::canInsertFromMimeData(const QMimeData *source) const
{
    qDebug()<<source->text().length();
    if(source->text().length()>256)
        return false;
    else return true;
}

void messInput::insertFromMimeData(const QMimeData *source)
{
    if(source->hasText())
        if(source->text().length()>256)
            insertPlainText(source->text().mid(0,255));
    else insertPlainText(source->text());
}

