#ifndef RESULT_H
#define RESULT_H

#include <QString>

template<typename T>
struct Result
{
    bool ok;
    QString error;
    T data;
};

template<>
struct Result<void>
{
    bool ok;
    QString error;
};

#endif // RESULT_H
