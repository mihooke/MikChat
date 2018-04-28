#ifndef LOGGING_H
#define LOGGING_H

#include <iostream>
#include <ostream>
#include <cstdio>

class Logging
{
public:
    enum Level : unsigned char
    {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
    };

    Logging(const char *fileName, int line)
        :_fileName(fileName), _line(line)
    {
    }
    ~Logging() {}

    static std::ostream &stream() { return _stream; }
    Level level() { return _level; }

    std::ostream &operator <<(int a)
    {
        _stream << _fileName << _line << a;
    }

private:
    static std::ostream &_stream;
    const char *_fileName;
    int _line;
    Level _level;
};

extern Logging::Level g_level;
std::ostream &Logging::_stream = std::cout;

#define LOG_INFO if (g_level <= Logging::Level::INFO) \
    Logging(__FILE__, __LINE__).stream()

#endif // LOGGING_H
