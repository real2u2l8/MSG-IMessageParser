/*
 * MSG File Parser Tool - Advanced MSG file analysis tool with MAPI property parsing
 * Copyright (C) 2025  real2u2l8
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include "pch.h"

/**
 * @brief �궗�슜�옄 �씤�꽣�럹�씠�뒪 愿��젴 �븿�닔�뱾�쓣 紐⑥븘�넃��� �꽕�엫�뒪�럹�씠�뒪
 * 
 * @details
 * �봽濡쒓렇�옩 �떎�뻾 �떆 �궗�슜�옄�뿉寃� 蹂댁뿬二쇰뒗 �솕硫닿낵 硫붿떆吏��뱾�쓣 泥섎━�빀�땲�떎.
 * �궗�슜踰� 異쒕젰, 諛곕꼫 �몴�떆, �뿉�윭 硫붿떆吏� �벑�씠 �룷�븿�맗�땲�떎.
 */
namespace UI {

    /**
     * @brief �봽濡쒓렇�옩 �궗�슜踰뺤쓣 異쒕젰
     * 
     * @param[in] programName �봽濡쒓렇�옩 �씠由�
     * 
     * @details
     * �봽濡쒓렇�옩�쓽 �궗�슜踰�, �씤�옄 �꽕紐�, �삁�젣, 湲곕뒫 �꽕紐� �벑�쓣
     * 肄섏넄�뿉 異쒕젰�빀�땲�떎.
     * 
     * @example
     * UI::PrintUsage();
     */
    void PrintUsage();

} // namespace UI
