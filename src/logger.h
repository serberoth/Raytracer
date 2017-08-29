/*---------------------------------------------------------------------------*
 * logger.h -
 * Written By: Nicholas DiPasquale
 * Copyright (c) 2005-2009 N.I.E.Software
 *---------------------------------------------------------------------------*/

#pragma once
#ifndef __LOGGER_H__
#define __LOGGER_H__

/*---------------------------------------------------------------------------*/

namespace niesoft {

/*---------------------------------------------------------------------------*/

class Logger : virtual public gc_cleanup {
public:
	typedef enum { OFF, INFO, ERROR, WARN, TRACE, } Level;

private:
	static Level s_level;

	static void out (const Level level, const String &message);

public:
	inline static void setLevel (const Level level) {
		s_level = level;
	}
	inline static Level getLevel () {
		return s_level;
	}

	static void info (const String &message);
	static void error (const String &message);
	static void warn (const String &message);
	static void debug (const String &message);
	static void trace (const String &message);

	inline friend std::ostream &operator << (std::ostream &out, const Logger::Level &level) {
		switch (level) {
		case INFO:	return out << " INFO";
		case ERROR:	return out << "ERROR";
		case WARN:	return out << " WARN";
		// case DEBUG:	return out << "DEBUG";
		case TRACE:	return out << "TRACE";
		case OFF: break;
		}
		return out;
	}

};

/*---------------------------------------------------------------------------*/

}; // namespace niesoft;

/*---------------------------------------------------------------------------*/
#endif // __LOGGER_H__
