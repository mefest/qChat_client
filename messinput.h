#ifndef MESSINPUT_H
#define MESSINPUT_H

#include <QTextEdit>

class messInput : public QTextEdit
{
    Q_OBJECT
public:
    explicit messInput(QWidget *parent = 0);

signals:
    void giveText(QString text);
public slots:

protected:

  virtual void 	keyPressEvent ( QKeyEvent * event );

};

#endif // MESSINPUT_H
