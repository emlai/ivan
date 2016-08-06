/*
    audio.h : MIDI Audio Implementation for IVAN
    Copyright (c) 2004-2016 Adrian M. Gin

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License v2 as published by
    the Free Software Foundation;

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

*/
#ifndef __AUDIO_H__
#define __AUDIO_H__


#ifdef USE_SDL
#include "SDL.h"
#endif

#include "error.h"
#include <vector>
#include "RtMidi.h"

class musicfile
{
public:
   musicfile(char* filename, int LowThreshold, int HighThreshold);
   ~musicfile();

   inline bool IsPlaying(void) { return isPlaying; }
   inline void SetPlayState(bool state) { isPlaying = state;}
   inline char* GetFilename(void) { return Filename; }

private:
   char* Filename;
   int LowThreshold;
   int HighThreshold;

   bool isPlaying;

};



class audio
{
public:
   static void Init();
   static void DeInit(void);

   static int Loop(void *ptr);

   static int PlayMIDIFile(char* filename, int32_t loops);

   static void SendMIDIEvent(std::vector<unsigned char>* message);

   /**
    * @param vol 0 - 128
    */
   static void SetVolumeLevel(int vol);

   static int GetVolumeLevel(void);

   /**
    * @param intensity 0 - 100
    */
   static void IntensityLevel(int intensity);

   /**
    * @param filename MIDI file location
    * @param intensitylow
    */
   static void LoadMIDIFile(char* filename, int intensitylow, int intensityhigh);

private:


   static bool isInit;
   static int  Volume;
   static int  Intensity;

   static std::vector<musicfile*> Tracks;

   static RtMidiOut* midiout;

};

#endif
