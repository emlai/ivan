#include "itemba.h"
#include "stack.h"
#include "strover.h"
#include "felist.h"
#include "lsquare.h"
#include "message.h"
#include "graphics.h"
#include "charba.h"
#include "area.h"
#include "femath.h"
#include "slot.h"

stack::stack(square* SquareUnder, uchar SquarePosition) : SquareUnder(SquareUnder), SquarePosition(SquarePosition)
{
  Item = new stacklist;
}

stack::~stack()
{
  Clean();
  delete Item;
}

bool stack::DrawToTileBuffer() const
{
  if(!GetItems())
    return false;

  for(stackiterator i = Item->begin(); i != Item->end(); ++i)
    (**i)->PositionedDrawToTileBuffer(SquarePosition);

  return true;
}

void stack::AddItem(item* ToBeAdded)
{
  stackslot* StackSlot = new stackslot;

  StackSlot->SetMotherStack(this);
  StackSlot->SetStackIterator(Item->insert(Item->end(), StackSlot));
  ToBeAdded->PlaceToSlot(StackSlot);

  ToBeAdded->SetSquareUnder(GetSquareUnder());

  if(GetSquareUnder())
    {
      GetSquareUnder()->SetDescriptionChanged(true);

      if(GetSquareUnder()->CanBeSeen())
	GetSquareUnder()->UpdateMemorizedDescription();
    }

  if(GetSquareTrulyUnder())
    {
      if(!game::GetInWilderness())
	GetLSquareTrulyUnder()->SignalEmitationIncrease(ToBeAdded->GetEmitation());

      GetSquareTrulyUnder()->SendNewDrawRequest();
      GetSquareTrulyUnder()->SendMemorizedUpdateRequest();

      if(GetSquareTrulyUnder()->CanBeSeen())
	GetSquareTrulyUnder()->UpdateMemorized();
    }
}

void stack::FastAddItem(item* ToBeAdded)
{
  stackslot* StackSlot = new stackslot;
  StackSlot->SetMotherStack(this);
  StackSlot->SetStackIterator(Item->insert(Item->end(), StackSlot));
  ToBeAdded->PlaceToSlot(StackSlot);
  ToBeAdded->SetSquareUnder(GetSquareUnder());
}

void stack::RemoveItem(stackiterator Iterator)
{
  ushort IEmit = GetEmitation();
  delete *Iterator;
  Item->erase(Iterator);

  if(GetSquareUnder())
    {
      GetSquareUnder()->SetDescriptionChanged(true);

      if(GetSquareUnder()->CanBeSeen())
	GetSquareUnder()->UpdateMemorizedDescription();
    }

  if(GetSquareTrulyUnder())
    {
      if(!game::GetInWilderness())
	GetLSquareTrulyUnder()->SignalEmitationDecrease(IEmit);

      GetSquareTrulyUnder()->SendNewDrawRequest();
      GetSquareTrulyUnder()->SendMemorizedUpdateRequest();

      if(GetSquareTrulyUnder()->CanBeSeen())
	GetSquareTrulyUnder()->UpdateMemorized();
    }
}

void stack::FastRemoveItem(stackiterator Iterator)
{
  delete *Iterator;
  Item->erase(Iterator);
}

void stack::Clean()
{
  for(stackiterator i = Item->begin(); i != Item->end(); ++i)
    {
      (**i)->SetExists(false);
      delete *i;
    }

  Item->clear();
}

item* stack::MoveItem(stackiterator Iterator, stack* MoveTo)
{
  if(GetSquareTrulyUnder())
    {
      GetSquareTrulyUnder()->SendNewDrawRequest();
      GetSquareTrulyUnder()->SendMemorizedUpdateRequest();
    }

  if(GetSquareUnder())
    GetSquareUnder()->SetDescriptionChanged(true);

  item* Item = ***Iterator;

  if(MoveTo->GetLSquareTrulyUnder() == GetLSquareTrulyUnder())
    {
      MoveTo->FastAddItem(***Iterator);
      FastRemoveItem(Iterator);

      if(GetSquareUnder() && GetSquareUnder()->CanBeSeen())
	GetSquareUnder()->UpdateMemorizedDescription();

      if(GetSquareTrulyUnder() && GetSquareTrulyUnder()->CanBeSeen())
	GetSquareTrulyUnder()->UpdateMemorized();
    }
  else
    {
      if(MoveTo->GetSquareTrulyUnder())
	{
	  MoveTo->GetSquareTrulyUnder()->SendNewDrawRequest();
	  MoveTo->GetSquareTrulyUnder()->SendMemorizedUpdateRequest();
	}

      if(MoveTo->GetSquareUnder())
	MoveTo->GetSquareUnder()->SetDescriptionChanged(true);

      MoveTo->AddItem(***Iterator);
      RemoveItem(Iterator);

      if(GetSquareUnder() && GetSquareUnder()->CanBeSeen())
	GetSquareUnder()->UpdateMemorizedDescription();

      if(MoveTo->GetSquareUnder() && MoveTo->GetSquareUnder()->CanBeSeen())
	MoveTo->GetSquareUnder()->UpdateMemorizedDescription();

      if(GetSquareTrulyUnder() && GetSquareTrulyUnder()->CanBeSeen())
	GetSquareTrulyUnder()->UpdateMemorized();
    }

  return Item;
}

ushort stack::GetEmitation() const // Calculates the biggest light emmision of the lsquare...
{
  ushort Emitation = 0;

  for(stackiterator i = Item->begin(); i != Item->end(); ++i)
    if((**i)->GetEmitation() > Emitation)
      Emitation = (**i)->GetEmitation();

  return Emitation;
}

ulong stack::SumOfMasses() const
{
  ulong Sum = 0;

  for(stackiterator i = Item->begin(); i != Item->end(); ++i)
    Sum += (**i)->GetWeight();

  return Sum;
}

void stack::Save(outputfile& SaveFile) const
{
  SaveFile << *Item << SquarePosition;
}

void stack::Load(inputfile& SaveFile)
{
  SaveFile >> *Item >> SquarePosition;

  for(stackiterator i = Item->begin(); i != Item->end(); ++i)
    {
      (*i)->SetStackIterator(i);
      (*i)->SetMotherStack(this);
    }

  SquareUnder = game::GetSquareInLoad();
}

vector2d stack::GetPos() const
{
  return GetLSquareUnder()->GetPos();
}

bool stack::SortedItems(character* Viewer, bool (item::*SorterFunction)(character*) const) const
{
  if(SorterFunction == 0)
    return GetItems() ? true : false;

  for(stackiterator i = Item->begin(); i != Item->end(); ++i)
    if(((***i)->*SorterFunction)(Viewer))
      return true;

  return false;
}

void stack::DeletePointers()
{
  while(GetItems())
    FastRemoveItem(GetBottomSlot());
}

void stack::Kick(ushort Strength, bool ShowOnScreen, uchar Direction)
{
  if(Strength > 3)
    {
      ImpactDamage(Strength);

      if(GetItems())
	(***Item->begin())->Fly(Direction, Strength, this, true);

      if(GetItems() && RAND() % 2)
	(***Item->begin())->Fly(Direction, Strength, this, true);
    }
  else
    if(GetItems() && ShowOnScreen)
      ADD_MESSAGE("Your weak kick has no effect.");
}

long stack::Score() const
{
  long Score = 0;

  for(stackiterator i = Item->begin(); i != Item->end(); ++i)
    Score += (**i)->Score();

  return Score;
}

void stack::SetSquareUnder(square* Square)
{
  SquareUnder = Square;
}

void stack::Polymorph()
{
  itemvector ItemVector;

  FillItemVector(ItemVector);

  ushort p = 0;

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->GetExists() && ItemVector[c]->Polymorph(this) && ++p == 5)
      break;
}

void stack::ReceiveSound(float Strength)
{
  itemvector ItemVector;

  FillItemVector(ItemVector);

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->GetExists())
      ItemVector[c]->ReceiveSound(Strength);
}

void stack::StruckByWandOfStriking(character* Zapper, std::string DeathMsg)
{
  itemvector ItemVector;

  FillItemVector(ItemVector);

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->GetExists())
      ItemVector[c]->StruckByWandOfStriking(Zapper, DeathMsg);
}

void stack::CheckForStepOnEffect(character* Stepper)
{
  itemvector ItemVector;

  FillItemVector(ItemVector);

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->GetExists())
      ItemVector[c]->GetStepOnEffect(Stepper);
}

square* stack::GetSquareTrulyUnder() const
{
  switch(SquarePosition)
    {
    case DOWN:
      if(game::IsValidPos(GetSquareUnder()->GetPos() + vector2d(0, -1)))
	return GetSquareUnder()->GetAreaUnder()->GetSquare(GetSquareUnder()->GetPos() + vector2d(0, -1));
      else
	return 0;
    case LEFT:
      if(game::IsValidPos(GetSquareUnder()->GetPos() + vector2d(1, 0)))
	return GetSquareUnder()->GetAreaUnder()->GetSquare(GetSquareUnder()->GetPos() + vector2d(1, 0));
      else
	return 0;
    case UP:
      if(game::IsValidPos(GetSquareUnder()->GetPos() + vector2d(0, 1)))
	return GetSquareUnder()->GetAreaUnder()->GetSquare(GetSquareUnder()->GetPos() + vector2d(0, 1));
      else
	return 0;
    case RIGHT:
      if(game::IsValidPos(GetSquareUnder()->GetPos() + vector2d(-1, 0)))
	return GetSquareUnder()->GetAreaUnder()->GetSquare(GetSquareUnder()->GetPos() + vector2d(-1, 0));
      else
	return 0;
    default:
      return GetSquareUnder();
    }
}

void stack::ImpactDamage(ushort Strength)
{
  itemvector ItemVector;

  FillItemVector(ItemVector);

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->GetExists())
      ItemVector[c]->ImpactDamage(Strength);
}

void stack::ReceiveFireDamage(character* Burner, std::string DeathMsg, long SizeOfEffect)
{
  itemvector ItemVector;

  FillItemVector(ItemVector);

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->GetExists())
      ItemVector[c]->ReceiveFireDamage(Burner, DeathMsg, this, SizeOfEffect);
}

void stack::Teleport()
{
  itemvector ItemVector;

  FillItemVector(ItemVector);

  for(ushort c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->GetExists())
      ItemVector[c]->Teleport();
}

lsquare* stack::GetLSquareTrulyUnder() const
{
  return (lsquare*)GetSquareTrulyUnder();
}

lsquare* stack::GetLSquareUnder() const
{
  return (lsquare*)SquareUnder;
}

void stack::FillItemVector(itemvector& ItemVector) const
{
  for(stackiterator i = Item->begin(); i != Item->end(); ++i)
    ItemVector.push_back(***i);
}

item* stack::GetBottomItem() const
{
  return ***GetBottomSlot();
}

item* stack::GetItem(ushort Index) const
{
  ushort c = 0;

  for(stackiterator i = Item->begin(); i != Item->end(); ++i, ++c)
    if(c == Index)
      return ***i;

  return 0;
}

ushort stack::SearchItem(item* ToBeSearched) const
{
  ushort c = 0;

  for(stackiterator i = Item->begin(); i != Item->end(); ++i, ++c)
    if(***i == ToBeSearched)
      return c;

  return 0xFFFF;
}

stackiterator stack::GetBottomSlot() const
{
  return Item->begin();
}

stackiterator stack::GetSlotAboveTop() const
{
  return Item->end();
}

item* stack::DrawContents(character* Viewer, std::string Topic, bool (item::*SorterFunction)(character*) const) const
{
  return DrawContents(0, Viewer, Topic, "", "", true, SorterFunction);
}

item* stack::DrawContents(stack* MergeStack, character* Viewer, std::string Topic, std::string ThisDesc, std::string ThatDesc, bool (item::*SorterFunction)(character*) const) const
{
  return DrawContents(MergeStack, Viewer, Topic, ThisDesc, ThatDesc, true, SorterFunction);
}

item* stack::DrawContents(character* Viewer, std::string Topic, bool SelectItem, bool (item::*SorterFunction)(character*) const) const
{
  return DrawContents(0, Viewer, Topic, "", "", SelectItem, SorterFunction);
}

item* stack::DrawContents(stack* MergeStack, character* Viewer, std::string Topic, std::string ThisDesc, std::string ThatDesc, bool SelectItem, bool (item::*SorterFunction)(character*) const) const
{
  felist ItemNames(Topic, WHITE, 0);

  ItemNames.AddDescription("");
  ItemNames.AddDescription(std::string("Overall weight: ") + SumOfMasses() + (MergeStack ? MergeStack->SumOfMasses() : 0) + " grams");
  ItemNames.AddDescription("");

  std::string Buffer = "Icon  Name                                             Weight     AV    Str";
  Viewer->AddSpecialItemInfoDescription(Buffer);
  ItemNames.AddDescription(Buffer);

  if(MergeStack)
    MergeStack->AddContentsToList(ItemNames, Viewer, ThatDesc, SelectItem, SorterFunction);

  AddContentsToList(ItemNames, Viewer, ThisDesc, SelectItem, SorterFunction);

  ushort Chosen = ItemNames.Draw(vector2d(10, 42), 780, MergeStack ? 12 : 15, SelectItem, false);

  if(Chosen & 0x8000)
    return 0;

  ushort Pos = 0;

  item* Item;

  if(MergeStack)
    {
      Item = MergeStack->SearchChosen(Pos, Chosen, Viewer, SorterFunction);

      if(Item)
	return Item;
    }

  Item = SearchChosen(Pos, Chosen, Viewer, SorterFunction);
  return Item;
}

void stack::AddContentsToList(felist& ItemNames, character* Viewer, std::string Desc, bool SelectItem, bool (item::*SorterFunction)(character*) const) const
{
  bool UseSorterFunction = SorterFunction ? true : false;
  bool DescDrawn = false;

  for(ushort c = 0; c < item::ItemCategories(); ++c)
    {
      bool CatDescDrawn = false;

      for(stackiterator i = Item->begin(); i != Item->end(); ++i)
	if((**i)->GetCategory() == c && (!UseSorterFunction || ((***i)->*SorterFunction)(Viewer)))
	  {
	    if(!DescDrawn && Desc != "")
	      {
		ItemNames.AddEntry("", LIGHTGRAY, 0, false);
		ItemNames.AddEntry(Desc, LIGHTGRAY, 0, false);
		ItemNames.AddEntry("", LIGHTGRAY, 0, false);
	      }

	    if(!CatDescDrawn)
	      {
		ItemNames.AddEntry(item::ItemCategoryName(c), LIGHTGRAY, 0, false);
		CatDescDrawn = true;
	      }

	    std::string Buffer = (**i)->Name(INDEFINITE);
	    Buffer.resize(49,' ');
	    Buffer += int((**i)->GetWeight());
	    Buffer.resize(60, ' ');
	    Buffer += int((**i)->GetStrengthValue());
	    Buffer.resize(66, ' ');
	    Buffer += int((**i)->GetWeaponStrength() / 100);
	    Viewer->AddSpecialItemInfo(Buffer, ***i);

	    if(!SelectItem)
	      Buffer = std::string("   ") + Buffer;

	    ItemNames.AddEntry(Buffer, LIGHTGRAY, (**i)->GetPicture());
	  }
    }
}

item* stack::SearchChosen(ushort& Pos, ushort Chosen, character* Viewer, bool (item::*SorterFunction)(character*) const) const
{
  bool UseSorterFunction = SorterFunction ? true : false;

  for(ushort c = 0; c < item::ItemCategories(); ++c)
    for(stackiterator i = Item->begin(); i != Item->end(); ++i)
      if((**i)->GetCategory() == c && (!UseSorterFunction || ((***i)->*SorterFunction)(Viewer)) && Pos++ == Chosen)
	return ***i;

  return 0;
}
