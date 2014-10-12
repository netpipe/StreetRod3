#include "bass.h"

BOOL BASS_Init(int device, DWORD freq, DWORD flags, HWND win)
{
  return true;
}

void BASS_SampleFree(HSAMPLE handle)
{
}

HSAMPLE BASS_SampleLoad(BOOL mem, void *file, DWORD offset, DWORD length, DWORD max, DWORD flags)
{
  return 0;
}

BOOL BASS_ChannelStop(DWORD handle)
{
  return true;
}

void BASS_StreamFree(HSTREAM handle)
{
}

HSTREAM BASS_StreamCreateFile(BOOL mem, void *file, DWORD offset, DWORD length, DWORD flags)
{
  return 0;
}

BOOL BASS_ChannelSetAttributes(DWORD handle, int freq, int volume, int pan)
{
  return true;
}

BOOL BASS_StreamPlay(HSTREAM handle, BOOL flush, DWORD flags)
{
  return true;
}

BOOL BASS_ChannelIsActive(DWORD handle)
{
  return true;
}

HCHANNEL BASS_SamplePlay(HSAMPLE handle)
{
  return 0;
}

BOOL BASS_Start()
{
  return true;
}

BOOL BASS_SetVolume(DWORD volume)
{
  return true;
}

void BASS_Free()
{
}

BOOL BASS_Stop()
{
  return true;
}

DWORD BASS_ErrorGetCode()
{
  return 0;
}
