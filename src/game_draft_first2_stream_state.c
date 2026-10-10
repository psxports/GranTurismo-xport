#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "psx_spu.h"

static void stream_status_error(uint8 status, uint32 response)
{
    /* TODO Bind the unselected stream error handler */
    (void)status; (void)response;
    gt_draft_failure("GTMAIN alternate stream error handler", 0x8006FB2Cu);
}

/* GTMAIN.EXE 8006FA28 */
sint32 gtmain_stream_stop_request(void)
{
    gtmain_stream_pause();
    w_u8(0x800994C2u, 9u);
    return 9;
}

/* GTMAIN.EXE 8006FA54 */
sint32 gtmain_stream_restart_request(void)
{
    gtmain_stream_pause();
    w_u8(0x800994C2u, 10u);
    return 10;
}

/* GTMAIN.EXE 8006FA80 */
sint32 gtmain_stream_position_update(const uint8 *position)
{
    uint32 sector = (uint32)CdPosToInt((CdlLOC *)position);
    uint32 mask = r_u32(0x800C00A4u), base = r_u32(0x800C00A8u);
    uint32 end = r_u32(0x800C00ACu), next = 16u, completed;
    sector = (sector & ~mask) | (base & mask);
    w_u32(0x800C00A0u, sector);
    if (sector < end) completed = 0u;
    else {
        sint32 restart = (sint16)r_u16(0x800994B4u);
        w_u32(0x800C00A0u, base);
        next = (uint32)(restart ? gtmain_stream_restart_request() : gtmain_stream_stop_request());
        completed = 1u;
    }
    w_u8(0x800994C0u, completed);
    w_u8(0x800994C2u, next);
    return (uint8)next;
}

/* GTMAIN.EXE 8006FE9C */
sint32 gtmain_stream_state_active(uint32 state)
{
    return state == 0u || state - 9u < 10u;
}

/* GTMAIN.EXE 8006FEBC */
void gtmain_stream_begin(void)
{
    uint32 state;
    if (gtmain_cd_pending_command()) return;
    EnterCriticalSectionPSX();
    state = r_u8(0x800994C2u);
    w_u32(0x800C00A0u, r_u32(0x800C00A8u));
    if (gtmain_stream_state_active(state)) {
        if (state) {
            gtmain_cd_stop_status(1u);
            w_u8(0x800994BFu, 1u);
        } else {
            gtmain_stream_callbacks_install();
            gtmain_stream_filter_apply();
            w_u8(0x800994BFu, 0u);
        }
    }
    ExitCriticalSection();
}

/* GTMAIN.EXE 8006FBA8 */
void gtmain_stream_alternate_sync(uint8 status, const uint8 *response)
{
    uint32 response_word, state, request;
    w_u8(0x800994BEu, status);
    response_word = xport_load_le32(response);
    w_u32(0x800C00C0u, response_word);
    if (status != 2u) { stream_status_error(status, response_word); return; }
    state = r_u8(0x800994C2u);
    request = r_u8(0x800994BFu);
    if (state != 14u) {
        w_u8(0x800994BFu, 0u);
        if (request == 1u) {
            if (state == 9u) state = 10u;
            else if (state >= 15u && state < 17u) state = 17u;
        } else if (request == 2u) {
            state = state >= 15u && state < 17u ? 18u : 9u;
        }
    }
    switch (state) {
    case 9:
        state = (uint32)gtmain_stream_callbacks_restore();
        break;
    case 10:
        state = (uint32)gtmain_stream_filter_apply();
        break;
    case 11:
        w_u8(0x800994C8u, r_u8(0x800C00B0u));
        state = 12u;
        gtmain_cd_command(14u, 0x800994C8u);
        break;
    case 12:
        {
            CdlLOC position;
            CdIntToPos((sint32)r_u32(0x800C00A0u), &position);
            state = 13u;
            gtmain_cd_command_native(2u, (uint8 *)&position);
        }
        break;
    case 13:
        gtmain_cd_command(27u, 0u);
        state = 14u;
        break;
    case 14:
        if (response[0] & 32u) { gtmain_cd_command(16u, 0u); state = 15u; }
        else gtmain_cd_command(1u, 0u);
        break;
    case 15:
        state = (uint32)gtmain_stream_position_update(response);
        break;
    case 17:
        state = (uint32)gtmain_stream_restart_request();
        break;
    case 18:
        state = (uint32)gtmain_stream_stop_request();
        break;
    default:
        break;
    }
    w_u8(0x800994C2u, state);
}

/* GTMAIN.EXE 80070CB4 */
uint32 gtmain_vector_normalize_xy(uint8 *out, const uint8 *input)
{
    uint32 x = (uint32)(sint32)(sint16)xport_load_le16(input);
    uint32 y = (uint32)(sint32)(sint16)xport_load_le16(input + 2u);
    uint32 length = 0u;
    if (x || y) {
        uint32 abs_x = (sint32)x < 0 ? 0u - x : x;
        uint32 abs_y = (sint32)y < 0 ? 0u - y : y;
        uint32 extent = abs_x < abs_y ? abs_y : abs_x;
        sint32 numerator, divisor;
        while (extent < 4096u) { extent <<= 1; x <<= 1; y <<= 1; }
        /* TODO The existing square-root service retains its native GTE adapter boundary */
        length = gtmain_fixed_sqrt(x * x + y * y, 0u);
        divisor = (sint32)length;
        numerator = (sint32)(x << 12);
        if (!divisor || (divisor == -1 && (uint32)numerator == 0x80000000u))
            gt_draft_failure("GTMAIN vector normalization division trap", 0x80070D60u);
        x = (uint32)(numerator / divisor);
        numerator = (sint32)(y << 12);
        if (!divisor || (divisor == -1 && (uint32)numerator == 0x80000000u))
            gt_draft_failure("GTMAIN vector normalization division trap", 0x80070D94u);
        y = (uint32)(numerator / divisor);
    }
    xport_store_le16(out, (uint16)x);
    xport_store_le16(out + 2u, (uint16)y);
    return length;
}
