// Copyright 2007-2010 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Rights Reserved.

#ifndef gmsec_util_shared_ptr_h
#define gmsec_util_shared_ptr_h


#include <gmsec/util/Mutex.h>


namespace gmsec
{
namespace util
{

template <class T> class shared_ptr
{
public:

	shared_ptr<T> ()
			: ptr(0), stash(0)
	{
	}

	shared_ptr<T> (T *t)
			: ptr(t), stash(0)
	{
		if (ptr)
			stash = new Stash();
	}

	shared_ptr<T>(const shared_ptr<T> &other)
			:
			ptr(other.ptr),
			stash(0)
	{
		attach(other.stash);
	}

	~shared_ptr<T>()
	{
		release();
	}

	int count()
	{
		if (!stash)
			return 0;
		return stash->count;
	}

	T* get()
	{
		return ptr;
	}

	void reset()
	{
		release();
	}

	void reset(T *t)
	{
		release();
		if (t)
			stash = new Stash();
		ptr = t;
	}

	T* operator-> ()
	{
		return ptr;
	}

	T& operator*()
	{
		return *ptr;
	}

	shared_ptr<T> &operator= (shared_ptr<T> &rhs)
	{
		if (this == &rhs)
			return *this;
		release();
		ptr = rhs.ptr;
		attach(rhs.stash);
		return *this;
	}

	operator bool ()
	{
		return ptr != 0;
	}


private:

	struct Stash
	{
		Stash() : count(1) { }
		volatile int count;
private:
		// not allowed
		Stash(const Stash &);
		Stash &operator= (const Stash &);
	};

	void release()
	{
		if (!stash)
			return;
		T* goner = 0;
		{
			AutoMutex hold(getMutex());
			if (--stash->count == 0)
			{
				goner = ptr;
				ptr = 0;
				delete stash;
			}
			stash = 0;
		}
		if (goner)
			delete goner;
	}

	void attach(Stash *s)
	{
		stash = s;
		AutoMutex hold(getMutex());
		++stash->count;
	}


	Mutex &getMutex()
	{
		static Mutex mutex;
		return mutex;
	}


	T *ptr;
	Stash *stash;
};

} // namespace util
} // namespace gmsec

#endif /*  gmsec_util_shared_ptr_h */
