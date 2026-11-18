#ifndef GT_GAME_FIRST2_REGISTRY_H
#define GT_GAME_FIRST2_REGISTRY_H
#include "psx.h"
#include "game_spu_access.h"
#include "game_native_policy.h"
__declspec(noreturn) void gt_draft_failure(const char *boundary, uint32 address);
uint32 gt_draft_unresolved_call(uint32 address, uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7);

uint32 sub_8007BD58(uint32 address, uint32 value, uint32 size);
__declspec(noreturn) void gt_draft_exception(uint32 code);
uint32 gtmain_menu_two_line_caption_draw(uint32 ordering, sint32 x, sint32 y, uint32 first_text, uint32 second_text, sint32 intensity);
void gtmain_sound_voice_tick_if_enabled(void);
uint32 gtmain_race_results_panel_draw(uint32 ordering, uint32 mode, uint32 show_hints);
sint32 gtmain_menu_name_keyboard_panel_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity);
void gtmain_race_time_roster_draw(uint32 ordering, sint32 y, sint32 count);
uint32 gtmain_race_results_commit(void);
uint32 gtmain_menu_result_mode0_run(uint8 flags, sint16 index, uint32 first, uint32 second, uint32 metric, sint16 threshold, uint32 extra, uint8 panel);
void gt_project_disc_toc_bind(void);

uint32 gtmain_sound_tempo(uint32 tempo);
void gtmain_sound_tick_limit(sint16 limit);
uint32 gtmain_sound_reverb_enable(uint32 enabled);
uint32 gtmain_spu_master_volume(sint16 left, sint16 right);
uint32 gtmain_sound_voice_pitch(uint32 owner, sint16 pitch);
uint32 gtmain_sound_voice_volume(uint32 owner, uint32 volume_address);
uint32 gtmain_sound_voice_release(uint32 owner);
uint32 gtmain_sound_voice_detach(uint32 owner);
uint32 gtmain_sound_voice_reverb(uint32 owner, uint8 reverb);
uint32 gtmain_sound_voice_priority(uint32 owner, uint8 priority);
uint32 gtmain_sound_pan(uint32 output, uint32 volume, uint32 pan);
uint32 gtmain_sound_pitch_ratio(uint32 pitch, uint32 reference);
uint32 gtmain_sound_instrument_relocate(uint32 image, uint32 spu_base);
uint32 gtmain_sound_engine_relocate(uint32 image, uint32 spu_base);
uint32 gtmain_sound_engine_channel_init(uint32 channel, uint32 engine, uint32 entry, uint8 priority);
void gtmain_sound_sequence_init(void);
sint32 gtmain_sound_voice_allocate(uint32 priority);
void gtmain_sound_voice_initialize(sint32 index, uint32 instrument);
uint32 gtmain_sound_voice_start(uint32 owner, uint32 instrument);
void gtmain_sound_engine_channel_stop(uint32 channel);
void gtmain_sound_engine_channel_priority(uint32 channel, uint8 priority);
uint32 gtmain_sound_voices_reset(void);
void gtmain_sound_shutdown(void);
uint32 gtmain_sound_sequence_initialize(uint32 sequence, uint32 data, uint32 bank);
sint32 gtmain_sound_ramp_initialize(uint32 ramp, sint32 current, sint32 target, sint32 milliseconds);
sint32 gtmain_sound_ramp_step(uint32 ramp);
sint32 gtmain_sound_sequence_fade(uint32 sequence, uint16 target, sint32 milliseconds);
sint32 gtmain_sound_master_fade(uint16 target, sint32 milliseconds);
uint32 gtmain_sound_sequence_link(uint32 sequence);
uint32 gtmain_sound_sequence_volume_apply(uint32 sequence);
uint32 gtmain_sound_sequence_unlink(uint32 sequence);
uint32 gtmain_sound_sequence_start(uint32 sequence);
void gtmain_sound_sequence_note(uint32 sequence, uint32 channel, sint32 key, sint32 velocity, uint32 deadline);
void gtmain_sound_sequence_events(uint32 sequence);
void gtmain_sound_sequence_voices(void);
void gtmain_sound_sequence_tick(void);
uint32 gtmain_sound_sample_upload(uint32 spu_address, uint32 source, uint32 size, uint32 relocation_result);
uint32 gtmain_sound_engine_pitch(uint32 channel, uint32 slot, uint32 speed, uint32 smooth);
uint32 gtmain_sound_engine_volume(uint32 channel, uint32 slot, sint32 speed, uint32 smooth);
uint32 gtmain_sound_engine_channel_update(uint32 channel);
sint32 gtmain_race_speed_percent(sint32 percent);
sint32 gtmain_race_balance_config(uint32 enabled);
sint32 gtmain_race_balance_boost(uint32 car, sint32 lap_delta, sint32 distance_delta);
sint32 gtmain_race_balance_slow(uint32 car, sint32 lap_delta, sint32 distance_delta);
uint32 gtmain_race_balance_update(uint32 reference_car, uint32 car);
sint32 gtmain_race_speed_apply(uint32 car);
sint32 gtmain_race_position_delta(uint32 first_car, uint32 second_car, sint32 *lap_delta, sint32 *distance_delta);
sint32 gtmain_race_position_compare(uint32 cars, sint32 first, sint32 second);
sint32 gtmain_race_positions_init(uint32 cars, sint32 count);
sint32 gtmain_race_positions_sort(uint32 cars, sint32 count);
sint64 gtmain_signed_divide64(sint64 numerator, sint64 denominator, uint32 caller_link);
uint64 gtmain_negate64(uint64 value);
uint64 gtmain_unsigned_divide64(uint64 numerator, uint64 denominator, uint64 *remainder);
uint32 gtmain_input_reset(uint32 state);
uint32 gtmain_input_packet(uint32 port);
sint32 gtmain_input_decode(uint32 state, uint32 packet);
uint32 gtmain_input_poll(void);
void gtmain_input_buttons_reset(uint32 state);
uint32 gtmain_input_buttons_update(uint32 state, uint32 buttons);
sint32 gtmain_input_button_repeat(uint32 counter, uint32 held);
uint32 gtmain_input_scan(uint32 state);
uint32 gtmain_input_port_update(uint32 state, uint32 port, uint32 slot);
sint32 gtmain_input_connected(uint32 port);
void gtmain_input_stop_when_ready(void);
uint32 gtmain_input_rumble(uint32 slot, uint8 small, uint8 large);
uint32 gtmain_model_translation(uint32 context, uint32 vector_nullable);
uint32 gtmain_fixed_sqrt(uint32 value, uint32 fractional_bits);
uint32 gtmain_vector_length10(const uint8 *vector);
uint8 *gtmain_vector_lerp(uint8 *out, const uint8 *first, const uint8 *second, sint32 fraction);
uint32 gtmain_vector_path_sample(uint32 path, uint32 output);
sint32 gtmain_model_rotation(uint8 *matrix, sint16 x, sint16 y, sint16 z);
sint32 gtmain_angles_lerp(uint8 *out, const uint8 *first, const uint8 *second, sint32 fraction);
uint32 gtmain_model_rotate_vector(uint8 *out, const uint8 *matrix, const uint8 *vector);
uint8 *gtmain_vector_subtract(uint8 *out, const uint8 *first, const uint8 *second);
sint32 gtmain_vector_narrow(uint8 *out, const uint8 *vector);
sint32 gtmain_matrix_multiply(uint8 *out, const uint8 *left, const uint8 *right);
uint8 *gtmain_vector_add(uint8 *out, const uint8 *first, const uint8 *second);
uint32 gtmain_matrix_bind_negated_axes(const uint8 *matrix);
sint32 gtmain_matrix_scale_gte(uint8 *out, const uint8 *matrix, uint16 scale);
void gtmain_matrix_shift(uint8 *out, const uint8 *matrix, sint32 shift);
uint32 gtmain_matrix_transpose(uint8 *out, const uint8 *matrix);
sint32 gtmain_matrix_rotate_short(uint8 *out, const uint8 *matrix, const uint8 *vector);
sint32 gtmain_text_glyph(uint8 glyph);
void gtmain_text_position(sint16 x, sint16 y);
void gtmain_text_advance(sint16 advance);
uint32 gtmain_text_bind(uint32 ordering_link);
uint32 gtmain_text_draw(uint32 ordering_link, sint16 x, sint16 y, uint32 guest_string);
uint32 gtmain_race_signed_text_draw(uint32 guest_text);
sint32 gtmain_text_font(sint32 index);
uint32 gtmain_primitive_texture_page(uint32 context, uint32 ordering_link);
sint32 gtmain_cd_stream_idle(void);
uint32 gtmain_cd_stream_callback(uint32 token);
sint32 gtmain_cd_stream_read(uint32 destination, uint32 sector, uint32 size);
sint32 gtmain_cd_stream_wait(void);
sint32 gtmain_cd_stream_reset(void);
sint32 gtmain_cd_stream_pause(void);
sint32 gtmain_cd_stream_seek(void);
sint32 gtmain_cd_stream_finish(void);
sint32 gtmain_cd_stream_restart(void);
void gtmain_cd_stream_sync(uint8 status, const uint8 *result);
void gtmain_cd_stream_ready(uint8 status, const uint8 *result);
sint32 gtmain_cd_stream_copy_sector(void);
sint32 gtmain_cd_stream_can_read(uint32 state);
sint32 gtmain_cd_stream_start(uint32 sector);
uint32 gtmain_cd_stream_destination(uint32 destination, uint32 size);
sint32 gtmain_cd_command(uint8 command, uint32 parameter);

sint32 gtmain_angle_asin(sint16 value);
sint32 gtmain_angle_atan(sint16 x, sint16 y);
sint32 gtmain_matrix_angles(uint8 *out, const uint8 *matrix);
sint32 gtmain_model_orientation(uint32 context, uint32 reference_nullable);
sint32 gtmain_cd_pending_command(void);
void gtmain_cd_default_sync(uint8 status, const uint8 *result);
void gtmain_cd_init_sync(uint8 status, const uint8 *result);
sint32 gtmain_cd_stop_status(uint32 stop);
uint32 gtmain_cd_sector_end(uint32 sector, uint32 size);
void gtmain_cd_mode(uint8 mode);
void gtmain_cd_filter(uint8 file, uint8 channel);
void gtmain_cd_parameter(uint16 parameter);
void gtmain_cd_buffers(uint32 first, uint32 second, uint32 mode);
sint32 gtmain_cd_stop(void);
uint32 gtmain_bcd_decode(uint32 value);
uint32 gtmain_bcd_encode(uint32 value);
sint32 gtmain_cd_directory_reset(void);
sint32 gtmain_cd_directory_start(void);
sint32 gtmain_cd_directory_tick(void);
uint32 gtmain_cd_directory_find(const uint8 *name, uint32 directory, uint32 buffer);

uint32 gtmain_packed_bytes_entry(uint32 base, uint32 index);
uint32 gtmain_packed_pairs_entry(uint32 base, uint32 index);
uint32 gtmain_resource_name(uint32 index);
uint32 gtmain_resource_pairs(uint32 index);
uint32 gtmain_resource_pairs_count(uint32 index);
uint32 gtmain_resource_records_end(uint32 base, sint32 count);
uint32 gtmain_resource_records_relocate(uint32 base);
uint32 gtmain_resource_section_relocate(uint32 section);
uint32 gtmain_memory_copy(uint32 destination, uint32 source, uint32 size);
uint32 gtmain_string_copy(uint32 destination, uint32 source);
uint32 gtmain_string_append(uint32 destination, uint32 source);
sint32 gtmain_string_compare(uint32 left, uint32 right);
uint32 gtmain_memory_move(uint32 destination, uint32 source, uint32 size);
sint32 gtmain_string_decimal(uint32 source);
uint32 gtmain_string_length(uint32 source);
uint32 gtmain_environment_find(uint32 name, uint32 length);
uint32 gtmain_string_skip_field(uint32 source, sint8 delimiter);
uint32 gtmain_string_copy_field(uint32 source, uint32 destination, sint8 delimiter);
sint32 gtmain_memory_compare(uint32 left, uint32 right, uint32 size);
uint32 gtmain_work_string_copy(uint32 destination, sint32 capacity);
uint32 gtmain_work_string_copy_default(uint32 destination);

sint32 gtmain_render_context_init(uint32 context);
uint32 gtmain_model_sections_relocate(uint32 model);
sint32 gtmain_model_texture_bits(uint32 model, uint32 page);
uint32 gtmain_model_bundle_relocate(uint32 bundle, uint32 page);
uint32 gtmain_matrix_negate_axes(uint8 *out, const uint8 *matrix);
void gtmain_model_distances_square(uint32 entries);
uint32 gtmain_model_materials_relocate(uint32 materials);

sint32 gtmain_race_session_enter(void);
sint32 gtmain_race_session_restore(void);
uint32 gtmain_race_render_config(uint32 mode);
sint32 gtmain_race_session_loop(void);
sint32 gtmain_race_track_load(uint32 track);
sint32 gtmain_race_cars_load(void);
sint32 gtmain_race_car_palettes(uint32 track);
void gtmain_race_cars_invalidate(void);
void gtmain_race_text_bind(uint32 text);
sint32 gtmain_race_arcade_track(uint32 track);
sint32 gtmain_race_track_index(uint32 track);
sint32 gtmain_race_track_find(uint32 names, uint32 name);
sint32 gtmain_race_track_invalidate(void);
sint32 gtmain_race_session_prepare(void);
uint32 gtmain_race_config_reset(void);

uint32 gtmain_environment_value(uint32 name);
uint32 gtmain_environment_field(uint32 name, sint32 index, uint32 destination);
sint32 gtmain_path_separator(sint8 character);
uint32 gtmain_path_resolve(uint32 destination, uint32 source);
uint32 gtmain_string_last_character(uint32 source, uint8 character);
uint32 gtmain_environment_reset(void);
void gtmain_runtime_empty(void);
sint32 gtmain_format_decimal_cursor(uint32 cursor_address);
sint32 gtmain_format_emit(sint8 character);

void gtmain_text_flags(uint8 flags);
uint32 gtmain_text_color(uint32 rgb);
uint32 gtmain_course_entry(uint32 index);
uint32 gtmain_course_name(uint32 index);
uint32 gtmain_matrix_bind_packed_negation(const uint8 *matrix);
uint32 gtmain_lighting_bind(uint32 context);
uint32 gtmain_model_position(uint32 context, sint32 x, sint32 y, sint32 z);
sint32 gtmain_resource_pair_value(uint32 key, uint32 pairs);
sint32 gtmain_car_count(void);
void gtmain_course_car_filter(uint32 enabled);
sint32 gtmain_race_loading_screen(void);
sint32 gtmain_race_loading_tick(void);
sint32 gtmain_race_loading_done(void);
sint32 gtmain_race_loading_draw(void);
sint32 gtmain_race_pad_axes_reset(uint32 state);
sint32 gtmain_race_pad_reset(uint32 state);

typedef struct GTMainFormatArguments {
    const uint32 *words;
    uint32 guest_address;
    uint32 count;
    uint32 index;
} GTMainFormatArguments;

uint32 gtmain_string_copy_bounded(uint32 destination, uint32 source, uint32 capacity);
sint32 gtmain_string_format(uint32 destination, uint32 format, const uint32 *arguments, uint32 argument_count);
sint32 gtmain_string_format_guest(uint32 destination, uint32 format, uint32 argument_address);
sint32 gtmain_format_engine(sint32 (*emit)(sint8), uint32 format, GTMainFormatArguments *arguments);
sint32 gtmain_format_string(sint32 (*emit)(sint8), uint8 flags, sint32 width, sint32 precision, uint32 source);
sint32 gtmain_format_number(sint32 (*emit)(sint8), uint8 flags, sint32 width, sint32 precision, uint8 sign, uint32 digits, sint32 length, sint32 radix);
sint32 gtmain_format_octal(sint32 (*emit)(sint8), uint8 flags, sint32 width, sint32 precision, uint32 low, uint32 high, sint8 sign);
sint32 gtmain_format_hex(sint32 (*emit)(sint8), uint8 flags, sint32 width, sint32 precision, uint32 low, uint32 high, uint32 letter);
sint32 gtmain_format_decimal(sint32 (*emit)(sint8), uint8 flags, sint32 width, sint32 precision, uint32 low, uint32 high, sint8 sign, uint32 decimal_point);

sint32 gtmain_frame_defaults(uint32 context);
uint32 gtmain_packet_draw_area(uint32 ordering_link, uint32 offset, uint32 top_left, uint32 bottom_right);
uint32 gtmain_packet_rectangle(uint32 ordering_link, uint32 corners, uint32 color);
uint32 gtmain_packet_page(uint32 ordering_link, uint16 page);
uint32 gtmain_packet_outline(uint32 ordering_link, uint32 corners, uint32 color);
uint32 gtmain_packet_sprite(uint32 ordering_link, uint32 xy, uint32 uv_clut, uint32 wh, uint32 color);
uint32 gtmain_menu_table_sprite_draw(uint32 ordering, uint32 x, uint32 y, uint32 index, uint32 color);
uint32 gtmain_packet_buffer_select(uint32 index);
uint32 gtmain_packet_buffers(uint32 first, uint32 second, uint32 bytes);
sint32 gtmain_draw_background(uint8 enabled, uint8 red, uint8 green, uint8 blue);

uint32 gtmain_race_frame_reset(uint32 font);
void gtmain_frame_enabled(uint32 enabled);
sint32 gtmain_display_mode_init(uint32 flags);
void gtmain_display_mask(uint32 enabled);

typedef struct GTMainArchiveTable {
    uint8 *bytes;
    uint32 guest_address;
} GTMainArchiveTable;

uint32 gtmain_sound_bank_load_original(uint32 destination, uint32 spu_base, uint32 buffer, uint32 mode);
uint32 gtmain_sound_path(void);
GTMainArchiveTable gtmain_archive_table_load(uint32 path, uint32 *sector, uint8 *buffer, uint32 buffer_guest, uint32 capacity);
uint32 gtmain_archive_section_load(uint32 path, const uint8 *table, uint32 sector, uint32 index, uint32 destination, uint32 limit);
uint32 gtmain_archive_table_load_guest(uint32 path, uint32 sector_out, uint32 buffer, uint32 capacity);

sint32 gtmain_cd_file_query(uint32 guest_path, uint8 descriptor[24]);
sint32 gtmain_cd_file_read(uint8 *destination, uint32 capacity, uint32 sector, uint32 bytes);
sint32 gtmain_cd_file_decode(uint8 *destination, uint32 capacity, uint32 byte_offset, uint32 compressed_bytes, uint32 output_bytes);
sint32 gtmain_cd_decode_ready(void);
sint32 gtmain_cd_decode_wait(void);

void gtmain_cd_stream_native_destination(uint8 *destination, uint32 capacity, uint32 bytes);

sint32 gtmain_cd_directory_select(const uint8 *path);

uint32 gtmain_car_name(uint32 index);
sint32 gtmain_car_record_find(uint32 name, uint32 archive, uint32 section, uint32 require_alternate);
sint32 gtmain_car_find(uint32 name);
uint32 gtmain_car_config_copy(uint32 destination, uint32 source);
uint32 gtmain_car_config_fields(uint32 destination, uint32 source, uint32 descriptors, uint32 archive);
uint32 gtmain_car_config_build(uint32 destination, uint32 selectors, uint32 archive);
uint32 gtmain_random_step(uint32 state);
uint32 gtmain_car_config_bind(uint32 first, uint32 second, uint32 third, sint16 fourth, uint16 fifth);

uint32 gtmain_spu_transfer_address(uint32 address);
uint32 gtmain_spu_transfer_mode(uint32 mode);
uint32 gtmain_spu_control_bit(uint32 enabled);
uint32 gtmain_data_checksum(uint32 data, uint32 bytes);
sint32 gtmain_replay_payload_import(sint32 index, uint32 ghost);
sint32 gtmain_audio_status(void);
uint32 gtmain_music_file(void);
uint32 gtmain_audio_buffer_end(uint32 index);
uint32 gtmain_audio_buffers_init(uint32 first, uint32 second, uint32 words);
uint32 gtmain_audio_buffers_relocate(void);
sint32 gtmain_audio_output_scale(uint32 shift);

sint32 gtmain_light_pair_transform(const uint8 *coefficients, const uint8 *vector);

uint32 gtmain_font_load(sint32 font);
sint32 gtmain_text_width(sint32 font, uint32 text);
uint32 gtmain_frame_ordering(uint32 alternate);

sint32 gtmain_driver_mode_set(uint32 unused, uint32 driver, sint32 mode);
uint32 gtmain_race_mode_record(void);
uint32 gtmain_race_record_selection_prepare(void);
uint32 gtmain_race_mode_vehicle_prepare(void);
uint32 gtmain_driver_leader(void);
sint32 gtmain_driver_assist(uint32 car);
sint32 gtmain_driver_demo_active(void);
sint32 gtmain_driver_is_primary(uint32 car);
uint32 gtmain_car_configuration_export(void);
sint32 gtmain_name_group_find(uint32 groups, uint32 name);
sint32 gtmain_race_preset_index(void);
sint32 gtmain_roster_shift(sint32 position);
uint32 gtmain_roster_player(uint32 index);
sint32 gtmain_roster_opponents(void);
sint32 gtmain_model_extent(uint32 model);
void gtmain_model_marker(uint32 model, uint32 destination);
uint32 gtmain_race_course_car_filter(void);
uint32 gtmain_race_saved_flag(void);
sint32 gtmain_collision_layout(uint8 *out, const uint8 *source);
uint32 gtmain_packet_three_tiles(uint32 link, uint32 tile, uint32 xy);
sint32 gtmain_vector_dot_fixed(const uint8 *vector, const uint8 *coefficients);
uint32 gtmain_course_resource_name(uint32 index);
uint32 gtmain_car_archive_load(uint32 buffer, uint32 capacity);

uint32 gtmain_cd_file_load(uint32 guest_path, uint32 guest_destination, uint32 requested_bytes, uint32 decoded_bytes);
uint8 * gtmain_lz_decode(uint8 *destination, uint32 destination_capacity, const uint8 *source, uint32 source_capacity, sint32 output_bytes);

uint8 * gtmain_resource_read_retry(uint32 path, uint8 *buffer, uint32 capacity, uint32 decoded_bytes);
void gtmain_font_texture_upload(uint32 destination, sint32 page, const uint8 *data);
uint32 gtmain_frame_tables_bind(uint32 *first, uint32 *second, uint32 count);
uint32 gtmain_frame_tables_clear(sint32 selection);
uint32 gtmain_ordering_clear(uint32 index);
sint32 gtmain_frame_run(uint8 *context);
uint32 gtmain_menu_frame_context_initialize(uint8 *context);
uint32 gtmain_menu_input_context_initialize(uint32 context);
sint32 gtmain_menu_frame_run(void);
uint32 gtmain_menu_result_mode4_run(uint32 extra, sint16 index, uint32 first);
uint32 gtmain_menu_result_mode4_alternate_run(uint32 extra, sint16 index, uint32 first);
void gtmain_frame_vblank(void);
uint32 gtmain_frame_prepare(uint32 alternate);
uint32 gtmain_frame_display_begin(void);
void gtmain_frame_swap(uint32 wait_mode);
void gtmain_frame_submit(uint32 alternate);
void gtmain_ordering_submit(uint32 index);
uint32 gtmain_frame_links_reset(uint32 alternate);

void gtmain_frame_events_disable(void);
void gtmain_frame_callback_remove(uint32 node);
uint8 *gtmain_frame_context_get(void);
void gtmain_native_frame_callback_insert(uint32 *node);
void gtmain_native_frame_callback_remove(uint32 *node);
void gtmain_native_frame_guest_removed(uint32 node, uint32 previous, uint32 next);
sint32 gtmain_frame_event_open(uint32 event_class, uint32 specification, uint32 mode, uint32 callback_token);
sint32 gtmain_frame_shutdown(void);
uint8 *gtmain_cd_file_load_native(uint32 guest_path, uint8 *destination, uint32 capacity, uint32 requested_bytes, uint32 decoded_bytes);

sint32 gtmain_course_start_segment(void);
uint32 gtmain_course_start_position(void);
sint32 gtmain_course_start_angle(void);
uint32 gtmain_course_grid_position(uint32 index);
sint32 gtmain_course_nearest_segment(const uint8 *position);
sint32 gtmain_course_segment_find(uint32 index, const uint8 *position);
sint32 gtmain_course_progress(uint32 index, const uint8 *position);
sint32 gtmain_car_record_find_variant(uint32 name, uint32 archive, uint32 section);
uint32 gtmain_car_selected_record(uint32 index, uint32 archive, uint32 section);
uint32 gtmain_race_driver_selection(uint32 index, uint32 archive);
uint32 gtmain_race_header_update(uint32 session);
sint32 gtmain_race_pad_contexts_reset(void);
uint8 *gtmain_race_context_get(void);
uint32 gtmain_race_buffers_bind(void);
uint32 gtmain_course_depth_table(uint32 course);
uint32 gtmain_race_scene_nodes_bind(void);
uint32 gtmain_menu_backdrop_draw(uint32 ordering_link, uint32 x, uint32 y,
    uint32 red, uint32 green, uint32 blue);
sint32 gtmain_opponent_configurations(uint32 archive, uint32 section);
uint32 gtmain_car_selection_bind(uint32 index, uint32 archive);
uint32 gtmain_car_saved_selection_load(uint32 index, uint32 saved_index, uint32 archive);
uint32 gtmain_player_configuration_build(void);

uint32 gtmain_frame_init(uint32 mode);
uint32 gtmain_frame_events_init(void);
void gtmain_frame_callback_insert(uint32 node);

uint32 gtmain_frame_environments_apply(void);
uint32 gtmain_frame_display_apply(uint32 alternate);
uint32 gtmain_frame_draw_apply(uint32 alternate);
sint32 gtmain_ordering_node_insert(uint32 node);
sint32 gtmain_projection_horizontal(uint32 context);
sint32 gtmain_projection_vertical(uint32 context);
uint32 gtmain_cd_directory_name(uint32 entry);
void gtmain_pad_system_init(void);
void gtmain_stream_callbacks_install(void);
sint32 gtmain_stream_callbacks_restore(void);
sint32 gtmain_stream_pause(void);
sint32 gtmain_stream_filter_apply(void);
uint32 gtmain_collision_probe_segment(uint8 *probes, uint32 segment);
uint32 gtmain_course_interval_find(sint32 lap, sint32 progress);
uint32 gtmain_collision_probes_update(uint32 segment_index, uint8 *probes);
uint32 gtmain_collision_polygon_find(uint32 segment, const uint8 *position);
uint32 gtmain_collision_polygon_height(uint32 segment, uint32 polygon, const uint8 *position);
uint32 gtmain_collision_query(uint8 *query);
uint32 gtmain_object_records_relocate(uint32 records);
uint32 gtmain_object_tail_end(uint32 tail);
uint32 gtmain_collision_edges_end(uint32 edges);
uint32 gtmain_collision_grid_relocate(uint32 grid, uint32 segment);
uint32 gtmain_collision_values_end(uint32 values);
uint32 gtmain_course_segments_relocate(uint32 course);
uint32 gtmain_distance_groups_relocate(uint32 groups);
void gtmain_distance_groups_bind(uint32 groups, uint32 records);

sint32 gtmain_race_sound_voice(uint32 guest_owner, sint32 instrument_index, sint32 volume, sint32 pan, sint32 priority, sint32 pitch_scale);
void gtmain_race_sound_car(uint32 guest_scratch, uint32 guest_car, uint32 car_index, uint32 priority_base);
void gtmain_race_sound_update(void);
uint32 gtmain_race_sound_volume_apply(void);
uint32 gtmain_sound_voice_start_values(uint32 guest_owner, uint32 packed_volume, uint16 pitch, uint16 sample, uint32 adsr, uint16 duration, uint8 priority, uint8 reverb);
uint32 gtmain_sound_voice_volume_value(uint32 guest_owner, uint32 packed_volume);

sint32 gtmain_cd_command_native(uint8 command, uint8 *parameter);

void gtmain_stream_alternate_sync(uint8 status, const uint8 *response);
sint32 gtmain_stream_position_update(const uint8 *position);
sint32 gtmain_stream_stop_request(void);
sint32 gtmain_stream_restart_request(void);
sint32 gtmain_stream_state_active(uint32 state);
sint32 gtmain_stream_begin(void);
uint32 gtmain_vector_normalize_xy(uint8 *out, const uint8 *input);

sint32 gtmain_race_sound_instrument(uint32 guest_instrument, sint32 volume, sint32 pan, sint32 priority);
sint32 gtmain_race_sound_instrument_pair(uint32 guest_instrument, uint32 stereo);
sint32 gtmain_race_sound_effect(sint32 instrument_index, sint32 volume, sint32 pan, sint32 priority);
sint32 gtmain_race_sound_effect_default(sint32 instrument_index);
uint32 gtmain_race_sound_channel_reverb(uint32 guest_channel);
uint32 gtmain_race_sound_car_init(uint32 guest_car);
sint32 gtmain_race_sound_car_stop(uint32 guest_car);
uint32 gtmain_race_sound_car_channels_prepare(uint32 car, uint32 engine_index);

uint32 gtmain_car_collision_positions(uint32 car);
sint32 gtmain_collision_height_query(sint32 x, sint32 z, sint32 y, uint32 *segment);
uint32 gtmain_car_rumble_update(uint32 car);
uint32 gtmain_course_segment_transform(uint32 context, uint32 segment);
uint32 gtmain_course_segment_draw(uint32 context, uint32 segment, uint32 mode);
uint32 gtmain_model_bounds_classify(uint32 context, const uint8 *points, uint32 count);
uint32 gtmain_model_billboards_draw(uint32 context, uint32 records, uint32 count);
uint32 gtmain_course_scene_draw(uint32 context, uint32 camera, uint32 mode);

sint32 gtmain_vector_normalize_xyz(uint8 *out, const uint8 *input);

sint32 gtmain_stream_entry_find(uint32 index);
sint32 gtmain_stream_entry_prepare(uint32 entry, uint32 parameter);
sint32 gtmain_stream_begin_if_loaded(void);
uint32 gtmain_stream_playlist_next(uint32 playlist);
sint32 gtmain_stream_mode_set(uint8 mode);
sint32 gtmain_stream_stop_wait(uint32 wait);
sint32 gtmain_stream_stop_async(void);
sint32 gtmain_stream_pause_state(void);
sint32 gtmain_stream_pause_if_loaded(void);
uint32 gtmain_tim_block_upload(uint32 block, sint32 xy, sint32 wh);
uint32 gtmain_tim_upload(uint32 image, sint32 first_xy, sint32 first_wh, sint32 second_xy, sint32 second_wh, uint32 reverse_order);
uint32 gtmain_texture_decode(uint32 destination, uint32 resource);
uint8 *gtmain_texture_decode_native(uint8 *destination, uint32 capacity, uint32 resource);
uint32 gtmain_tim_block_upload_native(const uint8 *block, uint32 capacity, sint32 xy, sint32 wh);
uint32 gtmain_tim_upload_native(const uint8 *image, uint32 capacity, sint32 first_xy,
    sint32 first_wh, sint32 second_xy, sint32 second_wh, uint32 reverse_order);
uint32 gtmain_race_hud_textures_load(void);
uint32 gtmain_race_overlay_textures_load(void);
uint8 *gtmain_race_hud_tim_block(uint8 *image, uint32 capacity, sint32 xy);
uint32 gtmain_race_icons_load(void);
uint32 gtmain_texture_workspace_size(uint32 image);
uint32 gtmain_race_resources_load(void);
void gtmain_race_message_flags_reset(void);
uint32 gtmain_race_message_tables_prepare(void);
uint32 gtmain_race_configuration_snapshot(void);
uint32 gtmain_race_driver_mode_finalize(void);
uint32 gtmain_race_view_initialize(uint32 view, uint32 car, uint32 pad);

uint32 gtmain_texture_palette_load(uint32 guest_record, uint32 guest_block, uint32 guest_image);
uint32 gtmain_texture_pixels_load(uint32 guest_record, uint32 guest_block, uint32 guest_image);
uint32 gtmain_texture_clut_native(uint32 id);
uint32 gtmain_texture_page_native(uint32 id);
sint32 gtmain_texture_x_native(uint32 id);
sint32 gtmain_texture_y_native(uint32 id);
uint32 gtmain_camera_flat_texture_ids(const uint32 *ids, uint32 ids_count, uint32 packets, sint32 count, uint32 vertices);
uint32 gtmain_camera_gouraud_texture_ids(const uint32 *ids, uint32 ids_count, uint32 packets, sint32 count, uint32 vertices);
sint32 gtmain_texture_name_find_native(const uint8 *name);
void gtmain_texture_workspace_publish(uint32 workspace);
uint32 gtmain_texture_records_load(uint32 guest_image);
uint32 gtmain_race_sound_bank_load(uint32 guest_dest, uint32 spu_base, const uint8 *archive_table, uint32 sector, uint32 section_index);
void gtmain_race_sound_banks_update(void);

void gtmain_race_mode0_prepare(void);
uint32 gtmain_race_rank_list_sort(void);
void gtmain_race_player_positions(void);
uint32 gtmain_race_player_positions_latch(void);
uint32 gtmain_car_distance_insert(uint32 car);
uint32 gtmain_car_render_defaults(uint32 context);
sint32 gtmain_car_model_draw(uint32 context, uint32 car, uint32 mode);
uint32 gtmain_race_cars_draw(uint32 context);
sint32 gtmain_race_cars_secondary_draw(uint32 context, uint32 links);
uint32 gtmain_race_car_by_rank(sint32 rank);

sint32 gtmain_stream_name_find(uint32 name);

void gtmain_texture_page_upload(uint32 image, sint32 page);

void gtmain_texture_palette_upload(uint32 image, sint32 palette, uint32 index);

sint32 gtmain_render_view_bind(uint32 destination, uint32 source);

uint32 gtmain_render_view_copy(uint32 destination, uint32 source);

sint32 gtmain_projection_prepare(uint32 context);

sint32 gtmain_angle_atan_wide(sint32 x, sint32 y);

uint32 gtmain_render_fog_set(uint32 context, uint32 rgb, uint32 near, uint32 far);

sint32 gtmain_model_transform_draw(uint32 context, uint32 model_variants, uint32 mode);

uint32 gtmain_view_plane_intersections(uint32 view, const uint8 *plane6);

uint32 gtmain_model_matrix_bind(uint32 context, const uint8 *matrix20);

uint32 gtmain_polygon_clip_draw(uint32 context);

uint32 gtmain_car_archive_path(void);

sint32 gtmain_car_archive_decoded_size(void);

uint32 gtmain_race_instrument_bank_load(uint32 guest_dest, uint32 guest_buffer, uint32 capacity);

uint32 gtmain_car_archive_records_load(uint32 guest_destination);

sint32 gtmain_pad_next_port(sint32 reason);

uint32 gtmain_pad_command_clear(uint32 guest_port);

uint32 gtmain_pad_command_byte(uint32 guest_port);

void gtmain_pad_command_select(uint32 guest_port);

sint32 gtmain_pad_port_begin(uint32 guest_port);

uint32 gtmain_pad_port_failure(uint32 guest_port);

void gtmain_pad_reset_port(uint32 state);

void gtmain_race_stream_transition(sint32 previous, sint32 current);

void gtmain_race_frame_tick(void);

sint32 gtmain_race_frame_status(void);

void gtmain_race_mode_prepare(void);

uint32 gtmain_race_ordering_reset(void);

void gtmain_race_overlay_draw(sint32 player);

void gtmain_race_view_draw(sint32 view_index);

void gtmain_race_updates_reset(void);

uint32 gtmain_race_messages_load(void);

sint32 gtmain_race_point_rotate(uint32 guest_xy, uint32 angle, sint32 origin_x, sint32 origin_y);

sint32 gtmain_race_pause_draw(void);

void gtmain_race_hud_draw(void);

void gtmain_race_menu_draw(void);

uint32 gtmain_race_object_flag(uint32 object);

sint32 gtmain_model_wheel_matrix(uint32 context, sint32 scale);

uint32 gtmain_model_wheel_draw(uint32 context, sint32 radius, uint32 detail);

uint32 gtmain_model_shadow_draw(uint32 context, uint32 model);

sint32 gtmain_driver_hidden(uint32 car);

uint32 gtmain_pad_response_apply(uint32 guest_port);

uint32 gtmain_pad_command_prepare(uint32 guest_port);

uint32 gtmain_pad_serial_wait(void);

uint32 gtmain_pad_callback_bind(void);

uint32 gtmain_pad_response_pending(uint32 guest_port);

uint32 gtmain_pad_config_command(uint32 guest_port, uint8 enabled);

sint32 gtmain_pad_protocol_step(uint32 port);

sint32 gtmain_pad_exchange(uint32 guest_port, sint32 outgoing);

sint32 gtmain_pad_exchange_timed(uint32 guest_port, uint8 outgoing);

uint32 gtmain_pad_serial_ack(void);

uint32 gtmain_pad_irq_bind(void);

uint32 gtmain_pad_irq_test(void);

uint32 gtmain_pad_poll(void);

uint32 gtmain_pad_port_address(uint8 selector);

void gtmain_pad_command_data_bind(uint32 port, uint32 data, uint8 count);

sint32 gtmain_pad_packet_start(uint32 port);

sint32 gtmain_pad_packet_command(uint32 port);

sint32 gtmain_pad_packet_command_tail(uint32 port, uint32 is_first_port);

sint32 gtmain_pad_packet_length(uint32 port);

sint32 gtmain_pad_packet_length_tail(uint32 port, uint32 prepare_children);

sint32 gtmain_pad_packet_ack(uint32 port);

sint32 gtmain_pad_packet_read(uint32 port);

void gtmain_race_input_poll(void);

void gtmain_race_demo_input(void);

void gtmain_race_pause_input(void);

uint32 gtmain_race_effects_tick(void);

void gtmain_race_cars_tick(void);

sint32 gtmain_race_hud_tick(void);

void gtmain_race_pause_blink(void);

sint32 gtmain_race_sound_callbacks_active(void);

sint32 gtmain_race_sound_callbacks_stopped(void);

uint32 gtmain_race_time_reset(void);

uint32 gtmain_race_lap_history_reset(void);

uint32 gtmain_race_split_reset(void);

uint32 gtmain_race_best_reset(void);

void gtmain_race_timers_reset(void);

sint32 gtmain_polygon_clip_flat(uint32 context);

sint32 gtmain_polygon_clip_gouraud(uint32 context);

void gtmain_race_input_update(uint32 state, uint32 configuration);

sint32 gtmain_race_menu_input(void);

uint32 gtmain_race_time_set(uint32 time);

uint32 gtmain_race_lap_time_store(uint32 time);

uint32 gtmain_time_string(uint32 destination, uint32 milliseconds);
void gtmain_time_string_native(char *output, uint32 capacity, uint32 milliseconds);

sint32 gtmain_race_sound_callbacks_apply(uint32 first_owner, uint32 second_owner, uint32 third_owner);

uint32 gtmain_race_effect_alloc(void);

uint32 gtmain_race_effect_init(uint32 effect);

uint32 gtmain_race_license_result_tick(void);

uint32 gtmain_race_assist_tick(void);

void gtmain_race_assist_reset(void);

void gtmain_race_mode_tick(void);

sint32 gtmain_race_countdown_tick(void);

uint32 gtmain_race_rumble_clear(void);

sint32 gtmain_polygon_clip_textured_gouraud(uint32 context);

uint32 gtmain_view_rectangle_prepare(uint32 view);

sint32 gtmain_font_string_draw(uint32 font, uint32 ordering_link, sint32 x, sint32 y, uint32 text);

uint32 gtmain_interrupt_start(void);

void gtmain_interrupt_dispatch(void);

uint32 gtmain_interrupt_callback(uint32 index, uint32 callback_token);

uint32 gtmain_interrupt_stop(void);

uint32 gtmain_vblank_interrupt_start(void);

uint32 gtmain_vblank_interrupt_dispatch(void);

uint32 gtmain_vblank_callback(uint32 index, uint32 token);

uint32 gtmain_vblank_callbacks_clear(uint32 guest_words, uint32 count);

uint32 gtmain_dma_interrupt_start(void);

uint32 gtmain_dma_interrupt_dispatch(void);

uint32 gtmain_interrupt_words_clear(uint32 guest_words, uint32 count);

uint32 gtmain_dma_callbacks_clear(uint32 guest_words, uint32 count);

sint32 gtmain_spu_dma_control(sint32 mode, uint32 address, uint32 bytes);

uint32 gtmain_spu_dma_read(uint32 destination, uint32 bytes);

uint32 gtmain_spu_register_write(uint32 index, uint32 value, uint32 scaled);

uint32 gtmain_spu_driver_start(void);

uint32 gtmain_spu_dma_write_mode(void);

uint32 gtmain_spu_dma_read_mode(void);

uint32 gtmain_spu_chip_init(uint32 mode);

uint32 gtmain_spu_transfer(uint32 source, uint32 size);

void gtmain_spu_delay(void);

uint32 gtmain_spu_dma_complete(void);

uint32 gtmain_race_pad_map(uint32 state, uint32 packet, uint32 buttons, uint32 threshold, uint32 first_axis, uint32 second_axis, uint32 analog);

uint32 gtmain_race_sound_car_primary(uint32 car);

uint32 gtmain_race_sound_car_secondary(uint32 car);

uint32 gtmain_race_lap_time_append(uint32 time);

void gtmain_race_physics_tick(void);

uint32 gtmain_race_camera_tick(uint32 camera);

uint32 gtmain_race_split_screen_update(void);

uint32 gtmain_race_effects_draw(void);

uint32 gtmain_race_player_hud_draw(sint32 player, uint32 source);

uint32 gtmain_race_demo_view_draw(uint32 context);

uint32 gtmain_race_stream_start(sint32 entry, sint32 parameter);

uint32 gtmain_model_primitives_draw(uint32 model, uint32 context);

uint32 gtmain_gpu_display_environment(uint32 guest_env20);

sint32 gtmain_gpu_pixels_read(uint32 guest_rect8, uint32 guest_destination);

sint32 gtmain_gpu_pixels_write(uint32 guest_rect8, uint32 guest_source);

sint32 gtmain_gpu_command_enqueue(uint32 callback_token, uint32 guest_data, sint32 copy_bytes, uint32 argument);

sint32 gtmain_gpu_clear_packet(uint32 guest_rect8, uint32 packed_color);

uint32 gtmain_gpu_draw_packet(uint32 guest_packet40, uint32 guest_environment28);

uint32 gtmain_gpu_clip_begin(sint16 x, sint16 y);

uint32 gtmain_gpu_clip_end(sint16 x, sint16 y);

uint32 gtmain_gpu_draw_mode(uint32 dither, uint32 display, uint16 texture_page);

uint32 gtmain_gpu_draw_offset(sint16 x, sint16 y);

uint32 gtmain_gpu_texture_window(uint32 guest_rect8);

uint32 gtmain_gpu_timeout_begin(void);

sint32 gtmain_gpu_timeout_poll(void);

uint32 gtmain_video_mode_get(void);

uint32 gtmain_gpu_display_mask(uint32 enabled);

sint32 gtmain_gpu_draw_sync(sint32 mode);

sint32 gtmain_gpu_rectangle_report(uint32 guest_label, uint32 guest_rect8);

sint32 gtmain_gpu_image_clear(uint32 rect, uint8 red, uint8 green, uint8 blue);

sint32 gtmain_gpu_image_load(uint32 rect, uint32 guest_source);

sint32 gtmain_gpu_image_store(uint32 rect, uint32 guest_destination);

uint32 gtmain_gpu_ordering_clear(uint32 guest_table, uint32 count);

sint32 gtmain_gpu_ordering_draw(uint32 guest_table);

uint32 gtmain_gpu_draw_environment(uint32 guest_env92);

sint32 gtmain_gpu_queue_sync(sint32 mode);

sint32 gtmain_gpu_device_reset(uint32 mode);

sint32 gtmain_gpu_ordering_dma_clear(uint32 guest_table, uint32 count);

uint32 gtmain_gpu_dma_submit(uint32 guest_packets);

uint32 gtmain_gpu_control_write(uint32 command);

uint32 gtmain_gpu_bytes_fill(uint32 guest_dest, uint8 value, uint32 count);

void gtmain_race_physics_prepare(uint32 cars, sint32 count);

void gtmain_race_collisions_tick(uint32 cars, sint32 count);
void gtmain_race_pair_response(uint32 cars, sint32 count);

void gtmain_race_wheel_contacts_prepare(uint32 cars, sint32 count);

void gtmain_race_wheel_contacts_query(uint32 unused_cars, sint32 count);

void gtmain_race_wheel_contacts_apply(uint32 cars, sint32 count);

void gtmain_race_pose_update(uint32 cars, sint32 count);

uint32 gtmain_race_lap_groups_tick(uint32 cars);

uint32 gtmain_race_clock_tick(void);

sint32 gtmain_race_pad_axis_value(sint32 mapping, uint32 unused_state, uint32 packet, uint32 range, uint32 analog);

uint32 gtmain_cd_controller_volume_init(void);

sint32 gtmain_cd_controller_init(void);

uint32 gtmain_cd_controller_interrupt(void);

uint32 gtmain_model_primitives_clipped_draw(uint32 model, uint32 context);

uint32 gtmain_cd_controller_service(void);

uint32 gtmain_interrupt_callback_enabled(void);

uint32 gtmain_interrupt_mask_exchange(uint16 mask);

uint32 gtmain_dma_callback_exchange(uint32 index, uint32 token);

sint32 gtmain_race_controls_decode(uint8 *controls, uint32 state, sint32 player, uint32 object);

void gtmain_race_controls_drive(uint32 car, const uint8 *controls, sint32 index);

void gtmain_race_controls_ai(uint32 car, const uint8 *unused_controls, sint32 index);

uint32 gtmain_race_controls_hud(uint32 car, const uint8 *controls);

uint32 gtmain_race_controls_hud_ai(uint32 car);

uint32 gtmain_race_controls_apply(uint32 car, const uint8 *controls, uint32 guest_output, sint32 index);

sint32 gtmain_race_curve32(uint32 curve, sint32 value);

sint32 gtmain_race_basis_set(uint32 first, uint32 second, uint32 third, sint32 first_angle, sint32 second_angle, sint32 third_angle);

sint32 gtmain_race_angle_wrap(sint32 angle);

sint32 gtmain_race_angle_delta(sint32 first, sint32 second);

uint32 gtmain_race_wheel_pose(uint32 car);

void gtmain_race_suspension_prepare(uint32 cars, sint32 count);

void gtmain_race_suspension_update(uint32 cars, sint32 count);

void gtmain_race_velocity_publish(uint32 cars, sint32 count);

uint32 gtmain_race_lap_car_tick(uint32 car, sint32 rank);

uint32 gtmain_race_lap_difference_show(sint32 car_index, sint32 difference);

void gtmain_bios_gpu_link(uint32 guest_link);

void gtmain_bios_exception_hook(uint32 guest_context);

void gtmain_bios_cd_remove(void);

void gtmain_bios_exception_reset(void);

void gtmain_bios_exception_return(void);

uint32 gtmain_menu_phase_tick(void);

uint32 gtmain_menu_counters_tick(void);

sint32 gtmain_menu_descriptor_lookup(uint32 guest_descriptor, sint32 selector, uint32 guest_value_out);

uint32 gtmain_car_wheels_draw(uint32 context, uint32 car, sint32 roll, sint16 steering, sint16 pitch);

sint32 gtmain_car_record_index(uint32 records, sint32 count, uint32 *ids, uint32 *keys, uint32 *id_count, uint32 *key_count);

uint32 gtmain_car_section_load(uint32 section, uint32 destination, uint32 limit);

sint32 gtmain_car_records_decode(uint32 records, sint32 count, const uint32 *destinations, uint32 destinations_count, uint32 texture_destination);

uint16 gtmain_color555_blend(uint16 first, uint16 second, sint16 weight);

uint32 gtmain_timer_random(void);

sint32 gtmain_text_format(uint32 ordering, sint16 x, sint16 y, uint32 format, GTMainFormatArguments *arguments);

uint32 gtmain_car_file_environment(void);

uint32 gtmain_menu_row_draw(uint32 ordering, sint32 x, sint32 y, uint32 guest_text, sint32 span, sint32 row, uint32 animated, sint32 number, uint32 alternate);

uint32 gtmain_menu_row_transition(uint32 ordering, sint32 x, sint32 y, uint32 guest_text, sint32 span, sint32 row, sint32 palette);

uint32 gtmain_menu_footer_draw(uint32 ordering, sint32 x, sint32 y, sint32 span);

sint32 gtmain_font_text_draw(uint32 ordering_link, sint32 x, sint32 y, uint32 guest_string);
uint32 gtmain_menu_status_text_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity);

sint32 gtmain_race_curve16(uint32 curve, sint32 value);

uint32 gtmain_race_suspension_car_prepare(uint32 car);

void gtmain_race_suspension_car_update(uint32 car);

void gtmain_race_input_playback(uint32 object, uint32 state);

sint32 gtmain_race_brake_assist(uint32 car);

uint32 gtmain_race_controls_launch(uint32 car, sint32 index, sint32 time);

sint32 gtmain_race_throttle_assist(uint32 car);

sint32 gtmain_race_lap_crossing(uint32 car, sint32 previous, sint32 rank);

void gtmain_race_control_latch(uint32 car, uint8 value, uint32 hold);

void gtmain_race_input_record(uint32 object, uint32 state);

uint32 gtmain_menu_palette_color(sint32 palette, sint32 intensity);
uint32 gtmain_menu_rank_bar_draw(uint32 ordering, sint32 value, sint32 maximum, sint32 x, sint32 y, sint32 intensity, uint32 full_height);

uint32 gtmain_menu_gray(uint8 intensity);

uint32 gtmain_menu_text_draw(uint32 ordering, sint32 x, sint32 y, uint32 guest_text, sint32 spacing);

uint32 gtmain_menu_text_wave(uint32 ordering, sint32 x, sint32 y, uint32 guest_text);

uint32 gtmain_menu_text_width(sint32 font, uint32 guest_text, sint32 spacing);
uint32 gtmain_menu_vehicle_course_labels_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity);

sint32 gtmain_matrix_scale_axes(uint8 *out, const uint8 *matrix, const uint8 *scales);

sint32 gtmain_model_range_index(uint32 ranges, const uint8 *position, sint32 divisor);

uint32 gtmain_view_projection_prepare(uint32 view);
uint32 gtmain_view_projection_prepare_native(uint8 *view);
uint32 gtmain_render_view_copy_native(uint32 context, const uint8 *view);
uint8 *gtmain_render_source(uint32 context);
uint32 gtmain_view_plane_intersections_native(uint8 *view, const uint8 *plane);
sint32 gtmain_model_orientation_native(uint8 *view, uint32 reference_nullable);

sint32 gtmain_course_name_find(uint32 name);
sint32 gtmain_course_selection_apply(sint32 mode);

sint32 gtmain_course_count(void);

uint32 gtmain_course_file_environment(void);

uint32 gtmain_menu_wave_select(uint32 guest_text);

uint32 gtmain_font_spaced_draw(uint32 guest_font, uint32 ordering_link, sint32 x, sint32 y, uint32 guest_text, sint32 spacing);

sint32 gtmain_audio_snapshot_upload(uint32 guest_snapshot);

sint32 gtmain_music_sector_init(void);

void gtmain_cd_file_retry_hook(void);

void gtmain_loading_callback_noop(void);

uint32 gtmain_loader_exec_image(uint32 executable);

uint32 gtmain_spu_write(uint32 guest_source, uint32 bytes);

sint32 gtmain_cd_descriptor_sector(const uint8 *descriptor);

uint32 gtmain_spu_write_transfer(uint32 guest_source, uint32 bytes);

sint32 gtmain_race_suspension_angle(uint32 guest_output, sint32 first, sint32 second);

uint32 gtmain_race_lap_report(uint32 unused_car, sint32 lap, uint32 checkpoint, uint32 time, uint32 unused_elapsed, sint32 rank);

uint32 gtmain_race_input_record_flush(uint32 object);

void gtmain_race_controls_ai_update(uint32 car, sint32 index);

uint32 gtmain_race_course_events(uint32 car, sint32 current, sint32 previous);

uint32 gtmain_menu_callback(uint32 token);
sint32 gtmain_menu_effect_context_bind(uint32 context, uint32 descriptor);
sint32 gtmain_menu_list_select(uint32 list, sint32 selection);
sint32 gtmain_menu_list_extent(uint32 list);
uint32 gtmain_course_caption_copy(uint32 text);
void gtmain_menu_record_dirty_clear(void);

uint32 gtmain_menu_name(uint32 guest_name);

sint32 gtmain_menu_push(uint32 descriptor);

sint32 gtmain_menu_push_tail(uint32 descriptor, uint32 context, sint32 previous_index);

uint32 gtmain_menu_flags(uint8 flags);

uint32 gtmain_menu_pulse_color(sint32 intensity);

sint32 gtmain_menu_pulse_component(sint32 channel, sint32 intensity);

sint32 gtmain_menu_selection(uint16 buttons, sint32 selection, sint32 count, uint32 decrement_mask, uint32 increment_mask, sint32 sound, uint32 wrap);

uint32 gtmain_menu_sound(sint32 effect);

uint32 gtmain_menu_input(void);

uint32 gtmain_menu_draw(void);

uint32 gtmain_menu_exit_requested(void);

uint32 gtmain_menu_transition(sint8 mode);

uint32 gtmain_menu_transition_tick(void);

uint32 gtmain_menu_tick(void);

sint32 gtmain_race_ai_direction(uint32 car, sint32 distance, sint32 *first, sint32 *second);

uint32 gtmain_race_ai_segment_update(uint32 car);

uint32 gtmain_race_engine_specification_summary(uint32 specification, uint32 detail);
uint32 gtmain_race_course_projection_set(uint32 position, uint32 first_basis, uint32 second_basis);
uint32 gtmain_race_route_pose_apply(uint32 car, uint32 route);
uint32 gt_heap_block_merge_next(uint32 block, uint32 incoming_result);
uint32 gtmain_heap_free(uint32 allocation);
uint32 gt_heap_block_split(uint32 block, uint32 size);
uint32 gtmain_menu_image_upload(uint32 image);
uint32 gtmain_stream_playlist_shuffle(uint32 output, sint32 count);
sint32 gtmain_settings_range_index(sint32 minimum, sint32 maximum, sint32 value, sint32 count);
void gtmain_parts_settings_import(uint32 settings, uint32 category, uint32 source);
uint32 gtmain_menu_fixed_text_extent(sint32 font, uint32 text);
uint32 gtmain_text_character_encode(uint16 character);
uint32 gtmain_race_tire_wear_update(uint32 car);
sint32 gtmain_race_pair_boost_apply(uint32 unused_peer, uint32 car, sint32 laps, sint32 distance);
sint32 gtmain_race_pair_boost_update(uint32 first_car, uint32 second_car);
uint32 gtmain_race_pair_cache_clear(sint32 car_index, sint32 count);
uint32 gtmain_race_wheel_progress_range(uint32 car, sint32 *minimum, sint32 *maximum);
uint32 gtmain_race_objective_update(uint32 car);
sint32 gtmain_race_course_progress(uint32 position);
uint32 gtmain_race_objective_initialize(sint32 mode, uint32 car, uint32 first, uint32 second);
uint32 gtmain_race_objective_mode2_time(uint32 time);
uint32 gtmain_race_objective_mode3_time(uint32 time);
uint32 gtmain_race_objective_mode4_time(uint32 lap, uint32 time);
uint32 gtmain_race_wheel_class_count(uint32 car);
sint32 gtmain_race_objective_fail(uint32 car);
sint32 gtmain_race_objective_complete(uint32 car);
sint32 gtmain_race_behavior_one(uint32 car);
sint32 gtmain_race_behavior_zero(uint32 car);
sint32 gtmain_race_behavior_toggle(uint32 car);
sint32 gtmain_race_behavior_set(uint32 car, sint32 behavior);

uint32 gtmain_race_wheel_surface_reset(uint32 car);

uint32 gtmain_race_vehicle_reset(uint32 car);

uint32 gtmain_race_finish_mark(uint32 car);

sint32 gtmain_race_ai_distance(uint32 parameters, sint32 speed);

sint32 gtmain_race_speed_control(sint32 speed, sint32 target, sint32 base, sint32 slope);

sint32 gtmain_race_ai_mode_right(sint32 mode);

sint32 gtmain_race_ai_mode_left(sint32 mode);

sint32 gtmain_race_ai_mode_neutral(sint32 mode);

void gtmain_race_ai_mode_set(uint32 car, uint8 mode);

void gtmain_race_split_record(sint32 rank, sint32 checkpoint, uint32 elapsed);

void gtmain_race_lap_record(sint32 rank, sint32 lap, uint32 time, uint32 elapsed);


uint32 gtmain_camera_resource_relocate(uint32 resource);

uint32 gtmain_camera_resource_load(uint32 car_index);

uint32 gtmain_camera_interval_update(uint32 camera);

uint32 gtmain_camera_spline_position(uint32 spline, uint8 *position12);

void gtmain_roster_generate(uint32 guest_list, uint32 count, uint32 mode, uint32 spec);

uint32 gtmain_car_start_delays(uint32 base_time);

uint32 gtmain_race_settings_snapshot(void);

uint32 gtmain_roster_initialize(void);

sint32 gtmain_car_grid_place(uint32 car, uint32 grid_index, uint32 course_segment);

uint32 gtmain_race_cars_initialize(void);

void gtmain_menu_frame_begin(void);

uint32 gtmain_menu_panel_draw(uint32 guest_context, sint32 x, sint32 y, uint32 previous);

uint32 gtmain_race_split_header_clear(void);

sint32 gtmain_race_wheel_state_init(uint32 car);

uint32 gtmain_race_gear_reset(uint32 car);

void gtmain_race_car_kind_set(uint32 car, uint8 kind);

sint32 gtmain_race_car_mode_set(uint32 car, sint8 mode);

uint32 gtmain_menu_number_scale(uint32 guest_destination, sint32 value, uint32 guest_suffix);

uint32 gtmain_menu_number_tail(uint32 guest_destination, sint32 value, uint32 guest_suffix, sint32 thousands);

uint32 gtmain_menu_text_digits(uint32 ordering, sint32 x, sint32 y, uint32 guest_text);

uint32 gtmain_menu_status_input(void);
uint32 gtmain_menu_item_transition_draw(uint32 output, uint32 ordering, uint32 selector, sint32 x, sint32 y, sint32 intensity, uint32 flags);
uint32 gtmain_menu_item_center_draw(uint32 ordering, uint32 descriptor, sint32 x, sint32 y, sint32 intensity);
uint32 gtmain_menu_sprite(uint32 guest_descriptor, uint32 ordering, uint16 x, sint32 y, uint8 intensity);

uint32 gtmain_menu_panel_callback(uint32 token, uint32 ordering, sint32 x, sint32 y, uint32 display_y, uint32 fade);

uint32 gtmain_race_ai_alternate_position(sint32 mode, sint32 unused_segment, sint32 progress, sint32 *x, sint32 *z);
void gtmain_race_ai_spline_position(sint32 mode, sint32 segment, sint32 progress, sint32 *first, sint32 *second);

void gtmain_race_ai_segment_speed(uint32 car);

void gtmain_race_ai_segment_bind(uint32 car);

sint32 gtmain_race_ai_mode_select(uint32 mode);

void gtmain_race_best_time_set(uint32 time);

void gtmain_race_best_lap_set(uint32 elapsed);

uint32 gtmain_camera_resource_path(void);

uint32 gtmain_race_simulation_configure(void);

void gtmain_race_controls_initialize(uint32 controls, uint32 preserve_history);

sint32 gtmain_race_points_apply(void);

void gtmain_race_vehicle_initialize(uint32 car, uint32 specification, uint32 segment, sint32 x, sint32 z, sint32 sine, sint32 cosine, uint32 body, uint8 variant, uint8 mode, uint32 transmission);

uint32 gtmain_race_vehicle_grid_initialize(uint32 car, uint32 segment, sint32 x, sint32 z, sint32 sine, sint32 cosine);

uint32 gtmain_race_vehicle_variant_set(uint32 car, sint8 variant);

uint32 gtmain_race_ai_default(uint32 offset);

sint32 gtmain_race_points_print(uint32 guest_label);

uint32 gtmain_spu_init_cold(void);

uint32 gtmain_spu_init_state(uint32 mode);

uint32 gtmain_spu_callback_bind(uint32 token);

uint32 gtmain_spu_read(uint32 guest_destination, uint32 bytes);

uint32 gtmain_cd_read_callback_bind(uint32 token);

uint32 gtmain_cd_read_mode(uint32 mode);

uint32 gtmain_cd_sector_read(uint32 guest_destination, uint32 words);

void gtmain_bios_gte_patch(void);

uint32 gtmain_race_effect_state_clear(void);

uint32 gtmain_bios_card_patch_copy(void);

void gtmain_race_specification_initialize(uint32 destination, uint32 specification, sint32 body, uint32 enabled, uint8 transmission);

uint32 gtmain_vblank_wait(sint32 target, uint32 budget);

uint32 gtmain_race_tuning_initialize(void);

uint32 gtmain_race_vehicle_bounds_update(uint32 car);

uint32 gtmain_race_vehicle_bounds_copy(uint32 car);

sint32 gtmain_race_vehicle_pose_update(uint32 car);

uint32 gtmain_race_engine_tuning_initialize(void);

uint32 gtmain_race_assist_preset(uint32 preset);

sint32 gtmain_race_vehicle_initialization_finish(void);
sint32 gtmain_race_selected_vehicle_initialize(void);

sint32 gtmain_race_aero_force(uint32 car, sint32 speed);

sint32 gtmain_race_aero_components(uint32 car, sint32 speed, sint32 *first, sint32 *second);

uint32 gtmain_spu_driver_stop(void);

uint32 gtmain_card_init_result(uint32 mode);
void gtmain_card_native_absent_init(uint32 mode);
uint32 gtmain_card_native_present(uint32 channel);

uint32 gtmain_card_start_result(void);

uint32 gtmain_card_stop(void);

void gtmain_bios_card_init(uint32 mode);

void gtmain_bios_card_start(void);

void gtmain_bios_card_stop(void);

void gtmain_bios_card_patch(void);

void gtmain_bios_card_patch_second(void);

uint32 gtmain_bios_card_patch_restore(void);

uint32 gtmain_runtime_destructors(void);

uint32 gtmain_runtime_finalize(void);

void gtmain_race_engine_curve_initialize(uint32 specification, uint32 curve, uint32 enabled);

void gtmain_race_transmission_initialize(uint32 specification, uint32 transmission, uint32 engine_curve);

sint32 gtmain_race_steering_peak(uint32 parameters);

void gtmain_race_force_parameters_initialize(uint32 parameters, uint32 enabled);

void gtmain_race_specification_curves_build(uint32 source_x, uint32 source_y, uint32 source_z, uint32 source_extra, sint32 count, uint32 source_a, uint32 source_b, uint32 source_c, uint32 source_d, sint32 secondary_count, uint32 curve_y, uint32 curve_z, uint32 curve_extra, uint32 output_x, uint32 output_y, uint32 output_z, uint32 output_extra, uint32 peak_first, uint32 peak_second);

uint32 gtmain_menu_status_reset(void);

uint32 gtmain_menu_bootstrap(void);

uint32 gtmain_course_resource_bind(uint32 guest_course);

uint32 gtmain_course_models_relocate(uint32 guest_models);

uint32 gtmain_model_resource_relocate(uint32 guest_model);

sint32 gtmain_race_ai_routes_relocate(uint32 routes);

sint32 gtmain_race_ai_route_segment(uint32 records, uint32 index, sint32 segment);

uint32 gtmain_race_ai_route_ground(sint32 *height, sint32 *pitch, sint32 *roll, uint32 *flags, sint32 x, sint32 z, sint32 angle, uint32 segment);

uint32 gtmain_race_event_tables_reset(void);

uint32 gtmain_race_speed_tables_reset(void);

uint32 gtmain_course_headers_relocate(uint32 course);

uint32 gtmain_model_resource_end(uint32 model);

sint32 gtmain_menu_default_callback(void);

uint32 gtmain_menu_archive_buffer(void);

void gtmain_menu_draw_callback_set(uint32 token);

sint32 gtmain_menu_effect_restart(void);

sint32 gtmain_menu_effect_reset(void);

void gtmain_race_engine_boost_initialize(uint32 specification, uint32 engine_curve);

sint32 gtmain_race_engine_boost_limit(uint32 engine_curve, sint16 first, sint16 second);

sint32 gtmain_race_engine_boost_curve(uint32 engine_curve, sint16 first, sint16 second);

sint32 gtmain_race_steering_curve(uint32 curve, sint32 angle);

void gtmain_race_top_speed_initialize(uint32 parameters);

uint32 gtmain_menu_result_prepare(void);
uint32 gtmain_menu_result_tail_prepare(sint32 competitor);

uint32 gtmain_menu_lap_results_prepare(void);
uint32 gtmain_menu_lap_results_tail_prepare(uint32 record_flags);

uint32 gtmain_menu_lap_results_draw(uint32 ordering, sint32 x, sint32 y, uint32 descriptor_mode, sint32 intensity);
uint32 gtmain_menu_lap_results_tail_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity, sint32 initial_mode);

uint32 gtmain_menu_result_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity);
uint32 gtmain_menu_result_tail_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity, sint32 mode, sint32 competitor);

sint32 gtmain_race_engine_torque(uint32 car);

sint32 gtmain_race_gear_select(uint32 car, const uint8 *controls);

void gtmain_race_controls_reset(uint32 car);

uint32 gtmain_race_drive_forces_update(uint32 car, uint32 index);

uint32 gtmain_race_traction_apply(uint32 car, uint32 index);

uint32 gtmain_race_controls_apply_basic(uint32 car, uint8 *controls);

sint32 gtmain_camera_resource_reset(void);

sint32 gtmain_menu_effect_tick(void);

uint32 gtmain_menu_panel_special_bind(uint32 context, uint32 descriptor);

uint32 gtmain_menu_effect_begin(void);

void gtmain_race_simulation_step(uint32 nodes, sint32 count);

uint32 gtmain_race_axle_slip_update(uint32 car, sint32 index);

sint32 gtmain_race_slip_ratio(uint32 scratch_row, sint32 reference, sint32 speed);

sint32 gtmain_race_axle_velocity_integrate(uint32 car, sint32 index, sint32 axle);

sint32 gtmain_race_slip_publish(uint32 car, sint32 index);

uint32 gtmain_race_controls_query(uint32 car, const uint8 *controls, uint32 *output, sint32 mode);

uint32 gtmain_race_controls_commit(uint32 car, uint8 *controls, uint32 output, sint32 mode);

uint32 gtmain_menu_mode_setup(sint32 mode);

void gtmain_menu_selector_reset(uint32 guest_selector);
sint32 gtmain_menu_selector_input(uint32 selector, uint32 input);
sint32 gtmain_menu_saved_slot_select(uint32 input);
sint32 gtmain_menu_save_slot_select(uint32 input);
sint32 gtmain_menu_roster_item_select(uint32 input);
sint32 gtmain_menu_angle_selector_tick(void);
sint32 gtmain_menu_parameter_selector_input(uint32 selector, uint32 input);
sint32 gtmain_menu_parameter_page_tick(void);
sint32 gtmain_menu_overlay_tick(void);

uint32 gtmain_menu_indicator_draw(uint32 ordering, sint32 x, sint32 y, sint32 phase, uint32 flags, uint32 descriptor_index, uint32 style, uint32 page);

sint32 gtmain_menu_descriptor_draw(uint32 ordering, uint32 guest_selector, sint32 x, sint32 y, sint32 intensity, uint32 publish, uint8 *native_output, uint32 output_capacity);

void gtmain_race_slip_curve_update(uint32 car);

void gtmain_race_lateral_force_update(uint32 car);

void gtmain_race_lateral_drag_update(uint32 car, sint32 index);

void gtmain_race_longitudinal_integrate(uint32 car, sint32 index);

void gtmain_race_engine_velocity_update(uint32 car, sint32 index);

void gtmain_race_yaw_integrate(uint32 car, sint32 index);

void gtmain_race_world_velocity_integrate(uint32 car);

uint32 gtmain_race_contact_query(uint32 car, uint32 *fraction, uint32 *corner);

uint32 gtmain_race_contact_response(uint32 car);

uint32 gtmain_race_controls_impulse(uint32 car, uint8 *controls, uint32 fraction);

uint32 gtmain_race_wheel_effects_update(uint32 car);

void gtmain_race_wheel_rotation_update(uint32 cars, sint32 count);

sint32 gtmain_race_lateral_drag_curve(uint32 curve, sint32 speed);

sint32 gtmain_race_lateral_low_speed(sint32 axle_speed, sint32 projected_speed);

uint32 gtmain_menu_asset_activate(uint32 index);

uint32 gtmain_menu_scaled_sprite(uint32 guest_descriptor, uint32 ordering, sint16 x, sint16 y, uint8 intensity, sint32 scale);

uint32 gtmain_menu_model_draw(uint32 ordering, sint16 x, sint16 y, sint16 phase, uint32 guest_model, uint16 depth, uint16 palette, uint32 command, uint16 page, sint32 projection, uint32 flags);

uint32 gtmain_race_pair_cache_update(uint32 cars, sint32 count);

sint32 gtmain_race_pair_clip(uint32 car_index, uint32 other_slot, uint32 car, sint32 *corner, sint32 *side);

void gtmain_race_pair_cache_refresh(uint32 cars, sint32 count);

void gtmain_race_pair_impulses_update(uint32 cars, sint32 count);

void gtmain_race_pair_proximity_update(uint32 first_car, uint32 second_car);

void gtmain_race_pair_clip_publish(uint32 cars, sint32 count);

sint32 gtmain_race_suspension_response(uint32 car, uint32 contact, sint32 height, uint32 parameters, uint32 *inactive_count);

uint32 gtmain_race_recorders_stop(void);

uint32 gtmain_race_points_save(void);

uint32 gtmain_race_session_save(void);

uint32 gtmain_race_record_size(uint32 record);

uint32 gtmain_race_record_copy(uint32 destination, uint32 source, uint32 size);

void gtmain_race_record_publish(uint32 source);

void gtmain_race_recorder_stop(uint32 object);

sint32 gtmain_menu_resource_status(uint32 mode);

uint32 gtmain_menu_entry_set(uint32 owner, uint32 index, sint16 x, sint16 y, uint32 text, sint16 height, uint8 first, uint8 second, uint32 resource, uint8 kind, uint8 state, uint32 extra);

void gtmain_race_suspension_step(uint32 cars, sint32 count);

void gtmain_cd_native_poll(void);
sint32 gtmain_cd_sector_native(void *destination, sint32 words);

sint32 gtmain_menu_angle_tick(void);
uint32 gtmain_race_records_size(void);
uint32 gtmain_menu_model_bundle_relocate(uint32 bundle, uint32 page);
uint32 gtmain_race_tim_block_upload(uint32 image, sint32 xy);

sint32 gtmain_menu_model_pose_tick(uint32 pose, uint32 rotate);
uint32 gtmain_race_records_import(uint32 source);

uint32 gtmain_roster_pool_count(uint32 list);
sint32 gtmain_roster_variant_class(sint32 character);
void gtmain_roster_select(uint32 *selected, uint32 list, sint32 count, uint32 *seed, uint32 excluded, uint32 pools);
void gtmain_roster_variants(uint32 *variants, const uint32 *selected, sint32 count, uint32 *seed, uint32 excluded_class);
void gtmain_roster_commit(const uint32 *selected, const uint32 *variants, sint32 count, uint32 seed);
void gtmain_race_arcade_roster(void);

uint32 gtmain_race_driver_configurations(uint32 archive, uint32 kind, uint32 mode, uint32 section);
uint32 gtmain_car_palette_tint(uint8 *output, const uint8 *input, uint32 tint, uint32 masks);

void gtmain_race_timing_flag_clear(void);
uint32 gtmain_menu_state_capture(uint32 state);
sint32 gtmain_race_meters_prepare(uint32 meters);
void gtmain_menu_resource_request_initialize(uint32 request, sint16 height, sint16 width, uint32 output);
sint32 gtmain_menu_glyph_index(sint8 column, sint8 row);
sint32 gtmain_race_pause_menu_measure(void);


void gtmain_race_current_time_draw(uint32 ordering, sint32 x, sint32 y);
sint32 gtmain_race_lap_times_draw(uint32 ordering, sint32 x, sint32 y);
uint32 gtmain_race_best_time_draw(uint32 ordering, sint32 x, sint32 y);
void gtmain_race_record_time_draw(uint32 ordering, sint32 x, sint32 y);

uint32 gtmain_race_instruments_draw(uint32 ordering, sint32 x, sint32 y, uint32 car);
uint32 gtmain_race_rpm_scale_draw(uint32 ordering, sint32 x, sint32 y, uint32 car);
uint32 gtmain_race_gear_draw(uint32 ordering, sint32 x, sint32 y, const sint16 *bounds, uint32 car);

sint32 gtmain_font_string_draw_native(uint32 font, uint32 ordering, sint32 x, sint32 y, const char *text);
sint32 gtmain_race_lap_counter_draw(uint32 ordering, sint32 x, sint32 y, uint32 car);

uint32 gtmain_race_position_draw(uint32 ordering, sint32 x, sint32 y, uint32 corners, uint32 car);
uint32 gtmain_race_position_hud_draw(uint32 ordering, uint32 car);

uint32 gtmain_race_minimap_draw(uint32 ordering, sint32 x, sint32 y);

uint32 gtmain_race_frame_finish(void);
#endif

uint32 gtmain_camera_mode_update(uint32 camera);
uint32 gtmain_camera_projection_set(uint32 camera, sint32 distance);
uint32 gtmain_camera_pose_prepare(uint32 camera, uint32 mode, sint32 distance);
uint32 gtmain_camera_direction_matrix(uint8 *matrix, const uint8 *vector);
uint32 gtmain_vector_path_dispatch(uint32 output, uint32 path);
uint32 gtmain_camera_path_distance_update(uint32 camera);
uint32 gtmain_camera_path_phase_update(uint32 camera);
uint32 gtmain_camera_config_pose_prepare(uint32 camera);
uint32 gtmain_camera_selected_pose_prepare(uint32 camera);
uint32 gtmain_camera_follow_update(uint32 camera);
uint32 gtmain_camera_follow_controls(uint32 camera);

uint32 gtmain_camera_car_select(uint32 camera, uint32 buttons);

uint32 gtmain_camera_countdown_update(uint32 camera);
uint32 gtmain_camera_countdown_mode(uint32 camera);
uint32 gtmain_camera_orbit_update(uint32 camera);

uint8 *gtmain_race_frame_draw(void);

uint32 gtmain_model_radial_draw(uint32 context, uint32 records, uint32 count);

uint32 gtmain_model_sprites_draw(uint32 context, uint32 records, uint32 count);

uint32 gtmain_model_radial_effect_draw(uint32 context, uint32 records, uint32 count);

sint32 gtmain_race_banner_draw(void);
uint32 gtmain_race_screen_rectangle(uint32 ordering_link);

uint32 gtmain_race_standard_hud_draw(void);

uint32 gtmain_race_labels_draw(uint32 ordering, sint32 y, uint32 mode, uint32 second_line);

void gtmain_race_times_draw(uint32 ordering);

uint32 gtmain_race_record_label_draw(void);
sint32 gtmain_race_record_text_draw(uint32 indication, uint32 guest_text);

uint32 gtmain_race_driver_hud_draw(uint32 ordering);
void gtmain_race_status_draw(uint32 ordering, sint32 y, uint32 car);

sint32 gtmain_race_countdown_text_draw(uint32 ordering, sint32 y);

uint32 gtmain_model_depth_cued_draw(uint32 context, uint32 model_header);
uint32 gtmain_model_depth_cued_clipped_draw(uint32 context, uint32 model_header);

uint32 gtmain_menu_record_workspace_clear(void);
uint32 gtmain_menu_machine_get_init(void);

uint32 gtmain_race_meter_color(sint32 value);
uint32 gtmain_race_meter_rectangle_draw(uint32 ordering, sint16 x, sint16 y, sint32 value);
uint32 gtmain_packet_rectangle_native(uint32 ordering, const sint16 *corners4, uint32 color);

uint32 gtmain_menu_countdown_toggle_tail(sint32 countdown);

sint32 gtmain_race_label_font2_draw(uint32 ordering, sint32 y, uint32 color, uint32 label);
sint32 gtmain_race_label_font3_draw(uint32 ordering, sint32 y, uint32 color, uint32 label);
uint32 gtmain_race_label_panel_draw(uint32 ordering, uint32 label, uint32 color);
uint32 gtmain_course_route_caption_update(void);

sint32 gtmain_camera_saved_pose_load(uint32 camera);
uint32 gtmain_race_records_export(uint32 destination);

sint32 gtmain_race_countdown_block_draw(uint32 ordering, sint32 y, sint32 label);
uint32 gtmain_race_centered_label_box_draw(uint32 ordering, uint32 color, uint32 text, uint32 corners, sint32 center_x);
sint32 gtmain_menu_machine_caption_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity);

uint32 gtmain_race_choice_highlight(uint32 command);
uint32 gtmain_race_choice_boxes_draw(void);

uint32 gtmain_menu_name_text_set(uint32 request, uint32 text);
sint32 gtmain_menu_name_edit(uint32 request, uint32 controls);
uint32 gtmain_menu_name_request_prepare(uint32 owner, sint32 index, uint32 unchanged_result);

uint32 gtmain_menu_machine_context_bind(uint8 kind, uint32 owner, uint32 resource, uint8 state, uint32 value, uint32 text);
uint32 gtmain_menu_entry_name_request_prepare(void);

sint32 gtmain_menu_range_index(sint32 start, sint32 end, sint32 value, sint32 count);

uint32 gtmain_menu_text_fit(uint32 text, sint32 width);
sint32 gtmain_menu_stream_check(void);
uint32 gtmain_menu_any_button_accept(void);

uint32 gtmain_menu_race_caption_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity, uint32 course_line);

uint32 gtmain_menu_quick_end_initialize(void);
sint32 gtmain_menu_quick_end_run(uint32 selector, uint8 disabled, uint32 first, uint32 second, uint8 style);
sint32 gtmain_menu_quick_end_mode3(uint8 disabled);
uint32 gtmain_menu_end_panel_initialize(void);
uint32 gtmain_menu_result_name_request_initialize(void);
sint32 gtmain_menu_name_keyboard_draw(uint32 ordering, uint32 request, sint32 x, sint32 y, sint32 intensity);
uint32 gtmain_menu_name_slots_draw(uint32 ordering, uint32 request, sint32 x, sint32 y, sint32 intensity);
uint32 gtmain_menu_result_name_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity);
uint32 gtmain_menu_reveal_input_tick(void);

sint32 gtmain_race_audio_stop_wait(void);

uint32 gtmain_menu_player_indicator_draw(uint32 ordering, sint32 x, sint32 y);

sint32 gtmain_replay_header_check(uint32 data);

uint32 gtmain_menu_result_panel_run(uint8 flags, sint16 index, uint32 first, uint32 second, uint32 metric, sint16 threshold, uint32 extra, uint8 mode, uint8 style, uint8 panel);

uint32 gtmain_menu_result_mode7_run(uint8 flags, uint32 first, uint32 second, uint32 metric, sint16 threshold, uint32 extra);

uint32 gtmain_menu_result_mode8_run(uint8 flags, uint32 first, uint32 second, uint32 metric, sint16 threshold, uint32 extra);

uint32 gtmain_race_result_snapshot_store(void);

uint32 gtmain_menu_result_reveal_tick(void);
uint32 gtmain_menu_result_plateau_tick(void);
uint32 gtmain_menu_result_plateau_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity);

uint32 gtmain_menu_replay_caption_initialize(void);

uint32 gtmain_menu_selection_snapshot_initialize(void);

uint32 gtmain_menu_end_panel_tick(void);
uint32 gt_draft_800380AC(void);

uint32 gtmain_menu_pose_model_draw(uint32 pose, uint32 ordering, sint32 x, sint32 y, uint32 arg4, uint32 arg5, uint32 arg6, uint32 arg7, uint32 arg8, uint32 arg9, uint32 arg10, uint32 arg11, uint32 arg12);

uint32 gtmain_menu_end_panel_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity);

uint32 gtmain_menu_race_list_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity);
uint32 gtmain_menu_race_roster_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity);
sint32 gtmain_menu_race_selection_tick(void);

sint32 gtmain_menu_name_commit_tick(void);

sint32 gtmain_menu_reward_tick(void);

uint32 gtmain_menu_reward_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity);

sint32 gtmain_menu_result_list_tick(void);

sint32 gtmain_menu_roster_slot_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity, uint32 slot);

uint32 gtmain_menu_roster_panel_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity);

uint32 gtmain_menu_progress_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity, sint32 selection, uint32 show_pending);

uint32 gtmain_bios_critical_enter(void);
uint32 gtmain_bios_critical_exit_result(void);

uint32 gtmain_race_meter_panel_draw(uint32 ordering, uint32 x, uint32 y, uint32 source);

uint32 gtmain_resource_sections_load(uint32 path, uint32 destination, uint32 requested_bytes);

uint32 gtmain_menu_transition_enter(uint32 descriptor);

uint32 gtmain_menu_transition_exit(uint32 descriptor);

uint32 gtmain_menu_mode_dispatch(sint32 mode);

uint32 gtmain_menu_record_header_prepare(void);
uint32 gtmain_menu_record_labels_prepare(void);

uint32 gtmain_menu_record_screen_draw(uint32 ordering, sint32 x, sint32 y, uint32 carried_argument, sint32 intensity);

uint32 gtmain_menu_choice_transition_draw(uint32 ordering, uint32 descriptor, sint32 x, sint32 y, sint32 intensity, uint32 publish, uint32 reverse, uint32 lower_title);
uint32 gtmain_packet_outline_native(uint32 ordering, const sint16 *corners4, uint32 color);

uint32 gtmain_menu_device_selection_input(void);
uint32 gtmain_menu_saved_entry_commit(uint32 owner, sint32 index);

sint32 gtmain_menu_saved_entry_input(uint32 owner, uint32 input);
uint32 gtmain_menu_saved_entry_delete(uint32 owner, sint32 index);

uint32 gtmain_menu_decimal_draw(uint32 ordering, sint32 x, sint32 y, sint32 value, sint32 precision, uint32 suffix, uint8 intensity, uint32 style);

sint32 gtmain_menu_entry_type_input(uint32 input);

uint32 gtmain_menu_machine_status_initialize(void);
uint32 gtmain_menu_settings_initialize(void);

uint32 gtmain_menu_settings_mask(uint32 table, sint32 count);
uint32 gtmain_menu_settings_masks_update(void);

uint32 gtmain_menu_settings_rows_draw(uint32 ordering, sint32 x, sint32 y, sint32 unused, sint32 intensity);
uint32 gtmain_menu_settings_row_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity, sint32 entry, sint32 row);

sint32 gtmain_menu_part_check(uint32 target, sint32 part_type);
sint32 gtmain_menu_parts_check(uint32 target);

uint32 gtmain_car_selection_copy(uint32 index, uint32 source, uint32 archive);
uint32 gtmain_parts_table(uint32 category);
uint32 gtmain_parts_count(uint32 category);
uint32 gtmain_parts_count_at(uint32 offset);
uint32 gtmain_parts_car_name(uint32 car);
uint32 gtmain_parts_label(uint32 category, uint32 selection, uint32 destination);
uint32 gtmain_parts_record(uint32 category, uint32 selection, uint32 archive);
uint32 gtmain_parts_selected_record(uint32 category, uint32 selection, uint32 archive);
uint32 gtmain_race_settings_reset(void);
uint32 gtmain_race_opponent_restore(void);
uint32 gtmain_race_selector_set(sint32 selector);
uint32 gtmain_race_rank_adjust(void);
uint32 gtmain_race_state_checkpoint(void);
uint32 gtmain_8004ef9c(uint32 context, sint32 first, sint32 second, sint32 third);
uint32 gtmain_race_points_rank_update(void);
uint32 gtmain_race_record_context_check(void);
sint32 gtmain_race_driver_setup_if_needed(uint32 kind);
uint32 gtmain_race_record_update(uint32 total);
uint32 gtmain_event_result_category(void);
uint32 gtmain_event_result_record(void);
uint32 gtmain_event_opponents_generate(void);
uint32 gtmain_event_race_parameter(void);
uint32 gtmain_event_race_has_next(void);
uint32 gtmain_event_prize_award(uint32 mode);
uint32 gtmain_event_championship_award(void);
sint32 gtmain_event_preview_models_load(void);
uint32 gtmain_event_results_begin(void);
uint32 gtmain_event_results_tick(void);
uint32 gtmain_event_speed_summary_begin(void);
uint32 gtmain_event_speed_maximum_begin(void);
uint32 gtmain_event_speed_maximum_select(void);
uint32 gtmain_event_speed_summary_tick(void);
uint32 gtmain_event_speed_summary_draw(void);
uint32 gtmain_event_points_table_draw(uint32 ordering, sint32 y, sint32 count, uint32 final_scores);
uint32 gtmain_trial_record_values_load(uint32 alternate);
uint32 gtmain_trial_records_commit(uint32 alternate);
uint32 gtmain_trial_caption_select(sint32 mode);
uint32 gtmain_trial_ranking_table(sint32 mode);
uint32 gtmain_trial_best_value_load(sint32 mode);
uint32 gtmain_menu_result_speed_run(uint32 extra, sint16 index, uint32 first);
uint32 gtmain_trial_ranking_commit(sint32 mode);
uint32 gtmain_race_summary_delay_begin(void);
uint32 gtmain_race_summary_delay_tick(void);
uint32 gtmain_race_signed_text_if_enabled(void);
uint32 gtmain_arcade_summary_draw(void);
void gtmain_two_player_times_reset(void);
uint32 gtmain_menu_result_mode2_run(uint8 player, uint32 record, uint32 time);
uint32 gtmain_two_player_result_show(void);
sint32 gtmain_arcade_car_count_setting(void);
uint32 gtmain_arcade_car_name_setting(sint32 index);
uint32 gtmain_course_segment_radial_draw(uint32 context, uint32 segment);
void gtmain_scene_resources_invalidate(void);
sint32 gtmain_two_player_course_select(void);
sint32 gtmain_arcade_course_index(uint32 course);
sint32 gtmain_championship_continue(uint8 disabled, uint8 has_next);
uint32 gtmain_championship_run(void);
sint32 gtmain_trial_course_options(void);
sint32 gtmain_trial_continue(uint8 disabled);
sint32 gtmain_two_player_continue(uint8 disabled);
sint32 gtmain_trial_record_continue(uint8 disabled, uint8 style, uint32 first, uint32 second);
sint32 gtmain_trial_mode0_continue(uint8 disabled);
sint32 gtmain_trial_mode1_continue(uint8 disabled);
sint32 gtmain_trial_mode2_continue(uint8 disabled);
sint32 gtmain_alternate_trial_continue(uint8 disabled);
uint32 gtmain_game_mode_run(void);
uint32 gtmain_license_results_commit(void);
sint32 gtmain_license_result_menu(sint32 index, sint32 position, uint32 flags);
uint32 gtmain_menu_result_mode6_run(uint8 flags, sint16 index, uint32 first, uint32 second,
    uint32 metric, sint16 threshold, uint32 extra);
uint32 gtmain_menu_result_mode1_run(uint8 flags, sint16 index, uint32 first, uint32 second,
    uint32 metric, sint16 threshold, uint32 extra);
uint32 gtmain_80034f5c(uint32 mode, sint32 rank, uint32 amount, uint32 balance, uint32 flags);
sint32 gtmain_race_rank_print(uint32 label);
sint32 gtmain_race_ranks_reset(void);
sint32 gtmain_race_rank_reorder(void);

uint32 gtmain_license_completion_masks(void);

uint32 gtmain_menu_text_layout(uint32 index, uint32 draw, uint32 y, uint32 ordering);
uint32 gtmain_menu_text_panel_draw(uint32 ordering, uint32 index);

uint32 gtmain_menu_background_draw(uint32 ordering);
sint32 gtmain_menu_heading_draw(uint32 ordering);
sint32 gtmain_menu_status_draw(uint32 ordering);
uint32 gtmain_menu_result_footer_draw(uint32 ordering);
uint32 gtmain_menu_button_draw(uint32 ordering, uint32 color, uint32 text, uint32 corners, uint32 x);
uint32 gtmain_packet_line_segment(uint32 ordering, uint32 endpoints, uint32 color);

uint32 gtmain_menu_request_draw(uint32 ordering, uint32 first_text, uint32 second_text, uint32 request);
uint32 gtmain_menu_scroll_tick(void);
uint32 gtmain_menu_request_phase_tick(void);
uint32 gtmain_menu_scroll_limit(uint32 request);
uint32 gtmain_menu_request_pulse_color(uint32 flags);

uint32 gtmain_save_payload_read(uint32 source);
uint32 gtmain_save_payload_write(uint32 destination);
uint32 gtmain_race_time_notice(uint32 unused0, uint32 unused1, uint32 unused2, uint32 time);
uint32 gtmain_race_result_hints_draw(uint32 ordering, uint32 first_text, uint32 second_text);
uint32 gtmain_race_choice_initialize(void);

uint32 gtmain_menu_result_mode5_run(uint8 flags, sint16 index, uint32 unused, uint32 alternate,
    uint32 record, uint32 metric, sint16 threshold, uint32 extra, uint8 panel);
uint32 gtmain_menu_reward_title_draw(uint32 ordering, sint32 x, sint32 y, uint32 text,
    sint32 palette, sint32 intensity, sint32 elapsed);
uint32 gtmain_menu_grouped_decimal(sint32 value, uint32 destination);
sint32 gtmain_menu_fixed_text_draw(uint32 ordering, sint32 x, sint32 y, uint32 text, sint32 font);

uint32 gtmain_menu_fixed_text_width(sint32 font, uint32 text);
uint32 gtmain_menu_reward_amount_draw(uint32 ordering, sint32 x, sint32 y, sint32 palette,
    sint32 intensity, uint32 amount, sint32 span, sint32 row_y, sint32 elapsed);

uint32 gtmain_menu_reward_initialize(void);
uint32 gtmain_menu_prize_draw(uint32 ordering, sint32 x, sint32 y, uint32 unused, sint32 intensity);
uint32 gtmain_menu_prize_initialize(void);
uint32 gtmain_menu_pose_initialize(uint32 pose);
uint32 gtmain_menu_gradient_draw(uint32 ordering, sint32 x, sint32 y, sint32 width, sint32 height,
    sint32 intensity, uint8 r0, uint8 g0, uint8 b0, uint8 r1, uint8 g1, uint8 b1);
uint32 gtmain_packet_gouraud_quad(uint32 ordering, uint32 coordinates, uint32 colors);

uint32 gtmain_event_competitor_complete(sint32 competitor);
uint32 gtmain_event_special_complete_count(void);
uint32 gtmain_event_all_medals_two(void);
sint32 gtmain_menu_stack_pop(void);
uint32 gtmain_menu_stack_replace(uint32 descriptor);

uint32 gtmain_menu_font_prepare(void);
uint32 gtmain_menu_font_context_initialize(uint32 context);
uint32 gtmain_menu_font_context_bind(uint32 context, uint32 rectangle);
uint32 gtmain_menu_caption_plain_draw(uint32 ordering, sint32 x, sint32 y, uint32 text);
uint32 gtmain_menu_caption_mixed_draw(uint32 ordering, sint32 x, sint32 y, uint32 text);
uint32 gtmain_menu_cached_glyph_draw(uint32 context, uint16 glyph, uint32 ordering, uint32 xy, uint32 wh);
uint32 gtmain_event_all_medals_four(void);

uint32 gtmain_font_cache_acquire(uint32 context, uint16 glyph, uint32 x_out, uint32 y_out);
uint32 gtmain_font_glyph_unpack(uint32 destination, const uint8 *rom_glyph30, sint32 pitch);

uint32 gtmain_menu_caption_width(uint32 text);
uint32 gtmain_menu_caption_enable(void);

uint32 gtmain_menu_option_selector_initialize(void);
uint32 gtmain_menu_option_background_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity);

uint32 gtmain_menu_option_column_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity,
    sint32 selection, uint32 list, uint32 resources, uint32 names, uint32 highlight);
uint32 gtmain_menu_option_columns_draw(uint32 ordering, sint32 x, sint32 y, sint32 unused, sint32 intensity);
uint32 gtmain_menu_option_entry_draw(uint32 ordering, sint32 selection, sint32 maximum, sint32 x,
    sint32 y, uint32 brightness, uint32 text, uint32 value, sint32 extra, uint32 flags, uint32 full_height);

void gtmain_menu_parameter_selector_reset(uint32 selector);
uint32 gtmain_menu_option_columns_initialize(void);

uint32 gtmain_menu_option_list_tick(void);
uint32 gtmain_menu_option_list_draw(uint32 ordering, sint32 x, sint32 y, sint32 unused, sint32 intensity);
uint32 gtmain_menu_selector_layout(uint32 selector);

uint32 gtmain_menu_option_list_initialize(void);
uint32 gtmain_menu_option_detail_tick(void);
uint32 gtmain_menu_option_detail_entry_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity,
    uint32 entry, uint32 caption, sint32 palette);

uint32 gtmain_menu_option_detail_column_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity,
    sint32 selected, uint32 resources, uint32 highlight);
uint32 gtmain_menu_option_detail_draw(uint32 ordering, sint32 x, sint32 y, sint32 unused, sint32 intensity);

uint32 gtmain_menu_option_detail_initialize(void);
uint32 gtmain_menu_quick_options_initialize(void);

uint32 gtmain_menu_selector_1c20_initialize(void);
uint32 gtmain_menu_selector_1ca8_initialize(void);
uint32 gtmain_menu_selector_1d4c_initialize(void);
uint32 gtmain_menu_selector_1da8_initialize(void);
uint32 gtmain_menu_selector_1e28_initialize(void);

sint32 gtmain_alternate_trial_mode8_continue(uint8 disabled);
sint32 gtmain_trial_mode4_continue(uint8 disabled);
sint32 gtmain_trial_mode4_course_options(void);
sint32 gtmain_menu_end_title_transition_draw(uint32 ordering, sint32 x, sint32 y, uint32 text,
    sint32 palette, sint32 intensity);

uint32 gtmain_menu_license_name_tick(void);
uint32 gtmain_menu_license_name_draw(uint32 ordering, sint32 x, sint32 y, sint32 unused, sint32 intensity);
uint32 gtmain_menu_license_name_initialize(void);

uint32 gtmain_menu_license_award_tick(void);
uint32 gtmain_menu_license_award_draw(uint32 ordering, sint32 x, sint32 y, sint32 unused, sint32 intensity);
uint32 gtmain_menu_license_award_draw_carried(uint32 ordering, sint32 x, sint32 y, sint32 unused,
    sint32 intensity, sint32 group, uint32 phase);

uint32 gtmain_menu_event_options_tick(void);
uint32 gtmain_menu_event_options_initialize(void);
uint32 gtmain_menu_asset_release(void);
/* TODO Bind original 800408A4 record request through the authorized save boundary */
uint32 gtmain_menu_result_record_request(uint32 check_token, uint32 complete_token);

uint32 gtmain_menu_event_results_tick(void);
uint32 gtmain_menu_event_results_column_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity,
    sint32 selection, uint32 list, uint32 resources);
uint32 gtmain_menu_event_results_draw(uint32 ordering, sint32 x, sint32 y, sint32 unused, sint32 intensity);
uint32 gtmain_menu_event_results_initialize(void);

uint32 sections_read_retry(uint32 path, uint32 destination, uint32 requested_bytes);
uint32 gtmain_resource_entry_decode(uint32 resource, uint32 index, uint32 destination, uint32 unused);
uint32 gtmain_pit_menu_textures_load(void);
sint32 gtmain_pit_menu_run(sint32 state);

uint32 gtmain_menu_effect_fade_begin(void);
uint32 gtmain_menu_text_cells_draw(uint32 ordering, sint32 x, sint32 y, uint32 text,
    sint32 cell_width, sint32 font);
uint32 gtmain_packet_flat_triangle(uint32 ordering, uint32 coordinates, uint32 color);

uint32 gtmain_race_specification_summary(uint32 specification, uint32 values, uint32 detail);
uint32 gtmain_race_engine_curve_summary(uint32 curve, uint32 detail);
uint32 gtmain_race_transmission_summary(uint32 transmission, uint32 curve, uint32 values);

uint32 gtmain_packet_triangle_up(uint32 ordering, sint32 x, sint32 y, uint32 color);
uint32 gtmain_packet_triangle_down(uint32 ordering, sint32 x, sint32 y, uint32 color);
uint32 gtmain_packet_triangle_right(uint32 ordering, sint32 x, sint32 y, uint32 color);
uint32 gtmain_packet_triangle_left(uint32 ordering, sint32 x, sint32 y, uint32 color);

sint32 gtmain_menu_name_field_draw(uint32 ordering, uint32 request, sint32 x, sint32 y, sint32 intensity);

sint32 gtmain_menu_scene_model_draw(uint32 ordering, sint16 x, sint16 y,
    sint16 camera_y, sint16 camera_x, sint16 camera_z,
    sint16 model_y, sint16 model_x, sint16 model_z,
    uint32 position_x, uint32 position_y, uint32 position_z,
    uint32 offset_y, uint32 model, uint32 unused,
    uint16 palette, uint32 command, uint16 page, sint16 projection,
    uint16 scale_x, uint16 scale_y);

typedef uint32 (*GTMenuDrawCallback)(uint32 ordering, sint32 x, sint32 y, sint32 intensity);
uint32 gtmain_menu_draw_callback_invoke(GTMenuDrawCallback callback, uint32 ordering,
    sint32 x, sint32 y, sint32 intensity);
uint32 gtmain_menu_caption_style(sint32 intensity);
uint32 gtmain_menu_one_line_caption_draw(uint32 ordering, sint32 x, sint32 y,
    uint32 text, sint32 intensity);

uint32 gtmain_menu_selection_peak_update(uint32 state, uint32 values, uint32 detail);
uint32 gtmain_menu_parts_parameters_apply(uint32 category);

uint32 gtmain_menu_selection_peak_reset(uint32 state);

uint32 gtmain_parts_rear_stabilizer_apply(void);
uint32 gtmain_parts_clutch_apply(void);

sint32 gtmain_parts_parameter_level(sint32 minimum, sint32 maximum, sint32 value, sint32 count);

sint32 gtmain_parts_parameter_value(sint32 minimum, sint32 maximum, sint32 level, sint32 count);
sint32 gtmain_parts_kind(uint32 category, uint32 selection, uint32 archive);
uint32 gtmain_parts_suspension_options_initialize(void);

uint32 gtmain_parts_front_stabilizer_options_for_category(sint32 category);
uint32 gtmain_parts_rear_stabilizer_options_initialize(void);
uint32 gtmain_parts_brake_options_for_category(sint32 category);
uint32 gtmain_parts_turbo_options_initialize(void);

uint32 gtmain_parts_spring_ranges_initialize(void);
uint32 gtmain_parts_ride_height_ranges_initialize(void);
uint32 gtmain_parts_damper_ranges_initialize(void);
uint32 gtmain_parts_turbo_range_initialize(void);
uint32 gtmain_parts_gear_ranges_initialize(void);

uint32 gtmain_parts_transmission_options_finish(uint32 flags);
uint32 gtmain_parts_transmission_options_initialize(void);
uint32 gtmain_parts_clutch_options_initialize(void);
sint32 gtmain_parts_all_options_initialize(void);

uint32 gtmain_parts_camber_apply(void);
uint32 gtmain_parts_stabilizer_levels_apply(void);

uint32 gtmain_parts_brake_levels_apply(void);
uint32 gtmain_parts_turbo_selection_apply(void);
uint32 gtmain_parts_turbo_value_apply(uint32 context, uint32 value);
uint32 gtmain_parts_gear_values_apply(void);
uint32 gtmain_parts_clutch_values_apply(void);
uint32 gtmain_parts_options_select(sint32 group, sint32 selection);

uint32 gtmain_parts_options_apply(sint32 group, sint32 selection);

sint32 gtmain_parts_field_input(uint32 selector, uint32 input);
sint32 gtmain_parts_settings_bind(void);
uint32 gtmain_parts_icon_draw(uint32 ordering, uint16 x, sint32 y, uint32 icon, uint32 color);
uint32 gtmain_parts_number_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity, sint32 value, sint32 decimals, uint32 suffix);
uint32 gtmain_parts_caption_draw(uint32 ordering, uint32 text, sint32 x, sint32 y, sint32 intensity);
uint32 gtmain_parts_menu_initialize(void);
uint32 gtmain_parts_category_descriptor(sint32 category);
sint32 gtmain_parts_menu_input(void);
sint32 gtmain_menu_category_input(uint32 selector, uint32 input);
void gtmain_menu_dropdown_reset(uint32 descriptor);
sint32 gtmain_menu_dropdown_input(uint32 descriptor, uint32 input);
void gtmain_menu_category_reset(uint32 selector);
uint32 gtmain_parts_change_initialize(void);
uint32 gtmain_parts_availability_draw(uint32 ordering, sint32 x, sint32 y, uint32 flags);
uint32 gtmain_parts_preview_draw(uint32 ordering, sint32 x, sint32 y, sint32 selection_style, sint32 intensity);
uint32 gtmain_parts_detail_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity, uint32 peaks, sint32 text_intensity, sint32 first_x, sint32 first_y, sint32 second_x, sint32 second_y, uint32 first_flags, uint32 second_flags);
uint32 gtmain_parts_gearing_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity, sint32 selection, sint32 graph_x, sint32 graph_y);
uint32 gtmain_parts_fields_draw(uint32 ordering, uint32 selector, sint32 x, sint32 y, sint32 selection_style, sint32 intensity);
void gtmain_parts_field_selector_reset(uint32 selector);
uint32 gtmain_parts_preview_initialize(void);
uint32 gtmain_parts_overlay_draw(uint32 ordering, sint32 x, sint32 y, sint32 intensity);
uint32 gtmain_parts_field_draw(uint32 ordering, uint32 field, sint32 x, sint32 y, sint32 intensity, uint32 selected);
uint32 gtmain_parts_graph_line(uint32 ordering, sint32 x, sint32 y, sint32 first_x, sint32 first_y, sint32 second_x, sint32 second_y, uint32 scale, sint32 palette, sint32 intensity);
uint32 gtmain_race_gearing_summary(uint32 specification, uint32 values);
void gtmain_parts_graph_curve(uint32 ordering, sint32 x, sint32 y, sint32 intensity, sint32 segments, uint32 x_values, uint32 y_values, uint32 scale, sint32 palette);
uint32 gtmain_parts_peak_label(uint32 ordering, sint32 x, sint32 y, sint32 intensity, uint32 rpm, uint32 value_text, uint32 unit, sint32 palette, uint32 title);
uint32 gtmain_saved_setting_restore(uint32 data, sint32 index);
uint32 gtmain_settings_checksum_publish(uint32 data);
sint32 gtmain_settings_checksum_validate(uint32 data);
uint32 gtmain_parts_category_names_update(sint32 group);
uint32 gtmain_parts_all_names_update(void);
uint32 gtmain_parts_record_matches_car(uint32 car, uint32 category, uint32 selection, uint32 archive);
uint32 gtmain_parts_inventory_flags(uint32 inventory, uint32 category);
uint32 gtmain_parts_available_build(uint32 descriptors, uint32 entries, sint32 count, uint32 category);
uint32 gtmain_parts_available_build_context(uint32 descriptors, uint32 entries, sint32 count, uint32 category, uint32 context);
uint32 gtmain_menu_settings_mask_context(uint32 table, sint32 count, uint32 context);
uint32 gtmain_race_settings_triplet_set(uint32 preset);
void gtmain_race_controls_hud_clear(uint32 car);
sint32 gtmain_race_route_fraction(sint32 time, sint32 *mode);
sint32 gtmain_race_ai_mode3(uint32 mode);
sint32 gtmain_race_ai_mode4(uint32 mode);
uint32 gtmain_menu_dropdown_row_style(uint32 flags, sint32 intensity);
uint32 gtmain_menu_dropdown_pair_row(uint32 ordering, uint32 record, sint32 x, sint32 y, uint32 border, sint32 intensity);
uint32 gtmain_menu_dropdown_number_row(uint32 ordering, uint32 record, sint32 x, sint32 y, uint32 border, sint32 intensity);
uint32 gtmain_menu_dropdown_list_row(uint32 ordering, uint32 record, sint32 x, sint32 y, uint32 border, sint32 intensity);
uint32 gtmain_menu_dropdown_text_row(uint32 ordering, uint32 record, sint32 x, sint32 y, uint32 border);
uint32 gtmain_menu_dropdown_draw(uint32 ordering, uint32 selector, sint32 x, sint32 y, sint32 selection_style, sint32 intensity, uint32 active);
uint32 gtmain_menu_category_draw(uint32 ordering, uint32 selector, sint32 x, sint32 y, sint32 selection_style, sint32 intensity, uint32 active);
uint32 gtmain_pad_events_take(void);
uint32 gtmain_runtime_finalizers(void);
uint32 gtmain_game_exit(sint32 status);
/* TODO Bind guest finalizer tokens to native void callbacks */
void gtmain_runtime_finalizer_invoke(uint32 callback);
/* TODO Bind BIOS A0:06 Exit at the native integration boundary */
uint32 gtmain_platform_exit(sint32 status);
sint32 gtmenu_record_count(void);
uint32 gtmenu_record_address(sint32 index);
sint32 gtmenu_record_counters_decrement(void);
sint32 gtmenu_record_counters_increment(void);
sint32 gtmenu_angle_wrap(sint32 angle);
sint32 gtmenu_angle_difference(sint32 first, sint32 second);
uint32 gtmenu_cursor_snap(uint32 index);
uint32 gtmenu_cursor_target(uint32 index);
sint32 gtmenu_cursor_position(sint32 x, sint32 y);
sint32 gtmenu_cursor_nearest(sint32 x, sint32 y);
uint32 gtmenu_cursor_step(void);
void gtmenu_list_initialize(uint32 list);
void gtmenu_list_clear(uint32 list);
uint32 gtmenu_list_append(uint32 list, uint32 node);
uint32 gtmenu_list_node_allocate(void);
/* Pending original GTMENU callees at 8004526C, 800452F8 and 800453A8 */
uint32 gtmenu_heap_free(uint32 allocation);
uint32 gtmenu_heap_alloc(uint32 bytes);
uint32 gtmenu_memory_fill(uint32 destination, uint8 value, uint32 bytes);
sint32 gtmenu_cursor_move(uint32 direction);
/* Pending original GTMENU 80031294 */
uint32 gtmenu_square_root(uint32 value, uint32 fractional_bits);
uint32 gtmenu_resource_group(uint32 index);
sint32 gtmenu_catalog_record_count(void);
uint32 gtmenu_catalog_record_address(sint32 index);
sint32 gtmenu_catalog_nodes_build(void);
sint32 gtmenu_record_nodes_build(uint8 kind);
uint32 gtmenu_offer_nodes_build(sint8 group, uint8 kind);
uint32 gtmenu_model_nodes_build(sint8 make, uint8 kind, sint8 group, uint32 flags);
sint32 gtmenu_offer_period(void);
sint32 gtmenu_days_get(void);
uint32 gtmenu_offer_archive_bind(void);
uint32 gtmenu_offer_period_table(sint32 period);
uint32 gtmenu_offer_group_table(sint8 group, uint32 offers);
/* Pending original GTMENU 80031380 */
sint32 gtmenu_model_name_index(uint32 model);
uint32 gtmenu_car_nodes_build(sint8 make, uint32 kind);
uint32 gtmenu_list_node_snapshot(uint32 list, uint32 index);
uint32 gtmenu_primary_list(void);
void gtmenu_days_set(uint32 days);
uint32 gtmenu_days_add(uint32 amount);
void gtmenu_lists_initialize(void);
sint32 gtmenu_current_record_index(void);
uint32 gtmenu_part_nodes_build(uint32 group);
/* Pending original GTMENU 8004544C */
uint32 gtmenu_memory_copy(uint32 destination, uint32 source, uint32 bytes);
void gtmenu_current_record_set(sint16 index);
void gtmenu_credits_set(uint32 credits);
uint32 gtmenu_player_state_initialize(void);
void gtmenu_lists_clear(void);
uint32 gtmenu_part_list_build(uint32 group, uint32 append);
uint32 gtmenu_resource_archive(void);
sint32 gtmenu_garage_add(uint32 node, uint8 color);
sint32 gtmenu_garage_remove(sint32 index);
uint32 gtmenu_garage_price(sint32 index);
uint32 gtmenu_car_list_rebuild(sint8 make, sint8 kind);
void gtmenu_list_build_finish(void);
uint32 gtmenu_car_detail_open(uint32 index);
uint32 gtmenu_car_category_open(uint32 state);
/* Pending original GTMENU 80031500 */
sint32 gtmenu_record_template_index(uint32 model, uint32 archive, uint32 mode);
uint32 gtmenu_parts_mark_owned(uint32 record);
/* Pending original GTMENU 80010E54, 800123BC, 80016A20 and 80016EB8 */
void gtmenu_list_build_hook(void);
uint32 gtmenu_sound_effect(sint32 effect);
uint32 gtmenu_dealer_detail_open(uint32 node, uint32 state);
uint32 gtmenu_garage_detail_open(uint32 index, uint32 state);
/* Pending original GTMENU 80017298 and 800173AC */
uint32 gtmenu_dealer_list_open(sint8 make, sint8 kind);
uint32 gtmenu_garage_list_open(sint8 make, sint8 kind);
uint32 gtmenu_sound_settings_apply(void);
uint32 gtmenu_sound_workspaces_clear(void);
uint32 gtmenu_sample_play(uint32 sample, uint32 handle, sint32 note, sint32 volume, sint32 pan);
uint32 gtmenu_sound_music_volume_set(uint16 volume);
/* Pending original GTMENU 8002FF7C, 8002FFCC and 8003004C */
uint32 gtmenu_voice_pan(uint32 parameters, sint32 volume, sint32 pan);
uint32 gtmenu_voice_pitch(uint32 note, uint32 reference);
uint32 gtmenu_voice_start(uint32 handle, uint32 parameters);
sint32 gtmenu_voice_allocate(uint32 priority);
void gtmenu_voice_configure(uint32 index, uint32 parameters);
uint32 gtmenu_voice_stop(uint32 handle);
uint32 gtmenu_voice_detach(uint32 handle);
uint32 gtmenu_voice_pitch_set(uint32 handle, uint16 pitch);
uint32 gtmenu_voice_volume_set(uint32 handle, uint32 parameters);
uint32 gtmenu_sound_bank_load(uint32 name);
uint32 gtmenu_music_select(sint32 track);
uint32 gtmenu_music_start(void);
uint32 gtmenu_music_stop(void);
uint32 gtmenu_music_fade_in(sint32 milliseconds);
uint32 gtmenu_sound_tick_register(void);
void gtmenu_sound_tick(void);
/* Pending original GTMENU 80017590, 8002F84C and 8002F93C */
uint32 gtmenu_menu_asset_load(uint32 name);
uint32 gtmenu_sound_bank_relocate(uint32 archive, uint32 sample_address);
void gtmenu_sound_samples_upload(uint32 sample_address, uint32 data, uint32 bytes);
/* Pending original GTMENU 80017668, 8002F98C and 8002FAD8 */
uint32 gtmenu_music_resource(uint32 name);
uint32 gtmenu_sequence_relocate(uint32 sequence);
uint32 gtmenu_sequence_initialize(uint32 sequence, uint32 data, uint32 bank);
/* Pending original GTMENU 8002FCE4, 8002FE54 and 8002FEDC */
uint32 gtmenu_sequence_start(uint32 sequence);
uint32 gtmenu_sequence_fade(uint32 sequence, uint16 target, sint32 milliseconds);
uint32 gtmenu_sequence_stop(uint32 sequence);
/* Frame callback registration and sound updates */
uint32 gtmenu_frame_callback_insert(uint32 entry);
void gtmenu_voices_update(void);
void gtmenu_voices_initialize(void);
uint32 gtmenu_voices_commit(void);
void gtmenu_spu_initialize(void);
void gtmenu_sequence_note_start(uint32 sequence, uint32 track, sint32 note, uint32 velocity, uint32 delay);
void gtmenu_sequences_update(void);
void gtmenu_sequence_tracks_update(uint32 sequence);
sint32 gtmenu_sequence_fade_step(uint32 state);
void gtmenu_sequence_notes_update(void);
void gtmenu_sequences_initialize(void);
uint32 gtmenu_reverb_preset_set(uint8 preset, uint32 delay, sint32 feedback);
uint32 gtmenu_reverb_enable(uint32 enabled);
uint32 gtmenu_reverb_depth_set(sint16 left, sint16 right);
void gtmenu_voices_shutdown(void);
uint32 gtmenu_voices_stop_all(void);
uint32 gtmenu_sequence_unlink(uint32 sequence);
uint32 gtmenu_sequence_link(uint32 sequence);
sint32 gtmenu_sequence_fade_initialize(uint32 state, uint32 current, uint32 target, sint32 milliseconds);
sint32 gtmenu_sound_master_fade(uint16 target, sint32 milliseconds);
sint32 gtmenu_car_record_find(uint32 name, uint32 archive, uint32 section, uint32 require_alternate);
sint32 gtmenu_car_record_find_variant(uint32 name, uint32 archive, uint32 section);
uint32 gtmenu_car_config_copy(uint32 destination, uint32 source);
uint32 gtmenu_car_config_fields(uint32 destination, uint32 source, uint32 descriptors, uint32 archive);
sint32 gtmenu_range_index(sint32 minimum, sint32 maximum, sint32 value, sint32 count);
uint32 gtmenu_archive_record(uint32 section, uint32 id, uint32 archive);
sint32 gtmenu_archive_record_matches(sint32 value, uint32 section, uint32 id, uint32 archive);
sint32 gtmenu_archive_record_flag(uint32 section, uint32 id, uint32 archive);
uint32 gtmenu_archive_record_string(uint32 section, uint32 id, uint32 archive);
uint32 gtmenu_packed_text_index_initialize(uint32 table);
uint32 gtmenu_line_packet(uint32 ordering, uint32 vertices, uint32 color);
uint32 gtmenu_texture_state_upload(uint32 state);
void gtmenu_spu_shutdown(void);
void gtmenu_sequence_rate_set(uint16 rate);
uint32 gtmenu_voice_rate_set(sint32 rate);
/* Pending original GTMENU 8002FC58 */
uint32 gtmenu_sequence_volume_apply(uint32 sequence);
uint32 gtmenu_credits_get(void);
uint32 gtmenu_license_results(sint32 group);
uint32 gtmenu_license_grade(sint32 group);
uint32 gtmenu_license_tests_complete(sint32 group);
uint32 gtmenu_license_entry_unlocked(sint32 entry);
uint32 gtmenu_progress_byte(uint32 index);
void gtmenu_progress_selection_set(uint16 selection);
sint32 gtmenu_save_record_ids_decode(uint32 data);
sint32 gtmenu_save_payload_import(uint32 data);
uint32 gtmenu_save_payload_export(uint32 data);
uint32 gtmenu_save_payload_valid(uint32 data);
uint32 gtmenu_loaded_payload_apply(uint32 data);
uint32 gtmenu_loaded_game_apply(void);
/* Native single-save replacement, outside original-body accounting */
sint32 gtmenu_native_save_action(uint32 writing);
void gtmenu_resource_archives_clear(void);
uint32 gtmenu_resource_archive_load(void);
uint32 gtmenu_resource_archive_release(uint32 archive);
/* Shared original checksum body retained in batch_1.c */
uint32 gtend_data_checksum(uint32 address, uint32 size);
/* Pending original GTMENU 80031590 */
uint32 gtmenu_parts_archive_acquire(uint32 mode);
uint32 gtmenu_model_index_by_name(uint32 name);
uint32 gtmenu_model_from_record_id(uint32 id);
uint32 gtmenu_record_has_parts(uint32 record);
uint32 gtmenu_record_matches_filter(uint32 record, uint32 kind, sint32 value);
uint32 gtmenu_engine_curve_build(uint32 model, uint32 curve, sint32 scale);
/* Pending original GTMENU 80045588 guest-string comparison */
sint32 gtmenu_string_compare(uint32 first, uint32 second);
uint32 gtmenu_installed_parts_value(uint32 record);
uint32 gtmenu_record_matches_make(uint32 record, sint32 make_index);
uint32 gtmenu_catalog_price(sint32 index);
uint32 gtmenu_catalog_choice_prepare(void);
uint32 gtmenu_record_display_name(uint32 record, uint32 destination);
uint32 gtmenu_catalog_purchase_commit(void);
/* Pending original GTMENU 800104A4 */
sint64 gtmenu_signed_divide64(sint64 numerator, sint64 denominator, uint32 caller_link);
uint32 gtmenu_transmission_build(uint32 specification, uint32 transmission, uint32 curve);
uint32 gtmenu_transmission_summary(uint32 transmission, uint32 curve, uint32 values);
uint32 gtmenu_specification_gears(uint32 specification, uint32 values);
uint32 gtmenu_engine_specification_summary(uint32 specification, uint32 detail);
uint32 gtmenu_engine_curve_initialize(uint32 specification, uint32 curve, uint32 enabled);
uint32 gtmenu_engine_curve_summary(uint32 curve, uint32 detail);
sint32 gtmenu_curve_evaluate(uint32 curve, sint32 position);
uint32 gtmenu_engine_boost_initialize(uint32 specification, uint32 curve, uint32 enabled);
sint32 gtmenu_engine_boost_curve(uint32 curve, sint16 first, sint16 second);
sint32 gtmenu_engine_boost_limit(uint32 curve, sint16 first, sint16 second);
uint32 gtmenu_catalog_parts_mark_owned(uint32 record);
uint32 gtmenu_owned_parts_category(uint32 inventory, uint32 category);
uint32 gtmenu_catalog_caption_draw(uint32 ordering);
uint32 gtmenu_catalog_input(uint32 event);
uint32 gtmenu_catalog_list_create(uint32 output, uint32 rectangle, uint32 preserve_position);
void gtmenu_ui_active_list_set(uint32 node, uint32 secondary);
uint32 gtmenu_ui_active_list(uint32 secondary);
/* GTMENU text and rectangle interfaces */
sint32 gtmenu_text_width(uint32 font, uint32 text);
uint32 gtmenu_text_color_set(uint32 rgb);
uint32 gtmenu_text_font_set(sint32 font);
uint32 gtmenu_text_draw(uint32 ordering, sint32 x, sint16 y, uint32 text);
uint32 gtmenu_number_group(uint32 destination, uint32 source);
uint32 gtmenu_rectangle_draw(uint32 ordering, uint32 rectangle, uint32 color);
/* Pending native ABI adapter for original GTMENU 80045698 and its %Ld format */
uint32 gtmenu_format_signed64(uint32 destination, uint32 format, sint64 value);
/* Pending original GTMENU 80014A50 and 80014920 */
uint32 gtmenu_menu_back(void);
uint32 gtmenu_menu_open(uint32 state);
uint32 gtmenu_ui_list_create(uint32 rectangle, uint32 list, uint8 kind);
uint32 gtmenu_catalog_selection_draw(uint32 ordering, uint32 selection);
uint32 gtmenu_catalog_buttons_draw(uint32 ordering);
uint32 gtmenu_catalog_footer_draw(uint32 ordering);
uint32 gtmenu_loading_banner_draw(uint32 ordering);
uint32 gtmenu_catalog_mode_draw(uint32 ordering);
uint32 gtmenu_catalog_draw_event(uint32 event);
uint32 gtmenu_catalog_selection_move(uint32 delta);
uint32 gtmenu_rectangle_pulse(uint32 ordering, uint32 rectangle, uint32 animation);
uint32 gtmenu_rectangle_pulse_reset(void);
uint32 gtmenu_render_auxiliary_reset(void);
uint32 gtmenu_render_reset(void);
uint32 gtmenu_render_mode_set(uint8 mode);
uint32 gtmenu_renderer_reset(void);
/* Pending GTMENU 8004AA20 soft-reset boundary */
void gtmenu_gpu_graph_reset(sint32 mode);
uint32 gtmenu_render_environments_bind(void);
uint32 gtmenu_render_display_apply(uint32 alternate);
uint32 gtmenu_render_drawing_apply(uint32 alternate);
uint32 gtmenu_text_surface_reset(void);
uint32 gtmenu_text_surface_initialize(uint32 rectangle, uint32 clut_position);
uint32 gtmenu_text_surface_state_clear(uint32 state);
uint32 gtmenu_text_surface_bounds_set(uint32 state, uint32 rectangle);
/* Pending GTMENU 8004B048 boundary to existing LoadImage API */
uint32 gtmenu_gpu_load_image(uint32 rectangle, uint32 data);
/* Pending GTMENU 8004AF20 boundary to existing ClearImage API */
uint32 gtmenu_gpu_clear_rectangle(uint32 rectangle, uint8 red, uint8 green, uint8 blue);
uint32 gtmenu_rectangle_shade(uint32 ordering, uint32 rectangle, uint8 intensity);
uint32 gtmenu_text_centered(uint32 ordering, uint32 rectangle, uint32 color, uint32 text);
uint32 gtmenu_draw_area(uint32 ordering, uint32 offset, uint32 first, uint32 last);
uint32 gtmenu_scene_draw(uint32 ordering);
uint32 gtmenu_texture_page_draw(uint32 ordering, uint32 mode);
typedef uint32 (*GTMenuMoveCallback)(uint32 delta);
typedef uint32 (*GTMenuActionCallback)(void);
uint32 gtmenu_input_dispatch(uint32 event, GTMenuMoveCallback move,
    GTMenuActionCallback accept, GTMenuActionCallback cancel);
uint32 gtmenu_input_acknowledge(uint32 event, GTMenuActionCallback action);
uint32 gtmenu_catalog_purchase_accept(void);
uint32 gtmenu_catalog_purchase_cancel(void);
uint32 gtmenu_catalog_return_accept(void);
uint32 gtmenu_catalog_message_input(uint32 event);
uint32 gtmenu_part_stage(sint16 index);
sint32 gtmenu_part_install(uint32 node);
uint32 gtmenu_part_adjustment_value(uint32 node);
uint32 gtmenu_category_name(sint32 category);
uint32 gtmenu_category_kind(sint32 category);
uint32 gtmenu_category_style(sint32 category);
void gtmenu_part_settings_apply(uint32 record, uint32 category, uint32 data);
sint32 gtmenu_selected_color(void);
uint32 gtmenu_part_caption(uint32 node, uint32 short_caption);
uint32 gtmenu_record_special_parts_mark(uint32 record);
uint32 gtmenu_record_is_special_model(uint32 record);
uint32 gtmenu_packed_text_entry(uint32 table, sint32 index);
/* Pending guest-string boundaries at GTMENU 800456C8 and 80045698 */
uint32 gtmenu_string_copy(uint32 destination, uint32 source);
uint32 gtmenu_format_strings2(uint32 destination, uint32 format, uint32 first, uint32 second);
uint32 gtmenu_format_strings3(uint32 destination, uint32 format, uint32 first, uint32 second, uint32 third);
sint32 gtmenu_page_category_set(uint32 page);
uint32 gtmenu_model_info_page_open(sint32 category);
/* Pending original GTMENU 80045754 and 8001BD48 */
sint32 gtmenu_string_compare_folded(uint32 first, uint32 second);
uint32 gtmenu_page_open(uint32 page);
uint32 gtmenu_model_info_draw(void);
uint32 gtmenu_model_info_close(void);
uint32 gtmenu_model_info_callback_restore(void);
uint32 gtmenu_current_ordering(void);
sint32 gtmenu_model_info_selection(void);
sint32 gtmenu_model_info_count(void);
uint32 gtmenu_price_draw(uint32 ordering, sint32 x, sint16 y, uint32 alignment, sint64 price);
sint32 gtmenu_power_display_value(uint32 value);
sint32 gtmenu_mass_display_value(sint32 value);
sint32 gtmenu_torque_display_value(sint32 value);
sint32 gtmenu_length_display_value(sint32 value);
sint32 gtmenu_displacement_display_value(sint32 value);
uint32 gtmenu_model_specifications_draw(uint32 unused_first, uint32 model);
uint32 gtmenu_model_detail_frame_draw(void);
uint32 gtmenu_model_detail_activate(void);
uint32 gtmenu_model_detail_event(uint32 event);
uint32 gtmenu_model_logo_reset(void);
uint32 gtmenu_current_car_logo_load(void);
uint32 gtmenu_textured_rectangle_packet(uint32 ordering, uint32 position, uint32 uv_clut, uint32 size, uint32 color);
uint32 gtmenu_logo_caption_draw(uint32 ordering, sint16 x, sint16 y, sint32 code);
uint32 gtmenu_model_info_actions_event(uint32 event);
uint32 gtmenu_page_back_target(void);
uint32 gtmenu_ui_list_has_entries(uint32 secondary);
uint32 gtmenu_page_transition_prepare(uint32 page);
uint32 gtmenu_page_timed_action(void);
uint32 gtmenu_format_string(uint32 destination, uint32 format, uint32 text);
uint32 gtmenu_format_integer_string_integer(uint32 destination, uint32 format, sint32 value, uint32 suffix, uint32 final_value);
uint32 gtmenu_format_integers3(uint32 destination, uint32 format, sint32 first, sint32 second, sint32 third);
uint32 gtmenu_format_integer_string_char_integer(uint32 destination, uint32 format, sint32 value, uint32 suffix, sint32 character, uint32 final_value);
uint32 gtmenu_model_preview_draw(uint32 ordering, sint32 x, sint32 y);
/* Pending guest formatter boundary with one signed integer and one guest string */
uint32 gtmenu_format_integer_string(uint32 destination, uint32 format, sint32 value, uint32 suffix);
uint32 gtmenu_model_color_count(uint32 index);
uint32 gtmenu_color_table_entry(uint32 table, uint32 index);
uint32 gtmenu_model_info_activate(void);
uint32 gtmenu_model_info_event(uint32 event);
uint32 gtmenu_model_info_position_set(uint32 position);
uint32 gtmenu_model_info_frame_draw(uint32 ordering);
uint32 gtmenu_model_info_change_available(uint32 model);
uint32 gtmenu_color_picker_bounds_set(uint32 rectangle);
uint32 gtmenu_model_has_more_info(uint32 model);
/* Pending original GTMENU model-info callees */
void gtmenu_model_color_next(void);
uint32 gtmenu_model_more_open(uint32 record, sint16 category, sint8 mode);
uint32 gtmenu_model_more_restore(void);
uint32 gtmenu_model_more_activate(void);
uint32 gtmenu_model_more_draw(uint32 ordering);
uint32 gtmenu_model_more_event(uint32 event);
uint32 gtmenu_confirmation_restore(void);
uint32 gtmenu_confirmation_draw(void);
uint32 gtmenu_confirmation_accept(void);
uint32 gtmenu_confirmation_cancel(void);
/* Pending original GTMENU 80020688 */
uint32 gtmenu_confirmation_activate(void);
uint32 gtmenu_confirmation_event(uint32 event);
uint32 gtmenu_confirmation_open(uint32 kind, uint32 text, sint16 target, sint8 style);
uint32 gtmenu_special_model_action(uint32 action);
uint32 gtmenu_model_code_action(uint32 action);
uint32 gtmenu_progress_three_complete(void);
uint32 gtmenu_event_license_check(sint32 kind);
uint32 gtmenu_special_event_check(uint32 kind);
uint32 gtmenu_entry_unavailable_prompt(void);
uint32 gtmenu_championship_entry_check(uint32 kind);
sint32 gtmenu_race_action(uint32 action);
/* Pending original GTMENU 800246CC */
uint32 gtmenu_input_state_address(uint32 secondary);
uint32 gtmenu_race_request(uint32 kind, uint32 option, uint32 first_record, uint32 second_record, uint32 selection);
void gtmenu_input_prefix_clear(uint32 state);
uint32 gtmenu_controller_repeat_initialize(uint32 state);
uint32 gtmenu_pointer_state_initialize(uint32 state);
sint32 gtmenu_resource_names_find(uint32 table, uint32 name);
uint32 gtmenu_link_store(uint32 index, uint32 name, uint32 value, uint32 upper);
sint32 gtmenu_link_find(uint32 output, uint32 name);
/* Pending original GTMENU 80012504, 80026A2C and 8002712C */
uint32 gtmenu_music_request(sint32 track);
uint32 gtmenu_current_car_caption_open(uint32 category);
void gtmenu_current_car_mileage_open(void);
uint32 gtmenu_debug_integer(uint32 format, sint32 value);
uint32 gtmenu_proposed_power(void);
uint32 gtmenu_hud_current_car_draw(uint32 ordering);
uint32 gtmenu_hud_credits_draw(uint32 ordering);
uint32 gtmenu_hud_days_draw(uint32 ordering);
uint32 gtmenu_model_spec_open(uint32 record, uint32 settings, uint32 stats, sint16 category, sint8 mode);
uint32 gtmenu_model_change_prepare(void);
uint32 gtmenu_model_purchase_open(uint32 mode, uint32 workspace, sint16 category, sint8 flags);
uint32 gtmenu_ui_default_activate(void);
/* Pending original GTMENU 80019C84, 80014ACC and 80028A4C */
uint32 gtmenu_versus_open(void);
uint32 gtmenu_trade_open(void);
uint32 gtmenu_race_info_open(sint16 category);
/* Pending guest-string diagnostic adapter for GTMENU 8004F7C0 */
uint32 gtmenu_debug_message(uint32 text);
uint32 gtmenu_model_info_angle_add(uint32 delta);
uint32 gtmenu_model_info_open_node(uint32 node, sint32 category);
uint32 gtmenu_model_info_open_id(uint16 id, sint32 category);
void gtmenu_model_info_selection_set(sint16 selection);
uint32 gtmenu_model_color_table(uint32 index);
sint32 gtmenu_model_color_index(uint32 code, uint32 table);
sint32 gtmenu_model_color_code(uint32 index, uint32 table);
uint32 gtmenu_model_color_name(uint32 id, sint8 code, uint32 archive);
uint32 gtmenu_settings_from_record(uint32 settings, uint32 record, uint32 archive);
uint32 gtmenu_model_stats_build(uint32 settings, uint32 output, uint32 stats);
uint32 gtmenu_model_info_bind(uint32 node, uint32 first, uint32 second, uint32 mode, uint32 extra);
void gtmenu_preview_control_set(uint32 mode);
uint32 gtmenu_model_preview_load(uint32 model);
uint32 gtmenu_model_logo_load(uint32 model);
uint32 gtmenu_current_car_preview_bind(uint32 rectangle);
uint32 gtmenu_installed_parts_draw(uint32 record);
/* The third original argument is forwarded to an unused loader argument */
uint32 gtmenu_model_preview_bind(uint32 node, uint32 mode, uint32 unused_extra);
uint32 gtmenu_model_info_open_garage(sint32 index, sint16 category);
uint32 gtmenu_model_purchase_commit(void);
uint32 gtmenu_garage_widget_create(uint32 output, uint32 rectangle);
uint32 gtmenu_ui_child_list_event(uint32 event, uint32 owner);
/* Pending original GTMENU 8001A7C8 */
uint32 gtmenu_ui_selection_mode_set(uint32 mode);
uint32 gtmenu_ui_selection_restore(void);
uint32 gtmenu_ui_list_event_dispatch(uint32 event, uint32 node);
uint32 gtmenu_current_car_widget_create(uint32 output, uint32 rectangle);
uint32 gtmenu_ui_selection_set(uint32 secondary, sint16 selection);
uint32 gtmenu_menu_texture_load(uint32 name, uint32 clut_position, uint32 clut_size,
    uint32 pixel_position, uint32 pixel_size, uint32 pixels_first);
uint32 gtmenu_music_resources_initialize(void);
sint32 gtmenu_resource_names_index(uint32 index, uint32 text);
/* Pending original GTMENU 80032244 */
uint32 gtmenu_tim_upload(uint32 image, uint32 clut_position, uint32 clut_size,
    uint32 pixel_position, uint32 pixel_size, uint32 pixels_first);
uint32 gtmenu_tim_block_upload(uint32 block, uint32 position, uint32 size);
uint32 gtmenu_page_resources_initialize(void);
uint32 gtmenu_system_textures_load(void);
uint32 gtmenu_pointer_pulse_draw(uint32 ordering, uint16 x, uint16 y);
void gtmenu_secondary_input_enable(uint32 enabled);
uint32 gtmenu_controller_is_mouse(uint32 port);
uint32 gtmenu_input_delay_reset(void);
uint32 gtmenu_controller_packet(uint32 port);
uint32 gtmenu_controller_state_update(uint32 state, uint32 packet);
uint32 gtmenu_controller_event_dispatch(uint32 state, uint32 secondary);
uint32 gtmenu_pointer_hover_update(void);
uint32 gtmenu_pointer_widget_at(sint16 x, sint16 y);
uint32 gtmenu_pointer_frame_draw(uint32 ordering, sint16 x, sint16 y, uint32 pointer, uint32 enabled);
uint32 gtmenu_pointer_history_store(uint32 x, uint32 y, uint32 alternate);
uint32 gtmenu_pointer_visibility_set(uint32 enabled, uint32 preserve);
/* TODO Bind GTMENU guest callback tokens during integration */
uint32 gtmenu_event_callback_invoke(uint32 callback, uint32 event);
uint32 gtmenu_page_command_set(uint32 command);
void gtmenu_page_command_clear(void);
uint32 gtmenu_page_style_reset(void);
uint32 gtmenu_layout_nodes_process(uint32 ordering, uint32 list);
uint32 gtmenu_layout_text(uint32 ordering, uint32 node);
/* Pending original GTMENU text layout helpers */
uint32 gtmenu_layout_vertical_align(void);
uint32 gtmenu_layout_horizontal_align(sint32 extent);
uint32 gtmenu_layout_glyph_outline(uint16 code, uint32 ordering, uint32 size, uint32 shade);
uint32 gtmenu_layout_glyph_draw(uint16 code, uint32 ordering, uint32 position, uint32 size, uint32 shade);
uint32 gtmenu_font_cache_acquire(uint32 context, uint16 code, uint32 x_out, uint32 y_out);
uint32 gtmenu_font_bitmap_unpack(uint32 pixels, const uint8 *bitmap, uint32 colors, sint32 pitch);
uint32 gtmenu_font_bitmap_resample12(uint32 pixels, const uint8 *bitmap, sint32 pitch);
uint32 gtmenu_layout_float_expire(void);
uint32 gtmenu_layout_underline_draw(uint32 ordering, uint32 position, uint32 size);
uint32 gtmenu_layout_link_region_add(uint32 position, uint32 size, uint32 reference);
uint32 gtmenu_link_region_create(void);
uint32 gtmenu_link_region_append(uint32 region);
uint32 gtmenu_layout_image(uint32 ordering, uint32 widget);
uint32 gtmenu_named_action_find(uint32 name);
/* Pending original GTMENU image upload and packet helpers */
uint32 gtmenu_bs_image_upload(uint32 name, uint32 rectangle);
uint32 gtmenu_bs_words_unpack(uint32 input, uint32 output);
void gtmenu_bs_input_complete(void);
uint32 gtmenu_layout_color_fill(uint32 ordering, uint32 color, uint32 position, uint32 size);
uint32 gtmenu_layout_tim_prepare(uint32 descriptor, uint32 widget, uint32 clut_position, uint32 pixel_position);
uint32 gtmenu_layout_image_sprite_draw(uint32 descriptor, uint32 ordering, uint32 widget);
uint32 gtmenu_layout_link(uint32 node);
uint32 gtmenu_layout_table(uint32 ordering, uint32 node);
uint32 gtmenu_layout_table_measure(uint32 node);
uint32 gtmenu_layout_list_measure(uint32 result, uint32 node);
uint32 gtmenu_layout_text_measure(uint32 result, uint32 position, uint32 text);
uint32 gtmenu_layout_tag_measure(uint32 result, uint32 position, uint8 code);
uint32 gtmenu_polygon_draw(uint32 ordering, uint32 vertices, sint32 count, uint32 color);
uint32 gtmenu_layout_table_border_draw(uint32 ordering, uint32 node);
uint32 gtmenu_layout_rule(uint32 ordering, uint32 widget);
uint32 gtmenu_layout_tag(uint32 widget);
uint32 gtmenu_background_tile_draw(uint32 ordering, uint32 tile, uint32 position, uint32 size);
uint32 gtmenu_background_texture_initialize(uint32 image, uint32 clut, uint32 pixels);
uint32 gtmenu_background_mode_set(uint8 mode);
uint32 gtmenu_archive_image_upload(uint32 archive, sint32 index, uint16 x, uint16 y, uint16 width, uint16 height);
uint32 gtmenu_background_archive_load(uint32 name, uint32 clut, uint32 pixels);
uint32 gtmenu_background_texture_load(uint32 name, uint32 clut, uint32 pixels);
uint32 gtmenu_background_draw(uint32 vertical_offset);
uint32 gtmenu_background_ordering(void);
uint32 gtmenu_navigation_owners_release(void);
void gtmenu_navigation_owner_release(uint32 owner);
uint32 gtmenu_list_create(void);
uint32 gtmenu_page_action_parse(uint32 input);
uint32 gtmenu_page_number_skip(uint32 input);
uint32 gtmenu_stream_u16_read(uint32 input, uint32 output);
uint32 gtmenu_stream_size_read(uint32 input, uint32 output);
uint32 gtmenu_stream_string_read(uint32 input, uint32 output, uint32 length);
uint32 gtmenu_string_copy_bounded(uint32 output, uint32 text, uint32 count);
uint32 gtmenu_layout_mode_record(uint32 name, uint32 mode, uint32 bounds);
uint32 gtmenu_layout_mode_append(uint32 owner, uint32 record);
uint32 gtmenu_packed_rectangle_contains(uint32 position, uint32 size, sint16 x, sint16 y);
uint32 gtmenu_layout_mode_contains(uint32 record, uint32 origin, sint16 x, sint16 y);
uint32 gtmenu_page_body_parse(uint32 list, uint32 input);
uint32 gtmenu_page_break_parse(uint32 list, uint32 input);
uint32 gtmenu_page_preformatted_parse(uint32 input);
uint32 gtmenu_page_optional_skip(uint32 input);
uint32 gtmenu_page_flag_skip(uint32 input);
uint32 gtmenu_page_attribute_number_skip(uint32 input);
uint32 gtmenu_page_object_skip(uint32 input);
uint32 gtmenu_page_table_counts_skip(uint32 input);
uint32 gtmenu_page_rule_parse(uint32 list, uint32 input);
uint32 gtmenu_stream_u24_read(uint32 input, uint32 output);
uint32 gtmenu_body_background_image_create(uint32 list, uint32 name);
uint32 gtmenu_body_background_color_create(uint32 color);
uint32 gtmenu_page_rule_create(uint32 list, uint32 flags, sint16 height, sint16 width);
uint32 gtmenu_page_tag_append(uint32 list, uint8 tag);
uint32 gtmenu_widget_alloc(uint32 bytes);
uint32 gtmenu_page_list_append(uint32 list, uint32 node);
uint32 gtmenu_image_format_flags(uint32 name);
/* Pending BIOS A0:24 adapter to the existing native strstr */
uint32 gtmenu_string_find(uint32 text, uint32 needle);
uint32 gtmenu_page_image_parse(uint32 list, uint32 input);
uint32 gtmenu_page_metadata_skip(uint32 input);
uint32 gtmenu_page_pair_skip(uint32 input);
uint32 gtmenu_page_block_mode_parse(uint32 list, uint32 input);
uint32 gtmenu_page_table_parse(uint32 list, uint32 input);
uint32 gtmenu_page_cell_parse(uint32 input);
uint32 gtmenu_page_header_cell_parse(uint32 input);
uint32 gtmenu_page_row_parse(uint32 input);
uint32 gtmenu_page_byte_skip(uint32 input);
uint32 gtmenu_page_action_text_parse(uint32 input);
uint32 gtmenu_page_text_append(uint32 list, uint32 text, uint32 length);
uint32 gtmenu_page_image_create(uint32 list, uint32 flags, uint32 name, uint32 action);
uint32 gtmenu_page_link_create(uint32 list, uint32 color, uint32 target, uint32 name);
uint32 gtmenu_page_link_parse(uint32 list, uint32 input);
uint32 gtmenu_page_block_mode_create(uint32 list, sint32 value);
uint32 gtmenu_table_create(sint16 border, sint16 padding, sint16 spacing, sint16 attributes);
uint32 gtmenu_table_cell_append(uint32 cell);
uint32 gtmenu_table_cell_create(void);
uint32 gtmenu_table_cell_attributes_parse(uint32 input, uint32 cell);
uint32 gtmenu_named_action_create(uint32 name);
uint32 gtmenu_named_action_insert(uint32 entry);
uint32 gtmenu_pointer_delay_reset(void);
uint32 gtmenu_garage_callback_reset(void);
sint32 gtmenu_record_pointer_index(uint32 record);
uint32 gtmenu_sell_price_draw(uint32 ordering);
uint32 gtmenu_used_sell_activate(void);
void gtmenu_used_sell_event(uint32 event);
uint32 gtmenu_used_sell_begin(uint32 previous_page);
uint32 gtmenu_used_sell_commit(void);
uint32 gtmenu_map_return(void);
uint32 gtmenu_car_wash_activate(void);
uint32 gtmenu_car_wash_draw(uint32 ordering);
uint32 gtmenu_car_wash_event(uint32 event);
void gtmenu_car_wash_begin(void);
uint32 gtmenu_text_draw_double(uint32 ordering, uint32 x, sint16 y, uint32 color, uint32 small, uint32 text);
uint16 gtmenu_font_code_encode(uint16 code);
uint32 gtmenu_race_info_return(void);
uint32 gtmenu_race_info_callback_restore(void);
uint32 gtmenu_race_info_activate(void);
uint32 gtmenu_race_info_draw(uint32 ordering);
uint32 gtmenu_race_info_event(uint32 event);
sint32 gtmenu_ui_selection_active(void);
/* Pending original GTMENU pointer VRAM copy helpers */
uint32 gtmenu_pointer_background_capture(uint32 ordering, sint16 x, sint16 y, sint32 field);
uint32 gtmenu_pointer_background_restore(uint32 ordering, sint16 x, sint16 y, sint32 field);
/* Pending original GTMENU glyph quad dependencies */
uint32 gtmenu_font_quad16_draw(uint32 state, uint16 code, uint32 ordering, uint32 position, uint32 size);
uint32 gtmenu_font_quad12_draw(uint32 state, uint16 code, uint32 ordering, uint32 position, uint32 size);
uint32 gtmenu_menu_initialize(void);
uint32 gtmenu_ordering_draw_current(void);
uint32 gtmenu_menu_frame_loop(void);
uint32 gtmenu_menu_state_reset(void);
uint32 gtmenu_page_runtime_release(void);
void gtmenu_hitboxes_disable(void);
uint32 gtmenu_press_button_draw(uint32 ordering);
uint32 gtmenu_letterbox_draw(uint32 ordering);
uint32 gtmenu_frames_flush(void);
/* Pending original GTMENU initialization and frame dependencies */
uint32 gtmenu_resources_initialize(void);
uint32 gtmenu_input_initialize(void);
uint32 gtmenu_navigation_initialize(void);
uint32 gtmenu_links_initialize(void);
uint32 gtmenu_initial_page_open(uint32 state);
uint32 gtmenu_ordering_draw(uint32 ordering);
uint32 gtmenu_frame_prepare(uint32 mode);
uint32 gtmenu_display_buffer_apply(uint32 alternate);
uint32 gtmenu_display_buffers_swap(uint32 mode);
uint32 gtmenu_display_enable(uint32 enabled);
uint32 gtmenu_input_update(void);
uint32 gtmenu_gpu_wait(sint32 mode);
void gtmenu_layout_release(void);
uint32 gtmenu_layout_initialize(void);
uint32 gtmenu_link_add(uint32 name, uint32 value, uint32 upper);
uint32 gtmenu_text_surface_address(void);
uint32 gtmenu_hitboxes_defaults_initialize(void);
uint32 gtmenu_hitboxes_bind(uint32 table);
uint32 gtmenu_input_direction(uint32 buttons);
sint32 gtmenu_pointer_center_hitbox(uint32 index);
sint32 gtmenu_pointer_position_set(sint16 x, sint16 y);
uint32 gtmenu_widget_node_release(uint32 node);
void gtmenu_widget_groups_release(uint32 widget);
uint32 gtmenu_configuration_defaults_initialize(void);
uint32 gtmenu_environment_value(uint32 name);
uint32 gtmenu_page_resource_load(uint32 name);
uint32 gtmenu_page_resource_index_load(sint32 index);
uint32 gtmenu_archive_entry_unpack(uint32 archive, sint32 index, uint32 output);
uint32 gtmenu_lz_unpack(uint32 output, uint32 input, sint32 length);
uint32 gtmenu_hitboxes_build(void);
uint32 gtmenu_navigation_node_create(uint32 kind, uint32 rectangle);
uint32 gtmenu_page_action_code(uint32 page);
void gtmenu_page_widgets_draw(uint32 ordering);
uint32 gtmenu_page_widget_create(uint32 output, uint32 description);
uint32 gtmenu_simple_list_widget_create(uint32 output, uint32 rectangle);
uint32 gtmenu_circle_bounds(uint32 output, uint32 packed_center, uint32 radius);
/* Pending original GTMENU hitbox and page helpers */
uint32 gtmenu_hitbox_buffer(void);
uint32 gtmenu_layout_rect_count(void);
uint32 gtmenu_hitbox_mode_set(uint32 mode);
sint32 gtmenu_ascii_lower(sint32 character);
uint32 gtmenu_catalog_widget_draw(uint32 ordering, uint32 owner);
uint32 gtmenu_page_footer_draw(uint32 ordering);
uint32 gtmenu_page_hint_draw(uint32 ordering, uint32 mode);
uint32 gtmenu_license_panel_pulse_draw(uint32 ordering);
void gtmenu_page_cursor_draw(uint32 ordering);
void gtmenu_progress_badge_draw(uint32 ordering, sint16 x, sint16 y, sint32 progress);
void gtmenu_progress_panel_draw(uint32 ordering);
uint32 gtmenu_time_format(uint32 destination, uint32 milliseconds);
/* Pending typed formatter boundary for GTMENU 80045698 */
uint32 gtmenu_format_integers4(uint32 destination, uint32 format, sint32 first, sint32 second, sint32 third, sint32 fourth);
uint32 gtmenu_model_info_layout_set(uint32 rectangle);
uint32 gtmenu_navigation_append(uint32 kind, uint32 rectangle);
void gtmenu_page_widgets_initialize(void);
uint32 gtmenu_navigation_bind_pair(uint32 first, uint32 second, uint32 first_rectangle, uint32 second_rectangle);
uint32 gtmenu_navigation_bind_trade(uint32 child, uint32 rectangle);
uint32 gtmenu_navigation_nodes_clear(void);
uint32 gtmenu_navigation_select_point(uint16 x, uint32 y);
void gtmenu_navigation_selection_set(uint32 node);
uint32 gtmenu_navigation_selection(void);
uint32 gtmenu_point_in_rectangle(uint32 rectangle, uint32 point);
uint32 gtmenu_navigation_input(uint32 event);
void gtmenu_car_spec_filter_set(uint8 enabled);
void gtmenu_hifi_filter_set(uint8 enabled);
uint32 gtmenu_triangle_wave(uint32 phase);
uint32 gtmenu_text_centered_at_y(uint32 ordering, uint32 rectangle, sint16 y, uint32 color, uint32 text);
uint32 gtmenu_two_choices_draw(uint32 selected, uint32 ordering, uint32 first_rectangle,
    uint32 second_rectangle, uint32 first_text, uint32 second_text, uint32 phase);
uint32 gtmenu_rectangle_outline(uint32 ordering, uint32 rectangle, uint32 color);
/* Pending original GTMENU 800457CC */
uint32 gtmenu_string_append(uint32 destination, uint32 source);
uint32 gtmenu_font_line_height(uint32 font);
uint32 gtmenu_text_fixed_width(sint32 font, uint32 text);
uint32 gtmenu_text_comma_width(sint32 font, uint32 text);
/* Pending original GTMENU 8004580C */
sint32 gtmenu_string_length(uint32 text);
sint32 gtmenu_glyph_run_width(uint32 state, uint32 text);
uint32 gtmenu_text_draw_mode(uint32 state, uint32 ordering);
uint32 gtmenu_text_cursor_draw_mode(uint32 ordering);
uint32 gtmenu_text_cursor_begin(uint32 ordering, sint16 x, sint16 y);
uint32 gtmenu_glyph_draw_fixed(uint8 character);
uint32 gtmenu_glyph_draw_numeric(uint8 character);
uint32 gtmenu_fonts_initialize(uint32 texture_page);
sint32 gtmenu_decimal_group(uint32 destination, uint32 source);
/* Pending original GTMENU 80032678 */
uint32 gtmenu_font_texture_initialize(uint32 state, uint32 texture_page, uint32 packed_pixels);
uint32 gtmenu_fullscreen_present(void);
uint32 gtmenu_fullscreen_clip(uint32 ordering);
uint32 gtmenu_clip_rectangle(uint32 ordering, uint32 rectangle);
/* Pending original GTMENU 80024AE8 and 800327DC */
uint32 gtmenu_scene_nodes_draw(uint32 ordering, uint32 scene);
uint32 gtmenu_display_buffers_clear(sint32 mode);
void gtmenu_car_info_color_toggle(void);
sint32 gtmenu_current_car_preview_initialize(void);
void gtmenu_parts_price_placeholders_draw(uint32 ordering, uint16 category, uint32 x, uint32 y);
uint32 gtmenu_parts_purchase_commit(void);
void gtmenu_selected_part_equip(uint32 record);
/* Pending guest formatter boundary with one signed integer */
uint32 gtmenu_format_integer(uint32 destination, uint32 format, sint32 value);
uint32 gtmenu_parts_purchase_open(void);
uint32 gtmenu_available_parts_initialize(void);
uint32 gtmenu_current_power(void);
uint32 gtmenu_parts_baseline_update(uint32 record);
uint32 gtmenu_ordering_arrays_allocate(void);
uint32 gtmenu_primitive_arrays_allocate(void);
uint32 gtmenu_render_lists_initialize(uint32 ordering_count, uint32 primitive_count);
uint32 gtmenu_render_lists_release(void);
uint32 gtmenu_secondary_ordering(void);
uint32 gtmenu_primitive_arrays_bind(uint32 first, uint32 second, uint32 count);
uint32 gtmenu_ordering_arrays_bind(uint32 first, uint32 second, uint32 count);
uint32 gtmenu_render_list_insert(uint32 node);
uint32 gtmenu_render_list_remove(uint32 node);
uint32 gtmenu_render_pool_initialize(uint32 first, uint32 second, uint32 capacity);
uint32 gtmenu_render_lists_partition(void);
uint32 gtmenu_model_info_texture_load(uint32 name);
/* Pending original GTMENU 80032A24, 80032E54, 80032E78, 80032EAC and 800329EC */
uint32 gtmenu_model_groups_load(uint32 records, sint32 count, uint32 geometry_buffers, uint32 texture_workspace);
uint32 gtmenu_model_record_indices(uint32 records, sint32 count, uint32 ids, uint32 keys, uint32 id_count, uint32 key_count);
uint32 gtmenu_model_resource_load(uint32 index, uint32 output, uint32 capacity);
sint32 gtmenu_car_count(void);
uint32 gtmenu_car_name(uint32 index);
uint32 gtmenu_model_texture_upload(uint32 texture, sint32 page);
uint32 gtmenu_model_palette_upload(uint32 texture, sint32 clut, uint32 palette);
uint32 gtmenu_model_geometry_bind(uint32 geometry, sint32 page);
uint32 gtmenu_model_sections_relocate(uint32 model);
uint32 gtmenu_model_texture_bits(uint32 model, uint32 page);
uint32 gtmenu_ordering_slot_clear(uint32 slot);
uint32 gtmenu_gpu_ordering_clear(uint32 table, uint32 count);
uint32 gtmenu_model_resource_root(void);
uint32 gtmenu_model_resource_tables_bind(uint32 root, uint32 cluts, uint32 pages, uint16 mode, uint16 count);
uint32 gtmenu_model_preview_frame_draw(void);
void gtmenu_preview_view_initialize(uint32 view);
uint32 gtmenu_preview_transform_initialize(uint32 transform);
uint32 gtmenu_preview_rotation_set(uint32 matrix, sint16 yaw, sint16 pitch, sint16 roll);
uint32 gtmenu_preview_vector_rotate(uint32 output, uint32 matrix, uint32 input);
sint32 gtmenu_preview_view_finalize(uint32 view, uint32 basis);
sint32 gtmenu_angle_asin(sint16 value);
sint32 gtmenu_angle_atan(sint16 x, sint16 y);
sint32 gtmenu_matrix_angles(uint32 output, uint32 matrix);
uint32 gtmenu_matrix_transpose(uint32 output, uint32 matrix);
sint32 gtmenu_matrix_multiply(uint32 output, uint32 left, uint32 right);
uint32 gtmenu_preview_context_bind(uint32 context, uint32 view);
uint32 gtmenu_preview_projection_bind(uint32 context, uint32 view);
uint32 gtmenu_preview_depth_cue_set(uint32 context, uint32 color, sint32 coefficient, sint32 offset);
sint32 gtmenu_preview_geometry_transform(uint32 context, uint32 geometry, uint32 mode);
sint32 gtmenu_model_range_index(uint32 variants, uint32 vector, sint32 extent);
uint32 gtmenu_matrix_bind_packed_negation(uint32 matrix);
sint32 gtmenu_matrix_scale_gte(uint32 output, uint32 matrix, uint16 scale);
void gtmenu_matrix_shift(uint32 output, uint32 matrix, sint32 shift);
uint32 gtmenu_model_matrix_bind(uint32 context, uint32 matrix);
uint32 gtmenu_lighting_bind(uint32 context);
uint32 gtmenu_model_bounds_classify(uint32 context, uint32 vertices, uint32 count);
sint32 gtmenu_matrix_rotate_short(uint32 output, uint32 matrix, uint32 vector);
uint32 gtmenu_matrix_negate_axes(uint32 output, uint32 matrix);
void gtmenu_model_primitives_draw(uint32 model, uint32 context);
void gtmenu_model_primitives_clipped_draw(uint32 model, uint32 context);
uint32 gtmenu_preview_geometry_draw(uint32 context, uint32 model, sint16 roll, sint16 yaw, sint16 pitch);
sint32 gtmenu_preview_wheel_matrix_scale(uint32 context, sint32 scale);
uint32 gtmenu_preview_wheel_position_set(uint32 context, uint32 x, uint32 y, uint32 z);
uint32 gtmenu_preview_wheel_draw(uint32 context, sint32 radius, uint32 detail);
uint32 gtmenu_preview_mesh_draw(uint32 context, uint32 mesh);
uint32 gtmenu_preview_rotation_bind(uint32 matrix);
uint32 gtmenu_model_resource_directory_load(uint32 workspace, uint32 capacity);
void gtmenu_model_resource_base_set(uint32 mode);
/* Pending guest-string diagnostic adapter for original GTMENU 8004F7C0 */
uint32 gtmenu_debug_string(uint32 format, uint32 text);
uint32 gtmenu_model_color_previous(uint32 model);
void gtmenu_model_preview_pitch_set(sint16 pitch);
uint32 gtmenu_model_info_page_restore(void);
uint32 gtmenu_model_info_actions_activate(void);
uint32 gtmenu_model_rotor_count(uint32 name);
