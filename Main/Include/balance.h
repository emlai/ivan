/*
 *
 *  Iter Vehemens ad Necem (IVAN)
 *  Copyright (C) Timo Kiviluoto
 *  Released under the GNU General
 *  Public License
 *
 *  See LICENSING which should be included
 *  along with this file for more details
 *
 */

#ifndef __BALANCE_H__
#define __BALANCE_H__

/*
 * Global balance defines for the project IVAN.
 * This file is created to hold macros that affect the game balance
 */

cdouble GLOBAL_WEAK_BODYPART_HIT_MODIFIER = 10.;

cint EDIT_ATTRIBUTE_DAY_MIN = 10; // last day there is no monster attribute plus
cint EDIT_ATTRIBUTE_DAY_SHIFT = 2; // attribute plus = floor((day - min) / 2^shift)

cint DANGER_PLUS_DAY_MIN = 5; // last day monster danger plus is zero
cdouble DANGER_PLUS_MULTIPLIER = .001; // increases danger plus

cint KAMIKAZE_INVISIBILITY_DAY_MIN = 30; // last day no dwarf can be generated invisible
cint KAMIKAZE_INVISIBILITY_DAY_MAX = 50; // after this, all dwarves are invisible

#endif
