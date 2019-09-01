#pragma once

#include <ostream>


namespace srf
{
namespace system
{

class LogTraits
{

};

LogTraits stamp;
LogTraits endl;


enum class LogLevel
{
    D, // Debug
    I, // Info
    W, // Warning
    E  // Error
};

template <LogLevel _level>
class LogStream
{


    template <typename T>
    LogStream<_level>& operator << (T str) { }

    template <>
    LogStream<_level>& operator << (LogTraits traits) { }

};


#ifndef _DEBUG

template <>
class LogStream<LogLevel::D>
{
    // do nothing for everything
    template <typename T>
    LogStream<LogLevel::D>& operator << (T str) { }
};

#endif

class LogSystem
{
public:
    LogSystem();
    ~LogSystem();

    std::ostream stream();
    void setStream(std::ostream stream);

    /// logging as level
#ifdef _DEBUG
    inline void D(const char* str, ...);
#else
    // disabled on release
    inline void D(const char* str, ...) { }
#endif
    inline void I(const char* str, ...);
    inline void W(const char* str, ...);
    inline void E(const char* str, ...);


    /// logging as generalized functions
    template <typename T>
    inline void write(LogLevel level, T str) { }

    template <typename T>
    inline void writeline(LogLevel level, T str) { }

    /// logging as stream
    template <LogLevel _level>
    LogStream<_level> out();

#ifndef _DEBUG
    // disabled on release
    template <>
    LogStream<LogLevel::D> out() { return LogStream<LogLevel::D>(); }

#endif

private:

    void _internal_write();

    std::ostream _out_stream;

};



}
}