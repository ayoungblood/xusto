/* test/minunit.h
 * From http://www.jera.com/techinfo/jtns/jtn002.html, slightly modified
 * Freely available with no warranty
 * Not quite the simplest unit test framework
 */
#include <stdio.h>
#define STRINGIZE_DETAIL(x) #x
#define STRINGIZE(x) STRINGIZE_DETAIL(x)
#define _FL __FILE__ ":" STRINGIZE(__LINE__) ": "
#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { const char *message = test(); tests_run++; \
                            if (message) return message; } while (0)
extern int tests_run;
