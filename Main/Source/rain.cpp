/* Compiled through materset.cpp */

rain::rain(liquid* Liquid, lsquare* LSquareUnder, vector2d Speed, int Team, bool OwnLiquid) : entity(OwnLiquid ? HAS_BE : 0), Next(0), Drop(0), Liquid(Liquid), LSquareUnder(LSquareUnder), Speed(Speed), SpeedAbs(long(sqrt(Speed.GetLengthSquare()))), Drops(0), OwnLiquid(OwnLiquid), Team(Team)
{
  Emitation = Liquid->GetEmitation();
}

rain::~rain()
{
  delete [] Drop;

  if(OwnLiquid)
    delete Liquid;
}

void rain::Draw(bitmap* Bitmap, vector2d Pos, color24 Luminance) const
{
  long Volume = Liquid->GetVolume();
  int Drops = this->Drops;

  if(!Volume && !Drops)
    return;

  int c, DropMax = Volume ? Limit<int>(Volume / 50, 1, MAX_RAIN_DROPS) : 0;

  if(Drops < DropMax)
    {
      drop* OldDrop = Drop;
      Drop = new drop[DropMax];

      for(c = 0; c < Drops; ++c)
	if(OldDrop[c].MaxAge)
	  Drop[c] = OldDrop[c];
	else
	  RandomizeDropPos(c);

      delete [] OldDrop;

      for(; Drops < DropMax; ++Drops)
	RandomizeDropPos(Drops);
    }
  else
    {
      for(c = 0; c < DropMax; ++c)
	if(!Drop[c].MaxAge)
	  RandomizeDropPos(c);

      for(; Drops > DropMax && !Drop[Drops - 1].MaxAge; --Drops);

      if(!Drops)
	{
	  this->Drops = 0;
	  delete [] Drop;
	  Drop = 0;
	  return;
	}
    }

  color16 Color = Liquid->GetRainColor();

  for(c = 0; c < Drops; ++c)
    if(Drop[c].MaxAge)
      {
	ulong Age = ushort(GET_TICK()) - Drop[c].StartTick;

	if(Age > Drop[c].MaxAge)
	  {
	    Drop[c].MaxAge = 0;
	    continue;
	  }

	vector2d DropPos = vector2d(Drop[c].StartPos) + (Speed * int(Age) >> 8);

	if(DropPos.X < 0 || DropPos.Y < 0 || DropPos.X >= 16 || DropPos.Y >= 16)
	  {
	    Drop[c].MaxAge = 0;
	    continue;
	  }

	Bitmap->AlphaPutPixel(Pos + DropPos, Color, Luminance, 255);
      }

  this->Drops = Drops;
}

void rain::RandomizeDropPos(int I) const
{
  Drop[I].StartTick = GET_TICK() - (RAND() & 3);
  vector2d Pos;

  if(Speed.X && (!Speed.Y || RAND() & 1))
    {
      Pos.X = Speed.X > 0 ? 0 : 15;
      Pos.Y = RAND() & 15;
    }
  else
    {
      Pos.X = RAND() & 15;
      Pos.Y = Speed.Y > 0 ? 0 : 15;
    }

  Drop[I].StartPos = Pos;
  int AgeModifier = 5000 / SpeedAbs;
  Drop[I].MaxAge = AgeModifier > 1 ? 1 + RAND() % AgeModifier : 1;
}

void rain::Be()
{
  long Volume = Liquid->GetVolume();

  if(Volume && !Liquid->IsPowder()) // gum
    {
      long Rand = 250000000 / (Volume * SpeedAbs);

      if(OwnLiquid)
	Rand >>= 3;

      if(Rand < 1 || !(RAND() % Rand))
	{
	  long DropVolume = Min(Volume, 50L);
	  /* Gum */
	  LSquareUnder->SpillFluid(Team == PLAYER_TEAM ? PLAYER : 0, Liquid->CloneLiquid(DropVolume), true, OwnLiquid);

	  if(OwnLiquid)
	    if(Volume == DropVolume)
	      {
		LSquareUnder->RemoveRain(this);
		SendToHell();
	      }
	    else
	      Liquid->EditVolume(-DropVolume);
	}
    }
}

void rain::Save(outputfile& SaveFile) const
{
  SaveFile << Liquid << Speed << (uchar)Team;
}

void rain::Load(inputfile& SaveFile)
{
  LSquareUnder = static_cast<lsquare*>(game::GetSquareInLoad());
  Liquid = static_cast<liquid*>(ReadType<material*>(SaveFile));
  Liquid->SetMotherEntity(this);
  Emitation = Liquid->GetEmitation();
  SaveFile >> Speed;
  Team = ReadType<uchar>(SaveFile);
  SpeedAbs = long(sqrt(Speed.GetLengthSquare()));
}

outputfile& operator<<(outputfile& SaveFile, const rain* Rain)
{
  if(Rain->HasOwnLiquid())
    {
      SaveFile << bool(true);
      Rain->Save(SaveFile);
    }
  else
    SaveFile << bool(false);

  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, rain*& Rain)
{
  if(ReadType<bool>(SaveFile))
    {
      Rain = new rain;
      Rain->Load(SaveFile);
    }
  else
    Rain = game::ConstructGlobalRain();

  return SaveFile;
}