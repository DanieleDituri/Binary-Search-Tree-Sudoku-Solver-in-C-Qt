#ifndef MYVALIDATOR_H
#define MYVALIDATOR_H

#include <QValidator>

class MyValidator : public QValidator
{
public:
    explicit MyValidator(QObject *parent = nullptr);

    QValidator::State validate(QString &input, int &pos) const override;
};

#endif // MYVALIDATOR_H
