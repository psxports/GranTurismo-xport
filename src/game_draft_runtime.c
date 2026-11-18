#include "psx_spu.h"
#include "game_draft_registry.h"
#include "game_startup.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Original GTMAIN SDK storage addresses */
const uint32 xport_cd_sync_callback_address = 0x800B202Cu;
const uint32 xport_cd_ready_callback_address = 0x800B2030u;
const uint32 xport_cd_status_address = 0x800B203Cu;
const uint32 xport_cd_setloc_table_address = 0x800B1F64u;
const uint32 xport_spu_register_pointer_address = 0x800B2344u;

__declspec(noreturn) void gt_draft_failure(const char *boundary, uint32 address)
{
    char message[512];
    FILE *log = NULL;
    sprintf_s(message, sizeof(message), "Native port stopped: %s (0x%08X). This adapter or implementation is missing. See native_failure.txt.", boundary, address);
    OutputDebugStringA(message);
    if (!fopen_s(&log, "native_failure.txt", "w") && log) { fprintf(log, "%s\n", message); fclose(log); }
    MessageBoxA(NULL, message, "Gran Turismo native startup error", MB_OK | MB_ICONERROR);
    ExitProcess(2u);
}

uint32 gt_draft_unresolved_call(uint32 address, uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    gt_draft_failure("external call", address);
}

uint32 gt_draft_unresolved_call9(uint32 address, uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7, uint32 a8)
{
    gt_draft_failure("external call", address);
}

uint32 gt_draft_unresolved_call10(uint32 address, uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7, uint32 a8, uint32 a9)
{
    gt_draft_failure("external call", address);
}

uint32 gt_draft_unresolved_call11(uint32 address, uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7, uint32 a8, uint32 a9, uint32 a10)
{
    gt_draft_failure("external call", address);
}

uint32 gt_draft_unresolved_call12(uint32 address, uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7, uint32 a8, uint32 a9, uint32 a10, uint32 a11)
{
    gt_draft_failure("external call", address);
}

uint32 gt_draft_buffer_address(void *data, uint32 size) { gt_draft_failure("native buffer to guest address", size); }
void xport_bind_native_spu_transfer(void)
{
    SpuNativeTransferGuestBinding binding = {
        0x800B23B4u, 0x800B2360u, 0x800B235Cu, 0x800B2378u,
        0x800B237Cu, 0x800B2398u, 0x800B239Cu, r_u32(0x800B236Cu)
    };
    if (!spu_bind_native_transfer_guest(&binding))
        gt_draft_failure("native SPU transfer binding", 0x80085C88u);
}
void gt_draft_buffer_release(uint32 address) { gt_draft_failure("native buffer release", address); }
uint32 gt_draft_gte_read(uint32 reg, uint32 control)
{
    if (!control && (reg == 30u || reg == 31u)) return xport_gte_read_data(reg);
    gt_draft_failure("GTE read adapter", reg | (control << 8));
}
void gt_draft_gte_write(uint32 reg, uint32 control, uint32 value)
{
    if (!control && (reg == 30u || reg == 31u)) { xport_gte_write_data(reg, value); return; }
    gt_draft_failure("GTE write adapter", reg | (control << 8));
}
void gt_draft_gte_command(uint32 command) { gt_draft_failure("GTE command adapter", command); }
__declspec(noreturn) void gt_draft_exception(uint32 code) { gt_draft_failure("guest exception adapter", code); }
uint32 gt_draft_incoming_register(uint32 function_address, uint32 register_index) { gt_draft_failure("nonstandard incoming register adapter", function_address); }
void gt_draft_loader_context_capture(uint32 address) { gt_draft_failure("loader context capture adapter", address); }
void gtmain_load_bios_kernel(void)
{
    uint32 descriptor;
    FILE *image = NULL;
    size_t received;
    int extra, failed;
    if (fopen_s(&image, "DATA/GTMAIN.bios-kernel.bin", "rb") || !image)
        gt_draft_failure("SCPH-5502 kernel image open", 0x500u);
    /* ROM boot copies this region before clearing kernel BSS */
    received = fread(psx_addr(0x500u, 0x8BF0u), 1u, 0x8BF0u, image);
    extra = fgetc(image);
    failed = ferror(image);
    fclose(image);
    if (received != 0x8BF0u || extra != EOF || failed)
        gt_draft_failure("SCPH-5502 kernel image read", (uint32)received);
    memset(psx_addr(0x7460u, 0x8920u - 0x7460u), 0, 0x8920u - 0x7460u);
    /* ROM boot allocates four interrupt queue descriptors */
    psx_bios_init_kernel_heap(0xA000E000u, 0x2000u);
    descriptor = psx_bios_kernel_malloc(32u);
    if (!descriptor || descriptor == ~0u)
        gt_draft_failure("BIOS interrupt queue allocation", descriptor);
    memset(psx_addr(descriptor, 32u), 0, 32u);
    w_u32(0xA0000100u, descriptor);
    w_u32(0xA0000104u, 32u);
    /* TODO Restore the remaining BIOS boot initialization through native APIs */
}

void gt_native_boot_snapshot_seed(void);

void gt_native_spu_init(void)
{
    static uint32 boot_seeded;
    static uint8 preserved[0x80000u];
    sint32 available = spu_download(0u, preserved, sizeof(preserved));
    SpuInit();
    /* SDK initialization preserves sample RAM across executable transitions */
    if (available && !spu_upload(0u, preserved, sizeof(preserved)))
        gt_draft_failure("SPU RAM initialization restore", 0u);
    if (!boot_seeded) {
        gt_native_boot_snapshot_seed();
        boot_seeded = 1u;
    }
}

void gtmain_load_image(void)
{
    const size_t size = 0xA4000u;
    FILE *image = NULL;
    uint8 *payload;
    size_t received;
    int extra;
    int failed;
    if (fopen_s(&image, "DATA/GTMAIN.payload.bin", "rb") || !image)
        gt_draft_failure("GTMAIN image open", 0x80010000u);
    payload = (uint8 *)malloc(size);
    if (!payload) {
        fclose(image);
        gt_draft_failure("GTMAIN image allocation", (uint32)size);
    }
    received = fread(payload, 1u, size, image);
    extra = fgetc(image);
    failed = ferror(image);
    fclose(image);
    if (received != size || extra != EOF || failed) {
        free(payload);
        gt_draft_failure("GTMAIN image size or read error", (uint32)received);
    }
    /* Verified decoded image loads at its original RAM address */
    memcpy(psx_addr(0x80010000u, size), payload, size);
    free(payload);
}

uint32 gtmain_launch_stack_bound(void)
{
    FILE *input = NULL;
    uint8 bytes[4];
    uint32 bound;
    size_t received;
    int extra;
    int failed;
    if (fopen_s(&input, "DATA/GTMAIN.launch.bin", "rb") || !input)
        gt_draft_failure("GTMAIN launch heap input open", 0x8008F1D8u);
    received = fread(bytes, 1u, sizeof(bytes), input);
    extra = fgetc(input);
    failed = ferror(input);
    fclose(input);
    if (received != sizeof(bytes) || extra != EOF || failed)
        gt_draft_failure("GTMAIN launch heap input read", 0x8008F1D8u);
    bound = bytes[0] | ((uint32)bytes[1] << 8) | ((uint32)bytes[2] << 16) | ((uint32)bytes[3] << 24);
    if (bound < 0x80010000u || bound >= 0x80200000u || (bound & 3u))
        gt_draft_failure("GTMAIN launch heap input range", bound);
    return bound;
}

GTMainStartupState *gtmain_loader_startup_state(void)
{
    gtmain_load_image();
    /* TODO Recover the parent loader contract without fabricating register state */
    gt_draft_failure("GTMAIN parent loader context", 0x80010000u);
}
void gtmain_loader_return(GTMainStartupState *state) { gt_draft_failure("GTMAIN loader return adapter", 0x8001004Cu); }

/* Existing link-only dependencies retain fail-fast behavior */
uint32 sub_8007BC84(void);
uint32 sub_80019EC0(uint32 argument0, uint32 argument1);
uint32 sub_80082928(uint32 result);
void gtmain_8007BC84(GTMainStartupState *state) { state->result = sub_8007BC84(); }
void gtmain_80019EC0(GTMainStartupState *state) { state->result = sub_80019EC0(state->argument0, state->argument1); }
void gtmain_80082928(GTMainStartupState *state) { state->result = sub_80082928(state->argument0); }
