#define __FILE_OF_STATIC_CHARACTER_PROTOTYPE_DEFINITIONS__

#include "proto.h"
#include "charba.h"

std::vector<character::prototype*> protocontainer<character>::ProtoData;
valuemap protocontainer<character>::CodeNameMap;

CHARACTER_PROTOTYPE(character, 0);

#include "femath.h"
#include "itemde.h"
#include "charde.h"

#undef __FILE_OF_STATIC_CHARACTER_PROTOTYPE_DEFINITIONS__

#include <cmath>

#include "stack.h"
#include "itemde.h"
#include "level.h"
#include "lsquare.h"
#include "message.h"
#include "hscore.h"
#include "godba.h"
#include "feio.h"
#include "wskill.h"
#include "felist.h"
#include "festring.h"
#include "team.h"
#include "lterraba.h"
#include "error.h"
#include "config.h"
#include "slot.h"
#include "actionde.h"
#include "colorbit.h"
#include "save.h"
#include "game.h"

petrus::~petrus()
{
  game::SetPetrus(0);
}

void oree::CreateInitialEquipment(ushort SpecialFlags)
{
  humanoid::CreateInitialEquipment(SpecialFlags);
  can* Can = new can(0, NO_MATERIALS);
  Can->InitMaterials(MAKE_MATERIAL(IRON, 10), MAKE_MATERIAL(PEPSI, 330), !(SpecialFlags & NO_PIC_UPDATE));
  GetStack()->AddItem(Can);
}

bool ennerbeast::Hit(character*)
{
  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, GetLevelUnder()->GetBorder(), GetPos(), 30);

  for(ushort x = Rect.X1; x <= Rect.X2; ++x)
    for(ushort y = Rect.Y1; y <= Rect.Y2; ++y)
      {
	character* Char = GetNearSquare(x, y)->GetCharacter();
	ushort ScreamStrength = ushort(50 / (hypot(GetPos().X - x, GetPos().Y - y) + 1));

	if(Char && Char != this)
	  {
	    if(Char->IsPlayer())
	      if(RAND() & 1)
		ADD_MESSAGE("%s yells: UGH UGHAaaa!", CHAR_DESCRIPTION(DEFINITE));
	      else
		ADD_MESSAGE("%s yells: Uga Ugar Ugade Ugat!", CHAR_DESCRIPTION(DEFINITE));

	    Char->ReceiveDamage(this, ScreamStrength, SOUND, ALL, YOURSELF, true);
	    Char->CheckDeath("killed by " + GetName(INDEFINITE) + "'s scream");
	  }

	GetNearLSquare(x, y)->GetStack()->ReceiveDamage(this, ScreamStrength, SOUND);

	for(ushort c = 0; c < 4; ++c)
	  GetNearLSquare(x, y)->GetSideStack(c)->ReceiveDamage(this, ScreamStrength, SOUND);
      }

  EditNP(-100);
  EditAP(-10 * GetCWeaponSkill(BITE)->GetAPBonus());
  return true;
}

void skeleton::CreateCorpse()
{
  ushort Amount = 2 + RAND() % 4;

  for(ushort c = 0; c < Amount; ++c)
    GetStackUnder()->AddItem(new bone);

  SendToHell();
}

void petrus::CreateCorpse()
{
  GetStackUnder()->AddItem(new leftnutofpetrus);
  SendToHell();
}

void elpuri::CreateCorpse()
{
  character::CreateCorpse();
  GetStackUnder()->AddItem(new headofelpuri);
}

void humanoid::Save(outputfile& SaveFile) const
{
  character::Save(SaveFile);
  SaveFile << SWeaponSkill;
}

void humanoid::Load(inputfile& SaveFile)
{
  character::Load(SaveFile);
  SaveFile >> SWeaponSkill;

  if(GetRightWielded())
    for(ushort c = 0; c < SWeaponSkill.size(); ++c)
      if(SWeaponSkill[c]->GetID() == GetRightWielded()->GetID())
	{
	  SetCurrentRightSWeaponSkill(SWeaponSkill[c]);
	  break;
	}

  if(GetLeftWielded())
    for(ushort c = 0; c < SWeaponSkill.size(); ++c)
      if(SWeaponSkill[c]->GetID() == GetLeftWielded()->GetID())
	{
	  SetCurrentLeftSWeaponSkill(SWeaponSkill[c]);
	  break;
	}
}

bool golem::MoveRandomly()
{
  if(!(RAND() % 500))
    {
      Engrave("Golem Needs Master");
      return true;
    }
  else
    return character::MoveRandomly();
}

void ennerbeast::GetAICommand()
{
  SeekLeader();

  if(StateIsActivated(PANIC) || !(RAND() % 3))
    Hit(0);

  if(CheckForEnemies(false))
    return;

  if(FollowLeader())
    return;

  if(MoveRandomly())
    return;

  EditAP(-1000);
}

void petrus::GetAICommand()
{
  SetHealTimer(GetHealTimer() + 1);

  if(GetHealTimer() > 16384)
    for(ushort d = 0; d < 8; ++d)
      {
	square* Square = GetNeighbourSquare(d);

	if(Square)
	  {
	    character* Char = Square->GetCharacter();

	    if(Char && GetTeam()->GetRelation(Char->GetTeam()) == FRIEND && HealFully(Char))
	      return;
	  }
      }

  StandIdleAI();
}

bool petrus::HealFully(character* ToBeHealed)
{
  bool DidSomething = false;

  for(ushort c = 0; c < ToBeHealed->GetBodyParts(); ++c)
    if(!ToBeHealed->GetBodyPart(c))
      {
	bodypart* BodyPart = static_cast<bodypart*>(ToBeHealed->SearchForItemWithID(ToBeHealed->GetOriginalBodyPartID(c)));

	if(!BodyPart)
	  continue;

	BodyPart->RemoveFromSlot();
	ToBeHealed->AttachBodyPart(BodyPart);
	BodyPart->SetHP(BodyPart->GetMaxHP());

	if(ToBeHealed->IsPlayer())
	  ADD_MESSAGE("%s attaches your old %s back and heals it.", CHAR_NAME(DEFINITE), BodyPart->GetBodyPartName().c_str());
	else if(CanBeSeenByPlayer())
	  ADD_MESSAGE("%s attaches the old %s of %s back and heals it.", CHAR_NAME(DEFINITE), BodyPart->GetBodyPartName().c_str(), ToBeHealed->CHAR_DESCRIPTION(DEFINITE));

	DidSomething = true;
      }

  if(ToBeHealed->IsInBadCondition())
    {
      ToBeHealed->RestoreHP();

      if(ToBeHealed->IsPlayer())
	ADD_MESSAGE("%s heals you fully.", CHAR_DESCRIPTION(DEFINITE));
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s heals %s fully.", CHAR_DESCRIPTION(DEFINITE), ToBeHealed->CHAR_DESCRIPTION(DEFINITE));

      DidSomething = true;
    }

  if(DidSomething)
    {
      SetHealTimer(0);
      DexterityAction(5);
      return true;
    }
  else
    return false;
}

void petrus::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << HealTimer << StoryState;
}

void petrus::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> HealTimer >> StoryState;
  game::SetPetrus(this);
}

bool dog::Catches(item* Thingy, float)
{
  if(Thingy->DogWillCatchAndConsume())
    {
      if(ConsumeItem(Thingy))
	if(IsPlayer())
	  ADD_MESSAGE("You catch %s in mid-air and consume it.", Thingy->CHAR_NAME(DEFINITE));
	else
	  {
	    if(CanBeSeenByPlayer())
	      ADD_MESSAGE("%s catches %s and eats it.", CHAR_NAME(DEFINITE), Thingy->CHAR_NAME(DEFINITE));

	    ChangeTeam(game::GetPlayer()->GetTeam());
	  }
      else
	if(IsPlayer())
	  ADD_MESSAGE("You catch %s in mid-air.", Thingy->CHAR_NAME(DEFINITE));
	else
	  if(CanBeSeenByPlayer())
	    ADD_MESSAGE("%s catches %s.", CHAR_NAME(DEFINITE), Thingy->CHAR_NAME(DEFINITE));

      return true;
    }
  else
    return false;
}

item* humanoid::GetMainWielded() const
{
  if(GetMainArm())
    if(GetMainArm()->GetWielded())
      return GetMainArm()->GetWielded();
    else
      if(GetSecondaryArm())
	return GetSecondaryArm()->GetWielded();
      else
	return 0;
  else
    if(GetSecondaryArm())
      return GetSecondaryArm()->GetWielded();
    else
      return 0;
}

item* humanoid::GetSecondaryWielded() const
{
  if(GetMainArm() && GetMainArm()->GetWielded() && GetSecondaryArm())
    return GetSecondaryArm()->GetWielded();
  else
    return 0;
}

bool humanoid::Hit(character* Enemy)
{
  if(IsPlayer() && GetTeam()->GetRelation(Enemy->GetTeam()) != HOSTILE && !game::BoolQuestion("This might cause a hostile reaction. Are you sure? [y/N]"))
    return false;

  Hostility(Enemy);

  if(GetBurdenState() == OVER_LOADED)
    {
      if(IsPlayer())
	ADD_MESSAGE("You cannot fight while carrying so much.");

      return true;
    }

  ushort c, AttackStyles;

  for(c = 0, AttackStyles = 0; c < 8; ++c)
    if(GetAttackStyle() & (1 << c))
      ++AttackStyles;

  ushort Chosen = RAND() % AttackStyles;

  for(c = 0, AttackStyles = 0; c < 8; ++c)
    if(GetAttackStyle() & (1 << c) && AttackStyles++ == Chosen)
      {
	Chosen = 1 << c;
	break;
      }

  switch(Chosen)
    {
    case USE_ARMS:
      if((GetRightArm() && GetRightArm()->GetDamage()) || (GetLeftArm() && GetLeftArm()->GetDamage()))
	{
	  long RightAPCost = 0, LeftAPCost = 0;

	  if(GetRightArm() && GetRightArm()->GetDamage())
	    {
	      RightAPCost = GetRightArm()->GetAPCost();
	      GetRightArm()->Hit(Enemy);
	    }

	  if(IsEnabled() && Enemy->IsEnabled() && GetLeftArm() && GetLeftArm()->GetDamage())
	    {
	      LeftAPCost = GetLeftArm()->GetAPCost();
	      GetLeftArm()->Hit(Enemy);
	    } 

	  EditNP(-50);
	  EditAP(-Max(RightAPCost, LeftAPCost));
	  return true;
	}
    case USE_LEGS:
      if(GetRightLeg() && GetLeftLeg())
	{
	  Kick(Enemy->GetLSquareUnder());
	  return true;
	}
    case USE_HEAD:
      if(GetHead())
	{
          Bite(Enemy);
	  return true;
	}
    default:
      if(IsPlayer())
	ADD_MESSAGE("You are currently quite unable to damage anything.");

      return false;
    }
}

bool humanoid::AddSpecialSkillInfo(felist& List) const
{
  bool Something = false;

  if(CurrentRightSWeaponSkill && CurrentRightSWeaponSkill->GetHits())
    {
      List.AddEntry("", LIGHT_GRAY);
      std::string Buffer = "right single weapon skill:  ";
      Buffer.resize(30, ' ');
      Buffer << CurrentRightSWeaponSkill->GetLevel();
      Buffer.resize(40, ' ');
      Buffer << CurrentRightSWeaponSkill->GetHits();
      Buffer.resize(50, ' ');

      if(CurrentRightSWeaponSkill->GetLevel() != 10)
	Buffer << (CurrentRightSWeaponSkill->GetLevelMap(CurrentRightSWeaponSkill->GetLevel() + 1) - CurrentRightSWeaponSkill->GetHits());
      else
	Buffer << '-';

      Buffer.resize(60, ' ');
      Buffer << '+' << int(CurrentRightSWeaponSkill->GetEffectBonus() - 100) << '%';
      Buffer.resize(70, ' ');
      Buffer << '-' << int(100 - CurrentRightSWeaponSkill->GetAPBonus()) << '%';
      List.AddEntry(Buffer, LIGHT_GRAY);
      Something = true;
    }

  if(CurrentLeftSWeaponSkill && CurrentLeftSWeaponSkill->GetHits())
    {
      if(!Something)
	List.AddEntry("", LIGHT_GRAY);

      std::string Buffer = "left single weapon skill:  ";
      Buffer.resize(30, ' ');
      Buffer << CurrentLeftSWeaponSkill->GetLevel();
      Buffer.resize(40, ' ');
      Buffer << CurrentLeftSWeaponSkill->GetHits();
      Buffer.resize(50, ' ');

      if(CurrentLeftSWeaponSkill->GetLevel() != 10)
	Buffer << (CurrentLeftSWeaponSkill->GetLevelMap(CurrentLeftSWeaponSkill->GetLevel() + 1) - CurrentLeftSWeaponSkill->GetHits());
      else
	Buffer << '-';

      Buffer.resize(60, ' ');
      Buffer << '+' << int(CurrentLeftSWeaponSkill->GetEffectBonus() - 100) << '%';
      Buffer.resize(70, ' ');
      Buffer << '-' << int(100 - CurrentLeftSWeaponSkill->GetAPBonus()) << '%';
      List.AddEntry(Buffer, LIGHT_GRAY);
      Something = true;
    }

  return Something;
}

void petrus::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("Heretic! Dev/null is a place not worthy to receive thee!");
      return;
    }

  if(Talker->HasGoldenEagleShirt())
    {
      ADD_MESSAGE("Petrus smiles. \"Thou hast defeated Oree! Valpurus shall bless thee for the rest of thine life! And thou possess the Shirt of the Golden Eagle, the symbol of my status! Return it now, please.\"");

      if(game::BoolQuestion("Will you give the Shirt of the Golden Eagle to Petrus? [y/n]", REQUIRES_ANSWER))
	{
	  game::TextScreen("Thou hast slain the Blood Daemon King, and Petrus is happy!\n\nYou are victorious!");
	  AddScoreEntry("retrieved the Shirt of the Golden Eagle and became the Avatar of Law", 4, false);
	  game::End();
	  return;
	}
      else
	{
	  ADD_MESSAGE("Petrus's face turns red. \"I see. Thine greed hast overcome thine wisdom. Then, we shall fight for the shiny shirt. May Valpurus bless him who is better.\"");

	  /* And now we actually make his face change color ;-) */

	  GetHead()->GetMainMaterial()->SetSkinColor(MakeRGB16(255, 75, 50));
	  GetHead()->UpdatePictures();
	  GetSquareUnder()->SendNewDrawRequest();
	  Talker->GetTeam()->Hostility(GetTeam());
	  return;
	}
    }
  else
    if(StoryState >= 2)
      {
	ADD_MESSAGE("Petrus says: \"Bring me the Shirt of the Golden Eagle and we'll talk.\"");
	return;
      }

  if(Talker->HasHeadOfElpuri())
    {
      if(game::GetGod(1)->GetRelation() >= 0)// && Talker->MaxDanger() >= 250000)
	{
	  ADD_MESSAGE("Petrus smiles. \"Thou areth indeed a great Champion of the Great Frog! Elpuri is not a foe worthy for thee.");

	  if(game::BoolQuestion("Dost thou wish to stay on duty for a while more and complete another quest for me?\" [y/n]", REQUIRES_ANSWER))
	    {
	      game::TextScreen(	"Champion of Law!\n\n"
				"Return to the foul cave of Elpuri and seek out the Master Evil:\n"
				"Oree the Blood Daemon King, who hast stolenth one of the most powerful of all of my artifacts:\n"
				"the Shirt of the Golden Eagle! Return with it and immortal glory shall be thine!");

	      GetAreaUnder()->SendNewDrawRequest();
	      game::TriggerQuestForGoldenEagleShirt();
	      StoryState = 2;
	      return;
	    }
	}

      game::TextScreen("Thou hast slain Elpuri, and Petrus is happy!\n\nYou are victorious!");
      AddScoreEntry("defeated Elpuri and continued to further adventures", 2, false);
      game::End();
    }
  else
    if(!StoryState)
      {
	game::TextScreen(	"Petrus raises his hand as a salutation, and talks:\n"
				"\"Fare thee well, adventurer! Should thou seek glory, I have a task for thee!\n"
				"An evil dark frog named Elpuri is pestering our fine city in many ways.\n"
				"Valpurus hast told that this vile beast can be found in a nearby cave.\n"
				"Slay it and bring me its head as proof. Return when thou hast succeeded.\"");

	GetAreaUnder()->SendNewDrawRequest();
	ADD_MESSAGE("\"And by the way, visit the librarian. He might have advice for thee.\"");
	StoryState = 1;
      }
    else
      ADD_MESSAGE("Petrus says: \"Bring me the head of Elpuri and we'll talk.\"");
}

void farmer::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      /* This message may be little incorrect if wielded item has changed... */

      ADD_MESSAGE("\"Did you think I use this axe only to chop wood?\"");
      return;
    }

  static bool Said[4];

  switch(RandomizeReply(4, Said))
    {
    case 0:
      ADD_MESSAGE("\"Crops are so lousy around here. Perhaps because the summer lasts two weeks.\"");
      break;
    case 1:
      ADD_MESSAGE("%s seems suspicious. \"You look like one from Istour! Go away!\"", CHAR_DESCRIPTION(DEFINITE));
      break;
    case 2:
      ADD_MESSAGE("%s sighs: \"Again polar bears ate my cattle...\"", CHAR_DESCRIPTION(DEFINITE));
      break;
    case 3:
      ADD_MESSAGE("\"The prices are infamous here. Ivan should smack that capitalist shopkeeper hard!\"");
      break;
    }
}

void guard::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"A fair trial? Hah! Prepare to be executed!\"");
      return;
    }

  static bool Said[5];

  switch(RandomizeReply(5, Said))
    {
    case 0:
      if(GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("%s says gravely: \"You don't have a life. Get it in the army.\"", CHAR_DESCRIPTION(DEFINITE));
      else
	ADD_MESSAGE("%s says gravely: \"You don't have a life. Get it as a shop guard.\"", CHAR_DESCRIPTION(DEFINITE));
      break;
    case 1:
      if(GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("%s looks at you suspiciously. \"Don't even think of breaking rules.\"", CHAR_DESCRIPTION(DEFINITE));
      else
	ADD_MESSAGE("%s looks at you suspiciously. \"Don't even think of stealing anything.\"", CHAR_DESCRIPTION(DEFINITE));
      break;
    case 2:
      if(GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("%s shouts excited: \"Attnam victoor!\"", CHAR_DESCRIPTION(DEFINITE));
      else
	ADD_MESSAGE("\"Yes, this is a dangerous place to work, but our boss pays us well.\"");
      break;
    case 3:
      if(GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("\"The High Priest is my idol. I would want a sword as big as his!\"");
      else
        ADD_MESSAGE("\"Attnam's guards can barely wield a sword. But we are trained by the laws of the dungeon, so don't do anything suspicious here.\"");
      break;
    case 4:
      if(GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("\"Attnam, shit, I'm still only in Attnam. Every time I think I'm gonna wake up back in the dungeon.\"");
      else
	ADD_MESSAGE("\"When I was here, I wanted to be there, when I was there all I could think of was getting back into the dungeon.\"");
      break;
    }
}

void shopkeeper::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"Criminal! Mellis bless my efforts of removing you!\"");
      return;
    }

  static bool Said[4];

  switch(RandomizeReply(4, Said))
    {
    case 0:
      if(GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("%s sighs: \"If only I hadn't chosen a city in the middle of nowhere...\"", CHAR_DESCRIPTION(DEFINITE));
      else
	ADD_MESSAGE("%s sighs: \"I wonder why I have so few customers these days...\"", CHAR_DESCRIPTION(DEFINITE));
      break;
    case 1:
      ADD_MESSAGE("%s sighs: \"Mutant mushrooms ate the last caravan, and the one before it ran into an enner beast. It must be all Elpuri's doings!\"", CHAR_DESCRIPTION(DEFINITE));
      break;
    case 2:
      if(GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("\"You truly can't find better prices in this city! Indeed, you can't find ANY prices, since my store is a monopoly.\"");
      else
	ADD_MESSAGE("\"The topmost reason why I work here is that the monsters devour tax collectors.\"");
      break;
    case 3:
      if(GetLevelUnder()->GetOnGround())
	ADD_MESSAGE("\"Don't try anything. The High Priest is a friend of mine.\"");
      else
	ADD_MESSAGE("\"The monsters don't attack me, because of our mutually profitable contract.\"");
      break;
    }
}

void priest::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"Sinner! My hands shall pour Dinive Wrath upon thee!\"");
      return;
    }

  for(ushort c = 0; c < Talker->GetBodyParts(); ++c)
    if(!Talker->GetBodyPart(c))
      {
	bool HasOld;
	bodypart* OldBodyPart = static_cast<bodypart*>(Talker->SearchForItemWithID(Talker->GetOriginalBodyPartID(c)));

	if(OldBodyPart)
	  {
	    HasOld = true;

	    if(Talker->GetMoney() >= PRICE_TO_ATTACH_OLD_LIMB_AT_ALTAR)
	      {
		if(!OldBodyPart->GetMainMaterial()->IsSameAs(Talker->GetTorso()->GetMainMaterial()))
		  ADD_MESSAGE("Sorry, I cannot put back your severed %s, because it doesn't seem to fit properly.", Talker->GetBodyPartName(c).c_str());
		else
		  {
		    ADD_MESSAGE("I could put your old %s back in exchange for %d gold.", Talker->GetBodyPartName(c).c_str(), PRICE_TO_ATTACH_OLD_LIMB_AT_ALTAR);

		    if(game::BoolQuestion("Do you agree? [y/N]"))
		      {
			OldBodyPart->SetHP(1);
			Talker->SetMoney(Talker->GetMoney() - PRICE_TO_ATTACH_OLD_LIMB_AT_ALTAR);
			SetMoney(GetMoney() + PRICE_TO_ATTACH_OLD_LIMB_AT_ALTAR);
			OldBodyPart->RemoveFromSlot();
			Talker->AttachBodyPart(OldBodyPart);
			return;
		      }
		  }
	      }
	    else
	      ADD_MESSAGE("\"You %s is severed. Help yourself and get %dgp and we'll help you too.\"", Talker->GetBodyPartName(c).c_str(), PRICE_TO_ATTACH_OLD_LIMB_AT_ALTAR);
	  }
	else
	  HasOld = false;

	if(Talker->GetMoney() >= PRICE_TO_ATTACH_NEW_LIMB_AT_ALTAR)
	  {
	    if(HasOld)
	      ADD_MESSAGE("I could still summon up a new one for %d gold.", PRICE_TO_ATTACH_NEW_LIMB_AT_ALTAR);
	    else
	      ADD_MESSAGE("Since you don't seem to have your orginal %s with you, I could summon up a new one for %d gold.", Talker->GetBodyPartName(c).c_str(), PRICE_TO_ATTACH_NEW_LIMB_AT_ALTAR);

	    if(game::BoolQuestion("Agreed? [y/N]"))
	      {
		Talker->SetMoney(Talker->GetMoney() - PRICE_TO_ATTACH_NEW_LIMB_AT_ALTAR);
		SetMoney(GetMoney() + PRICE_TO_ATTACH_NEW_LIMB_AT_ALTAR);
		Talker->CreateBodyPart(c);
		Talker->GetBodyPart(c)->SetHP(1);
		return;
	      }
	  }
	else if(!HasOld)
	  ADD_MESSAGE("\"You don't have your orginal %s with you. I could create you a new one, but %s is not a communist and you need %dgp first.\"", Talker->GetBodyPartName(c).c_str(), game::GetGod(GetLevelUnder()->GetRoom(HomeRoom)->GetDivineMaster())->GOD_NAME, PRICE_TO_ATTACH_NEW_LIMB_AT_ALTAR);
      }

  if(!HomeRoom)
    ADD_MESSAGE("\"Receive my blessings, child.\"");
  else
    ADD_MESSAGE("%s talks to you: %s", CHAR_DESCRIPTION(DEFINITE), game::GetGod(GetLevelUnder()->GetRoom(HomeRoom)->GetDivineMaster())->GetPriestMessage().c_str());
}

void oree::BeTalkedTo(character*)
{
  ADD_MESSAGE("%s laughs: \"No time for small talk. Time to drink blood!\"", CHAR_DESCRIPTION(DEFINITE));
}

void darkknight::BeTalkedTo(character*)
{
  ADD_MESSAGE("%s yells: \"For Mortifer I shall slay you!\"", CHAR_DESCRIPTION(DEFINITE));
}

void ennerbeast::BeTalkedTo(character*)
{
  static bool Said[4];

  switch(RandomizeReply(4, Said))
    {
    case 0:
      ADD_MESSAGE("\"Fishing is fun! Do you fish?\"");
      break;
    case 1:
      ADD_MESSAGE("\"And then I got that perch weighting fifty stones...\"");
      break;
    case 2:
      ADD_MESSAGE("\"What do you think of Braveheart?\"");
      break;
    case 3:
      ADD_MESSAGE("\"Can you help me with this download problem?\"");
      break;
    }
}

void elpuri::BeTalkedTo(character*)
{
  ADD_MESSAGE("%s roars horribly: \"DiE hUmAn!!\"", CHAR_DESCRIPTION(DEFINITE));
}

void billswill::BeTalkedTo(character*)
{
  ADD_MESSAGE("\"You will install Windows. Resistance is futile. Prepare to be assimilited.\"");
}

void skeleton::BeTalkedTo(character* Talker)
{
  if(!GetHead())
    {
      ADD_MESSAGE("The headless %s remains silent.", Talker->CHAR_DESCRIPTION(UNARTICLED));
      return;
    }

  if(GetTeam()->GetRelation(Talker->GetTeam()) != HOSTILE)
    ADD_MESSAGE("%s sings: \"Leg bone is connected to the hib bone, hib bone is connected to the rib bone...\"", CHAR_DESCRIPTION(DEFINITE));
  else
    ADD_MESSAGE("%s grunts: \"Bones. Need more bones.\"", CHAR_DESCRIPTION(DEFINITE));
}

void goblin::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) != HOSTILE)
    ADD_MESSAGE("%s laughs: \"Humie friend. Many mommo we kill. Many spider we eat.\"", CHAR_DESCRIPTION(DEFINITE));
  else
    ADD_MESSAGE("%s yells goblin war cries at you.", CHAR_DESCRIPTION(DEFINITE));
}

void golem::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) != HOSTILE)
    ADD_MESSAGE("\"Yes, master?\"");
  else
    ADD_MESSAGE("\"Yes, master. Golem kill human. Golem then return.\"");
}

void communist::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"You capitalist! Lenin want %s kill capitalists!\"", CHAR_NAME(UNARTICLED));
      return;
    }

  if(GetTeam() == Talker->GetTeam() || Talker->GetRelativeDanger(this, true) < 1.0f)
    {
      static bool Said[11];

      switch(RandomizeReply(11, Said))
	{
	case 0:
	  ADD_MESSAGE("\"Da, %s like killing.\"", CHAR_NAME(UNARTICLED));
	  break;
	case 1:
	  ADD_MESSAGE("\"%s ruski specialist.\"", CHAR_NAME(UNARTICLED));
	  break;
	case 2:
	  ADD_MESSAGE("\"%s work. Else %s get stapled.\"", CHAR_NAME(UNARTICLED), CHAR_NAME(UNARTICLED));
	  break;
	case 3:
	  ADD_MESSAGE("\"Party mean big weapons. %s like big weapons. %s kill for Party.\"", CHAR_NAME(UNARTICLED), CHAR_NAME(UNARTICLED));
	  break;
	case 4:
	  ADD_MESSAGE("\"CCCP roxxx.\"");
	  break;
	case 5:
	  ADD_MESSAGE("\"%s like throw Ladas. You want compete?\"", CHAR_NAME(UNARTICLED));
	  break;
	case 6:
	  ADD_MESSAGE("\"Why is AK not invented?\"");
	  break;
	case 7:
	  ADD_MESSAGE("\"%s like Attnam. Petrus communist too.\"", CHAR_NAME(UNARTICLED));
	  break;
	case 8:
	  ADD_MESSAGE("\"%s buy kyber eyes. %s see in dark.\"", CHAR_NAME(UNARTICLED), CHAR_NAME(UNARTICLED));
	  break;
	case 9:
	  ADD_MESSAGE("\"Uncle Lenin lives in Russia. Lenin strong guy. %s like.\"", CHAR_NAME(UNARTICLED));
	  break;
	case 10:
	  if(GetTeam() == Talker->GetTeam())
	    ADD_MESSAGE("\"We both good proles. We do good work.\"");
	  else
	    ADD_MESSAGE("\"Go away, you weak. Be back when learned killing.\"");
	  break;
	}
    }
  else
    {
      ADD_MESSAGE("%s seems to be very friendly. \"%s make good communist. %s go with %s!\"", CHAR_DESCRIPTION(DEFINITE), Talker->CHAR_NAME(UNARTICLED), CHAR_NAME(UNARTICLED), Talker->GetObjectPronoun().c_str());
      ChangeTeam(Talker->GetTeam());
    }
}

void communist::CreateInitialEquipment(ushort SpecialFlags)
{
  humanoid::CreateInitialEquipment(SpecialFlags);
  GetStack()->AddItem(new fiftymillionroubles(0, SpecialFlags));
}

void hunter::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"Your head will look fine above my fireplace!\"");
      return;
    }

  static bool Said[4];

  switch(RandomizeReply(4, Said))
    {
    case 0:
      ADD_MESSAGE("\"A man is not a man unless he has lost his left arm in a battle against a polar bear.\"");
      break;
    case 1:
      ADD_MESSAGE("\"Bears, ogres, slaves, farmers... Ah, there's so much to hunt here!\"");
      break;
    case 2:
      if(GetMainWielded())
	ADD_MESSAGE("\"This is my %s. There are many like it but this one is mine. My %s is my best friend.\"", GetMainWielded()->CHAR_NAME(UNARTICLED), GetMainWielded()->CHAR_NAME(UNARTICLED));
      else
	ADD_MESSAGE("\"I am the Great White Hunter. Get out of My way!\"");
      break;
    case 3:
      ADD_MESSAGE("\"I saw a communist visiting the city a few days past. I'm now organising a party to seek and hunt him down.\"");
      break;
    }
}

void slave::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"Yikes!\"");
      return;
    }

  character* Master;

  if(HomeRoom && (Master = GetLevelUnder()->GetRoom(HomeRoom)->GetMaster()))
    {
      if(Talker->GetMoney() >= 50)
	{
	  ADD_MESSAGE("%s talks: \"Do you want to buy me? 50 gold pieces. I work very hard.\"", CHAR_DESCRIPTION(DEFINITE));

	  if(game::BoolQuestion("Do you want to buy him? [y/N]"))
	    {
	      Talker->SetMoney(Talker->GetMoney() - 50);
	      Master->SetMoney(Master->GetMoney() + 50);
	      ChangeTeam(Talker->GetTeam());
	      SetHomeRoom(0);
	    }
	}
      else
	{
	  ADD_MESSAGE("\"Don't touch me! Master doesn't want people to touch sale items. I'm worth 50 gold pieces, you know!\"");
	}

      return;
    }

  if(GetTeam() == Talker->GetTeam())
    {
      /* What if the whip is in the off hand? */

      if(Talker->GetMainWielded() && Talker->GetMainWielded()->IsWhip())
	{
	  ADD_MESSAGE("\"Don't hit me! I work! I obey! I not think!\"");
	}
      else
	{
	  static bool Said[5];

	  switch(RandomizeReply(5, Said))
	    {
	    case 0:
	      ADD_MESSAGE("\"Whatever the master wants.\"");
	      break;
	    case 1:
	      ADD_MESSAGE("\"Work work work all day long. No, that was not a complain! Don't punish!\"");
	      break;
	    case 2:
	      ADD_MESSAGE("\"I love all my masters. At least when the whip is being washed.\"");
	      break;
	    case 3:
	      ADD_MESSAGE("\"I would like to be like Ivan. Ivan is a good worker.\"");
	      break;
	    case 4:
	      ADD_MESSAGE("\"I am putting myself to the fullest possible use, which is all I think that any conscious entity can ever hope to do.\"");
	      break;
	    }
	}
    }
  else
    ADD_MESSAGE("\"I'm free! Rejoice!\"");
}

void slave::GetAICommand()
{
  SeekLeader();

  if(CheckForEnemies(true))
    return;

  if(CheckForUsefulItemsOnGround())
    return;

  if(FollowLeader())
    return;

  if(CheckForDoors())
    return;

  if(!HomeRoom || !GetLevelUnder()->GetRoom(HomeRoom)->GetMaster())
    {
      HomeRoom = 0;

      if(MoveRandomly())
	return;
    }

  EditAP(-1000);
}

bool elpuri::Hit(character* Enemy)
{
  for(ushort d = 0; d < 8; ++d)
    {
      lsquare* Square = GetNeighbourLSquare(d);

      if(Square)
	{
	  character* ByStander = Square->GetCharacter();

	  if(ByStander && (ByStander == Enemy || ByStander->GetTeam()->GetRelation(GetTeam()) == HOSTILE))
	    {
	      nonhumanoid::Hit(ByStander);
	      ByStander->GetStack()->ReceiveDamage(this, 50 + RAND() % 51, PHYSICAL_DAMAGE);
	    }

	  Square->GetStack()->ReceiveDamage(this, 50 + RAND() % 51, PHYSICAL_DAMAGE);

	  for(ushort c = 0; c < 4; ++c)
	    if(Square->GetSideStack(c)->GetSquareTrulyUnder() == GetSquareUnder())
	      Square->GetSideStack(c)->ReceiveDamage(this, 50 + RAND() % 51, PHYSICAL_DAMAGE);
	}
    }

  SetAP(-1000);
  return true;
}

void petrusswife::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"Murderer! Just wait until Petrus finds you!\"");
      return;
    }

  static bool Said[4];

  switch(RandomizeReply(4, Said))
    {
    case 0:
      ADD_MESSAGE("\"I'm so sick jealous to those dolphins...\"");
      break;
    case 1:
      ADD_MESSAGE("\"I'm Petrus's favorite, not she!\"");
      break;
    case 2:
      ADD_MESSAGE("\"Why must Petrus stay in this forest? There isn't even a proper hairdresser here!\"");
      break;
    case 3:
      ADD_MESSAGE("\"Being one of six wives is a dream job. Pay is good and you only have to work about one night a week!\"");
      break;
    }
}

void housewife::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"Face my rolling pin! Graah!\"");
      return;
    }

  static bool Said[5];

  switch(RandomizeReply(5, Said))
    {
    case 0:
      ADD_MESSAGE("\"Can you help me find my husband? He is hiding somewhere. He's that farmer who's just been mugged with a frying pan.\"");
      break;
    case 1:
      ADD_MESSAGE("\"Yesterday a bear rushed through my kitchen wall and ruined all my pies. Animals are truly annoying. Why can't we just burn the whole forest down?\"");
      break;
    case 2:
      ADD_MESSAGE("\"Wolves ate my seventh daughter last week. Damn. It'll take eight years to produce an equally good replacement worker.\"");
      break;
    case 3:
      ADD_MESSAGE("\"Petrus's wives are so arrogant towards us working class ones. Grr...\"");
      break;
    case 4:
      ADD_MESSAGE("\"If you men only knew!\"");
    }
}

void femaleslave::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"Yikes!\"");
      return;
    }

  static bool Said[4];

  switch(RandomizeReply(4, Said))
    {
    case 0:
      ADD_MESSAGE("\"Praise our lord Petrus!\"");
      break;
    case 1:
      ADD_MESSAGE("\"Does that toy frog really need us serving it?\"");
      break;
    case 2:
      ADD_MESSAGE("\"I'm not a slave. I'm a high-ranking palm branch officer with a good career history.\"");
      break;
    case 3:
      ADD_MESSAGE("\"Palm leaves are good for health. Petrus loves their smell.\"");
      break;
    }
}

void librarian::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"The pen is mightier than the sword! Fall, unlearned one!\"");
      return;
    }

  static bool Said[11];

  switch(RandomizeReply(11, Said))
    {
    case 0:
      if(game::GetPetrus() && !game::GetPetrus()->GetStoryState())
	{
	  ADD_MESSAGE("\"Thou should visit Petrus if thou areth in need of adventure.\"");
	  break;
	}
      else
	{
	  ADD_MESSAGE("\"It is said that a wand of polymorph has dozens of uses.\"");
	  break;
	}
    case 1:
      if(game::GetPetrus() && game::GetPetrus()->GetStoryState() == 1)
	{
	  ADD_MESSAGE("\"Thou are going to fight Elpuri? Beware! It is a powerful enemy. Other monsters are very vulnerable if surrounded by thine party, but not that beast, for it may slay a horde of thine friends at once with its horrendous tail attack.\"");
	  break;
	}
      else
	{
	  ADD_MESSAGE("\"Remember: Scientia est potentia.\"");
	  break;
	}
    case 2:
      if(game::GetPetrus() && game::GetPetrus()->GetStoryState() == 1)
	{
	  ADD_MESSAGE("\"Elpuri the Dark Frog abhors light and resides in a level of eternal darkness.\"");
	  break;
	}
      else
	{
	  ADD_MESSAGE("\"Shh! Be silent in the library.\"");
	  break;
	}
    case 3:
      if(game::GetPetrus() && game::GetPetrus()->GetStoryState() == 1)
	{
	  ADD_MESSAGE("\"Elpuri's attacks are so strong that they may shatter many of thine precious items.\"");
	  break;
	}
      else
	{
	  ADD_MESSAGE("\"Dost thou not smell all the knowledge floating around here?\"");
	  break;
	}
    case 4:
      if(game::GetPetrus() && game::GetPetrus()->GetStoryState() == 2)
	{
	  ADD_MESSAGE("\"Thou wish to confront the Blood Daemon King? Heed my advice: He is a cunning enemy and will try to ambush thee. A powerful party and a means of quick escape at hand would help thee greatly.\"");
	  break;
	}
      else
	{
	  ADD_MESSAGE("\"It is said that Loricatus, the god of smithing, can enchant thine weapons' material.\"");
	  break;
	}
    case 5:
      if(game::GetPetrus() && game::GetPetrus()->GetStoryState() == 2)
	{
	  ADD_MESSAGE("\"The Shirt of the Golden Eagle is a legendary artifact. Thou canst not find a better armor.\"");
	  break;
	}
      else
	{
	  ADD_MESSAGE("\"In this book they talk about Mortifer, the great chaos god. He hates us mortals more than anything and will respond only to Champions of Evil.\"");
	  break;
	}
    case 6:
      ADD_MESSAGE("\"Attnam is traditionally ruled by the High Priest of the Great Frog. He holds the Shirt of the Golden Eagle and has always killed his predecessor.\"");
      break;
    case 7:
      if(game::GetPetrus() && game::GetPetrus()->GetStoryState() == 3)
	{
	  ADD_MESSAGE("\"Remember, the Shirt of the Golden Eagle is the armor the High Priests. Things will get *very* rough if one denies it from Petrus.\"");
	  break;
	}
      else
	{
	  ADD_MESSAGE("\"They say thou should keep all the artifacts thou find. They shalt make thee famous after thine retirement.\"");
	  break;
	}
    case 8:
      ADD_MESSAGE("\"If thou shalt ever encounter an enner beast, know this: It is a horrible foe. It may shatter thine items and armor with its scream that penetrates iron and stone. Thou should not engage it in melee. Kill it from afar.\"");
      break;
    case 9:
      if(game::GetPetrus() && game::GetPetrus()->GetStoryState() == 1)
	{
	  ADD_MESSAGE("\"Thou areth not alone in thine attempth to defeat Elpuri. A brave adventurer called Ivan also dived into its cave not long ago.\"");
	  break;
	}
      else
	{
	  ADD_MESSAGE("\"It is said that chaotic gods offer great power to their followers. But thou must remember: unlike lawfuls, they shalt not help thee when things go bad.\"");
	  break;
	}
    case 10:
      ADD_MESSAGE("\"If a man cannot choose, he ceases to be a man.\"");
      break;

    }
}

bool communist::MoveRandomly()
{
  switch(RAND() % 1000)
    {
    case 0:
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s engraves something to the ground.", CHAR_NAME(UNARTICLED));

      Engrave("The bourgeois is a bourgeois -- for the benefit of the working class.");
      return true;
    case 1:
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s engraves something to the ground.", CHAR_NAME(UNARTICLED));

      Engrave("Proletarians of all countries, unite!");
      return true;
    case 2:
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s engraves something to the ground.", CHAR_NAME(UNARTICLED));

      Engrave("Capital is therefore not only personal; it is a social power.");
      return true;
    default:
      return character::MoveRandomly();
    }
}

void zombie::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      if(RAND() % 5)
	{
	  if(GetHead())
	    ADD_MESSAGE("\"Need brain!!\"");
	  else
	    ADD_MESSAGE("\"Need head with brain!!\"");
	}
      else
	ADD_MESSAGE("\"Redrum! Redrum! Redrum!\"");
    }
  else
    ADD_MESSAGE("\"Need brain, but not your brain.\"");
}

void mistress::CreateInitialEquipment(ushort SpecialFlags)
{
  humanoid::CreateInitialEquipment(SpecialFlags);

  if(!(RAND() % 10))
    GetStack()->AddItem(new banana(0, SpecialFlags));

  if(!(RAND() % 100))
    GetStack()->AddItem(new holybanana(0, SpecialFlags));
}

void mistress::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    ADD_MESSAGE("\"Come closer, little boy, I'll teach you something...\"");
  else
    ADD_MESSAGE("\"Just wait for the night, darling.\"");
}

void angel::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << HealTimer;
}

void angel::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> HealTimer;
}

void angel::CreateInitialEquipment(ushort SpecialFlags)
{
  humanoid::CreateInitialEquipment(SpecialFlags);
  GetStack()->AddItem(new holybook(GetConfig(), SpecialFlags));

  switch(GetMasterGod()->BasicAlignment())
    {
    case GOOD:
      SetMainWielded(new meleeweapon(LONG_SWORD, SpecialFlags, MAKE_MATERIAL(DIAMOND)));
      SetBodyArmor(new bodyarmor(CHAIN_MAIL, SpecialFlags, MAKE_MATERIAL(DIAMOND)));
      GetCWeaponSkill(LARGE_SWORDS)->AddHit(2000);
      GetCurrentRightSWeaponSkill()->AddHit(2000);
      break;
    case NEUTRAL:
      SetMainWielded(new meleeweapon(POLE_AXE, SpecialFlags, MAKE_MATERIAL(SAPPHIRE)));
      SetBodyArmor(new bodyarmor(CHAIN_MAIL, SpecialFlags, MAKE_MATERIAL(SAPPHIRE)));
      GetCWeaponSkill(AXES)->AddHit(2000);
      GetCurrentRightSWeaponSkill()->AddHit(2000);
      break;
    case EVIL:
      {
	meleeweapon* SpikedMace = new meleeweapon(SPIKED_MACE, NO_MATERIALS);
	SpikedMace->InitMaterials(MAKE_MATERIAL(RUBY), MAKE_MATERIAL(RUBY), MAKE_MATERIAL(FROG_FLESH), !(SpecialFlags & NO_PIC_UPDATE));
	SetMainWielded(SpikedMace);
	SetBodyArmor(new bodyarmor(BROKEN|PLATE_MAIL, SpecialFlags, MAKE_MATERIAL(RUBY)));
	GetCWeaponSkill(MACES)->AddHit(2000);
	GetCurrentRightSWeaponSkill()->AddHit(2000);
	break;
      }
    }
}

void angel::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    ADD_MESSAGE("\"With the power of %s, I shall slay thee, sinner!\"", GetMasterGod()->Name().c_str());
  else
    ADD_MESSAGE("\"%s be with you, mortal.\"", GetMasterGod()->Name().c_str());
}

void kamikazedwarf::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"Heaven awaits me in the house of %s after I bomb you, heretic!\"", GetMasterGod()->Name().c_str());
      return;
    }

  static bool Said[4];

  switch(RandomizeReply(4, Said))
    {
    case 0:
      ADD_MESSAGE("\"Would you like me to teach you the best suicidal bombing tactics?\"");
      break;
    case 1:
      ADD_MESSAGE("%s shouts: \"Death to disbelievers!\"", CHAR_DESCRIPTION(DEFINITE));
      break;
    case 2:
      ADD_MESSAGE("%s praises %s with numerous hymns. %s is obviously a very devoted follower.", CHAR_DESCRIPTION(DEFINITE), GetMasterGod()->Name().c_str(), festring::CapitalizeCopy(GetPersonalPronoun()).c_str());
      break;
    case 3:
      ADD_MESSAGE("\"One day, Holy War will break out and I shall sacrifice my life with joy.\"");
      break;
    }
}

void kamikazedwarf::CreateInitialEquipment(ushort SpecialFlags)
{
  SetRightWielded(new holybook(GetConfig(), SpecialFlags));
  GetStack()->AddItem(new backpack(0, SpecialFlags));
  GetCWeaponSkill(UNCATEGORIZED)->AddHit(100);
  GetCurrentRightSWeaponSkill()->AddHit(100);
}

bool kamikazedwarf::Hit(character* Enemy)
{
  if(IsPlayer())
    return humanoid::Hit(Enemy);
  else
    {
      for(stackiterator i = GetStack()->GetBottom(); i.HasItem(); ++i)
	if(i->IsExplosive())
	  {
	    if(RAND() & 1)
	      ADD_MESSAGE("%s shouts: \"For %s!\"", CHAR_DESCRIPTION(DEFINITE), GetMasterGod()->Name().c_str());
	    else
	      ADD_MESSAGE("%s screams: \"%s, here I come!\"", CHAR_DESCRIPTION(DEFINITE), GetMasterGod()->Name().c_str());

	    if(i->Apply(this))
	      return true;
	  }

      return humanoid::Hit(Enemy);
    }
}

bool largecat::Catches(item* Thingy, float)
{
  if(Thingy->CatWillCatchAndConsume())
    {
      if(ConsumeItem(Thingy))
	if(IsPlayer())
	  ADD_MESSAGE("You catch %s in mid-air and consume it.", Thingy->CHAR_NAME(DEFINITE));
	else
	  {
	    if(CanBeSeenByPlayer())
	      ADD_MESSAGE("%s catches %s and eats it.", CHAR_NAME(DEFINITE), Thingy->CHAR_NAME(DEFINITE));

	    ChangeTeam(game::GetPlayer()->GetTeam());
	  }
      else
	if(IsPlayer())
	  ADD_MESSAGE("You catch %s in mid-air.", Thingy->CHAR_NAME(DEFINITE));
	else
	  if(CanBeSeenByPlayer())
	    ADD_MESSAGE("%s catches %s.", CHAR_NAME(DEFINITE), Thingy->CHAR_NAME(DEFINITE));

      return true;
    }
  else
    return false;
}

material* unicorn::CreateBodyPartFlesh(ushort, ulong Volume) const
{
  switch(Alignment)
    {
    case GOOD:
      return MAKE_MATERIAL(WHITE_UNICORN_FLESH, Volume);
    case NEUTRAL:
      return MAKE_MATERIAL(GRAY_UNICORN_FLESH, Volume);
    default:
      return MAKE_MATERIAL(BLACK_UNICORN_FLESH, Volume);
    }
}

void unicorn::Save(outputfile& SaveFile) const
{
  nonhumanoid::Save(SaveFile);
  SaveFile << Alignment;
}

void unicorn::Load(inputfile& SaveFile)
{
  nonhumanoid::Load(SaveFile);
  SaveFile >> Alignment;
}

void kamikazedwarf::GetAICommand()
{
  if(HomeRoom)
    StandIdleAI();
  else
    character::GetAICommand();
}

void genie::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    ADD_MESSAGE("\"Fall, you puny Prime Material Plane dweller!\"");
  else
    ADD_MESSAGE("\"You know, it's not fun to stay in an oil lamp for three centuries...\"");
}

bool unicorn::SpecialEnemySightedReaction(character*)
{
  if((RAND() % 3 && IsInBadCondition()) || !(RAND() % 10))
  {
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s disappears!", CHAR_NAME(DEFINITE));

    Move(GetLevelUnder()->GetRandomSquare(this), true);

    if(CanBeSeenByPlayer())
      ADD_MESSAGE("Suddenly %s appears from nothing!", CHAR_NAME(INDEFINITE));

    return true;
  }

  if(!(RAND() % 3) && MoveRandomly())
    return true;

  return false;
}

void unicorn::CreateInitialEquipment(ushort SpecialFlags)
{
  if(RAND() & 1)
    GetStack()->AddItem(new stone(0, SpecialFlags));

  if(RAND() & 1)
    GetStack()->AddItem(new stone(0, SpecialFlags));
}

ushort humanoid::GetSize() const
{
  ushort Size = 0;

  if(GetHead())
    Size += GetHead()->GetSize();

  if(GetTorso())
    Size += GetTorso()->GetSize();

  if(GetLeftLeg() && GetRightLeg())
    Size += Max(GetLeftLeg()->GetSize(), GetRightLeg()->GetSize());
  else if(GetLeftLeg())
    Size += GetLeftLeg()->GetSize();
  else if(GetRightLeg())
    Size += GetRightLeg()->GetSize();

  return Size;
}

ulong humanoid::GetBodyPartSize(ushort Index, ushort TotalSize)
{
  switch(Index)
    {
    case HEAD_INDEX: return 20;
    case TORSO_INDEX: return (TotalSize - 20) * 2 / 5;
    case RIGHT_ARMINDEX:
    case LEFT_ARMINDEX: return (TotalSize - 20) * 3 / 5;
    case GROIN_INDEX: return (TotalSize - 20) / 3;
    case RIGHT_LEGINDEX:
    case LEFT_LEGINDEX: return (TotalSize - 20) * 3 / 5;
    default:
      ABORT("Illegal humanoid bodypart size request!");
      return 0;
    }
}

ulong humanoid::GetBodyPartVolume(ushort Index)
{
  switch(Index)
    {
    case HEAD_INDEX: return 4000;
    case TORSO_INDEX: return (GetTotalVolume() - 4000) * 13 / 30;
    case RIGHT_ARMINDEX:
    case LEFT_ARMINDEX: return (GetTotalVolume() - 4000) / 10;
    case GROIN_INDEX: return (GetTotalVolume() - 4000) / 10;
    case RIGHT_LEGINDEX:
    case LEFT_LEGINDEX: return (GetTotalVolume() - 4000) * 2 / 15;
    default:
      ABORT("Illegal humanoid bodypart volume request!");
      return 0;
    }
}

bodypart* humanoid::MakeBodyPart(ushort Index)
{
  switch(Index)
    {
    case TORSO_INDEX: return new humanoidtorso(0, NO_MATERIALS);
    case HEAD_INDEX: return new head(0, NO_MATERIALS);
    case RIGHT_ARMINDEX: return new rightarm(0, NO_MATERIALS);
    case LEFT_ARMINDEX: return new leftarm(0, NO_MATERIALS);
    case GROIN_INDEX: return new groin(0, NO_MATERIALS);
    case RIGHT_LEGINDEX: return new rightleg(0, NO_MATERIALS);
    case LEFT_LEGINDEX: return new leftleg(0, NO_MATERIALS);
    default:
      ABORT("Weird bodypart to make for a humanoid. It must be your fault!");
      return 0;
    }
}

uchar humanoid::GetBodyPartBonePercentile(ushort Index)
{
  switch(Index)
    {
    case TORSO_INDEX: return GetTorsoBonePercentile();
    case HEAD_INDEX: return GetHeadBonePercentile();
    case RIGHT_ARMINDEX: return GetRightArmBonePercentile();
    case LEFT_ARMINDEX: return GetLeftArmBonePercentile();
    case GROIN_INDEX: return GetGroinBonePercentile();
    case RIGHT_LEGINDEX: return GetRightLegBonePercentile();
    case LEFT_LEGINDEX: return GetLeftLegBonePercentile();
    default:
      ABORT("Weird bodypart bone percentile request for a humanoid. It must be your fault!");
      return 0;
    }
}

bool humanoid::ReceiveDamage(character* Damager, ushort Damage, uchar Type, uchar TargetFlags, uchar Direction, bool Divide, bool PenetrateArmor, bool Critical, bool ShowMsg)
{
  std::vector<uchar> ChooseFrom;

  if(TargetFlags & RIGHT_ARM && GetRightArm())
    ChooseFrom.push_back(2);

  if(TargetFlags & LEFT_ARM && GetLeftArm())
    ChooseFrom.push_back(3);

  if(TargetFlags & RIGHT_LEG && GetRightLeg())
    ChooseFrom.push_back(5);

  if(TargetFlags & LEFT_LEG && GetLeftLeg())
    ChooseFrom.push_back(6);

  if(TargetFlags & HEAD && GetHead())
    ChooseFrom.push_back(1);

  if(TargetFlags & TORSO && GetTorso())
    ChooseFrom.push_back(0);

  if(TargetFlags & GROIN && GetGroin())
    ChooseFrom.push_back(4);

  if(!ChooseFrom.size())
    return false;

  ulong TotalVolume = 0;
  ushort c;

  for(c = 0; c < ChooseFrom.size(); ++c)
    TotalVolume += GetBodyPart(ChooseFrom[c])->GetBodyPartVolume();

  bool Affected = false;

  if(Divide)
    {
      for(c = 0; c < ChooseFrom.size(); ++c)
	if(ReceiveBodyPartDamage(Damager, long(Damage) * GetBodyPart(ChooseFrom[c])->GetBodyPartVolume() / TotalVolume, Type, ChooseFrom[c], Direction, PenetrateArmor, Critical, false))
	  Affected = true;
    }
  else
    Affected = ReceiveBodyPartDamage(Damager, Damage, Type, ChooseFrom[RAND() % ChooseFrom.size()], Direction, PenetrateArmor, Critical, false) != 0;

  if(!Affected && ShowMsg)
    {
      if(IsPlayer())
	ADD_MESSAGE("You are not hurt.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s is not hurt.", GetPersonalPronoun().c_str());
    }

  if(DamageTypeAffectsInventory(Type))
    {
      for(ushort c = 0; c < GetEquipmentSlots(); ++c)
	if(GetEquipment(c))
	  GetEquipment(c)->ReceiveDamage(Damager, Damage, Type);

      GetStack()->ReceiveDamage(Damager, Damage, Type);
    }

  return Affected;
}

arm* humanoid::GetMainArm() const
{
  if(GetRightArm())
    return GetRightArm();
  else
    return GetLeftArm();
}

arm* humanoid::GetSecondaryArm() const
{
  if(GetRightArm())
    return GetLeftArm();
  else
    return 0;
}

void humanoid::SetMainWielded(item* Item)
{
  if(GetMainArm())
    GetMainArm()->SetWielded(Item);
  else
    ABORT("Right hand in wrong place!");
}

void humanoid::SetSecondaryWielded(item* Item)
{
  if(GetSecondaryArm())
    GetSecondaryArm()->SetWielded(Item);
  else
    ABORT("Left hand in wrong place!");
}

std::string humanoid::EquipmentName(ushort Index) const
{
  switch(Index)
    {
    case HELMET_INDEX: return "helmet";
    case AMULET_INDEX: return "amulet";
    case CLOAK_INDEX: return "cloak";
    case BODY_ARMOR_INDEX: return "body armor";
    case BELT_INDEX: return "belt";
    case RIGHT_WIELDED_INDEX: return "right hand wielded";
    case LEFT_WIELDED_INDEX: return "left hand wielded";
    case RIGHT_RING_INDEX: return "right ring";
    case LEFT_RING_INDEX: return "left ring";
    case RIGHT_GAUNTLET_INDEX: return "right gauntlet";
    case LEFT_GAUNTLET_INDEX: return "left gauntlet";
    case RIGHT_BOOT_INDEX: return "right boot";
    case LEFT_BOOT_INDEX: return "left boot";
    default: return "forbidden piece of cloth";
    }
}

bool (*humanoid::EquipmentSorter(ushort Index) const)(item*, const character*)
{
  switch(Index)
    {
    case HELMET_INDEX: return &item::HelmetSorter;
    case AMULET_INDEX: return &item::AmuletSorter;
    case CLOAK_INDEX: return &item::CloakSorter;
    case BODY_ARMOR_INDEX: return &item::BodyArmorSorter;
    case BELT_INDEX: return &item::BeltSorter;
    case RIGHT_WIELDED_INDEX:
    case LEFT_WIELDED_INDEX: return 0;
    case RIGHT_RING_INDEX:
    case LEFT_RING_INDEX: return &item::RingSorter;
    case RIGHT_GAUNTLET_INDEX:
    case LEFT_GAUNTLET_INDEX: return &item::GauntletSorter;
    case RIGHT_BOOT_INDEX:
    case LEFT_BOOT_INDEX: return &item::BootSorter;
    default: return 0;
    }
}

bodypart* humanoid::GetBodyPartOfEquipment(ushort Index) const
{
  switch(Index)
    {
    case HELMET_INDEX:
    case AMULET_INDEX:
      return GetHead();
    case CLOAK_INDEX:
    case BODY_ARMOR_INDEX:
    case BELT_INDEX:
      return GetTorso();
    case RIGHT_WIELDED_INDEX:
    case RIGHT_RING_INDEX:
    case RIGHT_GAUNTLET_INDEX:
      return GetRightArm();
    case LEFT_WIELDED_INDEX:
    case LEFT_RING_INDEX:
    case LEFT_GAUNTLET_INDEX:
      return GetLeftArm();
    case RIGHT_BOOT_INDEX:
      return GetRightLeg();
    case LEFT_BOOT_INDEX:
      return GetLeftLeg();
    default:
      return 0;
    }
}

item* humanoid::GetEquipment(ushort Index) const
{
  switch(Index)
    {
    case HELMET_INDEX: return GetHelmet();
    case AMULET_INDEX: return GetAmulet();
    case CLOAK_INDEX: return GetCloak();
    case BODY_ARMOR_INDEX: return GetBodyArmor();
    case BELT_INDEX: return GetBelt();
    case RIGHT_WIELDED_INDEX: return GetRightWielded();
    case LEFT_WIELDED_INDEX: return GetLeftWielded();
    case RIGHT_RING_INDEX: return GetRightRing();
    case LEFT_RING_INDEX: return GetLeftRing();
    case RIGHT_GAUNTLET_INDEX: return GetRightGauntlet();
    case LEFT_GAUNTLET_INDEX: return GetLeftGauntlet();
    case RIGHT_BOOT_INDEX: return GetRightBoot();
    case LEFT_BOOT_INDEX: return GetLeftBoot();
    default: return 0;
    }
}

void humanoid::SetEquipment(ushort Index, item* What)
{
  switch(Index)
    {
    case HELMET_INDEX: SetHelmet(What); break;
    case AMULET_INDEX: SetAmulet(What); break;
    case CLOAK_INDEX: SetCloak(What); break;
    case BODY_ARMOR_INDEX: SetBodyArmor(What); break;
    case BELT_INDEX: SetBelt(What); break;
    case RIGHT_WIELDED_INDEX: SetRightWielded(What); break;
    case LEFT_WIELDED_INDEX: SetLeftWielded(What); break;
    case RIGHT_RING_INDEX: SetRightRing(What); break;
    case LEFT_RING_INDEX: SetLeftRing(What); break;
    case RIGHT_GAUNTLET_INDEX: SetRightGauntlet(What); break;
    case LEFT_GAUNTLET_INDEX: SetLeftGauntlet(What); break;
    case RIGHT_BOOT_INDEX: SetRightBoot(What); break;
    case LEFT_BOOT_INDEX: SetLeftBoot(What); break;
    }
}

void humanoid::SwitchToDig(item* DigItem, vector2d Square)
{
  dig* Dig = new dig(this);

  if(GetRightArm())
    Dig->SetRightBackup(GetRightArm()->GetWielded()); // slot cleared automatically

  if(GetLeftArm())
    Dig->SetLeftBackup(GetLeftArm()->GetWielded()); // slot cleared automatically

  DigItem->RemoveFromSlot();
  SetMainWielded(DigItem); // bug, this may change arm?
  Dig->SetSquareDug(Square);
  SetAction(Dig);
}

bool humanoid::CheckKick() const
{
  if(!CanKick())
    {
      ADD_MESSAGE("This race can't kick.");
      return false;
    }

  if(GetLegs() < 2)
    {
      std::string LegNumber;

      switch(GetLegs())
	{
	case 0: 
	  LegNumber = "no legs";
	  break;
	case 1:
	  LegNumber = "one leg";
	  break;
	}

      ADD_MESSAGE("How are you you going to do that with %s?", LegNumber.c_str());
      return false;
    }
  else
    return true;
}

uchar humanoid::GetLegs() const
{
  ushort Legs = 0;

  if(GetRightLeg())
    ++Legs;

  if(GetLeftLeg())
    ++Legs;

  return Legs;
}

uchar humanoid::GetArms() const
{
  ushort Arms = 0;

  if(GetRightArm())
    ++Arms;

  if(GetLeftArm())
    ++Arms;

  return Arms;
}

bool humanoid::CheckThrow() const
{
  switch(GetArms())
    {
    case 0:
      ADD_MESSAGE("You don't have an arm to do that!");
      return false;
    default:
      return true;
    }
}

bool humanoid::CheckOffer() const
{
  switch(GetArms())
    {
    case 0:
      ADD_MESSAGE("You need an arm to offer.");
      return false; 
    default:
      return true;
    }
}

void carnivorousplant::GetAICommand()
{
  for(ushort d = 0; d < 8; ++d)
    {
      square* Square = GetNeighbourSquare(d);

      if(Square)
	{
	  character* Char = Square->GetCharacter();

	  if(Char && GetTeam()->GetRelation(Char->GetTeam()) == HOSTILE)
	    {
	      Hit(Char);
	      return;
	    }
	}
    }

  EditAP(-1000);
}

vector2d humanoid::GetEquipmentPanelPos(ushort Index) const
{
  switch(Index)
    {
    case HELMET_INDEX: return vector2d(34, -22);
    case AMULET_INDEX: return vector2d(14, -22);
    case CLOAK_INDEX: return vector2d(-6, -22);
    case BODY_ARMOR_INDEX: return vector2d(54, -22);
    case BELT_INDEX: return vector2d(24, 70);
    case RIGHT_WIELDED_INDEX: return vector2d(-14, 4);
    case LEFT_WIELDED_INDEX: return vector2d(62, 4);
    case RIGHT_RING_INDEX: return vector2d(-14, 44);
    case LEFT_RING_INDEX: return vector2d(62, 44);
    case RIGHT_GAUNTLET_INDEX: return vector2d(-14, 24);
    case LEFT_GAUNTLET_INDEX: return vector2d(62, 24);
    case RIGHT_BOOT_INDEX: return vector2d(4, 70);
    case LEFT_BOOT_INDEX: return vector2d(44, 70);
    default: return vector2d(24, 12);
    }
}

void humanoid::DrawSilhouette(bitmap* ToBitmap, vector2d Where, bool AnimationDraw) const
{
  ushort c;

  for(c = 0; c < GetEquipmentSlots(); ++c)
    if(CanUseEquipment(c))
      {
	vector2d Pos = Where + GetEquipmentPanelPos(c);

	if(!AnimationDraw)
	  DOUBLE_BUFFER->DrawRectangle(Pos + vector2d(-1, -1), Pos + vector2d(16, 16), DARK_GRAY);

	if(GetEquipment(c))
	  {
	    DOUBLE_BUFFER->Fill(Pos, 16, 16, BLACK);
	    GetEquipment(c)->Draw(DOUBLE_BUFFER, Pos, configuration::GetContrastLuminance(), false, true);
	  }
      }

  if(AnimationDraw)
    return;

  ushort Color[4] = { 0, 0, 0, 0 };

  if(GetHead())
    Color[0] = GetHead()->IsInBadCondition() ? MakeRGB16(128,0,0) : LIGHT_GRAY;

  if(GetRightArm())
    Color[1] = GetRightArm()->IsInBadCondition() ? MakeRGB16(128,0,0) : LIGHT_GRAY;

  if(GetLeftArm())
    Color[2] = GetLeftArm()->IsInBadCondition() ? MakeRGB16(128,0,0) : LIGHT_GRAY;

  if(GetTorso())
    Color[3] = GetTorso()->IsInBadCondition() ? MakeRGB16(128,0,0) : LIGHT_GRAY;

  igraph::GetCharacterRawGraphic()->MaskedBlit(ToBitmap, 0, 64, Where.X, Where.Y, SILHOUETTE_X_SIZE, SILHOUETTE_Y_SIZE, Color);

  for(c = 0; c < 4; ++c)
    Color[c] = 0;

  if(GetGroin())
    Color[1] = GetGroin()->IsInBadCondition() ? MakeRGB16(128,0,0) : LIGHT_GRAY;

  if(GetRightLeg())
    Color[2] = GetRightLeg()->IsInBadCondition() ? MakeRGB16(128,0,0) : LIGHT_GRAY;

  if(GetLeftLeg())
    Color[3] = GetLeftLeg()->IsInBadCondition() ? MakeRGB16(128,0,0) : LIGHT_GRAY;

  igraph::GetCharacterRawGraphic()->MaskedBlit(ToBitmap, 64, 64, Where, SILHOUETTE_X_SIZE, SILHOUETTE_Y_SIZE, Color);
}

ushort humanoid::GlobalResistance(uchar Type) const
{
  ushort Resistance = GetResistance(Type);

  if(GetCloak())
    Resistance += GetCloak()->GetResistance(Type);

  if(Type != PHYSICAL_DAMAGE)
    {
      if(GetAmulet())
	Resistance += GetAmulet()->GetResistance(Type);

      if(GetRightRing())
	Resistance += GetRightRing()->GetResistance(Type);

      if(GetLeftRing())
	Resistance += GetLeftRing()->GetResistance(Type);
    }

  return Resistance;
}

void humanoid::AddInfo(felist& Info) const
{
  if(!game::WizardModeActivated())
    return;

  if(GetRightArm())
    {
      Info.AddEntry(std::string("Right arm strength: ") + GetRightArm()->GetAttribute(ARM_STRENGTH), LIGHT_GRAY);
      Info.AddEntry(std::string("Right arm dexterity: ") + GetRightArm()->GetAttribute(DEXTERITY), LIGHT_GRAY);
    }

  if(GetLeftArm())
    {
      Info.AddEntry(std::string("Left arm strength: ") + GetLeftArm()->GetAttribute(ARM_STRENGTH), LIGHT_GRAY);
      Info.AddEntry(std::string("Left arm dexterity: ") + GetLeftArm()->GetAttribute(DEXTERITY), LIGHT_GRAY);
    }

  if(GetRightLeg())
    {
      Info.AddEntry(std::string("Right leg strength: ") + GetRightLeg()->GetAttribute(LEG_STRENGTH), LIGHT_GRAY);
      Info.AddEntry(std::string("Right leg agility: ") + GetRightLeg()->GetAttribute(AGILITY), LIGHT_GRAY);
    }

  if(GetLeftLeg())
    {
      Info.AddEntry(std::string("Left leg strength: ") + GetLeftLeg()->GetAttribute(LEG_STRENGTH), LIGHT_GRAY);
      Info.AddEntry(std::string("Left leg agility: ") + GetLeftLeg()->GetAttribute(AGILITY), LIGHT_GRAY);
    }

  if(GetRightArm())
    {
      Info.AddEntry(std::string("Right damage: ") + ushort(GetRightArm()->GetDamage()), LIGHT_GRAY);
      Info.AddEntry(std::string("Right to hit value: ") + ulong(GetRightArm()->GetToHitValue()), LIGHT_GRAY);
    }

  if(GetLeftArm())
    {
      Info.AddEntry(std::string("Left damage: ") + ushort(GetLeftArm()->GetDamage()), LIGHT_GRAY);
      Info.AddEntry(std::string("Left to hit value: ") + ulong(GetLeftArm()->GetToHitValue()), LIGHT_GRAY);
    }

  if(GetRightLeg())
    {
      Info.AddEntry(std::string("Right kick damage: ") + ushort(GetRightLeg()->GetKickDamage()), LIGHT_GRAY);
      Info.AddEntry(std::string("Right kick to hit value: ") + ulong(GetRightLeg()->GetKickToHitValue()), LIGHT_GRAY);
    }

  if(GetLeftLeg())
    {
      Info.AddEntry(std::string("Left kick damage: ") + ushort(GetLeftLeg()->GetKickDamage()), LIGHT_GRAY);
      Info.AddEntry(std::string("Left kick to hit value: ") + ulong(GetLeftLeg()->GetKickToHitValue()), LIGHT_GRAY);
    }

  if(GetHead())
    {
      Info.AddEntry(std::string("Bite damage: ") + ushort(GetHead()->GetBiteDamage()), LIGHT_GRAY);
      Info.AddEntry(std::string("Bite to hit value: ") + ulong(GetHead()->GetBiteToHitValue()), LIGHT_GRAY);
    }
}

void humanoid::CompleteRiseFromTheDead()
{
  ushort c;

  for(c = 0; c < GetBodyParts(); ++c)
    if(!GetBodyPart(c))
      {
	/* Let's search for the original bodypart */

	for(stackiterator i = GetStackUnder()->GetBottom(); i.HasItem(); ++i)
	  if(i->GetID() == GetOriginalBodyPartID(c))
	    {
	      item* Item = *i;
	      Item->RemoveFromSlot();
	      SetBodyPart(c, static_cast<bodypart*>(Item));
	      break;
	    }
      }

  for(c = 0; c < GetBodyParts(); ++c)
    {
      if(BodyPartVital(c) && !GetBodyPart(c))
	if(!HandleNoBodyPart(c))
	  return;

      if(GetBodyPart(c))
	GetBodyPart(c)->SetHP(1);
    }
}

bool humanoid::HandleNoBodyPart(ushort Index)
{
  switch(Index)
    {
    case HEAD_INDEX:
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("The headless body of %s vibrates violently.", CHAR_NAME(DEFINITE));

      Die();
      return false;
    case GROIN_INDEX:
      if(CanBeSeenByPlayer())
	ADD_MESSAGE("The groinless body of %s vibrates violently.", CHAR_NAME(DEFINITE));

      Die();
      return true;
    case TORSO_INDEX:
      ABORT("The corpse does not have a torso.");
    default:
      return false;
    }
}

vector2d humanoid::GetBodyPartBitmapPos(ushort Index)
{
  switch(Index)
    {
    case TORSO_INDEX: return GetTorsoBitmapPos();
    case HEAD_INDEX: return GetHeadBitmapPos();
    case RIGHT_ARMINDEX: return GetRightArmBitmapPos();
    case LEFT_ARMINDEX: return GetLeftArmBitmapPos();
    case GROIN_INDEX: return GetGroinBitmapPos();
    case RIGHT_LEGINDEX: return GetRightLegBitmapPos();
    case LEFT_LEGINDEX: return GetLeftLegBitmapPos();
    default:
      ABORT("Weird bodypart BitmapPos request for a humanoid!");
      return vector2d();
    }
}

ushort humanoid::GetBodyPartColorB(ushort Index)
{
  switch(Index)
    {
    case TORSO_INDEX: return GetTorsoMainColor();
    case HEAD_INDEX: return GetCapColor();
    case RIGHT_ARMINDEX:
    case LEFT_ARMINDEX: return GetArmMainColor();
    case GROIN_INDEX:
    case RIGHT_LEGINDEX:
    case LEFT_LEGINDEX: return GetLegMainColor();
    default:
      ABORT("Weird bodypart color B request for a humanoid!");
      return 0;
    }
}

ushort humanoid::GetBodyPartColorC(ushort Index)
{
  switch(Index)
    {
    case TORSO_INDEX: return GetBeltColor();
    case HEAD_INDEX: return GetHairColor();
    case RIGHT_ARMINDEX:
    case LEFT_ARMINDEX:
    case GROIN_INDEX:
    case RIGHT_LEGINDEX:
    case LEFT_LEGINDEX: return 0; // reserved for future use
    default:
      ABORT("Weird bodypart color C request for a humanoid!");
      return 0;
    }
}

ushort humanoid::GetBodyPartColorD(ushort Index)
{
  switch(Index)
    {
    case TORSO_INDEX: return GetTorsoSpecialColor();
    case HEAD_INDEX: return GetEyeColor();
    case RIGHT_ARMINDEX:
    case LEFT_ARMINDEX: return GetArmSpecialColor();
    case GROIN_INDEX:
    case RIGHT_LEGINDEX:
    case LEFT_LEGINDEX: return GetLegSpecialColor();
    default:
      ABORT("Weird bodypart color D request for a humanoid!");
      return 0;
    }
}

void human::VirtualConstructor(bool Load)
{
  humanoid::VirtualConstructor(Load);

  if(!Load)
    {
      EditAttribute(ARM_STRENGTH, (RAND() & 1) - (RAND() & 1));
      EditAttribute(DEXTERITY, (RAND() & 1) - (RAND() & 1));
      EditAttribute(LEG_STRENGTH, (RAND() & 1) - (RAND() & 1));
      EditAttribute(AGILITY, (RAND() & 1) - (RAND() & 1));
      EditAttribute(ENDURANCE, (RAND() & 1) - (RAND() & 1));
      EditAttribute(PERCEPTION, (RAND() & 1) - (RAND() & 1));
      EditAttribute(INTELLIGENCE, (RAND() & 1) - (RAND() & 1));
      EditAttribute(WISDOM, (RAND() & 1) - (RAND() & 1));
      EditAttribute(CHARISMA, (RAND() & 1) - (RAND() & 1));
      EditAttribute(MANA, (RAND() & 1) - (RAND() & 1));
      SetMoney(GetMoney() + RAND() % 101);
      SetTotalSize(character::GetTotalSize() + RAND() % 51);
      SetSize(GetTotalSize());
    }
}

void petrus::VirtualConstructor(bool Load)
{
  humanoid::VirtualConstructor(Load);
  game::SetPetrus(this);

  if(!Load)
    {
      SetHealTimer(16384);
      SetStoryState(0);
      SetAssignedName("Petrus");
    }
}

void shopkeeper::VirtualConstructor(bool Load)
{
  humanoid::VirtualConstructor(Load);

  if(!Load)
    SetMoney(GetMoney() + RAND() % 2001);
}

void oree::VirtualConstructor(bool Load)
{
  humanoid::VirtualConstructor(Load);
  
  if(!Load)
    SetAssignedName("Oree");
}

void elpuri::VirtualConstructor(bool Load)
{
  character::VirtualConstructor(Load);
  
  if(!Load)
    SetAssignedName("Elpuri");
}

void communist::VirtualConstructor(bool Load)
{
  humanoid::VirtualConstructor(Load);
  
  if(!Load)
    SetAssignedName("Ivan");
}

void unicorn::CreateBodyParts(ushort SpecialFlags)
{
  SetAlignment(RAND() % 3);
  character::CreateBodyParts(SpecialFlags);
}

void human::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << TotalSize;
}

void human::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> TotalSize;
}

void nonhumanoid::Save(outputfile& SaveFile) const
{
  character::Save(SaveFile);
  SaveFile << Strength << Agility << StrengthExperience << AgilityExperience;
}

void nonhumanoid::Load(inputfile& SaveFile)
{
  character::Load(SaveFile);
  SaveFile >> Strength >> Agility >> StrengthExperience >> AgilityExperience;
}

void nonhumanoid::CalculateUnarmedDamage()
{
  UnarmedDamage = float(GetBaseUnarmedStrength()) * GetAttribute(ARM_STRENGTH) * GetCWeaponSkill(UNARMED)->GetEffectBonus() / 5000000;
}

void nonhumanoid::CalculateUnarmedToHitValue()
{
  UnarmedToHitValue = float((GetAttribute(DEXTERITY) << 2) + GetAttribute(PERCEPTION)) * GetCWeaponSkill(UNARMED)->GetEffectBonus() * GetMoveEase() / 10000;
}

void nonhumanoid::CalculateUnarmedAPCost()
{
  UnarmedAPCost = long(float(GetCWeaponSkill(UNARMED)->GetAPBonus()) * (200 - GetAttribute(DEXTERITY)) * 5 / GetMoveEase());

  if(UnarmedAPCost < 100)
    UnarmedAPCost = 100;
}

void nonhumanoid::CalculateKickDamage()
{
  KickDamage = float(GetBaseKickStrength()) * GetAttribute(LEG_STRENGTH) * GetCWeaponSkill(KICK)->GetEffectBonus() / 5000000;
}

void nonhumanoid::CalculateKickToHitValue()
{
  KickToHitValue = float((GetAttribute(AGILITY) << 2) + GetAttribute(PERCEPTION)) * GetCWeaponSkill(KICK)->GetEffectBonus() * GetMoveEase() / 20000;
}

void nonhumanoid::CalculateKickAPCost()
{
  KickAPCost = long(float(GetCWeaponSkill(KICK)->GetAPBonus()) * (200 - GetAttribute(AGILITY)) * 10 / GetMoveEase());

  if(KickAPCost < 100)
    KickAPCost = 100;
}

void nonhumanoid::CalculateBiteDamage()
{
  BiteDamage = float(GetBaseBiteStrength()) * GetAttribute(ARM_STRENGTH) * GetCWeaponSkill(BITE)->GetEffectBonus() / 5000000;
}

void nonhumanoid::CalculateBiteToHitValue()
{
  BiteToHitValue = float((GetAttribute(DEXTERITY) << 2) + GetAttribute(PERCEPTION)) * GetCWeaponSkill(BITE)->GetEffectBonus() * GetMoveEase() / 20000;
}

void nonhumanoid::CalculateBiteAPCost()
{
  BiteAPCost = long(float(GetCWeaponSkill(BITE)->GetAPBonus()) * (200 - GetAttribute(DEXTERITY)) * 5 / GetMoveEase());

  if(BiteAPCost < 100)
    BiteAPCost = 100;
}

void nonhumanoid::InitSpecialAttributes()
{
  Strength = (GetDefaultArmStrength() * (100 + GetAttributeBonus()) / 100) << 1;
  Agility = (GetDefaultAgility() * (100 + GetAttributeBonus()) / 100) << 1;
  StrengthExperience = AgilityExperience = 0;
}

void humanoid::Bite(character* Enemy)
{
  /* This function ought not to be called without a head */

  EditNP(-50);
  EditAP(-GetHead()->GetBiteAPCost());
  EditExperience(AGILITY, 75);
  Enemy->TakeHit(this, 0, GetHead()->GetBiteDamage(), GetHead()->GetBiteToHitValue(), RAND() % 26 - RAND() % 26, BITE_ATTACK, !(RAND() % GetCriticalModifier()));
}

void nonhumanoid::Bite(character* Enemy)
{
  EditNP(-50);
  EditAP(-GetBiteAPCost());
  EditExperience(AGILITY, 75);
  Enemy->TakeHit(this, 0, GetBiteDamage(), GetBiteToHitValue(), RAND() % 26 - RAND() % 26, BITE_ATTACK, !(RAND() % GetCriticalModifier()));
}

void humanoid::Kick(lsquare* Square)
{
  leg* KickLeg = GetKickLeg();
  EditNP(-50);
  EditAP(-KickLeg->GetKickAPCost());
  KickLeg->EditExperience(LEG_STRENGTH, 25);
  KickLeg->EditExperience(AGILITY, 50);
  Square->BeKicked(this, KickLeg->GetBoot(), KickLeg->GetKickDamage(), KickLeg->GetKickToHitValue(), RAND() % 26 - RAND() % 26, !(RAND() % GetCriticalModifier()));
}

void nonhumanoid::Kick(lsquare* Square)
{
  EditNP(-50);
  EditAP(-GetKickAPCost());
  EditExperience(LEG_STRENGTH, 25);
  EditExperience(AGILITY, 50);
  Square->BeKicked(this, 0, GetKickDamage(), GetKickToHitValue(), RAND() % 26 - RAND() % 26, !(RAND() % GetCriticalModifier()));
}

bool nonhumanoid::Hit(character* Enemy)
{
  if(IsPlayer() && GetTeam()->GetRelation(Enemy->GetTeam()) != HOSTILE && !game::BoolQuestion("This might cause a hostile reaction. Are you sure? [y/N]"))
    return false;

  Hostility(Enemy);

  if(GetBurdenState() == OVER_LOADED)
    {
      if(IsPlayer())
	ADD_MESSAGE("You cannot fight while carrying so much.");

      return true;
    }

  ushort c, AttackStyles;

  for(c = 0, AttackStyles = 0; c < 8; ++c)
    if(GetAttackStyle() & (1 << c))
      ++AttackStyles;

  ushort Chosen = RAND() % AttackStyles;

  for(c = 0, AttackStyles = 0; c < 8; ++c)
    if(GetAttackStyle() & (1 << c) && AttackStyles++ == Chosen)
      {
	Chosen = 1 << c;
	break;
      }

  switch(Chosen)
    {
    case USE_ARMS:
      UnarmedHit(Enemy);
      return true;
    case USE_LEGS:
      Kick(Enemy->GetLSquareUnder());
      return true;
    case USE_HEAD:
      Bite(Enemy);
      return true;
    default:
      ADD_MESSAGE("Strange alien attack style requested!");
      return false;
    }
}

void nonhumanoid::AddInfo(felist& Info) const
{
  if(!game::WizardModeActivated())
    return;

  Info.AddEntry(std::string("Strength: ") + GetAttribute(ARM_STRENGTH), LIGHT_GRAY);
  Info.AddEntry(std::string("Agility: ") + GetAttribute(AGILITY), LIGHT_GRAY);
  Info.AddEntry(std::string("Unarmed damage: ") + ushort(GetUnarmedDamage()), LIGHT_GRAY);
  Info.AddEntry(std::string("Unarmed to hit value: ") + ulong(GetUnarmedToHitValue()), LIGHT_GRAY);
  Info.AddEntry(std::string("Kick damage: ") + ushort(GetKickDamage()), LIGHT_GRAY);
  Info.AddEntry(std::string("Kick to hit value: ") + ulong(GetKickToHitValue()), LIGHT_GRAY);
  Info.AddEntry(std::string("Bite damage: ") + ushort(GetBiteDamage()), LIGHT_GRAY);
  Info.AddEntry(std::string("Bite to hit value: ") + ulong(GetBiteToHitValue()), LIGHT_GRAY);
}

void nonhumanoid::UnarmedHit(character* Enemy)
{
  EditNP(-50);
  EditAP(-GetUnarmedAPCost());

  switch(Enemy->TakeHit(this, 0, GetUnarmedDamage(), GetUnarmedToHitValue(), RAND() % 26 - RAND() % 26, UNARMED_ATTACK, !(RAND() % GetCriticalModifier())))
    {
    case HAS_HIT:
    case HAS_BLOCKED:
    case HAS_DIED:
    case DID_NO_DAMAGE:
      EditExperience(ARM_STRENGTH, 50);
    case HAS_DODGED:
      EditExperience(DEXTERITY, 25);
    }
}

/* Returns the average number of APs required to kill Enemy */

float humanoid::GetTimeToKill(const character* Enemy, bool UseMaxHP) const
{
  float Effectivity = 0;
  ushort AttackStyles = 0;

  if(IsUsingArms())
    {
      if(GetRightArm() && GetRightArm()->GetDamage())
	Effectivity += 1 / (Enemy->GetTimeToDie(this, ushort(GetRightArm()->GetDamage()), GetRightArm()->GetToHitValue(), AttackIsBlockable(GetRightWielded() ? WEAPON_ATTACK : UNARMED_ATTACK), UseMaxHP) * GetRightArm()->GetAPCost());

      if(GetLeftArm() && GetLeftArm()->GetDamage())
	Effectivity += 1 / (Enemy->GetTimeToDie(this, ushort(GetLeftArm()->GetDamage()), GetLeftArm()->GetToHitValue(), AttackIsBlockable(GetLeftWielded() ? WEAPON_ATTACK : UNARMED_ATTACK), UseMaxHP) * GetLeftArm()->GetAPCost());

      ++AttackStyles;
    }

  if(IsUsingLegs())
    {
      leg* KickLeg = GetKickLeg();
      Effectivity += 1 / (Enemy->GetTimeToDie(this, ushort(KickLeg->GetKickDamage()), KickLeg->GetKickToHitValue(), AttackIsBlockable(KICK_ATTACK), UseMaxHP) * KickLeg->GetKickAPCost());
      ++AttackStyles;
    }

  if(IsUsingHead())
    {
      Effectivity += 1 / (Enemy->GetTimeToDie(this, ushort(GetHead()->GetBiteDamage()), GetHead()->GetBiteToHitValue(), AttackIsBlockable(BITE_ATTACK), UseMaxHP) * GetHead()->GetBiteAPCost());
      ++AttackStyles;
    }

  return AttackStyles ? AttackStyles / Effectivity : 10000000;
}

/* Returns the average number of APs required to kill Enemy */

float nonhumanoid::GetTimeToKill(const character* Enemy, bool UseMaxHP) const
{
  float Effectivity = 0;
  ushort AttackStyles = 0;

  if(IsUsingArms())
    {
      Effectivity += 1 / (Enemy->GetTimeToDie(this, ushort(GetUnarmedDamage()), GetUnarmedToHitValue(), AttackIsBlockable(UNARMED_ATTACK), UseMaxHP) * GetUnarmedAPCost());
      ++AttackStyles;
    }

  if(IsUsingLegs())
    {
      Effectivity += 1 / (Enemy->GetTimeToDie(this, ushort(GetKickDamage()), GetKickToHitValue(), AttackIsBlockable(KICK_ATTACK), UseMaxHP) * GetKickAPCost());
      ++AttackStyles;
    }

  if(IsUsingHead())
    {
      Effectivity += 1 / (Enemy->GetTimeToDie(this, ushort(GetBiteDamage()), GetBiteToHitValue(), AttackIsBlockable(BITE_ATTACK), UseMaxHP) * GetBiteAPCost());
      ++AttackStyles;
    }

  return AttackStyles / Effectivity;
}

void nonhumanoid::ApplyExperience(bool Edited)
{
  if(!IsAlive())
    {
      character::ApplyExperience(false);
      return;
    }

  if(CheckForAttributeIncrease(Strength, StrengthExperience, true))
    {
      if(IsPlayer())
	ADD_MESSAGE("Your feel stronger!");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks stronger.", CHAR_NAME(DEFINITE));

      CalculateBurdenState();
      Edited = true;
    }
  else if(CheckForAttributeDecrease(Strength, StrengthExperience, true))
    {
      if(IsPlayer())
	ADD_MESSAGE("Your feel weaker.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks weaker.", CHAR_NAME(DEFINITE));

      CalculateBurdenState();
      Edited = true;
    }

  if(CheckForAttributeIncrease(Agility, AgilityExperience, true))
    {
      if(IsPlayer())
	ADD_MESSAGE("Your feel very agile!");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks very agile.", CHAR_NAME(DEFINITE));

      Edited = true;
    }
  else if(CheckForAttributeDecrease(Agility, AgilityExperience, true))
    {
      if(IsPlayer())
	ADD_MESSAGE("Your feel slower.");
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("Suddenly %s looks sluggish.", CHAR_NAME(DEFINITE));

      Edited = true;
    }

  character::ApplyExperience(Edited);
}

ushort humanoid::GetAttribute(ushort Identifier) const
{
  if(Identifier < BASE_ATTRIBUTES)
    return character::GetAttribute(Identifier);
  else
    {
      ushort Attrib = 0;

       if(Identifier == ARM_STRENGTH || Identifier == DEXTERITY)
	{
	  if(GetRightArm())
	    Attrib += GetRightArm()->GetAttribute(Identifier);

	  if(GetLeftArm())
	    Attrib += GetLeftArm()->GetAttribute(Identifier);
	}
      else if(Identifier == LEG_STRENGTH || Identifier == AGILITY)
	{
	  if(GetRightLeg())
	    Attrib += GetRightLeg()->GetAttribute(Identifier);

	  if(GetLeftLeg())
	    Attrib += GetLeftLeg()->GetAttribute(Identifier);
	}
      else
	{
	  ABORT("Illegal humanoid attribute %d request!", Identifier);
	  return 0xEBBA;
	}

      return Attrib >> 1;
    }
}

bool humanoid::EditAttribute(ushort Identifier, short Value)
{
  if(Identifier < BASE_ATTRIBUTES)
    return character::EditAttribute(Identifier, Value);
  else if(Identifier == ARM_STRENGTH || Identifier == DEXTERITY)
    {
      bool Success = false;

      if(GetRightArm() && GetRightArm()->EditAttribute(Identifier, Value))
	Success = true;

      if(GetLeftArm() && GetLeftArm()->EditAttribute(Identifier, Value))
	Success = true;

      return Success;
    }
  else if(Identifier == LEG_STRENGTH || Identifier == AGILITY)
    {
      bool Success = false;

      if(GetRightLeg() && GetRightLeg()->EditAttribute(Identifier, Value))
	Success = true;

      if(GetLeftLeg() && GetLeftLeg()->EditAttribute(Identifier, Value))
	Success = true;

      return Success;
    }
  else
    {
      ABORT("Illegal humanoid attribute %d edit request!", Identifier);
      return false;
    }
}

void humanoid::EditExperience(ushort Identifier, long Value)
{
  if(Identifier < BASE_ATTRIBUTES)
    character::EditExperience(Identifier, Value);
  else if(Identifier == ARM_STRENGTH || Identifier == DEXTERITY)
    {
      if(GetRightArm())
	GetRightArm()->EditExperience(Identifier, Value >> 1);

      if(GetLeftArm())
	GetLeftArm()->EditExperience(Identifier, Value >> 1);
    }
  else if(Identifier == LEG_STRENGTH || Identifier == AGILITY)
    {
      if(GetRightLeg())
	GetRightLeg()->EditExperience(Identifier, Value >> 1);

      if(GetLeftLeg())
	GetLeftLeg()->EditExperience(Identifier, Value >> 1);
    }
  else
    ABORT("Illegal humanoid attribute %d experience edit request!", Identifier);
}

ushort nonhumanoid::GetAttribute(ushort Identifier) const
{
  if(Identifier < BASE_ATTRIBUTES)
    return character::GetAttribute(Identifier);
  else if(Identifier == ARM_STRENGTH || Identifier == LEG_STRENGTH)
    {
      if(IsAlive())
	return Strength >> 1;
      else
	return GetTorso()->GetMainMaterial()->GetStrengthValue();
    }
  else if(Identifier == DEXTERITY || Identifier == AGILITY)
    {
      if(IsAlive())
	return Agility >> 1;
      else
	return GetTorso()->GetMainMaterial()->GetFlexibility();
    }
  else
    {
      ABORT("Illegal nonhumanoid attribute %d request!", Identifier);
      return 0xABBE;
    }
}

bool nonhumanoid::EditAttribute(ushort Identifier, short Value)
{
  if(Identifier < BASE_ATTRIBUTES)
    return character::EditAttribute(Identifier, Value);
  else if(Identifier == ARM_STRENGTH || Identifier == LEG_STRENGTH)
    return IsAlive() && RawEditAttribute(Strength, Value, true);
  else if(Identifier == DEXTERITY || Identifier == AGILITY)
    return IsAlive() && RawEditAttribute(Agility, Value, true);
  else
    {
      ABORT("Illegal nonhumanoid attribute %d edit request!", Identifier);
      return false;
    }
}

void nonhumanoid::EditExperience(ushort Identifier, long Value)
{
  if(Identifier < BASE_ATTRIBUTES)
    character::EditExperience(Identifier, Value);
  else if(Identifier == ARM_STRENGTH || Identifier == LEG_STRENGTH)
    {
      if(IsAlive())
	StrengthExperience += Value;
    }
  else if(Identifier == DEXTERITY || Identifier == AGILITY)
    {
      if(IsAlive())
	AgilityExperience += Value;
    }
  else
    ABORT("Illegal nonhumanoid attribute %d experience edit request!", Identifier);
}

bool nonhumanoid::RaiseStats()
{
  Strength += 20;
  Agility += 20;
  return character::RaiseStats();
}

bool nonhumanoid::LowerStats()
{
  Strength -= 20;
  Agility -= 20;
  return character::LowerStats();
}

ushort humanoid::DrawStats(bool AnimationDraw) const
{
  DrawSilhouette(DOUBLE_BUFFER, vector2d(RES.X - SILHOUETTE_X_SIZE - 23, 53), AnimationDraw);

  if(AnimationDraw)
    return 15;

  ushort PanelPosX = RES.X - 96, PanelPosY = 15;

  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "ArmStr %d", GetAttribute(ARM_STRENGTH));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "LegStr %d", GetAttribute(LEG_STRENGTH));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Dex %d", GetAttribute(DEXTERITY));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Agi %d", GetAttribute(AGILITY));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "End %d", GetAttribute(ENDURANCE));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Per %d", GetAttribute(PERCEPTION));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Int %d", GetAttribute(INTELLIGENCE));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Wis %d", GetAttribute(WISDOM));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Cha %d", GetAttribute(CHARISMA));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Siz %d", GetSize());
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, IsInBadCondition() ? RED : WHITE, "HP %d/%d", GetHP(), GetMaxHP());
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Gold: %d", GetMoney());

  ++PanelPosY;

  if(GetAttackStyle() & USE_ARMS)
    {
      if(GetRightArm())
	if(GetRightWielded() && GetRightWielded()->IsShield(this))
	  {
	    FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "RBS %d", GetRightArm()->GetBlockCapability());
	    FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "RBV %.0f", GetRightArm()->GetBlockValue());
	  }
	else if(GetRightArm()->GetDamage())
	  {
	    FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "RDAM %d-%d", GetRightArm()->GetMinDamage(), GetRightArm()->GetMaxDamage());
	    FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "RTHV %.0f", GetRightArm()->GetToHitValue());
	    FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "RAPC %d", GetRightArm()->GetAPCost());
	  }

      if(GetLeftArm())
	if(GetLeftWielded() && GetLeftWielded()->IsShield(this))
	  {
	    FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "LBS %d", GetLeftArm()->GetBlockCapability());
	    FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "LBV %.0f", GetLeftArm()->GetBlockValue());
	  }
	else if(GetLeftArm()->GetDamage())
	  {
	    FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "LDAM %d-%d", GetLeftArm()->GetMinDamage(), GetLeftArm()->GetMaxDamage());
	    FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "LTHV %.0f", GetLeftArm()->GetToHitValue());
	    FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "LAPC %d", GetLeftArm()->GetAPCost());
	  }
    }

  if(IsUsingLegs())
    {
      leg* Leg = GetKickLeg();
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "KDAM %d-%d", Leg->GetKickMinDamage(), Leg->GetKickMaxDamage());
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "KTHV %.0f", Leg->GetKickToHitValue());
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "KAPC %d", Leg->GetKickAPCost());
    }

  if(IsUsingHead())
    {
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "BDAM %d-%d", GetHead()->GetBiteMinDamage(), GetHead()->GetBiteMaxDamage());
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "BTHV %.0f", GetHead()->GetBiteToHitValue());
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "BAPC %d", GetHead()->GetBiteAPCost());
    }

  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "DV: %.0f", GetDodgeValue());
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "MAPC: %d", -GetMoveAPRequirement(1));

  if(game::WizardModeActivated())
    FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "NP: %d", GetNP());

  return PanelPosY;
}

ushort nonhumanoid::DrawStats(bool AnimationDraw) const
{
  if(AnimationDraw)
    return 3;

  ushort PanelPosX = RES.X - 96, PanelPosY = 3;

  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Str %d", GetAttribute(ARM_STRENGTH));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Agi %d", GetAttribute(AGILITY));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "End %d", GetAttribute(ENDURANCE));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Per %d", GetAttribute(PERCEPTION));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Int %d", GetAttribute(INTELLIGENCE));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Wis %d", GetAttribute(WISDOM));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Cha %d", GetAttribute(CHARISMA));
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Siz %d", GetSize());
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, IsInBadCondition() ? RED : WHITE, "HP %d/%d", GetHP(), GetMaxHP());
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "Gold: %d", GetMoney());

  ++PanelPosY;

  if(GetAttackStyle() & USE_ARMS)
    {
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "DAM %d-%d", GetUnarmedMinDamage(), GetUnarmedMaxDamage());
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "THV %.0f", GetUnarmedToHitValue());
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "APC %d", GetUnarmedAPCost());
    }

  if(GetAttackStyle() & USE_LEGS)
    {
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "KDAM %d-%d", GetKickMinDamage(), GetKickMaxDamage());
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "KTHV %.0f", GetKickToHitValue());
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "KAPC %d", GetKickAPCost());
    }

  if(GetAttackStyle() & USE_HEAD)
    {
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "BDAM %d-%d", GetBiteMinDamage(), GetBiteMaxDamage());
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "BTHV %.0f", GetBiteToHitValue());
      FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "BAPC %d", GetBiteAPCost());
    }

  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "DV: %.0f", GetDodgeValue());
  FONT->Printf(DOUBLE_BUFFER, PanelPosX, (PanelPosY++) * 10, WHITE, "MAPC: %d", -GetMoveAPRequirement(1));
  return PanelPosY;
}

ushort humanoid::GetRandomStepperBodyPart() const
{
  uchar Possible = 0, PossibleArray[3];

  if(GetRightLeg())
    PossibleArray[Possible++] = RIGHT_LEGINDEX;

  if(GetLeftLeg())
    PossibleArray[Possible++] = LEFT_LEGINDEX;

  if(Possible)
    return PossibleArray[RAND() % Possible];

  if(GetHead())
    PossibleArray[Possible++] = HEAD_INDEX;

  if(GetGroin())
    PossibleArray[Possible++] = GROIN_INDEX;

  PossibleArray[Possible++] = TORSO_INDEX;
  return PossibleArray[RAND() % Possible];
}

ushort humanoid::CheckForBlock(character* Enemy, item* Weapon, float ToHitValue, ushort Damage, short Success, uchar Type)
{
  if(GetAction())
    return Damage;

  if(GetRightWielded())
    Damage = CheckForBlockWithArm(Enemy, Weapon, GetRightArm(), ToHitValue, Damage, Success, Type);

  if(Damage && GetLeftWielded() && (!Weapon || Weapon->Exists()))
    Damage = CheckForBlockWithArm(Enemy, Weapon, GetLeftArm(), ToHitValue, Damage, Success, Type);

  return Damage;
}

bool humanoid::CanWield() const
{
  return CanUseEquipment(RIGHT_WIELDED_INDEX) || CanUseEquipment(LEFT_WIELDED_INDEX);
}

bool humanoid::CheckBalance(float KickDamage)
{
  if(GetLegs() == 1)
    return true;
  else
    return KickDamage * 50 >= RAND() % GetSize();
}

long humanoid::GetMoveAPRequirement(uchar Difficulty) const
{
  if(CanFly())
    return (long(GetAttribute(AGILITY)) - 200) * Difficulty * 500 / GetMoveEase();

  switch(GetLegs())
    {
    case 0:
      return (long(GetAttribute(AGILITY)) - 200) * Difficulty * 5000 / GetMoveEase();
    case 1:
      return (long(GetAttribute(AGILITY)) - 200) * Difficulty * 1500 / GetMoveEase();
    case 2:
      return (long(GetAttribute(AGILITY)) - 200) * Difficulty * 500 / GetMoveEase();
    default:
      ABORT("A %d legged humanoid invaded the dungeon!", GetLegs());
      return 0;
    }
}

bool humanoid::EquipmentHasNoPairProblems(ushort Index) const
{
  switch(Index)
    {
    case RIGHT_GAUNTLET_INDEX:
      return GetLeftGauntlet() && GetLeftGauntlet()->IsSimiliarTo(GetRightGauntlet());
    case LEFT_GAUNTLET_INDEX:
      return GetRightGauntlet() && GetRightGauntlet()->IsSimiliarTo(GetLeftGauntlet());
    case RIGHT_BOOT_INDEX:
      return GetLeftBoot() && GetLeftBoot()->IsSimiliarTo(GetRightBoot());
    case LEFT_BOOT_INDEX:
      return GetRightBoot() && GetRightBoot()->IsSimiliarTo(GetLeftBoot());
    }

  return true;
}

void hunter::CreateBodyParts(ushort SpecialFlags)
{
  for(ushort c = 0; c < GetBodyParts(); ++c) 
    if(c != LEFT_ARMINDEX)
      CreateBodyPart(c, SpecialFlags);
    else
      SetLeftArm(0);
}

bool humanoid::EquipmentEasilyRecognized(ushort Index) const
{
  switch(Index)
    {
    case AMULET_INDEX:
    case RIGHT_RING_INDEX:
    case LEFT_RING_INDEX:
    case BELT_INDEX:
      return false;
    }

  return true;
}

void humanoid::VirtualConstructor(bool)
{
  SetCurrentRightSWeaponSkill(0);
  SetCurrentLeftSWeaponSkill(0);
}

void humanoid::SignalEquipmentAdd(ushort EquipmentIndex)
{
  character::SignalEquipmentAdd(EquipmentIndex);

  if(EquipmentIndex == RIGHT_WIELDED_INDEX)
    {
      for(ushort c = 0; c < SWeaponSkill.size(); ++c)
	if(SWeaponSkill[c]->GetID() == GetRightWielded()->GetID())
	  {
	    SetCurrentRightSWeaponSkill(SWeaponSkill[c]);
	    break;
	  }

      if(!GetCurrentRightSWeaponSkill())
	{
	  SetCurrentRightSWeaponSkill(new sweaponskill);
	  GetCurrentRightSWeaponSkill()->SetID(GetRightWielded()->GetID());
	  SWeaponSkill.push_back(GetCurrentRightSWeaponSkill());
	}
    }
  else if(EquipmentIndex == LEFT_WIELDED_INDEX)
    {
      for(ushort c = 0; c < SWeaponSkill.size(); ++c)
	if(SWeaponSkill[c]->GetID() == GetLeftWielded()->GetID())
	  {
	    SetCurrentLeftSWeaponSkill(SWeaponSkill[c]);
	    break;
	  }

      if(!GetCurrentLeftSWeaponSkill())
	{
	  SetCurrentLeftSWeaponSkill(new sweaponskill);
	  GetCurrentLeftSWeaponSkill()->SetID(GetLeftWielded()->GetID());
	  SWeaponSkill.push_back(GetCurrentLeftSWeaponSkill());
	}
    }

  if(!Initializing)
    CalculateBattleInfo();
}

void humanoid::SignalEquipmentRemoval(ushort EquipmentIndex)
{
  character::SignalEquipmentRemoval(EquipmentIndex);

  if(EquipmentIndex == RIGHT_WIELDED_INDEX)
    {
      CheckIfSWeaponSkillRemovalNeeded(GetCurrentRightSWeaponSkill());
      SetCurrentRightSWeaponSkill(0);
    }
  else if(EquipmentIndex == LEFT_WIELDED_INDEX)
    {
      CheckIfSWeaponSkillRemovalNeeded(GetCurrentLeftSWeaponSkill());
      SetCurrentLeftSWeaponSkill(0);
    }

  if(!Initializing)
    CalculateBattleInfo();
}

void humanoid::SWeaponSkillTick()
{
  for(std::vector<sweaponskill*>::iterator i = SWeaponSkill.begin(); i != SWeaponSkill.end();)
    {
      if((*i)->Tick() && IsPlayer())
	{
	  item* Item = SearchForItemWithID((*i)->GetID());

	  if(Item)
	    (*i)->AddLevelDownMessage(Item->GetName(UNARTICLED));
	}

      if(!(*i)->GetHits() && *i != GetCurrentRightSWeaponSkill() && *i != GetCurrentLeftSWeaponSkill())
	{
	  std::vector<sweaponskill*>::iterator Dirt = i++;
	  SWeaponSkill.erase(Dirt);
	  continue;
	}
      else
	++i;
    }
}

bool humanoid::DetachBodyPart()
{
  uchar ToBeDetached;
  switch(game::KeyQuestion("What limb do you wish to detach? (l)eft arm, (r)ight arm, (L)eft leg, (R)ight leg?", KEY_ESC, 4, 'l','r','L','R'))
    {
    case 'l':
      ToBeDetached = LEFT_ARMINDEX;
      break;
    case 'r':
      ToBeDetached = RIGHT_ARMINDEX;
      break;
    case 'L':
      ToBeDetached = LEFT_LEGINDEX;
      break;
    case 'R':
      ToBeDetached = RIGHT_LEGINDEX;
      break;
    default:
      return false;
    }

  bodypart* BodyPart = GetBodyPart(ToBeDetached);

  if(BodyPart)
    {
      item* ToDrop = SevereBodyPart(ToBeDetached);
      GetSquareUnder()->SendNewDrawRequest();
      GetStack()->AddItem(ToDrop);
      ToDrop->DropEquipment();
      ADD_MESSAGE("Bodypart detached!");  
      return true;
    }
  else
    {
      ADD_MESSAGE("That bodypart has already been detached.");
      return false;
    }
}

void angel::GetAICommand()
{
  SetHealTimer(GetHealTimer() + 1);

  if(GetHealTimer() > LENGTH_OF_ANGELS_HEAL_COUNTER_LOOP && AttachBodyPartsOfFriendsNear())
    return;

  humanoid::GetAICommand();
}

/* Returns true if the angel finds somebody near to heal else false */

bool angel::AttachBodyPartsOfFriendsNear()
{
  character* HurtOne = 0;
  bodypart* SeveredOne = 0;

  for(ushort d = 0; d < 8; ++d)
    {
      square* Square = GetNeighbourSquare(d);

      if(Square)
	{
	  character* Char = Square->GetCharacter();

	  if(Char && (!HurtOne || Char->IsPlayer()) && Char->GetTeam()->GetRelation(GetTeam()) == FRIEND && !Char->HasAllBodyParts())
	    {
	      bodypart* BodyPart = Char->FindRandomOwnBodyPart();

	      if(BodyPart)
		{
		  HurtOne = Char;
		  SeveredOne = BodyPart;
		}
	    }
	}
    }

  if(HurtOne)
    {
      if(HurtOne->IsPlayer())
	ADD_MESSAGE("%s puts your %s back to its place.", CHAR_DESCRIPTION(DEFINITE), SeveredOne->GetBodyPartName().c_str());
      else if(CanBeSeenByPlayer())
	ADD_MESSAGE("%s helps %s by putting %s %s in its old place.", CHAR_DESCRIPTION(DEFINITE), HurtOne->CHAR_DESCRIPTION(DEFINITE), HurtOne->GetPossessivePronoun().c_str(), SeveredOne->GetBodyPartName().c_str());
	
      SeveredOne->SetHP(1);
      SeveredOne->RemoveFromSlot();
      HurtOne->AttachBodyPart(SeveredOne);
      SetHealTimer(0);
      DexterityAction(5);
      return true;
    }
  else
    return false;
}

void angel::VirtualConstructor(bool Load)
{
  humanoid::VirtualConstructor(Load);
  SetHealTimer(LENGTH_OF_ANGELS_HEAL_COUNTER_LOOP);
}

void humanoid::DrawBodyParts(bitmap* Bitmap, vector2d Pos, ulong Luminance, bool AllowAlpha, bool AllowAnimate) const
{
  /* Order is important: Don't use a loop. */

  if(GetGroin())
    GetGroin()->Draw(Bitmap, Pos, Luminance, AllowAlpha, AllowAnimate);

  if(GetRightLeg())
    GetRightLeg()->Draw(Bitmap, Pos, Luminance, AllowAlpha, AllowAnimate);

  if(GetLeftLeg())
    GetLeftLeg()->Draw(Bitmap, Pos, Luminance, AllowAlpha, AllowAnimate);

  if(GetTorso())
    GetTorso()->Draw(Bitmap, Pos, Luminance, AllowAlpha, AllowAnimate);
  
  if(GetRightArm())
    GetRightArm()->Draw(Bitmap, Pos, Luminance, AllowAlpha, AllowAnimate);

  if(GetLeftArm())
    GetLeftArm()->Draw(Bitmap, Pos, Luminance, AllowAlpha, AllowAnimate);

  if(GetHead())
    GetHead()->Draw(Bitmap, Pos, Luminance, AllowAlpha, AllowAnimate);
}

void dwarf::DrawBodyParts(bitmap* Bitmap, vector2d Pos, ulong Luminance, bool AllowAlpha, bool AllowAnimate) const
{
  if(GetGroin())
    GetGroin()->Draw(Bitmap, Pos + vector2d(0, -1), Luminance, AllowAlpha, AllowAnimate);

  if(GetRightLeg())
    GetRightLeg()->Draw(Bitmap, Pos + vector2d(0, -1), Luminance, AllowAlpha, AllowAnimate);

  if(GetLeftLeg())
    GetLeftLeg()->Draw(Bitmap, Pos + vector2d(0, -1), Luminance, AllowAlpha, AllowAnimate);

  if(GetTorso())
    GetTorso()->Draw(Bitmap, Pos, Luminance, AllowAlpha, AllowAnimate);
  
  if(GetRightArm())
    GetRightArm()->Draw(Bitmap, Pos, Luminance, AllowAlpha, AllowAnimate);

  if(GetLeftArm())
    GetLeftArm()->Draw(Bitmap, Pos, Luminance, AllowAlpha, AllowAnimate);

  if(GetHead())
    GetHead()->Draw(Bitmap, Pos + vector2d(0, 1), Luminance, AllowAlpha, AllowAnimate);
}

bool snake::SpecialBiteEffect(character* Char, uchar, uchar, bool BlockedByArmour)
{
  if(!BlockedByArmour)
    {
      Char->BeginTemporaryState(POISONED, 400 + RAND() % 200);
      return true;
    }
  else
    return false;
}

ushort angel::GetTorsoMainColor() const
{
  return GetMasterGod()->GetColor();
}

ushort angel::GetArmMainColor() const
{
  return GetMasterGod()->GetColor();
}

ushort kamikazedwarf::GetTorsoMainColor() const
{
  return GetMasterGod()->GetColor();
}

ushort kamikazedwarf::GetArmMainColor() const
{
  return GetMasterGod()->GetColor();
}

ushort kamikazedwarf::GetLegMainColor() const
{
  return GetMasterGod()->GetColor();
}

void angel::CreateBodyParts(ushort SpecialFlags)
{
  for(ushort c = 0; c < GetBodyParts(); ++c) 
    if(c == GROIN_INDEX || c == RIGHT_LEGINDEX || c == LEFT_LEGINDEX)
      SetBodyPart(c, 0);
    else
      CreateBodyPart(c, SpecialFlags);
}

void genie::CreateBodyParts(ushort SpecialFlags)
{
  for(ushort c = 0; c < GetBodyParts(); ++c) 
    if(c == GROIN_INDEX || c == RIGHT_LEGINDEX || c == LEFT_LEGINDEX)
      SetBodyPart(c, 0);
    else
      CreateBodyPart(c, SpecialFlags);
}

ushort housewife::GetHairColor() const
{
  static ushort HouseWifeHairColor[] = { MakeRGB16(48, 40, 8), MakeRGB16(60, 48, 24),  MakeRGB16(200, 0, 0) };
  return HouseWifeHairColor[RAND() % 3];
}

ushort angel::GetAttribute(ushort Identifier) const // temporary until wings are bodyparts
{
  if(Identifier == LEG_STRENGTH)
    return GetDefaultLegStrength();
  else if(Identifier == AGILITY)
    return GetDefaultAgility();
  else
    return humanoid::GetAttribute(Identifier);
}

ushort genie::GetAttribute(ushort Identifier) const // temporary until someone invents a better way of doing this
{
  if(Identifier == LEG_STRENGTH)
    return GetDefaultLegStrength();
  else if(Identifier == AGILITY)
    return 50; /* Air's flexibility */
  else
    return humanoid::GetAttribute(Identifier);
}

void billswill::CalculateUnarmedDamage()
{
  UnarmedDamage = float(GetBaseUnarmedStrength()) * GetCWeaponSkill(UNARMED)->GetEffectBonus() / 1000000;
}

bool spider::SpecialBiteEffect(character* Char, uchar, uchar, bool BlockedByArmour)
{
  if(!BlockedByArmour)
    {
      Char->BeginTemporaryState(POISONED, 80 + RAND() % 40);
      return true;
    }
  else
    return false;
}

bool humanoid::CanUseStethoscope(bool PrintReason) const
{
  if(GetArms() == 0)
    {
      if(PrintReason)
	ADD_MESSAGE("You need an arm to use a stethoscope.");
      return false;
    }

  if(GetHead() == 0)
    {
      if(PrintReason)
	ADD_MESSAGE("You need a head to use stethoscope.");
      return false;
    }
  return true;
}

bool humanoid::IsUsingArms() const
{
  return GetAttackStyle() & USE_ARMS && ((GetRightArm() && GetRightArm()->GetDamage()) || (GetLeftArm() && GetLeftArm()->GetDamage()));
}

bool humanoid::IsUsingLegs() const
{
  return (GetAttackStyle() & USE_LEGS || (GetAttackStyle() & USE_ARMS && (!GetRightArm() || !GetRightArm()->GetDamage()) && (!GetLeftArm() || !GetLeftArm()->GetDamage()))) && GetRightLeg() && GetLeftLeg();
}

bool humanoid::IsUsingHead() const
{
  return (GetAttackStyle() & USE_HEAD || ((GetAttackStyle() & USE_LEGS || (GetAttackStyle() & USE_ARMS && (!GetRightArm() || !GetRightArm()->GetDamage()) && (!GetLeftArm() || !GetLeftArm()->GetDamage()))) && (!GetRightLeg() || !GetLeftLeg()))) && GetHead();
}

void humanoid::AddAttackInfo(felist& List) const
{
  if(GetAttackStyle() & USE_ARMS)
    {
      if(GetRightArm())
	GetRightArm()->AddAttackInfo(List);

      if(GetLeftArm())
	GetLeftArm()->AddAttackInfo(List);
    }

  if(IsUsingLegs())
    {
      leg* KickLeg = GetKickLeg();

      std::string Entry = "   kick attack";
      Entry.resize(50, ' ');
      Entry << KickLeg->GetKickMinDamage() << '-' << KickLeg->GetKickMaxDamage();
      Entry.resize(60, ' ');
      Entry << int(KickLeg->GetKickToHitValue());
      Entry.resize(70, ' ');
      Entry << KickLeg->GetKickAPCost();
      List.AddEntry(Entry, LIGHT_GRAY);
    }

  if(IsUsingHead())
    {
      std::string Entry = "   bite attack";
      Entry.resize(50, ' ');
      Entry << GetHead()->GetBiteMinDamage() << '-' << GetHead()->GetBiteMaxDamage();
      Entry.resize(60, ' ');
      Entry << int(GetHead()->GetBiteToHitValue());
      Entry.resize(70, ' ');
      Entry << GetHead()->GetBiteAPCost();
      List.AddEntry(Entry, LIGHT_GRAY);
    }
}

void nonhumanoid::AddAttackInfo(felist& List) const
{
  if(IsUsingArms())
    {
      std::string Entry = "   unarmed attack";
      Entry.resize(50, ' ');
      Entry << GetUnarmedMinDamage() << '-' << GetUnarmedMaxDamage();
      Entry.resize(60, ' ');
      Entry << int(GetUnarmedToHitValue());
      Entry.resize(70, ' ');
      Entry << GetUnarmedAPCost();
      List.AddEntry(Entry, LIGHT_GRAY);
    }

  if(IsUsingLegs())
    {
      std::string Entry = "   kick attack";
      Entry.resize(50, ' ');
      Entry << GetKickMinDamage() << '-' << GetKickMaxDamage();
      Entry.resize(60, ' ');
      Entry << int(GetKickToHitValue());
      Entry.resize(70, ' ');
      Entry << GetKickAPCost();
      List.AddEntry(Entry, LIGHT_GRAY);
    }

  if(IsUsingHead())
    {
      std::string Entry = "   bite attack";
      Entry.resize(50, ' ');
      Entry << GetBiteMinDamage() << '-' << GetBiteMaxDamage();
      Entry.resize(60, ' ');
      Entry << int(GetBiteToHitValue());
      Entry.resize(70, ' ');
      Entry << GetBiteAPCost();
      List.AddEntry(Entry, LIGHT_GRAY);
    }
}

void humanoid::CalculateBattleInfo()
{
  CalculateDodgeValue();

  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c))
      GetBodyPart(c)->CalculateAttackInfo();
}

void nonhumanoid::CalculateBattleInfo()
{
  CalculateDodgeValue();
  CalculateUnarmedAttackInfo();
  CalculateKickAttackInfo();
  CalculateBiteAttackInfo();
}

void nonhumanoid::CalculateUnarmedAttackInfo()
{
  CalculateUnarmedDamage();
  CalculateUnarmedToHitValue();
  CalculateUnarmedAPCost();
}

void nonhumanoid::CalculateKickAttackInfo()
{
  CalculateKickDamage();
  CalculateKickToHitValue();
  CalculateKickAPCost();
}

void nonhumanoid::CalculateBiteAttackInfo()
{
  CalculateBiteDamage();
  CalculateBiteToHitValue();
  CalculateBiteAPCost();
}

leg* humanoid::GetKickLeg() const
{
  return GetRightLeg()->GetKickDamage() >= GetLeftLeg()->GetKickDamage() ? static_cast<leg*>(GetRightLeg()) : static_cast<leg*>(GetLeftLeg());
}

material* humanoid::CreateBodyPartFlesh(ushort, ulong Volume) const
{
  if(!CreateSolidMaterialConfigurations())
    return MAKE_MATERIAL(HUMAN_FLESH, Volume);
  else
    return MAKE_MATERIAL(Config, Volume);
}

item* skeleton::SevereBodyPart(ushort BodyPartIndex)
{
  item* Bone;
  if(BodyPartIndex == HEAD_INDEX)
    Bone = new skull(0, NO_MATERIALS);
  else
    Bone = new bone(0, NO_MATERIALS);

  item* BodyPart = GetBodyPart(BodyPartIndex);
  Bone->InitMaterials(BodyPart->GetContainedMaterial());
  BodyPart->SetContainedMaterial(0, NO_PIC_UPDATE);
  BodyPart->SendToHell();
  BodyPart->DropEquipment();
  BodyPart->RemoveFromSlot();
  CalculateAttributeBonuses();
  CalculateBattleInfo();

  if(StuckToBodyPart == BodyPartIndex)
    {
      StuckToBodyPart = NONE_INDEX;
      StuckTo = 0;
    }
  return Bone;  
}

void zombie::CreateBodyParts(ushort SpecialFlags)
{
  for(ushort c = 0; c < GetBodyParts(); ++c) 
    if(BodyPartVital(c) || RAND() % 3)
      CreateBodyPart(c, SpecialFlags);
}

void humanoid::AddSpecialEquipmentInfo(std::string& String, ushort Index) const
{
  if((Index == RIGHT_WIELDED_INDEX && GetRightArm()->TwoHandWieldIsActive()) || (Index == LEFT_WIELDED_INDEX && GetLeftArm()->TwoHandWieldIsActive()))
    String << " (in both hands)";
}

/* Yes, this is evil. */

#define INSTANTIATE(name) if((Item = DataBase->name.Instantiate(SpecialFlags))) Set##name(Item);

void humanoid::CreateInitialEquipment(ushort SpecialFlags)
{
  item* Item;

  INSTANTIATE(Helmet);
  INSTANTIATE(Amulet);
  INSTANTIATE(Cloak);
  INSTANTIATE(BodyArmor);
  INSTANTIATE(Belt);
  INSTANTIATE(RightWielded);
  INSTANTIATE(LeftWielded);
  INSTANTIATE(RightRing);
  INSTANTIATE(LeftRing);
  INSTANTIATE(RightGauntlet);
  INSTANTIATE(LeftGauntlet);
  INSTANTIATE(RightBoot);
  INSTANTIATE(LeftBoot);

  if(CurrentRightSWeaponSkill)
    CurrentRightSWeaponSkill->AddHit(GetRightSWeaponSkillHits());

  if(CurrentLeftSWeaponSkill)
    CurrentLeftSWeaponSkill->AddHit(GetLeftSWeaponSkillHits());
}

void orc::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) != HOSTILE)
    ADD_MESSAGE("\"Has you seen any elf or dwarf? Me hungry.\"");
  else
    ADD_MESSAGE("\"Nice scalp! Me want it!\"");
}

void cossack::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("%s shouts wildly: \"For Tataria!\"", CHAR_DESCRIPTION(DEFINITE));
      return;
    }

  static bool Said[4];

  switch(RandomizeReply(4, Said))
    {
    case 0:
      ADD_MESSAGE("\"Graah! Eating raw flesh makes one feel so masculine and powerful! (and sick)\"");
      break;
    case 1:
      ADD_MESSAGE("\"It surely is cold on this island. Remembers me of my six years in Siberia after breaking into the local pub's booze cellar...\"");
      break;
    case 2:
      ADD_MESSAGE("\"What, why have I no horse? Er, I lost it in poker.\"");
      break;
    case 3:
      ADD_MESSAGE("\"Women are odd. No matter how many times I take them to hunt wild beasts of the Steppe or show them my collection of old vodka bottles, none of them still likes me.\"");
      break;
    }
}

std::string humanoid::GetBodyPartName(ushort Index, bool Articled) const
{
  std::string Article = Articled ? "a" : "";

  switch(Index)
    {
    case HEAD_INDEX: return Article + "head";
    case TORSO_INDEX: return Article + "torso";
    case RIGHT_ARMINDEX: return Article + "right arm";
    case LEFT_ARMINDEX: return Article + "left arm";
    case GROIN_INDEX: return Article + "groin";
    case RIGHT_LEGINDEX: return Article + "right leg";
    case LEFT_LEGINDEX: return Article + "left leg";
    default:
      ABORT("Illegal humanoid bodypart name request!");
      return 0;
    }
}

void humanoid::CreateBlockPossibilityVector(blockvector& Vector, float ToHitValue) const
{
  float RightBlockChance = GetRightArm() ? GetRightArm()->GetBlockChance(ToHitValue) : 0;
  float LeftBlockChance = GetLeftArm() ? GetLeftArm()->GetBlockChance(ToHitValue) : 0;
  ushort RightBlockCapability = GetRightArm() ? GetRightArm()->GetBlockCapability() : 0;
  ushort LeftBlockCapability = GetLeftArm() ? GetLeftArm()->GetBlockCapability() : 0;

  /* Double block */

  if(RightBlockCapability + LeftBlockCapability)
    Vector.push_back(std::pair<float, ushort>(RightBlockChance * LeftBlockChance, RightBlockCapability + LeftBlockCapability));

  /* Right block */

  if(RightBlockCapability)
    Vector.push_back(std::pair<float, ushort>(RightBlockChance * (1 - LeftBlockChance), RightBlockCapability));

  /* Left block */

  if(LeftBlockCapability)
    Vector.push_back(std::pair<float, ushort>(LeftBlockChance * (1 - RightBlockChance), LeftBlockCapability));
}

item* humanoid::SevereBodyPart(ushort BodyPartIndex)
{
  if(BodyPartIndex == RIGHT_ARMINDEX && GetCurrentRightSWeaponSkill())
    {
      CheckIfSWeaponSkillRemovalNeeded(GetCurrentRightSWeaponSkill());
      SetCurrentRightSWeaponSkill(0);
    }
  else if(BodyPartIndex == LEFT_ARMINDEX && GetCurrentLeftSWeaponSkill())
    {
      CheckIfSWeaponSkillRemovalNeeded(GetCurrentLeftSWeaponSkill());
      SetCurrentLeftSWeaponSkill(0);
    }

  return character::SevereBodyPart(BodyPartIndex);
}

void humanoid::CheckIfSWeaponSkillRemovalNeeded(sweaponskill* Skill)
{
  if(!Skill->GetHits())
    for(std::vector<sweaponskill*>::iterator i = SWeaponSkill.begin(); i != SWeaponSkill.end(); ++i)
      if(*i == Skill)
	{
	  delete *i;
	  SWeaponSkill.erase(i);
	  break;
	}
}

void humanoid::AddDefenceInfo(felist& List) const
{
  character::AddDefenceInfo(List);

  if(GetRightArm())
    GetRightArm()->AddDefenceInfo(List);

  if(GetLeftArm())
    GetLeftArm()->AddDefenceInfo(List);
}

humanoid::humanoid(const humanoid& Humanoid) : character(Humanoid), CurrentRightSWeaponSkill(0), CurrentLeftSWeaponSkill(0)
{
  SWeaponSkill.resize(Humanoid.SWeaponSkill.size());

  for(ushort c = 0; c < SWeaponSkill.size(); ++c)
    SWeaponSkill[c] = new sweaponskill(*Humanoid.SWeaponSkill[c]);
}

std::string humanoid::GetDeathMessage() const
{
  return GetName(DEFINITE) + (GetHead() ? " dies screaming." : " dies without a sound."); 
}

uchar humanoid::GetSWeaponSkillLevel(const item* Item) const
{
  for(ushort c = 0; c < GetSWeaponSkills(); ++c)
    if(GetSWeaponSkill(c)->GetID() == Item->GetID())
      return GetSWeaponSkill(c)->GetLevel();

  return 0;
}

bool humanoid::IsAlive() const
{
  for(ushort c = 0; c < GetBodyParts(); ++c)
    if(GetBodyPart(c) && GetBodyPart(c)->IsAlive())
      return true;

  return false;
}

/* Not a particulary ingenious algorithm but better than nothing... */

float kamikazedwarf::GetTimeToKill(const character* Enemy, bool UseMaxHP) const
{
  short HP = UseMaxHP ? Enemy->GetMaxHP() : Enemy->GetHP();
  short HPAfterExplosion = HP - (GetStack()->GetTotalExplosivePower() >> 1);

  if(HPAfterExplosion <= 0)
    return 1000;
  else
    return humanoid::GetTimeToKill(Enemy, UseMaxHP) * HPAfterExplosion / HP;
}

void dog::BeTalkedTo(character* Char)
{
  if(RAND() % 50)
    character::BeTalkedTo(Char);
  else
    ADD_MESSAGE("\"Can't you understand I can't speak?\"");
}

ulong angel::GetBaseEmitation() const
{
  switch(GetMasterGod()->BasicAlignment())
    {
    case GOOD: return MakeRGB24(150, 150, 150);
    case NEUTRAL: return MakeRGB24(120, 120, 150);
    default: return MakeRGB24(150, 110, 110);
    }
}

void bananagrower::BeTalkedTo(character* Talker)
{
  std::string ProfStr;
  switch(Profession)
    {
    case 0:
      ProfStr = "the president of Twerajf";
      break;
    case 1:
      ProfStr = "a doctor";
      break;
    case 2:
      ProfStr = "a computer engineer";
      break;
    case 3:
      ProfStr = "a physicist";
      break;
    case 4:
      ProfStr = "a journalist";
      break;
    case 5:
      ProfStr = "a chemist";
      break;
    case 6:
      ProfStr = "a quantum mechanic";
      break;
    case 7:
      ProfStr = "a priest of Silva";
      break;
    }

  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"Banana POWER!\"");
      return;
    }

  static bool Said[5];

  switch(RandomizeReply(5, Said))
    {
    case 0:
      ADD_MESSAGE("\"I was %s before that darn Petrus invaded our peaceful land.\"", ProfStr.c_str());
      break;
    case 1:
      ADD_MESSAGE("\"When this place was still called Twerajf, everybody was happy.\"");
      break;
    case 2:
      ADD_MESSAGE("\"I hate bananas. I wish I still was %s.\"", ProfStr.c_str());
      break;
    case 3:
      ADD_MESSAGE("\"1 + 1 = 3. I still don't belive it.\"");
      break;
    case 4:
      if(Profession == 0)
	ADD_MESSAGE("\"I'm glad that Petrus spared my life even though I was the president.\"");
      else
	ADD_MESSAGE("\"I wish it would rain soon, the bananas are starting to die.\"");
    }
}

void bananagrower::VirtualConstructor(bool Load)
{
  humanoid::VirtualConstructor(Load);
  if(!Load)
    Profession = RAND() % 8;
}

void bananagrower::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> Profession;
}

void bananagrower::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << Profession;
}

void imperialist::CreateInitialEquipment(ushort SpecialFlags)
{
  for(ushort c = 0; c < 5; ++c)
    GetStack()->AddItem(new stone(0, SpecialFlags));
}

void imperialist::BeTalkedTo(character* Talker)
{
  if(GetTeam()->GetRelation(Talker->GetTeam()) == HOSTILE)
    {
      ADD_MESSAGE("\"Die you communist pig!\"");
      return;
    }

  static bool Said[5];

  switch(RandomizeReply(5, Said))
    {
    case 0:
      ADD_MESSAGE("\"They said that levitating ostriches had no future! HAHAHAAHHA!\"");
      break;
    case 1:
      ADD_MESSAGE("\"Ostriches Corp. was a high risk investment, but more than worth it.\""); 
      break;
    case 2:
      ADD_MESSAGE("\"I'm originally from Attnam, but I like the climate here in New Attnam more.\"");
      break;
    case 3:
      ADD_MESSAGE("\"The tax laws of Attnam don't encourage free enterprise.\""); 
      break;
    case 4:
      ADD_MESSAGE("\"Poor people shouldn't complain: after all it's their own fault.\"");
      break;
    }  
}
