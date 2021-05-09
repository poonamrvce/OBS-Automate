#ifndef __LOGGING_HPP__
#define __LOGGING_HPP__
#include<sstream>
#include<iostream>

#define COLOR_SET(color)          "\x1b["<<std::to_string(static_cast<int>(color))<<"m"
#define COLOR_RESET   "\x1b[0m"

enum LogSeverity{
    INFO,
    ERROR,
    DEBUG
};
enum class LogColor {
	Default = 39,
	Black = 30,
	Red,
	Green,
	Yellow,
	Blue,
	Magenta,
	Cyan,
	LightGray,
	DarkGray = 90,
	LightRed,
	LightGreen,
	LightYellow,
	LightBlue,
	LightMagenta,
	LightCyan,
	White
};


#define LOG(severity, ...) \
    switch(severity){\
                case INFO:std::cout << COLOR_SET(LogColor::Green)<<"[INFO]";\
                break;\
                case ERROR:std::cout << COLOR_SET(LogColor::Red)<<"[ERROR]";\
                break;\
                case DEBUG:std::cout << COLOR_SET(LogColor::LightMagenta)<<"[DEBUG]";\
                break;\
            }\
            std::cout << ' ' << __FILE__ << '('  << __LINE__ << ')' << __FUNCTION__ << ':' <<COLOR_RESET

#endif //__LOGGING_HPP__