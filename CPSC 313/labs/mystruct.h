#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* Here is your personalized structure! */
struct mystruct {
	int8_t f1;
	uint64_t f2;
	uint16_t f3;
};

#define	FMT1	"%c\n"
#define	FMT2	"0x %"PRIx64"\n"
#define	FMT3	"0x %"PRIx16"\n"

