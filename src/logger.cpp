/*---------------------------------------------------------------------------*
 * logger.cpp -
 * Written By: Nicholas DiPasquale
 * Copyright (c) 2005-2009 N.I.E.Software
 *---------------------------------------------------------------------------*/

#include "common.h"
#include <iostream>

/*---------------------------------------------------------------------------*/

namespace niesoft {

/*---------------------------------------------------------------------------*/

Logger::Level Logger::s_level = Logger::OFF;

/*---------------------------------------------------------------------------*/

void Logger::out (const Logger::Level level, const String &message) {
	Date date (std::time (NULL));
	cout << "[" << level << "] " << date << ": " << message << '\n';
}

/*---------------------------------------------------------------------------*/

void Logger::info (const String &message) {
#ifdef DEBUG
	if (s_level >= INFO) {
		out (INFO, message);
	}
#endif
}

/*---------------------------------------------------------------------------*/

void Logger::error (const String &message) {
#ifdef DEBUG
	if (s_level >= ERROR) {
		out (ERROR, message);
	}
#endif
}

/*---------------------------------------------------------------------------*/

void Logger::warn (const String &message) {
#ifdef DEBUG
	if (s_level >= WARN) {
		out (WARN, message);
	}
#endif
}

/*---------------------------------------------------------------------------*/

void Logger::debug (const String &message) {
#ifdef DEBUG
	// if (s_level >= DEBUG) {
	if (s_level >= TRACE) {
		// out (DEBUG, message);
		out (TRACE, message);
	}
#endif
}

/*---------------------------------------------------------------------------*/

void Logger::trace (const String &message) {
#ifdef DEBUG
	if (s_level >= TRACE) {
		out (TRACE, message);
	}
#endif
}

/*---------------------------------------------------------------------------*/
}; // namespace niesoft;
