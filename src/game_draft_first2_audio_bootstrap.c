#include "game_first2_registry.h"
#include "psx_spu.h"

sint32 gtmain_cd_descriptor_sector(const uint8 *descriptor)
{
    uint32 minute = descriptor[0], second = descriptor[1], frame;
    uint32 seconds = 60u * (10u * (minute >> 4) + (minute & 15u));
    seconds += 10u * (second >> 4) + (second & 15u);
    frame = descriptor[2];
    return (sint32)(seconds * 75u + 10u * (frame >> 4) + (frame & 15u) - 150u);
}
uint32 gtmain_spu_write_transfer(uint32 guest_source, uint32 bytes)
{
    if (r_u32(0x800B2360u)) (void)gtmain_spu_transfer(guest_source, bytes);
    else {
        uint32 address = r_u16(0x800B235Cu);
        uint32 shift = r_u32(0x800B236Cu);
        (void)gtmain_spu_dma_control(2, address << (shift & 31u), 0);
        (void)gtmain_spu_dma_control(1, 0, 0);
        (void)gtmain_spu_dma_control(3, guest_source, bytes);
    }
    return bytes;
}
uint32 gtmain_spu_write(uint32 guest_source, uint32 bytes)
{
    if (bytes > 0x7EFF0u) bytes = 0x7EFF0u;
    (void)gtmain_spu_write_transfer(guest_source, bytes);
    if (!r_u32(0x800B237Cu)) w_u32(0x800B2378u, 0);
    return bytes;
}
sint32 gtmain_audio_snapshot_upload(uint32 guest_snapshot)
{
    uint32 clock = r_u32(0x800C0A88u), prior, high, elapsed, checksum, voice;
    sint64 product = (sint64)(sint32)clock * (sint64)(sint32)0x88888889u;
    prior = r_u32(guest_snapshot + 64u);
    w_u32(guest_snapshot, 0x47546F73u);
    high = (uint32)((uint64)product >> 32);
    elapsed = (uint32)((sint32)(high + clock) >> 5) - (uint32)((sint32)clock >> 31);
    w_u32(guest_snapshot + 64u, elapsed + prior);
    checksum = gtmain_data_checksum(guest_snapshot, 45976u);
    w_u32(guest_snapshot + 45976u, checksum);
    for (voice = 0; voice < 24; ++voice) {
        uint32 address = 0x1F801C00u + voice * 16u;
        if (!spu_mmio_write(NULL, address + 2u, 2, 0, 3))
            gt_draft_failure("Native audio snapshot SPU right volume", address + 2u);
        if (!spu_mmio_write(NULL, address, 2, 0, 3))
            gt_draft_failure("Native audio snapshot SPU left volume", address);
    }
    (void)gtmain_spu_transfer_address(0x2000u);
    (void)gtmain_spu_transfer_mode(0);
    (void)gtmain_spu_write(guest_snapshot, 46016u);
    return SpuIsTransferCompleted(1);
}
sint32 gtmain_music_sector_init(void)
{
    uint8 descriptor[24];
    sint32 result;
    (void)gtmain_spu_control_bit(1);
    result = gtmain_audio_status();
    if (result) {
        do {
            uint32 path = gtmain_music_file();
            result = gtmain_cd_file_query(path, descriptor);
        } while (result);
        result = gtmain_cd_descriptor_sector(descriptor);
        w_u32(0x800994A4u, (uint32)result);
    }
    return result;
}
