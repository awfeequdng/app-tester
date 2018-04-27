#ifndef SQLIMPORT_H
#define SQLIMPORT_H

#include <QObject>

class SqlImport : public QObject
{
    Q_OBJECT
public:
    explicit SqlImport(QObject *parent = 0);

signals:

public slots:
};

#endif // SQLIMPORT_H
