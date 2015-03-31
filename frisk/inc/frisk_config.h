/** @file frisk_config.h
 *  @brief Basic functions, macros, and constants used throughout the Frisk framework.
*/
#ifndef _FRISK_CONFIG_H
#define _FRISK_CONFIG_H

/**
 * The 'fix' version of this Frisk release.
 */
#define FRISK_VERSION_FIX		(0x00) // 0

/**
 * The 'minor' version of this Frisk release.
 */
#define FRISK_VERSION_MINOR		(0x01) // 1

/**
* The 'major' version of this Frisk release.
*/
#define FRISK_VERSION_MAJOR		(0x00) // 0

// DO NOT MODIFY BELOW THIS LINE
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
/**
 * Creates a version integer where m=major, n=minor, f=fix
 * Uses the following format (where the number is the number of bits): M6N10F16
 */
#define MAKE_VERSION(m, n, f) ((((unsigned int)(m & 0x3F)) << 26) | (((unsigned int)(n & 0x400)) << 16) | ((unsigned int)(f & 0xffff)))

/**
 * The current Frisk version represented as an unsigned integer, at least 32-bits.
 */
#define FRISK_VERSION (MAKE_VERSION(FRISK_VERSION_MAJOR, FRISK_VERSION_MINOR, FRISK_VERSION_FIX))

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// DO NOT MODIFY ABOVE THIS LINE

/**
 * Returns whether or not x and y are approximately equal, within the given threshold.
 * @param x First argument
 * @param y Second argument
 * @param t Threshold
 * \note
 * This macro yields true if the following expression is true: |x - y| <= t
*/
#define APPROX_EQUAL(x,y,t) (((x) - (y) <= t) || ((y) - (x) <= t))

#define FRISK_STRINGCAT2(a,b) (a ## b)
#define FRISK_STRINGCAT3(a,b,c ) (FRISK_STRINGCAT2(a,b) ## c)
#define FRISK_STRINGCAT4(a,b,c,d) (FRISK_STRINGCAT3(a,b,c) ## d)
#define FRISK_STRINGCAT5(a,b,c,d,e) (FRISK_STRINGCAT4(a,b,c,d) ## e)

#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <list>


#endif /* _FRISK_CONFIG_H */