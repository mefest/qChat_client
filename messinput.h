#ifndef MESSINPUT_H
#define MESSINPUT_H

#include <QTextEdit>

class QClipboard;
class messInput : public QTextEdit
{
    Q_OBJECT
public:
    explicit messInput(QWidget *parent = 0);


signals:
    void giveText(QString text);
public slots:
    void paste();
protected:

  virtual void 	keyPressEvent ( QKeyEvent * event );
    virtual bool canInsertFromMimeData(const QMimeData *source) const;
    virtual void insertFromMimeData(const QMimeData *source);

};

#endif // MESSINPUT_H
