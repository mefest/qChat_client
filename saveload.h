#ifndef SAVELOAD_H
#define SAVELOAD_H

#include <QObject>

class QColor;

class saveLoad : public QObject
{
    Q_OBJECT
public:
    explicit saveLoad(QObject *parent = 0);

    QColor *_my;
    QColor *_other;
    QFont *_font;

    void save(QColor* my, QColor* other , QFont *font);
    void load(QColor* my, QColor* other , QFont *font);
    void saveS(QString name,QString server,QString port, QString key);
    void loadS(QString *name,QString *server,QString *port, QString *key);
    QColor* getMy() {return _my;}

signals:

public slots:

};

#endif // SAVELOAD_H
