#include <fstream>

#include "felist.h"
#include "graphics.h"
#include "bitmap.h"
#include "whandler.h"
#include "colorbit.h"
#include "save.h"
#include "festring.h"
#include "error.h"

const felist* FelistCurrentlyDrawn = 0;

bool FelistDrawController()
{
  FelistCurrentlyDrawn->DrawPage(DOUBLEBUFFER);
  return true;
}

felistentry::felistentry(const std::vector<bitmap*>& BitmapVector, const std::string& String, ushort Color, ushort Marginal, bool Selectable) : String(String), Color(Color), Marginal(Marginal), Selectable(Selectable)
{
  for(ushort c = 0; c < BitmapVector.size(); ++c)
    Bitmap.push_back(new bitmap(BitmapVector[c]));
}

felist::~felist()
{
  Empty();
}

ushort felist::Draw(vector2d DrawPos, ushort DrawWidth, ushort DrawPageLength, ushort DrawBackColor, bool DrawSelectable, bool BlitAfterwards, bool DrawBackroundAfterwards, bool Fade, bool DrawInverseMode)
{
  if(!Entry.size())
    return 0xFFFF;

  Pos = DrawPos;
  Width = DrawWidth;
  PageLength = DrawPageLength;
  Selectable = DrawSelectable;
  BackColor = DrawBackColor;
  InverseMode = DrawInverseMode;

  FelistCurrentlyDrawn = this;

  if(globalwindowhandler::ControlLoopsInstalled())
    globalwindowhandler::InstallControlLoop(FelistDrawController);

  bitmap BackGround(RES);
  bitmap* Buffer;

  if(Fade)
    {
      Buffer = new bitmap(RES, 0);
      BackGround.Fill(0);
    }
  else
    {
      Buffer = DOUBLEBUFFER;
      DOUBLEBUFFER->Blit(&BackGround);
    }

  if(Selectable)
    PageBegin = Selected - Selected % PageLength;
  else if(InverseMode)
    PageBegin = LastEntryIndex() - LastEntryIndex() % PageLength;
  else
    PageBegin = 0;

  ushort Return, c, Selectables = 0;

  bool JustSelectMove = false;

  for(c = 0; c < Entry.size(); ++c)
    if(Entry[c].Selectable)
      ++Selectables;

  for(;;)
    {
      bool AtTheEnd = DrawPage(Buffer);

      if(Fade)
	{
	  if(JustSelectMove)
	    {
	      Buffer->Blit(DOUBLEBUFFER);
	      graphics::BlitDBToScreen();
	    }
	  else
	    Buffer->FadeToScreen();

	  JustSelectMove = false;
	}
      else
	graphics::BlitDBToScreen();

      int Pressed = GETKEY(false);

      if(Selectable && Pressed > 64 && Pressed < 91)
	{
	  Return = Pressed - 65 + PageBegin < long(Entry.size()) ? Pressed - 65 + PageBegin : 0xFFFF;
	  break;
	}

      if(Selectable && Pressed > 96 && Pressed < 123)
	{
	  Return = Pressed - 97 + PageBegin < long(Entry.size()) ? Pressed - 97 + PageBegin : 0xFFFF;
	  break;
	}

      if(Selectable && Pressed == KEYUP)
	{
	  if(Selected)
	    {
	      --Selected;

	      if(Selected < PageBegin)
		{
		  BackGround.Blit(Buffer);
		  PageBegin -= PageLength;
		}
	      else
		JustSelectMove = true;
	    }
	  else
	    {
	      for(c = 0, Selected = 0xFFFF; c < Entry.size(); ++c)
		if(Entry[c].Selectable)
		  ++Selected;

	      if(PageBegin == Selected - Selected % PageLength)
		JustSelectMove = true;
	      else
		{
		  BackGround.Blit(Buffer);
		  PageBegin = Selected - Selected % PageLength;
		}
	    }

	  continue;
	}

      if(Selectable && Pressed == KEYDOWN)
	{
	  if(!AtTheEnd || Selected != Selectables - 1)
	    {
	      ++Selected;

	      if(Selected > PageBegin + PageLength - 1)
		{
		  BackGround.Blit(Buffer);
		  PageBegin += PageLength;
		}
	      else
		JustSelectMove = true;
	    }
	  else
	    {
	      if(!PageBegin)
		JustSelectMove = true;
	      else
		BackGround.Blit(Buffer);

	      Selected = PageBegin = 0;
	    }

	  continue;
	}

      if(Selectable && Pressed == KEYENTER)
	{
	  Return = Selected;
	  break;
	}

      if(Pressed == KEYESC || (AtTheEnd && !InverseMode) || (!PageBegin && InverseMode))
	{
	  Return = 0xFFFF;
	  break;
	}
      else
	{
	  BackGround.Blit(Buffer);

	  if(InverseMode)
	    PageBegin -= PageLength;
	  else
	    PageBegin += PageLength;

	  if(Selectable)
	    Selected = PageBegin;
	}
    }

  if(!Fade)
    {
      if(DrawBackroundAfterwards)
	BackGround.Blit(DOUBLEBUFFER);

      if(BlitAfterwards)
	graphics::BlitDBToScreen();
    }
  else
    delete Buffer;

  globalwindowhandler::DeInstallControlLoop(FelistDrawController);
  return Return;
}

bool felist::DrawPage(bitmap* Buffer) const
{
  ushort LastFillBottom = Pos.Y + 23 + Description.size() * 10;
  DrawDescription(Buffer, Pos, Width, BackColor);

  ushort c, i; // c == entry index, i == selectable index

  for(c = 0, i = 0; i != PageBegin; ++c)
    if(Entry[c].Selectable)
      ++i;

  for(;;)
    {
      std::string Str;
      ushort Marginal = Entry[c].Marginal;

      if(Selectable && Entry[c].Selectable)
	{
	  Str += char('A' + (i - PageBegin));
	  Str += ": ";
	  Marginal += 3;
	}

      Str += Entry[c].String;

      if(Entry[c].Bitmap.size())
	{
	  if(ushort(Str.length()) <= (Width - 50) / 8)
	    {
	      Buffer->Fill(Pos.X + 3, LastFillBottom, Width - 6, 20, BackColor);
	      Entry[c].Bitmap[globalwindowhandler::GetTick() % Entry[c].Bitmap.size()]->MaskedBlit(Buffer, 0, 0, Pos.X + 13, LastFillBottom, 16, 16);

	      if(Selectable && Entry[c].Selectable && Selected == i)
		  FONT->PrintfShade(Buffer, Pos.X + 37, LastFillBottom + 4, Entry[c].Color, "%s", Str.c_str());
	      else
		  FONT->Printf(Buffer, Pos.X + 37, LastFillBottom + 4, Entry[c].Color, "%s", Str.c_str());

	      LastFillBottom += 20;
	    }
	  else
	    {
	      std::vector<std::string> Chapter;
	      festring::SplitString(Str, Chapter, (Width - 50) / 8, Marginal);
	      ushort PictureTop = LastFillBottom + Chapter.size() * 5 - 9;

	      for(ushort l = 0; l < Chapter.size(); ++l)
		{
		  Buffer->Fill(Pos.X + 3, LastFillBottom, Width - 6, 10, BackColor);

		  if(Selectable && Entry[c].Selectable && Selected == i)
		      FONT->PrintfShade(Buffer, Pos.X + 37, LastFillBottom, Entry[c].Color, "%s", Chapter[l].c_str());
		  else
		      FONT->Printf(Buffer, Pos.X + 37, LastFillBottom, Entry[c].Color, "%s", Chapter[l].c_str());

		  LastFillBottom += 10;
		}

	      Entry[c].Bitmap[globalwindowhandler::GetTick() % Entry[c].Bitmap.size()]->MaskedBlit(Buffer, 0, 0, Pos.X + 13, PictureTop, 16, 16);
	    }
	}
      else
	{
	  std::vector<std::string> Chapter;
	  festring::SplitString(Str, Chapter, (Width - 26) / 8, Marginal);

	  for(ushort l = 0; l < Chapter.size(); ++l)
	    {
	      Buffer->Fill(Pos.X + 3, LastFillBottom, Width - 6, 10, BackColor);

	      if(Selectable && Entry[c].Selectable && Selected == i)
		  FONT->PrintfShade(Buffer, Pos.X + 13, LastFillBottom, Entry[c].Color, "%s", Chapter[l].c_str());
	      else
		  FONT->Printf(Buffer, Pos.X + 13, LastFillBottom, Entry[c].Color, "%s", Chapter[l].c_str());

	      LastFillBottom += 10;
	    }
	}

      if((i - PageBegin == PageLength - 1 && Entry[c].Selectable) || c == Entry.size() - 1)
	{
	  if((!InverseMode && c != Entry.size() - 1) || (InverseMode && PageBegin))
	    {
	      Buffer->Fill(Pos.X + 3, LastFillBottom, Width - 6, 30, BackColor);
	      FONT->Printf(Buffer, Pos.X + 13, LastFillBottom + 10, WHITE, "- Press SPACE to continue, ESC to exit -");
	      LastFillBottom += 30;
	    }
	  else
	    {
	      Buffer->Fill(Pos.X + 3, LastFillBottom, Width - 6, 10, BackColor);
	      LastFillBottom += 10;
	    }

	  Buffer->DrawRectangle(Pos.X + 1, Pos.Y + 1, Pos.X + Width - 2, LastFillBottom + 1, DARKGRAY, true);
	  return c == Entry.size() - 1;
	}

      if(Entry[c++].Selectable)
	++i;
    }
}

void felist::DrawDescription(bitmap* Buffer, vector2d Pos, ushort Width, ushort BackColor) const
{
  Buffer->Fill(Pos.X + 3, Pos.Y + 3, Width - 6, 20, BackColor);

  for(ushort c = 0; c < Description.size(); ++c)
    {
      Buffer->Fill(Pos.X + 3, Pos.Y + 13 + c * 10, Width - 6, 10, BackColor);
      FONT->Printf(Buffer, Pos.X + 13, Pos.Y + 13 + c * 10, Description[c].Color, Description[c].String.c_str());
    }

  Buffer->Fill(Pos.X + 3, Pos.Y + 13 + Description.size() * 10, Width - 6, 10, BackColor);
}

/* We suppose InverseMode == true here */

void felist::QuickDraw(vector2d Pos, ushort Width, ushort PageLength) const
{
  DOUBLEBUFFER->Fill(Pos.X + 3, Pos.Y + 3, Width - 6, 20 + PageLength * 10, 0);
  DOUBLEBUFFER->DrawRectangle(Pos.X + 1, Pos.Y + 1, Pos.X + Width - 2, Pos.Y + 24 + PageLength * 10, DARKGRAY, true);

  if(!PageLength)
    return;

  ushort Index = 0;
  ushort Bottom = Pos.Y + PageLength * 10 + 3;

  for(ushort c1 = 0; c1 <= Selected; ++c1)
    {
      const felistentry& CurrentEntry = Entry[Selected - c1];
      std::vector<std::string> Chapter;
      festring::SplitString(CurrentEntry.String, Chapter, (Width - 26) / 8, CurrentEntry.Marginal);

      for(ushort c2 = 0; c2 < Chapter.size(); ++c2)
	{
	  ushort Color = CurrentEntry.Color;
	  Color = MakeRGB16(GetRed16(Color) - ((GetRed16(Color) * 3 * Index / PageLength) >> 2), GetGreen16(Color) - ((GetGreen16(Color) * 3 * Index / PageLength) >> 2), GetBlue16(Color) - ((GetBlue16(Color) * 3 * Index / PageLength) >> 2));
	  FONT->Printf(DOUBLEBUFFER, Pos.X + 13, Bottom, Color, "%s", Chapter[Chapter.size() - c2 - 1].c_str());
	  Bottom -= 10;

	  if(++Index == PageLength)
	    return;
	}
    }
}

void felist::CreateQuickDrawFontCaches(colorizablebitmap* Font, ushort Color, ushort PageLength)
{
  if(PageLength < 2)
    return;

  for(ushort c = 0; c < PageLength; ++c)
    Font->CreateFontCache(MakeRGB16(GetRed16(Color) - ((GetRed16(Color) * 3 * c / PageLength) >> 2), GetGreen16(Color) - ((GetGreen16(Color) * 3 * c / PageLength) >> 2), GetBlue16(Color) - ((GetBlue16(Color) * 3 * c / PageLength) >> 2)));
}

void felist::Empty()
{
  for(ushort c = 0; c < Entry.size(); ++c)
    for(ushort i = 0; i < Entry[c].Bitmap.size(); ++i)
      delete Entry[c].Bitmap[i];

  Entry.clear();
}

void felist::AddEntry(const std::string& Str, ushort Color, ushort Marginal, bitmap* Bitmap, bool Selectable)
{
  if(!Bitmap)
    {
      Entry.push_back(felistentry(Str, Color, Marginal, Selectable));

      if(Maximum && Entry.size() > Maximum)
	Entry.erase(Entry.begin());
    }
  else
    AddEntry(Str, Color, Marginal, std::vector<bitmap*>(1, Bitmap), Selectable);
}

void felist::AddEntry(const std::string& Str, ushort Color, ushort Marginal, const std::vector<bitmap*>& Bitmap, bool Selectable)
{
  Entry.push_back(felistentry(Bitmap, Str, Color, Marginal, Selectable));

  if(Maximum && Entry.size() > Maximum)
    Entry.erase(Entry.begin());
}

void felist::Save(outputfile& SaveFile) const
{
  SaveFile << Entry << Description << Maximum << Selected;
}

void felist::Load(inputfile& SaveFile) 
{
  SaveFile >> Entry >> Description >> Maximum >> Selected;
}

void felist::AddDescription(const std::string& Str, ushort Color)
{
  Description.push_back(felistdescription(Str, Color)); 
}

outputfile& operator<<(outputfile& SaveFile, felistentry Entry)
{
  SaveFile << Entry.Bitmap << Entry.String << Entry.Color << Entry.Marginal << Entry.Selectable;
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, felistentry& Entry)
{
  SaveFile >> Entry.Bitmap >> Entry.String >> Entry.Color >> Entry.Marginal >> Entry.Selectable;
  return SaveFile;
}

outputfile& operator<<(outputfile& SaveFile, felistdescription Desc)
{
  SaveFile << Desc.String << Desc.Color;
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, felistdescription& Desc)
{
  SaveFile >> Desc.String >> Desc.Color;
  return SaveFile;
}

void felist::PrintToFile(const std::string& FileName)
{
  std::ofstream SaveFile(FileName.c_str(), std::ios::out);

  if(!SaveFile.is_open())
    return;

  ushort c;

  for(c = 0; c < Description.size(); ++c)
    SaveFile << Description[c].String << std::endl;

  SaveFile << std::endl;

  for(c = 0; c < Entry.size(); ++c)
    {
      if(Entry[c].Bitmap.size())
	SaveFile << "   ";

      SaveFile << Entry[c].String << std::endl;
    }
}

