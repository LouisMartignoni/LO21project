#ifndef COMPUTEREXCEPTION_H
#define COMPUTEREXCEPTION_H

#include <QString>

class ComputerException {
    QString info;
    QString remaining;
  public:
    ComputerException(const QString& str1, const QString& str2="") : info(str1), remaining(str2){}
    QString getInfo() const;
    QString getRemaining() const;
};

#endif // COMPUTEREXCEPTION_H
