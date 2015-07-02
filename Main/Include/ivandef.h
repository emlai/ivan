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

#ifndef __IVANDEF_H__
#define __IVANDEF_H__

/*
 * Global defines for the project IVAN.
 * This file is created to decrease the need of including headers in
 * other headers just for the sake of some silly macros, because it
 * decreases compilation efficiency and may cause cross-including
 *
 * List of macros that should be gathered here:
 * 1. all numeric defines used in multiple .cpp or .h files
 *    except those #defined in balance.h and confdef.h
 * 2. all inline functions used in multiple .cpp or .h files
 *    and independent enough (do not require other headers)
 * 3. class construction macros used in multiple .h files
 */

#include "v2.h"

#define IVAN_VERSION "0.50.3"

class item;
class material;
class character;

typedef const item citem;
typedef const material cmaterial;
typedef const character ccharacter;

struct databasebase
{
  int Config;
  ulong CommonFlags;
  ulong NameFlags;
};

template <class type, class base, class prototype>
class sysbase : public base
{
 public:
  typedef sysbase<type, base, prototype> mybase;
  static type* Spawn(int Config = 0, int SpecialFlags = 0)
  {
    type* T = new type;
    T->Initialize(Config, SpecialFlags);
    return T;
  }
  static type* Clone(const type* T) { return new type(*T); }
  virtual const prototype* FindProtoType() const { return &ProtoType; }
  static const prototype ProtoType;
};

template <class type, class base, class prototype>
class simplesysbase : public base
{
 public:
  typedef simplesysbase<type, base, prototype> mybase;
  static type* Spawn() { return new type; }
  virtual const prototype* GetProtoType() const { return &ProtoType; }
  static const prototype ProtoType;
};

#define SYSTEM_SPECIALIZATIONS(name)\
name##prototype** name##_ProtoData;\
valuemap name##_CodeNameMap;\
int name##_ProtoSize;\
\
template<> name##prototype**& protocontainer<name>::GetProtoData()\
{ return name##_ProtoData; }\
template<> valuemap& protocontainer<name>::GetCodeNameMap()\
{ return name##_CodeNameMap; }\
template<> int& protocontainer<name>::GetSizeRef()\
{ return name##_ProtoSize; }

#define EXTENDED_SYSTEM_SPECIALIZATIONS(name)\
SYSTEM_SPECIALIZATIONS(name)\
databasecreator<name>::databasemembermap name##_DataBaseMemberMap;\
template<> databasecreator<name>::databasemembermap&\
databasecreator<name>::GetDataBaseMemberMap()\
{ return name##_DataBaseMemberMap; }\
const name##prototype name::ProtoType

#define DATA_BASE_VALUE(type, data) type Get##data() const { return DataBase->data; }
#define DATA_BASE_VALUE_WITH_PARAMETER(type, data, param) type Get##data(param) const { return DataBase->data; }
#define DATA_BASE_TRUTH(data) truth data() const { return DataBase->data; }
#define DATA_BASE_TRUTH_WITH_PARAMETER(data, param) truth data(param) const { return DataBase->data; }

enum combatevent
{
  HAS_HIT = 0,
  HAS_BLOCKED = 1,
  HAS_DODGED = 2,
  HAS_DIED = 3,
  DID_NO_DAMAGE = 4,
  HAS_FAILED = 5
};

enum hungerlevel
{
  OVER_FED_LEVEL = 175000,
  BLOATED_LEVEL = 150000,
  SATIATED_LEVEL = 100000,
  NOT_HUNGER_LEVEL = 30000,
  HUNGER_LEVEL = 20000,
  VERY_HUNGER_LEVEL = 10000
};

enum burdenstate
{
  OVER_LOADED = 0,
  STRESSED = 1,
  BURDENED = 2,
  UNBURDENED = 3
};

enum hungerstate
{
  STARVING = 0,
  VERY_HUNGRY = 1,
  HUNGRY = 2,
  NOT_HUNGRY = 3,
  SATIATED = 4,
  BLOATED = 5,
  OVER_FED = 6
};

cint STATES = 21;

enum state : long
{
  NO_STATE = 0,
  POLYMORPHED = 1 << 0,
  HASTE = 1 << 1,
  SLOW = 1 << 2,
  POLYMORPH_CONTROL = 1 << 3,
  LIFE_SAVED = 1 << 4,
  LYCANTHROPY = 1 << 5,
  INVISIBLE = 1 << 6,
  INFRA_VISION = 1 << 7,
  ESP = 1 << 8,
  POISONED = 1 << 9,
  TELEPORT = 1 << 10,
  POLYMORPH = 1 << 11,
  TELEPORT_CONTROL = 1 << 12,
  PANIC = 1 << 13,
  CONFUSED = 1 << 14,
  PARASITIZED = 1 << 15,
  SEARCHING = 1 << 16,
  GAS_IMMUNITY = 1 << 17,
  LEVITATION = 1 << 18,
  LEPROSY = 1 << 19,
  HICCUPS = 1 << 20
};

enum bodypartid
{
  TORSO = 1,
  HEAD = 2,
  RIGHT_ARM = 4,
  LEFT_ARM = 8,
  ARMS = 12,
  GROIN = 16,
  RIGHT_LEG = 32,
  LEFT_LEG = 64,
  LEGS = 96,
  OTHER = 128,
  ALL = 255
};

enum damagetype
{
  PHYSICAL_DAMAGE = 1,
  SOUND = 2,
  ACID = 4,
  FIRE = 8,
  ELECTRICITY = 16,
  ENERGY = 32,
  POISON = 64,
  DRAIN = 128,
  MUSTARD_GAS_DAMAGE = 256,
  PSI = 512,
  THROW = 32768,
};

enum sex
{
  UNDEFINED = 0,
  MALE = 1,
  FEMALE = 2,
  TRANSSEXUAL = 3
};

culong ALL_BODYPART_FLAGS = 0x7F;

/* The maximum bodyparts a character can have */

cint MAX_BODYPARTS = 7;
cint HUMANOID_BODYPARTS = 7;

enum bodypartindex
{
  TORSO_INDEX = 0,
  HEAD_INDEX = 1,
  RIGHT_ARM_INDEX = 2,
  LEFT_ARM_INDEX = 3,
  GROIN_INDEX = 4,
  RIGHT_LEG_INDEX = 5,
  LEFT_LEG_INDEX = 6,
  NONE_INDEX = MAX_BODYPARTS
};

cint DIRECTION_COMMAND_KEYS = 8;
cint EXTENDED_DIRECTION_COMMAND_KEYS = 9;

enum direction
{
  NORTHWEST = 0,
  NORTH = 1,
  NORTHEAST = 2,
  WEST = 3,
  EAST = 4,
  SOUTHWEST = 5,
  SOUTH = 6,
  SOUTHEAST = 7,
  YOURSELF = 8,
  RANDOM_DIR = 9,
  DIR_ERROR = 0xFF
};

#ifndef LIGHT_BORDER
#define LIGHT_BORDER 80
#endif

enum alignment
{
  ALPP = 0,
  ALP = 1,
  AL = 2,
  ALM = 3,
  ANP = 4,
  AN = 5,
  ANM = 6,
  ACP = 7,
  AC = 8,
  ACM = 9,
  ACMM = 10
};

enum caseflag
{
  UNARTICLED = 0,
  PLURAL = 1,
  ARTICLE_BIT = 2,
  DEFINITE = 2,
  INDEFINE_BIT = 4,
  INDEFINITE = 6,
  STRIPPED = 8
};

cint RAW_TYPES = 7;

enum rawtype
{
  GR_GLTERRAIN = 0,
  GR_OLTERRAIN = 1,
  GR_ITEM = 2,
  GR_CHARACTER = 3,
  GR_HUMANOID = 4,
  GR_EFFECT = 5,
  GR_CURSOR = 6
};

cint GRAPHIC_TYPES = 4;

enum graphictype
{
  GR_WTERRAIN = 0,
  GR_FOW = 1,
  GR_SYMBOL = 2,
  GR_SMILEY = 3
};

/* SpecialFlags for graphics system. No one knows what "ST_" means... */

enum graphicspecialflag
{
  ST_NORMAL = 0,
  ST_RIGHT_ARM = 8,
  ST_LEFT_ARM = 16,
  ST_GROIN = 24,
  ST_RIGHT_LEG = 32,
  ST_LEFT_LEG = 40,
  ST_OTHER_BODYPART = 48,
  ST_WIELDED = 56,
  ST_CLOAK = 64,
  ST_LIGHTNING = 128,
  ST_DISALLOW_R_COLORS = 256,
  ST_FLAME_1 = 512,
  ST_FLAME_2 = 1024,
  ST_FLAME_3 = 2048,
  ST_FLAME_4 = 4096,
  ST_FLAMES = ST_FLAME_1|ST_FLAME_2|ST_FLAME_3|ST_FLAME_4,
  ST_FLAME_SHIFT = 9
};

enum wobbleflag
{
  WOBBLE = 1,
  WOBBLE_HORIZONTALLY_BIT = 2,
  WOBBLE_VERTICALLY = WOBBLE,
  WOBBLE_HORIZONTALLY = WOBBLE|WOBBLE_HORIZONTALLY_BIT,
  WOBBLE_SPEED_SHIFT = 2,
  WOBBLE_SPEED_RANGE = 3 << WOBBLE_SPEED_SHIFT,
  WOBBLE_FREQ_SHIFT = 4,
  WOBBLE_FREQ_RANGE = 3 << WOBBLE_FREQ_SHIFT
};

cv2 SILHOUETTE_SIZE(48, 64);

cint ITEM_CATEGORIES = 18;

enum itemcategory
{
  ANY_CATEGORY = 0x7FFFFFFF,
  HELMET = 1 << 0,
  AMULET = 1 << 1,
  CLOAK = 1 << 2,
  BODY_ARMOR = 1 << 3,
  WEAPON = 1 << 4,
  SHIELD = 1 << 5,
  RING = 1 << 6,
  GAUNTLET = 1 << 7,
  BELT = 1 << 8,
  BOOT = 1 << 9,
  FOOD = 1 << 10,
  POTION = 1 << 11,
  SCROLL = 1 << 12,
  BOOK = 1 << 13,
  WAND = 1 << 14,
  TOOL = 1 << 15,
  VALUABLE = 1 << 16,
  MISC = 1 << 17
};

enum basicalignment
{
  GOOD = 1,
  NEUTRAL = 2,
  EVIL = 3
};

enum consumetype
{
  CT_FRUIT = 1,
  CT_MEAT = 2,
  CT_METAL = 4,
  CT_MINERAL = 8,
  CT_LIQUID = 16,
  CT_BONE = 32,
  CT_PROCESSED = 64,
  CT_MISC_ORGANIC = 128,
  CT_PLASTIC = 256,
  CT_GAS = 512
};

/* Possible square positions for item. The first four are used for items
   on walls */

enum squareposition
{
  LEFT = 0,
  DOWN = 1,
  UP = 2,
  RIGHT = 3,
  CENTER = 4 // item on ground
};

enum relation
{
  HOSTILE = 1,
  UNCARING = 2,
  FRIEND = 4
};

cint MARTIAL_SKILL_CATEGORIES = 3;
cint WEAPON_SKILL_CATEGORIES = 11;

enum skillcategory
{
  UNARMED = 0,
  KICK = 1,
  BITE = 2,
  UNCATEGORIZED = 3,
  SMALL_SWORDS = 4,
  LARGE_SWORDS = 5,
  BLUNT_WEAPONS = 6,
  AXES = 7,
  POLE_ARMS = 8,
  WHIPS = 9,
  SHIELDS = 10
};

enum lockstate
{
  UNLOCKED = 0,
  LOCKED = 1
};

enum effect
{
  EFFECT_NOTHING = 0,
  EFFECT_POISON = 1,
  EFFECT_DARKNESS = 2,
  EFFECT_OMMEL_URINE = 3,
  EFFECT_PEPSI = 4,
  EFFECT_KOBOLD_FLESH = 5,
  EFFECT_HEAL = 6,
  EFFECT_LYCANTHROPY = 7,
  EFFECT_SCHOOL_FOOD = 8,
  EFFECT_ANTIDOTE = 9,
  EFFECT_CONFUSE = 10,
  EFFECT_POLYMORPH = 11,
  EFFECT_ESP = 12,
  EFFECT_SKUNK_SMELL = 13,
  EFFECT_MAGIC_MUSHROOM = 14,
  EFFECT_TRAIN_PERCEPTION = 15,
  EFFECT_HOLY_BANANA = 16,
  EFFECT_EVIL_WONDER_STAFF_VAPOUR = 17,
  EFFECT_GOOD_WONDER_STAFF_VAPOUR = 18,
  EFFECT_PEA_SOUP = 19,
  EFFECT_BLACK_UNICORN_FLESH = 20,
  EFFECT_GRAY_UNICORN_FLESH = 21,
  EFFECT_WHITE_UNICORN_FLESH = 22,
  EFFECT_TELEPORT_CONTROL = 23,
  EFFECT_MUSHROOM = 24,
  EFFECT_OMMEL_CERUMEN = 25,
  EFFECT_OMMEL_SWEAT = 26,
  EFFECT_OMMEL_TEARS = 27,
  EFFECT_OMMEL_SNOT = 28,
  EFFECT_OMMEL_BONE = 29,
  EFFECT_MUSTARD_GAS = 30,
  EFFECT_MUSTARD_GAS_LIQUID = 31
};

/* CEM = Consume End Message */

enum cem
{
  CEM_NOTHING = 0,
  CEM_SCHOOL_FOOD = 1,
  CEM_BONE = 2,
  CEM_FROG_FLESH = 3,
  CEM_OMMEL = 4,
  CEM_PEPSI = 5,
  CEM_KOBOLD_FLESH = 6,
  CEM_HEALING_LIQUID = 7,
  CEM_ANTIDOTE = 8,
  CEM_ESP = 9,
  CEM_HOLY_BANANA = 10,
  CEM_PEA_SOUP = 11,
  CEM_BLACK_UNICORN_FLESH = 12,
  CEM_GRAY_UNICORN_FLESH = 13,
  CEM_WHITE_UNICORN_FLESH = 14,
  CEM_OMMEL_BONE = 15
};

enum hitmessage
{
  HM_NOTHING = 0,
  HM_SCHOOL_FOOD = 1,
  HM_FROG_FLESH = 2,
  HM_OMMEL = 3,
  HM_PEPSI = 4,
  HM_KOBOLD_FLESH = 5,
  HM_HEALING_LIQUID = 6,
  HM_ANTIDOTE = 7,
  HM_CONFUSE = 8,
  HM_HOLY_BANANA = 9
};

enum attacktype
{
  UNARMED_ATTACK = 0,
  WEAPON_ATTACK = 1,
  KICK_ATTACK = 2,
  BITE_ATTACK = 3,
  THROW_ATTACK = 4
};

enum attackstyle
{
  USE_ARMS = 1,
  USE_LEGS = 2,
  USE_HEAD = 4
};

cint ATTRIBUTES = 11;
cint BASE_ATTRIBUTES = 7;

enum attribute
{
  ENDURANCE = 0,
  PERCEPTION = 1,
  INTELLIGENCE = 2,
  WISDOM = 3,
  WILL_POWER = 4,
  CHARISMA = 5,
  MANA = 6,

  ARM_STRENGTH = 7,
  LEG_STRENGTH = 8,
  DEXTERITY = 9,
  AGILITY = 10
};

enum attributeflag
{
  F_ENDURANCE = 1 << ENDURANCE,
  F_PERCEPTION = 1 << PERCEPTION,
  F_INTELLIGENCE = 1 << INTELLIGENCE,
  F_WISDOM = 1 << WISDOM,
  F_WILL_POWER = 1 << WILL_POWER,
  F_CHARISMA = 1 << CHARISMA,
  F_MANA = 1 << MANA,

  F_ARM_STRENGTH = 1 << ARM_STRENGTH,
  F_LEG_STRENGTH = 1 << LEG_STRENGTH,
  F_DEXTERITY = 1 << DEXTERITY,
  F_AGILITY = 1 << AGILITY
};

enum truthanswer
{
  NO = 0,
  YES = 1
};

cint REQUIRES_ANSWER = -1;

cint MAX_EQUIPMENT_SLOTS = 13;

enum equipmentindex
{
  HELMET_INDEX = 0,
  AMULET_INDEX = 1,
  CLOAK_INDEX = 2,
  BODY_ARMOR_INDEX = 3,
  BELT_INDEX = 4,
  RIGHT_WIELDED_INDEX = 5,
  LEFT_WIELDED_INDEX = 6,
  RIGHT_RING_INDEX = 7,
  LEFT_RING_INDEX = 8,
  RIGHT_GAUNTLET_INDEX = 9,
  LEFT_GAUNTLET_INDEX = 10,
  RIGHT_BOOT_INDEX = 11,
  LEFT_BOOT_INDEX = 12
};

cint WORLD_MAP = 255;

cuchar DEFAULT_TEAM = 0xFF;

/* Hard-coded teams */

enum teamid
{
  PLAYER_TEAM = 0,
  MONSTER_TEAM = 1,
  ATTNAM_TEAM = 2,
  SUMO_TEAM = 3,
  HOSTILE_ANGEL_TEAM = 4,
  DUNGEON_SHOP_TEAM = 5,
  IVAN_TEAM = 6,
  NEW_ATTNAM_TEAM = 7,
  COLONIST_TEAM = 8,
  TOURIST_GUIDE_TEAM = 9,
  TOURIST_TEAM = 10,
  BETRAYED_TEAM = 11,
  NO_TEAM = 0xFFFF
};

enum entityspawnflag
{
  LOAD = 1,
  NO_PIC_UPDATE = 2,
  NO_EQUIPMENT_PIC_UPDATE = NO_PIC_UPDATE << 1,
  NO_MATERIALS = 8,
  NO_EQUIPMENT = 16,
  NO_SIGNALS = 32,
};

enum squareflag
{
  NOT_WALKABLE = 1,
  HAS_CHARACTER = 2,
  IN_ROOM = 4,
  NOT_IN_ROOM = 8,
  ATTACHABLE = 16|NOT_IN_ROOM, /* overrides IN_ROOM */
  HAS_NO_OTERRAIN = 32
};

enum contentscriptflag
{
  DEFAULT_ATTACHED_AREA = 0xFE,
  DEFAULT_ATTACHED_ENTRY = 0xFE,
  NO_ENTRY = 0
};

enum dungeonindex
{
  RANDOM = 0,
  ELPURI_CAVE = 1,
  ATTNAM = 2,
  NEW_ATTNAM = 3,
  UNDER_WATER_TUNNEL = 4,
  UNDER_WATER_TUNNEL_EXIT = 0x80
};

enum levelindex
{
  VESANA_LEVEL = 2,
  CRYSTAL_LEVEL = 3,
  SPIDER_LEVEL = 4,
  ENNER_BEAST_LEVEL = 4,
  ZOMBIE_LEVEL = 5,
  IVAN_LEVEL = 7,
  DARK_LEVEL = 8,
  OREE_LAIR = 12
};

/* stack::DrawContents flags */

enum stackdrawflag
{
  NO_SELECT = 1,           // only show items
  NO_MULTI_SELECT = 2,     // select only one item
  NO_SPECIAL_INFO = 4,     // show only name and amount
  REMEMBER_SELECTED = 8,   // if DrawContents will be called multiple times,
                           // remember the selected item
  NONE_AS_CHOICE = 16,     // "none" is a choice, for instance when wielding
  SELECT_PAIR = 32         // if NO_MULTI_SELECT is on, selects a pair if
                           // appropriate
};

enum roomshape
{
  RECTANGLE = 1,
  ROUND_CORNERS = 2,
};

cint GODS = 15;

enum godid
{
  VALPURUS = 1,
  LEGIFER = 2,
  ATAVUS = 3,
  DULCIS = 4,
  SEGES = 5,
  SOPHOS = 6,
  TERRA = 7,
  SILVA = 7,
  LORICATUS = 8,
  MELLIS = 9,
  CLEPTIA = 10,
  NEFAS = 11,
  SCABIES = 12,
  INFUSCOR = 13,
  CRUENTUS = 14,
  MORTIFER = 15,
  ATHEIST = 16
};

clong MAX_PRICE = 2147483647L;

cint PERMANENT = 0xFFFF;

enum hitresult
{
  MISSED = 0,
  HIT = 1,
  CATCHED = 2
};

cint BEAM_EFFECTS = 13;

enum beameffect
{
  BEAM_POLYMORPH = 0,
  BEAM_STRIKE = 1,
  BEAM_FIRE_BALL = 2,
  BEAM_TELEPORT = 3,
  BEAM_HASTE = 4,
  BEAM_SLOW = 5,
  BEAM_RESURRECT = 6,
  BEAM_INVISIBILITY = 7,
  BEAM_DUPLICATE = 8,
  BEAM_LIGHTNING = 9,
  BEAM_DOOR_CREATION = 10,
  BEAM_ACID_RAIN = 11,
  BEAM_NECROMANCY = 12
};

cint BEAM_STYLES = 3;

enum beamstyle
{
  PARTICLE_BEAM = 0,
  LIGHTNING_BEAM = 1,
  SHIELD_BEAM = 2
};

clong RANDOM_COLOR = 0x10000;

/* Entry indices, not actual config defines */

enum entryindex
{
  UPSTAIRS = 100,
  DOWNSTAIRS = 200,
  FOUNTAIN = 0xFFFF
};

cint ALL_ITEMS = 0xFFFF;

enum statedataflag
{
  NO_FLAGS = 0,
  SECRET = 1,
  DUR_TEMPORARY = 2,
  DUR_PERMANENT = 4,
  DUR_FLAGS = 2|4,
  SRC_FOUNTAIN = 8,
  SRC_MUSHROOM = 16,
  SRC_MAGIC_MUSHROOM = 32,
  SRC_CONFUSE_READ = 64,
  SRC_EVIL = 128,
  SRC_GOOD = 256,
  SRC_FLAGS = 8|16|32|64|128|256,
  RANDOMIZABLE = DUR_FLAGS|SRC_FLAGS
};

enum mapcheatmode
{
  MAP_HIDDEN = 0,
  SHOW_MAP_IN_TRUE_LIGHT = 1,
  SHOW_MAP_IN_UNIFORM_LIGHT = 2
};

ccol24 DIM_LUMINANCE = 0x6E6E6E;

enum configmodifier
{
  BROKEN = 128,
  WINDOW = 1024
};

enum itemflag
{
  CANNIBALIZED = 4,
  SQUARE_POSITION_BITS = 16|32|64,
  SQUARE_POSITION_SHIFT = 4
};

enum bodypartflag
{
  UNIQUE = 128,
  BADLY_HURT = 256,
  STUCK = 512,
  BODYPART_SPARKLE_SHIFT = 9
};

enum configflag
{
  NO_BROKEN = 1,
  IGNORE_BROKEN_PRICE = 2
};

cint MAX_SQUARES_UNDER = 16;
cint MAX_NEIGHBOUR_SQUARES = 20;

enum lockconstant
{
  N_LOCK_ID = 1024,
  S_LOCK_ID = 16384,
  LOCK_DELTA = 1024
};

cint LOCK_BITS = 0xFC00;

cint BROKEN_LOCK = S_LOCK_ID;

enum locktype
{
  /* Normal lock types, which can be randomized */

  ROUND_LOCK = N_LOCK_ID + LOCK_DELTA * 1,
  SQUARE_LOCK = N_LOCK_ID + LOCK_DELTA * 2,
  TRIANGULAR_LOCK = N_LOCK_ID + LOCK_DELTA * 3,

  /* Special lock types, which must be generated in the script */

  HEXAGONAL_LOCK = S_LOCK_ID + LOCK_DELTA * 1,
  OCTAGONAL_LOCK = S_LOCK_ID + LOCK_DELTA * 2,
  HEART_SHAPED_LOCK = S_LOCK_ID + LOCK_DELTA * 3
};

enum leveltype
{
  DESERT = 1,
  JUNGLE = 2,
  STEPPE = 3,
  LEAFY_FOREST = 4,
  EVERGREEN_FOREST = 5,
  TUNDRA = 6,
  GLACIER = 7
};

enum movetype
{
  NO_MOVE = 0,
  WALK = 1,
  SWIM = 2,
  FLY = 4,
  ETHEREAL = 8,
  ANY_MOVE = 15
};

enum keyindex
{
  KEY_UP_INDEX = NORTH,
  KEY_LEFT_INDEX = WEST,
  KEY_RIGHT_INDEX = EAST,
  KEY_DOWN_INDEX = SOUTH
};

enum actionresult
{
  NO_ACTION = 0,
  SUCCESS = 1,
  BLOCKED = 2
};

enum slottype
{
  STACK_SLOT = 1,
  CHARACTER_SLOT = 2,
  GEAR_SLOT = 3
};

enum rustlevel
{
  NOT_RUSTED = 0,
  SLIGHTLY_RUSTED = 1,
  RUSTED = 2,
  VERY_RUSTED = 3
};

enum burnlevel
{
  NOT_BURNT = 0,
  SLIGHTLY_BURNT = 1,
  MODERATELY_BURNT = 2,
  HEAVILY_BURNT = 3
};

cint HUMAN_BODY_ARMOR_PIXELS = 68;

cpriority ARMOR_OUTLINE_PRIORITY = (7 << 4) + 7;
cpriority CLOAK_PRIORITY = (8 << 4) + 7;

cint BODY_ARMOR_PARTS = 6;

cv2 SUMO_ROOM_POS(25, 35);
cv2 SUMO_ARENA_POS(19, 12);

cint MAX_RAIN_DROPS = 32;

enum contestresult
{
  WON = 0,
  LOST = 1,
  DISQUALIFIED = 2
};

enum emitterconstant : ulong
{
  EMITTER_IDENTIFIER_BITS = 0xFFFF,
  EMITTER_SQUARE_PART_BITS = 0xF000000,
  EMITTER_SHADOW_BITS = 0xF0000000,
  EMITTER_SQUARE_PART_SHIFT = 24,
  EMITTER_SHADOW_SHIFT = 28,

  RE_SUN_EMITATED = 0x200000,
  ID_X_COORDINATE = 0x400000,
  ID_BEGIN = 0x800000,

  FORCE_ADD = 0x400000,
  SECONDARY_SUN_LIGHT = 0x800000
};

enum squaregraphicflag
{
  ALLOW_EMITATION_CONTINUE = 1,
  SQUARE_FREEZED = 2,
  INSIDE = 4,
  NEW_DRAW_REQUEST = 8,
  STRONG_BIT = 16,
  STRONG_NEW_DRAW_REQUEST = NEW_DRAW_REQUEST|STRONG_BIT,
  DESCRIPTION_CHANGE = 32,
  MEMORIZED_UPDATE_REQUEST = 128,
  IN_SQUARE_STACK = 256,
  CHECK_SUN_LIGHT_NEEDED = 512,
  IS_TRANSPARENT = 1024,
  PERFECTLY_QUADRI_HANDLED = 2048
};

/* Slows down protosystem::BalancedCreateItem() but makes it produce more
   accurate results */

cint BALANCED_CREATE_ITEM_ITERATIONS = 100;

cint CONFIG_TABLE_SIZE = 256;

cint SPARKLE_POS_X_ERROR = 128;

enum bodypartcolorflag
{
  SKIN_COLOR = 1,
  CAP_COLOR = 2,
  HAIR_COLOR = 4,
  EYE_COLOR = 8,
  TORSO_MAIN_COLOR = 16,
  BELT_COLOR = 32,
  BOOT_COLOR = 64,
  TORSO_SPECIAL_COLOR = 128,
  ARM_MAIN_COLOR = 256,
  GAUNTLET_COLOR = 512,
  ARM_SPECIAL_COLOR = 1024,
  LEG_MAIN_COLOR = 2048,
  LEG_SPECIAL_COLOR = 4096,
  CLOTH_COLOR = CAP_COLOR\
                 |TORSO_MAIN_COLOR\
                 |ARM_MAIN_COLOR\
                 |GAUNTLET_COLOR\
                 |LEG_MAIN_COLOR
};

enum charactercontentscriptflag
{
  IS_LEADER = 1,
  IS_MASTER = 2
};

enum stackflag
{
  /* If set, all items are always considered visible, so CanBeSeenBy calls
     become unneeded */

  STACK_HIDDEN = 1,

  /* All costly updates (like emitation's) are avoided if this is set.
     Allows much faster removing and adding items, but make sure the stack is
     returned to the original state (excluding item order) before switching
     this off. */

  STACK_FREEZED = 2
};

cint SUN_BEAM_DIRECTIONS = 48;

enum squarepartflag
{
  SP_TOP_LEFT = 1,
  SP_TOP_RIGHT = 2,
  SP_BOTTOM_LEFT = 4,
  SP_BOTTOM_RIGHT = 8,
  SP_TOP = SP_TOP_LEFT|SP_TOP_RIGHT,
  SP_LEFT = SP_TOP_LEFT|SP_BOTTOM_LEFT,
  SP_RIGHT = SP_TOP_RIGHT|SP_BOTTOM_RIGHT,
  SP_BOTTOM = SP_BOTTOM_LEFT|SP_BOTTOM_RIGHT
};

cint CONDITION_COLORS = 5;

cushort NATURAL_MATERIAL_FORM = 0x7FFF;

cdouble EXP_DIVISOR = 2e-8;
cdouble EXP_MULTIPLIER = 5e+7;
cdouble MIN_EXP = 5e+7;
cdouble MAX_EXP = 5e+10;

enum characterdatabaseflag
{
  HAS_BEEN_GENERATED = 1,
  HAS_BEEN_SEEN = 2
};

cint DEPENDS_ON_ATTRIBUTES = 0xFFFF;

enum tiredness
{
  FAINTING = 0,
  EXHAUSTED = 1,
  UNTIRED = 2
};

cdouble DEFAULT_GENERATION_DANGER = 0.05;
cdouble ANGEL_GENERATION_DANGER = 0.10;

enum duplicationflag
{
  MIRROR_IMAGE = 1,
  IGNORE_PROHIBITIONS = 2,
  CHANGE_TEAM = 4,
  LE_BASE_SHIFT = 3,
  LE_BASE_RANGE = 0x7FFF,
  LE_RAND_SHIFT = 18,
  LE_RAND_RANGE = 0x3FFF
};

enum actionflag
{
  IN_DND_MODE = 1,
  TERMINATING = 2
};

enum fluidflag
{
  HAS_BODY_ARMOR_PICTURES = 1,
  FLUID_INSIDE = 2
};

cint COMMAND_FLAGS = 4;

enum commandflag
{
  FOLLOW_LEADER = 1,
  FLEE_FROM_ENEMIES = 2,
  DONT_CHANGE_EQUIPMENT = 4,
  DONT_CONSUME_ANYTHING_VALUABLE = 8,
  ALL_COMMAND_FLAGS = 1|2|4|8
};

cint CHAT_MENU_ENTRIES = 5;

enum managementflag
{
  CHANGE_EQUIPMENT = 1,
  TAKE_ITEMS = 2,
  GIVE_ITEMS = 4,
  ISSUE_COMMANDS = 8,
  CHAT_IDLY = 16,
  ALL_MANAGEMENT_FLAGS = 1|2|4|8|16
};

cuchar NO_PARAMETERS = 0xFF;

cint CURSOR_TYPES = 4;

enum cursortype
{
  DARK_CURSOR = 0,
  RED_CURSOR = 1,
  BLUE_CURSOR = 2,
  YELLOW_CURSOR = 3
};

enum cursorflag
{
  CURSOR_FLASH = 0x2000,
  CURSOR_TARGET = 0x4000,
  CURSOR_BIG = 0x8000,
  CURSOR_FLAGS = CURSOR_BIG|CURSOR_FLASH|CURSOR_TARGET
};

enum colortype
{
  GRAY_FRACTAL = 0,
  RED_FRACTAL = 1,
  GREEN_FRACTAL = 2,
  BLUE_FRACTAL = 3,
  YELLOW_FRACTAL = 4
};

cint PHYSICAL_DAMAGE_TYPES = 3;

enum physicaldamagetype
{
  BLUNT = 1,
  SLASH = 2,
  PIERCE = 4
};

cint SILHOUETTE_TYPES = 2;

enum silhouettetype
{
  SILHOUETTE_NORMAL = 0,
  SILHOUETTE_INTER_LACED = 1
};

enum warnflag
{
  WARNED = 1,
  HAS_CAUSED_PANIC = 2
};

/* MaxHP calculation flags */

enum hpflag
{
  MAY_CHANGE_HPS = 1,
  CHECK_USABILITY = 2
};

enum traptype
{
  ITEM_TRAP = 0x8000,
  FLUID_TRAP = 0x10000
};

enum deathflag
{
  FORCE_MSG = 1,
  FORCE_DEATH = 2,
  DISALLOW_CORPSE = 4,
  DISALLOW_MSG = 8,
  IGNORE_UNCONSCIOUSNESS = 16,
  IGNORE_TRAPS = 32,
  FORBID_REINCARNATION = 64
};

enum characterflag
{
  C_PLAYER = 4,
  C_INITIALIZING = 8,
  C_POLYMORPHED = 16,
  C_IN_NO_MSG_MODE = 32,
  C_PICTURE_UPDATES_FORBIDDEN = 64
};

/*************************/
/* Common DataBase flags */
/*************************/

enum commonflag
{
  IS_ABSTRACT = 1,
  HAS_SECONDARY_MATERIAL = 2,
  CREATE_DIVINE_CONFIGURATIONS = 4,
  CAN_BE_WISHED = 8,
  CAN_BE_DESTROYED = 16,
  IS_VALUABLE = 32,
  CAN_BE_MIRRORED = 64
};

enum nameflag
{
  USE_AN = 1,
  USE_ADJECTIVE_AN = 2,
  NO_ARTICLE = 4, // for instance "Petrus's wive number 4"
  FORCE_THE = 8,
  SHOW_MATERIAL = 16 // only works for terrains
};

/****************************/
/* Character DataBase flags */
/****************************/

/* CommonFlags */
/* NameFlags */

enum characterbodyflag
{
  HAS_HEAD = 1,
  HAS_EYES = 2,
  HAS_A_LEG = 4,
  SPILLS_BLOOD = 8,
  SWEATS = 16,
  USES_NUTRITION = 32,
  ALWAYS_USE_MATERIAL_ATTRIBUTES = 64,
  IS_ENORMOUS = 128,
  IS_EXTRA_FRAGILE = 256,
  IS_PLANT = 512,
  IS_ROOTED = 1024
};

enum characterabilityflag
{
  CAN_USE_EQUIPMENT = 1,
  CAN_KICK = 2,
  CAN_TALK = 4,
  CAN_READ = 8,
  CAN_OPEN = 16,
  CAN_ZAP = 32,
  CAN_THROW = 64,
  CAN_APPLY = 128,
  CAN_HEAR = 256
};

enum charactercopyrightflag
{
  IS_UNIQUE = 1,
  CAN_BE_GENERATED = 2,
  CAN_BE_NAMED = 4
};

enum charactereffectflag
{
  BODY_PARTS_DISAPPEAR_WHEN_SEVERED = 1,
  DESTROYS_WALLS = 2,
  BITE_CAPTURES_BODY_PART = 4
};

enum characterimmunityflag
{
  IMMUNITY_POLYMORPH = 1,
  IMMUNITY_CHARM = 2,
  IMMUNITY_CLONING = 4,
  IMMUNITY_CONFUSE = 8,
  IMMUNITY_LEPROSY = 16,
  IMMUNITY_ITEM_TELEPORT = 32,
  IMMUNITY_STICKINESS = 64,
  IMMUNITY_CHOKING = 128,
  IMMUNITY_UNCONSCIOUSNESS = 256
};

enum charactermiscflag
{
  CREATE_GOLEM_MATERIAL_CONFIGURATIONS = 2,
  IGNORE_DANGER = 4,
  AUTOMATICALLY_SEEN = 8,
  WILL_CARRY_ITEMS = 16,
  IS_EXTRA_COWARD = 32
};

/***********************/
/* Item DataBase flags */
/***********************/

/* CommonFlags */
/* NameFlags */
/* AttributeAffectFlags */

enum itemgenerationflag
{
  CREATE_ITEM_LOCK_CONFIGURATIONS = 2,
  CAN_BE_AUTO_INITIALIZED = 4, // used only in WMode
  CAN_BE_GENERATED_IN_CONTAINER = 8,
  CAN_BE_SPAWNED_BY_POLYMORPH = 16
};

enum iteminteractionflag
{
  MATERIAL_CAN_BE_CHANGED = 1,
  CAN_BE_POLYMORPHED = 2,
  CAN_BE_CLONED = 4,
  CAN_BE_ENCHANTED = 8,
  CAN_BE_BROKEN = 16,
  AFFECTS_CARRYING_CAPACITY = 32
};

enum itemcategoryflag
{
  IS_QUEST_ITEM = 1,
  CAN_BE_USED_BY_SMITH = 2,
  IS_KAMIKAZE_WEAPON = 4,
  IS_TWO_HANDED = 8,
  IS_GOOD_WITH_PLANTS = 16
};

enum itemmiscflag
{
  HANDLE_IN_PAIRS = 1,
  PRICE_IS_PROPORTIONAL_TO_ENCHANTMENT = 2,
  FLEXIBILITY_IS_ESSENTIAL = 4,
  HAS_NORMAL_PICTURE_DIRECTION = 8,
  CAN_BE_PILED = 16,
  CAN_BE_PICKED_UP = 32,
  ALLOW_EQUIP = 64
};

/**************************/
/* Terrain DataBase flags */
/**************************/

/* CommonFlags */
/* NameFlags */

enum olterrainflag
{
  CREATE_OLTERRAIN_LOCK_CONFIGURATIONS = 2,
  CREATE_WINDOW_CONFIGURATIONS = 4,
  IS_UP_LINK = 8,
  IS_WALL = 16,
  USE_BORDER_TILES = 32,
  IS_ALWAYS_TRANSPARENT = 64,
  SHOW_THINGS_UNDER = 128,
  IS_SAFE_TO_CREATE_DOOR = 256
};

/***************************/
/* Material DataBase flags */
/***************************/

/* CommonFlags */
/* NameFlags (only USE_AN) */

enum materialcategoryflag
{
  IS_METAL = 1,
  IS_BLOOD = 2,
  CAN_BE_TAILORED = 4,
  IS_SPARKLING = 8,
  IS_SCARY = 16,
  IS_GOLEM_MATERIAL = 32,
  IS_BEVERAGE = 64
};

enum materialbodyflag
{
  IS_ALIVE = 1,
  IS_WARM = 2,
  CAN_HAVE_PARASITE = 4,
  USE_MATERIAL_ATTRIBUTES = 8,
  CAN_REGENERATE = 16
};

enum materialinteractionflag
{
  CAN_BURN = 1,
  CAN_EXPLODE = 2,
  CAN_DISSOLVE = 4,
  AFFECT_INSIDE = 8,
  EFFECT_IS_GOOD = 16,
  IS_AFFECTED_BY_MUSTARD_GAS = 32,
  RISES_FROM_ASHES = 64
};

/*************************/
/* End of DataBase flags */
/*************************/

cint TILE_SIZE = 16;
cv2 TILE_V2(TILE_SIZE, TILE_SIZE);

enum blitdataflag
{
  SQUARE_INDEX_MASK = 0xFFFF,
  ALLOW_ANIMATE = 0x10000,
  ALLOW_ALPHA = 0x20000
};

cint TALENTS = 4;

enum talent
{
  TALENT_STRONG,
  TALENT_FAST_N_ACCURATE,
  TALENT_HEALTHY,
  TALENT_CLEVER
};

culong BORDER_PARTNER_ANIMATED = 16 << 24;

enum roomflag
{
  NO_MONSTER_GENERATION = 1
};

cint NO_TAMING = -1;

cint SADIST_HIT = 1;

cint EQUIPMENT_DATAS = 13;

cint SPECIAL_CONFIGURATION_GENERATION_LEVELS = 2;

#endif
