#include <fstream>

#include "config.h"
#include "felist.h"
#include "graphics.h"
#include "stdover.h"
#include "feio.h"
#include "game.h"
#include "area.h"
#include "save.h"
#include "bitmap.h"

#ifdef WIN32
#define CONFIG_FILENAME "ivan.cfg"
#endif

#ifdef __DJGPP__
#define CONFIG_FILENAME "ivan.cfg"
#endif

#ifdef USE_SDL
#define CONFIG_FILENAME (std::string(getenv("HOME")) + "/.ivan.conf").c_str()
#endif

std::string configuration::DefaultName;
ushort configuration::AutoSaveInterval = 500;
ushort configuration::Contrast = 100;
bool configuration::AutoDropLeftOvers = true;
bool configuration::OutlineCharacters = false;
bool configuration::OutlineItems = false;
ushort configuration::CharacterOutlineColor = BLUE;
ushort configuration::ItemOutlineColor = RED;
bool configuration::BeepOnCritical = false;
bool configuration::FullScreenMode = false;
ushort configuration::ContrastLuminance = 256;

void configuration::Save()
{
  std::ofstream SaveFile(CONFIG_FILENAME, std::ios::out);

  if(!SaveFile.is_open())
    return;

  SaveFile << "DefaultName = \"" << DefaultName << "\";\n";
  SaveFile << "AutoSaveInterval = " << AutoSaveInterval << ";\n";
  SaveFile << "Contrast = " << Contrast << ";\n";
  SaveFile << "AutoDropLeftOvers = " << AutoDropLeftOvers << ";\n";
  SaveFile << "OutlineCharacters = " << OutlineCharacters << ";\n";
  SaveFile << "OutlineItems = " << OutlineItems << ";\n";
  SaveFile << "CharacterOutlineColor = " << GET_RED(CharacterOutlineColor) << ", " << GET_GREEN(CharacterOutlineColor) << ", " << GET_BLUE(CharacterOutlineColor) << ";\n";
  SaveFile << "ItemOutlineColor = " << GET_RED(ItemOutlineColor) << ", " << GET_GREEN(ItemOutlineColor) << ", " << GET_BLUE(ItemOutlineColor) << ";\n";
  SaveFile << "BeepOnCritical = " << BeepOnCritical << ";\n";
  SaveFile << "FullScreenMode = " << FullScreenMode << ";\n";
}

void configuration::Load()
{
  inputfile SaveFile(CONFIG_FILENAME, false);

  if(!SaveFile.IsOpen())
    return;

  std::string Word;

  for(SaveFile.ReadWord(Word, false); !SaveFile.Eof(); SaveFile.ReadWord(Word, false))
    {
      if(Word == "DefaultName")
	{
	  SaveFile.ReadWord();
	  SetDefaultName(SaveFile.ReadWord());
	}

      if(Word == "AutoSaveInterval")
	SetAutoSaveInterval(SaveFile.ReadNumber());

      if(Word == "Contrast")
	SetContrast(SaveFile.ReadNumber());

      if(Word == "AutoDropLeftOvers")
	SetAutoDropLeftOvers(SaveFile.ReadBool());

      if(Word == "OutlineCharacters")
	SetOutlineCharacters(SaveFile.ReadBool());

      if(Word == "OutlineItems")
	SetOutlineItems(SaveFile.ReadBool());

      if(Word == "CharacterOutlineColor")
	{
	  uchar Red = SaveFile.ReadNumber();
	  uchar Green = SaveFile.ReadNumber();
	  uchar Blue = SaveFile.ReadNumber();

	  SetCharacterOutlineColor(MAKE_RGB(Red, Green, Blue));
	}

      if(Word == "ItemOutlineColor")
	{
	  uchar Red = SaveFile.ReadNumber();
	  uchar Green = SaveFile.ReadNumber();
	  uchar Blue = SaveFile.ReadNumber();

	  SetItemOutlineColor(MAKE_RGB(Red, Green, Blue));
	}

      if(Word == "BeepOnCritical")
	SetBeepOnCritical(SaveFile.ReadBool());

      if(Word == "FullScreenMode")
	SetFullScreenMode(SaveFile.ReadBool());
    }
}

void configuration::EditContrast(short Change)
{
  Contrast += Change;

  if(short(Contrast) < 0) Contrast = 0;
  if(short(Contrast) > 200) Contrast = 200;

  ContrastLuminance = (Contrast << 8) / 100;
}

void configuration::ShowConfigScreen()
{
  vector2d QuestionPos = game::IsRunning() ? vector2d(16, 6) : vector2d(30, 46);
  ushort Chosen;
  bool BoolChange = false;

  felist List("Which setting do you wish to configure?", WHITE);
  List.AddDescription("");
  List.AddDescription("Setting                                    Value");

  while(true)
    {
      if(game::IsRunning())
	{
	  game::DrawEverythingNoBlit();
	  DOUBLEBUFFER->Fill(16, 6, game::GetScreenSize().X << 4, 23, 0);
	}

      List.Empty();
      List.AddEntry(std::string("Player's default name:                  ") + (DefaultName.length() ? DefaultName : "-"), LIGHTGRAY);
      List.AddEntry(std::string("Autosave interval:                      ") + AutoSaveInterval + " turns", LIGHTGRAY);
      List.AddEntry(std::string("Contrast:                               ") + Contrast + "/100", LIGHTGRAY);
      List.AddEntry(std::string("Drop food leftovers automatically:      ") + (AutoDropLeftOvers ? "yes" : "no"), LIGHTGRAY);
      List.AddEntry(std::string("Outline all characters:                 ") + (OutlineCharacters ? "yes" : "no"), LIGHTGRAY);
      List.AddEntry(std::string("Outline all items:                      ") + (OutlineItems ? "yes" : "no"), LIGHTGRAY);

#ifdef WIN32
      List.AddEntry(std::string("Beep on critical messages:              ") + (BeepOnCritical ? "yes" : "no"), LIGHTGRAY);
      List.AddEntry(std::string("Run the game in full screen mode:       ") + (FullScreenMode ? "yes" : "no"), LIGHTGRAY);
#endif

      Chosen = List.Draw(vector2d(game::IsRunning() ? 26 : 10, game::IsRunning() ? 42 : 10), game::IsRunning() ? 652 : 780, 20, game::IsRunning() ? MAKE_RGB(0, 0, 16) : BLACK, true, false, game::IsRunning(), !game::IsRunning() && !BoolChange);

      switch(Chosen)
	{
	case 0:
	  SetDefaultName(iosystem::StringQuestion("Set new default name (3-20 letters):", QuestionPos, WHITE, 0, 20, !game::IsRunning(), true));
	  BoolChange = false;
	  continue;
	case 1:
	  SetAutoSaveInterval(iosystem::NumberQuestion("Set new autosave interval (1-50000 turns, 0 for never):", QuestionPos, WHITE, !game::IsRunning()));
	  BoolChange = false;
	  continue;
	case 2:
	  iosystem::ScrollBarQuestion("Set new contrast value (0-200, '<' and '>' move the slider):", QuestionPos, Contrast, 5, 0, 200, WHITE, LIGHTGRAY, DARKGRAY, !game::IsRunning(), &ContrastHandler);
	  BoolChange = false;
	  continue;
	case 3:
	  SetAutoDropLeftOvers(!GetAutoDropLeftOvers());
	  BoolChange = true;
	  continue;
	case 4:
	  SetOutlineCharacters(!GetOutlineCharacters());
	  if(game::IsRunning()) game::GetCurrentArea()->SendNewDrawRequest();
	  BoolChange = true;
	  continue;
	case 5:
	  SetOutlineItems(!GetOutlineItems());
	  if(game::IsRunning()) game::GetCurrentArea()->SendNewDrawRequest();
	  BoolChange = true;
	  continue;
#ifdef WIN32
	case 6:
	  SetBeepOnCritical(!GetBeepOnCritical());
	  BoolChange = true;
	  continue;
	case 7:
	  graphics::SwitchMode();
	  BoolChange = true;
	  continue;
#endif
	}

      break;
    }

  Save();
}

void configuration::SetDefaultName(const std::string& What)
{
  if(What.length() < 3)
    DefaultName.resize(0);
  else
    DefaultName = What;
}

void configuration::SetAutoSaveInterval(long What)
{
  if(What < 0) What = 0;
  if(What > 50000) What = 50000;

  AutoSaveInterval = What;
}

void configuration::SetContrast(short What)
{
  if(What < 0) What = 0;
  if(What > 200) What = 200;

  Contrast = What;
  ContrastLuminance = (Contrast << 8) / 100;
}

void configuration::SwitchModeHandler()
{
  SetFullScreenMode(!GetFullScreenMode());
  Save();
}

void configuration::ContrastHandler(long Value)
{
  SetContrast(Value);

  if(game::IsRunning())
    {
      game::GetCurrentArea()->SendNewDrawRequest();
      game::DrawEverythingNoBlit();
    }
}
