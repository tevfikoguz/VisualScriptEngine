#ifndef DEBUG_HPP
#define DEBUG_HPP

#ifdef DEBUG
#define DBGBREAK() DbgBreak()
#define DBGASSERT(condition) DbgAssert(condition)
#define DBGERROR(condition) DbgError(condition)
#define DBGVERIFY(condition) DbgVerify(condition)
#else
#define DBGBREAK()
#define DBGASSERT(condition)
#define DBGERROR(condition) (condition)
#define DBGVERIFY(condition) (condition)
#endif

void DbgBreak ();
void DbgAssert (bool condition);
bool DbgError (bool condition);
bool DbgVerify (bool condition);

void EnableLeakDetection ();

#endif