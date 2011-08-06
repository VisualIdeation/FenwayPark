#ifndef TYPES_H_
#define TYPES_H_

typedef signed char Int8;
typedef unsigned char Uint8;
typedef short Int16;
typedef unsigned short Uint16;
typedef int Int32;
typedef unsigned int Uint32;
typedef long Int64;
typedef unsigned long Uint64;

/* HASH Functions */

/*
 * Uint64Hash - Nice little helper class for hashing a Uint64.
 */
struct Uint64Hash {
	union Uint64_val {
		Uint64 full;
		struct {
			Uint32 low32;
			Uint32 high32;
		} nested;
	};

	Uint32 operator()(Uint64 val) const {
		Uint64_val temp;
		temp.full = val;
		return temp.nested.low32 + temp.nested.high32;
	}
};

#endif   /* TYPES_H_ */
