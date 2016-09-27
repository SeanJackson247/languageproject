#ifndef UTILITY_H
#define UTILITY_H

#include<QString>

int operatorCode(QString data);
bool isOperator(QString data);
bool isHigherPrecedenceThan(QString a,QString b);
bool isLowerPrecedenceThan(QString a,QString b);

#endif // UTILITY_H
