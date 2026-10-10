#include "game_first2_registry.h"
#include "game_draft_registry.h"
#include "game_draft_agent3_support.h"

static sint32 polygon_gte_flags(void)
{
    PsxGteSnapshot state;
    psx_gte_snapshot(&state);
    return state.flag;
}

/* Unverified draft from complete exported pseudocode with guest address operations */
/* Continued from the existing first and second renderer group fragments */
/* GTE operations use the live shared services in original order */
/* TODO Provisional raw argument types and nonstandard callee result carriers need integration review */
uint32 gt_draft_8001A758(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 a4, uint32 a5, uint32 a6, uint32 a7)
{
  sint32 gte_a0;
  sint32 gte_a1;
  sint32 gte_a3;
  sint32 gte_fp;
  sint32 gte_s4;
  sint32 gte_s5;
  sint32 gte_s6;
  sint32 gte_t0;
  sint32 gte_t1;
  sint32 gte_t2;
  sint32 gte_v0;
  sint32 gte_v1;
  sint32 result;
  sint32 v5;
  uint32 v6;
  sint32 v7;
  uint32 v8;
  sint32 v9;
  sint32 v13;
  uint32 v14;
  sint8 v15;
  uint32 v16;
  sint32 v17;
  sint32 v20;
  sint32 v21;
  sint32 v25;
  sint32 v26;
  sint32 v27;
  sint32 v28;
  sint32 v29;
  sint32 v30;
  sint32 v31;
  sint32 v32;
  sint32 v33;
  sint32 v34;
  sint32 v35;
  sint32 v36;
  sint32 v37;
  sint32 v38;
  uint32 v42;
  sint8 v43;
  sint8 v44;
  sint8 v45;
  sint32 v46;
  sint32 v47;
  uint32 v48;
  sint32 v49;
  uint32 v53;
  sint32 v54;
  uint32 v55;
  sint8 v56;
  sint32 v57;
  uint32 v58;
  sint32 v67;
  sint32 v68;
  sint32 v70;
  sint32 v71;
  sint32 v72;
  sint32 v73;
  sint32 v74;
  sint32 v75;
  sint32 v76;
  sint32 v77;
  sint32 v78;
  sint32 v79;
  sint32 v80;
  sint32 v81;
  sint32 v82;
  sint32 v83;
  sint32 v84;
  sint32 v85;
  sint32 v86;
  sint32 v87;
  sint32 v88;
  uint32 v92;
  sint8 v93;
  sint8 v94;
  sint8 v95;
  sint32 v96;
  uint32 v97;
  sint32 v98;
  sint32 v102;
  uint32 v103;
  sint8 v104;
  uint32 v105;
  sint32 v106;
  sint32 v109;
  sint32 v110;
  sint32 v114;
  sint32 v115;
  sint32 v116;
  sint32 v117;
  sint32 v118;
  sint32 v119;
  sint32 v120;
  sint32 v121;
  sint32 v122;
  sint32 v123;
  sint32 v124;
  sint32 v125;
  sint32 v126;
  sint32 v127;
  uint32 v131;
  sint8 v132;
  sint8 v133;
  sint8 v134;
  sint32 v135;
  sint32 v136;
  uint32 v137;
  sint32 v138;
  uint32 v142;
  sint32 v143;
  uint32 v144;
  sint8 v145;
  sint32 v146;
  uint32 v147;
  sint32 v156;
  sint32 v157;
  sint32 v159;
  sint32 v160;
  sint32 v161;
  sint32 v162;
  sint32 v163;
  sint32 v164;
  sint32 v165;
  sint32 v166;
  sint32 v167;
  sint32 v168;
  sint32 v169;
  sint32 v170;
  sint32 v171;
  sint32 v172;
  sint32 v173;
  sint32 v174;
  sint32 v175;
  sint32 v176;
  sint32 v177;
  uint32 v181;
  sint8 v182;
  sint8 v183;
  sint8 v184;
  sint32 v185;
  uint32 v186;
  sint32 v187;
  sint32 v191;
  uint32 v192;
  sint8 v193;
  uint32 v194;
  sint32 v195;
  sint32 v198;
  sint32 v199;
  sint32 v203;
  sint32 v204;
  sint32 v205;
  sint32 v206;
  sint32 v207;
  sint32 v208;
  sint32 v209;
  sint32 v210;
  sint32 v211;
  sint32 v212;
  sint32 v213;
  sint32 v214;
  sint32 v215;
  uint32 v216;
  uint32 v217;
  uint32 v218;
  uint32 v222;
  sint8 v223;
  sint8 v224;
  sint8 v225;
  sint32 v226;
  sint32 v227;
  uint32 v228;
  sint32 v229;
  uint32 v233;
  sint32 v234;
  uint32 v235;
  sint8 v236;
  sint32 v237;
  uint32 v238;
  sint32 v247;
  sint32 v248;
  sint32 v250;
  sint32 v251;
  sint32 v252;
  sint32 v253;
  sint32 v254;
  sint32 v255;
  sint32 v256;
  sint32 v257;
  sint32 v258;
  sint32 v259;
  sint32 v260;
  sint32 v261;
  sint32 v262;
  sint32 v263;
  sint32 v264;
  sint32 v265;
  sint32 v266;
  uint32 v267;
  uint32 v268;
  uint32 v269;
  uint32 v270;
  sint32 v271;
  uint32 v275;
  sint8 v276;
  sint8 v277;
  sint8 v278;
  sint32 v279;
  uint32 v280;
  sint32 v281;
  sint32 v285;
  uint32 v286;
  sint8 v287;
  uint32 v288;
  sint32 v289;
  sint32 v292;
  sint32 v293;
  sint32 v297;
  sint32 v298;
  sint32 v299;
  sint32 v300;
  sint32 v301;
  sint32 v302;
  sint32 v303;
  sint32 v304;
  sint32 v305;
  sint32 v306;
  sint32 v307;
  sint32 v308;
  sint32 v309;
  uint32 v310;
  uint32 v311;
  uint32 v312;
  uint32 v316;
  sint8 v317;
  sint8 v318;
  sint8 v319;
  sint32 v320;
  uint32 v321;
  sint32 v322;
  uint32 v326;
  sint32 v327;
  uint32 v328;
  sint8 v329;
  sint32 v330;
  uint32 v331;
  sint32 v340;
  sint32 v341;
  sint32 v343;
  sint32 v344;
  sint32 v345;
  sint32 v346;
  sint32 v347;
  sint32 v348;
  sint32 v349;
  sint32 v350;
  sint32 v351;
  sint32 v352;
  sint32 v353;
  sint32 v354;
  sint32 v355;
  sint32 v356;
  sint32 v357;
  sint32 v358;
  sint32 v359;
  uint32 v360;
  uint32 v361;
  uint32 v362;
  uint32 v363;
  sint32 v364;
  uint32 v368;
  sint8 v369;
  sint8 v370;
  sint8 v371;
  sint32 v372;
  result = (sint8)r_u8((uint32)(0x8008FBE0u));
  if (!(sint8)r_u8((uint32)(0x8008FBE0u)))
  {
    v372 = r_u32((uint32)(0x8008F220u));
    gtmain_model_translation(a0, 0);
    gtmain_model_matrix_bind(a0, psx_addr(a0 + 64u, 20u));
    v5 = r_u32((uint32)((uint32)((sint32)((uint32)(v372) + (uint32)(24)))));
    w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(780)))), (uint32)(r_u32((uint32)(0x800BFE4C))));
    w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(776)))), (uint32)(v5));
    v6 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)((uint32)((sint32)((uint32)(4) * (uint32)((sint32)a1))) + (uint32)(v372))) + (uint32)(28)))));
    v7 = r_u16((uint32)((uint32)((sint32)((uint32)(v372) + (uint32)(40)))));
    v8 = v6;
    if (r_u16((uint32)((uint32)((sint32)((uint32)(v372) + (uint32)(40))))))
    {
      v9 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(776)))));
      gte_s6 = ((uint32)(v9) + (uint32)(((uint32)(8) * (uint32)((r_u32((uint32)(v6)) & 0xFFF)))));
      gte_s5 = ((uint32)(v9) + (uint32)((((uint32)(r_u32((uint32)(v6))) >> ((uint32)(9) & 31u)) & 0x7FF8)));
      gte_s4 = ((uint32)(v9) + (uint32)(((uint32)(8) * (uint32)((r_u32((uint32)((v6 + 4u * (uint32)(1)))) & 0xFFF)))));
      v13 = (((uint32)(r_u32((uint32)(v6))) >> 24u) & 0xFFu);
      if (r_u16((uint32)((uint32)((sint32)((uint32)(v372) + (uint32)(40))))))
      {
        do
        {
          v14 = (v8 + 4u * (uint32)(2));
          xport_gte_write_data(0u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s6) + (uint32)(0))))));
          xport_gte_write_data(1u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s6) + (uint32)(4))))));
          xport_gte_write_data(2u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s5) + (uint32)(0))))));
          xport_gte_write_data(3u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s5) + (uint32)(4))))));
          xport_gte_write_data(4u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s4) + (uint32)(0))))));
          xport_gte_write_data(5u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s4) + (uint32)(4))))));
          v15 = v13;
          (v7 -= 1u);
          xport_gte_execute(0x280030);
          v16 = r_u32((uint32)((v8 + 4u * (uint32)(7))));
          v17 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(776)))));
          gte_s6 = ((uint32)(v17) + (uint32)(((uint32)(8) * (uint32)((v16 & 0xFFF)))));
          gte_s5 = ((uint32)(v17) + (uint32)((((uint32)(v16) >> ((uint32)(9) & 31u)) & 0x7FF8)));
          gte_s4 = ((uint32)(v17) + (uint32)(((uint32)(8) * (uint32)((r_u32((uint32)((v8 + 4u * (uint32)(8)))) & 0xFFF)))));
          v13 = (((uint32)(v16) >> 24u) & 0xFFu);
          gte_a1 = (sint32)gte_read_h();
          gte_t0 = polygon_gte_flags();
          v20 = ((sint32)(gte_a1) >> ((uint32)(1) & 31u));
          v21 = ((uint32)gte_a1 < 0x400u);
          if ((gte_t0 >= 0))
            goto LABEL_28;
          if (!v21)
          {
            gte_v0 = xport_gte_read_data(17u);
            gte_v1 = xport_gte_read_data(18u);
            gte_a0 = xport_gte_read_data(19u);
            if ((((gte_v0 | gte_v1) | gte_a0) >= v20))
            {
              LABEL_28:
              if (v21)
                goto LABEL_32;

              w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x14C)))), (uint32)(xport_gte_read_data(12u)));
              w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x16C)))), (uint32)(xport_gte_read_data(13u)));
              w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x18C)))), (uint32)(xport_gte_read_data(14u)));
              v25 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(332)))));
              v26 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(364)))));
              v27 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(396)))));
              v28 = v25;
              v29 = (v25 < v26);
              if ((v26 < v25))
                v25 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(364)))));
              if (v29)
                v28 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(364)))));
              if ((v27 < v25))
                v25 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(396)))));
              if ((v28 < v27))
                v28 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(396)))));
              if (((v28 <= 0) || (v25 >= (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(4))))))))
                goto LABEL_36;
              v30 = (gte_t0 | (sint32)((uint32)(1023) - (uint32)((sint32)((uint32)(v28) - (uint32)(v25)))));
              v31 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(334)))));
              v32 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(366)))));
              v33 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(398)))));
              v34 = v31;
              v35 = (v31 < v32);
              if ((v32 < v31))
                v31 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(366)))));
              if (v35)
                v34 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(366)))));
              v36 = (v34 < v33);
              if ((v33 < v31))
                v31 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(398)))));
              if ((v34 < v33))
                v34 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(398)))));
              if (((v34 <= 0) || (v31 >= (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(6))))))))
                goto LABEL_36;
              if (((v30 | (sint32)((uint32)(511) - (uint32)((sint32)((uint32)(v34) - (uint32)(v31))))) >= 0))
              {
                LABEL_32:
                gte_a1 = xport_gte_read_data(12u);

                gte_a3 = xport_gte_read_data(13u);
                gte_t0 = xport_gte_read_data(14u);
                if (((((gte_a1 & gte_a3) & gte_t0) & 0x80008000) == 0))
                {
                  v42 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(772)))));
                  w_u32((uint32)((v8 + 4u * (uint32)(4))), (uint32)(gte_a1));
                  if (((v15 & 1) == 0))
                    v42 += 1u * (uint32)(4);
                  w_u32((uint32)((v8 + 4u * (uint32)(5))), (uint32)(gte_a3));
                  w_u32((uint32)((v8 + 4u * (uint32)(6))), (uint32)(gte_t0));
                  v43 = (sint8)r_u8((uint32)(v42));
                  v44 = (sint8)r_u8((uint32)((v42 + 1u * (uint32)(1))));
                  v45 = (sint8)r_u8((uint32)((v42 + 1u * (uint32)(2))));
                  w_u16((uint32)((uint32)(v42)), (uint32)((uint16)(v14)));
                  w_u8((uint32)((v42 + 1u * (uint32)(2))), (uint32)(((uint32)(((uint32)((uint32)(v14)) << ((uint32)(8) & 31u))) >> ((uint32)(24) & 31u))));
                  w_u8((uint32)(v14), (uint32)(v43));
                  w_u8((uint32)(((uint32)(v8) + 1u * (uint32)(9))), (uint32)(v44));
                  w_u8((uint32)(((uint32)(v8) + 1u * (uint32)(10))), (uint32)(v45));
                }
              }
              else
              {
                v37 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(772)))));
                if (((v15 & 1) == 0))
                  v37 = ((uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(772)))))) + (uint32)(4));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(168)))), (uint32)(v37));
                v38 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(780)))));
                w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(184)))), (uint32)(3));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(172)))), (uint32)(v38));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(156)))), (uint32)(r_u32((uint32)((v8 + 4u * (uint32)(3))))));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x144)))), (uint32)(xport_gte_read_data(0u)));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x148)))), (uint32)(xport_gte_read_data(1u)));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x164)))), (uint32)(xport_gte_read_data(2u)));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x168)))), (uint32)(xport_gte_read_data(3u)));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x184)))), (uint32)(xport_gte_read_data(4u)));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x188)))), (uint32)(xport_gte_read_data(5u)));
                gt_draft_unresolved_call(0x80068CB0u, (sint32)a0, v36, 0u, 0u, 0u, 0u, 0u, 0u);
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(780)))), (uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(180)))))));
              }
            }
          }
          LABEL_36:
          v8 += 4u * (uint32)(7);

        }
        while (v7);
      }
      v6 = v8;
    }
    v46 = v372;
    v47 = r_u16((uint32)((uint32)((sint32)((uint32)(v372) + (uint32)(42)))));
    v48 = v6;
    if (r_u16((uint32)((uint32)((sint32)((uint32)(v372) + (uint32)(42))))))
    {
      v49 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(776)))));
      gte_fp = ((uint32)(v49) + (uint32)(((uint32)(8) * (uint32)((r_u32((uint32)(v6)) & 0xFFF)))));
      gte_s5 = ((uint32)(v49) + (uint32)((((uint32)(r_u32((uint32)(v6))) >> ((uint32)(9) & 31u)) & 0x7FF8)));
      gte_s4 = ((uint32)(v49) + (uint32)(((uint32)(8) * (uint32)((r_u32((uint32)((v6 + 4u * (uint32)(1)))) & 0xFFF)))));
      v53 = (uint32)(((uint32)(v49) + (uint32)((((uint32)(r_u32((uint32)((v6 + 4u * (uint32)(1))))) >> ((uint32)(9) & 31u)) & 0x7FF8))));
      v54 = (((uint32)(r_u32((uint32)(v6))) >> 24u) & 0xFFu);
      if (r_u16((uint32)((uint32)((sint32)((uint32)(v372) + (uint32)(42))))))
      {
        do
        {
          v55 = (v48 + 4u * (uint32)(2));
          xport_gte_write_data(0u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_fp) + (uint32)(0))))));
          xport_gte_write_data(1u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_fp) + (uint32)(4))))));
          xport_gte_write_data(2u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s5) + (uint32)(0))))));
          xport_gte_write_data(3u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s5) + (uint32)(4))))));
          w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x144)))), (uint32)(xport_gte_read_data(0u)));
          w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x148)))), (uint32)(xport_gte_read_data(1u)));
          xport_gte_write_data(4u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s4) + (uint32)(0))))));
          xport_gte_write_data(5u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s4) + (uint32)(4))))));
          v56 = v54;
          v57 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(776)))));
          xport_gte_execute(0x280030);
          v58 = r_u32((uint32)((v48 + 4u * (uint32)(8))));
          gte_v1 = (sint32)r_u32((uint32)(v53));
          gte_a0 = (sint32)r_u32((uint32)((v53 + 4u * (uint32)(1))));
          gte_fp = ((uint32)(v57) + (uint32)(((uint32)(8) * (uint32)((v58 & 0xFFF)))));
          gte_a1 = polygon_gte_flags();
          gte_v0 = xport_gte_read_data(17u);
          gte_v0 = xport_gte_read_data(17u);
          gte_v0 = xport_gte_read_data(18u);
          w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x14C)))), (uint32)(xport_gte_read_data(12u)));
          xport_gte_write_data(0u, gte_v1);
          xport_gte_write_data(1u, gte_a0);
          v54 = (((uint32)(v58) >> 24u) & 0xFFu);
          (v47 -= 1u);
          xport_gte_execute(0x180001);
          gte_s5 = ((uint32)(v57) + (uint32)((((uint32)(v58) >> ((uint32)(9) & 31u)) & 0x7FF8)));
          gte_s4 = ((uint32)(v57) + (uint32)(((uint32)(8) * (uint32)((r_u32((uint32)((v48 + 4u * (uint32)(9)))) & 0xFFF)))));
          v53 = (uint32)(((uint32)(v57) + (uint32)((((uint32)(r_u32((uint32)((v48 + 4u * (uint32)(9))))) >> ((uint32)(9) & 31u)) & 0x7FF8))));
          gte_v0 = polygon_gte_flags();
          gte_v1 = (sint32)gte_read_h();
          v67 = (gte_a1 | gte_v0);
          v68 = ((uint32)gte_v1 < 0x400u);
          if ((v67 < 0))
          {
            if (v68)
              goto LABEL_80;
            gte_v0 = xport_gte_read_data(19u);
          }
          if (v68)
            goto LABEL_76;
          w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x16C)))), (uint32)(xport_gte_read_data(12u)));
          w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x18C)))), (uint32)(xport_gte_read_data(13u)));
          w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x1AC)))), (uint32)(xport_gte_read_data(14u)));
          v70 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(332)))));
          v71 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(364)))));
          v72 = v70;
          v73 = (v70 < v71);
          if ((v71 < v70))
            v70 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(364)))));
          v74 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(396)))));
          if (v73)
            v72 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(364)))));
          if ((v74 < v70))
            v70 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(396)))));
          v75 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(428)))));
          if ((v72 < v74))
            v72 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(396)))));
          if ((v75 < v70))
            v70 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(428)))));
          if ((v72 < v75))
            v72 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(428)))));
          v76 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(4)))));
          if (((v72 <= 0) || (v70 >= v76)))
            goto LABEL_80;
          v77 = ((v67 | v70) | (sint32)((uint32)(v76) - (uint32)(v72)));
          v78 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(334)))));
          v79 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(366)))));
          v80 = v78;
          v81 = (v78 < v79);
          if ((v79 < v78))
            v78 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(366)))));
          v82 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(398)))));
          if (v81)
            v80 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(366)))));
          if ((v82 < v78))
            v78 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(398)))));
          v83 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(430)))));
          if ((v80 < v82))
            v80 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(398)))));
          if ((v83 < v78))
            v78 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(430)))));
          if ((v80 < v83))
            v80 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(430)))));
          v84 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(6)))));
          if (((v80 <= 0) || (v78 >= v84)))
            goto LABEL_80;
          v85 = ((v77 | v78) | (sint32)((uint32)(v84) - (uint32)(v80)));
          if ((v85 >= 0))
          {
            LABEL_76:
            v88 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(332)))));

            gte_a3 = xport_gte_read_data(12u);
            gte_t0 = xport_gte_read_data(13u);
            gte_t1 = xport_gte_read_data(14u);
            if ((((((v88 & gte_a3) & gte_t0) & gte_t1) & 0x80008000) == 0))
            {
              v92 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(772)))));
              w_u32((uint32)((v48 + 4u * (uint32)(4))), (uint32)(v88));
              if (((v56 & 1) == 0))
                v92 += 1u * (uint32)(4);
              w_u32((uint32)((v48 + 4u * (uint32)(5))), (uint32)(gte_a3));
              w_u32((uint32)((v48 + 4u * (uint32)(6))), (uint32)(gte_t0));
              w_u32((uint32)((v48 + 4u * (uint32)(7))), (uint32)(gte_t1));
              v93 = (sint8)r_u8((uint32)(v92));
              v94 = (sint8)r_u8((uint32)((v92 + 1u * (uint32)(1))));
              v95 = (sint8)r_u8((uint32)((v92 + 1u * (uint32)(2))));
              w_u16((uint32)((uint32)(v92)), (uint32)((uint16)(v55)));
              w_u8((uint32)((v92 + 1u * (uint32)(2))), (uint32)(((uint32)(((uint32)((uint32)(v55)) << ((uint32)(8) & 31u))) >> ((uint32)(24) & 31u))));
              w_u8((uint32)(v55), (uint32)(v93));
              w_u8((uint32)(((uint32)(v48) + 1u * (uint32)(9))), (uint32)(v94));
              w_u8((uint32)(((uint32)(v48) + 1u * (uint32)(10))), (uint32)(v95));
            }
          }
          else
          {
            v86 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(772)))));
            if (((v56 & 1) == 0))
              v86 = ((uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(772)))))) + (uint32)(4));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(168)))), (uint32)(v86));
            v87 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(780)))));
            w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(184)))), (uint32)(4));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(172)))), (uint32)(v87));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(156)))), (uint32)(r_u32((uint32)((v48 + 4u * (uint32)(3))))));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x184)))), (uint32)(xport_gte_read_data(0u)));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x188)))), (uint32)(xport_gte_read_data(1u)));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x164)))), (uint32)(xport_gte_read_data(2u)));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x168)))), (uint32)(xport_gte_read_data(3u)));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x1A4)))), (uint32)(xport_gte_read_data(4u)));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x1A8)))), (uint32)(xport_gte_read_data(5u)));
            gt_draft_unresolved_call(0x80068CB0u, (sint32)a0, v85, 0u, 0u, 0u, 0u, 0u, 0u);
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(780)))), (uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(180)))))));
          }
          LABEL_80:
          v48 += 4u * (uint32)(8);

        }
        while (v47);
      }
      v6 = v48;
      v46 = v372;
    }
    v96 = r_u16((uint32)((uint32)((sint32)((uint32)(v46) + (uint32)(44)))));
    v97 = v6;
    if (r_u16((uint32)((uint32)((sint32)((uint32)(v46) + (uint32)(44))))))
    {
      v98 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(776)))));
      gte_s6 = ((uint32)(v98) + (uint32)(((uint32)(8) * (uint32)((r_u32((uint32)(v6)) & 0xFFF)))));
      gte_s5 = ((uint32)(v98) + (uint32)((((uint32)(r_u32((uint32)(v6))) >> ((uint32)(9) & 31u)) & 0x7FF8)));
      gte_s4 = ((uint32)(v98) + (uint32)(((uint32)(8) * (uint32)((r_u32((uint32)((v6 + 4u * (uint32)(1)))) & 0xFFF)))));
      v102 = (((uint32)(r_u32((uint32)(v6))) >> 24u) & 0xFFu);
      if (r_u16((uint32)((uint32)((sint32)((uint32)(v46) + (uint32)(44))))))
      {
        do
        {
          v103 = (v97 + 4u * (uint32)(2));
          xport_gte_write_data(0u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s6) + (uint32)(0))))));
          xport_gte_write_data(1u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s6) + (uint32)(4))))));
          xport_gte_write_data(2u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s5) + (uint32)(0))))));
          xport_gte_write_data(3u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s5) + (uint32)(4))))));
          xport_gte_write_data(4u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s4) + (uint32)(0))))));
          xport_gte_write_data(5u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s4) + (uint32)(4))))));
          v104 = v102;
          (v96 -= 1u);
          xport_gte_execute(0x280030);
          v105 = r_u32((uint32)((v97 + 4u * (uint32)(9))));
          v106 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(776)))));
          gte_s6 = ((uint32)(v106) + (uint32)(((uint32)(8) * (uint32)((v105 & 0xFFF)))));
          gte_s5 = ((uint32)(v106) + (uint32)((((uint32)(v105) >> ((uint32)(9) & 31u)) & 0x7FF8)));
          gte_s4 = ((uint32)(v106) + (uint32)(((uint32)(8) * (uint32)((r_u32((uint32)((v97 + 4u * (uint32)(10)))) & 0xFFF)))));
          v102 = (((uint32)(v105) >> 24u) & 0xFFu);
          gte_a1 = (sint32)gte_read_h();
          gte_t0 = polygon_gte_flags();
          v109 = ((sint32)(gte_a1) >> ((uint32)(1) & 31u));
          v110 = ((uint32)gte_a1 < 0x400u);
          if ((gte_t0 >= 0))
            goto LABEL_108;
          if (!v110)
          {
            gte_v0 = xport_gte_read_data(17u);
            gte_v1 = xport_gte_read_data(18u);
            gte_a0 = xport_gte_read_data(19u);
            if ((((gte_v0 | gte_v1) | gte_a0) >= v109))
            {
              LABEL_108:
              if (v110)
                goto LABEL_112;

              w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x14C)))), (uint32)(xport_gte_read_data(12u)));
              w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x16C)))), (uint32)(xport_gte_read_data(13u)));
              w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x18C)))), (uint32)(xport_gte_read_data(14u)));
              v114 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(332)))));
              v115 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(364)))));
              v116 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(396)))));
              v117 = v114;
              v118 = (v114 < v115);
              if ((v115 < v114))
                v114 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(364)))));
              if (v118)
                v117 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(364)))));
              if ((v116 < v114))
                v114 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(396)))));
              if ((v117 < v116))
                v117 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(396)))));
              if (((v117 <= 0) || (v114 >= (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(4))))))))
                goto LABEL_116;
              v119 = (gte_t0 | (sint32)((uint32)(1023) - (uint32)((sint32)((uint32)(v117) - (uint32)(v114)))));
              v120 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(334)))));
              v121 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(366)))));
              v122 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(398)))));
              v123 = v120;
              v124 = (v120 < v121);
              if ((v121 < v120))
                v120 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(366)))));
              if (v124)
                v123 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(366)))));
              v125 = (v123 < v122);
              if ((v122 < v120))
                v120 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(398)))));
              if ((v123 < v122))
                v123 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(398)))));
              if (((v123 <= 0) || (v120 >= (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(6))))))))
                goto LABEL_116;
              if (((v119 | (sint32)((uint32)(511) - (uint32)((sint32)((uint32)(v123) - (uint32)(v120))))) >= 0))
              {
                LABEL_112:
                gte_a1 = xport_gte_read_data(12u);

                gte_a3 = xport_gte_read_data(13u);
                gte_t0 = xport_gte_read_data(14u);
                if (((((gte_a1 & gte_a3) & gte_t0) & 0x80008000) == 0))
                {
                  v131 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(772)))));
                  w_u32((uint32)((v97 + 4u * (uint32)(4))), (uint32)(gte_a1));
                  if (((v104 & 1) == 0))
                    v131 += 1u * (uint32)(4);
                  w_u32((uint32)((v97 + 4u * (uint32)(6))), (uint32)(gte_a3));
                  w_u32((uint32)((v97 + 4u * (uint32)(8))), (uint32)(gte_t0));
                  v132 = (sint8)r_u8((uint32)(v131));
                  v133 = (sint8)r_u8((uint32)((v131 + 1u * (uint32)(1))));
                  v134 = (sint8)r_u8((uint32)((v131 + 1u * (uint32)(2))));
                  w_u16((uint32)((uint32)(v131)), (uint32)((uint16)(v103)));
                  w_u8((uint32)((v131 + 1u * (uint32)(2))), (uint32)(((uint32)(((uint32)((uint32)(v103)) << ((uint32)(8) & 31u))) >> ((uint32)(24) & 31u))));
                  w_u8((uint32)(v103), (uint32)(v132));
                  w_u8((uint32)(((uint32)(v97) + 1u * (uint32)(9))), (uint32)(v133));
                  w_u8((uint32)(((uint32)(v97) + 1u * (uint32)(10))), (uint32)(v134));
                }
              }
              else
              {
                v126 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(772)))));
                if (((v104 & 1) == 0))
                  v126 = ((uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(772)))))) + (uint32)(4));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(168)))), (uint32)(v126));
                v127 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(780)))));
                w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(184)))), (uint32)(3));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(172)))), (uint32)(v127));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(156)))), (uint32)(r_u32((uint32)((v97 + 4u * (uint32)(3))))));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(340)))), (uint32)(r_u32((uint32)((v97 + 4u * (uint32)(3))))));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(372)))), (uint32)(r_u32((uint32)((v97 + 4u * (uint32)(5))))));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(404)))), (uint32)(r_u32((uint32)((v97 + 4u * (uint32)(7))))));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x144)))), (uint32)(xport_gte_read_data(0u)));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x148)))), (uint32)(xport_gte_read_data(1u)));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x164)))), (uint32)(xport_gte_read_data(2u)));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x168)))), (uint32)(xport_gte_read_data(3u)));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x184)))), (uint32)(xport_gte_read_data(4u)));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x188)))), (uint32)(xport_gte_read_data(5u)));
                gt_draft_unresolved_call(0x80069254u, (sint32)a0, v125, 0u, 0u, 0u, 0u, 0u, 0u);
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(780)))), (uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(180)))))));
              }
            }
          }
          LABEL_116:
          v97 += 4u * (uint32)(9);

        }
        while (v96);
      }
      v6 = v97;
    }
    v135 = v372;
    v136 = r_u16((uint32)((uint32)((sint32)((uint32)(v372) + (uint32)(46)))));
    v137 = v6;
    if (r_u16((uint32)((uint32)((sint32)((uint32)(v372) + (uint32)(46))))))
    {
      v138 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(776)))));
      gte_fp = ((uint32)(v138) + (uint32)(((uint32)(8) * (uint32)((r_u32((uint32)(v6)) & 0xFFF)))));
      gte_s5 = ((uint32)(v138) + (uint32)((((uint32)(r_u32((uint32)(v6))) >> ((uint32)(9) & 31u)) & 0x7FF8)));
      gte_s4 = ((uint32)(v138) + (uint32)(((uint32)(8) * (uint32)((r_u32((uint32)((v6 + 4u * (uint32)(1)))) & 0xFFF)))));
      v142 = (uint32)(((uint32)(v138) + (uint32)((((uint32)(r_u32((uint32)((v6 + 4u * (uint32)(1))))) >> ((uint32)(9) & 31u)) & 0x7FF8))));
      v143 = (((uint32)(r_u32((uint32)(v6))) >> 24u) & 0xFFu);
      if (r_u16((uint32)((uint32)((sint32)((uint32)(v372) + (uint32)(46))))))
      {
        do
        {
          v144 = (v137 + 4u * (uint32)(2));
          xport_gte_write_data(0u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_fp) + (uint32)(0))))));
          xport_gte_write_data(1u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_fp) + (uint32)(4))))));
          xport_gte_write_data(2u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s5) + (uint32)(0))))));
          xport_gte_write_data(3u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s5) + (uint32)(4))))));
          w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x144)))), (uint32)(xport_gte_read_data(0u)));
          w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x148)))), (uint32)(xport_gte_read_data(1u)));
          xport_gte_write_data(4u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s4) + (uint32)(0))))));
          xport_gte_write_data(5u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s4) + (uint32)(4))))));
          v145 = v143;
          v146 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(776)))));
          xport_gte_execute(0x280030);
          v147 = r_u32((uint32)((v137 + 4u * (uint32)(11))));
          gte_v1 = (sint32)r_u32((uint32)(v142));
          gte_a0 = (sint32)r_u32((uint32)((v142 + 4u * (uint32)(1))));
          gte_fp = ((uint32)(v146) + (uint32)(((uint32)(8) * (uint32)((v147 & 0xFFF)))));
          gte_a1 = polygon_gte_flags();
          gte_v0 = xport_gte_read_data(17u);
          gte_v0 = xport_gte_read_data(17u);
          gte_v0 = xport_gte_read_data(18u);
          w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x14C)))), (uint32)(xport_gte_read_data(12u)));
          xport_gte_write_data(0u, gte_v1);
          xport_gte_write_data(1u, gte_a0);
          v143 = (((uint32)(v147) >> 24u) & 0xFFu);
          (v136 -= 1u);
          xport_gte_execute(0x180001);
          gte_s5 = ((uint32)(v146) + (uint32)((((uint32)(v147) >> ((uint32)(9) & 31u)) & 0x7FF8)));
          gte_s4 = ((uint32)(v146) + (uint32)(((uint32)(8) * (uint32)((r_u32((uint32)((v137 + 4u * (uint32)(12)))) & 0xFFF)))));
          v142 = (uint32)(((uint32)(v146) + (uint32)((((uint32)(r_u32((uint32)((v137 + 4u * (uint32)(12))))) >> ((uint32)(9) & 31u)) & 0x7FF8))));
          gte_v0 = polygon_gte_flags();
          gte_v1 = (sint32)gte_read_h();
          v156 = (gte_a1 | gte_v0);
          v157 = ((uint32)gte_v1 < 0x400u);
          if ((v156 < 0))
          {
            if (v157)
              goto LABEL_160;
            gte_v0 = xport_gte_read_data(19u);
          }
          if (v157)
            goto LABEL_156;
          w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x16C)))), (uint32)(xport_gte_read_data(12u)));
          w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x18C)))), (uint32)(xport_gte_read_data(13u)));
          w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x1AC)))), (uint32)(xport_gte_read_data(14u)));
          v159 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(332)))));
          v160 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(364)))));
          v161 = v159;
          v162 = (v159 < v160);
          if ((v160 < v159))
            v159 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(364)))));
          v163 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(396)))));
          if (v162)
            v161 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(364)))));
          if ((v163 < v159))
            v159 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(396)))));
          v164 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(428)))));
          if ((v161 < v163))
            v161 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(396)))));
          if ((v164 < v159))
            v159 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(428)))));
          if ((v161 < v164))
            v161 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(428)))));
          v165 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(4)))));
          if (((v161 <= 0) || (v159 >= v165)))
            goto LABEL_160;
          v166 = ((v156 | v159) | (sint32)((uint32)(v165) - (uint32)(v161)));
          v167 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(334)))));
          v168 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(366)))));
          v169 = v167;
          v170 = (v167 < v168);
          if ((v168 < v167))
            v167 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(366)))));
          v171 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(398)))));
          if (v170)
            v169 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(366)))));
          if ((v171 < v167))
            v167 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(398)))));
          v172 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(430)))));
          if ((v169 < v171))
            v169 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(398)))));
          if ((v172 < v167))
            v167 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(430)))));
          if ((v169 < v172))
            v169 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(430)))));
          v173 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(6)))));
          if (((v169 <= 0) || (v167 >= v173)))
            goto LABEL_160;
          v174 = ((v166 | v167) | (sint32)((uint32)(v173) - (uint32)(v169)));
          if ((v174 >= 0))
          {
            LABEL_156:
            v177 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(332)))));

            gte_a3 = xport_gte_read_data(12u);
            gte_t0 = xport_gte_read_data(13u);
            gte_t1 = xport_gte_read_data(14u);
            if ((((((v177 & gte_a3) & gte_t0) & gte_t1) & 0x80008000) == 0))
            {
              v181 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(772)))));
              w_u32((uint32)((v137 + 4u * (uint32)(4))), (uint32)(v177));
              if (((v145 & 1) == 0))
                v181 += 1u * (uint32)(4);
              w_u32((uint32)((v137 + 4u * (uint32)(6))), (uint32)(gte_a3));
              w_u32((uint32)((v137 + 4u * (uint32)(8))), (uint32)(gte_t0));
              w_u32((uint32)((v137 + 4u * (uint32)(10))), (uint32)(gte_t1));
              v182 = (sint8)r_u8((uint32)(v181));
              v183 = (sint8)r_u8((uint32)((v181 + 1u * (uint32)(1))));
              v184 = (sint8)r_u8((uint32)((v181 + 1u * (uint32)(2))));
              w_u16((uint32)((uint32)(v181)), (uint32)((uint16)(v144)));
              w_u8((uint32)((v181 + 1u * (uint32)(2))), (uint32)(((uint32)(((uint32)((uint32)(v144)) << ((uint32)(8) & 31u))) >> ((uint32)(24) & 31u))));
              w_u8((uint32)(v144), (uint32)(v182));
              w_u8((uint32)(((uint32)(v137) + 1u * (uint32)(9))), (uint32)(v183));
              w_u8((uint32)(((uint32)(v137) + 1u * (uint32)(10))), (uint32)(v184));
            }
          }
          else
          {
            v175 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(772)))));
            if (((v145 & 1) == 0))
              v175 = ((uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(772)))))) + (uint32)(4));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(168)))), (uint32)(v175));
            v176 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(780)))));
            w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(184)))), (uint32)(4));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(172)))), (uint32)(v176));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(156)))), (uint32)(r_u32((uint32)((v137 + 4u * (uint32)(3))))));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(340)))), (uint32)(r_u32((uint32)((v137 + 4u * (uint32)(3))))));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(372)))), (uint32)(r_u32((uint32)((v137 + 4u * (uint32)(5))))));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(436)))), (uint32)(r_u32((uint32)((v137 + 4u * (uint32)(7))))));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(404)))), (uint32)(r_u32((uint32)((v137 + 4u * (uint32)(9))))));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x184)))), (uint32)(xport_gte_read_data(0u)));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x188)))), (uint32)(xport_gte_read_data(1u)));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x164)))), (uint32)(xport_gte_read_data(2u)));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x168)))), (uint32)(xport_gte_read_data(3u)));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x1A4)))), (uint32)(xport_gte_read_data(4u)));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x1A8)))), (uint32)(xport_gte_read_data(5u)));
            gt_draft_unresolved_call(0x80069254u, (sint32)a0, v174, 0u, 0u, 0u, 0u, 0u, 0u);
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(780)))), (uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(180)))))));
          }
          LABEL_160:
          v137 += 4u * (uint32)(11);

        }
        while (v136);
      }
      v6 = v137;
      v135 = v372;
    }
    v185 = r_u16((uint32)((uint32)((sint32)((uint32)(v135) + (uint32)(48)))));
    v186 = v6;
    if (r_u16((uint32)((uint32)((sint32)((uint32)(v135) + (uint32)(48))))))
    {
      v187 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(776)))));
      gte_s6 = ((uint32)(v187) + (uint32)(((uint32)(8) * (uint32)((r_u32((uint32)(v6)) & 0xFFF)))));
      gte_s5 = ((uint32)(v187) + (uint32)((((uint32)(r_u32((uint32)(v6))) >> ((uint32)(9) & 31u)) & 0x7FF8)));
      gte_s4 = ((uint32)(v187) + (uint32)(((uint32)(8) * (uint32)((r_u32((uint32)((v6 + 4u * (uint32)(1)))) & 0xFFF)))));
      v191 = (((uint32)(r_u32((uint32)(v6))) >> 24u) & 0xFFu);
      if (r_u16((uint32)((uint32)((sint32)((uint32)(v135) + (uint32)(48))))))
      {
        do
        {
          v192 = (v186 + 4u * (uint32)(2));
          xport_gte_write_data(0u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s6) + (uint32)(0))))));
          xport_gte_write_data(1u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s6) + (uint32)(4))))));
          xport_gte_write_data(2u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s5) + (uint32)(0))))));
          xport_gte_write_data(3u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s5) + (uint32)(4))))));
          xport_gte_write_data(4u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s4) + (uint32)(0))))));
          xport_gte_write_data(5u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s4) + (uint32)(4))))));
          v193 = v191;
          (v185 -= 1u);
          xport_gte_execute(0x280030);
          v194 = r_u32((uint32)((v186 + 4u * (uint32)(10))));
          v195 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(776)))));
          gte_s6 = ((uint32)(v195) + (uint32)(((uint32)(8) * (uint32)((v194 & 0xFFF)))));
          gte_s5 = ((uint32)(v195) + (uint32)((((uint32)(v194) >> ((uint32)(9) & 31u)) & 0x7FF8)));
          gte_s4 = ((uint32)(v195) + (uint32)(((uint32)(8) * (uint32)((r_u32((uint32)((v186 + 4u * (uint32)(11)))) & 0xFFF)))));
          v191 = (((uint32)(v194) >> 24u) & 0xFFu);
          gte_a1 = (sint32)gte_read_h();
          gte_t0 = polygon_gte_flags();
          v198 = ((sint32)(gte_a1) >> ((uint32)(1) & 31u));
          v199 = ((uint32)gte_a1 < 0x400u);
          if ((gte_t0 >= 0))
            goto LABEL_188;
          if (!v199)
          {
            gte_v0 = xport_gte_read_data(17u);
            gte_v1 = xport_gte_read_data(18u);
            gte_a0 = xport_gte_read_data(19u);
            if ((((gte_v0 | gte_v1) | gte_a0) >= v198))
            {
              LABEL_188:
              if (v199)
                goto LABEL_192;

              w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x14C)))), (uint32)(xport_gte_read_data(12u)));
              w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x16C)))), (uint32)(xport_gte_read_data(13u)));
              w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x18C)))), (uint32)(xport_gte_read_data(14u)));
              v203 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(332)))));
              v204 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(364)))));
              v205 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(396)))));
              v206 = v203;
              v207 = (v203 < v204);
              if ((v204 < v203))
                v203 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(364)))));
              if (v207)
                v206 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(364)))));
              if ((v205 < v203))
                v203 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(396)))));
              if ((v206 < v205))
                v206 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(396)))));
              if (((v206 <= 0) || (v203 >= (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(4))))))))
                goto LABEL_196;
              v208 = (gte_t0 | (sint32)((uint32)(1023) - (uint32)((sint32)((uint32)(v206) - (uint32)(v203)))));
              v209 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(334)))));
              v210 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(366)))));
              v211 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(398)))));
              v212 = v209;
              v213 = (v209 < v210);
              if ((v210 < v209))
                v209 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(366)))));
              if (v213)
                v212 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(366)))));
              if ((v211 < v209))
                v209 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(398)))));
              if ((v212 < v211))
                v212 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(398)))));
              if (((v212 <= 0) || (v209 >= (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(6))))))))
                goto LABEL_196;
              if (((v208 | (sint32)((uint32)(511) - (uint32)((sint32)((uint32)(v212) - (uint32)(v209))))) >= 0))
              {
                LABEL_192:
                gte_a1 = xport_gte_read_data(12u);

                gte_a3 = xport_gte_read_data(13u);
                gte_t0 = xport_gte_read_data(14u);
                if (((((gte_a1 & gte_a3) & gte_t0) & 0x80008000) == 0))
                {
                  v222 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(772)))));
                  w_u32((uint32)((v186 + 4u * (uint32)(4))), (uint32)(gte_a1));
                  if (((v193 & 1) == 0))
                    v222 += 1u * (uint32)(4);
                  w_u32((uint32)((v186 + 4u * (uint32)(6))), (uint32)(gte_a3));
                  w_u32((uint32)((v186 + 4u * (uint32)(8))), (uint32)(gte_t0));
                  v223 = (sint8)r_u8((uint32)(v222));
                  v224 = (sint8)r_u8((uint32)((v222 + 1u * (uint32)(1))));
                  v225 = (sint8)r_u8((uint32)((v222 + 1u * (uint32)(2))));
                  w_u16((uint32)((uint32)(v222)), (uint32)((uint16)(v192)));
                  w_u8((uint32)((v222 + 1u * (uint32)(2))), (uint32)(((uint32)(((uint32)((uint32)(v192)) << ((uint32)(8) & 31u))) >> ((uint32)(24) & 31u))));
                  w_u8((uint32)(v192), (uint32)(v223));
                  w_u8((uint32)(((uint32)(v186) + 1u * (uint32)(9))), (uint32)(v224));
                  w_u8((uint32)(((uint32)(v186) + 1u * (uint32)(10))), (uint32)(v225));
                }
              }
              else
              {
                v214 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(772)))));
                if (((v193 & 1) == 0))
                  v214 = ((uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(772)))))) + (uint32)(4));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(168)))), (uint32)(v214));
                v215 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(780)))));
                w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(184)))), (uint32)(3));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(172)))), (uint32)(v215));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(156)))), (uint32)(r_u32((uint32)((v186 + 4u * (uint32)(3))))));
                v216 = r_u32((uint32)((v186 + 4u * (uint32)(5))));
                v217 = r_u32((uint32)((v186 + 4u * (uint32)(7))));
                v218 = r_u32((uint32)((v186 + 4u * (uint32)(9))));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(164)))), (uint32)(v217));
                w_u8((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(164)))), (uint32)((((uint32)(v216) >> 16u) & 0xFFu)));
                w_u8((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(165)))), (uint32)((((uint32)(v216) >> 24u) & 0xFFu)));
                w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(336)))), (uint32)(((((uint32)((uint16)(v216)) << ((uint32)(7) & 31u)) & 0x7F80) | 0x40)));
                w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(368)))), (uint32)(((((uint32)((uint16)(v217)) << ((uint32)(7) & 31u)) & 0x7F80) | 0x40)));
                w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(338)))), (uint32)(((((uint32)(v216) >> ((uint32)(1) & 31u)) & 0x7F80) | 0x40)));
                w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(370)))), (uint32)(((((uint32)(v217) >> ((uint32)(1) & 31u)) & 0x7F80) | 0x40)));
                w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(402)))), (uint32)(((((uint32)(v218) >> ((uint32)(1) & 31u)) & 0x7F80) | 0x40)));
                w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(400)))), (uint32)(((((uint32)((uint16)(v218)) << ((uint32)(7) & 31u)) & 0x7F80) | 0x40)));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x144)))), (uint32)(xport_gte_read_data(0u)));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x148)))), (uint32)(xport_gte_read_data(1u)));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x164)))), (uint32)(xport_gte_read_data(2u)));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x168)))), (uint32)(xport_gte_read_data(3u)));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x184)))), (uint32)(xport_gte_read_data(4u)));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x188)))), (uint32)(xport_gte_read_data(5u)));
                gtmain_polygon_clip_draw(a0);
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(780)))), (uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(180)))))));
              }
            }
          }
          LABEL_196:
          v186 += 4u * (uint32)(10);

        }
        while (v185);
      }
      v6 = v186;
    }
    v226 = v372;
    v227 = r_u16((uint32)((uint32)((sint32)((uint32)(v372) + (uint32)(50)))));
    v228 = v6;
    if (r_u16((uint32)((uint32)((sint32)((uint32)(v372) + (uint32)(50))))))
    {
      v229 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(776)))));
      gte_fp = ((uint32)(v229) + (uint32)(((uint32)(8) * (uint32)((r_u32((uint32)(v6)) & 0xFFF)))));
      gte_s5 = ((uint32)(v229) + (uint32)((((uint32)(r_u32((uint32)(v6))) >> ((uint32)(9) & 31u)) & 0x7FF8)));
      gte_s4 = ((uint32)(v229) + (uint32)(((uint32)(8) * (uint32)((r_u32((uint32)((v6 + 4u * (uint32)(1)))) & 0xFFF)))));
      v233 = (uint32)(((uint32)(v229) + (uint32)((((uint32)(r_u32((uint32)((v6 + 4u * (uint32)(1))))) >> ((uint32)(9) & 31u)) & 0x7FF8))));
      v234 = (((uint32)(r_u32((uint32)(v6))) >> 24u) & 0xFFu);
      if (r_u16((uint32)((uint32)((sint32)((uint32)(v372) + (uint32)(50))))))
      {
        do
        {
          v235 = (v228 + 4u * (uint32)(2));
          xport_gte_write_data(0u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_fp) + (uint32)(0))))));
          xport_gte_write_data(1u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_fp) + (uint32)(4))))));
          xport_gte_write_data(2u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s5) + (uint32)(0))))));
          xport_gte_write_data(3u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s5) + (uint32)(4))))));
          w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x144)))), (uint32)(xport_gte_read_data(0u)));
          w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x148)))), (uint32)(xport_gte_read_data(1u)));
          xport_gte_write_data(4u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s4) + (uint32)(0))))));
          xport_gte_write_data(5u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s4) + (uint32)(4))))));
          v236 = v234;
          v237 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(776)))));
          xport_gte_execute(0x280030);
          v238 = r_u32((uint32)((v228 + 4u * (uint32)(12))));
          gte_v1 = (sint32)r_u32((uint32)(v233));
          gte_a0 = (sint32)r_u32((uint32)((v233 + 4u * (uint32)(1))));
          gte_fp = ((uint32)(v237) + (uint32)(((uint32)(8) * (uint32)((v238 & 0xFFF)))));
          gte_a1 = polygon_gte_flags();
          gte_v0 = xport_gte_read_data(17u);
          gte_v0 = xport_gte_read_data(17u);
          gte_v0 = xport_gte_read_data(18u);
          w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x14C)))), (uint32)(xport_gte_read_data(12u)));
          xport_gte_write_data(0u, gte_v1);
          xport_gte_write_data(1u, gte_a0);
          v234 = (((uint32)(v238) >> 24u) & 0xFFu);
          (v227 -= 1u);
          xport_gte_execute(0x180001);
          gte_s5 = ((uint32)(v237) + (uint32)((((uint32)(v238) >> ((uint32)(9) & 31u)) & 0x7FF8)));
          gte_s4 = ((uint32)(v237) + (uint32)(((uint32)(8) * (uint32)((r_u32((uint32)((v228 + 4u * (uint32)(13)))) & 0xFFF)))));
          v233 = (uint32)(((uint32)(v237) + (uint32)((((uint32)(r_u32((uint32)((v228 + 4u * (uint32)(13))))) >> ((uint32)(9) & 31u)) & 0x7FF8))));
          gte_v0 = polygon_gte_flags();
          gte_v1 = (sint32)gte_read_h();
          v247 = (gte_a1 | gte_v0);
          v248 = ((uint32)gte_v1 < 0x400u);
          if ((v247 < 0))
          {
            if (v248)
              goto LABEL_240;
            gte_v0 = xport_gte_read_data(19u);
          }
          if (v248)
            goto LABEL_236;
          w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x16C)))), (uint32)(xport_gte_read_data(12u)));
          w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x18C)))), (uint32)(xport_gte_read_data(13u)));
          w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x1AC)))), (uint32)(xport_gte_read_data(14u)));
          v250 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(332)))));
          v251 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(364)))));
          v252 = v250;
          v253 = (v250 < v251);
          if ((v251 < v250))
            v250 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(364)))));
          v254 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(396)))));
          if (v253)
            v252 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(364)))));
          if ((v254 < v250))
            v250 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(396)))));
          v255 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(428)))));
          if ((v252 < v254))
            v252 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(396)))));
          if ((v255 < v250))
            v250 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(428)))));
          if ((v252 < v255))
            v252 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(428)))));
          v256 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(4)))));
          if (((v252 <= 0) || (v250 >= v256)))
            goto LABEL_240;
          v257 = ((v247 | v250) | (sint32)((uint32)(v256) - (uint32)(v252)));
          v258 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(334)))));
          v259 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(366)))));
          v260 = v258;
          v261 = (v258 < v259);
          if ((v259 < v258))
            v258 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(366)))));
          v262 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(398)))));
          if (v261)
            v260 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(366)))));
          if ((v262 < v258))
            v258 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(398)))));
          v263 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(430)))));
          if ((v260 < v262))
            v260 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(398)))));
          if ((v263 < v258))
            v258 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(430)))));
          if ((v260 < v263))
            v260 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(430)))));
          v264 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(6)))));
          if (((v260 <= 0) || (v258 >= v264)))
            goto LABEL_240;
          if ((((v257 | v258) | (sint32)((uint32)(v264) - (uint32)(v260))) >= 0))
          {
            LABEL_236:
            v271 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(332)))));

            gte_a3 = xport_gte_read_data(12u);
            gte_t0 = xport_gte_read_data(13u);
            gte_t1 = xport_gte_read_data(14u);
            if ((((((v271 & gte_a3) & gte_t0) & gte_t1) & 0x80008000) == 0))
            {
              v275 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(772)))));
              w_u32((uint32)((v228 + 4u * (uint32)(4))), (uint32)(v271));
              if (((v236 & 1) == 0))
                v275 += 1u * (uint32)(4);
              w_u32((uint32)((v228 + 4u * (uint32)(6))), (uint32)(gte_a3));
              w_u32((uint32)((v228 + 4u * (uint32)(8))), (uint32)(gte_t0));
              w_u32((uint32)((v228 + 4u * (uint32)(10))), (uint32)(gte_t1));
              v276 = (sint8)r_u8((uint32)(v275));
              v277 = (sint8)r_u8((uint32)((v275 + 1u * (uint32)(1))));
              v278 = (sint8)r_u8((uint32)((v275 + 1u * (uint32)(2))));
              w_u16((uint32)((uint32)(v275)), (uint32)((uint16)(v235)));
              w_u8((uint32)((v275 + 1u * (uint32)(2))), (uint32)(((uint32)(((uint32)((uint32)(v235)) << ((uint32)(8) & 31u))) >> ((uint32)(24) & 31u))));
              w_u8((uint32)(v235), (uint32)(v276));
              w_u8((uint32)(((uint32)(v228) + 1u * (uint32)(9))), (uint32)(v277));
              w_u8((uint32)(((uint32)(v228) + 1u * (uint32)(10))), (uint32)(v278));
            }
          }
          else
          {
            v265 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(772)))));
            if (((v236 & 1) == 0))
              v265 = ((uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(772)))))) + (uint32)(4));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(168)))), (uint32)(v265));
            v266 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(780)))));
            w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(184)))), (uint32)(4));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(172)))), (uint32)(v266));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(156)))), (uint32)(r_u32((uint32)((v228 + 4u * (uint32)(3))))));
            v267 = r_u32((uint32)((v228 + 4u * (uint32)(5))));
            v268 = r_u32((uint32)((v228 + 4u * (uint32)(7))));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(164)))), (uint32)(v268));
            w_u8((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(164)))), (uint32)((((uint32)(v267) >> 16u) & 0xFFu)));
            w_u8((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(165)))), (uint32)((((uint32)(v267) >> 24u) & 0xFFu)));
            w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(336)))), (uint32)(((((uint32)((uint16)(v267)) << ((uint32)(7) & 31u)) & 0x7F80) | 0x40)));
            w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(338)))), (uint32)(((((uint32)(v267) >> ((uint32)(1) & 31u)) & 0x7F80) | 0x40)));
            v269 = r_u32((uint32)((v228 + 4u * (uint32)(9))));
            w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(368)))), (uint32)(((((uint32)((uint16)(v268)) << ((uint32)(7) & 31u)) & 0x7F80) | 0x40)));
            w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(370)))), (uint32)(((((uint32)(v268) >> ((uint32)(1) & 31u)) & 0x7F80) | 0x40)));
            v270 = r_u32((uint32)((v228 + 4u * (uint32)(11))));
            w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(432)))), (uint32)(((((uint32)((uint16)(v269)) << ((uint32)(7) & 31u)) & 0x7F80) | 0x40)));
            w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(434)))), (uint32)(((((uint32)(v269) >> ((uint32)(1) & 31u)) & 0x7F80) | 0x40)));
            w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(400)))), (uint32)(((((uint32)((uint16)(v270)) << ((uint32)(7) & 31u)) & 0x7F80) | 0x40)));
            w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(402)))), (uint32)(((((uint32)(v270) >> ((uint32)(1) & 31u)) & 0x7F80) | 0x40)));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x184)))), (uint32)(xport_gte_read_data(0u)));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x188)))), (uint32)(xport_gte_read_data(1u)));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x164)))), (uint32)(xport_gte_read_data(2u)));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x168)))), (uint32)(xport_gte_read_data(3u)));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x1A4)))), (uint32)(xport_gte_read_data(4u)));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x1A8)))), (uint32)(xport_gte_read_data(5u)));
            gtmain_polygon_clip_draw(a0);
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(780)))), (uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(180)))))));
          }
          LABEL_240:
          v228 += 4u * (uint32)(12);

        }
        while (v227);
      }
      v6 = v228;
      v226 = v372;
    }
    v279 = r_u16((uint32)((uint32)((sint32)((uint32)(v226) + (uint32)(52)))));
    v280 = v6;
    if (r_u16((uint32)((uint32)((sint32)((uint32)(v226) + (uint32)(52))))))
    {
      v281 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(776)))));
      gte_s6 = ((uint32)(v281) + (uint32)(((uint32)(8) * (uint32)((r_u32((uint32)(v6)) & 0xFFF)))));
      gte_s5 = ((uint32)(v281) + (uint32)((((uint32)(r_u32((uint32)(v6))) >> ((uint32)(9) & 31u)) & 0x7FF8)));
      gte_s4 = ((uint32)(v281) + (uint32)(((uint32)(8) * (uint32)((r_u32((uint32)((v6 + 4u * (uint32)(1)))) & 0xFFF)))));
      v285 = (((uint32)(r_u32((uint32)(v6))) >> 24u) & 0xFFu);
      if (r_u16((uint32)((uint32)((sint32)((uint32)(v226) + (uint32)(52))))))
      {
        do
        {
          v286 = (v280 + 4u * (uint32)(2));
          xport_gte_write_data(0u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s6) + (uint32)(0))))));
          xport_gte_write_data(1u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s6) + (uint32)(4))))));
          xport_gte_write_data(2u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s5) + (uint32)(0))))));
          xport_gte_write_data(3u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s5) + (uint32)(4))))));
          xport_gte_write_data(4u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s4) + (uint32)(0))))));
          xport_gte_write_data(5u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s4) + (uint32)(4))))));
          v287 = v285;
          (v279 -= 1u);
          xport_gte_execute(0x280030);
          v288 = r_u32((uint32)((v280 + 4u * (uint32)(12))));
          v289 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(776)))));
          gte_s6 = ((uint32)(v289) + (uint32)(((uint32)(8) * (uint32)((v288 & 0xFFF)))));
          gte_s5 = ((uint32)(v289) + (uint32)((((uint32)(v288) >> ((uint32)(9) & 31u)) & 0x7FF8)));
          gte_s4 = ((uint32)(v289) + (uint32)(((uint32)(8) * (uint32)((r_u32((uint32)((v280 + 4u * (uint32)(13)))) & 0xFFF)))));
          v285 = (((uint32)(v288) >> 24u) & 0xFFu);
          gte_a1 = (sint32)gte_read_h();
          gte_t0 = polygon_gte_flags();
          v292 = ((sint32)(gte_a1) >> ((uint32)(1) & 31u));
          v293 = ((uint32)gte_a1 < 0x400u);
          if ((gte_t0 >= 0))
            goto LABEL_268;
          if (!v293)
          {
            gte_v0 = xport_gte_read_data(17u);
            gte_v1 = xport_gte_read_data(18u);
            gte_a0 = xport_gte_read_data(19u);
            if ((((gte_v0 | gte_v1) | gte_a0) >= v292))
            {
              LABEL_268:
              if (v293)
                goto LABEL_272;

              w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x14C)))), (uint32)(xport_gte_read_data(12u)));
              w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x16C)))), (uint32)(xport_gte_read_data(13u)));
              w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x18C)))), (uint32)(xport_gte_read_data(14u)));
              v297 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(332)))));
              v298 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(364)))));
              v299 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(396)))));
              v300 = v297;
              v301 = (v297 < v298);
              if ((v298 < v297))
                v297 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(364)))));
              if (v301)
                v300 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(364)))));
              if ((v299 < v297))
                v297 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(396)))));
              if ((v300 < v299))
                v300 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(396)))));
              if (((v300 <= 0) || (v297 >= (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(4))))))))
                goto LABEL_276;
              v302 = (gte_t0 | (sint32)((uint32)(1023) - (uint32)((sint32)((uint32)(v300) - (uint32)(v297)))));
              v303 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(334)))));
              v304 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(366)))));
              v305 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(398)))));
              v306 = v303;
              v307 = (v303 < v304);
              if ((v304 < v303))
                v303 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(366)))));
              if (v307)
                v306 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(366)))));
              if ((v305 < v303))
                v303 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(398)))));
              if ((v306 < v305))
                v306 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(398)))));
              if (((v306 <= 0) || (v303 >= (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(6))))))))
                goto LABEL_276;
              if (((v302 | (sint32)((uint32)(511) - (uint32)((sint32)((uint32)(v306) - (uint32)(v303))))) >= 0))
              {
                LABEL_272:
                gte_a1 = xport_gte_read_data(12u);

                gte_a3 = xport_gte_read_data(13u);
                gte_t0 = xport_gte_read_data(14u);
                if (((((gte_a1 & gte_a3) & gte_t0) & 0x80008000) == 0))
                {
                  v316 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(772)))));
                  w_u32((uint32)((v280 + 4u * (uint32)(4))), (uint32)(gte_a1));
                  if (((v287 & 1) == 0))
                    v316 += 1u * (uint32)(4);
                  w_u32((uint32)((v280 + 4u * (uint32)(7))), (uint32)(gte_a3));
                  w_u32((uint32)((v280 + 4u * (uint32)(10))), (uint32)(gte_t0));
                  v317 = (sint8)r_u8((uint32)(v316));
                  v318 = (sint8)r_u8((uint32)((v316 + 1u * (uint32)(1))));
                  v319 = (sint8)r_u8((uint32)((v316 + 1u * (uint32)(2))));
                  w_u16((uint32)((uint32)(v316)), (uint32)((uint16)(v286)));
                  w_u8((uint32)((v316 + 1u * (uint32)(2))), (uint32)(((uint32)(((uint32)((uint32)(v286)) << ((uint32)(8) & 31u))) >> ((uint32)(24) & 31u))));
                  w_u8((uint32)(v286), (uint32)(v317));
                  w_u8((uint32)(((uint32)(v280) + 1u * (uint32)(9))), (uint32)(v318));
                  w_u8((uint32)(((uint32)(v280) + 1u * (uint32)(10))), (uint32)(v319));
                }
              }
              else
              {
                v308 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(772)))));
                if (((v287 & 1) == 0))
                  v308 = ((uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(772)))))) + (uint32)(4));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(168)))), (uint32)(v308));
                v309 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(780)))));
                w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(184)))), (uint32)(3));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(172)))), (uint32)(v309));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(156)))), (uint32)(r_u32((uint32)((v280 + 4u * (uint32)(3))))));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(340)))), (uint32)(r_u32((uint32)((v280 + 4u * (uint32)(3))))));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(372)))), (uint32)(r_u32((uint32)((v280 + 4u * (uint32)(6))))));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(404)))), (uint32)(r_u32((uint32)((v280 + 4u * (uint32)(9))))));
                v310 = r_u32((uint32)((v280 + 4u * (uint32)(5))));
                v311 = r_u32((uint32)((v280 + 4u * (uint32)(8))));
                v312 = r_u32((uint32)((v280 + 4u * (uint32)(11))));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(164)))), (uint32)(v311));
                w_u8((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(164)))), (uint32)((((uint32)(v310) >> 16u) & 0xFFu)));
                w_u8((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(165)))), (uint32)((((uint32)(v310) >> 24u) & 0xFFu)));
                w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(336)))), (uint32)(((((uint32)((uint16)(v310)) << ((uint32)(7) & 31u)) & 0x7F80) | 0x40)));
                w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(368)))), (uint32)(((((uint32)((uint16)(v311)) << ((uint32)(7) & 31u)) & 0x7F80) | 0x40)));
                w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(338)))), (uint32)(((((uint32)(v310) >> ((uint32)(1) & 31u)) & 0x7F80) | 0x40)));
                w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(370)))), (uint32)(((((uint32)(v311) >> ((uint32)(1) & 31u)) & 0x7F80) | 0x40)));
                w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(402)))), (uint32)(((((uint32)(v312) >> ((uint32)(1) & 31u)) & 0x7F80) | 0x40)));
                w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(400)))), (uint32)(((((uint32)((uint16)(v312)) << ((uint32)(7) & 31u)) & 0x7F80) | 0x40)));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x144)))), (uint32)(xport_gte_read_data(0u)));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x148)))), (uint32)(xport_gte_read_data(1u)));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x164)))), (uint32)(xport_gte_read_data(2u)));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x168)))), (uint32)(xport_gte_read_data(3u)));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x184)))), (uint32)(xport_gte_read_data(4u)));
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x188)))), (uint32)(xport_gte_read_data(5u)));
                gtmain_polygon_clip_textured_gouraud(a0);
                w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(780)))), (uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(180)))))));
              }
            }
          }
          LABEL_276:
          v280 += 4u * (uint32)(12);

        }
        while (v279);
      }
      v6 = v280;
    }
    v320 = r_u16((uint32)((uint32)((sint32)((uint32)(v372) + (uint32)(54)))));
    v321 = v6;
    if (r_u16((uint32)((uint32)((sint32)((uint32)(v372) + (uint32)(54))))))
    {
      v322 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(776)))));
      gte_fp = ((uint32)(v322) + (uint32)(((uint32)(8) * (uint32)((r_u32((uint32)(v6)) & 0xFFF)))));
      gte_s5 = ((uint32)(v322) + (uint32)((((uint32)(r_u32((uint32)(v6))) >> ((uint32)(9) & 31u)) & 0x7FF8)));
      gte_s4 = ((uint32)(v322) + (uint32)(((uint32)(8) * (uint32)((r_u32((uint32)((v6 + 4u * (uint32)(1)))) & 0xFFF)))));
      v326 = (uint32)(((uint32)(v322) + (uint32)((((uint32)(r_u32((uint32)((v6 + 4u * (uint32)(1))))) >> ((uint32)(9) & 31u)) & 0x7FF8))));
      v327 = (((uint32)(r_u32((uint32)(v6))) >> 24u) & 0xFFu);
      if (r_u16((uint32)((uint32)((sint32)((uint32)(v372) + (uint32)(54))))))
      {
        do
        {
          v328 = (v321 + 4u * (uint32)(2));
          xport_gte_write_data(0u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_fp) + (uint32)(0))))));
          xport_gte_write_data(1u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_fp) + (uint32)(4))))));
          xport_gte_write_data(2u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s5) + (uint32)(0))))));
          xport_gte_write_data(3u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s5) + (uint32)(4))))));
          w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x144)))), (uint32)(xport_gte_read_data(0u)));
          w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x148)))), (uint32)(xport_gte_read_data(1u)));
          xport_gte_write_data(4u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s4) + (uint32)(0))))));
          xport_gte_write_data(5u, r_u32((uint32)((uint32)((sint32)((uint32)(gte_s4) + (uint32)(4))))));
          v329 = v327;
          v330 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(776)))));
          xport_gte_execute(0x280030);
          v331 = r_u32((uint32)((v321 + 4u * (uint32)(15))));
          gte_v1 = (sint32)r_u32((uint32)(v326));
          gte_a0 = (sint32)r_u32((uint32)((v326 + 4u * (uint32)(1))));
          gte_fp = ((uint32)(v330) + (uint32)(((uint32)(8) * (uint32)((v331 & 0xFFF)))));
          gte_a1 = polygon_gte_flags();
          gte_v0 = xport_gte_read_data(17u);
          gte_v0 = xport_gte_read_data(17u);
          gte_v0 = xport_gte_read_data(18u);
          w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x14C)))), (uint32)(xport_gte_read_data(12u)));
          xport_gte_write_data(0u, gte_v1);
          xport_gte_write_data(1u, gte_a0);
          v327 = (((uint32)(v331) >> 24u) & 0xFFu);
          (v320 -= 1u);
          xport_gte_execute(0x180001);
          gte_s5 = ((uint32)(v330) + (uint32)((((uint32)(v331) >> ((uint32)(9) & 31u)) & 0x7FF8)));
          gte_s4 = ((uint32)(v330) + (uint32)(((uint32)(8) * (uint32)((r_u32((uint32)((v321 + 4u * (uint32)(16)))) & 0xFFF)))));
          v326 = (uint32)(((uint32)(v330) + (uint32)((((uint32)(r_u32((uint32)((v321 + 4u * (uint32)(16))))) >> ((uint32)(9) & 31u)) & 0x7FF8))));
          gte_v0 = polygon_gte_flags();
          gte_v1 = (sint32)gte_read_h();
          v340 = (gte_a1 | gte_v0);
          v341 = ((uint32)gte_v1 < 0x400u);
          if ((v340 < 0))
          {
            if (v341)
              goto LABEL_320;
            gte_v0 = xport_gte_read_data(19u);
          }
          if (v341)
            goto LABEL_316;
          w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x16C)))), (uint32)(xport_gte_read_data(12u)));
          w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x18C)))), (uint32)(xport_gte_read_data(13u)));
          w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x1AC)))), (uint32)(xport_gte_read_data(14u)));
          v343 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(332)))));
          v344 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(364)))));
          v345 = v343;
          v346 = (v343 < v344);
          if ((v344 < v343))
            v343 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(364)))));
          v347 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(396)))));
          if (v346)
            v345 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(364)))));
          if ((v347 < v343))
            v343 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(396)))));
          v348 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(428)))));
          if ((v345 < v347))
            v345 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(396)))));
          if ((v348 < v343))
            v343 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(428)))));
          if ((v345 < v348))
            v345 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(428)))));
          v349 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(4)))));
          if (((v345 <= 0) || (v343 >= v349)))
            goto LABEL_320;
          v350 = ((v340 | v343) | (sint32)((uint32)(v349) - (uint32)(v345)));
          v351 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(334)))));
          v352 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(366)))));
          v353 = v351;
          v354 = (v351 < v352);
          if ((v352 < v351))
            v351 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(366)))));
          v355 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(398)))));
          if (v354)
            v353 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(366)))));
          if ((v355 < v351))
            v351 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(398)))));
          v356 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(430)))));
          if ((v353 < v355))
            v353 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(398)))));
          if ((v356 < v351))
            v351 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(430)))));
          if ((v353 < v356))
            v353 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(430)))));
          v357 = (sint16)r_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(6)))));
          if (((v353 <= 0) || (v351 >= v357)))
            goto LABEL_320;
          if ((((v350 | v351) | (sint32)((uint32)(v357) - (uint32)(v353))) >= 0))
          {
            LABEL_316:
            v364 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(332)))));

            gte_a3 = xport_gte_read_data(12u);
            gte_t0 = xport_gte_read_data(13u);
            gte_t2 = xport_gte_read_data(14u);
            if ((((((v364 & gte_a3) & gte_t0) & gte_t2) & 0x80008000) == 0))
            {
              v368 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(772)))));
              w_u32((uint32)((v321 + 4u * (uint32)(4))), (uint32)(v364));
              if (((v329 & 1) == 0))
                v368 += 1u * (uint32)(4);
              w_u32((uint32)((v321 + 4u * (uint32)(7))), (uint32)(gte_a3));
              w_u32((uint32)((v321 + 4u * (uint32)(10))), (uint32)(gte_t0));
              w_u32((uint32)((v321 + 4u * (uint32)(13))), (uint32)(gte_t2));
              v369 = (sint8)r_u8((uint32)(v368));
              v370 = (sint8)r_u8((uint32)((v368 + 1u * (uint32)(1))));
              v371 = (sint8)r_u8((uint32)((v368 + 1u * (uint32)(2))));
              w_u16((uint32)((uint32)(v368)), (uint32)((uint16)(v328)));
              w_u8((uint32)((v368 + 1u * (uint32)(2))), (uint32)(((uint32)(((uint32)((uint32)(v328)) << ((uint32)(8) & 31u))) >> ((uint32)(24) & 31u))));
              w_u8((uint32)(v328), (uint32)(v369));
              w_u8((uint32)(((uint32)(v321) + 1u * (uint32)(9))), (uint32)(v370));
              w_u8((uint32)(((uint32)(v321) + 1u * (uint32)(10))), (uint32)(v371));
            }
          }
          else
          {
            v358 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(772)))));
            if (((v329 & 1) == 0))
              v358 = ((uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(772)))))) + (uint32)(4));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(168)))), (uint32)(v358));
            v359 = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(780)))));
            w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(184)))), (uint32)(4));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(172)))), (uint32)(v359));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(156)))), (uint32)(r_u32((uint32)((v321 + 4u * (uint32)(3))))));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(340)))), (uint32)(r_u32((uint32)((v321 + 4u * (uint32)(3))))));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(372)))), (uint32)(r_u32((uint32)((v321 + 4u * (uint32)(6))))));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(436)))), (uint32)(r_u32((uint32)((v321 + 4u * (uint32)(9))))));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(404)))), (uint32)(r_u32((uint32)((v321 + 4u * (uint32)(12))))));
            v360 = r_u32((uint32)((v321 + 4u * (uint32)(5))));
            v361 = r_u32((uint32)((v321 + 4u * (uint32)(8))));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(164)))), (uint32)(v361));
            w_u8((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(164)))), (uint32)((((uint32)(v360) >> 16u) & 0xFFu)));
            w_u8((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(165)))), (uint32)((((uint32)(v360) >> 24u) & 0xFFu)));
            w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(336)))), (uint32)(((((uint32)((uint16)(v360)) << ((uint32)(7) & 31u)) & 0x7F80) | 0x40)));
            w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(338)))), (uint32)(((((uint32)(v360) >> ((uint32)(1) & 31u)) & 0x7F80) | 0x40)));
            v362 = r_u32((uint32)((v321 + 4u * (uint32)(11))));
            w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(368)))), (uint32)(((((uint32)((uint16)(v361)) << ((uint32)(7) & 31u)) & 0x7F80) | 0x40)));
            w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(370)))), (uint32)(((((uint32)(v361) >> ((uint32)(1) & 31u)) & 0x7F80) | 0x40)));
            v363 = r_u32((uint32)((v321 + 4u * (uint32)(14))));
            w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(432)))), (uint32)(((((uint32)((uint16)(v362)) << ((uint32)(7) & 31u)) & 0x7F80) | 0x40)));
            w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(434)))), (uint32)(((((uint32)(v362) >> ((uint32)(1) & 31u)) & 0x7F80) | 0x40)));
            w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(400)))), (uint32)(((((uint32)((uint16)(v363)) << ((uint32)(7) & 31u)) & 0x7F80) | 0x40)));
            w_u16((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(402)))), (uint32)(((((uint32)(v363) >> ((uint32)(1) & 31u)) & 0x7F80) | 0x40)));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x184)))), (uint32)(xport_gte_read_data(0u)));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x188)))), (uint32)(xport_gte_read_data(1u)));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x164)))), (uint32)(xport_gte_read_data(2u)));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x168)))), (uint32)(xport_gte_read_data(3u)));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x1A4)))), (uint32)(xport_gte_read_data(4u)));
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(0x1A8)))), (uint32)(xport_gte_read_data(5u)));
            gtmain_polygon_clip_textured_gouraud(a0);
            w_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(780)))), (uint32)(r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(180)))))));
          }
          LABEL_320:
          v321 += 4u * (uint32)(15);

        }
        while (v320);
      }
    }
    result = r_u32((uint32)((uint32)((sint32)((uint32)((sint32)a0) + (uint32)(780)))));
    w_u32((uint32)(0x800BFE4C), (uint32)(result));
  }
  return result;
}

