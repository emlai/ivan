#ifndef __ITEMBA_H__
#define __ITEMBA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"
#include "vector2d.h"
#include "object.h"

class bitmap;
class character;
class humanoid;
class material;
class object;
class stack;
class outputfile;
class inputfile;
class slot;
class item;
template <class type> class database;

struct itemdatabase
{
  void InitDefaults() { IsAbstract = false; }
  ushort Possibility;
  vector2d InHandsPic;
  ulong OfferModifier;
  long Score;
  bool IsDestroyable;
  bool CanBeWished;
  bool IsMaterialChangeable;
  uchar WeaponCategory;
  bool IsPolymorphSpawnable;
  bool IsAutoInitializable;
  ushort OneHandedStrengthPenalty;
  ushort OneHandedToHitPenalty;
  uchar Category;
  ushort SoundResistance;
  ushort EnergyResistance;
  ushort AcidResistance;
  ushort FireResistance;
  ushort PoisonResistance;
  ushort BulimiaResistance;
  bool IsStackable;
  ushort StrengthModifier;
  ushort FormModifier;
  ulong NPModifier;
  ushort DefaultSize;
  ulong DefaultMainVolume;
  ulong DefaultSecondaryVolume;
  ulong DefaultContainedVolume;
  vector2d BitmapPos;
  ulong Price;
  ushort BaseEmitation;
  std::string Article;
  std::string Adjective;
  std::string AdjectiveArticle;
  std::string NameSingular;
  std::string NamePlural;
  std::string PostFix;
  uchar ArticleMode;
  std::vector<long> MainMaterialConfig;
  std::vector<long> SecondaryMaterialConfig;
  std::vector<long> ContainedMaterialConfig;
  std::vector<long> MaterialConfigChances;
  bool IsAbstract;
  bool IsPolymorphable;
};

class itemprototype
{
 public:
  friend class database<item>;
  itemprototype(itemprototype*);
  virtual ~itemprototype() { }
  virtual item* Clone(ushort = 0, bool = true, bool = false) const = 0;
  item* CloneAndLoad(inputfile&) const;
  virtual std::string ClassName() const = 0;
  ushort GetIndex() const { return Index; }
  const itemdatabase* GetDataBase() const { return &DataBase; }
  const itemprototype* GetBase() const { return Base; }
  DATABASEBOOL(IsAutoInitializable);
  DATABASEBOOL(CanBeWished);
  DATABASEBOOL(IsPolymorphSpawnable);
  DATABASEVALUE(ushort, Possibility);
  DATABASEVALUE(const std::string&, NameSingular);
  DATABASEVALUE(const std::string&, NamePlural);
  DATABASEBOOL(IsAbstract);
  const std::map<ushort, itemdatabase>& GetConfig() const { return Config; }
 protected:
  ushort Index;
  itemdatabase DataBase;
  itemprototype* Base;
  std::map<ushort, itemdatabase> Config;
};

/* Presentation of the item class */

class item : public object
{
 public:
  typedef itemprototype prototype;
  typedef itemdatabase database;
  typedef std::map<ushort, itemdatabase> databasemap;
  item(donothing);
  virtual float GetWeaponStrength() const;
  virtual void DrawToTileBuffer(bool) const;
  virtual void DrawToTileBuffer(vector2d Pos, bool) const;
  virtual void PositionedDrawToTileBuffer(uchar, bool) const;
  virtual item* TryToOpen(character*);
  virtual bool Consume(character*, long);
  virtual bool IsHeadOfElpuri() const { return false; }
  virtual bool IsPetrussNut() const { return false; }
  virtual bool IsGoldenEagleShirt() const { return false; }
  virtual bool CanBeRead(character*) const { return false; }
  virtual bool Read(character*) { return false; }
  virtual void ReceiveHitEffect(character*, character*) { }
  virtual void DipInto(material*, character*) { }
  virtual material* CreateDipMaterial() { return 0; }
  virtual item* BetterVersion() const { return 0; }
  virtual short CalculateOfferValue(char) const;
  virtual bool Fly(character*, uchar, ushort);
  virtual bool HitCharacter(character*, character*, float);
  virtual bool DogWillCatchAndConsume() const { return false; }
  virtual item* PrepareForConsuming(character*);
  virtual bool Apply(character*);
  virtual bool Zap(character*, vector2d, uchar);
  virtual bool Polymorph(stack*);
  virtual void CheckPickUpEffect(character*) { }
  virtual bool GetStepOnEffect(character*) { return false; }
  virtual bool IsTheAvatar() const { return false; }
  virtual void SignalSquarePositionChange(bool) { }
  virtual bool IsBadFoodForAI(character*) const;
  virtual std::string GetConsumeVerb() const { return "eating"; }
  virtual bool IsExplosive() const { return false; }
  virtual bool CatWillCatchAndConsume() const { return false; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void ChargeFully(character*) { }
  virtual void SetSize(ushort Value) { Size = Value; }
  virtual ushort GetSize() const { return Size; }
  virtual ulong GetID() const { return ID; }
  virtual void SetID(ulong What) { ID = What; }
  virtual void TeleportRandomly();
  virtual ushort GetStrengthValue() const;
  virtual slot* GetSlot() const { return Slot; }
  virtual void SetSlot(slot* What) { Slot = What; }
  virtual void PlaceToSlot(slot*);
  virtual void RemoveFromSlot();
  virtual void MoveTo(stack*);
  virtual void DonateSlotTo(item*);
  static std::string ItemCategoryName(uchar);
  static bool ConsumableSorter(item* Item, character* Char) { return Item->IsConsumable(Char); }
  static bool OpenableSorter(item* Item, character* Char) { return Item->IsOpenable(Char); }
  static bool ReadableSorter(item* Item, character* Char) { return Item->IsReadable(Char); }
  static bool DippableSorter(item* Item, character* Char) { return Item->IsDippable(Char); }
  static bool DipDestinationSorter(item* Item, character* Char) { return Item->IsDipDestination(Char); }
  static bool AppliableSorter(item* Item, character* Char) { return Item->IsAppliable(Char); }
  static bool ZappableSorter(item* Item, character* Char) { return Item->IsZappable(Char); }
  static bool ChargeableSorter(item* Item, character* Char) { return Item->IsChargeable(Char); }
  static bool HelmetSorter(item* Item, character* Char) { return Item->IsHelmet(Char); }
  static bool AmuletSorter(item* Item, character* Char) { return Item->IsAmulet(Char); }
  static bool CloakSorter(item* Item, character* Char) { return Item->IsCloak(Char); }
  static bool BodyArmorSorter(item* Item, character* Char) { return Item->IsBodyArmor(Char); }
  static bool RingSorter(item* Item, character* Char) { return Item->IsRing(Char); }
  static bool GauntletSorter(item* Item, character* Char) { return Item->IsGauntlet(Char); }
  static bool BeltSorter(item* Item, character* Char) { return Item->IsBelt(Char); }
  static bool BootSorter(item* Item, character* Char) { return Item->IsBoot(Char); }
  virtual bool IsConsumable(character*) const;
  virtual bool IsOpenable(character*) const { return false; }
  virtual bool IsReadable(character*) const { return false; }
  virtual bool IsDippable(character*) const { return false; }
  virtual bool IsDipDestination(character*) const { return false; }
  virtual bool IsAppliable(character*) const { return false; }
  virtual bool IsZappable(character*) const { return false; }
  virtual bool IsChargeable(character*) const { return false; }
  virtual bool IsHelmet(character*) const { return false; }
  virtual bool IsAmulet(character*) const { return false; }
  virtual bool IsCloak(character*) const { return false; }
  virtual bool IsBodyArmor(character*) const { return false; }
  virtual bool IsRing(character*) const { return false; }
  virtual bool IsGauntlet(character*) const { return false; }
  virtual bool IsBelt(character*) const { return false; }
  virtual bool IsBoot(character*) const { return false; }
  virtual bool IsOnGround() const;
  virtual ushort GetResistance(uchar) const;
  virtual void GenerateLeftOvers(character*);
  virtual void Be();
  virtual bool RemoveMaterial(uchar);
  virtual ushort GetType() const { return GetProtoType()->GetIndex(); }
  virtual void SetDivineMaster(uchar) { }
  virtual bool ReceiveDamage(character*, short, uchar) { return false; }
  virtual void AddConsumeEndMessage(character*) const;
  virtual bool IsEqual(item*) const { return false; }
  virtual bool RaiseTheDead(character*) { return false; }
  virtual bool FitsBodyPartIndex(uchar, character*) const { return false; }
  virtual const prototype* GetProtoType() const;
  virtual const database* GetDataBase() const { return DataBase; }
  //virtual bool CanOpenDoors() const { return false; }
  //virtual uchar GetLockType() const { return 0xFF; }
  virtual bool CanOpenLockType(uchar) const { return false; }
  virtual bool IsWhip() const { return false; }

  DATABASEVALUE(ushort, Possibility);
  DATABASEVALUE(vector2d, InHandsPic);
  DATABASEVALUE(ulong, OfferModifier);
  DATABASEVALUE(long, Score);
  DATABASEBOOL(IsDestroyable);
  DATABASEBOOL(CanBeWished);
  DATABASEBOOL(IsMaterialChangeable);
  DATABASEVALUE(uchar, WeaponCategory);
  DATABASEBOOL(IsPolymorphSpawnable);
  DATABASEBOOL(IsAutoInitializable);
  DATABASEVALUEWITHPARAMETER(ushort, OneHandedStrengthPenalty, character*);
  DATABASEVALUEWITHPARAMETER(ushort, OneHandedToHitPenalty, character*);
  DATABASEVALUE(uchar, Category);
  DATABASEVALUE(ushort, SoundResistance);
  DATABASEVALUE(ushort, EnergyResistance);
  DATABASEVALUE(ushort, AcidResistance);
  DATABASEVALUE(ushort, FireResistance);
  DATABASEVALUE(ushort, PoisonResistance);
  DATABASEVALUE(ushort, BulimiaResistance);
  DATABASEBOOL(IsStackable);
  DATABASEVALUE(ushort, StrengthModifier);
  DATABASEVALUE(ushort, FormModifier);
  DATABASEVALUE(ulong, NPModifier);
  DATABASEVALUE(ushort, DefaultSize);
  DATABASEVALUE(ulong, DefaultMainVolume);
  DATABASEVALUE(ulong, DefaultSecondaryVolume);
  DATABASEVALUE(ulong, DefaultContainedVolume);
  DATABASEVALUEWITHPARAMETER(vector2d, BitmapPos, ushort);
  DATABASEVALUE(ulong, Price);
  DATABASEVALUE(ushort, BaseEmitation);
  DATABASEVALUE(std::string, Article);
  DATABASEVALUE(std::string, Adjective);
  DATABASEVALUE(std::string, AdjectiveArticle);
  DATABASEVALUE(std::string, NameSingular);
  DATABASEVALUE(std::string, NamePlural);
  DATABASEVALUE(std::string, PostFix);
  DATABASEVALUE(uchar, ArticleMode);
  DATABASEVALUE(const std::vector<long>&, MainMaterialConfig);
  DATABASEVALUE(const std::vector<long>&, SecondaryMaterialConfig);
  DATABASEVALUE(const std::vector<long>&, ContainedMaterialConfig);
  DATABASEVALUE(const std::vector<long>&, MaterialConfigChances);
  DATABASEBOOL(IsPolymorphable);

  virtual bool SavesLifeWhenWorn() const { return false; }
  static item* Clone(ushort, bool, bool) { return 0; }
  virtual bool CanBeSoldInLibrary(character* Librarian) const { return CanBeRead(Librarian); }
  //virtual bool ReceiveApply(character*) { return false; }
  virtual bool TryKey(item*, character*) { return false; }

 protected:
  virtual void LoadDataBaseStats();
  virtual void VirtualConstructor(bool) { }
  virtual void Initialize(uchar, bool, bool);
  virtual void InstallDataBase();
  virtual void GenerateMaterials();
  virtual uchar GetGraphicsContainerIndex(ushort) const { return GRITEM; }
  virtual ushort RandomizeMaterialConfiguration();
  virtual void InitChosenMaterial(material*&, const std::vector<long>&, ulong, ushort);
  //virtual bool ShowMaterial() const { return true; }
  virtual bool ShowMaterial() const;
  slot* Slot;
  bool Cannibalised;
  ushort Size;
  ulong ID;
  graphic_id InHandsGraphicId;
  const database* DataBase;
};

#ifdef __FILE_OF_STATIC_ITEM_PROTOTYPE_DECLARATIONS__

#define ITEM_PROTOTYPE(name, baseproto)\
  \
  class name##_prototype : public itemprototype\
  {\
   public:\
    name##_prototype(itemprototype* Base) : itemprototype(Base) { }\
    virtual item* Clone(ushort Config, bool CallGenerateMaterials, bool Load) const { return name::Clone(Config, CallGenerateMaterials, Load); }\
    virtual std::string ClassName() const { return #name; }\
  } name##_ProtoType(baseproto);\
  \
  const item::prototype* name::GetProtoType() const { return &name##_ProtoType; }

#else

#define ITEM_PROTOTYPE(name, baseproto)

#endif

#define ITEM(name, base, data)\
\
name : public base\
{\
 public:\
  name(ushort Config = 0, bool CallGenerateMaterials = true, bool Load = false) : base(donothing()) { Initialize(Config, CallGenerateMaterials, Load); }\
  name(material* FirstMaterial, ushort Config = 0) : base(donothing()) { Initialize(Config, true, false); SetMainMaterial(FirstMaterial); }\
  name(donothing D) : base(D) { }\
  virtual const prototype* GetProtoType() const;\
  static item* Clone(ushort Config, bool CallGenerateMaterials, bool Load) { return new name(Config, CallGenerateMaterials, Load); }\
  data\
}; ITEM_PROTOTYPE(name, &base##_ProtoType);

#define ABSTRACT_ITEM(name, base, data)\
\
name : public base\
{\
 public:\
  name(donothing D) : base(D) { }\
  virtual const prototype* GetProtoType() const;\
  static item* Clone(ushort, bool, bool) { return 0; }\
  data\
}; ITEM_PROTOTYPE(name, &base##_ProtoType);

#endif

