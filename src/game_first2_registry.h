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
void gtmain_text_draw(uint32 ordering_link, sint16 x, sint16 y, uint32 guest_string);
void gtmain_race_signed_text_draw(uint32 guest_text);
sint32 gtmain_text_font(sint32 index);
uint32 gtmain_primitive_texture_page(uint32 context, uint32 ordering_link);
sint32 gtmain_cd_stream_idle(void);
uint32 gtmain_cd_stream_callback(uint32 token);
void gtmain_cd_stream_read(uint32 destination, uint32 sector, uint32 size);
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
void gtmain_cd_stream_start(uint32 sector);
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
void gtmain_cd_directory_start(void);
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
sint32 gtmain_model_distances_square(uint32 entries);
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
void gtmain_race_config_reset(void);

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
void gtmain_race_loading_draw(void);
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
void gtmain_frame_tables_clear(sint32 selection);
void gtmain_ordering_clear(uint32 index);
sint32 gtmain_frame_run(uint8 *context);
uint32 gtmain_menu_frame_context_initialize(uint8 *context);
uint32 gtmain_menu_input_context_initialize(uint32 context);
sint32 gtmain_menu_frame_run(void);
uint32 gtmain_menu_result_mode4_run(uint32 extra, sint16 index, uint32 first);
uint32 gtmain_menu_result_mode4_alternate_run(uint32 extra, sint16 index, uint32 first);
void gtmain_frame_vblank(void);
void gtmain_frame_prepare(uint32 alternate);
void gtmain_frame_display_begin(void);
void gtmain_frame_swap(uint32 wait_mode);
void gtmain_frame_submit(uint32 alternate);
void gtmain_ordering_submit(uint32 index);
void gtmain_frame_links_reset(uint32 alternate);

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

void gtmain_frame_environments_apply(void);
void gtmain_frame_display_apply(uint32 alternate);
void gtmain_frame_draw_apply(uint32 alternate);
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
void gtmain_stream_begin(void);
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
void gtmain_car_rumble_update(uint32 car);
uint32 gtmain_course_segment_transform(uint32 context, uint32 segment);
uint32 gtmain_course_segment_draw(uint32 context, uint32 segment, uint32 mode);
uint32 gtmain_model_bounds_classify(uint32 context, const uint8 *points, uint32 count);
uint32 gtmain_model_billboards_draw(uint32 context, uint32 records, uint32 count);
uint32 gtmain_course_scene_draw(uint32 context, uint32 camera, uint32 mode);

sint32 gtmain_vector_normalize_xyz(uint8 *out, const uint8 *input);

sint32 gtmain_stream_entry_find(uint32 index);
sint32 gtmain_stream_entry_prepare(uint32 entry, uint32 parameter);
void gtmain_stream_begin_if_loaded(void);
uint32 gtmain_stream_playlist_next(uint32 playlist);
void gtmain_stream_mode_set(uint8 mode);
sint32 gtmain_stream_stop_wait(uint32 wait);
sint32 gtmain_stream_stop_async(void);
void gtmain_stream_pause_state(void);
void gtmain_stream_pause_if_loaded(void);
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
uint8 * gtmain_texture_clut_native(uint32 id);
uint8 * gtmain_texture_page_native(uint32 id);
sint32 gtmain_texture_x_native(uint32 id);
sint32 gtmain_texture_y_native(uint32 id);
uint32 gtmain_camera_flat_texture_ids(const uint32 *ids, uint32 ids_count, uint32 packets, sint32 count, uint32 vertices);
uint32 gtmain_camera_gouraud_texture_ids(const uint32 *ids, uint32 ids_count, uint32 packets, sint32 count, uint32 vertices);
sint32 gtmain_texture_name_find_native(const uint8 *name);
void gtmain_texture_workspace_publish(uint8 *workspace, uint32 capacity);
void gtmain_texture_workspace_release(uint8 *workspace);
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

void gtmain_race_ordering_reset(void);

void gtmain_race_overlay_draw(sint32 player);

void gtmain_race_view_draw(sint32 view_index);

void gtmain_race_updates_reset(void);

void gtmain_race_messages_load(void);

void gtmain_race_point_rotate(uint32 guest_xy, uint32 angle, sint32 origin_x, sint32 origin_y);

void gtmain_race_pause_draw(void);

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

void gtmain_race_effects_tick(void);

void gtmain_race_cars_tick(void);

void gtmain_race_hud_tick(void);

void gtmain_race_pause_blink(void);

void gtmain_race_sound_callbacks_active(void);

void gtmain_race_sound_callbacks_stopped(void);

void gtmain_race_time_reset(void);

void gtmain_race_lap_history_reset(void);

void gtmain_race_split_reset(void);

void gtmain_race_best_reset(void);

void gtmain_race_timers_reset(void);

sint32 gtmain_polygon_clip_flat(uint32 context);

sint32 gtmain_polygon_clip_gouraud(uint32 context);

void gtmain_race_input_update(uint32 state, uint32 configuration);

sint32 gtmain_race_menu_input(void);

void gtmain_race_time_set(uint32 time);

void gtmain_race_lap_time_store(uint32 time);

uint32 gtmain_time_string(uint32 destination, uint32 milliseconds);
void gtmain_time_string_native(char *output, uint32 capacity, uint32 milliseconds);

sint32 gtmain_race_sound_callbacks_apply(uint32 first_owner, uint32 second_owner, uint32 third_owner);

uint32 gtmain_race_effect_alloc(void);

void gtmain_race_effect_init(uint32 effect);

uint32 gtmain_race_license_result_tick(void);

uint32 gtmain_race_assist_tick(void);

void gtmain_race_assist_reset(void);

void gtmain_race_mode_tick(void);

void gtmain_race_countdown_tick(void);

void gtmain_race_rumble_clear(void);

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

void gtmain_race_pad_map(uint32 state, uint32 packet, uint32 buttons, uint32 threshold, uint32 first_axis, uint32 second_axis, uint32 analog);

void gtmain_race_sound_car_primary(uint32 car);

void gtmain_race_sound_car_secondary(uint32 car);

void gtmain_race_lap_time_append(uint32 time);

void gtmain_race_physics_tick(void);

uint32 gtmain_race_camera_tick(uint32 camera);

void gtmain_race_split_screen_update(void);

void gtmain_race_effects_draw(void);

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

void gtmain_race_clock_tick(void);

sint32 gtmain_race_pad_axis_value(sint32 mapping, uint32 unused_state, uint32 packet, uint32 range, uint32 analog);

uint32 gtmain_cd_controller_volume_init(void);

sint32 gtmain_cd_controller_init(void);

uint32 gtmain_cd_controller_interrupt(void);

uint32 gtmain_model_primitives_clipped_draw(uint32 model, uint32 context);

uint32 gtmain_cd_controller_service(void);

uint32 gtmain_interrupt_callback_enabled(void);

uint32 gtmain_interrupt_mask_exchange(uint16 mask);

uint32 gtmain_dma_callback_exchange(uint32 index, uint32 token);

void gtmain_race_controls_decode(uint8 *controls, uint32 state, sint32 player, uint32 object);

void gtmain_race_controls_drive(uint32 car, const uint8 *controls, sint32 index);

void gtmain_race_controls_ai(uint32 car, const uint8 *unused_controls, sint32 index);

void gtmain_race_controls_hud(uint32 car, const uint8 *controls);

void gtmain_race_controls_hud_ai(uint32 car);

void gtmain_race_controls_apply(uint32 car, const uint8 *controls, uint32 guest_output, sint32 index);

sint32 gtmain_race_curve32(uint32 curve, sint32 value);

void gtmain_race_basis_set(uint32 first, uint32 second, uint32 third, sint32 first_angle, sint32 second_angle, sint32 third_angle);

sint32 gtmain_race_angle_wrap(sint32 angle);

sint32 gtmain_race_angle_delta(sint32 first, sint32 second);

void gtmain_race_wheel_pose(uint32 car);

void gtmain_race_suspension_prepare(uint32 cars, sint32 count);

void gtmain_race_suspension_update(uint32 cars, sint32 count);

void gtmain_race_velocity_publish(uint32 cars, sint32 count);

uint32 gtmain_race_lap_car_tick(uint32 car, sint32 rank);

void gtmain_race_lap_difference_show(sint32 car_index, sint32 difference);

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

void gtmain_race_suspension_car_prepare(uint32 car);

void gtmain_race_suspension_car_update(uint32 car);

void gtmain_race_input_playback(uint32 object, uint32 state);

void gtmain_race_brake_assist(uint32 car);

void gtmain_race_controls_launch(uint32 car, sint32 index, sint32 time);

void gtmain_race_throttle_assist(uint32 car);

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

void gtmain_race_lap_report(uint32 unused_car, sint32 lap, uint32 checkpoint, uint32 time, uint32 unused_elapsed, sint32 rank);

void gtmain_race_input_record_flush(uint32 object);

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

void gtmain_race_ai_segment_update(uint32 car);

sint32 gtmain_race_behavior_set(uint32 car, sint32 behavior);

uint32 gtmain_race_wheel_surface_reset(uint32 car);

void gtmain_race_vehicle_reset(uint32 car);

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

void gtmain_race_vehicle_pose_update(uint32 car);

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

void gtmain_race_axle_slip_update(uint32 car, sint32 index);

void gtmain_race_slip_ratio(uint32 scratch_row, sint32 reference, sint32 speed);

void gtmain_race_axle_velocity_integrate(uint32 car, sint32 index, sint32 axle);

void gtmain_race_slip_publish(uint32 car, sint32 index);

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
void gtmain_race_best_time_draw(uint32 ordering, sint32 x, sint32 y);
void gtmain_race_record_time_draw(uint32 ordering, sint32 x, sint32 y);

uint32 gtmain_race_instruments_draw(uint32 ordering, sint32 x, sint32 y, uint32 car);
void gtmain_race_rpm_scale_draw(uint32 ordering, sint32 x, sint32 y, uint32 car);
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
void gtmain_menu_stream_check(void);
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
