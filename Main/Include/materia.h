/*
 *
 *  Iter Vehemens ad Necem (IVAN)
 *  Copyright (C) Timo Kiviluoto
 *  Released under the GNU General
 *  Public License
 *
 *  See LICENSING which should included
 *  with this file for more details
 *
 */

#ifndef __MATERIA_H__
#define __MATERIA_H__

#include "script.h"
#include "ivandef.h"

#define MAKE_MATERIAL material::MakeMaterial

class entity;
class bodypart;
class materialprototype;
template <class type> class databasecreator;

typedef material* (*materialspawner)(int, long, truth);
typedef material* (*materialcloner)(const material*);

struct materialdatabase : public databasebase
{
  typedef materialprototype prototype;
  void InitDefaults(const prototype*, int);
  void PostProcess() { }
  const prototype* ProtoType;
  ulong CategoryFlags;
  ulong BodyFlags;
  ulong InteractionFlags;
  int StrengthValue;
  int ConsumeType;
  int Density;
  int Color;
  int RainColor;
  long PriceModifier;
  col24 Emitation;
  int NutritionValue;
  festring NameStem;
  festring AdjectiveStem;
  int Effect;
  int ConsumeEndMessage;
  int HitMessage;
  long ExplosivePower;
  alpha Alpha;
  int Flexibility;
  int SpoilModifier;
  int EffectStrength;
  int DigProductMaterial;
  int ConsumeWisdomLimit;
  int AttachedGod;
  festring BreatheMessage;
  int StepInWisdomLimit;
  int RustModifier;
  int Acidicity;
  contentscript<item> NaturalForm;
  int HardenedMaterial;
  int IntelligenceRequirement;
  int Stickiness;
};

class materialprototype
{
 public:
  friend class databasecreator<material>;
  materialprototype(const materialprototype*, materialspawner, materialcloner, const char*);
  material* Spawn(int Config, long Volume = 0) const { return Spawner(Config, Volume, false); }
  material* SpawnAndLoad(inputfile&) const;
  material* Clone(const material* Material) const { return Cloner(Material); }
  const char* GetClassID() const { return ClassID; }
  int GetIndex() const { return Index; }
  const materialprototype* GetBase() const { return Base; }
  int CreateSpecialConfigurations(materialdatabase**, int Configs) { return Configs; }
  const materialdatabase* ChooseBaseForConfig(materialdatabase** TempConfig, int, int) { return *TempConfig; }
  const materialdatabase*const* GetConfigData() const { return ConfigData; }
  int GetConfigSize() const { return ConfigSize; }
 private:
  int Index;
  const materialprototype* Base;
  materialdatabase** ConfigData;
  materialdatabase** ConfigTable[CONFIG_TABLE_SIZE];
  int ConfigSize;
  materialspawner Spawner;
  materialcloner Cloner;
  const char* ClassID;
};

class material
{
 public:
  friend class databasecreator<material>;
  typedef materialprototype prototype;
  typedef materialdatabase database;
  material(int NewConfig, long InitVolume = 0, truth Load = false) : MotherEntity(0) { Initialize(NewConfig, InitVolume, Load); }
  material() : MotherEntity(0) { }
  virtual ~material() { }
  void AddName(festring&, truth = false, truth = true) const;
  festring GetName(truth = false, truth = true) const;
  material* TakeDipVolumeAway();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  truth Effect(character*, long);
  virtual material* EatEffect(character*, long);
  truth HitEffect(character*, bodypart*);
  virtual col16 GetSkinColor() const { return GetColor(); }
  virtual void SetSkinColor(int) { }
  long GetRawPrice() const;
  truth CanBeDug(material* ShovelMaterial) const;
  virtual truth HasBe() const { return false; }
  virtual void Be() { }
  int GetType() const { return GetProtoType()->GetIndex(); }
  virtual void AddConsumeEndMessage(character*) const;
  DATA_BASE_VALUE(const prototype*, ProtoType);
  DATA_BASE_VALUE(int, Config);
  DATA_BASE_VALUE(ulong, CommonFlags);
  DATA_BASE_VALUE(ulong, NameFlags);
  DATA_BASE_VALUE(ulong, CategoryFlags);
  DATA_BASE_VALUE(ulong, BodyFlags);
  DATA_BASE_VALUE(ulong, InteractionFlags);
  virtual DATA_BASE_VALUE(int, StrengthValue);
  DATA_BASE_VALUE(int, ConsumeType);
  DATA_BASE_VALUE(int, Density);
  DATA_BASE_VALUE(int, Color);
  DATA_BASE_VALUE(int, RainColor);
  DATA_BASE_VALUE(long, PriceModifier);
  DATA_BASE_VALUE(col24, Emitation);
  DATA_BASE_VALUE(int, NutritionValue);
  DATA_BASE_VALUE(const festring&, NameStem);
  DATA_BASE_VALUE(const festring&, AdjectiveStem);
  DATA_BASE_VALUE(int, Effect);
  DATA_BASE_VALUE(int, ConsumeEndMessage);
  DATA_BASE_VALUE(int, HitMessage);
  DATA_BASE_VALUE(long, ExplosivePower);
  DATA_BASE_VALUE(alpha, Alpha);
  DATA_BASE_VALUE(int, Flexibility);
  DATA_BASE_VALUE(int, SpoilModifier);
  DATA_BASE_VALUE(int, EffectStrength);
  DATA_BASE_VALUE(int, DigProductMaterial);
  DATA_BASE_VALUE(int, ConsumeWisdomLimit);
  DATA_BASE_VALUE(int, AttachedGod);
  DATA_BASE_VALUE(int, RustModifier);
  DATA_BASE_VALUE(int, Acidicity);
  DATA_BASE_VALUE(const contentscript<item>&, NaturalForm);
  DATA_BASE_VALUE(int, IntelligenceRequirement);
  DATA_BASE_VALUE(int, Stickiness);
  const database* GetDataBase() const { return DataBase; }
  material* SpawnMore() const { return GetProtoType()->Spawn(GetConfig(), Volume); }
  material* SpawnMore(long Volume) const { return GetProtoType()->Spawn(GetConfig(), Volume); }
  long GetTotalExplosivePower() const;
  static material* MakeMaterial(int, long = 0);
  virtual truth IsFlesh() const { return false; }
  virtual truth IsLiquid() const { return false; }
  virtual const char* GetConsumeVerb() const;
  entity* GetMotherEntity() const { return MotherEntity; }
  void SetMotherEntity(entity* What) { MotherEntity = What; }
  truth IsSameAs(const material* What) const { return What->GetConfig() == GetConfig(); }
  truth IsTransparent() const { return GetAlpha() != 255; }
  //virtual material* Duplicate() const = 0;
  virtual long GetTotalNutritionValue() const;
  virtual truth IsVeryCloseToSpoiling() const { return false; }
  virtual void AddWetness(long) { }
  virtual int GetSpoilLevel() const { return 0; }
  virtual void ResetSpoiling() { }
  truth CanBeEatenByAI(const character*) const;
  virtual void SetSpoilCounter(int) { }
  DATA_BASE_VALUE(const festring&, BreatheMessage);
  truth BreatheEffect(character*);
  virtual truth SkinColorIsSparkling() const { return IsSparkling(); }
  virtual void SetSkinColorIsSparkling(truth) { }
  DATA_BASE_VALUE(int, StepInWisdomLimit);
  virtual void SetRustLevel(int) { }
  virtual int GetRustLevel() const { return NOT_RUSTED; }
  virtual int GetRustData() const { return NOT_RUSTED; }
  virtual truth TryToRust(long, long = 0) { return false; }
  static const database* GetDataBase(int);
  virtual truth CanSpoil() const { return false; }
  truth IsSolid() const { return !IsLiquid(); }
  /* A dummy materialpredicate */
  truth True() const { return true; }
  void FinishConsuming(character*);
  long GetVolume() const { return Volume; }
  long GetWeight() const { return Volume ? Volume * GetDensity() / 1000 : 0; }
  void EditVolume(long What) { SetVolume(Volume + What); }
  void SetVolume(long);
  void SetVolumeNoSignals(long What) { Volume = What; }
  virtual truth IsPowder() const { return false; }
  static item* CreateNaturalForm(int, long);
  item* CreateNaturalForm(long) const;
  virtual truth IsInfectedByLeprosy() const { return false; }
  virtual void SetIsInfectedByLeprosy(truth) { }
  virtual truth AddRustLevelDescription(festring&, truth) const { return false; }
  int GetHardenedMaterial(const item*) const;
  int GetHardenModifier(const item*) const;
  virtual int GetSpoilPercentage() const { return 0; }
  virtual truth Spoils() const { return false; }
  virtual truth IsExplosive() const;
  virtual truth IsSparkling() const;
  material* Duplicate() const { return DataBase->ProtoType->Clone(this); }
  truth IsStuckTo(const character*) const;
 protected:
  virtual void PostConstruct() { }
  void Initialize(int, long, truth);
  virtual const prototype* FindProtoType() const { return &ProtoType; }
  static const prototype ProtoType;
  const database* DataBase;
  entity* MotherEntity;
  long Volume;
};

template <class type, class base>
class materialsysbase : public base
{
 public:
  typedef materialsysbase<type, base> mybase;
  static type* Spawn(int Config = 0, long Volume = 0, truth Load = false)
  {
    type* M = new type;
    M->Initialize(Config, Volume, Load);
    return M;
  }
  static material* Clone(const type* T) { return new type(*T); }
  virtual const materialprototype* FindProtoType() const { return &ProtoType; }
  static const materialprototype ProtoType;
};

#ifdef __FILE_OF_STATIC_MATERIAL_PROTOTYPE_DEFINITIONS__
#define MATERIAL_PROTO(name, base)\
template<> const materialprototype name##sysbase::ProtoType(&base::ProtoType, (materialspawner)(&name##sysbase::Spawn), (materialcloner)(&name##sysbase::Clone), #name);
#else
#define MATERIAL_PROTO(name, base)
#endif

#define MATERIAL(name, base)\
class name;\
typedef materialsysbase<name, base> name##sysbase;\
MATERIAL_PROTO(name, base)\
class name : public name##sysbase

#endif
