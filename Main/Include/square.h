#ifndef __SQUARE_H__
#define __SQUARE_H__

#include <string>

#include "typedef.h"
#include "vector2d.h"

class area;
class material;
class bitmap;
class character;
class groundterrain;
class overterrain;
class outputfile;
class inputfile;
class squarescript;

class square
{
public:
	square(area*, vector2d);
	virtual ~square();
	virtual void Save(outputfile&) const;
	virtual void Load(inputfile&);
	virtual void DrawMemorized() const;
	virtual void UpdateMemorizedAndDraw() = 0;
	virtual bool DrawTerrain() const = 0;
	virtual bool DrawCharacters() const = 0;
	virtual void SetCharacter(character* What ) { Character = What; }
	virtual void AddCharacter(character*);
	virtual void RemoveCharacter();
	virtual character* GetCharacter() const		{ return Character; }
	virtual ulong GetLastSeen() const				{ return LastSeen; }
	virtual vector2d GetPos() const			{ return Pos; }
	virtual void SetLastSeen(ulong);
	virtual ushort GetPopulation() const { if(Character) return 1; else return 0; }
	virtual area* GetAreaUnder() const { return AreaUnder; }
	//virtual void EmptyFlag()			{ Flag = false; }
	//virtual void SetFlag()			{ Flag = true; }
	//virtual bool RetrieveFlag() const		{ return Flag; }
	virtual groundterrain* GetGroundTerrain() const = 0;
	virtual overterrain* GetOverTerrain() const = 0;
	virtual std::string GetMemorizedDescription() { return MemorizedDescription; }
	virtual void SetMemorizedDescription(std::string What) { MemorizedDescription = What; }
	virtual void UpdateMemorizedDescription(bool = false) = 0;
	virtual bool CanBeSeen() const;
	virtual bool CanBeSeenIgnoreDarkness() const { return CanBeSeen(); }
	virtual bool CanBeSeenFrom(vector2d, ulong) const;
	virtual void DrawCheat() = 0;
	virtual void SendNewDrawRequest() { NewDrawRequested = true; }
	virtual bitmap* GetMemorized() const { return Memorized; }
	//virtual void SetDescriptionChanged(bool) {}
	virtual void SetDescriptionChanged(bool What) { DescriptionChanged = What; }
protected:
	std::string MemorizedDescription;
	area* AreaUnder;
	character* Character;
	vector2d Pos;
	bool NewDrawRequested;
	bitmap* Memorized;
	ulong LastSeen;
	bool DescriptionChanged;
};

inline outputfile& operator<<(outputfile& SaveFile, square* Square)
{
	Square->Save(SaveFile);
	return SaveFile;
}

#endif
