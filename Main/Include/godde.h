#ifndef __GODDE_H__
#define __GODDE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "godba.h"

class GOD
(
  valpurus,
  god,
 public:
  virtual std::string Name() const { return "Valpurus"; }
  virtual std::string Description() const { return "king of gods"; }
  virtual uchar Alignment() const { return ALPP; }
  virtual void Pray();
  virtual uchar BasicAlignment() const { return GOOD; }
  virtual void AddPriestMessage() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  venius,
  god,
 public:
  virtual std::string Name() const { return "Venius"; }
  virtual std::string Description() const { return "god of paladins and holy fire"; }
  virtual uchar Alignment() const { return ALP; }
  virtual uchar BasicAlignment() const { return GOOD; }
  virtual void AddPriestMessage() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  atavus,
  god,
 public:
  virtual std::string Name() const { return "Atavus"; }
  virtual std::string Description() const { return "god of gifts"; }
  virtual uchar Alignment() const { return ALP; }
  virtual void Pray();
  virtual uchar BasicAlignment() const { return GOOD; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  dulcis,
  god,
 public:
  virtual std::string Name() const { return "Dulcis"; }
  virtual std::string Description() const { return "goddess of music"; }
  virtual uchar Alignment() const { return AL; }
  virtual uchar BasicAlignment() const { return GOOD; }
  virtual void AddPriestMessage() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  inasnum,
  god,
 public:
  virtual std::string Name() const { return "Inasnum"; }
  virtual std::string Description() const { return "god of prophecies and dreams"; }
  virtual uchar Alignment() const { return AL; }
  virtual uchar BasicAlignment() const { return GOOD; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  seges,
  god,
 public:
  virtual std::string Name() const { return "Seges"; }
  virtual std::string Description() const { return "goddess of health and nutrition"; }
  virtual uchar Alignment() const { return AL; }
  virtual uchar BasicAlignment() const { return GOOD; }
  virtual void AddPriestMessage() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  consummo,
  god,
 public:
  virtual std::string Name() const { return "Consummo"; }
  virtual std::string Description() const { return "god of knowledge, wisdom and understanding"; }
  virtual uchar Alignment() const { return ALM; }
  virtual uchar BasicAlignment() const { return GOOD; }
  virtual void AddPriestMessage() const;
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  silva,
  god,
 public:
  virtual std::string Name() const { return "Silva"; }
  virtual std::string Description() const { return "goddess of nature"; }
  virtual uchar Alignment() const { return ANP; }
  virtual uchar BasicAlignment() const { return NEUTRAL; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  loricatus,
  god,
 public:
  virtual std::string Name() const { return "Loricatus"; }
  virtual std::string Description() const { return "god of fire, machines and weaponry"; }
  virtual uchar Alignment() const { return AN; }
  virtual uchar BasicAlignment() const { return NEUTRAL; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  mellis,
  god,
 public:
  virtual std::string Name() const { return "Mellis"; }
  virtual std::string Description() const { return "god of money, trade and politics"; }
  virtual uchar Alignment() const { return ANM; }
  virtual uchar BasicAlignment() const { return NEUTRAL; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  calamus,
  god,
 public:
  virtual std::string Name() const { return "Calamus"; }
  virtual std::string Description() const { return "god of assassins and thieves"; }
  virtual uchar Alignment() const { return ACP; }
  virtual uchar BasicAlignment() const { return EVIL; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  pestifer,
  god,
 public:
  virtual std::string Name() const { return "Pestifer"; }
  virtual std::string Description() const { return "god of pain, misery and annoying noise"; }
  virtual uchar Alignment() const { return AC; }
  virtual uchar BasicAlignment() const { return EVIL; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  macellarius,
  god,
 public:
  virtual std::string Name() const { return "Macellarius"; }
  virtual std::string Description() const { return "god of greed and forbidden pleasures"; }
  virtual uchar Alignment() const { return AC; }
  virtual uchar BasicAlignment() const { return EVIL; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  scabies,
  god,
 public:
  virtual std::string Name() const { return "Scabies"; }
  virtual std::string Description() const { return "goddess of mutations, deseases and famine"; }
  virtual uchar Alignment() const { return AC; }
  virtual uchar BasicAlignment() const { return EVIL; }
  virtual void PlayerVomitedOnAltar();
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  infuscor,
  god,
 public:
  virtual std::string Name() const { return "Infuscor"; }
  virtual std::string Description() const { return "goddess of wrong knowledge and vile magic"; }
  virtual uchar Alignment() const { return ACM; }
  virtual uchar BasicAlignment() const { return EVIL; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  cruentus,
  god,
 public:
  virtual std::string Name() const { return "Cruentus"; }
  virtual std::string Description() const { return "god of war and blood"; }
  virtual uchar Alignment() const { return ACM; }
  virtual void Pray();
  virtual uchar BasicAlignment() const { return EVIL; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

class GOD
(
  erado,
  god,
 public:
  virtual std::string Name() const { return "Erado"; }
  virtual std::string Description() const { return "destroyer of worlds"; }
  virtual uchar Alignment() const { return ACMM; }
  virtual void Pray();
  virtual uchar BasicAlignment() const { return EVIL; }
 protected:
  virtual void PrayGoodEffect();
  virtual void PrayBadEffect();
);

#endif