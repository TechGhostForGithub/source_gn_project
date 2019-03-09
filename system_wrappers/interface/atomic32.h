// Atomic, system independent 32-bit integer.  Unless you know what you're
// doing, use locks instead! :-)
//
// Note: assumes 32-bit (or higher) system
#ifndef GN_SYSTEM_WRAPPERS_INTERFACE_ATOMIC32_H_
#define GN_SYSTEM_WRAPPERS_INTERFACE_ATOMIC32_H_

#include <stddef.h>

#include "common_types.h"
#include "system_wrappers/interface/constructor_magic.h"

namespace gn
{

	// 32 bit atomic variable.  Note that this class relies on the compiler to
	// align the 32 bit value correctly (on a 32 bit boundary), so as long as you're
	// not doing things like reinterpret_cast over some custom allocated memory
	// without being careful with alignment, you should be fine.
	class Atomic32
	{
	public:
		Atomic32(int32_t initial_value = 0);
		~Atomic32();

		// Prefix operator!
		int32_t operator++();
		int32_t operator--();

		int32_t operator+=(int32_t value);
		int32_t operator-=(int32_t value);

		// Sets the value atomically to new_value if the value equals compare value.
		// The function returns true if the exchange happened.
		bool CompareExchange(int32_t new_value, int32_t compare_value);
		int32_t Value()
		{
			return *this += 0;
		}

	private:
		// Disable the + and - operator since it's unclear what these operations
		// should do.
		Atomic32 operator+(const Atomic32& other);
		Atomic32 operator-(const Atomic32& other);

		// Checks if |_value| is 32bit aligned.
		inline bool Is32bitAligned() const
		{
			return (reinterpret_cast<ptrdiff_t>(&value_) & 3) == 0;
		}

		DISALLOW_COPY_AND_ASSIGN(Atomic32);

		int32_t value_;
	};

}  // namespace gn

#endif  // GN_SYSTEM_WRAPPERS_INTERFACE_ATOMIC32_H_
