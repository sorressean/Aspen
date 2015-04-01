/** @file expectations.h
 *  @brief Contains definitions for all of the expectations
 */

#ifndef _EXPECTATIONS_H
#define _EXPECTATIONS_H
#include "frisk_config.h"

#define REPEAT(n) \
  for (unsigned int _temp_frisk_counter_i = 0; _temp_frisk_counter_i < (n); _temp_frisk_counter_i++)

#define FRISK_BASE_EXPECTATION(test, result, desc, msg) do { \
	if ((test).getOption(FRISK_OPTION_CONTINUE)) { \
		if ( !(result) ) { (test).addFailure( __FILE__, (msg), (desc), __LINE__); } \
	} else { \
		if ( !(result) ) { (test).addFailure(__FILE__, (msg), (desc), __LINE__); return; } \
	}; } while (0);

#define EXPECT_FORCE_FAIL(test, msg) \
	FRISK_BASE_EXPECTATION(test, false, ("Forced failure."), (msg))

#define EXPECT_IMPLEMENTED(test, msg) \
  FRISK_BASE_EXPECTATION(test, false, (msg), ("Expected test to be implemented, but it isn't."))

// BELOW ARE DEFINITIONS FOR PRIMITIVE EXPECTATIONS
// Primitive expectations should only be used on types that implement
// the following operators:
// > (greater than)
// < (less than)
// >= (greater than or equal to)
// <= (less than or equal to)
// == (equal to)
// != (not equal to)
// (cast to boolean)
// !(cast to boolean)


#define EXPECT_EQUAL(test, a, b, desc) \
	FRISK_BASE_EXPECTATION(test, ((a) == (b)), (desc), ("Expected " #a " == " #b " to be true, but it was false.") )

#define EXPECT_NOTEQUAL(test, a, b, desc) \
	FRISK_BASE_EXPECTATION(test, ((a) != (b)), (desc), ("Expected " #a " != " #b " to be true, but it was false.") )

#define EXPECT_GREATER(test, a, b,desc) \
	FRISK_BASE_EXPECTATION(test, ((a) > (b)), (desc), ("Expected " #a " > " #b " to be true, but it was false.") )

#define EXPECT_LESS(test, a, b,desc) \
	FRISK_BASE_EXPECTATION(test, ((a) < (b)), (desc), ("Expected " #a " < " #b " to be true, but it was false.") )

#define EXPECT_GREATER_OR_EQUAL(test, a, b, desc) \
	FRISK_BASE_EXPECTATION(test, ((a) >= (b)), (desc), ("Expected " #a " >= " #b " to be true, but it was false.") )

#define EXPECT_LESS_OR_EQUAL(test, a, b, desc) \
	FRISK_BASE_EXPECTATION(test, ((a) <= (b)), (desc), ("Expected " #a " <= " #b " to be true, but it was false.") )

#define EXPECT_TRUE(test, a, desc) \
	FRISK_BASE_EXPECTATION(test, (a), (desc), ("Expected " #a " to be true, but it was false.") )

#define EXPECT_FALSE(test, a, desc) \
	FRISK_BASE_EXPECTATION(test, (!(a)), (desc), ("Expected " #a " to be false, but it was true.") )

#define EXPECT_THROW(test, fn, ex, desc) \
  do { \
    bool _frisk_result = false;\
    try { (fn); } catch( (ex) &x ) { _frisk_result = true; } \
    

// BELOW ARE NON-PRIMITIVE EXPECTATIONS
// THEY ONlY WORK ON THE CLASS OF OBJETS FOR WHICH THEY ARE CREATED (please read the documentation).


// Arrays
#define EXPECT_ARRAY_EQUAL(test, a, b, len1, len2, desc) \
	FRISK_BASE_EXPECTATION(test, ((len1) == (len2)), (desc), ("Expected array " #a " to equal array " #b ", but the lengths were not equal.")); \
	for (size_t i = 0; i < (len1); i++) { \
		if ((a)[i] != (b)[i]) { \
			const char *m = "Expected array " #a " to equal array " #b ", but element " #a "[%d] != " #b "[%d].";\
			char buff[512] = {0};\
			sprintf(buff, m, i, i);\
			FRISK_BASE_EXPECTATION(test, (false), (desc), (buff)); \
			break;\
		} \
	}

#define EXPECT_ARRAY_NOTEQUAL(test, a, b, len1, len2, desc) \
	if (len1 != len2) {\
		bool is_equal = true;\
		for (size_t i = 0; i < (len1); i++) { \
			if ((a)[i] != (b)[i]) { \
				is_equal = false;\
				break;\
			} \
		}\
		FRISK_BASE_EXPECTATION(test, !(is_equal), (desc), ("Expected array " #a " to not equal array " #b ", but they were equal.")); \
	}

#define EXPECT_ARRAY_CONTAINS(test, a, len, val, desc) \
  do { \
    bool _frisk_result = false; \
    for (int i = 0; i < len; i++) { \
      if ((a)[i] == (val)) { _frisk_result = true; break; }\
    }\
    FRISK_BASE_EXPECTATION((test), _frisk_result, (desc), ("Expected array " #a " to contain value " #val ", but it didn't."));\
  } while(0);


// Strings/lists that implement "length", "count", or "size" method
#define EXPECT_LENGTH_EQUAL(test, a, b, desc) \
	FRISK_BASE_EXPECTATION(test, ( (a).length() == (b).length() ), (desc), ("Expected length of " #a " to equal length of " #b ", but they were not equal."))

#define EXPET_LENGTH_NOTEQUAL(test, a,b, desc) \
		FRISK_BASE_EXPECTATION(test, ( (a).length() != (b).length() ), (desc), ("Expected length of " #a " to not equal length of " #b ", but they were equal."))

#define EXPECT_COUNT_EQUAL(test, a, b, desc) \
	FRISK_BASE_EXPECTATION(test, ( (a).count() == (b).count() ), (desc), ("Expected count of " #a " to equal count of " #b ", but they were not equal."))

#define EXPET_COUNT_NOTEQUAL(test, a,b, desc) \
	FRISK_BASE_EXPECTATION(test, ( (a).count() != (b).count() ), (desc), ("Expected count of " #a " to not equal count of " #b ", but they were equal."))

#define EXPECT_SIZE_EQUAL(test, a, b, desc) \
	FRISK_BASE_EXPECTATION(test, ( (a).size() == (b).size() ), (desc), ("Expected size of " #a " to equal size of " #b ", but they were not equal."))

#define EXPET_SIZE_NOTEQUAL(test, a,b, desc) \
	FRISK_BASE_EXPECTATION(test, ( (a).size() != (b).size() ), (desc), ("Expected size of " #a " to not equal size of " #b ", but they were equal."))

// c-string length equal
#define EXPECT_CSTRING_LENGTH_EQUAL(test, a, b, desc) \
	FRISK_BASE_EXPECTATION(test, ( (strlen( (a) ) == strlen( (b) )) ), (desc), ("Expected string length of " #a " to equal string length of " #b ", but they were not equal."))

#define EXPECT_CSTRING_LENGTH_NOTEQUAL(test, a, b, desc) \
	FRISK_BASE_EXPECTATION(test, ( (strlen( (a) ) != strlen( (b) )) ), (desc), ("Expected string length of " #a " to not equal string length of " #b ", but they were equal."))

// c-string equality with case-sensitive
#define EXPECT_CSTRING_EQUAL(test, a, b, max, desc) \
  FRISK_BASE_EXPECTATION(test, ( (strncmp((a), (b), (max)) == 0)), (desc), ("Expected string " #a " to equal string " #b ", but they were not equal."))

#define EXPECT_CSTRING_NOTEQUAL(test, a, b, max, desc) \
  FRISK_BASE_EXPECTATION(test, ( (strncmp((a), (b), (max)) != 0)), (desc), ("Expected string " #a " to not equal string " #b ", but they were equal."))



// ------------ std::vector expectations -------------------
#define EXPECT_VECTOR_CONTAINS(test, vec, val, desc) \
  do {\
    bool _frisk_result = false;\
    for (auto it = (test).begin(); it != (test).end(); it++) {\
      if ( (*it) == (val) ) { _frisk_result = true; break; } \
    }\
    FRISK_BASE_EXPECTATION(test, _frisk_result, (desc), ("Expected vector " #vec " to contain element " #val ", but it didn't."));\
  } while (0);


// ----------- std::list expectations ----------------------
#define EXPECT_LIST_CONTAINS(test, vec, val, desc) \
  do {\
    bool _frisk_result = false;\
    for (auto it = (test).begin(); it != (test).end(); it++) {\
      if ( (*it) == (val) ) { _frisk_result = true; break; } \
    }\
    FRISK_BASE_EXPECTATION(test, _frisk_result, (desc), ("Expected vector " #vec " to contain element " #val ", but it didn't."));\
  } while (0);

#endif /* _EXPECTATIONS_H */
