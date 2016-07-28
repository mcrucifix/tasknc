/*
Copyright (c) 2000                      RIPE NCC


All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the name of the author not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS; IN NO EVENT SHALL
AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

/*
-------------------------------------------------------------------------------
Module Header
Filename          : iso8601.C
Author            : Rene Wilhelm
Date              : 13-OCT-2000
Description       : Functions for decoding ISO8601 date/period strings
Language Version  : C++
OSs Tested        : Solaris 2.6
$Id: iso8601.C,v 1.3 2000/12/08 14:39:18 wilhelm Exp $
-------------------------------------------------------------------------------
*/
/*
Modified by M. Crucifix on 2016/07/29 for inclusion in tasknc. 
Added partial support for digited numbers and included a passive 'T' marker
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// #include "iso8601.h"


/*
 *  Subroutine header
 *
 *  Error handler
 *
 *  Input: 	  none
 *  Output: 	  error message
 *  Side effects: terminate program
 */

void invaliddate(char* flag) {*flag = 'I'; }

/*
 *  Subroutine header
 *
 *  decode ascii ISO8601 time period string into an ending time in time_t
 *  format (seconds since 1970) and a duration
 *
 *  Input:   char   *datestring  string to be decoded
 *  Output:  time_t& endtime     system timestamp of end time
 *           time_t& duration    duration in seconds
 *
 */

/*
 *  Subroutine header
 *
 *  decode ascii time string into duration of time or fixed point in time
 *  the string should be in the formats described by ISO8601 standard
 *
 *  note that to not complicate the routine too much, points in time
 *  MUST be complete representations of either a date (CCYYMMDD) or a
 *  date+time (CCYYMMDDTHHMMSS). - and : are allowed as separators.
 *
 *  similarly, duration are only supported when specified in days,
 *  hours, minutes and seconds. Lenght of months and years are
 *  content dependant.
 *  
 *
 *  Input:   char*   text  string to be decoded
 *  Output:  time_t& time  system timestamp or duration  (seconds)
 *           char&   flag  indicates if returned time is timestamp or duration
 */

#define DAYFLAG    0x001000
#define DHMSFLAG   0x001111
#define HOURFLAG   0x000100
#define HMSFLAG    0x000111
#define MINFLAG    0x000010
#define MSFLAG     0x000011
#define SECFLAG    0x000001

void parseISO8601 (char *text, time_t *isotime, char *flag) {
	char *c;
	int num;

	struct tm  tmstruct;
 
	int year     = 0;
	int month    = 0;
	int seconds  = 0;
	int minutes  = 0;
	int hours    = 0;
	int days     = 0;

	int dateflags = 0;   /* flag which date component we've seen */

	c = text;
	*isotime = 0;

	if (*c++ == 'P') {
		/* duration */
		*flag = 'D';
		while (*c != '\0') {
			num = 0;
			while (*c >= '0' && *c <= '9') {
				/* assumes ASCII sequence! */
				num = 10 * num + *c++ - '0';
			}
       /* ignores number- after digit */
		  if (*c == '.') { c++; while (*c >= '0' && *c <= '9') {c++;}  }

			switch (*c++) {
			case 'D':
				if (dateflags & DHMSFLAG) {
					/* day, hour, min or sec already set */
					invaliddate(flag);
				}
				else {	
					dateflags |= DAYFLAG;
					days = num;
				}
				break;
      case 'T':
				if (dateflags & HMSFLAG) {
					/* hour, min or sec already set */
					invaliddate(flag);
				}
        break;
			case 'H':
				if (dateflags & HMSFLAG) {
					/* hour, min or sec already set */
					invaliddate(flag);
				}
				else {	
					dateflags |= HOURFLAG;
					hours = num;
				}
				break;
			case 'M':
			  	if (dateflags & MSFLAG) {
					/* min or sec already set */
					invaliddate(flag);
				}
				else {
					dateflags |= MINFLAG;
					minutes = num;
			  	}
				break;
			case 'S':
				if (dateflags & SECFLAG) {
					/* sec already set */
					invaliddate(flag);
				}
				else {	
					dateflags |= SECFLAG;
					seconds = num;
				}
				break;
			default:
				invaliddate(flag);
			}
		}
		*isotime = seconds + 60 * minutes + 3600 * hours +
			   86400 * days;
	}
	else {
		/* point in time, must be one of 
			CCYYMMDD
			CCYY-MM-DD
			CCYYMMDDTHHMM
			CCYY-MM-DDTHH:MM
			CCYYMMDDTHHMMSS
			CCYY-MM-DDTHH:MM:SS
		*/
		c = text;
		* flag = 'T';

		/* NOTE: we have to check for the extended format first, 
		   because otherwise the separting '-' will be interpreted
		   by sscanf as signs of a 1 digit integer .... :-(  */

		if (sscanf(text, "%4u-%2u-%2u", &year, &month, &days) == 3) {
			c += 10;
		}
		else if (sscanf(text, "%4u%2u%2u", &year, &month, &days) == 3) {
			c += 8;
		}
		else {
			invaliddate(flag);
		}

		tmstruct.tm_year = year - 1900;
		tmstruct.tm_mon  = month - 1;
		tmstruct.tm_mday = days;

		if (*c == '\0') {
			tmstruct.tm_hour = 0;
			tmstruct.tm_sec = 0;
			tmstruct.tm_min = 0;
			*isotime = mktime (&tmstruct);
		}
		else if (*c == 'T') {
			/* time of day part */
			c++;
			if (sscanf(c, "%2d%2d", &hours, &minutes)  == 2) {
				c += 4;
			}
			else if (sscanf(c, "%2d:%2d", &hours, &minutes) == 2) {
				c += 5;
			}
			else {
				invaliddate(flag);
			}

			if (*c == ':') {
				c++;
			}

			if (*c != '\0') {
			   	if (sscanf(c, "%2d", &seconds)  == 1) {
					c += 2;
			   	}
			   	else {
					invaliddate(flag);
			   	}
				if (*c != '\0') {      /* something left? */
					invaliddate(flag);
				}
			}
			tmstruct.tm_hour = hours;
			tmstruct.tm_min = minutes;
			tmstruct.tm_sec = seconds;
			*isotime = mktime (&tmstruct);
		}

		else {
			invaliddate(flag);
		}
	}

}
