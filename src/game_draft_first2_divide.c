#include "game_draft_registry.h"
#include "game_first2_registry.h"
#include <stdio.h>

/* GTMAIN.EXE:80082A88 */
uint64 gtmain_negate64(uint64 value)
{
    return (uint64)0 - value;
}

/* GTMAIN.EXE:80082AA8 */
uint64 gtmain_unsigned_divide64(uint64 numerator, uint64 denominator, uint64 *remainder)
{
    uint64 quotient;
    if (!denominator) {
        gt_draft_exception(7u);
        return 0;
    }
    /* The original normalized unsigned division produces the quotient and optional remainder */
    quotient = numerator / denominator;
    if (remainder) *remainder = numerator % denominator;
    return quotient;
}

/* GTMAIN.EXE:8001E4A8 */
sint64 gtmain_signed_divide64(sint64 numerator, sint64 denominator, uint32 caller_link)
{
    uint64 first = (uint64)numerator, second = (uint64)denominator, quotient;
    uint32 negative = 0u;
    if (!second) {
        printf("Division by Zero, called by %08X\n", caller_link);
        return 0;
    }
    if (numerator < 0) { first = gtmain_negate64(first); negative ^= 1u; }
    if (denominator < 0) { second = gtmain_negate64(second); negative ^= 1u; }
    quotient = gtmain_unsigned_divide64(first, second, NULL);
    if (negative) quotient = gtmain_negate64(quotient);
    /* Preserve the original wrapping result for INT64_MIN divided by minus one */
    return (sint64)quotient;
}
