#ifndef __CHARDE_H__
#define __CHARDE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "charba.h"
#include "materde.h"
#include "femath.h"
#include "proto.h"
#include "wskill.h"
#include "game.h" // remove this

class head;
class arm;
class rightarm;
class leftarm;
class groin;
class rightleg;
class leftleg;
class humanoidtorso;

class ABSTRACT_CHARACTER
(
  humanoid,
  character,
 public:
  virtual ~humanoid();
  virtual void VirtualConstructor();
  virtual void Load(inputfile&);
  //virtual uchar GetSex() const { return MALE; }
  virtual void Save(outputfile&) const;
  virtual bool CanWield() const { return true; }
  //virtual bool CanWear() const { return true; }
  virtual bool Hit(character*);
  virtual gweaponskill* GetCategoryWeaponSkill(ushort Index) const { return CategoryWeaponSkill[Index]; }
  virtual void CharacterSpeciality(ushort = 1);
  virtual bool ShowWeaponSkills();
  virtual long GetStatScore() const;
  virtual void AddSpecialItemInfo(std::string&, item*);
  virtual void AddSpecialItemInfoDescription(std::string&);
  virtual void KickHit();
  //static bool CanBeGenerated() { return false; }
  virtual ushort GetSize() const;

  virtual head* GetHead() const;
  virtual void SetHead(head* What);
  virtual rightarm* GetRightArm() const;
  virtual void SetRightArm(rightarm* What);
  virtual leftarm* GetLeftArm() const;
  virtual void SetLeftArm(leftarm* What);
  virtual groin* GetGroin() const;
  virtual void SetGroin(groin* What);
  virtual rightleg* GetRightLeg() const;
  virtual void SetRightLeg(rightleg* What);
  virtual leftleg* GetLeftLeg() const;
  virtual void SetLeftLeg(leftleg* What);

  virtual humanoidtorso* GetHumanoidTorso() const;
  virtual void SetHumanoidTorso(humanoidtorso* What);

  virtual arm* GetMainArm() const;
  virtual arm* GetSecondaryArm() const;

  virtual void DrawToTileBuffer(bool) const;

  virtual bool ReceiveDamage(character*, short, uchar, uchar = ALL, uchar = 8, bool = false, bool = false, bool = false);

  virtual bool BodyPartVital(ushort);

  virtual bool BodyPartCanBeSevered(ushort) const;

  virtual item* GetMainWielded() const;
  virtual item* GetSecondaryWielded() const;

  virtual void SetMainWielded(item*);
  virtual void SetSecondaryWielded(item*);

  //virtual bool CanWieldInMainHand() const;
  //virtual bool CanWieldInSecondaryHand() const;

  virtual arm* GetMainWeaponArm() const;
  virtual arm* GetSecondaryWeaponArm() const;
  virtual void MainHit(character*);
  virtual void SecondaryHit(character*);

  virtual float GetMainAttackStrength() const;
  virtual float GetSecondaryAttackStrength() const;

  virtual float GetMainToHitValue() const;
  virtual float GetSecondaryToHitValue() const;

  /*virtual characterslot* GetHeadSlot() const { return GetBodyPartSlot(1); }
  virtual characterslot* GetRightArmSlot() const { return GetBodyPartSlot(2); }
  virtual characterslot* GetLeftArmSlot() const { return GetBodyPartSlot(3); }
  virtual characterslot* GetGroinSlot() const { return GetBodyPartSlot(4); }
  virtual characterslot* GetRightLegSlot() const { return GetBodyPartSlot(5); }
  virtual characterslot* GetLeftLegSlot() const { return GetBodyPartSlot(6); }*/

  virtual std::string EquipmentName(ushort) const;
  virtual bodypart* GetBodyPartOfEquipment(ushort) const;
  virtual item* GetEquipment(ushort) const;
  virtual ushort EquipmentSlots() const { return 13; }

  virtual item* GetHelmet() const;
  virtual item* GetAmulet() const;
  virtual item* GetCloak() const;
  virtual item* GetBodyArmor() const;
  virtual item* GetBelt() const;
  virtual item* GetRightWielded() const;
  virtual item* GetLeftWielded() const;
  virtual item* GetRightRing() const;
  virtual item* GetLeftRing() const;
  virtual item* GetRightGauntlet() const;
  virtual item* GetLeftGauntlet() const;
  virtual item* GetRightBoot() const;
  virtual item* GetLeftBoot() const;
  virtual void SwitchToDig(item*, vector2d);
  virtual uchar GetLegs() const;
  virtual uchar GetArms() const;
  virtual bool CheckKick() const;
  virtual float GetAPStateMultiplier() const;
  virtual short GetLengthOfOpen(vector2d) const;
  virtual short GetLengthOfClose(vector2d) const;
  virtual bool CheckThrow() const;
  virtual bool CheckOffer() const;

  virtual void SetHelmet(item*);
  virtual void SetAmulet(item*);
  virtual void SetCloak(item*);
  virtual void SetBodyArmor(item*);
  virtual void SetBelt(item*);
  virtual void SetRightWielded(item*);
  virtual void SetLeftWielded(item*);
  virtual void SetRightRing(item*);
  virtual void SetLeftRing(item*);
  virtual void SetRightGauntlet(item*);
  virtual void SetLeftGauntlet(item*);
  virtual void SetRightBoot(item*);
  virtual void SetLeftBoot(item*);

  virtual bool (*EquipmentSorter(ushort) const)(item*, character*);
  virtual void SetEquipment(ushort, item*);
  virtual bool DrawSilhouette(bitmap*, vector2d);
  virtual ushort GlobalResistance(uchar) const;
  virtual void AddInfo(felist&) const;
  virtual void CompleteRiseFromTheDead();
  //virtual void CreateBodyPart(ushort);
  virtual bool HandleNoBodyPart(ushort);

  virtual bool CheckWearEquipment() const { return true; }

 protected:
  virtual vector2d GetBodyPartBitmapPos(ushort, ushort);
  virtual ushort GetBodyPartColor1(ushort, ushort);
  virtual ushort GetBodyPartColor2(ushort, ushort);
  virtual ushort GetBodyPartColor3(ushort, ushort);

  /*virtual vector2d GetHeadBitmapPos() const { return vector2d(96, 0); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 0); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 0); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0, 0); }
  virtual vector2d GetRightArmBitmapPos() const { return GetArmBitmapPos(); }
  virtual vector2d GetLeftArmBitmapPos() const { return GetArmBitmapPos(); }
  virtual vector2d GetRightLegBitmapPos() const { return GetLegBitmapPos(); }
  virtual vector2d GetLeftLegBitmapPos() const { return GetLegBitmapPos(); }
  virtual vector2d GetGroinBitmapPos() const { return GetLegBitmapPos(); }

  virtual ushort SkinColor() const { return MAKE_RGB(180, 120, 90); }

  virtual ushort CapColor() const { return ClothColor(); }
  virtual ushort HairColor() const { return MAKE_RGB(160, 80, 0); }
  virtual ushort EyeColor() const { return MAKE_RGB(112, 72, 42); }

  virtual ushort TorsoMainColor() const { return ClothColor(); }
  virtual ushort BeltColor() const { return MAKE_RGB(48, 48, 48); }
  virtual ushort TorsoSpecialColor() const { return MAKE_RGB(0, 0, 0); }

  virtual ushort ArmMainColor() const { return ClothColor(); }
  virtual ushort ArmSpecialColor() const { return MAKE_RGB(0, 0, 0); }

  virtual ushort LegMainColor() const { return ClothColor(); }
  virtual ushort LegSpecialColor() const { return MAKE_RGB(0, 0, 0); }

  virtual ushort ClothColor() const { return MAKE_RGB(111, 74, 37); }*/

  /*virtual void CreateHead();
  virtual void CreateTorso();
  virtual void CreateRightArm();
  virtual void CreateLeftArm();
  virtual void CreateGroin();
  virtual void CreateRightLeg();
  virtual void CreateLeftLeg();*/
  //virtual void UpdateBodyPartPictures(bool = true);
  /*virtual void UpdateHeadPicture(bool = true);
  virtual void UpdateTorsoPicture(bool = true);
  virtual void UpdateRightArmPicture(bool = true);
  virtual void UpdateLeftArmPicture(bool = true);
  virtual void UpdateGroinPicture(bool = true);
  virtual void UpdateRightLegPicture(bool = true);
  virtual void UpdateLeftLegPicture(bool = true);*/
  /*virtual material* CreateHeadFlesh(ulong Volume) const { return CreateTorsoFlesh(Volume); }
  virtual material* CreateHeadBone(ulong Volume) const { return CreateTorsoBone(Volume); }
  virtual material* CreateRightArmFlesh(ulong Volume) const { return CreateArmFlesh(Volume); }
  virtual material* CreateRightArmBone(ulong Volume) const { return CreateArmBone(Volume); }
  virtual material* CreateLeftArmFlesh(ulong Volume) const { return CreateArmFlesh(Volume); }
  virtual material* CreateLeftArmBone(ulong Volume) const { return CreateArmBone(Volume); }
  virtual material* CreateArmFlesh(ulong Volume) const { return CreateTorsoFlesh(Volume); }
  virtual material* CreateArmBone(ulong Volume) const { return CreateTorsoBone(Volume); }
  virtual material* CreateGroinFlesh(ulong Volume) const { return CreateLegFlesh(Volume); }
  virtual material* CreateGroinBone(ulong Volume) const { return CreateLegBone(Volume); }
  virtual material* CreateRightLegFlesh(ulong Volume) const { return CreateLegFlesh(Volume); }
  virtual material* CreateRightLegBone(ulong Volume) const { return CreateLegBone(Volume); }
  virtual material* CreateLeftLegFlesh(ulong Volume) const { return CreateLegFlesh(Volume); }
  virtual material* CreateLeftLegBone(ulong Volume) const { return CreateLegBone(Volume); }
  virtual material* CreateLegFlesh(ulong Volume) const { return CreateTorsoFlesh(Volume); }
  virtual material* CreateLegBone(ulong Volume) const { return CreateTorsoBone(Volume); }*/
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new humanflesh(Volume); }
  /*virtual uchar HeadBonePercentile() const { return 20; }
  virtual uchar TorsoBonePercentile() const { return 10; }
  virtual uchar RightArmBonePercentile() const { return ArmBonePercentile(); }
  virtual uchar LeftArmBonePercentile() const { return ArmBonePercentile(); }
  virtual uchar ArmBonePercentile() const { return 30; }
  virtual uchar GroinBonePercentile() const { return 40; }
  virtual uchar RightLegBonePercentile() const { return LegBonePercentile(); }
  virtual uchar LeftLegBonePercentile() const { return LegBonePercentile(); }
  virtual uchar LegBonePercentile() const { return 30; }*/
  /*virtual ulong GetHeadVolume() const;
  virtual ulong GetTorsoVolume() const;
  virtual ulong GetRightArmVolume() const { return GetArmVolume(); }
  virtual ulong GetLeftArmVolume() const { return GetArmVolume(); }
  virtual ulong GetArmVolume() const;
  virtual ulong GetGroinVolume() const;
  virtual ulong GetRightLegVolume() const { return GetLegVolume(); }
  virtual ulong GetLeftLegVolume() const { return GetLegVolume(); }
  virtual ulong GetLegVolume() const;*/

  /*virtual ushort GetHeadSize(ushort) const;
  virtual ushort GetTorsoSize(ushort) const;
  virtual ushort GetRightArmSize(ushort TotalSize) const { return GetArmSize(TotalSize); }
  virtual ushort GetLeftArmSize(ushort TotalSize) const { return GetArmSize(TotalSize); }
  virtual ushort GetArmSize(ushort) const;
  virtual ushort GetGroinSize(ushort) const;
  virtual ushort GetRightLegSize(ushort TotalSize) const { return GetLegSize(TotalSize); }
  virtual ushort GetLeftLegSize(ushort TotalSize) const { return GetLegSize(TotalSize); }
  virtual ushort GetLegSize(ushort) const;*/

  virtual ulong GetBodyPartSize(ushort, ushort);
  virtual ulong GetBodyPartVolume(ushort);
  virtual uchar GetBodyPartBonePercentile(ushort);
  virtual bodypart* MakeBodyPart(ushort);

  virtual uchar BodyParts() const { return 7; }

  //virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(0,0); } // remove this
  ////virtual float GetMeleeStrength() const { return 1000; }
  virtual std::string GetDeathMessage() { return Name(DEFINITE) + " dies screaming."; }
  gweaponskill* CategoryWeaponSkill[WEAPON_SKILL_GATEGORIES];
);

class CHARACTER
(
  human,
  humanoid,
  {
    SetAgility(15 + RAND() % 11);
    SetStrength(10 + RAND() % 6);
    SetEndurance(10 + RAND() % 6);
    SetPerception(10 + RAND() % 6);
    SetMoney(200 + RAND() % 101);
  },
 public:
 protected:
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, 0); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 0); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 0); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0, 0); }
  virtual ulong TotalVolume() const { return 60000; }
  virtual std::string NameSingular() const { return "human"; }
  virtual ushort TotalSize() const { return 180; }
);

class CHARACTER
(
  petrus,
  humanoid,
  {
    SetAgility(75);
    SetStrength(75);
    SetEndurance(75);
    SetPerception(75);
    SetHealTimer(100);
    SetStoryState(0);
    game::SetPetrus(this);
    SetAssignedName("Petrus");
  },
 public:
  virtual ~petrus();
  virtual void Load(inputfile&);
  virtual void BeTalkedTo(character*);
  virtual ushort GetEmitation() const { return 333; }
  virtual void HealFully(character*);
  virtual void SetHealTimer(ushort What) { HealTimer = What; }
  virtual ushort GetHealTimer() { return HealTimer; }
  virtual void Save(outputfile&) const;
  virtual bool Charmable() const { return false; }
  virtual bool Polymorph(character* Char, ushort) { delete Char; return false; }
  virtual uchar GetStoryState() const { return StoryState; }
  virtual void SetStoryState(uchar What) { StoryState = What; }
  virtual bool HasInfraVision() const { return true; }
  virtual bool CanBeDisplaced() const { return false; }
  virtual void CreateInitialEquipment();
  virtual bool CanBeAssignedAName() const { return false; }
  virtual bool IsUnique() const { return true; }
 protected:
  virtual ushort HairColor() const { return MAKE_RGB(160, 160, 160); }
  virtual ushort ClothColor() const { return MAKE_RGB(48, 48, 48); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, 144); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 80); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 16); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0,0); }
  virtual ulong TotalVolume() const { return 80000; }
  virtual std::string NameSingular() const { return "High Priest of the Great Frog"; }
  virtual void CreateCorpse();
  virtual std::string GetDeathMessage() { return "The High Priest disappears in a bright light and only his left nut is left behind."; }
  //virtual float GetMeleeStrength() const { return 20000; }
  virtual void GetAICommand();
  virtual ushort TotalSize() const { return 225; }
  ushort HealTimer;
  uchar StoryState;
);

class CHARACTER
(
  farmer,
  humanoid,
  {
    SetAgility(10);
    SetStrength(15);
    SetEndurance(20);
    SetPerception(18);
    SetMoney(20);
  },
 public:
  virtual void CreateInitialEquipment();
  virtual void BeTalkedTo(character*);
 protected:
  virtual ushort TorsoSpecialColor() const { return MAKE_RGB(0, 96, 0); }
  virtual ushort GetHairColor(ushort Frame) const { return RAND() % 2 ? humanoid::GetHairColor(Frame) : MAKE_RGB(160, 160, 160); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, (4 + RAND() % 2) * 16); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 32); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, (RAND() % 2) * 16); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0,(RAND() % 3) * 16); }
  virtual ulong TotalVolume() const { return 30000; }
  virtual std::string NameSingular() const { return "farmer"; }
  virtual ushort TotalSize() const { return 170; }
);

class CHARACTER
(
  guard,
  humanoid,
  {
    SetAgility(15);
    SetStrength(20);
    SetEndurance(20);
    SetPerception(24);
  },
 public:
  virtual void GetAICommand() { StandIdleAI(); }
  virtual void CreateInitialEquipment();
  virtual void BeTalkedTo(character*);
 protected:
  virtual ushort ClothColor() const { return MAKE_RGB(0, 128, 128); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, 112); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 128); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 80); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0, 64); }
  virtual ulong TotalVolume() const { return 60000; }
  virtual std::string NameSingular() const { return "guard"; }
  //virtual float GetMeleeStrength() const { return 2000; }
  virtual ushort TotalSize() const { return 180; }
);

class CHARACTER
(
  shopkeeper,
  humanoid,
  {
    SetAgility(10);
    SetStrength(30);
    SetEndurance(25);
    SetPerception(30);
    SetMoney(3000 + RAND() % 2001);
  },
 public:
  virtual void GetAICommand() { StandIdleAI(); }
  virtual void CreateInitialEquipment();
  virtual void BeTalkedTo(character*);
  virtual bool Polymorph(character* Char, ushort) { delete Char; return false; }
 protected:
  virtual ushort TorsoSpecialColor() const { return MAKE_RGB(0, 96, 0); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, 64); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 32); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 16); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0, 32); }
  virtual ulong TotalVolume() const { return 100000; }
  virtual std::string NameSingular() const { return "shopkeeper"; }
  //virtual float GetMeleeStrength() const { return 2000; }
  virtual ushort TotalSize() const { return 160; }
);

class CHARACTER
(
  priest,
  humanoid,
  {
    SetAgility(10);
    SetStrength(20);
    SetEndurance(15);
    SetPerception(18);
  },
 public:
  virtual void GetAICommand() { StandIdleAI(); }
  virtual void CreateInitialEquipment();
  virtual void BeTalkedTo(character*);
  virtual bool CanSwim() const { return true; }
 protected:
  virtual ushort CapColor() const { return MAKE_RGB(180, 0, 80); }
  virtual ushort TorsoSpecialColor() const { return MAKE_RGB(0, 96, 0); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, 128); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 32); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 16); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0, 32); }
  virtual ulong TotalVolume() const { return 100000; }
  virtual std::string NameSingular() const { return "priest"; }
  virtual ushort TotalSize() const { return 180; }
);

class CHARACTER
(
  oree,
  humanoid,
  {
    SetAgility(50);
    SetStrength(30);
    SetEndurance(30);
    SetPerception(30);
    SetAssignedName("Oree");
  },
 public:
  virtual bool Charmable() const { return false; }
  virtual bool Polymorph(character* Char, ushort) { delete Char; return false; }
  virtual ulong GetBloodColor() const { return BLACK; }
  virtual void BeTalkedTo(character*);
  virtual bool HasInfraVision() const { return true; }
  virtual bool CanBeDisplaced() const { return false; }
  virtual void CreateInitialEquipment();
  virtual bool CanBeAssignedAName() const { return false; }
  virtual bool IsUnique() const { return true; }
 protected:

  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, 32); } 
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(48, 160); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(80, 160); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(16, 144); }

  virtual ushort ClothColor() const { return MAKE_RGB(30, 30, 30); }
  //virtual ushort SkinColor() const { return MAKE_RGB(160, 0, 0); }
  virtual ushort EyeColor() const { return MAKE_RGB(64, 0, 0); }

  virtual ulong TotalVolume() const { return 110000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new daemonflesh(Volume); }
  virtual std::string GetDeathMessage() { return Name(DEFINITE) + "vomits blood for one last time and then dies."; }
  virtual std::string NameSingular() const { return "Blood Daemon King"; }
  //virtual float GetMeleeStrength() const { return 30000; }
  virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBloodVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBloodVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBloodVerb(Critical); }
  virtual ushort TotalSize() const { return 225; }
);

class CHARACTER
(
  darkknight,
  humanoid,
  {
    SetAgility(30);
    SetStrength(30);
    SetEndurance(30);
    SetPerception(30);
  },
 public:
  static bool CanBeGenerated() { return true; }
  virtual void BeTalkedTo(character*);
  virtual void CreateInitialEquipment();
 protected:
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, 240); } 
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 176); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 16); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0, 64); }
  virtual ushort ClothColor() const { return MAKE_RGB(0,0,30); }
  virtual ushort EyeColor() const { return MAKE_RGB(100, 0, 0); }
  virtual ulong TotalVolume() const { return 110000; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(128,0); }
  virtual std::string NameSingular() const { return "dark knight"; }
  //virtual float GetMeleeStrength() const { return 5000; }
  virtual ushort TotalSize() const { return 200; }
);

class CHARACTER
(
  ennerbeast,
  character,
  {
    SetAgility(10);
    SetStrength(10);
    SetEndurance(25);
    SetPerception(12);
  },
 public:
  virtual bool CanOpen() const { return false; }
  static bool CanBeGenerated() { return false; }
  virtual bool Hit(character*);
  virtual bool Polymorph(character* Char, ushort) { delete Char; return false; }
  virtual void BeTalkedTo(character*);
  virtual ulong CurrentDanger() const { return character::CurrentDanger() * 5; }
  virtual ulong MaxDanger() const { return character::MaxDanger() * 5; }
 protected:
  //virtual void CreateHead();
  virtual std::string Article() const { return "an"; }
  virtual ulong TotalVolume() const { return 30000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new ennerbeastflesh(Volume); }
  virtual std::string GetDeathMessage() { return Name(DEFINITE) + " dies and the world is finally freed from this terrible monster."; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(96,0); }
  virtual std::string NameSingular() const { return "enner beast"; }
  //virtual float GetMeleeStrength() const { return 100000; }
  virtual void GetAICommand();
  virtual ushort TotalSize() const { return 150; }
);

class ABSTRACT_CHARACTER
(
  frog,
  character,
 public:
  virtual bool CanOpen() const { return false; }
 protected:
  virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBiteVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  //virtual float GetMeleeStrength() const { return 20000; }
  virtual std::string TalkVerb() const { return "croaks"; }
);

class CHARACTER
(
  darkfrog,
  frog,
  {
    SetAgility(30);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(18);
  },
 public:
  virtual ulong GetBloodColor() const { return BLACK; }
  virtual bool HasInfraVision() const { return true; }
 protected:
  virtual ulong TotalVolume() const { return 2500; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new darkfrogflesh(Volume); }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(80,0); }
  virtual std::string NameSingular() const { return "dark frog"; }
  virtual ushort TotalSize() const { return 25; }
);

class CHARACTER
(
  elpuri,
  darkfrog,
  {
    SetAgility(10);
    SetStrength(30);
    SetEndurance(50);
    SetPerception(30);
    SetAssignedName("Elpuri");
  },
 public:
  static bool CanBeGenerated() { return false; }
  virtual bool Charmable() const { return false; }
  virtual bool Polymorph(character* Char, ushort) { delete Char; return false; }
  virtual ulong GetBloodColor() const { return BLACK; }
  virtual void BeTalkedTo(character*);
  virtual bool Hit(character*);
  virtual bool CanBeDisplaced() const { return false; }
  virtual bool CanBeAssignedAName() const { return false; }
  virtual bool CanOpen() const { return true; }
  virtual bool IsUnique() const { return true; }
 protected:
  virtual void CreateCorpse();
  virtual ulong TotalVolume() const { return 277500; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new elpuriflesh(Volume); }
  virtual std::string GetDeathMessage() { return Name(DEFINITE) + " groans horribly and drops " + PossessivePronoun() + " head."; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(64,0); }
  virtual std::string NameSingular() const { return "Master Dark Frog"; }
  virtual ushort TotalSize() const { return 300; }
);

class CHARACTER
(
  billswill,
  character,
  {
    SetAgility(40);
    SetStrength(5);
    SetEndurance(20);
    SetPerception(27);
  },
 public:
  virtual void SpillBlood(uchar) { }
  virtual void SpillBlood(uchar, vector2d) { }
  virtual void BeTalkedTo(character*);
  virtual bool HasInfraVision() const { return true; }
  virtual std::string StandVerb() const { return "floating"; }
  static ushort Frequency() { return 100; }
  virtual bool CanFly() const { return true; }
 protected:
  virtual ulong TotalVolume() const { return 500000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new air(Volume); }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(48,0); }
  virtual std::string NameSingular() const { return "pure mass of Bill's will"; }
  virtual std::string NamePlural() const { return "pure masses of Bill's will"; }
  virtual void CreateCorpse() { SetExists(false); }
  virtual std::string GetDeathMessage() { return Name(DEFINITE) + " vanishes from existence."; }
  //virtual float GetMeleeStrength() const { return 20000; }
  virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonPSIVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonPSIVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonPSIVerb(Critical); }
  virtual ushort TotalSize() const { return 100; }
);

class CHARACTER
(
  skeleton,
  humanoid,
  {
    SetAgility(10);
    SetStrength(5);
    SetEndurance(10);
    SetPerception(15);
  },
 public:
  static bool CanBeGenerated() { return true; }
  virtual void SpillBlood(uchar) { }
  virtual void SpillBlood(uchar, vector2d) { }
  virtual void BeTalkedTo(character*);
  virtual void CreateInitialEquipment();
  virtual uchar GetSex() const { return MALE + RAND() % 2; }
 protected:
  //virtual ushort SkinColor() const { return MAKE_RGB(144, 144, 144); }
  virtual ushort EyeColor() const { return MAKE_RGB(100, 0, 0); }
  virtual ushort ClothColor() const { return MAKE_RGB(111,74, 37); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, 208);}
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 96); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 96); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0, 80); }
  virtual ulong TotalVolume() const { return 60000; }
  virtual std::string GetDeathMessage() { return Name(DEFINITE) + " is transformed into a crumpled heap of bones."; }
  virtual std::string NameSingular() const { return "skeleton"; }
  virtual void CreateCorpse();
  //virtual float GetMeleeStrength() const { return 1000; }
  virtual ushort TotalSize() const { return 150; }
);

class CHARACTER
(
  goblin,
  humanoid,
  {
    SetAgility(15);
    SetStrength(10);
    SetEndurance(15);
    SetPerception(18);
  },
 public:
  static bool CanBeGenerated() { return true; }
  virtual void BeTalkedTo(character*);
  virtual void CreateInitialEquipment();
  virtual uchar GetSex() const { return UNDEFINED; }
 protected:
  //virtual ushort SkinColor() const { return MAKE_RGB(0, 128, 0); }
  virtual ushort ClothColor() const { return MAKE_RGB(111,74,37); } 
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, 48); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32,112); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 144); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(16, 64); }
  virtual ulong TotalVolume() const { return 25000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new goblinoidflesh(Volume); }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(144,0); }
  virtual std::string NameSingular() const { return "goblin"; }
  //virtual float GetMeleeStrength() const { return 2000; }
  virtual ushort TotalSize() const { return 100; }
);

class ABSTRACT_CHARACTER
(
  mommo,
  character,
 public:
  virtual std::string StandVerb() const { return "bubbling"; }
  virtual bool CanOpen() const { return false; }
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new brownslime(Volume); }
  virtual std::string GetDeathMessage() { return Name(DEFINITE) + " turns into lifeless goo."; }
  virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBrownSlimeVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBrownSlimeVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBrownSlimeVerb(Critical); }
  //virtual float GetMeleeStrength() const { return 20000; }
  virtual std::string TalkVerb() const { return "vibrates oddly"; }
);

class CHARACTER
(
  conicalmommo,
  mommo,
  {
    SetAgility(2);
    SetStrength(4);
    SetEndurance(50);
    SetPerception(9);
  },
 public:
  virtual ulong GetBloodColor() const { return MAKE_RGB(7,155,0); }
 protected:
  virtual ulong TotalVolume() const { return 250000; }
  virtual std::string NameSingular() const { return "conical mommo slime"; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(176,0); }
  virtual ushort TotalSize() const { return 100; }
);

class CHARACTER
(
  flatmommo,
  mommo,
  {
    SetAgility(4);
    SetStrength(2);
    SetEndurance(25);
    SetPerception(9);
  },
 public:
  virtual ulong GetBloodColor() const { return MAKE_RGB(7,155,0); }
 protected:
  virtual ulong TotalVolume() const { return 150000; }
  virtual std::string NameSingular() const { return "flat mommo slime"; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(192,0); }
  virtual ushort TotalSize() const { return 75; }
);

class CHARACTER
(
  golem,
  character,
  {
    SetAgility(5);
    SetStrength(20);
    SetEndurance(20);
    SetPerception(12);
  },
 public:
  virtual bool MoveRandomly();
  virtual void SpillBlood(uchar) { }
  virtual void SpillBlood(uchar, vector2d) { }
  virtual void BeTalkedTo(character*);
  virtual bool CheckForUsefulItemsOnGround() { return false; }
 protected:
  virtual ulong TotalVolume() const { return 100000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return protosystem::CreateRandomSolidMaterial(Volume); }
  virtual std::string GetDeathMessage() { return "The Holy Words of " + Name(DEFINITE) + " fly away and the monster collapses."; }
  virtual std::string NameSingular() const { return "golem"; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(256,0); }
  //virtual float GetMeleeStrength() const;
  virtual ushort TotalSize() const { return 250; }
  virtual bool ShowMaterial() const { return true; }
);

class CHARACTER
(
  wolf,
  character,
  {
    SetAgility(20);
    SetStrength(10);
    SetEndurance(10);
    SetPerception(24);
  },
 protected:
  virtual ulong TotalVolume() const { return 40000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new wolfflesh(Volume); }
  virtual std::string NameSingular() const { return "wolf"; }
  virtual std::string NamePlural() const { return "wolves"; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(224,0); }
  //virtual float GetMeleeStrength() const { return 7500; }
  virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBiteVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string TalkVerb() const { return "howls"; }
  virtual ushort TotalSize() const { return 100; }
);

class CHARACTER
(
  dog,
  character,
  {
    SetAgility(15);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(21);
  },
 public:
  virtual bool Catches(item*, float);
 protected:
  virtual ulong TotalVolume() const { return 30000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new dogflesh(Volume); }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(240,0); }
  virtual std::string NameSingular() const { return "dog"; }
  //virtual float GetMeleeStrength() const { return 5000; }
  virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBiteVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string TalkVerb() const { return "barks"; }
  virtual ushort TotalSize() const { return 70; }
  virtual ushort GetEatFlags() const { return character::GetEatFlags()|BONE; }
);

class CHARACTER
(
  spider,
  character,
  {
    SetAgility(5);
    SetStrength(2);
    SetEndurance(1);
    SetPerception(9);
  },
 public:
  virtual ulong GetBloodColor() const { return BLACK; }
  virtual bool HasInfraVision() const { return true; }
  virtual bool CanOpen() const { return false; }
 protected:
  virtual ulong TotalVolume() const { return 100; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new spiderflesh(Volume); }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(272,0); }
  virtual std::string NameSingular() const { return "spider"; }
  virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBiteVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  //virtual float GetMeleeStrength() const { return 10000; }
  virtual void CreateCorpse() { SetExists(false); }
  virtual std::string TalkVerb() const { return "says nothing"; }
  virtual ushort TotalSize() const { return 10; }
);

class CHARACTER
(
  jackal,
  character,
  {
    SetAgility(10);
    SetStrength(3);
    SetEndurance(3);
    SetPerception(18);
  },
 protected:
  virtual ulong TotalVolume() const { return 20000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new jackalflesh(Volume); }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(304,0); }
  virtual std::string NameSingular() const { return "jackal"; }
  virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBiteVerb(Critical); }                                                                                                                                                                                                                                                                                          //Jackals are unoriginal
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  //virtual float GetMeleeStrength() const { return 5000; }
  virtual std::string TalkVerb() const { return "howls"; }
  virtual ushort TotalSize() const { return 80; }
);

class CHARACTER
(
  donkey,
  character,
  {
    SetAgility(5);
    SetStrength(10);
    SetEndurance(10);
    SetPerception(15);
  },
 protected:
  virtual ulong TotalVolume() const { return 40000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new donkeyflesh(Volume); }
  virtual std::string GetDeathMessage() { return Name(DEFINITE) + " neighs one last time and then dies."; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(288,0); }
  virtual std::string NameSingular() const { return "mutant donkey"; }
  virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBiteVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  //virtual float GetMeleeStrength() const { return 1000; }
  virtual std::string TalkVerb() const { return "neighs"; }
  virtual ushort TotalSize() const { return 150; }
);

class CHARACTER
(
  communist,
  humanoid,
  {
    SetAgility(20);
    SetStrength(50);
    SetEndurance(50);
    SetPerception(18);
    SetAssignedName("Ivan");
  },
 public:
  virtual bool MoveRandomly();
  virtual void CreateInitialEquipment();
  virtual void BeTalkedTo(character*);
  virtual bool HasInfraVision() const { return true; }
  virtual uchar CriticalModifier() const { return 4; }
  virtual bool CanBeAssignedAName() const { return false; }
 protected:
  virtual ushort BeltColor() const { return MAKE_RGB(0, 0, 0); }
  virtual ushort ArmSpecialColor() const { return MAKE_RGB(160, 0, 0); }
  virtual ushort ClothColor() const { return MAKE_RGB(64, 56, 24); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, 176); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 144); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 112); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0,0); }
  virtual ulong TotalVolume() const { return 120000; }
  virtual std::string GetDeathMessage() { return Name(DEFINITE) + " falls groaning bravely: \"Party revenges " + Name(UNARTICLED) + "\"!"; }
  virtual std::string NameSingular() const { return "communist"; }
  //virtual float GetMeleeStrength() const { return 5000; }
  virtual ushort TotalSize() const { return 230; }
  virtual bool ShowClassDescription() const { return false; }
);

class CHARACTER
(
  hunter,
  humanoid,
  {
    SetAgility(20);
    SetStrength(15);
    SetEndurance(15);
    SetPerception(24);
  },
 public:
  virtual void CreateInitialEquipment();
  virtual void BeTalkedTo(character*);
 protected:
  virtual void CreateLeftArm() { SetLeftArm(0); }
  virtual ushort ClothColor() const { return MAKE_RGB(128, 80, 48); }
  virtual ushort BeltColor() const { return MAKE_RGB(144, 96, 60); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, 192); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 192); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 128); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0, 96); }
  virtual ulong TotalVolume() const { return 80000; }
  virtual std::string NameSingular() const { return "hunter"; }
  //virtual float GetMeleeStrength() const { return 2000; }
  virtual ushort TotalSize() const { return 180; }
);

class CHARACTER
(
  polarbear,
  character,
  {
    SetAgility(10);
    SetStrength(30);
    SetEndurance(30);
    SetPerception(24);
  },
 public:
  static bool CanBeGenerated() { return false; }
 protected:
  virtual ulong TotalVolume() const { return 500000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new polarbearflesh(Volume); }
  virtual std::string GetDeathMessage() { return Name(DEFINITE) + " groans terribly and falls dead to the ground."; }
  virtual std::string NameSingular() const { return "polar bear"; }
  //virtual float GetMeleeStrength() const { return 10000; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(336,0); }
  virtual std::string TalkVerb() const { return "growls"; }
  virtual ushort TotalSize() const { return 250; }
);

class CHARACTER
(
  dolphin,
  character,
  {
    SetAgility(30);
    SetStrength(10);
    SetEndurance(10);
    SetPerception(30);
  },
 public:
  static bool CanBeGenerated() { return false; }
  virtual std::string StandVerb() const { return "swimming"; }
  virtual bool CanBeDisplaced() const { return false; }
  virtual bool CanOpen() const { return false; }
  virtual bool CanWalk() const { return false; }
  virtual bool CanSwim() const { return true; }
  virtual bool Polymorph(character* Char, ushort) { delete Char; return false; }
 protected:
  virtual ulong TotalVolume() const { return 150000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new dolphinflesh(Volume); }
  virtual std::string NameSingular() const { return "female dolphin in season"; }
  //virtual float GetMeleeStrength() const { return 1000; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(320,0); }
  virtual std::string TalkVerb() const { return "peeps passionately to you"; }
  virtual ushort TotalSize() const { return 300; }
);

class CHARACTER
(
  lightfrog,
  frog,
  {
    SetAgility(30);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(18);
  },
 public:
  static bool CanBeGenerated() { return false; }
  virtual bool MoveRandomly() { return MoveRandomlyInRoom(); }
  virtual bool CanSwim() const { return true; }
 protected:
  virtual ulong TotalVolume() const { return 2500; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new lightfrogflesh(Volume); }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(80,0); }
  virtual std::string NameSingular() const { return "light frog"; }
  virtual ushort TotalSize() const { return 25; }
);

class CHARACTER
(
  slave,
  humanoid,
  {
    SetAgility(10);
    SetStrength(20);
    SetEndurance(15);
    SetPerception(15);
  },
 public:
  virtual void BeTalkedTo(character*);
  virtual void GetAICommand();
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new humanflesh(Volume); }
  //virtual ushort SkinColor() const { return MAKE_RGB(128, 80, 48); }
  virtual ushort HairColor() const { return MAKE_RGB(80, 48, 32); }
  virtual ushort EyeColor() const { return MAKE_RGB(64, 48, 24); }
  virtual ushort ClothColor() const { return MAKE_RGB(56, 48, 20); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96,0); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32,0); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 0); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0,16); }
  virtual ulong TotalVolume() const { return 60000; }
  virtual std::string NameSingular() const { return "slave"; }
  virtual ushort TotalSize() const { return 160; }
);

class CHARACTER
(
  petrusswife,
  humanoid,
  {
    SetAgility(10);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(21);
  },
 public:
  virtual void BeTalkedTo(character*);
  virtual uchar GetSex() const { return FEMALE; }
  virtual bool MoveRandomly() { return MoveRandomlyInRoom(); }
 protected:
  virtual ushort ClothColor() const { return MAKE_RGB(150, 0, 0); }
  virtual ushort BeltColor() const { return MAKE_RGB(180, 180, 0); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(112,0); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 160); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 160); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0, 112); }
  virtual ulong TotalVolume() const { return 50000; }
  virtual std::string NameSingular() const { return "Petrus's wife"; }
  //virtual float GetMeleeStrength() const { return 500; }
  virtual ushort TotalSize() const { return 170; }
  virtual bool ShowArticle() const { return false; }
);

class CHARACTER
(
  petrusswife1,
  petrusswife,
  {
    petrusswife::SetDefaultStats();
  },
 protected:
  virtual ushort HairColor() const { return MAKE_RGB(0, 0, 0); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(112, 0); }
  virtual std::string NameSingular() const { return "Petrus's wife number 1"; }
);

class CHARACTER
(
  petrusswife2,
  petrusswife,
  {
    petrusswife::SetDefaultStats();
  },
 protected:
  virtual ushort HairColor() const { return MAKE_RGB(0, 0, 0); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(112, 16); }
  virtual std::string NameSingular() const { return "Petrus's wife number 2"; }
);

class CHARACTER
(
  petrusswife3,
  petrusswife,
  {
    petrusswife::SetDefaultStats();
  },
 protected:
  virtual ushort HairColor() const { return MAKE_RGB(60, 48, 24); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(112,0); }
  virtual std::string NameSingular() const { return "Petrus's wife number 3"; }
);

class CHARACTER
(
  petrusswife4,
  petrusswife,
  {
    petrusswife::SetDefaultStats();
  },
 protected:
  virtual ushort HairColor() const { return MAKE_RGB(200, 96, 0); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(112,48); }
  virtual std::string NameSingular() const { return "Petrus's wife number 4"; }
);

class CHARACTER
(
  petrusswife5,
  petrusswife,
  {
    petrusswife::SetDefaultStats();
  },
 protected:
  virtual ushort HairColor() const { return MAKE_RGB(80, 64, 32); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(112,64); }
  virtual std::string NameSingular() const { return "Petrus's wife number 5"; }
);

class CHARACTER
(
  petrusswife6,
  petrusswife,
  {
    petrusswife::SetDefaultStats();
  },
 protected:
  virtual ushort HairColor() const { return MAKE_RGB(144, 0, 0); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(112,80); }
  virtual std::string NameSingular() const { return "Petrus's wife number 6"; }
);

class CHARACTER
(
  housewife,
  humanoid,
  {
    SetAgility(15);
    SetStrength(10);
    SetEndurance(15);
    SetPerception(24);
  },
 public:
  virtual void BeTalkedTo(character*);
  virtual uchar GetSex() const { return FEMALE; }
 protected:
  virtual ushort TorsoMainColor() const { return MAKE_RGB(160, 160, 160); }
  virtual ushort ArmMainColor() const { return MAKE_RGB(100, 100, 100); }
  virtual ushort LegMainColor() const { return MAKE_RGB(180, 80, 0); }
  virtual ushort HairColor() const { return MAKE_RGB(60, 48, 24); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(112, (RAND() % 6) * 16); } //may produce gibberling headed housewives...
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 160); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 160); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0, 112); }
  virtual ulong TotalVolume() const { return 70000; }
  virtual std::string NameSingular() const { return "housewife"; }
  //virtual float GetMeleeStrength() const { return 500; }
  virtual ushort TotalSize() const { return 160; }
);

class CHARACTER
(
  femaleslave,
  humanoid,
  {
    SetAgility(10);
    SetStrength(10);
    SetEndurance(15);
    SetPerception(18);
  },
 public:
  virtual void BeTalkedTo(character*);
  virtual uchar GetSex() const { return FEMALE; }
  virtual void CreateInitialEquipment();
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new humanflesh(Volume); }
  //virtual ushort SkinColor() const { return MAKE_RGB(160, 100, 64); }
  virtual ushort HairColor() const { return MAKE_RGB(80, 48, 32); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(112, 96); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 208); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 208); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0, 144); }
  virtual ulong TotalVolume() const { return 40000; }
  virtual std::string NameSingular() const { return "female slave"; }
  //virtual float GetMeleeStrength() const { return 500; }
  virtual void GetAICommand() { StandIdleAI(); }
  virtual ushort TotalSize() const { return 170; }
);

class CHARACTER
(
  librarian,
  humanoid,
  {
    SetAgility(5);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(12);
  },
 public:
  virtual void BeTalkedTo(character*);
 protected:
  virtual ushort HairColor() const { return MAKE_RGB(160, 160, 160); }
  virtual ushort ClothColor() const { return MAKE_RGB(48, 48, 48); }
  virtual ushort ArmMainColor() const { return MAKE_RGB(144, 144, 144); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, 224); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 80); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 16); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0, 0); }
  virtual ulong TotalVolume() const { return 80000; }
  virtual std::string NameSingular() const { return "librarian"; }
  //virtual float GetMeleeStrength() const { return 500; }
  virtual void GetAICommand() { StandIdleAI(); }
  virtual ushort TotalSize() const { return 170; }
);

class CHARACTER
(
  zombie,
  humanoid,
  {
    SetAgility(5);
    SetStrength(10);
    SetEndurance(5);
    SetPerception(12);
  },
 public:
  static bool CanBeGenerated() { return true; }
  virtual void BeTalkedTo(character*);
  virtual void SpillBlood(uchar, vector2d);
  //virtual float GetMeleeStrength() const { return 1500; }
  virtual uchar GetSex() const { return UNDEFINED; }
 protected:
  //virtual ushort SkinColor() const { return MAKE_RGB(0, 120, 120); }
  virtual ushort EyeColor() const { return MAKE_RGB(200, 0, 0); }
  virtual ushort ClothColor() const { return MAKE_RGB(56, 16, 96); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(112, 112); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(32, 224); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 224); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0, 160); }
  virtual ulong TotalVolume() const { return 50000; }
  virtual std::string GetDeathMessage() { return Name(DEFINITE) + " is slain (again)."; }
  virtual std::string NameSingular() const { return "zombie"; }
  virtual ushort TotalSize() const { return 160; }
);

class CHARACTER
(
  imp,
  humanoid,
  {
    SetAgility(15);
    SetStrength(10);
    SetEndurance(10);
    SetPerception(15);
  },
 public:
  virtual uchar GetSex() const { return UNDEFINED; }
 protected:
  //virtual ushort SkinColor() const { return MAKE_RGB(128, 0, 0); }
  virtual ushort EyeColor() const { return MAKE_RGB(150, 80, 0); }
  virtual ushort ClothColor() const { return MAKE_RGB(111,74,37); }
  virtual ushort CapColor() const { return MAKE_RGB(30,0,0); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(96, 16); } 
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(48, 112); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 32); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(16, 80); }
  virtual std::string Article() const { return "an"; }
  virtual ulong TotalVolume() const { return 40000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new sulfur(Volume); }
  virtual std::string NameSingular() const { return "imp"; }
  //virtual float GetMeleeStrength() const { return 20000; }
  virtual ushort TotalSize() const { return 100; }
);

class CHARACTER
(
  bat,
  character,
  {
    SetAgility(40);
    SetStrength(2);
    SetEndurance(1);
    SetPerception(24);
  },
 public:
  virtual std::string StandVerb() const { return "flying"; }
  virtual bool CanOpen() const { return false; }
  virtual bool HasInfraVision() const { return true; }
  virtual bool CanFly() const { return true; }
 protected:
  virtual ulong TotalVolume() const { return 1000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new batflesh(Volume); }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(464,0); }
  virtual std::string NameSingular() const { return "bat"; }
  //virtual float GetMeleeStrength() const { return 20000; }
  virtual std::string TalkVerb() const { return "squeaks"; }
  virtual ushort TotalSize() const { return 20; }
);

class CHARACTER
(
  mistress,
  humanoid,
  {
    SetAgility(35);
    SetStrength(25);
    SetEndurance(50);
    SetPerception(30);
  },
 public:
  virtual void BeTalkedTo(character*);
  static bool CanBeGenerated() { return true; }
  virtual uchar GetSex() const { return FEMALE; }
  //virtual float GetMeleeStrength() const { return 5000; }
  virtual void CreateInitialEquipment();
 protected:
  //virtual ushort SkinColor() const { return MAKE_RGB(255, 212, 192); }
  virtual ushort HairColor() const { return MAKE_RGB(35, 35, 35); }
  virtual ushort ClothColor() const { return MAKE_RGB(35, 35, 35); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(112, 240); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(48, 96); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(80, 80); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(16, 32); }
  virtual ulong TotalVolume() const { return 60000; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(352,0); }
  virtual std::string NameSingular() const { return "mistress"; }
  virtual ushort TotalSize() const { return 180; }
);

class CHARACTER
(
  werewolf,
  humanoid,
  {
    SetChangeCounter(RAND() % 2500);

    /*if(RAND() % 2)
      ChangeIntoHuman();
    else
      ChangeIntoWolf();*/
  },
 public:
  static bool CanBeGenerated() { return true; }
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual bool HasInfraVision() const { return GetIsWolf(); }
  virtual void ChangeIntoHuman();
  virtual void ChangeIntoWolf();
  virtual void Be();
  virtual bool GetIsWolf() const { return IsWolf; } 
  virtual void SetIsWolf(bool What) { IsWolf = What; }
  virtual void SetChangeCounter(ushort What) { ChangeCounter = What; }
  virtual ushort GetChangeCounter() { return ChangeCounter; }
  virtual ulong MaxDanger() const;
  //virtual bool CanWield() const { return !GetIsWolf(); }
  virtual uchar GetSex() const { return IsWolf ? UNDEFINED : MALE; }
 protected:
  //virtual ushort SkinColor() const { return IsWolf ? MAKE_RGB(88, 96, 88) : humanoid::SkinColor(); }
  virtual ushort EyeColor() const { return MAKE_RGB(160, 0, 0); }
  virtual ushort ClothColor() const { return MAKE_RGB(96, 64, 32); }
  virtual vector2d GetHeadBitmapPos() const { return IsWolf ? vector2d(112, 144) : vector2d(96, 0); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(48, 0); }
  virtual vector2d GetArmBitmapPos() const { return IsWolf ? vector2d(80, 240) : vector2d(64, 0); }
  virtual vector2d GetLegBitmapPos() const { return IsWolf ? vector2d(0, 192) : vector2d(0, 176); }
  virtual ulong TotalVolume() const { return 80000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new werewolfflesh(Volume); }
  virtual std::string NameSingular() const { return "werewolf"; }
  //virtual float GetMeleeStrength() const;
  virtual std::string TalkVerb() const { return IsWolf ? "howls" : "screams"; }
  virtual ushort TotalSize() const { return 170; }
  ushort ChangeCounter;
  bool IsWolf;
);

class CHARACTER
(
  kobold,
  humanoid,
  {
    SetAgility(10);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(12);
  },
 public:
  virtual uchar GetSex() const { return UNDEFINED; }
  static bool CanBeGenerated() { return true; }
  virtual void CreateInitialEquipment();
 protected:
  //virtual ushort SkinColor() const { return MAKE_RGB(30, 100, 110); }
  virtual ushort HairColor() const { return MAKE_RGB(35, 35, 35); }
  virtual ushort ClothColor() const { return MAKE_RGB(111, 74, 37); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(112, 208); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(48, 176); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 176); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(16, 112); }
  virtual ulong TotalVolume() const { return 30000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new koboldflesh(Volume); }
  virtual std::string GetDeathMessage() { return Name(DEFINITE) + " dies yelling like a tortured hyena."; }
  virtual std::string NameSingular() const { return "kobold"; }
  //virtual float GetMeleeStrength() const { return 2000; }
  virtual ushort TotalSize() const { return 90; }
);

class CHARACTER
(
  gibberling,
  humanoid,
  {
    SetAgility(20);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(15);
  },
 public:
  virtual uchar GetSex() const { return UNDEFINED; }
 protected:
  //virtual ushort SkinColor() const { return MAKE_RGB(100, 100, 200); }
  virtual ushort HairColor() const { return MAKE_RGB(50, 20, 80); }
  virtual ushort ClothColor() const { return MAKE_RGB(111, 74, 37); }
  virtual ushort EyeColor() const { return MAKE_RGB(50,0,0); }
  virtual vector2d GetHeadBitmapPos() const { return vector2d(112, 32); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(48, 144); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(80, 144); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(16, 128); }
  virtual ulong TotalVolume() const { return 30000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new gibberlingflesh(Volume); }
  virtual std::string NameSingular() const { return "gibberling"; }
  //virtual float GetMeleeStrength() const { return 10000; }
  virtual ushort TotalSize() const { return 90; }
);

class CHARACTER  
(        
  largecat,
  character,
  {
    SetAgility(25);
    SetStrength(5);
    SetEndurance(5);
    SetPerception(21);
  },
 public:
  virtual bool Catches(item*, float);
 protected:
  virtual ulong TotalVolume() const { return 15000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new catflesh(Volume); }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(496,0); }
  virtual std::string NameSingular() const { return "large cat"; }
  //virtual float GetMeleeStrength() const { return 20000; }
  virtual std::string TalkVerb() const { return "mews"; }
  virtual ushort TotalSize() const { return 60; }
);

class CHARACTER  
(        
  largerat,
  character,
  {
    SetAgility(10);
    SetStrength(3);
    SetEndurance(2);
    SetPerception(12);
  },
 public:
  virtual bool CanOpen() const { return false; }
 protected:
  virtual ulong TotalVolume() const { return 2000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new ratflesh(Volume); }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(512,0); }
  virtual std::string NameSingular() const { return "large rat"; }
  //virtual float GetMeleeStrength() const { return 20000; }
  virtual std::string TalkVerb() const { return "squeaks"; }
  virtual ushort TotalSize() const { return 30; }
);

class CHARACTER  
(        
  angel,
  humanoid,
  {
    SetAgility(35);
    SetStrength(35);
    SetEndurance(35);
    SetPerception(45);
  },
 public:
  virtual void BeTalkedTo(character*);
  virtual bool Charmable() const { return false; }
  virtual bool Polymorph(character* Char, ushort) { delete Char; return false; }
  virtual bool HasInfraVision() const { return true; }
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual uchar GetSex() const { return FEMALE; }
  virtual void SetDivineMaster(uchar);
  virtual uchar GetDivineMaster() const { return DivineMaster; }
  virtual bool CanFly() const { return true; }
 protected:
  virtual ulong TotalVolume() const { return 60000; }
  virtual std::string GetDeathMessage() { return Name(DEFINITE) + " leaves this mortal plane behind."; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(432,0); }
  virtual std::string NameSingular() const { return "angel"; }
  //virtual float GetMeleeStrength() const { return 10000; }
  virtual void CreateCorpse() { SetExists(false); }
  virtual ushort TotalSize() const { return 180; }
  virtual std::string Article() const { return "an"; }
  virtual bool ShowPostFix() const { return true; }
  virtual std::string PostFix() const { return DivineMasterDescription(DivineMaster); }
  uchar DivineMaster;
);

class ABSTRACT_CHARACTER
(
  dwarf,
  humanoid,
 public:
  virtual void DrawToTileBuffer(bool) const;
 protected:
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new dwarfflesh(Volume); }
);

class CHARACTER
(
  kamikazedwarf,
  dwarf,
  {
    SetAgility(20);
    SetStrength(20);
    SetEndurance(20);
    SetPerception(24);
    SetDivineMaster(1 + RAND() % (game::GetGods() - 1));
  },
 public:
  virtual void BeTalkedTo(character*);
  static bool CanBeGenerated() { return true; }
  //virtual float GetMeleeStrength() const { return 2000; }
  virtual ulong CurrentDanger() const { return character::CurrentDanger() * 50; }
  virtual ulong MaxDanger() const { return character::MaxDanger() * 50; }
  virtual bool Hit(character*);
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual uchar GetDivineMaster() const { return DivineMaster; }
  virtual bool CheckForUsefulItemsOnGround() { return false; }
  virtual void GetAICommand();
  virtual void CreateInitialEquipment();
  static ushort Frequency() { return 1000; }
  virtual void SetDivineMaster(uchar);
 protected:
  virtual vector2d GetHeadBitmapPos() const { return vector2d(112, 160); }
  virtual vector2d GetTorsoBitmapPos() const { return vector2d(48, 16); }
  virtual vector2d GetArmBitmapPos() const { return vector2d(64, 0); }
  virtual vector2d GetLegBitmapPos() const { return vector2d(0, 208); }
  virtual ushort HairColor() const { return MAKE_RGB(160, 160, 160); }
  virtual ushort ClothColor() const { return MAKE_RGB(111, 74, 37); }
  virtual ulong TotalVolume() const { return 60000; }
  virtual std::string GetDeathMessage() { return Name(DEFINITE) + " dies smiling."; }
  virtual std::string NameSingular() const { return "kamikaze dwarf"; }
  virtual ushort TotalSize() const { return 130; }
  uchar DivineMaster;
);

class CHARACTER  
(        
  mammoth,
  character,
  {
    SetAgility(20);
    SetStrength(80);
    SetEndurance(80);
    SetPerception(18);
  },
 protected:
  virtual ulong TotalVolume() const { return 5000000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new mammothflesh(Volume); }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(528,0); }
  virtual std::string NameSingular() const { return "mammoth"; }
  //virtual float GetMeleeStrength() const { return 5000; }
  virtual std::string TalkVerb() const { return "roars"; }
  virtual ushort TotalSize() const { return 500; }
);

class CHARACTER
(
  unicorn,
  character,
  {
    SetAgility(40);
    SetStrength(10);
    SetEndurance(25);
    SetPerception(18);
    SetAlignment(RAND() % 3);
  },
 public:
  //virtual float GetMeleeStrength() const { return 5000; }
  virtual std::string TalkVerb() const { return "neighs"; }
  virtual void SetAlignment(uchar What) { Alignment = What; }
  virtual uchar GetAlignment() const { return Alignment; }
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual bool SpecialEnemySightedReaction(character*);
  virtual void CreateInitialEquipment();
 protected:
  virtual ulong TotalVolume() const { return 200000; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(544, 0); }
  virtual std::string NameSingular() const;
  virtual material* CreateBodyPartFlesh(ushort, ulong) const;
  virtual ushort TotalSize() const { return 200; }
  uchar Alignment;
);

class CHARACTER
(
  genie,
  humanoid,
  {
    SetAgility(30);
    SetStrength(30);
    SetEndurance(30);
    SetPerception(18);
  },
 public:
  virtual void SpillBlood(uchar) { }
  virtual void SpillBlood(uchar, vector2d) { }
  virtual void BeTalkedTo(character*);
  virtual std::string StandVerb() const { return "floating"; }
  virtual bool CanFly() const { return true; }
 protected:
  virtual ulong TotalVolume() const { return 200000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new air(Volume); } // temporary
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(416,0); }
  virtual std::string NameSingular() const { return "genie"; }
  virtual void CreateCorpse() { SetExists(false); }
  virtual std::string GetDeathMessage() { return Name(DEFINITE) + " vanishes from existence."; }
  //virtual float GetMeleeStrength() const { return 5000; }
  virtual ushort TotalSize() const { return 250; }
);

class CHARACTER
(
  lion,
  character,
  {
    SetAgility(40);
    SetStrength(30);
    SetEndurance(30);
    SetPerception(24);
  },
 protected:
  virtual ulong TotalVolume() const { return 150000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new lionflesh(Volume); }
  virtual std::string GetDeathMessage() { return Name(DEFINITE) + " growls."; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(576,0); }
  virtual std::string NameSingular() const { return "lion"; }
  virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBiteVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  //virtual float GetMeleeStrength() const { return 3000; }
  virtual std::string TalkVerb() const { return "growls"; }
  virtual ushort TotalSize() const { return 200; }
);

class CHARACTER
(
  carnivorousplant,
  character,
  {
    SetAgility(5);
    SetStrength(30);
    SetEndurance(4);
    SetPerception(5);
  },
 public:
  virtual bool CanOpen() const { return false; }
 protected:
  virtual ulong TotalVolume() const { return 20000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new leaf(Volume); }
  virtual std::string GetDeathMessage() { return Name(DEFINITE) + " howls."; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(0,16); }
  virtual std::string NameSingular() const { return "carnivorous plant"; }
  virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBiteVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  //virtual float GetMeleeStrength() const { return 2500; }
  virtual std::string TalkVerb() const { return "is silent"; }
  virtual ushort TotalSize() const { return 250; }
  virtual void GetAICommand();
  //virtual void CreateTorso();
);

class CHARACTER
(
  buffalo,
  character,
  {
    SetAgility(4);
    SetStrength(3);
    SetEndurance(30);
    SetPerception(24);
  },
 protected:
  virtual ulong TotalVolume() const { return 200000; }
  virtual material* CreateBodyPartFlesh(ushort, ulong Volume) const { return new buffaloflesh(Volume); }
  virtual std::string GetDeathMessage() { return Name(DEFINITE) + " snarls one last time."; }
  virtual vector2d GetTorsoBitmapPos(ushort) const { return vector2d(16,16); }
  virtual std::string NameSingular() const { return "buffalo"; }
  virtual std::string ThirdPersonMeleeHitVerb(bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  virtual std::string FirstPersonHitVerb(character*, bool Critical) const { return FirstPersonBiteVerb(Critical); }
  virtual std::string AICombatHitVerb(character*, bool Critical) const { return ThirdPersonBiteVerb(Critical); }
  //virtual float GetMeleeStrength() const { return 500; }
  virtual std::string TalkVerb() const { return "snarls"; }
  virtual ushort TotalSize() const { return 250; }
);

#endif

