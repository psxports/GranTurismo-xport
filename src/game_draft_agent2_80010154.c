#include "game_draft_registry.h"

/* Unverified full pseudocode draft; TODO Resolve omitted carriers and adapter ABI */
uint32 gt_draft_80010154(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
    sint32 v4 = 0;
    uint32 v5 = 0;
    sint32 v6 = 0;
    uint32 v8 = 0;
    sint32 v9 = 0;
    sint32 v10 = 0;
    sint32 v11 = 0;
    sint16 v12 = 0;
    sint32 v13 = 0;
    uint32 v17 = 0;
    sint32 v21 = 0;
    uint32 v24 = 0;
    sint8 v25 = 0;
    sint8 v26 = 0;
    sint8 v27 = 0;
    sint32 v28 = 0;
    uint32 v30 = 0;
    sint32 v31 = 0;
    sint32 v32 = 0;
    sint32 v33 = 0;
    sint16 v34 = 0;
    sint32 v35 = 0;
    uint32 v39 = 0;
    uint32 v40 = 0;
    sint32 v47 = 0;
    sint32 v49 = 0;
    uint32 v50 = 0;
    sint32 v51 = 0;
    uint32 v54 = 0;
    sint8 v55 = 0;
    sint8 v56 = 0;
    sint8 v57 = 0;
    sint32 v58 = 0;
    uint32 v60 = 0;
    sint32 v61 = 0;
    sint32 v62 = 0;
    sint32 v63 = 0;
    sint16 v64 = 0;
    sint32 v65 = 0;
    uint32 v69 = 0;
    sint32 v73 = 0;
    uint32 v77 = 0;
    sint8 v78 = 0;
    sint8 v79 = 0;
    sint8 v80 = 0;
    sint32 v81 = 0;
    uint32 v83 = 0;
    sint32 v84 = 0;
    sint32 v85 = 0;
    sint32 v86 = 0;
    sint16 v87 = 0;
    sint32 v88 = 0;
    uint32 v92 = 0;
    uint32 v93 = 0;
    sint32 v100 = 0;
    sint32 v102 = 0;
    sint32 v103 = 0;
    uint32 v106 = 0;
    sint32 v110 = 0;
    sint32 v112 = 0;
    sint32 v113 = 0;
    sint32 v114 = 0;
    sint16 v115 = 0;
    sint32 v116 = 0;
    uint32 v117 = 0;
    sint32 v118 = 0;
    uint32 v122 = 0;
    uint32 v123 = 0;
    uint32 v124 = 0;
    sint32 v128 = 0;
    uint32 v131 = 0;
    uint32 v132 = 0;
    sint32 v133 = 0;
    sint32 v134 = 0;
    sint32 v135 = 0;
    sint32 v136 = 0;
    uint32 v137 = 0;
    sint32 v138 = 0;
    sint32 v139 = 0;
    sint32 v141 = 0;
    sint32 v142 = 0;
    sint32 v143 = 0;
    sint32 v144 = 0;
    sint32 v145 = 0;
    sint32 v146 = 0;
    sint32 v147 = 0;
    sint32 v148 = 0;
    sint32 v149 = 0;
    sint32 v150 = 0;
    sint32 v151 = 0;
    sint32 v152 = 0;
    sint32 v153 = 0;
    sint32 v154 = 0;
    sint32 v155 = 0;
    sint32 v156 = 0;
    sint32 v157 = 0;
    sint32 v158 = 0;
    sint32 v159 = 0;
    sint32 v160 = 0;
    sint32 v161 = 0;
    sint32 v162 = 0;
    sint32 v163 = 0;
    sint32 v164 = 0;
    sint32 v166 = 0;
    sint32 v169 = 0;
    sint32 v171 = 0;
    sint32 v172 = 0;
    sint32 v176 = 0;
    sint32 v177 = 0;
    sint32 v178 = 0;
    sint32 v179 = 0;
    sint32 v180 = 0;
    sint32 v182 = 0;
    sint32 v183 = 0;
    uint32 v184 = 0;
    uint32 v185 = 0;
    sint32 v186 = 0;
    sint8 v187 = 0;
    sint8 v188 = 0;
    sint8 v189 = 0;
    sint8 v190 = 0;
    sint8 v191 = 0;
    sint8 v192 = 0;
    sint32 v193 = 0;
    sint32 v195 = 0;
    sint32 v196 = 0;
    sint32 v197 = 0;
    sint16 v198 = 0;
    sint32 v199 = 0;
    uint32 v200 = 0;
    sint32 v201 = 0;
    uint32 v205 = 0;
    uint32 v206 = 0;
    uint32 v207 = 0;
    sint32 v214 = 0;
    sint32 v215 = 0;
    sint32 v217 = 0;
    sint32 v218 = 0;
    sint32 v221 = 0;
    uint32 v222 = 0;
    uint32 v223 = 0;
    sint32 v224 = 0;
    uint32 v225 = 0;
    sint32 v226 = 0;
    sint32 v227 = 0;
    sint32 v228 = 0;
    sint32 v230 = 0;
    sint32 v231 = 0;
    sint16 v232 = 0;
    sint16 v233 = 0;
    sint32 v234 = 0;
    sint32 v235 = 0;
    sint32 v236 = 0;
    sint32 v237 = 0;
    sint32 v238 = 0;
    sint32 v239 = 0;
    sint32 v240 = 0;
    sint32 v241 = 0;
    sint32 v242 = 0;
    sint32 v243 = 0;
    sint32 v244 = 0;
    sint32 v245 = 0;
    sint32 v246 = 0;
    sint32 v247 = 0;
    sint32 v248 = 0;
    sint32 v249 = 0;
    sint32 v250 = 0;
    sint32 v251 = 0;
    sint32 v252 = 0;
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
    sint32 v266 = 0;
    sint32 v267 = 0;
    sint32 v268 = 0;
    uint32 v269 = 0;
    uint32 v270 = 0;
    sint32 v272 = 0;
    sint32 v273 = 0;
    sint32 v274 = 0;
    sint32 v275 = 0;
    sint32 v276 = 0;
    sint32 v277 = 0;
    sint32 v278 = 0;
    sint32 v279 = 0;
    sint32 v280 = 0;
    sint32 v281 = 0;
    sint32 v282 = 0;
    sint32 v283 = 0;
    sint32 v284 = 0;
    sint32 v285 = 0;
    sint32 v286 = 0;
    sint32 v287 = 0;
    sint32 v288 = 0;
    sint32 v289 = 0;
    sint32 v290 = 0;
    sint32 v291 = 0;
    sint32 v292 = 0;
    sint32 v293 = 0;
    sint32 v294 = 0;
    sint32 v295 = 0;
    sint32 v296 = 0;
    sint32 v297 = 0;
    sint32 v298 = 0;
    sint32 v299 = 0;
    sint32 v300 = 0;
    sint32 v301 = 0;
    sint32 v302 = 0;
    sint32 v303 = 0;
    sint32 v304 = 0;
    sint32 v305 = 0;
    sint32 v306 = 0;
    sint32 v307 = 0;
    sint32 v308 = 0;
    sint32 v309 = 0;
    sint32 v310 = 0;
    sint32 v311 = 0;
    sint32 v315 = 0;
    sint32 v317 = 0;
    uint32 v318 = 0;
    sint32 v319 = 0;
    sint32 v320 = 0;
    sint8 v321 = 0;
    sint32 v322 = 0;
    sint32 v327 = 0;
    sint32 v328 = 0;
    sint32 v329 = 0;
    sint32 v331 = 0;
    sint32 v332 = 0;
    sint32 v334 = 0;
    sint32 v335 = 0;
    uint32 v336 = 0;
    uint32 v337 = 0;
    sint32 v338 = 0;
    sint8 v339 = 0;
    sint8 v340 = 0;
    sint8 v341 = 0;
    sint8 v342 = 0;
    sint8 v343 = 0;
    sint8 v344 = 0;
    sint32 v345 = 0;
    uint32 v347 = 0;
    sint32 v348 = 0;
    sint32 v349 = 0;
    sint32 v350 = 0;
    sint16 v351 = 0;
    sint32 v352 = 0;
    sint32 v353 = 0;
    uint32 v357 = 0;
    uint32 v358 = 0;
    uint32 v359 = 0;
    sint32 v363 = 0;
    sint32 v364 = 0;
    uint32 v368 = 0;
    uint32 v369 = 0;
    sint32 v370 = 0;
    sint32 v371 = 0;
    sint32 v372 = 0;
    sint32 v373 = 0;
    uint32 v375 = 0;
    sint32 v376 = 0;
    sint32 v377 = 0;
    sint32 v378 = 0;
    sint16 v379 = 0;
    sint32 v380 = 0;
    sint32 v381 = 0;
    uint32 v385 = 0;
    uint32 v386 = 0;
    uint32 v387 = 0;
    sint32 v394 = 0;
    sint32 v396 = 0;
    uint32 v399 = 0;
    uint32 v403 = 0;
    uint32 v404 = 0;
    sint32 v405 = 0;
    sint32 v406 = 0;
    sint32 v407 = 0;
    sint32 v408 = 0;
    uint32 carrier_a0 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_a1 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_a2 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_a3 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_s1 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_s4 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_s5 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_s6 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_t0 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_t1 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_t2 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_t3 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_t4 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_t5 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_t6 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_t7 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_t8 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_t9 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_v0 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
    uint32 carrier_v1 = 0; /* TODO Recover nonstandard incoming or omitted carrier */
  v4 = a1 + 164;
  if ( a2 )
    v4 = r_u32(a1 + 148);
    w_u32(a0 + 928, a1);
    w_u32(a0 + 936, r_u32(v4));
  gt_draft_unresolved_call(0x80026EA8u, a0, 0, 0, 0, 0, 0, 0, 0);
  v5 = r_u32(v4 + 4);
    w_u32(a0 + 956, v5);
  v6 = r_u16(v4 + 48);
  carrier_t3 = v5;
    w_u32(a0 + 932, v4);
  if ( v6 )
  {
    v8 = r_u32(v5);
    v9 = r_u32(a0 + 952);
    v10 = r_u32(a0 + 936);
    v11 = r_u32(a0 + 948);
    v12 = r_u16(a0 + 40);
    v13 = r_u32(a0 + 940);
    carrier_s1 = v10 + 8 * (r_u32(v5) & 0x1FF);
    carrier_t7 = v10 + ((r_u32(v5) >> 6) & 0xFF8);
    carrier_t6 = v10 + ((r_u32(v5) >> 15) & 0xFF8);
    do
    {
gt_draft_gte_write(0, 0, r_u32(carrier_t7 + (0)));
gt_draft_gte_write(1, 0, r_u32(carrier_t7 + (4)));
gt_draft_gte_write(2, 0, r_u32(carrier_s1 + (0)));
gt_draft_gte_write(3, 0, r_u32(carrier_s1 + (4)));
gt_draft_gte_write(4, 0, r_u32(carrier_t6 + (0)));
gt_draft_gte_write(5, 0, r_u32(carrier_t6 + (4)));
      --v6;
      v17 = v8;
gt_draft_gte_command(0x280030);
      v8 = (sint32)r_u32(carrier_t3 + 4u * (3));
      carrier_s1 = v10 + 8 * (v8 & 0x1FF);
      carrier_t7 = v10 + ((v8 >> 6) & 0xFF8);
      carrier_t6 = v10 + ((v8 >> 15) & 0xFF8);
carrier_a0 = gt_draft_gte_read(17, 0);
carrier_a1 = gt_draft_gte_read(18, 0);
carrier_t0 = gt_draft_gte_read(19, 0);
gt_draft_gte_command(0x1400006);
      if ( carrier_a0 < carrier_a1 )
        carrier_a0 = carrier_a1;
      if ( carrier_a0 < carrier_t0 )
        carrier_a0 = carrier_t0;
      v21 = carrier_a0 >> v12;
carrier_a1 = gt_draft_gte_read(24, 0);
      if ( v21 >= 4096 )
        v21 = 4095;
      if ( (carrier_a1 & v17 & 0x80000000) == 0 )
      {
        carrier_v0 = 0;
        if ( ((sint32)r_u32(carrier_t3 + 4u * (1)) & 0x8000000) == 0 )
          carrier_v0 = r_u16(2 * v21 + v13);
gt_draft_gte_write(6, 0, r_u32(carrier_t3 + (8)));
gt_draft_gte_write(8, 0, carrier_v0);
        v24 = (uint32)(v11 + 4 * (((v17 >> 23) & 0x30) + v21));
gt_draft_gte_command(0x780010);
    w_u32(v9, 0x4000000);
        v25 = (sint8)r_u8(v24);
        v26 = (sint8)r_u8(v24 + 1u * (1));
        v27 = (sint8)r_u8(v24 + 1u * (2));
    w_u16(v24, v9);
    w_u8(v24 + 1u * (2), (uint32)(v9 << 8) >> 24);
    w_u8(v9, v25);
    w_u8(v9 + 1, v26);
    w_u8(v9 + 2, v27);
w_u32(v9 + (4), gt_draft_gte_read(22, 0));
w_u32(v9 + (0xC), gt_draft_gte_read(12, 0));
w_u32(v9 + (8), gt_draft_gte_read(13, 0));
w_u32(v9 + (0x10), gt_draft_gte_read(14, 0));
        v9 += 20;
      }
      carrier_t3 += 12;
    }
    while ( v6 );
    v4 = r_u32(a0 + 932);
    w_u32(a0 + 952, v9);
    w_u32(a0 + 956, carrier_t3);
  }
  v28 = r_u16(v4 + 50);
  carrier_t6 = r_u32(a0 + 956);
    w_u32(a0 + 932, v4);
  if ( v28 )
  {
    v30 = (sint32)r_u32(carrier_t6);
    v31 = r_u32(a0 + 952);
    v32 = r_u32(a0 + 936);
    v33 = r_u32(a0 + 948);
    v34 = r_u16(a0 + 40);
    v35 = r_u32(a0 + 940);
    carrier_t8 = v32 + 8 * ((sint32)r_u32(carrier_t6) & 0x1FF);
    carrier_s1 = v32 + (((sint32)r_u32(carrier_t6) >> 6) & 0xFF8);
    carrier_a3 = v32 + (((sint32)r_u32(carrier_t6) >> 15) & 0xFF8);
    do
    {
gt_draft_gte_write(0, 0, r_u32(carrier_s1 + (0)));
gt_draft_gte_write(1, 0, r_u32(carrier_s1 + (4)));
gt_draft_gte_write(2, 0, r_u32(carrier_a3 + (0)));
gt_draft_gte_write(3, 0, r_u32(carrier_a3 + (4)));
gt_draft_gte_write(4, 0, r_u32(carrier_t8 + (0)));
gt_draft_gte_write(5, 0, r_u32(carrier_t8 + (4)));
      --v28;
      v39 = v30;
gt_draft_gte_command(0x280030);
      v30 = (sint32)r_u32(carrier_t6 + 4u * (3));
      v40 = (uint32)(v32 + 8 * ((sint32)r_u32(carrier_t6 + 4u * (1)) & 0x1FF));
      carrier_v1 = (sint32)r_u32(v40);
      carrier_a0 = (sint32)r_u32(v40 + 4u * (1));
gt_draft_gte_command(0x1400006);
      carrier_t8 = v32 + 8 * (v30 & 0x1FF);
      carrier_s1 = v32 + ((v30 >> 6) & 0xFF8);
gt_draft_gte_write(0, 0, carrier_v1);
gt_draft_gte_write(1, 0, carrier_a0);
carrier_a1 = gt_draft_gte_read(24, 0);
w_u32(v31 + (0xC), gt_draft_gte_read(12, 0));
carrier_v1 = gt_draft_gte_read(17, 0);
carrier_a0 = gt_draft_gte_read(18, 0);
carrier_a3 = gt_draft_gte_read(19, 0);
      v47 = (0u - carrier_a1);
gt_draft_gte_command(0x180001);
      if ( carrier_v1 < carrier_a0 )
        carrier_v1 = carrier_a0;
      if ( carrier_v1 < carrier_a3 )
        carrier_v1 = carrier_a3;
      carrier_a3 = v32 + ((v30 >> 15) & 0xFF8);
gt_draft_gte_write(6, 0, r_u32(carrier_t6 + (8)));
carrier_a0 = gt_draft_gte_read(19, 0);
      v49 = v39 & v47;
gt_draft_gte_command(0x1400006);
      v50 = carrier_v1 >= carrier_a0;
      v51 = carrier_v1 >> v34;
      if ( !v50 )
        v51 = carrier_a0 >> v34;
      if ( v51 >= 4096 )
        v51 = 4095;
      carrier_a0 = 0;
      if ( ((sint32)r_u32(carrier_t6 + 4u * (1)) & 0x8000000) == 0 )
        carrier_a0 = r_u16(2 * v51 + v35);
carrier_v0 = gt_draft_gte_read(24, 0);
gt_draft_gte_write(8, 0, carrier_a0);
gt_draft_gte_command(0x780010);
      if ( (v49 & carrier_v0) >= 0 )
      {
    w_u32(v31, 83886080);
        v54 = (uint32)(v33 + 4 * (((v39 >> 23) & 0x30) + v51));
w_u32(v31 + (0x14), gt_draft_gte_read(12, 0));
w_u32(v31 + (8), gt_draft_gte_read(13, 0));
w_u32(v31 + (0x10), gt_draft_gte_read(14, 0));
        v55 = (sint8)r_u8(v54);
        v56 = (sint8)r_u8(v54 + 1u * (1));
        v57 = (sint8)r_u8(v54 + 1u * (2));
    w_u16(v54, v31);
    w_u8(v54 + 1u * (2), (uint32)(v31 << 8) >> 24);
    w_u8(v31, v55);
    w_u8(v31 + 1, v56);
    w_u8(v31 + 2, v57);
w_u32(v31 + (4), gt_draft_gte_read(22, 0));
        v31 += 24;
      }
      carrier_t6 += 12;
    }
    while ( v28 );
    v4 = r_u32(a0 + 932);
    w_u32(a0 + 952, v31);
    w_u32(a0 + 956, carrier_t6);
  }
  v58 = r_u16(v4 + 52);
  carrier_t3 = r_u32(a0 + 956);
    w_u32(a0 + 932, v4);
  if ( v58 )
  {
    v60 = (sint32)r_u32(carrier_t3);
    v61 = r_u32(a0 + 952);
    v62 = r_u32(a0 + 936);
    v63 = r_u32(a0 + 948);
    v64 = r_u16(a0 + 40);
    v65 = r_u32(a0 + 940);
    carrier_t8 = v62 + 8 * ((sint32)r_u32(carrier_t3) & 0x1FF);
    carrier_s1 = v62 + (((sint32)r_u32(carrier_t3) >> 6) & 0xFF8);
    carrier_t7 = v62 + (((sint32)r_u32(carrier_t3) >> 15) & 0xFF8);
    do
    {
gt_draft_gte_write(0, 0, r_u32(carrier_s1 + (0)));
gt_draft_gte_write(1, 0, r_u32(carrier_s1 + (4)));
gt_draft_gte_write(2, 0, r_u32(carrier_t8 + (0)));
gt_draft_gte_write(3, 0, r_u32(carrier_t8 + (4)));
gt_draft_gte_write(4, 0, r_u32(carrier_t7 + (0)));
gt_draft_gte_write(5, 0, r_u32(carrier_t7 + (4)));
      --v58;
      v69 = v60;
gt_draft_gte_command(0x280030);
      v60 = (sint32)r_u32(carrier_t3 + 4u * (5));
      carrier_t8 = v62 + 8 * (v60 & 0x1FF);
      carrier_s1 = v62 + ((v60 >> 6) & 0xFF8);
      carrier_t7 = v62 + ((v60 >> 15) & 0xFF8);
carrier_a0 = gt_draft_gte_read(17, 0);
carrier_a3 = gt_draft_gte_read(18, 0);
carrier_t1 = gt_draft_gte_read(19, 0);
gt_draft_gte_command(0x1400006);
      if ( carrier_a0 < carrier_a3 )
        carrier_a0 = carrier_a3;
      if ( carrier_a0 < carrier_t1 )
        carrier_a0 = carrier_t1;
      v73 = carrier_a0 >> v64;
carrier_v1 = gt_draft_gte_read(24, 0);
      if ( v73 >= 4096 )
        v73 = 4095;
      if ( (carrier_v1 & v69 & 0x80000000) == 0 )
      {
        carrier_v1 = 0;
        if ( ((sint32)r_u32(carrier_t3 + 4u * (1)) & 0x8000000) == 0 )
          carrier_v1 = r_u16(2 * v73 + v65);
        carrier_v0 = (sint32)r_u32(carrier_t3 + 4u * (2));
gt_draft_gte_write(21, 0, r_u32(carrier_t3 + (0xC)));
gt_draft_gte_write(22, 0, r_u32(carrier_t3 + (0x10)));
gt_draft_gte_write(6, 0, carrier_v0);
gt_draft_gte_write(20, 0, carrier_v0);
gt_draft_gte_write(8, 0, carrier_v1);
        v77 = (uint32)(v63 + 4 * (((v69 >> 23) & 0x30) + v73));
gt_draft_gte_command(0xF8002A);
    w_u32(v61, 100663296);
        v78 = (sint8)r_u8(v77);
        v79 = (sint8)r_u8(v77 + 1u * (1));
        v80 = (sint8)r_u8(v77 + 1u * (2));
    w_u16(v77, v61);
    w_u8(v77 + 1u * (2), (uint32)(v61 << 8) >> 24);
    w_u8(v61, v78);
    w_u8(v61 + 1, v79);
    w_u8(v61 + 2, v80);
w_u32(v61 + (4), gt_draft_gte_read(20, 0));
w_u32(v61 + (0xC), gt_draft_gte_read(21, 0));
w_u32(v61 + (0x14), gt_draft_gte_read(22, 0));
w_u32(v61 + (0x10), gt_draft_gte_read(12, 0));
w_u32(v61 + (8), gt_draft_gte_read(13, 0));
w_u32(v61 + (0x18), gt_draft_gte_read(14, 0));
        v61 += 28;
      }
      carrier_t3 += 20;
    }
    while ( v58 );
    v4 = r_u32(a0 + 932);
    w_u32(a0 + 952, v61);
    w_u32(a0 + 956, carrier_t3);
  }
  v81 = r_u16(v4 + 54);
  carrier_t2 = r_u32(a0 + 956);
    w_u32(a0 + 932, v4);
  if ( v81 )
  {
    v83 = (sint32)r_u32(carrier_t2);
    v84 = r_u32(a0 + 952);
    v85 = r_u32(a0 + 936);
    v86 = r_u32(a0 + 948);
    v87 = r_u16(a0 + 40);
    v88 = r_u32(a0 + 940);
    carrier_t8 = v85 + 8 * ((sint32)r_u32(carrier_t2) & 0x1FF);
    carrier_s1 = v85 + (((sint32)r_u32(carrier_t2) >> 6) & 0xFF8);
    carrier_t0 = v85 + (((sint32)r_u32(carrier_t2) >> 15) & 0xFF8);
    do
    {
gt_draft_gte_write(0, 0, r_u32(carrier_s1 + (0)));
gt_draft_gte_write(1, 0, r_u32(carrier_s1 + (4)));
gt_draft_gte_write(2, 0, r_u32(carrier_t0 + (0)));
gt_draft_gte_write(3, 0, r_u32(carrier_t0 + (4)));
gt_draft_gte_write(4, 0, r_u32(carrier_t8 + (0)));
gt_draft_gte_write(5, 0, r_u32(carrier_t8 + (4)));
      --v81;
      v92 = v83;
gt_draft_gte_command(0x280030);
      v83 = (sint32)r_u32(carrier_t2 + 4u * (6));
      v93 = (uint32)(v85 + 8 * ((sint32)r_u32(carrier_t2 + 4u * (1)) & 0x1FF));
      carrier_v1 = (sint32)r_u32(v93);
      carrier_a0 = (sint32)r_u32(v93 + 4u * (1));
gt_draft_gte_command(0x1400006);
      carrier_t8 = v85 + 8 * (v83 & 0x1FF);
      carrier_s1 = v85 + ((v83 >> 6) & 0xFF8);
gt_draft_gte_write(0, 0, carrier_v1);
gt_draft_gte_write(1, 0, carrier_a0);
carrier_a2 = gt_draft_gte_read(24, 0);
w_u32(v84 + (0x10), gt_draft_gte_read(12, 0));
carrier_v1 = gt_draft_gte_read(17, 0);
carrier_a0 = gt_draft_gte_read(18, 0);
carrier_t0 = gt_draft_gte_read(19, 0);
      v100 = (0u - carrier_a2);
gt_draft_gte_command(0x180001);
      if ( carrier_v1 < carrier_a0 )
        carrier_v1 = carrier_a0;
      if ( carrier_v1 < carrier_t0 )
        carrier_v1 = carrier_t0;
      carrier_t0 = v85 + ((v83 >> 15) & 0xFF8);
gt_draft_gte_write(6, 0, r_u32(carrier_t2 + (8)));
carrier_a0 = gt_draft_gte_read(19, 0);
      v102 = v92 & v100;
gt_draft_gte_command(0x1400006);
      v50 = carrier_v1 >= carrier_a0;
      v103 = carrier_v1 >> v87;
      if ( !v50 )
        v103 = carrier_a0 >> v87;
      if ( v103 >= 4096 )
        v103 = 4095;
      carrier_a0 = 0;
      if ( ((sint32)r_u32(carrier_t2 + 4u * (1)) & 0x8000000) == 0 )
        carrier_a0 = r_u16(2 * v103 + v88);
carrier_v0 = gt_draft_gte_read(24, 0);
gt_draft_gte_write(8, 0, carrier_a0);
gt_draft_gte_command(0x780010);
      if ( (v102 & carrier_v0) >= 0 )
      {
        v106 = (uint32)(v86 + 4 * (((v92 >> 23) & 0x30) + v103));
        carrier_v0 = (sint32)r_u32(carrier_t2 + 4u * (3));
        carrier_v1 = (sint32)r_u32(carrier_t2 + 4u * (4));
        carrier_a0 = (sint32)r_u32(carrier_t2 + 4u * (5));
w_u32(v84 + (4), gt_draft_gte_read(22, 0));
gt_draft_gte_write(20, 0, carrier_v0);
gt_draft_gte_write(21, 0, carrier_v1);
gt_draft_gte_write(22, 0, carrier_a0);
gt_draft_gte_command(0xF8002A);
    w_u32(v84, 0x8000000);
w_u32(v84 + (0x20), gt_draft_gte_read(12, 0));
w_u32(v84 + (8), gt_draft_gte_read(13, 0));
w_u32(v84 + (0x18), gt_draft_gte_read(14, 0));
    carrier_v1 = (carrier_v1 & 0xFFFF00FFu) | (((uint32)(r_u8(v106)) & 0xFFu) << 8);
    carrier_v1 = (carrier_v1 & 0xFF00FFFFu) | (((uint32)(r_u8(v106 + 1u * (1))) & 0xFFu) << 16);
    carrier_v1 = (carrier_v1 & 0x00FFFFFFu) | (((uint32)(r_u8(v106 + 1u * (2))) & 0xFFu) << 24);
    w_u16(v106, v84);
    w_u8(v106 + 1u * (2), (uint32)(v84 << 8) >> 24);
    w_u8(v84, (((uint32)(carrier_v1) >> 8) & 0xFFu));
    w_u8(v84 + 1, (((uint32)(carrier_v1) >> 16) & 0xFFu));
    w_u8(v84 + 2, (((uint32)(carrier_v1) >> 24) & 0xFFu));
w_u32(v84 + (0xC), gt_draft_gte_read(20, 0));
w_u32(v84 + (0x1C), gt_draft_gte_read(21, 0));
w_u32(v84 + (0x14), gt_draft_gte_read(22, 0));
        v84 += 36;
      }
      carrier_t2 += 24;
    }
    while ( v81 );
    v4 = r_u32(a0 + 932);
    w_u32(a0 + 952, v84);
    w_u32(a0 + 956, carrier_t2);
  }
  v110 = r_u16(v4 + 56);
  carrier_t3 = r_u32(a0 + 956);
    w_u32(a0 + 932, v4);
  if ( v110 )
  {
    v112 = r_u32(a0 + 952);
    v113 = r_u32(a0 + 936);
    v114 = r_u32(a0 + 948);
    v115 = r_u16(a0 + 40);
    v116 = r_u32(a0 + 940);
    v117 = (sint32)r_u32(carrier_t3);
    v118 = r_u32(a0 + 944);
    carrier_t7 = v113 + 8 * ((sint32)r_u32(carrier_t3) & 0x1FF);
    carrier_t5 = v113 + (((sint32)r_u32(carrier_t3) >> 6) & 0xFF8);
    carrier_t4 = v113 + (((sint32)r_u32(carrier_t3) >> 15) & 0xFF8);
    do
    {
gt_draft_gte_write(0, 0, r_u32(carrier_t5 + (0)));
gt_draft_gte_write(1, 0, r_u32(carrier_t5 + (4)));
gt_draft_gte_write(2, 0, r_u32(carrier_t7 + (0)));
gt_draft_gte_write(3, 0, r_u32(carrier_t7 + (4)));
gt_draft_gte_write(4, 0, r_u32(carrier_t4 + (0)));
gt_draft_gte_write(5, 0, r_u32(carrier_t4 + (4)));
      --v110;
      v122 = v117;
gt_draft_gte_command(0x280030);
      v117 = (sint32)r_u32(carrier_t3 + 4u * (3));
      v123 = (sint32)r_u32(carrier_t3 + 4u * (1));
      carrier_t7 = v113 + 8 * (v117 & 0x1FF);
      carrier_t5 = v113 + ((v117 >> 6) & 0xFF8);
      carrier_t4 = v113 + ((v117 >> 15) & 0xFF8);
      v124 = (uint32)(v118 + 32 * (v123 << 9 >> 18));
carrier_a0 = gt_draft_gte_read(17, 0);
carrier_a1 = gt_draft_gte_read(18, 0);
carrier_t0 = gt_draft_gte_read(19, 0);
gt_draft_gte_command(0x1400006);
      if ( carrier_a0 < carrier_a1 )
        carrier_a0 = carrier_a1;
      if ( carrier_a0 < carrier_t0 )
        carrier_a0 = carrier_t0;
      v128 = carrier_a0 >> v115;
carrier_a1 = gt_draft_gte_read(24, 0);
      if ( v128 >= 4096 )
        v128 = 4095;
      if ( (carrier_a1 & v122 & 0x80000000) == 0 )
      {
        carrier_v0 = 0;
        if ( (v123 & 0x8000000) == 0 )
          carrier_v0 = r_u16(2 * v128 + v116);
gt_draft_gte_write(6, 0, r_u32(carrier_t3 + (8)));
gt_draft_gte_write(8, 0, carrier_v0);
        v131 = (uint32)(v114 + 4 * (((v122 >> 23) & 0x30) + v128));
gt_draft_gte_command(0x780010);
        if ( carrier_a1 < 0 )
          carrier_a1 = (0u - carrier_a1);
        v132 = (uint32)(v124 + 16);
        if ( r_u16(v124) < (uint32)carrier_a1 )
          v132 = (uint32)(v124 + 4);
        v133 = (sint32)r_u32(v132);
        v134 = (sint32)r_u32(v132 + 4u * (1));
        v135 = (sint32)r_u32(v132 + 4u * (2));
    w_u32(a0 + 172, v112);
        if ( carrier_a1 < 800 )
        {
    w_u32(v112, 117440512);
    w_u32(v112 + 12, v133);
    w_u32(v112 + 20, v134);
    w_u32(v112 + 28, v135);
          v190 = (sint8)r_u8(v131);
          v191 = (sint8)r_u8(v131 + 1u * (1));
          v192 = (sint8)r_u8(v131 + 1u * (2));
    w_u16(v131, v112);
    w_u8(v131 + 1u * (2), (uint32)(v112 << 8) >> 24);
    w_u8(v112, v190);
    w_u8(v112 + 1, v191);
    w_u8(v112 + 2, v192);
w_u32(v112 + (4), gt_draft_gte_read(22, 0));
w_u32(v112 + (0x10), gt_draft_gte_read(12, 0));
w_u32(v112 + (8), gt_draft_gte_read(13, 0));
w_u32(v112 + (0x18), gt_draft_gte_read(14, 0));
          v112 += 32;
        }
        else
        {
    w_u16(a0 + 188, 5);
    w_u32(a0 + 968, carrier_t4);
    w_u32(a0 + 956, carrier_t3);
    w_u32(a0 + 984, v110);
    w_u32(a0 + 960, carrier_t7);
    w_u32(a0 + 964, carrier_t5);
    w_u32(a0 + 168, v131);
    w_u32(a0 + 184, 1152);
    w_u16(a0 + 306, v133);
    w_u16(a0 + 322, v134);
    w_u16(a0 + 338, v135);
w_u32(a0 + (0x9C), gt_draft_gte_read(22, 0));
    w_u32(a0 + 164, v134);
    w_u8(a0 + 164, (((uint32)(v133) >> 16) & 0xFFu));
    w_u8(a0 + 165, (((uint32)(v133) >> 24) & 0xFFu));
w_u32(a0 + (0x144), gt_draft_gte_read(0, 0));
w_u32(a0 + (0x148), gt_draft_gte_read(1, 0));
w_u32(a0 + (0x134), gt_draft_gte_read(2, 0));
w_u32(a0 + (0x138), gt_draft_gte_read(3, 0));
w_u32(a0 + (0x154), gt_draft_gte_read(4, 0));
w_u32(a0 + (0x158), gt_draft_gte_read(5, 0));
w_u32(a0 + (0x14C), gt_draft_gte_read(12, 0));
w_u32(a0 + (0x13C), gt_draft_gte_read(13, 0));
w_u32(a0 + (0x15C), gt_draft_gte_read(14, 0));
          v136 = 0;
          v137 = (uint32)(a0 + 204);
          v138 = r_u32(a0 + 172);
    w_u32(a0 + 204, a0 + 304);
    w_u32(a0 + 208, a0 + 320);
    w_u32(a0 + 212, a0 + 336);
          v139 = r_u32(a0 + 156);
          carrier_t1 = a0 + 352;
    w_u32(a0 + 176, v138);
    w_u32(a0 + 180, v138);
    w_u32(a0 + 160, v139 | 0x8000000);
LABEL_80:
          v141 = r_u32(v137);
          v142 = r_u32(v137 + 4u * (1));
          v143 = r_u32(v137 + 4u * (2));
          v144 = (sint16)r_u16(r_u32(v137) + 16);
          v145 = (sint16)r_u16(v142 + 4);
          v146 = (sint16)r_u16(v143 + 4);
    w_u16(carrier_t1 + 16, (v144 + v145) >> 1);
    w_u16(carrier_t1 + 80, (v145 + v146) >> 1);
    w_u16(carrier_t1 + 144, (v146 + v144) >> 1);
          v147 = (sint16)r_u16(v141 + 6);
          v148 = (sint16)r_u16(v142 + 6);
          v149 = (sint16)r_u16(v143 + 6);
    w_u16(carrier_t1 + 24, (v147 + v148) >> 1);
    w_u16(carrier_t1 + 88, (v148 + v149) >> 1);
    w_u16(carrier_t1 + 152, (v149 + v147) >> 1);
          v150 = (sint16)r_u16(v141 + 8);
          v151 = (sint16)r_u16(v142 + 8);
          v152 = (sint16)r_u16(v143 + 8);
    w_u16(carrier_t1 + 32, (v150 + v151) >> 1);
    w_u16(carrier_t1 + 96, (v151 + v152) >> 1);
    w_u16(carrier_t1 + 160, (v152 + v150) >> 1);
gt_draft_gte_write(0, 0, r_u32(carrier_t1 + (4)));
gt_draft_gte_write(1, 0, r_u32(carrier_t1 + (8)));
gt_draft_gte_write(2, 0, r_u32(carrier_t1 + (0x14)));
gt_draft_gte_write(3, 0, r_u32(carrier_t1 + (0x18)));
gt_draft_gte_write(4, 0, r_u32(carrier_t1 + (0x24)));
gt_draft_gte_write(5, 0, r_u32(carrier_t1 + (0x28)));
          v153 = r_u8(v141 + 2);
          v154 = r_u8(v142 + 2);
gt_draft_gte_command(0x280030);
          v155 = r_u8(v143 + 2);
    w_u8(carrier_t1 + 8, (v153 + v154) >> 1);
    w_u8(carrier_t1 + 72, (v154 + v155) >> 1);
    w_u8(carrier_t1 + 136, (v155 + v153) >> 1);
          v156 = r_u8(v141 + 3);
          v157 = r_u8(v142 + 3);
          v158 = r_u8(v143 + 3);
    w_u8(carrier_t1 + 12, (v156 + v157) >> 1);
    w_u8(carrier_t1 + 76, (v157 + v158) >> 1);
    w_u8(carrier_t1 + 140, (v158 + v156) >> 1);
w_u32(carrier_t1 + (0xC), gt_draft_gte_read(12, 0));
w_u32(carrier_t1 + (0x1C), gt_draft_gte_read(13, 0));
w_u32(carrier_t1 + (0x2C), gt_draft_gte_read(14, 0));
          v159 = r_u32(a0 + 180);
          v160 = r_u32(a0 + 156);
    v161 = (v161 & 0xFF00FFFFu) | (((uint32)(r_u8(a0 + 164)) & 0xFFu) << 16);
    v161 = (v161 & 0x00FFFFFFu) | (((uint32)(r_u8(a0 + 165)) & 0xFFu) << 24);
          v162 = r_u32(a0 + 164);
          v163 = 2;
          v164 = 1;
          do
          {
            carrier_a3 = r_u32(v137 + 4u * (v163));
            v166 = 16 * v164;
            if ( v164 < 0 )
            {
              v164 = 2;
              v166 = 32;
            }
            carrier_v1 = carrier_t1 + v166;
            carrier_v0 = r_u32(v137 + 4u * (v164));
gt_draft_gte_write(12, 0, r_u32(carrier_a3 + (0xC)));
gt_draft_gte_write(13, 0, r_u32(carrier_v1 + (0xC)));
gt_draft_gte_write(14, 0, r_u32(carrier_v0 + (0xC)));
    w_u8(v159 + 3, 7);
    carrier_v0 = (carrier_v0 & 0xFFFF0000u) | (((uint32)(r_u16(carrier_v0 + 8)) & 0xFFFFu) << 0);
gt_draft_gte_command(0x1400006);
    v161 = (v161 & 0xFFFF0000u) | (((uint32)(r_u16(carrier_a3 + 8)) & 0xFFFFu) << 0);
    w_u32(v159 + 28, carrier_v0);
    v162 = (v162 & 0xFFFF0000u) | (((uint32)(r_u16(carrier_v1 + 8)) & 0xFFFFu) << 0);
            v169 = (v159 + 32) << 8;
    w_u32(v159 + 12, v161);
    w_u32(v159 + 20, v162);
    w_u8(v159, (((uint32)(v169) >> 8) & 0xFFu));
    w_u8(v159 + 1, (((uint32)(v169) >> 16) & 0xFFu));
    w_u8(v159 + 2, (((uint32)(v169) >> 24) & 0xFFu));
carrier_v0 = gt_draft_gte_read(24, 0);
w_u32(v159 + (8), gt_draft_gte_read(12, 0));
w_u32(v159 + (0x10), gt_draft_gte_read(13, 0));
            if ( carrier_v0 )
            {
w_u32(v159 + (0x18), gt_draft_gte_read(14, 0));
    w_u32(v159 + 4, v160);
    w_u32(a0 + 176, v159);
              v159 += 32;
            }
            v164 = --v163 - 1;
          }
          while ( v163 >= 0 );
    w_u32(a0 + 180, v159);
          v171 = 3;
          do
          {
            v172 = v171 - 1;
            if ( v171 >= 0 )
            {
              while ( 1 )
              {
                if ( v171 == 3 )
                {
                  carrier_t3 = carrier_t1;
                  carrier_t2 = carrier_t1 + 64;
                  carrier_t0 = carrier_t1 + 128;
                }
                else
                {
                  carrier_t3 = r_u32(v137 + 4u * (v171));
                  carrier_t2 = carrier_t1 + 64 * v171;
                  if ( v172 < 0 )
                    v172 = 2;
                  carrier_t0 = carrier_t1 + 64 * v172;
                }
                --v171;
gt_draft_gte_write(12, 0, r_u32(carrier_t2 + (0xC)));
gt_draft_gte_write(13, 0, r_u32(carrier_t3 + (0xC)));
gt_draft_gte_write(14, 0, r_u32(carrier_t0 + (0xC)));
                v176 = r_u32(a0 + 184);
gt_draft_gte_command(0x1400006);
                v177 = r_u32(a0 + 180);
                v178 = v136 < r_u16(a0 + 188);
    v179 = (v179 & 0xFF00FFFFu) | (((uint32)(r_u8(a0 + 164)) & 0xFFu) << 16);
    v179 = (v179 & 0x00FFFFFFu) | (((uint32)(r_u8(a0 + 165)) & 0xFFu) << 24);
                v180 = r_u32(a0 + 164);
    v179 = (v179 & 0xFFFF0000u) | (((uint32)(r_u16(carrier_t2 + 8)) & 0xFFFFu) << 0);
    v180 = (v180 & 0xFFFF0000u) | (((uint32)(r_u16(carrier_t3 + 8)) & 0xFFFFu) << 0);
    w_u32(v177 + 12, v179);
    w_u32(v177 + 20, v180);
carrier_v0 = gt_draft_gte_read(24, 0);
w_u32(v177 + (8), gt_draft_gte_read(12, 0));
w_u32(v177 + (0x10), gt_draft_gte_read(13, 0));
                if ( carrier_v0 < 0 )
                  carrier_v0 = (0u - carrier_v0);
w_u32(v177 + (0x18), gt_draft_gte_read(14, 0));
                if ( carrier_v0 >= v176 )
                {
                  v182 = a0 + v136;
                  if ( v178 )
                    break;
                }
    v180 = (v180 & 0xFFFF0000u) | (((uint32)(r_u16(carrier_t0 + 8)) & 0xFFFFu) << 0);
    w_u32(v177 + 4, r_u32(a0 + 156));
    w_u32(v177 + 28, v180);
    w_u8(v177 + 3, 7);
                v183 = (v177 + 32) << 8;
    w_u8(v177, (((uint32)(v183) >> 8) & 0xFFu));
    w_u8(v177 + 1, (((uint32)(v183) >> 16) & 0xFFu));
    w_u8(v177 + 2, (((uint32)(v183) >> 24) & 0xFFu));
                v172 = v171 - 1;
    w_u32(a0 + 176, v177);
    w_u32(a0 + 180, v177 + 32);
                if ( v171 < 0 )
                  goto LABEL_98;
              }
              v137 += 12;
              carrier_t1 += 192;
              ++v136;
    w_u8(v182 + 196, v171);
    w_u32(v137, carrier_t3);
    w_u32(v137 + 4u * (1), carrier_t2);
    w_u32(v137 + 4u * (2), carrier_t0);
              goto LABEL_80;
            }
LABEL_98:
            --v136;
            v171 = (sint8)r_u8(a0 + v136 + 196);
            v137 -= 3;
            carrier_t1 -= 48;
          }
          while ( v136 >= 0 );
          v184 = r_u32(a0 + 168);
          v185 = r_u32(a0 + 176);
          v186 = r_u32(a0 + 172) << 8;
          v187 = (sint8)r_u8(v184);
          v188 = (sint8)r_u8(v184 + 1u * (1));
          v189 = (sint8)r_u8(v184 + 1u * (2));
    w_u8(v184, (((uint32)(v186) >> 8) & 0xFFu));
    w_u8(v184 + 1u * (1), (((uint32)(v186) >> 16) & 0xFFu));
    w_u8(v184 + 1u * (2), (((uint32)(v186) >> 24) & 0xFFu));
    w_u8(v185, v187);
    w_u8(v185 + 1u * (1), v188);
    w_u8(v185 + 1u * (2), v189);
          v112 = r_u32(a0 + 180);
          v113 = r_u32(a0 + 936);
          v114 = r_u32(a0 + 948);
          v115 = r_u16(a0 + 40);
          v116 = r_u32(a0 + 940);
          v118 = r_u32(a0 + 944);
          carrier_t3 = r_u32(a0 + 956);
          v110 = r_u32(a0 + 984);
          carrier_t7 = r_u32(a0 + 960);
          carrier_t5 = r_u32(a0 + 964);
          carrier_t4 = r_u32(a0 + 968);
        }
      }
      carrier_t3 += 12;
    }
    while ( v110 );
    v4 = r_u32(a0 + 932);
    w_u32(a0 + 952, v112);
    w_u32(a0 + 956, carrier_t3);
  }
  v193 = r_u16(v4 + 58);
  carrier_s4 = r_u32(a0 + 956);
    w_u32(a0 + 932, v4);
  if ( v193 )
  {
    v195 = r_u32(a0 + 952);
    v196 = r_u32(a0 + 936);
    v197 = r_u32(a0 + 948);
    v198 = r_u16(a0 + 40);
    v199 = r_u32(a0 + 940);
    v200 = (sint32)r_u32(carrier_s4);
    v201 = r_u32(a0 + 944);
    carrier_t6 = v196 + 8 * ((sint32)r_u32(carrier_s4) & 0x1FF);
    carrier_s6 = v196 + (((sint32)r_u32(carrier_s4) >> 6) & 0xFF8);
    carrier_s5 = v196 + (((sint32)r_u32(carrier_s4) >> 15) & 0xFF8);
    do
    {
gt_draft_gte_write(0, 0, r_u32(carrier_s6 + (0)));
gt_draft_gte_write(1, 0, r_u32(carrier_s6 + (4)));
gt_draft_gte_write(2, 0, r_u32(carrier_s5 + (0)));
gt_draft_gte_write(3, 0, r_u32(carrier_s5 + (4)));
w_u32(a0 + (0x144), gt_draft_gte_read(0, 0));
w_u32(a0 + (0x148), gt_draft_gte_read(1, 0));
gt_draft_gte_write(4, 0, r_u32(carrier_t6 + (0)));
gt_draft_gte_write(5, 0, r_u32(carrier_t6 + (4)));
      --v193;
      v205 = v200;
gt_draft_gte_command(0x280030);
      v206 = (sint32)r_u32(carrier_s4 + 4u * (1));
      v200 = (sint32)r_u32(carrier_s4 + 4u * (3));
      v207 = (uint32)(v196 + 8 * (v206 & 0x1FF));
      carrier_v1 = (sint32)r_u32(v207);
      carrier_a0 = (sint32)r_u32(v207 + 4u * (1));
gt_draft_gte_command(0x1400006);
      carrier_t6 = v196 + 8 * (v200 & 0x1FF);
      carrier_s6 = v196 + ((v200 >> 6) & 0xFF8);
gt_draft_gte_write(0, 0, carrier_v1);
gt_draft_gte_write(1, 0, carrier_a0);
carrier_a3 = gt_draft_gte_read(24, 0);
w_u32(v195 + (0x10), gt_draft_gte_read(12, 0));
w_u32(a0 + (0x14C), gt_draft_gte_read(12, 0));
carrier_v1 = gt_draft_gte_read(17, 0);
carrier_a0 = gt_draft_gte_read(18, 0);
carrier_a1 = gt_draft_gte_read(19, 0);
      v214 = (0u - carrier_a3);
gt_draft_gte_command(0x180001);
      if ( carrier_v1 < carrier_a0 )
        carrier_v1 = carrier_a0;
      v215 = (v205 >> 23) & 0x30;
      if ( carrier_v1 < carrier_a1 )
        carrier_v1 = carrier_a1;
      carrier_s5 = v196 + ((v200 >> 15) & 0xFF8);
gt_draft_gte_write(6, 0, r_u32(carrier_s4 + (8)));
carrier_a0 = gt_draft_gte_read(19, 0);
      v217 = v205 & v214;
gt_draft_gte_command(0x1400006);
      v50 = carrier_v1 >= carrier_a0;
      v218 = carrier_v1 >> v198;
      if ( !v50 )
        v218 = carrier_a0 >> v198;
      if ( v218 >= 4096 )
        v218 = 4095;
      carrier_v0 = 0;
      if ( (v206 & 0x8000000) == 0 )
        carrier_v0 = r_u16(2 * v218 + v199);
carrier_a1 = gt_draft_gte_read(24, 0);
gt_draft_gte_write(8, 0, carrier_v0);
      v221 = v215 + v218;
gt_draft_gte_command(0x780010);
      if ( (v217 & carrier_a1) >= 0 )
      {
        v222 = (uint32)(v197 + 4 * v221);
        if ( v214 < 0 )
          v214 = -v214;
        v223 = (uint32)(v201 + 32 * ((v206 >> 9) & 0x3FFF));
        if ( carrier_a1 < 0 )
          carrier_a1 = (0u - carrier_a1);
        v224 = v214 + carrier_a1;
        v225 = (uint32)(v223 + 16);
        if ( r_u16(v223) < (uint32)(v214 + carrier_a1) )
          v225 = (uint32)(v223 + 4);
        v226 = (sint32)r_u32(v225);
        v227 = (sint32)r_u32(v225 + 4u * (1));
        v228 = (sint32)r_u32(v225 + 4u * (2));
    w_u32(a0 + 172, v195);
        if ( v224 < 1568 )
        {
    w_u32(v195, 150994944);
    w_u32(v195 + 12, v226);
    w_u32(v195 + 20, v227);
    w_u32(v195 + 36, v228);
    w_u8(v195 + 28, (((uint32)(v228) >> 16) & 0xFFu));
    w_u8(v195 + 29, (((uint32)(v228) >> 24) & 0xFFu));
w_u32(v195 + (0x20), gt_draft_gte_read(12, 0));
w_u32(v195 + (8), gt_draft_gte_read(13, 0));
w_u32(v195 + (0x18), gt_draft_gte_read(14, 0));
          v342 = (sint8)r_u8(v222);
          v343 = (sint8)r_u8(v222 + 1u * (1));
          v344 = (sint8)r_u8(v222 + 1u * (2));
    w_u16(v222, v195);
    w_u8(v222 + 1u * (2), (uint32)(v195 << 8) >> 24);
    w_u8(v195, v342);
    w_u8(v195 + 1, v343);
    w_u8(v195 + 2, v344);
w_u32(v195 + (4), gt_draft_gte_read(22, 0));
          v195 += 40;
        }
        else
        {
    w_u16(a0 + 188, 5);
          carrier_t4 = a0;
    w_u32(a0 + 976, v200);
    w_u32(a0 + 960, carrier_t6);
    w_u32(a0 + 168, v222);
    w_u32(a0 + 184, 2048);
w_u32(a0 + (0x9C), gt_draft_gte_read(22, 0));
    w_u32(a0 + 164, v227);
    w_u8(a0 + 164, (((uint32)(v226) >> 16) & 0xFFu));
    w_u8(a0 + 165, (((uint32)(v226) >> 24) & 0xFFu));
    w_u16(a0 + 306, v226);
    w_u16(a0 + 322, v227);
    w_u16(a0 + 338, v228);
    w_u32(a0 + 352, v228);
w_u32(a0 + (0x164), gt_draft_gte_read(0, 0));
w_u32(a0 + (0x168), gt_draft_gte_read(1, 0));
w_u32(a0 + (0x154), gt_draft_gte_read(2, 0));
w_u32(a0 + (0x158), gt_draft_gte_read(3, 0));
w_u32(a0 + (0x134), gt_draft_gte_read(4, 0));
w_u32(a0 + (0x138), gt_draft_gte_read(5, 0));
w_u32(a0 + (0x15C), gt_draft_gte_read(12, 0));
w_u32(a0 + (0x13C), gt_draft_gte_read(13, 0));
w_u32(a0 + (0x16C), gt_draft_gte_read(14, 0));
          if ( v224 >= 3136 )
          {
            v272 = 0;
            v270 = (uint32)(a0 + 204);
            carrier_t3 = a0 + 368;
            v273 = r_u32(a0 + 172);
            v274 = r_u32(a0 + 156);
    w_u32(a0 + 204, a0 + 304);
    w_u32(a0 + 208, a0 + 320);
    w_u32(a0 + 212, a0 + 336);
    w_u32(a0 + 216, a0 + 352);
    w_u32(a0 + 176, v273);
    w_u32(a0 + 180, v273);
    w_u32(a0 + 160, v274 & 0xF7FFFFFF);
LABEL_128:
            v309 = r_u32(v270 + 4u * (1));
            v275 = r_u32(v270 + 4u * (2));
            v276 = r_u32(v270);
            v277 = r_u32(v270 + 4u * (3));
            v278 = (sint16)r_u16(v309 + 4);
            v279 = (sint16)r_u16(v275 + 4);
            v280 = (sint16)r_u16(r_u32(v270) + 16);
            v281 = (sint16)r_u16(v277 + 4);
    w_u16(carrier_t3 + 144, (v278 + v279) >> 1);
            v282 = (v281 + v280) >> 1;
            v283 = v280 + v278;
    w_u16(carrier_t3 + 272, v282);
    w_u16(carrier_t3 + 80, v283 >> 1);
    w_u16(carrier_t3 + 208, (v279 + v281) >> 1);
    w_u16(carrier_t3 + 16, (v283 + v279 + v281) >> 2);
            v284 = (sint16)r_u16(v309 + 6);
            v285 = (sint16)r_u16(v275 + 6);
            v286 = (sint16)r_u16(v276 + 6);
            v287 = (sint16)r_u16(v277 + 6);
    w_u16(carrier_t3 + 152, (v284 + v285) >> 1);
            v288 = (v287 + v286) >> 1;
            v289 = v286 + v284;
    w_u16(carrier_t3 + 280, v288);
    w_u16(carrier_t3 + 88, v289 >> 1);
    w_u16(carrier_t3 + 216, (v285 + v287) >> 1);
    w_u16(carrier_t3 + 24, (v289 + v285 + v287) >> 2);
            v290 = (sint16)r_u16(v309 + 8);
            v291 = (sint16)r_u16(v275 + 8);
            v292 = (sint16)r_u16(v276 + 8);
            v293 = (sint16)r_u16(v277 + 8);
    w_u16(carrier_t3 + 160, (v290 + v291) >> 1);
            v294 = (v293 + v292) >> 1;
            v295 = v292 + v290;
    w_u16(carrier_t3 + 288, v294);
    w_u16(carrier_t3 + 96, v295 >> 1);
    w_u16(carrier_t3 + 224, (v291 + v293) >> 1);
    w_u16(carrier_t3 + 32, (v295 + v291 + v293) >> 2);
gt_draft_gte_write(0, 0, r_u32(carrier_t3 + (4)));
gt_draft_gte_write(1, 0, r_u32(carrier_t3 + (8)));
gt_draft_gte_write(2, 0, r_u32(carrier_t3 + (0x14)));
gt_draft_gte_write(3, 0, r_u32(carrier_t3 + (0x18)));
gt_draft_gte_write(4, 0, r_u32(carrier_t3 + (0x24)));
gt_draft_gte_write(5, 0, r_u32(carrier_t3 + (0x28)));
            v296 = r_u8(v276 + 2);
            v297 = r_u8(v309 + 2);
gt_draft_gte_command(0x280030);
            v298 = r_u8(v275 + 2);
            v299 = r_u8(v277 + 2);
    w_u8(carrier_t3 + 136, (v297 + v298) >> 1);
            v300 = (v299 + v296) >> 1;
            v301 = v296 + v297;
    w_u8(carrier_t3 + 264, v300);
    w_u8(carrier_t3 + 72, v301 >> 1);
    w_u8(carrier_t3 + 200, (v298 + v299) >> 1);
    w_u8(carrier_t3 + 8, (v301 + v298 + v299) >> 2);
w_u32(carrier_t3 + (0xC), gt_draft_gte_read(12, 0));
w_u32(carrier_t3 + (0x1C), gt_draft_gte_read(13, 0));
w_u32(carrier_t3 + (0x2C), gt_draft_gte_read(14, 0));
gt_draft_gte_write(2, 0, r_u32(carrier_t3 + (0x34)));
gt_draft_gte_write(3, 0, r_u32(carrier_t3 + (0x38)));
gt_draft_gte_write(4, 0, r_u32(carrier_t3 + (0x44)));
gt_draft_gte_write(5, 0, r_u32(carrier_t3 + (0x48)));
            v302 = r_u8(v276 + 3);
            v303 = r_u8(v309 + 3);
gt_draft_gte_command(0x280030);
            v304 = r_u8(v275 + 3);
            v305 = r_u8(v277 + 3);
    w_u8(carrier_t3 + 140, (v303 + v304) >> 1);
            v306 = (v305 + v302) >> 1;
            v307 = v302 + v303;
    w_u8(carrier_t3 + 268, v306);
    w_u8(carrier_t3 + 76, v307 >> 1);
    w_u8(carrier_t3 + 204, (v304 + v305) >> 1);
    w_u8(carrier_t3 + 12, (v307 + v304 + v305) >> 2);
w_u32(carrier_t3 + (0x3C), gt_draft_gte_read(13, 0));
w_u32(carrier_t3 + (0x4C), gt_draft_gte_read(14, 0));
            v308 = r_u32(a0 + 180);
    v309 = (v309 & 0xFF00FFFFu) | (((uint32)(r_u8(a0 + 164)) & 0xFFu) << 16);
    v309 = (v309 & 0x00FFFFFFu) | (((uint32)(r_u8(a0 + 165)) & 0xFFu) << 24);
            v310 = r_u32(a0 + 164);
            v311 = 3;
            do
            {
              carrier_v1 = r_u32(v270 + 4u * (1));
              carrier_a1 = r_u32(v270);
              carrier_a3 = carrier_t3 + 64;
              if ( !v311 )
                carrier_v1 = r_u32(v270 - 48u);
gt_draft_gte_write(12, 0, r_u32(carrier_a1 + (0xC)));
gt_draft_gte_write(13, 0, r_u32(carrier_v1 + (0xC)));
gt_draft_gte_write(14, 0, r_u32(carrier_a3 + (0xC)));
              --v311;
              v270 += 16;
gt_draft_gte_command(0x1400006);
              v315 = r_u32(a0 + 160);
    v309 = (v309 & 0xFFFF0000u) | (((uint32)(r_u16(carrier_a1 + 8)) & 0xFFFFu) << 0);
    w_u32(v308 + 4, v315);
    v310 = (v310 & 0xFFFF0000u) | (((uint32)(r_u16(carrier_v1 + 8)) & 0xFFFFu) << 0);
    w_u32(v308 + 12, v309);
    v315 = (v315 & 0xFFFF0000u) | (((uint32)(r_u16(carrier_t3 + 72)) & 0xFFFFu) << 0);
    w_u32(v308 + 20, v310);
    w_u32(v308 + 28, v315);
carrier_v0 = gt_draft_gte_read(24, 0);
w_u32(v308 + (8), gt_draft_gte_read(12, 0));
w_u32(v308 + (0x10), gt_draft_gte_read(13, 0));
              if ( carrier_v0 )
              {
w_u32(v308 + (0x18), gt_draft_gte_read(14, 0));
                v317 = (v308 + 32) << 8;
    w_u8(v308 + 3, 7);
    w_u8(v308, (((uint32)(v317) >> 8) & 0xFFu));
    w_u8(v308 + 1, (((uint32)(v317) >> 16) & 0xFFu));
    w_u8(v308 + 2, (((uint32)(v317) >> 24) & 0xFFu));
    w_u32(a0 + 176, v308);
                v308 += 32;
              }
              carrier_t3 += 64;
            }
            while ( v311 >= 0 );
    w_u32(a0 + 180, v308);
            v318 = v270 - 16;
            v319 = carrier_t3 - 256;
            v320 = 3;
            do
            {
              v321 = v320 - 1;
              if ( v320 >= 0 )
              {
                while ( 1 )
                {
                  v322 = v321 & 3;
                  if ( (v320 & 1) != 0 )
                  {
                    carrier_t5 = v319 + 16 * v322 + 16;
                    carrier_t0 = r_u32(v318 + 4u * (v320));
                    carrier_t2 = v319 + 16 * v320 + 16;
                    carrier_t1 = v319;
                  }
                  else
                  {
                    carrier_t5 = r_u32(v318 + 4u * (v320));
                    carrier_t0 = v319 + 16 * v320 + 16;
                    carrier_t2 = v319;
                    carrier_t1 = v319 + 16 * v322 + 16;
                  }
                  --v320;
gt_draft_gte_write(12, 0, r_u32(carrier_t0 + (0xC)));
gt_draft_gte_write(13, 0, r_u32(carrier_t5 + (0xC)));
gt_draft_gte_write(14, 0, r_u32(carrier_t2 + (0xC)));
                  v327 = r_u32(a0 + 180);
gt_draft_gte_command(0x1400006);
    w_u32(v327 + 4, r_u32(a0 + 156));
    v328 = (v328 & 0xFF00FFFFu) | (((uint32)(r_u8(a0 + 164)) & 0xFFu) << 16);
    v328 = (v328 & 0x00FFFFFFu) | (((uint32)(r_u8(a0 + 165)) & 0xFFu) << 24);
                  v329 = r_u32(a0 + 164);
    v328 = (v328 & 0xFFFF0000u) | (((uint32)(r_u16(carrier_t0 + 8)) & 0xFFFFu) << 0);
    v329 = (v329 & 0xFFFF0000u) | (((uint32)(r_u16(carrier_t5 + 8)) & 0xFFFFu) << 0);
    w_u32(v327 + 12, v328);
    w_u32(v327 + 20, v329);
    v328 = (v328 & 0xFFFF0000u) | (((uint32)(r_u16(carrier_t2 + 8)) & 0xFFFFu) << 0);
carrier_a0 = gt_draft_gte_read(24, 0);
gt_draft_gte_write(12, 0, r_u32(carrier_t1 + (0xC)));
    v329 = (v329 & 0xFFFF0000u) | (((uint32)(r_u16(carrier_t1 + 8)) & 0xFFFFu) << 0);
    w_u32(v327 + 28, v328);
                  v331 = r_u32(carrier_t0 + 48);
gt_draft_gte_command(0x1400006);
    w_u32(v327 + 36, v329);
    w_u32(v327 + 8, v331);
    w_u8(v327 + 3, 9);
                  v332 = (v327 + 40) << 8;
    w_u8(v327, (((uint32)(v332) >> 8) & 0xFFu));
    w_u8(v327 + 1, (((uint32)(v332) >> 16) & 0xFFu));
    w_u8(v327 + 2, (((uint32)(v332) >> 24) & 0xFFu));
carrier_v0 = gt_draft_gte_read(24, 0);
                  v334 = carrier_a0 - carrier_v0;
w_u32(v327 + (0x10), gt_draft_gte_read(13, 0));
                  if ( v334 < 0 )
                    v334 = -v334;
                  if ( v334 >= r_u32(a0 + 184) )
                  {
                    v335 = a0 + v272;
                    if ( v272 < r_u16(a0 + 188) )
                      break;
                  }
w_u32(v327 + (0x18), gt_draft_gte_read(14, 0));
w_u32(v327 + (0x20), gt_draft_gte_read(12, 0));
                  v321 = v320 - 1;
    w_u32(a0 + 176, v327);
    w_u32(a0 + 180, v327 + 40);
                  if ( v320 < 0 )
                    goto LABEL_144;
                }
                v270 = v318 + 16;
                carrier_t3 = v319 + 80;
                ++v272;
    w_u8(v335 + 196, v320);
    w_u32(v270, carrier_t5);
    w_u32(v270 + 4u * (1), carrier_t0);
    w_u32(v270 + 4u * (2), carrier_t2);
    w_u32(v270 + 4u * (3), carrier_t1);
                goto LABEL_128;
              }
LABEL_144:
              --v272;
              v320 = (sint8)r_u8(a0 + v272 + 196);
              v318 -= 4;
              v319 -= 80;
            }
            while ( v272 >= 0 );
            v336 = r_u32(a0 + 168);
            v337 = r_u32(a0 + 176);
            v338 = r_u32(a0 + 172) << 8;
            v339 = (sint8)r_u8(v336);
            v340 = (sint8)r_u8(v336 + 1u * (1));
            v341 = (sint8)r_u8(v336 + 1u * (2));
    w_u8(v336, (((uint32)(v338) >> 8) & 0xFFu));
    w_u8(v336 + 1u * (1), (((uint32)(v338) >> 16) & 0xFFu));
    w_u8(v336 + 1u * (2), (((uint32)(v338) >> 24) & 0xFFu));
    w_u8(v337, v339);
    w_u8(v337 + 1u * (1), v340);
    w_u8(v337 + 1u * (2), v341);
          }
          else
          {
            v230 = r_u32(a0 + 172);
            v231 = r_u32(a0 + 156);
            v232 = r_u16(a0 + 164);
            v233 = r_u16(a0 + 166);
            v234 = v230 & 0xFFFFFF | 0x9000000;
    w_u32(v230, v234 + 40);
    w_u32(v230 + 40, v234 + 80);
    w_u32(v230 + 80, v234 + 120);
    w_u32(v230 + 120, v234 + 160);
    w_u32(v230 + 160, v234 + 200);
    w_u32(v230 + 200, v234);
    w_u32(v230 + 4, v231);
    w_u32(v230 + 44, v231);
    w_u32(v230 + 84, v231);
    w_u32(v230 + 124, v231);
    w_u32(v230 + 164, v231);
    w_u32(v230 + 204, v231);
    w_u16(v230 + 14, v232);
    w_u16(v230 + 54, v232);
    w_u16(v230 + 94, v232);
    w_u16(v230 + 134, v232);
    w_u16(v230 + 174, v232);
    w_u16(v230 + 214, v232);
    w_u16(v230 + 22, v233);
    w_u16(v230 + 62, v233);
    w_u16(v230 + 102, v233);
    w_u16(v230 + 142, v233);
    w_u16(v230 + 182, v233);
    w_u16(v230 + 222, v233);
            v235 = r_u32(a0 + 316);
            v236 = r_u32(a0 + 332);
            v237 = r_u32(a0 + 348);
            v238 = r_u32(a0 + 364);
    w_u32(v230 + 208, v235);
    w_u32(v230 + 24, v235);
    w_u32(v230 + 8, v236);
    w_u32(v230 + 48, v236);
    w_u32(v230 + 104, v236);
    w_u32(v230 + 88, v237);
    w_u32(v230 + 144, v237);
    w_u32(v230 + 128, v238);
    w_u32(v230 + 168, v238);
    w_u32(v230 + 224, v238);
    v235 = (v235 & 0xFFFF0000u) | (((uint32)(r_u16(a0 + 306)) & 0xFFFFu) << 0);
    v236 = (v236 & 0xFFFF0000u) | (((uint32)(r_u16(a0 + 322)) & 0xFFFFu) << 0);
    v237 = (v237 & 0xFFFF0000u) | (((uint32)(r_u16(a0 + 338)) & 0xFFFFu) << 0);
    v238 = (v238 & 0xFFFF0000u) | (((uint32)(r_u16(a0 + 354)) & 0xFFFFu) << 0);
    w_u16(v230 + 212, v235);
    w_u16(v230 + 28, v235);
    w_u16(v230 + 12, v236);
    w_u16(v230 + 52, v236);
    w_u16(v230 + 108, v236);
    w_u16(v230 + 92, v237);
    w_u16(v230 + 148, v237);
    w_u16(v230 + 132, v238);
    w_u16(v230 + 172, v238);
    w_u16(v230 + 228, v238);
            v239 = (sint16)r_u16(a0 + 308);
            v240 = (sint16)r_u16(a0 + 324);
            v241 = (sint16)r_u16(a0 + 340);
            v242 = (sint16)r_u16(a0 + 356);
            v243 = v239 + v240;
            v244 = (v239 + v240 + v241 + v242) >> 2;
            v245 = v240 + v241;
            v246 = v241 + v242;
            v247 = v242 + v239;
            v248 = (sint16)r_u16(a0 + 310);
    w_u16(a0 + 404, v245 >> 1);
            v249 = (sint16)r_u16(a0 + 326);
    w_u16(a0 + 420, v246 >> 1);
            v250 = (sint16)r_u16(a0 + 342);
    w_u16(a0 + 436, v247 >> 1);
            v251 = (sint16)r_u16(a0 + 358);
    w_u16(a0 + 372, v244);
    w_u16(a0 + 388, v243 >> 1);
            v252 = v248 + v249;
            v253 = (v248 + v249 + v250 + v251) >> 2;
            v254 = v249 + v250;
            v255 = v250 + v251;
            v256 = v251 + v248;
    w_u16(a0 + 374, v253);
            v257 = (sint16)r_u16(a0 + 312);
    w_u16(a0 + 406, v254 >> 1);
            v258 = (sint16)r_u16(a0 + 328);
    w_u16(a0 + 422, v255 >> 1);
            v259 = (sint16)r_u16(a0 + 344);
    w_u16(a0 + 438, v256 >> 1);
            v260 = (sint16)r_u16(a0 + 360);
    w_u16(a0 + 390, v252 >> 1);
    w_u16(a0 + 376, (v257 + v258 + v259 + v260) >> 2);
    w_u16(a0 + 392, (v257 + v258) >> 1);
    w_u16(a0 + 408, (v258 + v259) >> 1);
    w_u16(a0 + 424, (v259 + v260) >> 1);
    w_u16(a0 + 440, (v260 + v257) >> 1);
gt_draft_gte_write(0, 0, r_u32(carrier_t4 + (0x184)));
gt_draft_gte_write(1, 0, r_u32(carrier_t4 + (0x188)));
gt_draft_gte_write(2, 0, r_u32(carrier_t4 + (0x194)));
gt_draft_gte_write(3, 0, r_u32(carrier_t4 + (0x198)));
gt_draft_gte_write(4, 0, r_u32(carrier_t4 + (0x1A4)));
gt_draft_gte_write(5, 0, r_u32(carrier_t4 + (0x1A8)));
            v261 = r_u8(a0 + 306);
            v262 = r_u8(a0 + 322);
gt_draft_gte_command(0x280030);
            v263 = r_u8(a0 + 338);
            v264 = r_u8(a0 + 354);
    w_u8(a0 + 370, (uint32)(v261 + v262 + v263 + v264) >> 2);
    w_u8(a0 + 386, (uint32)(v261 + v262) >> 1);
    w_u8(a0 + 402, (uint32)(v262 + v263) >> 1);
    w_u8(a0 + 418, (uint32)(v263 + v264) >> 1);
    w_u8(a0 + 434, (uint32)(v264 + v261) >> 1);
w_u32(v230 + (0x10), gt_draft_gte_read(12, 0));
w_u32(v230 + (0x38), gt_draft_gte_read(12, 0));
w_u32(v230 + (0x60), gt_draft_gte_read(13, 0));
w_u32(v230 + (0x98), gt_draft_gte_read(13, 0));
w_u32(v230 + (0xC0), gt_draft_gte_read(13, 0));
w_u32(v230 + (0x88), gt_draft_gte_read(14, 0));
w_u32(v230 + (0xB0), gt_draft_gte_read(14, 0));
gt_draft_gte_write(0, 0, r_u32(carrier_t4 + (0x1B4)));
gt_draft_gte_write(1, 0, r_u32(carrier_t4 + (0x1B8)));
gt_draft_gte_write(2, 0, r_u32(carrier_t4 + (0x174)));
gt_draft_gte_write(3, 0, r_u32(carrier_t4 + (0x178)));
            v265 = r_u8(a0 + 307);
            v266 = r_u8(a0 + 323);
gt_draft_gte_command(0x280030);
            v267 = r_u8(a0 + 339);
            v268 = r_u8(a0 + 355);
    w_u8(a0 + 371, (uint32)(v265 + v266 + v267 + v268) >> 2);
    w_u8(a0 + 387, (uint32)(v265 + v266) >> 1);
    w_u8(a0 + 403, (uint32)(v266 + v267) >> 1);
    w_u8(a0 + 419, (uint32)(v267 + v268) >> 1);
    w_u8(a0 + 435, (uint32)(v268 + v265) >> 1);
w_u32(v230 + (0xD8), gt_draft_gte_read(12, 0));
w_u32(v230 + (0x20), gt_draft_gte_read(12, 0));
w_u32(v230 + (0x48), gt_draft_gte_read(12, 0));
w_u32(v230 + (0x40), gt_draft_gte_read(13, 0));
w_u32(v230 + (0x70), gt_draft_gte_read(13, 0));
w_u32(v230 + (0xB8), gt_draft_gte_read(13, 0));
w_u32(v230 + (0xE8), gt_draft_gte_read(13, 0));
    v253 = (v253 & 0xFFFF0000u) | (((uint32)(r_u16(a0 + 370)) & 0xFFFFu) << 0);
    v268 = (v268 & 0xFFFF0000u) | (((uint32)(r_u16(a0 + 386)) & 0xFFFFu) << 0);
    v259 = (v259 & 0xFFFF0000u) | (((uint32)(r_u16(a0 + 402)) & 0xFFFFu) << 0);
    v266 = (v266 & 0xFFFF0000u) | (((uint32)(r_u16(a0 + 418)) & 0xFFFFu) << 0);
    v267 = (v267 & 0xFFFF0000u) | (((uint32)(r_u16(a0 + 434)) & 0xFFFFu) << 0);
    w_u16(v230 + 68, v253);
    w_u16(v230 + 116, v253);
    w_u16(v230 + 188, v253);
    w_u16(v230 + 236, v253);
    w_u16(v230 + 20, v268);
    w_u16(v230 + 60, v268);
    w_u16(v230 + 100, v259);
    w_u16(v230 + 156, v259);
    w_u16(v230 + 196, v259);
    w_u16(v230 + 140, v266);
    w_u16(v230 + 180, v266);
    w_u16(v230 + 220, v267);
    w_u16(v230 + 36, v267);
    w_u16(v230 + 76, v267);
            v269 = r_u32(a0 + 168);
    w_u32(a0 + 176, v230 + 200);
    w_u32(a0 + 180, v230 + 240);
    v259 = (v259 & 0xFFFF00FFu) | (((uint32)(r_u8(v269)) & 0xFFu) << 8);
    v259 = (v259 & 0xFF00FFFFu) | (((uint32)(r_u8(v269 + 1u * (1))) & 0xFFu) << 16);
    v259 = (v259 & 0x00FFFFFFu) | (((uint32)(r_u8(v269 + 1u * (2))) & 0xFFu) << 24);
    w_u16(v269, v230);
    w_u8(v269 + 1u * (2), (uint32)(v230 << 8) >> 24);
    w_u8(v230 + 200, (((uint32)(v259) >> 8) & 0xFFu));
    w_u8(v230 + 201, (((uint32)(v259) >> 16) & 0xFFu));
    w_u8(v230 + 202, (((uint32)(v259) >> 24) & 0xFFu));
          }
          v195 = r_u32(a0 + 180);
          v196 = r_u32(a0 + 936);
          v197 = r_u32(a0 + 948);
          v199 = r_u32(a0 + 940);
          v198 = r_u16(a0 + 40);
          v201 = r_u32(a0 + 944);
          v200 = r_u32(a0 + 976);
          carrier_t6 = r_u32(a0 + 960);
        }
      }
      carrier_s4 += 12;
    }
    while ( v193 );
    v4 = r_u32(a0 + 932);
    w_u32(a0 + 952, v195);
    w_u32(a0 + 956, carrier_s4);
  }
  v345 = r_u16(v4 + 60);
  carrier_t4 = r_u32(a0 + 956);
    w_u32(a0 + 932, v4);
  if ( v345 )
  {
    v347 = (sint32)r_u32(carrier_t4);
    v348 = r_u32(a0 + 952);
    v349 = r_u32(a0 + 936);
    v350 = r_u32(a0 + 948);
    v351 = r_u16(a0 + 40);
    v352 = r_u32(a0 + 940);
    v353 = r_u32(a0 + 944);
    carrier_t9 = v349 + 8 * ((sint32)r_u32(carrier_t4) & 0x1FF);
    carrier_t8 = v349 + (((sint32)r_u32(carrier_t4) >> 6) & 0xFF8);
    carrier_s1 = v349 + (((sint32)r_u32(carrier_t4) >> 15) & 0xFF8);
    do
    {
gt_draft_gte_write(0, 0, r_u32(carrier_t8 + (0)));
gt_draft_gte_write(1, 0, r_u32(carrier_t8 + (4)));
gt_draft_gte_write(2, 0, r_u32(carrier_t9 + (0)));
gt_draft_gte_write(3, 0, r_u32(carrier_t9 + (4)));
gt_draft_gte_write(4, 0, r_u32(carrier_s1 + (0)));
gt_draft_gte_write(5, 0, r_u32(carrier_s1 + (4)));
      --v345;
      v357 = v347;
gt_draft_gte_command(0x280030);
      v347 = (sint32)r_u32(carrier_t4 + 4u * (5));
      v358 = (sint32)r_u32(carrier_t4 + 4u * (1));
      carrier_t9 = v349 + 8 * (v347 & 0x1FF);
      carrier_t8 = v349 + ((v347 >> 6) & 0xFF8);
      carrier_s1 = v349 + ((v347 >> 15) & 0xFF8);
      v359 = (uint32)(v353 + ((v358 & 0x7FFE00) >> 4));
carrier_a0 = gt_draft_gte_read(17, 0);
carrier_a1 = gt_draft_gte_read(18, 0);
carrier_t1 = gt_draft_gte_read(19, 0);
      v363 = v358 & 0x8000000;
gt_draft_gte_command(0x1400006);
      if ( carrier_a0 < carrier_a1 )
        carrier_a0 = carrier_a1;
      if ( carrier_a0 < carrier_t1 )
        carrier_a0 = carrier_t1;
      v364 = carrier_a0 >> v351;
carrier_v1 = gt_draft_gte_read(24, 0);
      if ( v364 >= 4096 )
        v364 = 4095;
      if ( (carrier_v1 & v357 & 0x80000000) == 0 )
      {
        carrier_a1 = 0;
        if ( !v363 )
          carrier_a1 = r_u16(2 * v364 + v352);
        carrier_v0 = (sint32)r_u32(carrier_t4 + 4u * (2));
gt_draft_gte_write(21, 0, r_u32(carrier_t4 + (0xC)));
gt_draft_gte_write(22, 0, r_u32(carrier_t4 + (0x10)));
gt_draft_gte_write(6, 0, carrier_v0);
gt_draft_gte_write(20, 0, carrier_v0);
gt_draft_gte_write(8, 0, carrier_a1);
        v368 = (uint32)(v350 + 4 * (((v357 >> 23) & 0x30) + v364));
gt_draft_gte_command(0xF8002A);
        if ( carrier_v1 < 0 )
          carrier_v1 = (0u - carrier_v1);
        v369 = (uint32)(v359 + 16);
        if ( r_u16(v359) < (uint32)carrier_v1 )
          v369 = (uint32)(v359 + 4);
        v370 = (sint32)r_u32(v369);
        v371 = (sint32)r_u32(v369 + 4u * (1));
        v372 = (sint32)r_u32(v369 + 4u * (2));
    w_u32(v348, 150994944);
    w_u32(v348 + 12, v370);
    w_u32(v348 + 24, v371);
    w_u32(v348 + 36, v372);
    v371 = (v371 & 0xFFFF00FFu) | (((uint32)(r_u8(v368)) & 0xFFu) << 8);
    v371 = (v371 & 0xFF00FFFFu) | (((uint32)(r_u8(v368 + 1u * (1))) & 0xFFu) << 16);
    v371 = (v371 & 0x00FFFFFFu) | (((uint32)(r_u8(v368 + 1u * (2))) & 0xFFu) << 24);
    w_u16(v368, v348);
    w_u8(v368 + 1u * (2), (uint32)(v348 << 8) >> 24);
    w_u8(v348, (((uint32)(v371) >> 8) & 0xFFu));
    w_u8(v348 + 1, (((uint32)(v371) >> 16) & 0xFFu));
    w_u8(v348 + 2, (((uint32)(v371) >> 24) & 0xFFu));
w_u32(v348 + (4), gt_draft_gte_read(20, 0));
w_u32(v348 + (0x10), gt_draft_gte_read(21, 0));
w_u32(v348 + (0x1C), gt_draft_gte_read(22, 0));
w_u32(v348 + (0x14), gt_draft_gte_read(12, 0));
w_u32(v348 + (8), gt_draft_gte_read(13, 0));
w_u32(v348 + (0x20), gt_draft_gte_read(14, 0));
        v348 += 40;
      }
      carrier_t4 += 20;
    }
    while ( v345 );
    v4 = r_u32(a0 + 932);
    w_u32(a0 + 952, v348);
    w_u32(a0 + 956, carrier_t4);
  }
  v373 = r_u16(v4 + 62);
  carrier_t3 = r_u32(a0 + 956);
    w_u32(a0 + 932, v4);
  if ( v373 )
  {
    v375 = (sint32)r_u32(carrier_t3);
    v376 = r_u32(a0 + 952);
    v377 = r_u32(a0 + 936);
    v378 = r_u32(a0 + 948);
    v379 = r_u16(a0 + 40);
    v380 = r_u32(a0 + 940);
    v381 = r_u32(a0 + 944);
    carrier_t9 = v377 + 8 * ((sint32)r_u32(carrier_t3) & 0x1FF);
    carrier_t8 = v377 + (((sint32)r_u32(carrier_t3) >> 6) & 0xFF8);
    carrier_s1 = v377 + (((sint32)r_u32(carrier_t3) >> 15) & 0xFF8);
    do
    {
gt_draft_gte_write(0, 0, r_u32(carrier_t8 + (0)));
gt_draft_gte_write(1, 0, r_u32(carrier_t8 + (4)));
gt_draft_gte_write(2, 0, r_u32(carrier_s1 + (0)));
gt_draft_gte_write(3, 0, r_u32(carrier_s1 + (4)));
gt_draft_gte_write(4, 0, r_u32(carrier_t9 + (0)));
gt_draft_gte_write(5, 0, r_u32(carrier_t9 + (4)));
      --v373;
      v385 = v375;
gt_draft_gte_command(0x280030);
      v386 = (sint32)r_u32(carrier_t3 + 4u * (1));
      v375 = (sint32)r_u32(carrier_t3 + 4u * (6));
      v387 = (uint32)(v377 + 8 * (v386 & 0x1FF));
      carrier_v1 = (sint32)r_u32(v387);
      carrier_a0 = (sint32)r_u32(v387 + 4u * (1));
gt_draft_gte_command(0x1400006);
      carrier_t9 = v377 + 8 * (v375 & 0x1FF);
      carrier_t8 = v377 + ((v375 >> 6) & 0xFF8);
gt_draft_gte_write(0, 0, carrier_v1);
gt_draft_gte_write(1, 0, carrier_a0);
carrier_t1 = gt_draft_gte_read(24, 0);
w_u32(v376 + (0x14), gt_draft_gte_read(12, 0));
carrier_v1 = gt_draft_gte_read(17, 0);
carrier_a0 = gt_draft_gte_read(18, 0);
carrier_a1 = gt_draft_gte_read(19, 0);
      v394 = (0u - carrier_t1);
gt_draft_gte_command(0x180001);
      if ( carrier_v1 < carrier_a0 )
        carrier_v1 = carrier_a0;
      if ( carrier_v1 < carrier_a1 )
        carrier_v1 = carrier_a1;
      carrier_s1 = v377 + ((v375 >> 15) & 0xFF8);
gt_draft_gte_write(6, 0, r_u32(carrier_t3 + (8)));
carrier_a0 = gt_draft_gte_read(19, 0);
gt_draft_gte_command(0x1400006);
      v50 = carrier_v1 >= carrier_a0;
      v396 = carrier_v1 >> v379;
      if ( !v50 )
        v396 = carrier_a0 >> v379;
      if ( v396 >= 4096 )
        v396 = 4095;
      carrier_v0 = 0;
      if ( (v386 & 0x8000000) == 0 )
        carrier_v0 = r_u16(2 * v396 + v380);
carrier_t0 = gt_draft_gte_read(24, 0);
gt_draft_gte_write(8, 0, carrier_v0);
gt_draft_gte_command(0x780010);
      if ( (v385 & v394 & carrier_t0 & 0x80000000) == 0 )
      {
        v399 = (uint32)(v378 + 4 * (((v385 >> 23) & 0x30) + v396));
        carrier_v0 = (sint32)r_u32(carrier_t3 + 4u * (3));
        carrier_v1 = (sint32)r_u32(carrier_t3 + 4u * (4));
        carrier_a0 = (sint32)r_u32(carrier_t3 + 4u * (5));
w_u32(v376 + (4), gt_draft_gte_read(22, 0));
gt_draft_gte_write(20, 0, carrier_v0);
gt_draft_gte_write(21, 0, carrier_v1);
gt_draft_gte_write(22, 0, carrier_a0);
gt_draft_gte_command(0xF8002A);
        if ( v394 < 0 )
          v394 = -v394;
        v403 = (uint32)(v381 + 32 * ((v386 >> 9) & 0x3FFF));
        if ( carrier_t0 < 0 )
          carrier_t0 = (0u - carrier_t0);
        v404 = (uint32)(v403 + 16);
        if ( r_u16(v403) < (uint32)(v394 + carrier_t0) )
          v404 = (uint32)(v403 + 4);
        v405 = (sint32)r_u32(v404);
        v406 = (sint32)r_u32(v404 + 4u * (1));
        v407 = (sint32)r_u32(v404 + 4u * (2));
    w_u32(v376, 201326592);
    w_u32(v376 + 12, v405);
    w_u32(v376 + 24, v406);
    w_u32(v376 + 48, v407);
    w_u8(v376 + 36, (((uint32)(v407) >> 16) & 0xFFu));
    w_u8(v376 + 37, (((uint32)(v407) >> 24) & 0xFFu));
w_u32(v376 + (0x2C), gt_draft_gte_read(12, 0));
w_u32(v376 + (8), gt_draft_gte_read(13, 0));
w_u32(v376 + (0x20), gt_draft_gte_read(14, 0));
    v407 = (v407 & 0xFFFF00FFu) | (((uint32)(r_u8(v399)) & 0xFFu) << 8);
    v407 = (v407 & 0xFF00FFFFu) | (((uint32)(r_u8(v399 + 1u * (1))) & 0xFFu) << 16);
    v407 = (v407 & 0x00FFFFFFu) | (((uint32)(r_u8(v399 + 1u * (2))) & 0xFFu) << 24);
    w_u16(v399, v376);
    w_u8(v399 + 1u * (2), (uint32)(v376 << 8) >> 24);
    w_u8(v376, (((uint32)(v407) >> 8) & 0xFFu));
    w_u8(v376 + 1, (((uint32)(v407) >> 16) & 0xFFu));
    w_u8(v376 + 2, (((uint32)(v407) >> 24) & 0xFFu));
w_u32(v376 + (0x10), gt_draft_gte_read(20, 0));
w_u32(v376 + (0x28), gt_draft_gte_read(21, 0));
w_u32(v376 + (0x1C), gt_draft_gte_read(22, 0));
        v376 += 52;
      }
      carrier_t3 += 24;
    }
    while ( v373 );
    v4 = r_u32(a0 + 932);
    w_u32(a0 + 952, v376);
    w_u32(a0 + 956, carrier_t3);
  }
  v408 = gt_draft_unresolved_call(0x80026594u, a0, r_u32(a0 + 956), r_u16(v4 + 64), 0, 0, 0, 0, 0);
  return gt_draft_unresolved_call(0x800267E4u, a0, v408, r_u16(v4 + 66), 0, 0, 0, 0, 0);
}
