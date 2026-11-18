#include "game_first2_registry.h"
#include "game_draft_registry.h"

static sint32 course_polygon_gte_flags(void)
{
    PsxGteSnapshot state;
    psx_gte_snapshot(&state);
    return state.flag;
}


static sint32 course_polygon_facing(uint32 context, uint32 first, uint32 second, uint32 third)
{
    sint32 eye_x = (sint16)r_u16(context + 56u), eye_y = (sint16)r_u16(context + 58u), eye_z = (sint16)r_u16(context + 60u);
    sint32 first_x, first_y, first_z, second_x, second_y, second_z;
    sint32 last_x, last_y, last_z;
    uint32 mac_x, mac_y, mac_z, diagonal_x, diagonal_y, diagonal_z, packed, result;
    first_x = (sint16)r_u16(context + first);
    first_y = (sint16)r_u16(context + first + 2u);
    xport_gte_write_control(0u, (uint32)(first_x - eye_x));
    first_z = (sint16)r_u16(context + first + 4u);
    xport_gte_write_control(2u, (uint32)(first_y - eye_y));
    second_x = (sint16)r_u16(context + second);
    xport_gte_write_control(4u, (uint32)(first_z - eye_z));
    second_y = (sint16)r_u16(context + second + 2u);
    xport_gte_write_data(9u, (uint32)(second_x - eye_x));
    second_z = (sint16)r_u16(context + second + 4u);
    xport_gte_write_data(10u, (uint32)(second_y - eye_y));
    xport_gte_write_data(11u, (uint32)(second_z - eye_z));
    last_x = (sint16)r_u16(context + third) - eye_x;
    last_y = (sint16)r_u16(context + third + 2u) - eye_y;
    xport_gte_execute(0x170000Cu);
    last_z = (sint16)r_u16(context + third + 4u) - eye_z;
    mac_x = xport_gte_read_data(25u); mac_y = xport_gte_read_data(26u); mac_z = xport_gte_read_data(27u);
    xport_gte_write_data(9u, last_x); xport_gte_write_data(10u, last_y); xport_gte_write_data(11u, last_z);
    packed = (mac_x & 0xFFFu) | ((mac_y & 0xFFFu) << 16);
    xport_gte_write_control(16u, packed);
    mac_x = (uint32)((sint32)mac_x >> 12); mac_y = (uint32)((sint32)mac_y >> 12);
    packed = (mac_z & 0xFFFu) | ((mac_x & 0xFFFu) << 16);
    xport_gte_write_control(17u, packed); mac_z = (uint32)((sint32)mac_z >> 12);
    packed = (mac_y & 0xFFFu) | ((mac_z & 0xFFFu) << 16);
    xport_gte_write_control(18u, packed); mac_z = (uint32)((sint32)mac_z >> 12);
    packed = (uint16)((sint32)mac_x >> 12) | ((uint32)((sint32)mac_y >> 12) << 16);
    xport_gte_write_control(19u, packed); xport_gte_write_control(20u, mac_z);
    diagonal_x = r_u32(context + 96u); diagonal_y = r_u32(context + 104u);
    xport_gte_execute(0x45E012u); diagonal_z = r_u32(context + 112u);
    xport_gte_write_control(0u, diagonal_x); xport_gte_write_control(2u, diagonal_y); xport_gte_write_control(4u, diagonal_z);
    mac_x = xport_gte_read_data(25u); mac_y = xport_gte_read_data(26u); mac_z = xport_gte_read_data(27u);
    result = mac_z + (uint32)((sint32)(mac_y + (uint32)((sint32)mac_x >> 12)) >> 12);
    if (r_u32(context + 924u)) result = 0u - result;
    return (sint32)result;
}

static void course_polygon_link(uint32 ordering, uint32 first_packet, uint32 last_packet)
{
    uint32 previous = r_u8(ordering);
    previous |= (uint32)r_u8(ordering + 1u) << 8;
    previous |= (uint32)r_u8(ordering + 2u) << 16;
    w_masked_u32(ordering, first_packet, 7u);
    w_masked_u32(last_packet, previous, 7u);
}

/* Unverified pseudocode draft with the reviewed three-argument ABI */
/* GTE operations use live shared services in original order */
/* Saved full draft with cumulative original-image review */
/* Original: GTMAIN.EXE:sub_80011DA0. */ uint32 gt_draft_80011DA0(uint32 a0, uint32 a1, uint32 a2)
{
    sint32 v4 = 0;
    uint32 v5 = 0;
    sint32 v6 = 0;
    sint32 v8 = 0;
    uint32 v9 = 0;
    sint32 v10 = 0;
    sint32 v14 = 0;
    sint32 v15 = 0;
    sint32 v20 = 0;
    sint32 v22 = 0;
    sint32 v23 = 0;
    sint32 v24 = 0;
    sint32 v25 = 0;
    sint32 v48 = 0;
    uint32 v50 = 0;
    sint32 v51 = 0;
    sint32 v52 = 0;
    sint32 v53 = 0;
    sint32 v54 = 0;
    sint32 v55 = 0;
    sint32 v56 = 0;
    sint32 v57 = 0;
    sint32 v58 = 0;
    sint32 v59 = 0;
    sint32 v60 = 0;
    sint32 v61 = 0;
    sint32 v62 = 0;
    sint32 v66 = 0;
    sint32 v68 = 0;
    uint32 v69 = 0;
    sint32 v70 = 0;
    sint32 v74 = 0;
    sint32 v75 = 0;
    uint32 v76 = 0;
    sint32 v84 = 0;
    sint32 v87 = 0;
    sint32 v88 = 0;
    sint32 v89 = 0;
    sint32 v91 = 0;
    sint32 v92 = 0;
    sint32 v93 = 0;
    sint32 v94 = 0;
    uint32 v118 = 0;
    sint32 v119 = 0;
    sint32 v120 = 0;
    sint32 v121 = 0;
    sint32 v122 = 0;
    sint32 v123 = 0;
    sint32 v124 = 0;
    sint32 v125 = 0;
    sint32 v126 = 0;
    sint32 v127 = 0;
    sint32 v128 = 0;
    sint32 v129 = 0;
    sint32 v130 = 0;
    sint32 v131 = 0;
    sint8 v132 = 0;
    sint8 v133 = 0;
    sint8 v134 = 0;
    sint32 v135 = 0;
    sint32 v137 = 0;
    uint32 v138 = 0;
    sint32 v139 = 0;
    sint32 v143 = 0;
    sint32 v144 = 0;
    sint32 v149 = 0;
    sint32 v151 = 0;
    sint32 v152 = 0;
    sint32 v153 = 0;
    sint32 v154 = 0;
    sint32 v177 = 0;
    uint32 v180 = 0;
    sint32 v181 = 0;
    sint32 v182 = 0;
    sint32 v183 = 0;
    sint32 v184 = 0;
    sint32 v185 = 0;
    sint32 v186 = 0;
    sint32 v187 = 0;
    sint32 v188 = 0;
    sint32 v189 = 0;
    sint32 v190 = 0;
    sint32 v191 = 0;
    sint32 v192 = 0;
    sint8 v193 = 0;
    sint8 v194 = 0;
    sint8 v195 = 0;
    sint32 v196 = 0;
    sint32 v198 = 0;
    uint32 v199 = 0;
    sint32 v200 = 0;
    sint32 v204 = 0;
    sint32 v205 = 0;
    uint32 v206 = 0;
    sint32 v214 = 0;
    sint32 v217 = 0;
    sint32 v218 = 0;
    sint32 v219 = 0;
    sint32 v221 = 0;
    sint32 v222 = 0;
    sint32 v223 = 0;
    sint32 v224 = 0;
    uint32 v248 = 0;
    sint32 v253 = 0;
    sint32 v254 = 0;
    sint32 v255 = 0;
    sint32 v256 = 0;
    sint32 v257 = 0;
    sint32 v258 = 0;
    sint32 v259 = 0;
    sint32 v260 = 0;
    sint32 v261 = 0;
    sint32 v262 = 0;
    sint32 v263 = 0;
    sint32 v264 = 0;
    sint32 v265 = 0;
    sint8 v266 = 0;
    sint8 v267 = 0;
    sint8 v268 = 0;
    sint32 v269 = 0;
    sint32 v271 = 0;
    uint32 v272 = 0;
    sint32 v273 = 0;
    sint32 v277 = 0;
    uint32 v278 = 0;
    sint32 v279 = 0;
    sint32 v284 = 0;
    sint32 v286 = 0;
    sint32 v287 = 0;
    sint32 v288 = 0;
    sint32 v289 = 0;
    sint32 v312 = 0;
    uint32 v314 = 0;
    sint32 v315 = 0;
    uint32 v316 = 0;
    uint32 v317 = 0;
    sint32 v318 = 0;
    sint32 v319 = 0;
    sint32 v320 = 0;
    sint32 v321 = 0;
    sint32 v322 = 0;
    sint32 v323 = 0;
    sint32 v324 = 0;
    sint32 v325 = 0;
    sint32 v326 = 0;
    sint32 v327 = 0;
    sint32 v328 = 0;
    sint32 v329 = 0;
    sint32 v330 = 0;
    sint32 v331 = 0;
    uint16 v333 = 0;
    sint32 v335 = 0;
    sint32 v336 = 0;
    sint32 v337 = 0;
    sint32 v338 = 0;
    sint32 v339 = 0;
    sint32 v340 = 0;
    sint32 v341 = 0;
    sint32 v342 = 0;
    sint32 v345 = 0;
    sint32 v346 = 0;
    sint16 v347 = 0;
    sint16 v348 = 0;
    sint16 v349 = 0;
    sint32 v350 = 0;
    uint32 v351 = 0;
    sint32 v352 = 0;
    sint32 v353 = 0;
    sint32 v354 = 0;
    sint32 v355 = 0;
    sint32 v356 = 0;
    sint32 v357 = 0;
    sint32 v358 = 0;
    sint32 v359 = 0;
    sint32 v360 = 0;
    sint32 v361 = 0;
    sint32 v362 = 0;
    sint32 v363 = 0;
    sint32 v364 = 0;
    sint32 v365 = 0;
    sint32 v366 = 0;
    sint32 v367 = 0;
    sint32 v368 = 0;
    sint32 v369 = 0;
    sint32 v370 = 0;
    sint32 v371 = 0;
    sint32 v372 = 0;
    sint32 v373 = 0;
    sint32 v376 = 0;
    sint32 v377 = 0;
    sint16 v378 = 0;
    sint16 v379 = 0;
    sint16 v380 = 0;
    uint32 v381 = 0;
    sint32 v382 = 0;
    sint32 v383 = 0;
    uint32 v384;
    uint32 v385;
    sint32 v386 = 0;
    sint32 v387 = 0;
    sint32 v389 = 0;
    sint16 v392 = 0;
    uint32 v393;
    sint32 v394 = 0;
    sint32 v396 = 0;
    sint32 v400 = 0;
    sint32 v401 = 0;
    sint32 v402 = 0;
    sint32 v403 = 0;
    uint32 v404;
    uint32 v405;
    uint32 v406;
    sint32 v408 = 0;
    sint32 v409 = 0;
    uint32 v410 = 0;
    sint32 v411 = 0;
    uint32 v412 = 0;
    sint32 v413 = 0;
    sint8 v414 = 0;
    sint8 v415 = 0;
    sint8 v416 = 0;
    sint8 v417 = 0;
    sint8 v418 = 0;
    sint8 v419 = 0;
    sint32 v420 = 0;
    sint32 v422 = 0;
    uint32 v423 = 0;
    sint32 v424 = 0;
    sint32 v428 = 0;
    sint32 v429 = 0;
    uint32 v430 = 0;
    uint32 v431 = 0;
    sint32 v439 = 0;
    sint32 v442 = 0;
    sint32 v443 = 0;
    sint32 v444 = 0;
    sint32 v446 = 0;
    sint32 v447 = 0;
    sint32 v448 = 0;
    sint32 v449 = 0;
    uint32 v473 = 0;
    uint32 v474 = 0;
    sint32 v475 = 0;
    uint32 v476 = 0;
    sint32 v477 = 0;
    sint32 v478 = 0;
    sint32 v479 = 0;
    sint32 v480 = 0;
    sint32 v481 = 0;
    sint32 v482 = 0;
    sint32 v483 = 0;
    sint32 v484 = 0;
    sint32 v485 = 0;
    sint32 v486 = 0;
    sint32 v487 = 0;
    sint32 v488 = 0;
    sint32 v489 = 0;
    sint32 v490 = 0;
    sint32 v491 = 0;
    sint32 v492 = 0;
    uint16 v494 = 0;
    sint32 v496 = 0;
    sint32 v497 = 0;
    sint32 v498 = 0;
    sint32 v499 = 0;
    sint32 v500 = 0;
    sint32 v501 = 0;
    sint32 v502 = 0;
    sint32 v503 = 0;
    sint32 v506 = 0;
    sint32 v507 = 0;
    sint16 v508 = 0;
    sint16 v509 = 0;
    sint16 v510 = 0;
    sint32 v511 = 0;
    uint32 v512 = 0;
    sint32 v513 = 0;
    sint32 v514 = 0;
    sint32 v515 = 0;
    sint32 v516 = 0;
    sint32 v517 = 0;
    sint32 v518 = 0;
    sint32 v519 = 0;
    sint32 v520 = 0;
    sint32 v521 = 0;
    sint32 v522 = 0;
    sint32 v523 = 0;
    sint32 v524 = 0;
    sint32 v525 = 0;
    sint32 v526 = 0;
    sint32 v527 = 0;
    sint32 v528 = 0;
    sint32 v529 = 0;
    sint32 v530 = 0;
    sint32 v531 = 0;
    sint32 v532 = 0;
    sint32 v533 = 0;
    sint32 v534 = 0;
    sint32 v535 = 0;
    sint32 v536 = 0;
    sint32 v537 = 0;
    sint32 v538 = 0;
    sint32 v539 = 0;
    sint32 v540 = 0;
    sint32 v541 = 0;
    sint32 v542 = 0;
    sint32 v543 = 0;
    sint32 v544 = 0;
    sint32 v545 = 0;
    sint32 v546 = 0;
    sint32 v547 = 0;
    sint32 v548 = 0;
    sint32 v549 = 0;
    sint32 v550 = 0;
    sint32 v553 = 0;
    sint32 v554 = 0;
    sint16 v555 = 0;
    sint16 v556 = 0;
    sint16 v557 = 0;
    uint32 v558 = 0;
    sint32 v559 = 0;
    uint32 v560;
    uint32 v561;
    sint32 v562 = 0;
    sint32 v566 = 0;
    uint32 v567;
    uint32 v569 = 0;
    uint32 v570 = 0;
    sint32 v571 = 0;
    sint32 v572 = 0;
    sint32 v577 = 0;
    sint32 v578 = 0;
    sint32 v579 = 0;
    uint32 v580;
    uint32 v581;
    sint32 v583 = 0;
    sint32 v584 = 0;
    sint32 v586 = 0;
    sint32 v587 = 0;
    uint32 v588 = 0;
    sint32 v589 = 0;
    uint32 v590 = 0;
    sint32 v591 = 0;
    sint8 v592 = 0;
    sint8 v593 = 0;
    sint8 v594 = 0;
    sint8 v595 = 0;
    sint8 v596 = 0;
    sint8 v597 = 0;
    sint32 v598 = 0;
    sint32 v600 = 0;
    uint32 v601 = 0;
    sint32 v602 = 0;
    sint32 v606 = 0;
    uint32 v607 = 0;
    sint32 v608 = 0;
    sint32 v613 = 0;
    sint32 v615 = 0;
    sint32 v616 = 0;
    sint32 v617 = 0;
    sint32 v618 = 0;
    sint32 v641 = 0;
    uint32 v643 = 0;
    sint32 v644 = 0;
    uint32 v645 = 0;
    uint32 v647 = 0;
    sint32 v648 = 0;
    sint32 v649 = 0;
    sint32 v650 = 0;
    sint32 v651 = 0;
    sint32 v652 = 0;
    sint32 v653 = 0;
    sint32 v654 = 0;
    sint32 v655 = 0;
    sint32 v656 = 0;
    sint32 v657 = 0;
    sint32 v658 = 0;
    uint32 v659 = 0;
    uint32 v660 = 0;
    uint32 v661 = 0;
    sint8 v662 = 0;
    sint8 v663 = 0;
    sint8 v664 = 0;
    sint32 v665 = 0;
    sint32 v667 = 0;
    uint32 v668 = 0;
    sint32 v669 = 0;
    sint32 v673 = 0;
    sint32 v674 = 0;
    uint32 v675 = 0;
    uint32 v676 = 0;
    sint32 v684 = 0;
    sint32 v687 = 0;
    sint32 v688 = 0;
    sint32 v689 = 0;
    sint32 v691 = 0;
    sint32 v692 = 0;
    sint32 v693 = 0;
    sint32 v694 = 0;
    uint32 v718 = 0;
    uint32 v719 = 0;
    uint32 v720 = 0;
    sint32 v725 = 0;
    sint32 v726 = 0;
    sint32 v727 = 0;
    sint32 v728 = 0;
    sint32 v729 = 0;
    sint32 v730 = 0;
    sint32 v731 = 0;
    sint32 v732 = 0;
    sint32 v733 = 0;
    sint32 v734 = 0;
    sint32 v735 = 0;
    sint32 v736 = 0;
    sint32 v737 = 0;
    sint32 v738 = 0;
    uint32 v739 = 0;
    uint32 v740 = 0;
    uint32 v741 = 0;
    sint16 v742 = 0;
    uint32 v743 = 0;
    uint32 v744 = 0;
    sint8 v745 = 0;
    sint8 v746 = 0;
    sint8 v747 = 0;
    sint32 v748 = 0;
    uint32 carrier_a0;
    uint32 carrier_a1;
    uint32 carrier_a2;
    uint32 carrier_a3;
    uint32 carrier_s1;
    uint32 carrier_s2;
    uint32 carrier_s3;
    uint32 carrier_s4;
    uint32 carrier_t0;
    uint32 carrier_t1;
    uint32 carrier_t2;
    uint32 carrier_t3;
    uint32 carrier_t4;
    uint32 carrier_t5;
    uint32 carrier_t6;
    uint32 carrier_t7;
    uint32 carrier_t8;
    uint32 carrier_t9;
    uint32 carrier_v0;
    uint32 carrier_v1;
  v4 = a1 + 164;
  if ( a2 )
    v4 = r_u32(a1 + 148);
    w_u32(a0 + 928, a1);
    w_u32(a0 + 936, r_u32(v4));
  gtmain_course_segment_transform(a0, a1);
  v5 = r_u32(v4 + 4);
    w_u32(a0 + 956, v5);
  v6 = r_u16(v4 + 48);
  carrier_t4 = v5;
    w_u32(a0 + 932, v4);
  if ( v6 )
  {
    v8 = r_u32(a0 + 952);
    v9 = r_u32(v5);
    v10 = r_u32(a0 + 936);
    carrier_t8 = v10 + 8 * ((uint32)v9 & 0x1FF);
    carrier_s2 = v10 + (((uint32)v9 >> 6) & 0xFF8);
    carrier_s1 = v10 + (((uint32)v9 >> 15) & 0xFF8);
    do
    {
      uint32 polygon_metadata, ordering_base;
      uint16 near_depth, depth_shift;
      sint32 viewport_width, viewport_height;
xport_gte_write_data(0, r_u32(carrier_s2 + (0)));
xport_gte_write_data(1, r_u32(carrier_s2 + (4)));
xport_gte_write_data(2, r_u32(carrier_t8 + (0)));
xport_gte_write_data(3, r_u32(carrier_t8 + (4)));
xport_gte_write_data(4, r_u32(carrier_s1 + (0)));
xport_gte_write_data(5, r_u32(carrier_s1 + (4)));
      --v6;
      v14 = v9;
xport_gte_execute(0x280030);
      v9 = r_u32(carrier_t4 + 4u * (3));
      polygon_metadata = r_u32(carrier_t4 + 4u);
      v15 = r_u32(a0 + 936);
      near_depth = r_u16(a0 + 8u);
      depth_shift = r_u16(a0 + 40u);
      carrier_t8 = v15 + 8 * (v9 & 0x1FF);
      carrier_s2 = v15 + ((v9 >> 6) & 0xFF8);
      carrier_s1 = v15 + ((v9 >> 15) & 0xFF8);
carrier_t6 = carrier_t6 = course_polygon_gte_flags();
carrier_t0 = carrier_t0 = xport_gte_read_data(17);
v15 = carrier_v1 = xport_gte_read_data(18);
carrier_a0 = xport_gte_read_data(19);
      ordering_base = r_u32(a0 + 948u);
xport_gte_execute(0x1400006);
      if ( carrier_t0 < carrier_v1 )
        carrier_t0 = carrier_v1;
      v20 = ((uint32)v14 >> 23) & 0x30;
      if ( carrier_t0 < carrier_a0 )
        carrier_t0 = carrier_a0;
      if ( carrier_t0 >= near_depth )
      {
v5 = carrier_v0 = xport_gte_read_data(24);
        v22 = carrier_v0 & v14;
        if ( (sint32)carrier_t6 < 0 )
        {
          v22 = 0;
          if ( v14 < 0 )
          {
w_u32(a0 + (0x164), xport_gte_read_data(0));
w_u32(a0 + (0x168), xport_gte_read_data(1));
w_u32(a0 + (0x144), xport_gte_read_data(2));
w_u32(a0 + (0x148), xport_gte_read_data(3));
w_u32(a0 + (0x184), xport_gte_read_data(4));
w_u32(a0 + (0x188), xport_gte_read_data(5));
            v22 = course_polygon_facing(a0, 324u, 388u, 356u);
          }
        }
        v48 = carrier_t0 >> (depth_shift & 31u);
        if ( v22 >= 0 )
        {
          if ( v48 >= 4096 )
            v48 = 4095;
          carrier_v0 = 0;
          if ( (polygon_metadata & 0x8000000u) == 0 )
            carrier_v0 = r_u16(2 * v48 + r_u32(a0 + 940));
w_u32(a0 + (0x16C), xport_gte_read_data(12));
w_u32(a0 + (0x14C), xport_gte_read_data(13));
w_u32(a0 + (0x18C), xport_gte_read_data(14));
xport_gte_write_data(6, r_u32(carrier_t4 + (8)));
xport_gte_write_data(8, carrier_v0);
          v50 = (uint32)(ordering_base + 4 * (v20 + v48));
xport_gte_execute(0x780010);
          v51 = (sint16)r_u16(a0 + 332);
          v52 = (sint16)r_u16(a0 + 364);
          v53 = (sint16)r_u16(a0 + 396);
          v54 = v51;
          v55 = v51 < v52;
          if ( v52 < v51 )
            v51 = v52;
          if ( v55 )
            v54 = v52;
          if ( v53 < v51 )
            v51 = v53;
          if ( v54 < v53 )
            v54 = v53;
          viewport_width = (sint16)r_u16(a0 + 4u);
          if ( v54 > 0 && v51 < viewport_width )
          {
            v56 = carrier_t6 | (1023 - (v54 - v51));
            v57 = (sint16)r_u16(a0 + 334);
            v58 = (sint16)r_u16(a0 + 366);
            v59 = (sint16)r_u16(a0 + 398);
            v60 = v57;
            v61 = v57 < v58;
            if ( v58 < v57 )
              v57 = v58;
            if ( v61 )
              v60 = v58;
            v62 = v60 < v59;
            if ( v59 < v57 )
              v57 = v59;
            if ( v60 < v59 )
              v60 = v59;
            viewport_height = (sint16)r_u16(a0 + 6u);
            if ( v60 > 0 && v57 < viewport_height )
            {
              if ( (v56 | (511 - (v60 - v57))) >= 0 )
              {
    w_u32(v8, 0x4000000);
w_u32(v8 + (4), xport_gte_read_data(22));
w_u32(v8 + (0xC), xport_gte_read_data(12));
w_u32(v8 + (8), xport_gte_read_data(13));
w_u32(v8 + (0x10), xport_gte_read_data(14));
                course_polygon_link(v50, (uint32)v8, (uint32)v8);
                v8 += 20;
              }
              else
              {
    w_u32(a0 + 956, carrier_t4);
    w_u32(a0 + 984, v6);
    w_u32(a0 + 976, v9);
    w_u32(a0 + 960, carrier_t8);
    w_u32(a0 + 964, carrier_s2);
    w_u32(a0 + 968, carrier_s1);
    w_u32(a0 + 168, v50);
    w_u32(a0 + 172, v8);
    w_u16(a0 + 184, 3);
w_u32(a0 + (0x9C), xport_gte_read_data(22));
w_u32(a0 + (0x164), xport_gte_read_data(0));
w_u32(a0 + (0x168), xport_gte_read_data(1));
w_u32(a0 + (0x144), xport_gte_read_data(2));
w_u32(a0 + (0x148), xport_gte_read_data(3));
w_u32(a0 + (0x184), xport_gte_read_data(4));
w_u32(a0 + (0x188), xport_gte_read_data(5));
                gtmain_polygon_clip_flat(a0);
                v8 = r_u32(a0 + 180);
                carrier_t4 = r_u32(a0 + 956);
                v6 = r_u32(a0 + 984);
                v9 = r_u32(a0 + 976);
                carrier_t8 = r_u32(a0 + 960);
                carrier_s2 = r_u32(a0 + 964);
                carrier_s1 = r_u32(a0 + 968);
              }
            }
          }
        }
      }
      carrier_t4 += 12;
    }
    while ( v6 );
    v4 = r_u32(a0 + 932);
    w_u32(a0 + 956, carrier_t4);
    w_u32(a0 + 952, v8);
  }
  v66 = r_u16(v4 + 50);
  carrier_t8 = r_u32(a0 + 956);
    w_u32(a0 + 932, v4);
  if ( v66 )
  {
    v68 = r_u32(a0 + 952);
    v69 = r_u32(carrier_t8);
    v70 = r_u32(a0 + 936);
    carrier_s4 = v70 + 8 * (v69 & 0x1FF);
    carrier_s3 = v70 + ((v69 >> 6) & 0xFF8);
    carrier_s2 = v70 + ((v69 >> 15) & 0xFF8);
    do
    {
      uint32 fourth_vertex_word, fog_table, ordering_base;
      uint16 near_depth, depth_shift;
      sint32 viewport_width, viewport_height;
xport_gte_write_data(0, r_u32(carrier_s3 + (0)));
xport_gte_write_data(1, r_u32(carrier_s3 + (4)));
xport_gte_write_data(2, r_u32(carrier_s2 + (0)));
xport_gte_write_data(3, r_u32(carrier_s2 + (4)));
w_u32(a0 + (0x164), xport_gte_read_data(0));
w_u32(a0 + (0x168), xport_gte_read_data(1));
xport_gte_write_data(4, r_u32(carrier_s4 + (0)));
xport_gte_write_data(5, r_u32(carrier_s4 + (4)));
      v74 = r_u32(a0 + 936);
      v75 = v69;
xport_gte_execute(0x280030);
      fourth_vertex_word = r_u32(carrier_t8 + 4u);
      v69 = r_u32(carrier_t8 + 4u * (3));
      v76 = (uint32)(v74 + 8 * (fourth_vertex_word & 0x1FF));
      carrier_v1 = (sint32)r_u32(v76);
      carrier_a0 = (sint32)r_u32(v76 + 4u * (1));
w_u32(a0 + (0x16C), xport_gte_read_data(12));
w_u32(v68 + (0xC), xport_gte_read_data(12));
v9 = carrier_t3 = course_polygon_gte_flags();
      near_depth = r_u16(a0 + 8u);
      --v66;
xport_gte_execute(0x1400006);
      carrier_s4 = v74 + 8 * (v69 & 0x1FF);
      carrier_s3 = v74 + ((v69 >> 6) & 0xFF8);
xport_gte_write_data(0, carrier_v1);
xport_gte_write_data(1, carrier_a0);
carrier_a0 = carrier_a0 = xport_gte_read_data(24);
v48 = carrier_t0 = xport_gte_read_data(17);
carrier_v1 = carrier_v1 = xport_gte_read_data(18);
v59 = carrier_a2 = xport_gte_read_data(19);
      depth_shift = r_u16(a0 + 40u);
      fog_table = r_u32(a0 + 940u);
xport_gte_execute(0x180001);
      if ( carrier_t0 < carrier_v1 )
        carrier_t0 = carrier_v1;
      v84 = ((uint32)v75 >> 23) & 0x30;
      if ( carrier_t0 < carrier_a2 )
        carrier_t0 = carrier_a2;
      carrier_s2 = v74 + ((v69 >> 15) & 0xFF8);
xport_gte_write_data(6, r_u32(carrier_t8 + (8)));
v76 = carrier_v0 = course_polygon_gte_flags();
carrier_v1 = carrier_v1 = xport_gte_read_data(19);
      v87 = carrier_t3 | carrier_v0;
xport_gte_execute(0x1400006);
      v88 = (0u - carrier_a0);
      if ( carrier_t0 < carrier_v1 )
        carrier_t0 = carrier_v1;
      if ( carrier_t0 >= near_depth )
      {
        v89 = carrier_t0 >> (depth_shift & 31u);
        ordering_base = r_u32(a0 + 948u);
v76 = carrier_v0 = xport_gte_read_data(24);
        v91 = v75 & v88 & carrier_v0;
        if ( v87 < 0 )
        {
          v91 = 0;
          if ( v75 < 0 )
          {
w_u32(a0 + (0x184), xport_gte_read_data(2));
w_u32(a0 + (0x188), xport_gte_read_data(3));
w_u32(a0 + (0x144), xport_gte_read_data(4));
w_u32(a0 + (0x148), xport_gte_read_data(5));
            v91 = course_polygon_facing(a0, 324u, 388u, 356u);
          }
        }
        if ( v91 >= 0 )
        {
          if ( v89 >= 4096 )
            v89 = 4095;
          carrier_v0 = 0;
          if ( (fourth_vertex_word & 0x8000000u) == 0 )
            carrier_v0 = r_u16(2 * v89 + fog_table);
w_u32(a0 + (0x18C), xport_gte_read_data(12));
w_u32(a0 + (0x14C), xport_gte_read_data(13));
w_u32(a0 + (0x1AC), xport_gte_read_data(14));
xport_gte_write_data(8, carrier_v0);
          v118 = (uint32)(ordering_base + 4 * (v84 + v89));
xport_gte_execute(0x780010);
          v119 = (sint16)r_u16(a0 + 332);
          v120 = (sint16)r_u16(a0 + 364);
          v121 = v119;
          v122 = v119 < v120;
          if ( v120 < v119 )
            v119 = v120;
          v123 = (sint16)r_u16(a0 + 396);
          if ( v122 )
            v121 = v120;
          if ( v123 < v119 )
            v119 = v123;
          v124 = (sint16)r_u16(a0 + 428);
          if ( v121 < v123 )
            v121 = v123;
          if ( v124 < v119 )
            v119 = v124;
          if ( v121 < v124 )
            v121 = v124;
          viewport_width = (sint16)r_u16(a0 + 4u);
          if ( v121 > 0 && v119 < viewport_width )
          {
            v125 = v87 | (1023 - (v121 - v119));
            v126 = (sint16)r_u16(a0 + 334);
            v127 = (sint16)r_u16(a0 + 366);
            v128 = v126;
            v129 = v126 < v127;
            if ( v127 < v126 )
              v126 = v127;
            v130 = (sint16)r_u16(a0 + 398);
            if ( v129 )
              v128 = v127;
            if ( v130 < v126 )
              v126 = v130;
            v131 = (sint16)r_u16(a0 + 430);
            if ( v128 < v130 )
              v128 = v130;
            if ( v131 < v126 )
              v126 = v131;
            if ( v128 < v131 )
              v128 = v131;
            viewport_height = (sint16)r_u16(a0 + 6u);
            if ( v128 > 0 && v126 < viewport_height )
            {
              if ( (v125 | (511 - (v128 - v126))) >= 0 )
              {
    w_u32(v68, 83886080);
w_u32(v68 + (4), xport_gte_read_data(22));
w_u32(v68 + (0x14), xport_gte_read_data(12));
w_u32(v68 + (8), xport_gte_read_data(13));
w_u32(v68 + (0x10), xport_gte_read_data(14));
                course_polygon_link(v118, (uint32)v68, (uint32)v68);
                v68 += 24;
              }
              else
              {
    w_u32(a0 + 956, carrier_t8);
    w_u32(a0 + 984, v66);
    w_u32(a0 + 976, v69);
    w_u32(a0 + 960, carrier_s4);
    w_u32(a0 + 964, carrier_s3);
    w_u32(a0 + 968, carrier_s2);
    w_u32(a0 + 168, v118);
    w_u32(a0 + 172, v68);
    w_u16(a0 + 184, 4);
w_u32(a0 + (0x9C), xport_gte_read_data(22));
w_u32(a0 + (0x1A4), xport_gte_read_data(0));
w_u32(a0 + (0x1A8), xport_gte_read_data(1));
w_u32(a0 + (0x184), xport_gte_read_data(2));
w_u32(a0 + (0x188), xport_gte_read_data(3));
w_u32(a0 + (0x144), xport_gte_read_data(4));
w_u32(a0 + (0x148), xport_gte_read_data(5));
                gtmain_polygon_clip_flat(a0);
                v68 = r_u32(a0 + 180);
                carrier_t8 = r_u32(a0 + 956);
                v66 = r_u32(a0 + 984);
                v69 = r_u32(a0 + 976);
                carrier_s4 = r_u32(a0 + 960);
                carrier_s3 = r_u32(a0 + 964);
                carrier_s2 = r_u32(a0 + 968);
              }
            }
          }
        }
      }
      carrier_t8 += 12;
    }
    while ( v66 );
    v4 = r_u32(a0 + 932);
    w_u32(a0 + 956, carrier_t8);
    w_u32(a0 + 952, v68);
  }
  v135 = r_u16(v4 + 52);
  carrier_t3 = r_u32(a0 + 956);
    w_u32(a0 + 932, v4);
  if ( v135 )
  {
    v137 = r_u32(a0 + 952);
    v138 = r_u32(carrier_t3);
    v139 = r_u32(a0 + 936);
    carrier_t8 = v139 + 8 * (v138 & 0x1FF);
    carrier_s2 = v139 + ((v138 >> 6) & 0xFF8);
    carrier_s1 = v139 + ((v138 >> 15) & 0xFF8);
    do
    {
      uint32 polygon_metadata, ordering_base;
      uint16 near_depth, depth_shift;
      sint32 viewport_width, viewport_height;
xport_gte_write_data(0, r_u32(carrier_s2 + (0)));
xport_gte_write_data(1, r_u32(carrier_s2 + (4)));
xport_gte_write_data(2, r_u32(carrier_t8 + (0)));
xport_gte_write_data(3, r_u32(carrier_t8 + (4)));
xport_gte_write_data(4, r_u32(carrier_s1 + (0)));
xport_gte_write_data(5, r_u32(carrier_s1 + (4)));
      --v135;
      v143 = v138;
xport_gte_execute(0x280030);
      v138 = r_u32(carrier_t3 + 4u * (5));
      polygon_metadata = r_u32(carrier_t3 + 4u);
      v144 = r_u32(a0 + 936);
      near_depth = r_u16(a0 + 8u);
      depth_shift = r_u16(a0 + 40u);
      carrier_t8 = v144 + 8 * (v138 & 0x1FF);
      carrier_s2 = v144 + ((v138 >> 6) & 0xFF8);
      carrier_s1 = v144 + ((v138 >> 15) & 0xFF8);
v56 = carrier_t6 = course_polygon_gte_flags();
v118 = carrier_t0 = xport_gte_read_data(17);
v144 = carrier_v1 = xport_gte_read_data(18);
v91 = carrier_a0 = xport_gte_read_data(19);
      ordering_base = r_u32(a0 + 948u);
xport_gte_execute(0x1400006);
      if ( carrier_t0 < carrier_v1 )
        carrier_t0 = carrier_v1;
      v149 = ((uint32)v143 >> 23) & 0x30;
      if ( carrier_t0 < carrier_a0 )
        carrier_t0 = carrier_a0;
      if ( carrier_t0 >= near_depth )
      {
v126 = carrier_v0 = xport_gte_read_data(24);
        v151 = carrier_v0 & v143;
        if ( (sint32)carrier_t6 < 0 )
        {
          v151 = 0;
          if ( v143 < 0 )
          {
w_u32(a0 + (0x164), xport_gte_read_data(0));
w_u32(a0 + (0x168), xport_gte_read_data(1));
w_u32(a0 + (0x144), xport_gte_read_data(2));
w_u32(a0 + (0x148), xport_gte_read_data(3));
w_u32(a0 + (0x184), xport_gte_read_data(4));
w_u32(a0 + (0x188), xport_gte_read_data(5));
            v151 = course_polygon_facing(a0, 324u, 388u, 356u);
          }
        }
        v177 = carrier_t0 >> (depth_shift & 31u);
        if ( v151 >= 0 )
        {
          if ( v177 >= 4096 )
            v177 = 4095;
          carrier_v1 = 0;
          if ( (polygon_metadata & 0x8000000u) == 0 )
            carrier_v1 = r_u16(2 * v177 + r_u32(a0 + 940));
w_u32(a0 + (0x16C), xport_gte_read_data(12));
w_u32(a0 + (0x14C), xport_gte_read_data(13));
w_u32(a0 + (0x18C), xport_gte_read_data(14));
          carrier_v0 = r_u32(carrier_t3 + 4u * (2));
xport_gte_write_data(21, r_u32(carrier_t3 + (0xC)));
xport_gte_write_data(22, r_u32(carrier_t3 + (0x10)));
xport_gte_write_data(6, carrier_v0);
xport_gte_write_data(20, carrier_v0);
xport_gte_write_data(8, carrier_v1);
          v180 = (uint32)(ordering_base + 4 * (v149 + v177));
xport_gte_execute(0xF8002A);
          v181 = (sint16)r_u16(a0 + 332);
          v182 = (sint16)r_u16(a0 + 364);
          v183 = (sint16)r_u16(a0 + 396);
          v184 = v181;
          v185 = v181 < v182;
          if ( v182 < v181 )
            v181 = v182;
          if ( v185 )
            v184 = v182;
          if ( v183 < v181 )
            v181 = v183;
          if ( v184 < v183 )
            v184 = v183;
          viewport_width = (sint16)r_u16(a0 + 4u);
          if ( v184 > 0 && v181 < viewport_width )
          {
            v186 = carrier_t6 | (1023 - (v184 - v181));
            v187 = (sint16)r_u16(a0 + 334);
            v188 = (sint16)r_u16(a0 + 366);
            v189 = (sint16)r_u16(a0 + 398);
            v190 = v187;
            v191 = v187 < v188;
            if ( v188 < v187 )
              v187 = v188;
            if ( v191 )
              v190 = v188;
            v192 = v190 < v189;
            if ( v189 < v187 )
              v187 = v189;
            if ( v190 < v189 )
              v190 = v189;
            viewport_height = (sint16)r_u16(a0 + 6u);
            if ( v190 > 0 && v187 < viewport_height )
            {
              if ( (v186 | (511 - (v190 - v187))) >= 0 )
              {
    w_u32(v137, 100663296);
w_u32(v137 + (4), xport_gte_read_data(20));
w_u32(v137 + (0xC), xport_gte_read_data(21));
w_u32(v137 + (0x14), xport_gte_read_data(22));
w_u32(v137 + (0x10), xport_gte_read_data(12));
w_u32(v137 + (8), xport_gte_read_data(13));
w_u32(v137 + (0x18), xport_gte_read_data(14));
                course_polygon_link(v180, (uint32)v137, (uint32)v137);
                v137 += 28;
              }
              else
              {
    w_u32(a0 + 956, carrier_t3);
    w_u32(a0 + 984, v135);
    w_u32(a0 + 976, v138);
    w_u32(a0 + 960, carrier_t8);
    w_u32(a0 + 964, carrier_s2);
    w_u32(a0 + 968, carrier_s1);
    w_u32(a0 + 168, v180);
    w_u32(a0 + 172, v137);
    w_u16(a0 + 184, 3);
w_u32(a0 + (0x9C), xport_gte_read_data(20));
w_u32(a0 + (0x154), xport_gte_read_data(20));
w_u32(a0 + (0x174), xport_gte_read_data(21));
w_u32(a0 + (0x194), xport_gte_read_data(22));
w_u32(a0 + (0x164), xport_gte_read_data(0));
w_u32(a0 + (0x168), xport_gte_read_data(1));
w_u32(a0 + (0x144), xport_gte_read_data(2));
w_u32(a0 + (0x148), xport_gte_read_data(3));
w_u32(a0 + (0x184), xport_gte_read_data(4));
w_u32(a0 + (0x188), xport_gte_read_data(5));
                gtmain_polygon_clip_gouraud(a0);
                v137 = r_u32(a0 + 180);
                carrier_t3 = r_u32(a0 + 956);
                v135 = r_u32(a0 + 984);
                v138 = r_u32(a0 + 976);
                carrier_t8 = r_u32(a0 + 960);
                carrier_s2 = r_u32(a0 + 964);
                carrier_s1 = r_u32(a0 + 968);
              }
            }
          }
        }
      }
      carrier_t3 += 20;
    }
    while ( v135 );
    v4 = r_u32(a0 + 932);
    w_u32(a0 + 956, carrier_t3);
    w_u32(a0 + 952, v137);
  }
  v196 = r_u16(v4 + 54);
  carrier_t4 = r_u32(a0 + 956);
    w_u32(a0 + 932, v4);
  if ( v196 )
  {
    v198 = r_u32(a0 + 952);
    v199 = r_u32(carrier_t4);
    v200 = r_u32(a0 + 936);
    carrier_s4 = v200 + 8 * (v199 & 0x1FF);
    carrier_s3 = v200 + ((v199 >> 6) & 0xFF8);
    carrier_t9 = v200 + ((v199 >> 15) & 0xFF8);
    do
    {
      uint32 fourth_vertex_word, fog_table, ordering_base;
      uint16 near_depth, depth_shift;
      sint32 viewport_width, viewport_height;
xport_gte_write_data(0, r_u32(carrier_s3 + (0)));
xport_gte_write_data(1, r_u32(carrier_s3 + (4)));
xport_gte_write_data(2, r_u32(carrier_t9 + (0)));
xport_gte_write_data(3, r_u32(carrier_t9 + (4)));
w_u32(a0 + (0x164), xport_gte_read_data(0));
w_u32(a0 + (0x168), xport_gte_read_data(1));
xport_gte_write_data(4, r_u32(carrier_s4 + (0)));
xport_gte_write_data(5, r_u32(carrier_s4 + (4)));
      v204 = r_u32(a0 + 936);
      v205 = v199;
xport_gte_execute(0x280030);
      fourth_vertex_word = r_u32(carrier_t4 + 4u);
      v199 = r_u32(carrier_t4 + 4u * (6));
      v206 = (uint32)(v204 + 8 * (fourth_vertex_word & 0x1FF));
      carrier_v1 = (sint32)r_u32(v206);
      carrier_a0 = (sint32)r_u32(v206 + 4u * (1));
w_u32(a0 + (0x16C), xport_gte_read_data(12));
w_u32(v198 + (0x10), xport_gte_read_data(12));
carrier_t3 = carrier_t3 = course_polygon_gte_flags();
      near_depth = r_u16(a0 + 8u);
      --v196;
xport_gte_execute(0x1400006);
      carrier_s4 = v204 + 8 * (v199 & 0x1FF);
      carrier_s3 = v204 + ((v199 >> 6) & 0xFF8);
xport_gte_write_data(0, carrier_v1);
xport_gte_write_data(1, carrier_a0);
carrier_a0 = carrier_a0 = xport_gte_read_data(24);
v177 = carrier_t0 = xport_gte_read_data(17);
carrier_v1 = carrier_v1 = xport_gte_read_data(18);
v189 = carrier_a2 = xport_gte_read_data(19);
      depth_shift = r_u16(a0 + 40u);
      fog_table = r_u32(a0 + 940u);
xport_gte_execute(0x180001);
      if ( carrier_t0 < carrier_v1 )
        carrier_t0 = carrier_v1;
      v214 = ((uint32)v205 >> 23) & 0x30;
      if ( carrier_t0 < carrier_a2 )
        carrier_t0 = carrier_a2;
      carrier_t9 = v204 + ((v199 >> 15) & 0xFF8);
xport_gte_write_data(6, r_u32(carrier_t4 + (8)));
v206 = carrier_v0 = course_polygon_gte_flags();
carrier_v1 = carrier_v1 = xport_gte_read_data(19);
      v217 = carrier_t3 | carrier_v0;
xport_gte_execute(0x1400006);
      v218 = (0u - carrier_a0);
      if ( carrier_t0 < carrier_v1 )
        carrier_t0 = carrier_v1;
      if ( carrier_t0 >= near_depth )
      {
        v219 = carrier_t0 >> (depth_shift & 31u);
        ordering_base = r_u32(a0 + 948u);
v206 = carrier_v0 = xport_gte_read_data(24);
        v221 = v205 & v218 & carrier_v0;
        if ( v217 < 0 )
        {
          v221 = 0;
          if ( v205 < 0 )
          {
w_u32(a0 + (0x184), xport_gte_read_data(2));
w_u32(a0 + (0x188), xport_gte_read_data(3));
w_u32(a0 + (0x144), xport_gte_read_data(4));
w_u32(a0 + (0x148), xport_gte_read_data(5));
            v221 = course_polygon_facing(a0, 324u, 388u, 356u);
          }
        }
        if ( v221 >= 0 )
        {
          if ( v219 >= 4096 )
            v219 = 4095;
          carrier_v0 = 0;
          if ( (fourth_vertex_word & 0x8000000u) == 0 )
            carrier_v0 = r_u16(2 * v219 + fog_table);
w_u32(a0 + (0x18C), xport_gte_read_data(12));
w_u32(a0 + (0x14C), xport_gte_read_data(13));
w_u32(a0 + (0x1AC), xport_gte_read_data(14));
xport_gte_write_data(8, carrier_v0);
          v248 = (uint32)(ordering_base + 4 * (v214 + v219));
xport_gte_execute(0x780010);
          carrier_v1 = r_u32(carrier_t4 + 4u * (3));
          carrier_a0 = r_u32(carrier_t4 + 4u * (4));
          carrier_a1 = r_u32(carrier_t4 + 4u * (5));
carrier_v0 = carrier_v0 = xport_gte_read_data(22);
xport_gte_write_data(20, carrier_v1);
xport_gte_write_data(21, carrier_a0);
xport_gte_write_data(22, carrier_a1);
    w_u32(a0 + 156, carrier_v0);
    w_u32(a0 + 340, carrier_v0);
xport_gte_execute(0xF8002A);
    w_u32(v198 + 4, carrier_v0);
          v253 = (sint16)r_u16(a0 + 332);
          v254 = (sint16)r_u16(a0 + 364);
          v255 = v253;
          v256 = v253 < v254;
          if ( v254 < v253 )
            v253 = v254;
          v257 = (sint16)r_u16(a0 + 396);
          if ( v256 )
            v255 = v254;
          if ( v257 < v253 )
            v253 = v257;
          v258 = (sint16)r_u16(a0 + 428);
          if ( v255 < v257 )
            v255 = v257;
          if ( v258 < v253 )
            v253 = v258;
          if ( v255 < v258 )
            v255 = v258;
          viewport_width = (sint16)r_u16(a0 + 4u);
          if ( v255 > 0 && v253 < viewport_width )
          {
            v259 = v217 | (1023 - (v255 - v253));
            v260 = (sint16)r_u16(a0 + 334);
            v261 = (sint16)r_u16(a0 + 366);
            v262 = v260;
            v263 = v260 < v261;
            if ( v261 < v260 )
              v260 = v261;
            v264 = (sint16)r_u16(a0 + 398);
            if ( v263 )
              v262 = v261;
            if ( v264 < v260 )
              v260 = v264;
            v265 = (sint16)r_u16(a0 + 430);
            if ( v262 < v264 )
              v262 = v264;
            if ( v265 < v260 )
              v260 = v265;
            if ( v262 < v265 )
              v262 = v265;
            viewport_height = (sint16)r_u16(a0 + 6u);
            if ( v262 > 0 && v260 < viewport_height )
            {
              if ( (v259 | (511 - (v262 - v260))) >= 0 )
              {
    w_u32(v198, 0x8000000);
w_u32(v198 + (0xC), xport_gte_read_data(20));
w_u32(v198 + (0x1C), xport_gte_read_data(21));
w_u32(v198 + (0x14), xport_gte_read_data(22));
w_u32(v198 + (0x20), xport_gte_read_data(12));
w_u32(v198 + (8), xport_gte_read_data(13));
w_u32(v198 + (0x18), xport_gte_read_data(14));
                course_polygon_link(v248, (uint32)v198, (uint32)v198);
                v198 += 36;
              }
              else
              {
    w_u32(a0 + 956, carrier_t4);
    w_u32(a0 + 984, v196);
    w_u32(a0 + 976, v199);
    w_u32(a0 + 960, carrier_s4);
    w_u32(a0 + 964, carrier_s3);
    w_u32(a0 + 968, carrier_t9);
    w_u32(a0 + 168, v248);
    w_u32(a0 + 172, v198);
    w_u16(a0 + 184, 4);
w_u32(a0 + (0x174), xport_gte_read_data(20));
w_u32(a0 + (0x194), xport_gte_read_data(21));
w_u32(a0 + (0x1B4), xport_gte_read_data(22));
w_u32(a0 + (0x1A4), xport_gte_read_data(0));
w_u32(a0 + (0x1A8), xport_gte_read_data(1));
w_u32(a0 + (0x184), xport_gte_read_data(2));
w_u32(a0 + (0x188), xport_gte_read_data(3));
w_u32(a0 + (0x144), xport_gte_read_data(4));
w_u32(a0 + (0x148), xport_gte_read_data(5));
                gtmain_polygon_clip_gouraud(a0);
                v198 = r_u32(a0 + 180);
                carrier_t4 = r_u32(a0 + 956);
                v196 = r_u32(a0 + 984);
                v199 = r_u32(a0 + 976);
                carrier_s4 = r_u32(a0 + 960);
                carrier_s3 = r_u32(a0 + 964);
                carrier_t9 = r_u32(a0 + 968);
              }
            }
          }
        }
      }
      carrier_t4 += 24;
    }
    while ( v196 );
    v4 = r_u32(a0 + 932);
    w_u32(a0 + 956, carrier_t4);
    w_u32(a0 + 952, v198);
  }
  v269 = r_u16(v4 + 56);
  carrier_t6 = r_u32(a0 + 956);
    w_u32(a0 + 932, v4);
  if ( v269 )
  {
    v271 = r_u32(a0 + 952);
    v272 = r_u32(carrier_t6);
    v273 = r_u32(a0 + 936);
    carrier_t9 = v273 + 8 * (v272 & 0x1FF);
    carrier_s3 = v273 + ((v272 >> 6) & 0xFF8);
    carrier_s2 = v273 + ((v272 >> 15) & 0xFF8);
    do
    {
      uint32 ordering_base;
      uint16 near_depth, depth_shift;
      sint16 viewport_width, viewport_height;
xport_gte_write_data(0, r_u32(carrier_s3 + (0)));
xport_gte_write_data(1, r_u32(carrier_s3 + (4)));
xport_gte_write_data(2, r_u32(carrier_t9 + (0)));
xport_gte_write_data(3, r_u32(carrier_t9 + (4)));
xport_gte_write_data(4, r_u32(carrier_s2 + (0)));
xport_gte_write_data(5, r_u32(carrier_s2 + (4)));
      --v269;
      v277 = v272;
xport_gte_execute(0x280030);
      v272 = r_u32(carrier_t6 + 4u * (3));
      v278 = r_u32(carrier_t6 + 4u * (1));
      v279 = r_u32(a0 + 936);
      near_depth = r_u16(a0 + 8u);
      depth_shift = r_u16(a0 + 40u);
      carrier_t9 = v279 + 8 * (v272 & 0x1FF);
      carrier_s3 = v279 + ((v272 >> 6) & 0xFF8);
      carrier_s2 = v279 + ((v272 >> 15) & 0xFF8);
carrier_t4 = carrier_t4 = course_polygon_gte_flags();
v248 = carrier_t0 = xport_gte_read_data(17);
v279 = carrier_v1 = xport_gte_read_data(18);
carrier_a0 = carrier_a0 = xport_gte_read_data(19);
      ordering_base = r_u32(a0 + 948u);
xport_gte_execute(0x1400006);
      if ( carrier_t0 < carrier_v1 )
        carrier_t0 = carrier_v1;
      v284 = ((uint32)v277 >> 23) & 0x30;
      if ( carrier_t0 < carrier_a0 )
        carrier_t0 = carrier_a0;
      if ( carrier_t0 >= near_depth )
      {
v135 = carrier_t7 = xport_gte_read_data(24);
        v286 = carrier_t7 & v277;
        if ( (sint32)carrier_t4 < 0 )
        {
          v286 = 0;
          if ( v277 < 0 )
          {
w_u32(a0 + (0x144), xport_gte_read_data(0));
w_u32(a0 + (0x148), xport_gte_read_data(1));
w_u32(a0 + (0x134), xport_gte_read_data(2));
w_u32(a0 + (0x138), xport_gte_read_data(3));
w_u32(a0 + (0x154), xport_gte_read_data(4));
w_u32(a0 + (0x158), xport_gte_read_data(5));
            v286 = course_polygon_facing(a0, 308u, 340u, 324u);
          }
        }
        v312 = carrier_t0 >> (depth_shift & 31u);
        if ( v286 >= 0 )
        {
          if ( v312 >= 4096 )
            v312 = 4095;
          carrier_a0 = 0;
          if ( (v278 & 0x8000000) == 0 )
            carrier_a0 = r_u16(2 * v312 + r_u32(a0 + 940));
          v314 = (uint32)(r_u32(a0 + 944) + 32u * ((uint32)v278 << 9 >> 18));
          if ( (sint32)carrier_t7 < 0 )
            carrier_t7 = (0u - carrier_t7);
          v315 = v284 + v312;
          v316 = (uint32)(v314 + 16);
          if ( r_u16(v314) < (uint32)carrier_t7 )
            v316 = (uint32)(v314 + 4);
w_u32(a0 + (0x14C), xport_gte_read_data(12));
w_u32(a0 + (0x13C), xport_gte_read_data(13));
w_u32(a0 + (0x15C), xport_gte_read_data(14));
xport_gte_write_data(6, r_u32(carrier_t6 + (8)));
xport_gte_write_data(8, carrier_a0);
          v317 = (uint32)(ordering_base + 4 * v315);
xport_gte_execute(0x780010);
          v318 = (sint16)r_u16(a0 + 316);
          v319 = (sint16)r_u16(a0 + 332);
          v320 = (sint16)r_u16(a0 + 348);
          v321 = v318;
          v322 = v318 < v319;
          if ( v319 < v318 )
            v318 = v319;
          if ( v322 )
            v321 = v319;
          if ( v320 < v318 )
            v318 = v320;
          if ( v321 < v320 )
            v321 = v320;
          viewport_width = (sint16)r_u16(a0 + 4u);
          if ( v321 > 0 && v318 < viewport_width )
          {
            v323 = carrier_t4 | (1023 - (v321 - v318));
            v324 = (sint16)r_u16(a0 + 318);
            v325 = (sint16)r_u16(a0 + 334);
            v326 = (sint16)r_u16(a0 + 350);
            v327 = v324;
            v328 = v324 < v325;
            if ( v325 < v324 )
              v324 = v325;
            if ( v328 )
              v327 = v325;
            if ( v326 < v324 )
              v324 = v326;
            if ( v327 < v326 )
              v327 = v326;
            viewport_height = (sint16)r_u16(a0 + 6u);
            if ( v327 > 0 && v324 < viewport_height )
            {
              if ( (sint32)carrier_t7 >= 800 )
                v323 = -1;
              v329 = (sint32)r_u32(v316);
              v330 = (sint32)r_u32(v316 + 4u * (1));
              v331 = (sint32)r_u32(v316 + 4u * (2));
              if ( (v323 | (511 - (v327 - v324))) >= 0 )
              {
    w_u32(v271, 117440512);
    w_u32(v271 + 12, v329);
    w_u32(v271 + 20, v330);
    w_u32(v271 + 28, v331);
w_u32(v271 + (4), xport_gte_read_data(22));
w_u32(v271 + (0x10), xport_gte_read_data(12));
w_u32(v271 + (8), xport_gte_read_data(13));
w_u32(v271 + (0x18), xport_gte_read_data(14));
                course_polygon_link(v317, v271, v271);
                v271 += 32;
              }
              else
              {
    w_u16(a0 + 188, 5);
                uint32 subdivision_depth;
                carrier_t4 = a0;
    w_u32(a0 + 956, carrier_t6);
    w_u32(a0 + 984, v269);
    w_u32(a0 + 976, v272);
    w_u32(a0 + 960, carrier_t9);
    w_u32(a0 + 964, carrier_s3);
    w_u32(a0 + 968, carrier_s2);
    w_u32(a0 + 168, v317);
    w_u32(a0 + 172, v271);
    w_u32(a0 + 184, 1152);
    w_u16(a0 + 306, v329);
    w_u16(a0 + 322, v330);
    w_u16(a0 + 338, v331);
w_u32(a0 + (0x9C), xport_gte_read_data(22));
    w_u32(a0 + 164, v330);
    w_masked_u32(a0 + 164u, (uint32)v329 >> 16, 3u);
w_u32(a0 + (0x144), xport_gte_read_data(0));
w_u32(a0 + (0x148), xport_gte_read_data(1));
w_u32(a0 + (0x134), xport_gte_read_data(2));
w_u32(a0 + (0x138), xport_gte_read_data(3));
w_u32(a0 + (0x154), xport_gte_read_data(4));
w_u32(a0 + (0x158), xport_gte_read_data(5));
xport_gte_write_data(0, r_u32(carrier_t4 + (0x134)));
xport_gte_write_data(1, r_u32(carrier_t4 + (0x138)));
                v333 = -1;
                carrier_t1 = (uint32)(a0 + 304);
xport_gte_execute(0x180001);
                v335 = 0;
                v336 = 0;
                v337 = r_u16(a0 + 8);
                v338 = (sint16)r_u16(a0 + 4);
                v339 = (sint16)r_u16(a0 + 6);
                v340 = r_u32(a0 + 172);
    w_u32(a0 + 208, a0 + 320);
    w_u32(a0 + 212, a0 + 336);
                v341 = r_u32(a0 + 156);
                v342 = 3;
    w_u32(a0 + 204, a0 + 304);
    w_u32(a0 + 176, v340);
    w_u32(a0 + 180, v340);
    w_u32(a0 + 160, v341 | 0x8000000);
                do
                {
v317 = carrier_t3 = course_polygon_gte_flags();
v331 = carrier_a1 = xport_gte_read_data(11);
w_u32(carrier_t1 + (0xC), xport_gte_read_data(14));
xport_gte_write_data(0, r_u32(carrier_t1 + (0x14)));
xport_gte_write_data(1, r_u32(carrier_t1 + (0x18)));
                  v345 = (sint16)r_u16(carrier_t1 + 2u * (6));
                  v346 = (sint16)r_u16(carrier_t1 + 2u * (7));
xport_gte_execute(0x180001);
                  --v342;
                  v347 = (sint32)carrier_a1 < v337;
                  if ( v345 < 0 )
                    v347 |= 2u;
                  v348 = v347 | (4 * (v338 < v345));
                  if ( v346 < 0 )
                    v348 |= 8u;
                  v349 = v348 | (16 * (v339 < v346));
                  if ( (sint32)carrier_t3 < 0 )
                    v349 |= 0x20u;
                  w_u16(carrier_t1, v349);
                  if ( (sint32)carrier_a1 < v336 )
                    v336 = carrier_a1;
                  v333 &= v349;
                  if ( v335 < (sint32)carrier_a1 )
                    v335 = carrier_a1;
                  carrier_t1 += 16;
                }
                while ( v342 );
                subdivision_depth = r_u16(a0 + 188u);
                v350 = 0;
                if ( (v333 & 0x1F) == 0 )
                {
                  v351 = (uint32)(a0 + 204);
                  if ( v335 - ((v335 - v336) >> ((subdivision_depth + 1u) & 31u)) >= v337 )
                  {
LABEL_244:
                    v352 = r_u32(v351);
                    v353 = r_u32(v351 + 4u * (1));
                    v354 = r_u32(v351 + 4u * (2));
                    v355 = (sint16)r_u16(v352 + 4);
                    v356 = (sint16)r_u16(v353 + 4);
                    v357 = (sint16)r_u16(v354 + 4);
                    w_u16(carrier_t1 + 2u * (2), (v355 + v356) >> 1);
                    w_u16(carrier_t1 + 2u * (10), (v356 + v357) >> 1);
                    w_u16(carrier_t1 + 2u * (18), (v357 + v355) >> 1);
                    v358 = (sint16)r_u16(v352 + 6);
                    v359 = (sint16)r_u16(v353 + 6);
                    v360 = (sint16)r_u16(v354 + 6);
                    w_u16(carrier_t1 + 2u * (3), (v358 + v359) >> 1);
                    w_u16(carrier_t1 + 2u * (11), (v359 + v360) >> 1);
                    w_u16(carrier_t1 + 2u * (19), (v360 + v358) >> 1);
                    v361 = (sint16)r_u16(v352 + 8);
                    v362 = (sint16)r_u16(v353 + 8);
                    v363 = (sint16)r_u16(v354 + 8);
                    w_u16(carrier_t1 + 2u * (4), (v361 + v362) >> 1);
                    w_u16(carrier_t1 + 2u * (12), (v362 + v363) >> 1);
                    w_u16(carrier_t1 + 2u * (20), (v363 + v361) >> 1);
xport_gte_write_data(0, r_u32(carrier_t1 + (4)));
xport_gte_write_data(1, r_u32(carrier_t1 + (8)));
                    v364 = (sint16)r_u16(a0 + 4);
                    v365 = (sint16)r_u16(a0 + 6);
xport_gte_execute(0x180001);
                    v366 = r_u8(v352 + 2);
                    v367 = r_u8(v353 + 2);
                    v368 = r_u8(v354 + 2);
                    w_u8(carrier_t1 + 2u, (v366 + v367) >> 1);
                    w_u8(carrier_t1 + 18u, (v367 + v368) >> 1);
                    w_u8(carrier_t1 + 34u, (v368 + v366) >> 1);
                    v369 = r_u8(v352 + 3);
                    v370 = 3;
                    v371 = r_u8(v353 + 3);
                    v372 = r_u8(v354 + 3);
                    w_u8(carrier_t1 + 3u, (v369 + v371) >> 1);
                    w_u8(carrier_t1 + 19u, (v371 + v372) >> 1);
                    w_u8(carrier_t1 + 35u, (v372 + v369) >> 1);
                    v373 = r_u16(a0 + 8);
                    do
                    {
v353 = carrier_a3 = course_polygon_gte_flags();
v345 = carrier_v0 = xport_gte_read_data(19);
w_u32(carrier_t1 + (0xC), xport_gte_read_data(14));
xport_gte_write_data(0, r_u32(carrier_t1 + (0x14)));
xport_gte_write_data(1, r_u32(carrier_t1 + (0x18)));
                      v376 = (sint16)r_u16(carrier_t1 + 2u * (6));
                      v377 = (sint16)r_u16(carrier_t1 + 2u * (7));
xport_gte_execute(0x180001);
                      --v370;
                      v378 = carrier_v0 < v373;
                      if ( v376 < 0 )
                        v378 |= 2u;
                      v379 = v378 | (4 * (v364 < v376));
                      if ( v377 < 0 )
                        v379 |= 8u;
                      v380 = v379 | (16 * (v365 < v377));
                      if ( (sint32)carrier_a3 < 0 )
                        v380 |= 0x20u;
                      w_u16(carrier_t1, v380);
                      carrier_t1 += 16;
                    }
                    while ( v370 );
                    v381 = carrier_t1 - 48;
                    v382 = r_u32(a0 + 180);
                    v383 = r_u32(a0 + 156);
                    v384 = (uint32)r_u16(a0 + 164u) << 16;
                    v385 = r_u32(a0 + 164);
                    v386 = 2;
                    v387 = 1;
                    do
                    {
                      uint32 first_flags, third_flags;
                      carrier_t0 = (uint32)r_u32(v351 + 4u * (v386));
                      v389 = 8 * v387;
                      if ( v387 < 0 )
                      {
                        v387 = 2;
                        v389 = 16;
                      }
                      carrier_a3 = (v381 + 2u * (v389));
                      carrier_a2 = (uint32)r_u32(v351 + 4u * (v387));
                      first_flags = r_u16(carrier_t0);
                      v392 = r_u16(carrier_a3);
                      third_flags = r_u16(carrier_a2);
                      v393 = (first_flags | (uint32)v392 | third_flags) & 0x20u;
                      --v386;
                      if ( v393 == 0 )
                      {
xport_gte_write_data(12, r_u32(carrier_t0 + (0xC)));
xport_gte_write_data(13, r_u32(carrier_a3 + (0xC)));
xport_gte_write_data(14, r_u32(carrier_a2 + (0xC)));
    w_u8(v382 + 3, 7);
    v393 = (v393 & 0xFFFF0000u) | (((uint32)(r_u16(carrier_a2 + 2u)) & 0xFFFFu) << 0);
xport_gte_execute(0x1400006);
    v384 = (v384 & 0xFFFF0000u) | (((uint32)(r_u16(carrier_t0 + 2u)) & 0xFFFFu) << 0);
    w_u32(v382 + 28, v393);
    v385 = (v385 & 0xFFFF0000u) | (((uint32)(r_u16(carrier_a3 + 2u)) & 0xFFFFu) << 0);
    w_u32(v382 + 12, v384);
    w_u32(v382 + 20, v385);
                        w_masked_u32(v382, (uint32)v382 + 32u, 7u);
v394 = carrier_v0 = xport_gte_read_data(24);
w_u32(v382 + (8), xport_gte_read_data(12));
w_u32(v382 + (0x10), xport_gte_read_data(13));
                        if ( carrier_v0 )
                        {
w_u32(v382 + (0x18), xport_gte_read_data(14));
    w_u32(v382 + 4, v383);
    w_u32(a0 + 176, v382);
                          v382 += 32;
                        }
                      }
                      v387 = v386 - 1;
                    }
                    while ( v386 >= 0 );
    w_u32(a0 + 180, v382);
                    v396 = 3;
                    do
                    {
                      while ( 1 )
                      {
                        uint32 first_flags, second_flags, third_flags;
                        sint32 threshold;
                        v400 = v396 - 1;
                        if ( v396 < 0 )
                          break;
                        if ( v396 == 3 )
                        {
                          carrier_t0 = v381;
                          carrier_t3 = v381 + 16;
                          carrier_t2 = v381 + 32;
                        }
                        else
                        {
                          carrier_t0 = (uint32)r_u32(v351 + 4u * (v396));
                          carrier_t3 = (v381 + 2u * (8 * v396));
                          if ( v400 < 0 )
                            v400 = 2;
                          carrier_t2 = (v381 + 2u * (8 * v400));
                        }
                        first_flags = r_u16(carrier_t0);
                        second_flags = r_u16(carrier_t3);
                        third_flags = r_u16(carrier_t2);
                        --v396;
                        if ( (first_flags & second_flags & third_flags & 0x1Fu) == 0 )
                        {
                          v401 = v350 < r_u16(a0 + 188);
                          if ( ((first_flags | second_flags | third_flags) & 0x20u) != 0 )
                          {
    w_u32(v351 + 4u * (3), carrier_t0);
                            if ( v401 )
                              goto LABEL_268;
                          }
                          else
                          {
                            v403 = r_u32(a0 + 180);
xport_gte_write_data(12, r_u32(carrier_t3 + (0xC)));
xport_gte_write_data(13, r_u32(carrier_t0 + (0xC)));
xport_gte_write_data(14, r_u32(carrier_t2 + (0xC)));
                            v404 = (uint32)r_u16(a0 + 164u) << 16;
                            v405 = r_u32(a0 + 164);
xport_gte_execute(0x1400006);
                            v406 = r_u32(a0 + 156);
    v404 = (v404 & 0xFFFF0000u) | (((uint32)(r_u16(carrier_t3 + 2u)) & 0xFFFFu) << 0);
    w_u32(v403 + 4, v406);
    v405 = (v405 & 0xFFFF0000u) | (((uint32)(r_u16(carrier_t0 + 2u)) & 0xFFFFu) << 0);
    w_u32(v403 + 12, v404);
    v406 = (v406 & 0xFFFF0000u) | (((uint32)(r_u16(carrier_t2 + 2u)) & 0xFFFFu) << 0);
    w_u32(v403 + 20, v405);
    w_u32(v403 + 28, v406);
v406 = carrier_v0 = xport_gte_read_data(24);
                            threshold = (sint32)r_u32(a0 + 184u);
w_u32(v403 + (8), xport_gte_read_data(12));
                            if ( (sint32)carrier_v0 < 0 )
                              carrier_v0 = (0u - carrier_v0);
                            v408 = threshold < (sint32)carrier_v0 && v401;
    w_u32(v351 + 4u * (3), carrier_t0);
                            if ( v408 )
                            {
LABEL_268:
                              v351 += 12;
                              carrier_t1 = v381 + 48;
                              v402 = a0 + v350++;
    w_u8(v402 + 196, v396);
    w_u32(v351 + 4u * (1), carrier_t3);
    w_u32(v351 + 4u * (2), carrier_t2);
                              goto LABEL_244;
                            }
w_u32(v403 + (0x10), xport_gte_read_data(13));
w_u32(v403 + (0x18), xport_gte_read_data(14));
    w_u8(v403 + 3, 7);
                            w_masked_u32(v403, (uint32)v403 + 32u, 7u);
    w_u32(a0 + 176, v403);
    w_u32(a0 + 180, v403 + 32);
                          }
                        }
                      }
                      --v350;
                      v396 = (sint8)r_u8(a0 + v350 + 196);
                      v351 -= 12;
                      v381 -= 48;
                    }
                    while ( v350 >= 0 );
                    v410 = r_u32(a0 + 168);
                    v411 = r_u32(a0 + 172);
                    uint32 chain_end = r_u32(a0 + 180u);
                    v412 = r_u32(a0 + 176);
                    if ( (uint32)v411 != chain_end )
                    {
                      course_polygon_link(v410, (uint32)v411, (uint32)v412);
                    }
                  }
                }
                v271 = r_u32(a0 + 180);
                carrier_t6 = r_u32(a0 + 956);
                v269 = r_u32(a0 + 984);
                v272 = r_u32(a0 + 976);
                carrier_t9 = r_u32(a0 + 960);
                carrier_s3 = r_u32(a0 + 964);
                carrier_s2 = r_u32(a0 + 968);
              }
            }
          }
        }
      }
      carrier_t6 += 12;
    }
    while ( v269 );
    v4 = r_u32(a0 + 932);
    w_u32(a0 + 956, carrier_t6);
    w_u32(a0 + 952, v271);
  }
  v420 = r_u16(v4 + 58);
  carrier_t8 = r_u32(a0 + 956);
    w_u32(a0 + 932, v4);
  if ( v420 )
  {
    v422 = r_u32(a0 + 952);
    v423 = r_u32(carrier_t8);
    v424 = r_u32(a0 + 936);
    carrier_t9 = v424 + 8 * (v423 & 0x1FF);
    carrier_s4 = v424 + ((v423 >> 6) & 0xFF8);
    carrier_s3 = v424 + ((v423 >> 15) & 0xFF8);
    do
    {
      uint16 near_depth, depth_shift;
      uint32 fog_table, ordering_base;
      sint16 viewport_width, viewport_height;
xport_gte_write_data(0, r_u32(carrier_s4 + (0)));
xport_gte_write_data(1, r_u32(carrier_s4 + (4)));
xport_gte_write_data(2, r_u32(carrier_s3 + (0)));
xport_gte_write_data(3, r_u32(carrier_s3 + (4)));
w_u32(a0 + (0x144), xport_gte_read_data(0));
w_u32(a0 + (0x148), xport_gte_read_data(1));
xport_gte_write_data(4, r_u32(carrier_t9 + (0)));
xport_gte_write_data(5, r_u32(carrier_t9 + (4)));
      v428 = r_u32(a0 + 936);
      v429 = v423;
xport_gte_execute(0x280030);
      v430 = r_u32(carrier_t8 + 4u * (1));
      v423 = r_u32(carrier_t8 + 4u * (3));
      v431 = (uint32)(v428 + 8 * (v430 & 0x1FF));
      carrier_v1 = (sint32)r_u32(v431);
      carrier_a0 = (sint32)r_u32(v431 + 4u * (1));
w_u32(a0 + (0x14C), xport_gte_read_data(12));
w_u32(v422 + (0x10), xport_gte_read_data(12));
carrier_t3 = carrier_t3 = course_polygon_gte_flags();
      near_depth = r_u16(a0 + 8u);
      --v420;
xport_gte_execute(0x1400006);
      carrier_t9 = v428 + 8 * (v423 & 0x1FF);
      carrier_s4 = v428 + ((v423 >> 6) & 0xFF8);
xport_gte_write_data(0, carrier_v1);
xport_gte_write_data(1, carrier_a0);
carrier_t2 = carrier_t2 = xport_gte_read_data(24);
carrier_t0 = carrier_t0 = xport_gte_read_data(17);
carrier_v1 = carrier_v1 = xport_gte_read_data(18);
carrier_a0 = carrier_a0 = xport_gte_read_data(19);
      depth_shift = r_u16(a0 + 40u);
      fog_table = r_u32(a0 + 940u);
xport_gte_execute(0x180001);
      if ( carrier_t0 < carrier_v1 )
        carrier_t0 = carrier_v1;
      v439 = ((uint32)v429 >> 23) & 0x30;
      if ( carrier_t0 < carrier_a0 )
        carrier_t0 = carrier_a0;
      carrier_s3 = v428 + ((v423 >> 15) & 0xFF8);
xport_gte_write_data(6, r_u32(carrier_t8 + (8)));
v431 = carrier_v0 = course_polygon_gte_flags();
carrier_v1 = carrier_v1 = xport_gte_read_data(19);
      v442 = carrier_t3 | carrier_v0;
xport_gte_execute(0x1400006);
      v443 = (0u - carrier_t2);
      if ( carrier_t0 < carrier_v1 )
        carrier_t0 = carrier_v1;
      if ( carrier_t0 >= near_depth )
      {
        v444 = carrier_t0 >> (depth_shift & 31u);
        ordering_base = r_u32(a0 + 948u);
carrier_t4 = carrier_t4 = xport_gte_read_data(24);
        v446 = v429 & v443 & carrier_t4;
        if ( v442 < 0 )
        {
          v446 = 0;
          if ( v429 < 0 )
          {
w_u32(a0 + (0x154), xport_gte_read_data(2));
w_u32(a0 + (0x158), xport_gte_read_data(3));
w_u32(a0 + (0x134), xport_gte_read_data(4));
w_u32(a0 + (0x138), xport_gte_read_data(5));
            v446 = course_polygon_facing(a0, 308u, 340u, 324u);
          }
        }
        if ( v446 >= 0 )
        {
          if ( v444 >= 4096 )
            v444 = 4095;
          carrier_v0 = 0;
          if ( (v430 & 0x8000000) == 0 )
            carrier_v0 = r_u16(2 * v444 + fog_table);
w_u32(a0 + (0x15C), xport_gte_read_data(12));
w_u32(a0 + (0x13C), xport_gte_read_data(13));
w_u32(a0 + (0x16C), xport_gte_read_data(14));
xport_gte_write_data(8, carrier_v0);
          v473 = (uint32)(ordering_base + 4 * (v439 + v444));
xport_gte_execute(0x780010);
          if ( v443 < 0 )
            v443 = (sint32)(0u - (uint32)v443);
          v474 = (uint32)(r_u32(a0 + 944) + 32 * (v430 << 9 >> 18));
          if ( (sint32)carrier_t4 < 0 )
            carrier_t4 = (0u - carrier_t4);
          v475 = v443 + carrier_t4;
          v476 = (uint32)(v474 + 16);
          if ( r_u16(v474) < (uint32)v475 )
            v476 = (uint32)(v474 + 4);
          v477 = (sint16)r_u16(a0 + 316);
          v478 = (sint16)r_u16(a0 + 332);
          v479 = v477;
          v480 = v477 < v478;
          if ( v478 < v477 )
            v477 = v478;
          v481 = (sint16)r_u16(a0 + 348);
          if ( v480 )
            v479 = v478;
          if ( v481 < v477 )
            v477 = v481;
          v482 = (sint16)r_u16(a0 + 364);
          if ( v479 < v481 )
            v479 = v481;
          if ( v482 < v477 )
            v477 = v482;
          if ( v479 < v482 )
            v479 = v482;
          viewport_width = (sint16)r_u16(a0 + 4u);
          if ( v479 > 0 && v477 < viewport_width )
          {
            v483 = v442 | (1023 - (v479 - v477));
            v484 = (sint16)r_u16(a0 + 318);
            v485 = (sint16)r_u16(a0 + 334);
            v486 = v484;
            v487 = v484 < v485;
            if ( v485 < v484 )
              v484 = v485;
            v488 = (sint16)r_u16(a0 + 350);
            if ( v487 )
              v486 = v485;
            if ( v488 < v484 )
              v484 = v488;
            v489 = (sint16)r_u16(a0 + 366);
            if ( v486 < v488 )
              v486 = v488;
            if ( v489 < v484 )
              v484 = v489;
            if ( v486 < v489 )
              v486 = v489;
            viewport_height = (sint16)r_u16(a0 + 6u);
            if ( v486 > 0 && v484 < viewport_height )
            {
              if ( v475 >= 1568 )
                v483 = -1;
              v490 = (sint32)r_u32(v476);
              v491 = (sint32)r_u32(v476 + 4u * (1));
              v492 = (sint32)r_u32(v476 + 4u * (2));
              if ( (v483 | (511 - (v486 - v484))) >= 0 )
              {
    w_u32(v422, 150994944);
    w_u32(v422 + 12, v490);
    w_u32(v422 + 20, v491);
    w_u32(v422 + 36, v492);
    w_masked_u32(v422 + 28u, (uint32)v492 >> 16, 3u);
w_u32(v422 + (4), xport_gte_read_data(22));
w_u32(v422 + (0x20), xport_gte_read_data(12));
w_u32(v422 + (8), xport_gte_read_data(13));
w_u32(v422 + (0x18), xport_gte_read_data(14));
                course_polygon_link(v473, v422, v422);
                v422 += 40;
              }
              else
              {
    w_u16(a0 + 188, 5);
                uint32 subdivision_depth;
                carrier_t7 = a0;
    w_u32(a0 + 956, carrier_t8);
    w_u32(a0 + 984, v420);
    w_u32(a0 + 976, v423);
    w_u32(a0 + 960, carrier_t9);
    w_u32(a0 + 964, carrier_s4);
    w_u32(a0 + 968, carrier_s3);
    w_u32(a0 + 168, v473);
    w_u32(a0 + 172, v422);
    w_u32(a0 + 184, 2048);
w_u32(a0 + (0x9C), xport_gte_read_data(22));
    w_u32(a0 + 164, v491);
    w_masked_u32(a0 + 164u, (uint32)v490 >> 16, 3u);
    w_u16(a0 + 306, v490);
    w_u16(a0 + 322, v491);
    w_u16(a0 + 338, v492);
    w_u32(a0 + 352, v492);
w_u32(a0 + (0x164), xport_gte_read_data(0));
w_u32(a0 + (0x168), xport_gte_read_data(1));
w_u32(a0 + (0x154), xport_gte_read_data(2));
w_u32(a0 + (0x158), xport_gte_read_data(3));
w_u32(a0 + (0x134), xport_gte_read_data(4));
w_u32(a0 + (0x138), xport_gte_read_data(5));
xport_gte_write_data(0, r_u32(carrier_t7 + (0x134)));
xport_gte_write_data(1, r_u32(carrier_t7 + (0x138)));
                v494 = -1;
                carrier_t3 = (uint32)(a0 + 304);
xport_gte_execute(0x180001);
                v496 = 0;
                v497 = 0;
                v498 = 4;
                v499 = r_u16(a0 + 8);
                v500 = (sint16)r_u16(a0 + 4);
                v501 = (sint16)r_u16(a0 + 6);
                v502 = r_u32(a0 + 172);
                v503 = r_u32(a0 + 156);
    w_u32(a0 + 208, a0 + 320);
    w_u32(a0 + 212, a0 + 336);
    w_u32(a0 + 204, a0 + 304);
    w_u32(a0 + 216, a0 + 352);
    w_u32(a0 + 176, v502);
    w_u32(a0 + 180, v502);
    w_u32(a0 + 160, v503 & 0xF7FFFFFF);
                do
                {
v422 = carrier_t1 = course_polygon_gte_flags();
w_u32(carrier_t3 + (0xC), xport_gte_read_data(14));
v491 = carrier_a1 = xport_gte_read_data(11);
xport_gte_write_data(0, r_u32(carrier_t3 + (0x14)));
xport_gte_write_data(1, r_u32(carrier_t3 + (0x18)));
                  v506 = (sint16)r_u16(carrier_t3 + 2u * (6));
                  v507 = (sint16)r_u16(carrier_t3 + 2u * (7));
xport_gte_execute(0x180001);
                  --v498;
                  v508 = (sint32)carrier_a1 < v499;
                  if ( v506 < 0 )
                    v508 |= 2u;
                  v509 = v508 | (4 * (v500 < v506));
                  if ( v507 < 0 )
                    v509 |= 8u;
                  v510 = v509 | (16 * (v501 < v507));
                  if ( (sint32)carrier_t1 < 0 )
                    v510 |= 0x20u;
                  w_u16(carrier_t3, v510);
                  if ( (sint32)carrier_a1 < v497 )
                    v497 = carrier_a1;
                  v494 &= v510;
                  if ( v496 < (sint32)carrier_a1 )
                    v496 = carrier_a1;
                  carrier_t3 += 16;
                }
                while ( v498 );
                subdivision_depth = r_u16(a0 + 188u);
                v511 = 0;
                if ( (v494 & 0x1F) == 0 )
                {
                  v512 = (uint32)(a0 + 204);
                  if ( v496 - ((v496 - v497) >> ((subdivision_depth + 1u) & 31u)) >= v499 )
                  {
LABEL_350:
                    v513 = r_u32(v512 + 4u * (1));
                    v514 = r_u32(v512 + 4u * (2));
                    v515 = r_u32(v512);
                    v516 = r_u32(v512 + 4u * (3));
                    v517 = (sint16)r_u16(v513 + 4);
                    v518 = (sint16)r_u16(v514 + 4);
                    v519 = (sint16)r_u16(v515 + 4);
                    v520 = (sint16)r_u16(v516 + 4);
                    w_u16(carrier_t3 + 2u * (18), (v517 + v518) >> 1);
                    v521 = (v520 + v519) >> 1;
                    v522 = v519 + v517;
                    w_u16(carrier_t3 + 2u * (34), v521);
                    w_u16(carrier_t3 + 2u * (10), v522 >> 1);
                    w_u16(carrier_t3 + 2u * (26), (v518 + v520) >> 1);
                    w_u16(carrier_t3 + 2u * (2), (v522 + v518 + v520) >> 2);
                    v523 = (sint16)r_u16(v513 + 6);
                    v524 = (sint16)r_u16(v514 + 6);
                    v525 = (sint16)r_u16(v515 + 6);
                    v526 = (sint16)r_u16(v516 + 6);
                    w_u16(carrier_t3 + 2u * (19), (v523 + v524) >> 1);
                    v527 = (v526 + v525) >> 1;
                    v528 = v525 + v523;
                    w_u16(carrier_t3 + 2u * (35), v527);
                    w_u16(carrier_t3 + 2u * (11), v528 >> 1);
                    w_u16(carrier_t3 + 2u * (27), (v524 + v526) >> 1);
                    w_u16(carrier_t3 + 2u * (3), (v528 + v524 + v526) >> 2);
                    v529 = (sint16)r_u16(v513 + 8);
                    v530 = (sint16)r_u16(v514 + 8);
                    v531 = (sint16)r_u16(v515 + 8);
                    v532 = (sint16)r_u16(v516 + 8);
                    w_u16(carrier_t3 + 2u * (20), (v529 + v530) >> 1);
                    v533 = (v532 + v531) >> 1;
                    v534 = v531 + v529;
                    w_u16(carrier_t3 + 2u * (36), v533);
                    w_u16(carrier_t3 + 2u * (12), v534 >> 1);
                    w_u16(carrier_t3 + 2u * (28), (v530 + v532) >> 1);
                    w_u16(carrier_t3 + 2u * (4), (v534 + v530 + v532) >> 2);
xport_gte_write_data(0, r_u32(carrier_t3 + (4)));
xport_gte_write_data(1, r_u32(carrier_t3 + (8)));
                    v535 = (sint16)r_u16(a0 + 4);
                    v536 = (sint16)r_u16(a0 + 6);
xport_gte_execute(0x180001);
                    v537 = r_u8(v513 + 2);
                    v538 = r_u8(v514 + 2);
                    v539 = r_u8(v515 + 2);
                    v540 = r_u8(v516 + 2);
                    w_u8(carrier_t3 + 34u, (v537 + v538) >> 1);
                    v541 = (v540 + v539) >> 1;
                    v542 = v539 + v537;
                    w_u8(carrier_t3 + 66u, v541);
                    w_u8(carrier_t3 + 18u, v542 >> 1);
                    w_u8(carrier_t3 + 50u, (v538 + v540) >> 1);
                    w_u8(carrier_t3 + 2u, (v542 + v538 + v540) >> 2);
                    v543 = r_u8(v513 + 3);
                    v544 = 5;
                    v545 = r_u8(v514 + 3);
                    v546 = r_u8(v515 + 3);
                    v547 = r_u8(v516 + 3);
                    w_u8(carrier_t3 + 35u, (v543 + v545) >> 1);
                    v548 = (v547 + v546) >> 1;
                    v549 = v546 + v543;
                    w_u8(carrier_t3 + 67u, v548);
                    w_u8(carrier_t3 + 19u, v549 >> 1);
                    w_u8(carrier_t3 + 51u, (v545 + v547) >> 1);
                    w_u8(carrier_t3 + 3u, (v549 + v545 + v547) >> 2);
                    v550 = r_u16(a0 + 8);
                    do
                    {
v543 = carrier_a2 = course_polygon_gte_flags();
w_u32(carrier_t3 + (0xC), xport_gte_read_data(14));
v548 = carrier_v0 = xport_gte_read_data(11);
xport_gte_write_data(0, r_u32(carrier_t3 + (0x14)));
xport_gte_write_data(1, r_u32(carrier_t3 + (0x18)));
                      v553 = (sint16)r_u16(carrier_t3 + 2u * (6));
                      v554 = (sint16)r_u16(carrier_t3 + 2u * (7));
xport_gte_execute(0x180001);
                      --v544;
                      v555 = (sint32)carrier_v0 < v550;
                      if ( v553 < 0 )
                        v555 |= 2u;
                      v556 = v555 | (4 * (v535 < v553));
                      if ( v554 < 0 )
                        v556 |= 8u;
                      v557 = v556 | (16 * (v536 < v554));
                      if ( (sint32)carrier_a2 < 0 )
                        v557 |= 0x20u;
                      w_u16(carrier_t3, v557);
                      carrier_t3 += 16;
                    }
                    while ( v544 );
                    v558 = carrier_t3 - 80;
                    v559 = r_u32(a0 + 180);
                    v560 = (uint32)r_u16(a0 + 164u) << 16;
                    v561 = r_u32(a0 + 164);
                    v562 = 3;
                    do
                    {
                      uint32 first_flags, second_flags, third_flags;
                      carrier_a2 = (uint32)r_u32(v512 + 4u * (1));
                      carrier_a3 = r_u32(v512);
                      carrier_t0 = v558 + 16;
                      if ( !v562 )
                        carrier_a2 = r_u32(v512 - 12u);
                      first_flags = r_u16(carrier_a3);
                      second_flags = r_u16(carrier_a2);
                      third_flags = r_u16(carrier_t0);
                      --v562;
                      if ( ((first_flags | second_flags | third_flags) & 0x20u) == 0 )
                      {
xport_gte_write_data(12, r_u32(carrier_a3 + (0xC)));
xport_gte_write_data(13, r_u32(carrier_a2 + (0xC)));
xport_gte_write_data(14, r_u32(carrier_t0 + (0xC)));
                        v566 = v559 + 32;
xport_gte_execute(0x1400006);
                        v567 = r_u32(a0 + 160);
    v560 = (v560 & 0xFFFF0000u) | (((uint32)(r_u16(carrier_a3 + 2u)) & 0xFFFFu) << 0);
    w_u32(v559 + 4, v567);
    v561 = (v561 & 0xFFFF0000u) | (((uint32)(r_u16(carrier_a2 + 2u)) & 0xFFFFu) << 0);
    w_u32(v559 + 12, v560);
    v567 = (v567 & 0xFFFF0000u) | (((uint32)(r_u16(v558 + 2u * (9))) & 0xFFFFu) << 0);
    w_u32(v559 + 20, v561);
    w_u32(v559 + 28, v567);
v567 = carrier_v0 = xport_gte_read_data(24);
w_u32(v559 + (8), xport_gte_read_data(12));
w_u32(v559 + (0x10), xport_gte_read_data(13));
                        if ( carrier_v0 )
                        {
w_u32(v559 + (0x18), xport_gte_read_data(14));
    w_u8(v559 + 3, 7);
                          w_masked_u32(v559, v566, 7u);
    w_u32(a0 + 176, v559);
                          v559 += 32;
                        }
                      }
                      v558 += 16;
                      v512 += 4;
                    }
                    while ( v562 >= 0 );
    w_u32(a0 + 180, v559);
                    v569 = v512 - 16;
                    v570 = v558 - 64;
                    v571 = 3;
                    do
                    {
                      while ( v571 >= 0 )
                      {
                        uint32 first_flags, second_flags, third_flags, fourth_flags;
                        uint32 first_screen_xy;
                        sint32 threshold;
                        v572 = ((uint8)v571 - 1) & 3;
                        if ( (v571 & 1) != 0 )
                        {
                          carrier_t1 = (v570 + 2u * (8 * v572 + 8));
                          carrier_t2 = (uint32)r_u32(v569 + 4u * (v571));
                          carrier_t6 = (v570 + 2u * (8 * v571 + 8));
                          carrier_t5 = v570;
                        }
                        else
                        {
                          carrier_t1 = (uint32)r_u32(v569 + 4u * (v571));
                          carrier_t2 = (v570 + 2u * (8 * v571 + 8));
                          carrier_t6 = v570;
                          carrier_t5 = (v570 + 2u * (8 * v572 + 8));
                        }
                        first_flags = r_u16(carrier_t1);
                        second_flags = r_u16(carrier_t2);
                        third_flags = r_u16(carrier_t6);
                        fourth_flags = r_u16(carrier_t5);
                        --v571;
                        if ( (first_flags & second_flags & third_flags & fourth_flags & 0x1Fu) == 0 )
                        {
                          v577 = v511 < r_u16(a0 + 188);
                          if ( ((first_flags | second_flags | third_flags | fourth_flags) & 0x20u) != 0 )
                          {
    w_u32(v569 + 4u * (4), carrier_t1);
                            if ( v577 )
                              goto LABEL_372;
                          }
                          else
                          {
xport_gte_write_data(12, r_u32(carrier_t2 + (0xC)));
xport_gte_write_data(13, r_u32(carrier_t1 + (0xC)));
xport_gte_write_data(14, r_u32(carrier_t6 + (0xC)));
                            v579 = r_u32(a0 + 180);
                            v580 = (uint32)r_u16(a0 + 164u) << 16;
xport_gte_execute(0x1400006);
                            v581 = r_u32(a0 + 164);
    v580 = (v580 & 0xFFFF0000u) | (((uint32)(r_u16(carrier_t2 + 2u)) & 0xFFFFu) << 0);
    v581 = (v581 & 0xFFFF0000u) | (((uint32)(r_u16(carrier_t1 + 2u)) & 0xFFFFu) << 0);
    w_u32(v579 + 12, v580);
    v580 = (v580 & 0xFFFF0000u) | (((uint32)(r_u16(carrier_t6 + 2u)) & 0xFFFFu) << 0);
    w_u32(v579 + 20, v581);
    v581 = (v581 & 0xFFFF0000u) | (((uint32)(r_u16(carrier_t5 + 2u)) & 0xFFFFu) << 0);
    w_u32(v579 + 28, v580);
v553 = carrier_a0 = xport_gte_read_data(24);
xport_gte_write_data(12, r_u32(carrier_t5 + (0xC)));
    w_u32(v579 + 36, v581);
                            v583 = r_u32(a0 + 156);
                            first_screen_xy = r_u32(carrier_t2 + 12u);
xport_gte_execute(0x1400006);
    w_u32(v579 + 8, first_screen_xy);
    w_u32(v579 + 4, v583);
    w_u8(v579 + 3, 9);
                            w_masked_u32(v579, (uint32)v579 + 40u, 7u);
                            threshold = (sint32)r_u32(a0 + 184u);
v584 = carrier_v0 = xport_gte_read_data(24);
w_u32(v579 + (0x20), xport_gte_read_data(12));
                            v586 = carrier_a0 - carrier_v0;
w_u32(v579 + (0x10), xport_gte_read_data(13));
                            if ( v586 < 0 )
                              v586 = (sint32)(0u - (uint32)v586);
w_u32(v579 + (0x18), xport_gte_read_data(14));
                            v587 = threshold < v586 && v577;
    w_u32(v569 + 4u * (4), carrier_t1);
                            if ( v587 )
                            {
LABEL_372:
                              carrier_t3 = v570 + 80;
                              v512 = v569 + 16;
                              v578 = a0 + v511++;
    w_u8(v578 + 196, v571);
    w_u32(v512 + 4u * (1), carrier_t2);
    w_u32(v512 + 4u * (2), carrier_t6);
    w_u32(v512 + 4u * (3), carrier_t5);
                              goto LABEL_350;
                            }
    w_u32(a0 + 176, v579);
    w_u32(a0 + 180, v579 + 40);
                          }
                        }
                      }
                      --v511;
                      v571 = (sint8)r_u8(a0 + v511 + 196);
                      v569 -= 16;
                      v570 -= 80;
                    }
                    while ( v511 >= 0 );
                    v588 = r_u32(a0 + 168);
                    v589 = r_u32(a0 + 172);
                    uint32 chain_end = r_u32(a0 + 180u);
                    v590 = r_u32(a0 + 176);
                    if ( (uint32)v589 != chain_end )
                    {
                      course_polygon_link(v588, (uint32)v589, (uint32)v590);
                    }
                  }
                }
                v422 = r_u32(a0 + 180);
                carrier_t8 = r_u32(a0 + 956);
                v420 = r_u32(a0 + 984);
                v423 = r_u32(a0 + 976);
                carrier_t9 = r_u32(a0 + 960);
                carrier_s4 = r_u32(a0 + 964);
                carrier_s3 = r_u32(a0 + 968);
              }
            }
          }
        }
      }
      carrier_t8 += 12;
    }
    while ( v420 );
    v4 = r_u32(a0 + 932);
    w_u32(a0 + 956, carrier_t8);
    w_u32(a0 + 952, v422);
  }
  v598 = r_u16(v4 + 60);
  carrier_t4 = r_u32(a0 + 956);
    w_u32(a0 + 932, v4);
  if ( v598 )
  {
    v600 = r_u32(a0 + 952);
    v601 = r_u32(carrier_t4);
    v602 = r_u32(a0 + 936);
    carrier_t9 = v602 + 8 * (v601 & 0x1FF);
    carrier_s3 = v602 + ((v601 >> 6) & 0xFF8);
    carrier_s2 = v602 + ((v601 >> 15) & 0xFF8);
    do
    {
      uint16 near_depth, depth_shift;
      uint32 ordering_base;
      sint16 viewport_width, viewport_height;
xport_gte_write_data(0, r_u32(carrier_s3 + (0)));
xport_gte_write_data(1, r_u32(carrier_s3 + (4)));
xport_gte_write_data(2, r_u32(carrier_t9 + (0)));
xport_gte_write_data(3, r_u32(carrier_t9 + (4)));
xport_gte_write_data(4, r_u32(carrier_s2 + (0)));
xport_gte_write_data(5, r_u32(carrier_s2 + (4)));
      --v598;
      v606 = v601;
xport_gte_execute(0x280030);
      v601 = r_u32(carrier_t4 + 4u * (5));
      v607 = r_u32(carrier_t4 + 4u * (1));
      v608 = r_u32(a0 + 936);
      near_depth = r_u16(a0 + 8u);
      depth_shift = r_u16(a0 + 40u);
      carrier_t9 = v608 + 8 * (v601 & 0x1FF);
      carrier_s3 = v608 + ((v601 >> 6) & 0xFF8);
      carrier_s2 = v608 + ((v601 >> 15) & 0xFF8);
carrier_t7 = carrier_t7 = course_polygon_gte_flags();
v577 = carrier_t0 = xport_gte_read_data(17);
v608 = carrier_v1 = xport_gte_read_data(18);
v588 = carrier_a0 = xport_gte_read_data(19);
      ordering_base = r_u32(a0 + 948u);
xport_gte_execute(0x1400006);
      if ( carrier_t0 < carrier_v1 )
        carrier_t0 = carrier_v1;
      v613 = ((uint32)v606 >> 23) & 0x30;
      if ( carrier_t0 < carrier_a0 )
        carrier_t0 = carrier_a0;
      if ( carrier_t0 >= near_depth )
      {
carrier_t3 = carrier_t3 = xport_gte_read_data(24);
        v615 = carrier_t3 & v606;
        if ( (sint32)carrier_t7 < 0 )
        {
          v615 = 0;
          if ( v606 < 0 )
          {
w_u32(a0 + (0x164), xport_gte_read_data(0));
w_u32(a0 + (0x168), xport_gte_read_data(1));
w_u32(a0 + (0x144), xport_gte_read_data(2));
w_u32(a0 + (0x148), xport_gte_read_data(3));
w_u32(a0 + (0x184), xport_gte_read_data(4));
w_u32(a0 + (0x188), xport_gte_read_data(5));
            v615 = course_polygon_facing(a0, 324u, 388u, 356u);
          }
        }
        v641 = carrier_t0 >> (depth_shift & 31u);
        if ( v615 >= 0 )
        {
          if ( v641 >= 4096 )
            v641 = 4095;
          carrier_a0 = 0;
          if ( (v607 & 0x8000000) == 0 )
            carrier_a0 = r_u16(2 * v641 + r_u32(a0 + 940));
          v643 = (uint32)(r_u32(a0 + 944) + 32 * (v607 << 9 >> 18));
          if ( (sint32)carrier_t3 < 0 )
            carrier_t3 = (0u - carrier_t3);
          v644 = v613 + v641;
          v645 = (uint32)(v643 + 16);
          if ( r_u16(v643) < (uint32)carrier_t3 )
            v645 = (uint32)(v643 + 4);
w_u32(a0 + (0x16C), xport_gte_read_data(12));
w_u32(a0 + (0x14C), xport_gte_read_data(13));
w_u32(a0 + (0x18C), xport_gte_read_data(14));
          carrier_v0 = r_u32(carrier_t4 + 4u * (2));
xport_gte_write_data(21, r_u32(carrier_t4 + (0xC)));
xport_gte_write_data(22, r_u32(carrier_t4 + (0x10)));
xport_gte_write_data(6, carrier_v0);
xport_gte_write_data(20, carrier_v0);
xport_gte_write_data(8, carrier_a0);
          v647 = (uint32)(ordering_base + 4 * v644);
xport_gte_execute(0xF8002A);
          v648 = (sint16)r_u16(a0 + 332);
          v649 = (sint16)r_u16(a0 + 364);
          v650 = (sint16)r_u16(a0 + 396);
          v651 = v648;
          v652 = v648 < v649;
          if ( v649 < v648 )
            v648 = v649;
          if ( v652 )
            v651 = v649;
          if ( v650 < v648 )
            v648 = v650;
          if ( v651 < v650 )
            v651 = v650;
          viewport_width = (sint16)r_u16(a0 + 4u);
          if ( v651 > 0 && v648 < viewport_width )
          {
            v653 = carrier_t7 | (1023 - (v651 - v648));
            v654 = (sint16)r_u16(a0 + 334);
            v655 = (sint16)r_u16(a0 + 366);
            v656 = (sint16)r_u16(a0 + 398);
            v657 = v654;
            v658 = v654 < v655;
            if ( v655 < v654 )
              v654 = v655;
            if ( v658 )
              v657 = v655;
            if ( v656 < v654 )
              v654 = v656;
            if ( v657 < v656 )
              v657 = v656;
            viewport_height = (sint16)r_u16(a0 + 6u);
            if ( v657 > 0 && v654 < viewport_height )
            {
              v659 = r_u32(v645);
              v660 = r_u32(v645 + 4u * (1));
              v661 = r_u32(v645 + 4u * (2));
              if ( (v653 | (511 - (v657 - v654))) >= 0 )
              {
    w_u32(v600, 150994944);
    w_u32(v600 + 12, v659);
    w_u32(v600 + 24, v660);
    w_u32(v600 + 36, v661);
w_u32(v600 + (4), xport_gte_read_data(20));
w_u32(v600 + (0x10), xport_gte_read_data(21));
w_u32(v600 + (0x1C), xport_gte_read_data(22));
w_u32(v600 + (0x14), xport_gte_read_data(12));
w_u32(v600 + (8), xport_gte_read_data(13));
w_u32(v600 + (0x20), xport_gte_read_data(14));
                course_polygon_link(v647, v600, v600);
                v600 += 40;
              }
              else
              {
    w_u32(a0 + 956, carrier_t4);
    w_u32(a0 + 984, v598);
    w_u32(a0 + 976, v601);
    w_u32(a0 + 960, carrier_t9);
    w_u32(a0 + 964, carrier_s3);
    w_u32(a0 + 968, carrier_s2);
    w_u32(a0 + 168, v647);
    w_u32(a0 + 172, v600);
    w_u16(a0 + 184, 3);
    w_u32(a0 + 164, v660);
    w_masked_u32(a0 + 164u, v659 >> 16, 3u);
    w_u16(a0 + 336, ((uint16)v659 << 7) & 0x7F80 | 0x40);
    w_u16(a0 + 338, (v659 >> 1) & 0x7F80 | 0x40);
    w_u16(a0 + 368, ((uint16)v660 << 7) & 0x7F80 | 0x40);
    w_u16(a0 + 370, (v660 >> 1) & 0x7F80 | 0x40);
    w_u16(a0 + 400, ((uint16)v661 << 7) & 0x7F80 | 0x40);
    w_u16(a0 + 402, (v661 >> 1) & 0x7F80 | 0x40);
w_u32(a0 + (0x9C), xport_gte_read_data(20));
w_u32(a0 + (0x154), xport_gte_read_data(20));
w_u32(a0 + (0x174), xport_gte_read_data(21));
w_u32(a0 + (0x194), xport_gte_read_data(22));
w_u32(a0 + (0x164), xport_gte_read_data(0));
w_u32(a0 + (0x168), xport_gte_read_data(1));
w_u32(a0 + (0x144), xport_gte_read_data(2));
w_u32(a0 + (0x148), xport_gte_read_data(3));
w_u32(a0 + (0x184), xport_gte_read_data(4));
w_u32(a0 + (0x188), xport_gte_read_data(5));
                gtmain_polygon_clip_textured_gouraud(a0);
                v600 = r_u32(a0 + 180);
                carrier_t4 = r_u32(a0 + 956);
                v598 = r_u32(a0 + 984);
                v601 = r_u32(a0 + 976);
                carrier_t9 = r_u32(a0 + 960);
                carrier_s3 = r_u32(a0 + 964);
                carrier_s2 = r_u32(a0 + 968);
              }
            }
          }
        }
      }
      carrier_t4 += 20;
    }
    while ( v598 );
    v4 = r_u32(a0 + 932);
    w_u32(a0 + 956, carrier_t4);
    w_u32(a0 + 952, v600);
  }
  v665 = r_u16(v4 + 62);
  carrier_t5 = r_u32(a0 + 956);
    w_u32(a0 + 932, v4);
  if ( v665 )
  {
    v667 = r_u32(a0 + 952);
    v668 = r_u32(carrier_t5);
    v669 = r_u32(a0 + 936);
    carrier_t9 = v669 + 8 * (v668 & 0x1FF);
    carrier_s4 = v669 + ((v668 >> 6) & 0xFF8);
    carrier_s3 = v669 + ((v668 >> 15) & 0xFF8);
    do
    {
      uint16 near_depth, depth_shift;
      uint32 fog_table, ordering_base;
      sint16 viewport_width, viewport_height;
xport_gte_write_data(0, r_u32(carrier_s4 + (0)));
xport_gte_write_data(1, r_u32(carrier_s4 + (4)));
xport_gte_write_data(2, r_u32(carrier_s3 + (0)));
xport_gte_write_data(3, r_u32(carrier_s3 + (4)));
w_u32(a0 + (0x164), xport_gte_read_data(0));
w_u32(a0 + (0x168), xport_gte_read_data(1));
xport_gte_write_data(4, r_u32(carrier_t9 + (0)));
xport_gte_write_data(5, r_u32(carrier_t9 + (4)));
      v673 = r_u32(a0 + 936);
      v674 = v668;
xport_gte_execute(0x280030);
      v675 = r_u32(carrier_t5 + 4u * (1));
      v668 = r_u32(carrier_t5 + 4u * (6));
      v676 = (uint32)(v673 + 8 * (v675 & 0x1FF));
      carrier_v1 = (sint32)r_u32(v676);
      carrier_a0 = (sint32)r_u32(v676 + 4u * (1));
w_u32(a0 + (0x16C), xport_gte_read_data(12));
w_u32(v667 + (0x14), xport_gte_read_data(12));
carrier_t4 = carrier_t4 = course_polygon_gte_flags();
      near_depth = r_u16(a0 + 8u);
      --v665;
xport_gte_execute(0x1400006);
      carrier_t9 = v673 + 8 * (v668 & 0x1FF);
      carrier_s4 = v673 + ((v668 >> 6) & 0xFF8);
xport_gte_write_data(0, carrier_v1);
xport_gte_write_data(1, carrier_a0);
v645 = carrier_t2 = xport_gte_read_data(24);
v641 = carrier_t0 = xport_gte_read_data(17);
carrier_v1 = carrier_v1 = xport_gte_read_data(18);
carrier_a0 = carrier_a0 = xport_gte_read_data(19);
      depth_shift = r_u16(a0 + 40u);
      fog_table = r_u32(a0 + 940u);
xport_gte_execute(0x180001);
      if ( carrier_t0 < carrier_v1 )
        carrier_t0 = carrier_v1;
      v684 = ((uint32)v674 >> 23) & 0x30;
      if ( carrier_t0 < carrier_a0 )
        carrier_t0 = carrier_a0;
      carrier_s3 = v673 + ((v668 >> 15) & 0xFF8);
xport_gte_write_data(6, r_u32(carrier_t5 + (8)));
v676 = carrier_v0 = course_polygon_gte_flags();
carrier_v1 = carrier_v1 = xport_gte_read_data(19);
      v687 = carrier_t4 | carrier_v0;
xport_gte_execute(0x1400006);
      v688 = (0u - carrier_t2);
      if ( carrier_t0 < carrier_v1 )
        carrier_t0 = carrier_v1;
      if ( carrier_t0 >= near_depth )
      {
        v689 = carrier_t0 >> (depth_shift & 31u);
        ordering_base = r_u32(a0 + 948u);
v647 = carrier_t3 = xport_gte_read_data(24);
        v691 = v674 & v688 & carrier_t3;
        if ( v687 < 0 )
        {
          v691 = 0;
          if ( v674 < 0 )
          {
w_u32(a0 + (0x184), xport_gte_read_data(2));
w_u32(a0 + (0x188), xport_gte_read_data(3));
w_u32(a0 + (0x144), xport_gte_read_data(4));
w_u32(a0 + (0x148), xport_gte_read_data(5));
            v691 = course_polygon_facing(a0, 324u, 388u, 356u);
          }
        }
        if ( v691 >= 0 )
        {
          if ( v689 >= 4096 )
            v689 = 4095;
          carrier_v0 = 0;
          if ( (v675 & 0x8000000) == 0 )
            carrier_v0 = r_u16(2 * v689 + fog_table);
w_u32(a0 + (0x18C), xport_gte_read_data(12));
w_u32(a0 + (0x14C), xport_gte_read_data(13));
w_u32(a0 + (0x1AC), xport_gte_read_data(14));
xport_gte_write_data(8, carrier_v0);
          v718 = (uint32)(ordering_base + 4 * (v684 + v689));
xport_gte_execute(0x780010);
          if ( v688 < 0 )
            v688 = (sint32)(0u - (uint32)v688);
          v719 = (uint32)(r_u32(a0 + 944) + 32 * (v675 << 9 >> 18));
          if ( (sint32)carrier_t3 < 0 )
            carrier_t3 = (0u - carrier_t3);
          v720 = (uint32)(v719 + 16);
          if ( r_u16(v719) < (uint32)(v688 + carrier_t3) )
            v720 = (uint32)(v719 + 4);
          carrier_v1 = r_u32(carrier_t5 + 4u * (3));
          carrier_a0 = r_u32(carrier_t5 + 4u * (4));
          carrier_a1 = r_u32(carrier_t5 + 4u * (5));
carrier_v0 = carrier_v0 = xport_gte_read_data(22);
xport_gte_write_data(20, carrier_v1);
xport_gte_write_data(21, carrier_a0);
xport_gte_write_data(22, carrier_a1);
    w_u32(a0 + 156, carrier_v0);
    w_u32(a0 + 340, carrier_v0);
xport_gte_execute(0xF8002A);
    w_u32(v667 + 4, carrier_v0);
          v725 = (sint16)r_u16(a0 + 332);
          v726 = (sint16)r_u16(a0 + 364);
          v727 = v725;
          v728 = v725 < v726;
          if ( v726 < v725 )
            v725 = v726;
          v729 = (sint16)r_u16(a0 + 396);
          if ( v728 )
            v727 = v726;
          if ( v729 < v725 )
            v725 = v729;
          v730 = (sint16)r_u16(a0 + 428);
          if ( v727 < v729 )
            v727 = v729;
          if ( v730 < v725 )
            v725 = v730;
          if ( v727 < v730 )
            v727 = v730;
          viewport_width = (sint16)r_u16(a0 + 4u);
          if ( v727 > 0 && v725 < viewport_width )
          {
            v731 = v687 | (1023 - (v727 - v725));
            v732 = (sint16)r_u16(a0 + 334);
            v733 = (sint16)r_u16(a0 + 366);
            v734 = v732;
            v735 = v732 < v733;
            if ( v733 < v732 )
              v732 = v733;
            v736 = (sint16)r_u16(a0 + 398);
            if ( v735 )
              v734 = v733;
            if ( v736 < v732 )
              v732 = v736;
            v737 = (sint16)r_u16(a0 + 430);
            if ( v734 < v736 )
              v734 = v736;
            if ( v737 < v732 )
              v732 = v737;
            if ( v734 < v737 )
              v734 = v737;
            viewport_height = (sint16)r_u16(a0 + 6u);
            if ( v734 > 0 && v732 < viewport_height )
            {
              v738 = v731 | (511 - (v734 - v732));
              v739 = r_u32(v720 + 4u * (2));
              v740 = r_u32(v720);
              v741 = r_u32(v720 + 4u * (1));
              if ( v738 >= 0 )
              {
    w_u32(v667, 201326592);
    w_u32(v667 + 12, v740);
    w_u32(v667 + 24, v741);
    w_u32(v667 + 48, v739);
    w_masked_u32(v667 + 36u, v739 >> 16, 3u);
w_u32(v667 + (0x10), xport_gte_read_data(20));
w_u32(v667 + (0x28), xport_gte_read_data(21));
w_u32(v667 + (0x1C), xport_gte_read_data(22));
w_u32(v667 + (0x2C), xport_gte_read_data(12));
w_u32(v667 + (8), xport_gte_read_data(13));
w_u32(v667 + (0x20), xport_gte_read_data(14));
                course_polygon_link(v718, v667, v667);
                v667 += 52;
              }
              else
              {
    w_u32(a0 + 956, carrier_t5);
    w_u32(a0 + 984, v665);
    w_u32(a0 + 976, v668);
    w_u32(a0 + 960, carrier_t9);
    w_u32(a0 + 964, carrier_s4);
    w_u32(a0 + 968, carrier_s3);
    w_u32(a0 + 168, v718);
    w_u32(a0 + 172, v667);
    w_u16(a0 + 184, 4);
    w_u32(a0 + 164, v741);
    w_masked_u32(a0 + 164u, v740 >> 16, 3u);
                v742 = (uint16)v739 << 7;
                v743 = v739 >> 1;
                v744 = (((uint32)(v739) >> 16) & 0xFFFFu);
    w_u16(a0 + 336, ((uint16)v740 << 7) & 0x7F80 | 0x40);
    w_u16(a0 + 338, (v740 >> 1) & 0x7F80 | 0x40);
    w_u16(a0 + 368, ((uint16)v741 << 7) & 0x7F80 | 0x40);
    w_u16(a0 + 370, (v741 >> 1) & 0x7F80 | 0x40);
    w_u16(a0 + 432, ((uint16)v744 << 7) & 0x7F80 | 0x40);
    w_u16(a0 + 400, v742 & 0x7F80 | 0x40);
    w_u16(a0 + 402, v743 & 0x7F80 | 0x40);
    w_u16(a0 + 434, (v744 >> 1) & 0x7F80 | 0x40);
w_u32(a0 + (0x174), xport_gte_read_data(20));
w_u32(a0 + (0x194), xport_gte_read_data(21));
w_u32(a0 + (0x1B4), xport_gte_read_data(22));
w_u32(a0 + (0x1A4), xport_gte_read_data(0));
w_u32(a0 + (0x1A8), xport_gte_read_data(1));
w_u32(a0 + (0x184), xport_gte_read_data(2));
w_u32(a0 + (0x188), xport_gte_read_data(3));
w_u32(a0 + (0x144), xport_gte_read_data(4));
w_u32(a0 + (0x148), xport_gte_read_data(5));
                gtmain_polygon_clip_textured_gouraud(a0);
                v667 = r_u32(a0 + 180);
                carrier_t5 = r_u32(a0 + 956);
                v665 = r_u32(a0 + 984);
                v668 = r_u32(a0 + 976);
                carrier_t9 = r_u32(a0 + 960);
                carrier_s4 = r_u32(a0 + 964);
                carrier_s3 = r_u32(a0 + 968);
              }
            }
          }
        }
      }
      carrier_t5 += 24;
    }
    while ( v665 );
    v4 = r_u32(a0 + 932);
    w_u32(a0 + 956, carrier_t5);
    w_u32(a0 + 952, v667);
  }
  v748 = gtmain_model_billboards_draw(a0, r_u32(a0 + 956), r_u16(v4 + 64));
  return gtmain_model_radial_draw(a0, v748, r_u16(v4 + 66));
}
