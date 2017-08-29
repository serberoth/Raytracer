/*---------------------------------------------------------------------------*
 * date.h -
 * Written By: Nicholas DiPasquale
 * Copyright (c) 2005-2009 N.I.E.Software
 *---------------------------------------------------------------------------*/

#pragma once
#ifndef __DATE_H__
#define __DATE_H__

/*---------------------------------------------------------------------------*/

using namespace std;

namespace niesoft {

/*---------------------------------------------------------------------------*/

class Date : virtual public gc_cleanup {
private:
	time_t _time;
	struct {
		int _year;
		int _month;
		int _day;
		int _hr;
		int _min;
		int _sec;
	};

	/*
	const stirng FULL_MONTHS[] = { "January", "February", "March", "April",
			"May", "June", "July", "August", "September", "October", "November",
			"December", };
	const string SHORT_MONTHS[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
			"Jul", "Aug", "Sep", "Oct", "Nov", "Dec", };
	 */

	inline int calcMonth (int dayOfYear, int year) {
		int (*m)(int) = isLeapYear (year) ? dayInMonthLeap : dayInMonth;
		int accum = 0;
		for (int i = 0; i < 12; ++i) {
			accum += m(i);
			if (dayOfYear < accum) {
				return i;
			}
		}
		return -1;
	}

	inline int calcDayOfMonth (int dayOfYear, int year) {
		int (*m)(int) = isLeapYear (year) ? dayInMonthLeap : dayInMonth;
		for (int i = 0, accum = 0; 12; ++i) {
			accum += m (i);
			if (dayOfYear < accum) {
				return dayOfYear - (accum - m (i)) + 1;
			}
		}
		return -1;
	}

	inline static int dayInMonth (int month) {
		const int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, };
		return daysInMonth[month];
	}
	inline static int dayInMonthLeap (int month) {
		const int daysInMonth[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, };
		return daysInMonth[month];
	}

	inline bool isLeapYear (int year) {
		return ((year % 4) == 0) && ((year % 100) != 0) && ((year % 400) == 0);
	}

public:
	explicit Date (time_t time = std::time (NULL)) : _time (time) {
		// time returns the time since 0:0:0,0 1 Jan 1970
		_year = (_time / 31556926) + 1970;
		// This is day in year
		int dayOfYear = (_time / 86400) % (isLeapYear (_year) ? 366 : 365);
		dayOfYear -= (_year - 1972) / 4 + 1;
		_month = calcMonth (dayOfYear, _year);
		_day = calcDayOfMonth (dayOfYear, _year);
		_hr = (_time / 3600) % 24;
		_min = (_time / 60) % 60;
		_sec = _time % 60;
	}
	~Date (void) { }

	inline time_t time (void) const {
		return _time;
	}

	inline operator time_t () const {
		return _time;
	}

	inline int year (void) const {
		return _year;
	}

	inline int month (void) const {
		return _month;
	}

	inline int day (void) const {
		return _day;
	}

	inline int hr (void) const {
		return _hr;
	}

	inline int min (void) const {
		return _min;
	}

	inline int sec (void) const {
		return _sec;
	}

	inline friend std::ostream &operator << (std::ostream &out, const Date &date) {
		return out << setfill ('0') << setw (4) << date._year << "-" << setw (2) << date._month << "-" << setw (2) << date._day << " "
			<< setw (2) << date._hr << ":" << setw (2) << date._min << ":" << setw (2) << date._sec << " (UTC)" << setfill (' ');
	}

};

/*---------------------------------------------------------------------------*/

}; // namespace niesoft;

/*---------------------------------------------------------------------------*/
#endif // __DATE_H__
