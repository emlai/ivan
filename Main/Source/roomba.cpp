#include "roomba.h"
#include "save.h"
#include "lterraba.h"
#include "error.h"

room::room(bool SetStats) : Master(0)
{
	if(SetStats)
		ABORT("Baa!");
}

void room::Save(outputfile& SaveFile) const
{
	typeable::Save(SaveFile);

	SaveFile << Pos << Size << Door << Index << DivineOwner;
}

void room::Load(inputfile& SaveFile)
{
	typeable::Load(SaveFile);

	SaveFile >> Pos >> Size >> Door >> Index >> DivineOwner;
}

void room::HandleInstantiatedOverLevelTerrain(overlevelterrain* Terrain)
{
	if(Terrain->IsDoor())
		Door.push_back(Terrain->GetPos());
}
