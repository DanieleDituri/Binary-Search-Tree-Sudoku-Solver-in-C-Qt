#include "myvalidator.h"

MyValidator::MyValidator(QObject *parent)
    : QValidator(parent)
{
}

QValidator::State MyValidator::validate(QString &input, int &pos) const
{
    bool ok;
    int value = input.toInt(&ok);

    if (ok && value >= 1 && value <= 9)
        return QValidator::Acceptable;
    else if (input.isEmpty())
        return QValidator::Intermediate; // Campo vuoto
    else
        return QValidator::Invalid;
}

