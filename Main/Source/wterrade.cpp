#include "wterrade.h"
#include "game.h"
#include "message.h"
#include "dungeon.h"
#include "level.h"
#include "worldmap.h"
#include "charba.h"
#include "feio.h"
#include "igraph.h"

bool attnam::GoDown(character* Who) const
{
	iosystem::TextScreen(FONTW, "Entering dungeon...\n\nThis may take some time, please wait.", false);
	game::GetDungeon(1)->PrepareLevel(0);
	//ABORT("Entering dungeons temporarily disabled!");
	game::GetWorldMap()->RemoveCharacter(Who->GetPos());
	game::SetInWilderness(false);
	game::SetCurrentDungeon(1);
	game::SetCurrent(0);
	delete game::GetWorldMap();
	game::SetWorldMap(0);
	//game::GetCurrentDungeon()->LoadLevel();
	//game::GetCurrentLevel()->PutPlayerAround(game::GetCurrentLevel()->GetUpStairs());
	game::GetCurrentLevel()->PutPlayer(true);
	game::GetCurrentLevel()->Luxify();
	game::GetCurrentLevel()->UpdateLOS();
	game::UpDateCameraX();
	game::UpDateCameraY();
	return true;
}

bool elpuricave::GoDown(character* Who) const
{
	iosystem::TextScreen(FONTW, "Entering dungeon...\n\nThis may take some time, please wait.", false);
	game::GetDungeon(0)->PrepareLevel(0);
	//ABORT("Entering dungeons temporarily disabled!");
	game::GetWorldMap()->RemoveCharacter(Who->GetPos());
	game::SetInWilderness(false);
	game::SetCurrentDungeon(0);
	game::SetCurrent(0);
	delete game::GetWorldMap();
	game::SetWorldMap(0);
	//game::GetCurrentDungeon()->LoadLevel();
	//game::GetCurrentLevel()->PutPlayerAround(game::GetCurrentLevel()->GetUpStairs());
	game::GetCurrentLevel()->PutPlayer(true);
	game::GetCurrentLevel()->Luxify();
	game::GetCurrentLevel()->UpdateLOS();
	game::UpDateCameraX();
	game::UpDateCameraY();
	return true;
}
