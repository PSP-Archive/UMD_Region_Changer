// mp3player.h: headers for psp mp3player code
//
// All public functions for mp3player
//
//////////////////////////////////////////////////////////////////////
#ifndef _MP3PLAYER_H_
#define _MP3PLAYER_H_

#include <mad.h>

void MP3_Init(int channel);
int MP3_Play();
void MP3_Pause();
int MP3_Stop();
void MP3_End();
void MP3_FreeTune();
int MP3_Load(char *filename);
void MP3_GetTimeString(char *dest);
int MP3_EndOfStream();

#endif
