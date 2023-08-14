#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include "qsocket.h"
#include "loginmodel.h"
#include "admincon.h"
#include "friendcon.h"
#include "groupcon.h"
#include "departmodel.h"
#include "departcon.h"

#include <QObject>

class DataProcessor : public QObject
{
    Q_OBJECT
public:
    explicit DataProcessor(QObject *parent = nullptr);

public slots:
    void processData(const QByteArray&data);
};

#endif // DATAPROCESSOR_H
