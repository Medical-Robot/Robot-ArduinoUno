#ifndef __STRTOD__H__
#define __STRTOD__H__

/*
 *----------------------------------------------------------------------
 *
 * strtod --
 *
 *	This procedure converts a floating-point number from an ASCII
 *	decimal representation to internal double-precision format.
 *
 * Results:
 *	The return value is the double-precision floating-point
 *	representation of the characters in string.  If endPtr isn't
 *	NULL, then *endPtr is filled in with the address of the
 *	next character after the last one that was part of the
 *	floating-point number.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

/* const char *string: A decimal ASCII floating-point number,
 * optionally preceded by white space.
 * Must have form "-I.FE-X", where I is the
 * integer part of the mantissa, F is the
 * fractional part of the mantissa, and X
 * is the exponent.  Either of the signs
 * may be "+", "-", or omitted.  Either I
 * or F may be omitted, or both.  The decimal
 * point isn't necessary unless F is present.
 * The "E" may actually be an "e".  E and X
 * may both be omitted (but not just one).
 */
/* char **endPtr: If non-NULL, store terminating character's
 * address here.
 */

double strtod_(const char *string, char **endPtr);

#endif