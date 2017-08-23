#ifndef _PTHREAD_WRAPPER_NONCOPYABLE_H_
#define _PTHREAD_WRAPPER_NONCOPYABLE_H_
#define USE_CPP0X

/* NonCopyable is the base class for objects that do not want to
 * be copied. It hides its copy-constructor and its assignment-operator.
 */

#ifdef USE_CPP0X
class NonCopyable {
public:
	NonCopyable() = default;

	NonCopyable(NonCopyable&&) = delete;
	NonCopyable& operator= (NonCopyable&&) = delete;

	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator= (const NonCopyable&) = delete;
};
#else
class NonCopyable {
public:
	NonCopyable() {}
private:
	NonCopyable(const NonCopyable&);
	NonCopyable& operator= (const NonCopyable&);
};
#endif

#endif