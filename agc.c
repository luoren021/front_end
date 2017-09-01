//#include <stdio.h>
#include <stdlib.h>
#include "agc.h"
#include "legacy\gain_control.h"
#include "legacy\analog_agc.h" 

void* CskyAgc_init(int agcMode, int fs) 
{
	int minLevel = 0;
	int maxLevel = 255;
	LegacyAgc* stt = (LegacyAgc* )malloc(sizeof(LegacyAgc));
	if (!(stt))
		return NULL;
	stt->initFlag = 0;
	stt->lastError = 0; 

	WebRtcAgc_Init((void *)stt, minLevel, maxLevel, agcMode, fs);

	return (void *)stt;
}

int CskyAgc_Process(void* agcInst,
	signed short* const* in_near,
	unsigned short num_bands,
	unsigned short frame_in_ms,
	signed short* const* out_agc) 
{
	int framlen;
	int inMicLevel = 127;
	int outMicLevel = 0;
	unsigned char saturationWarning;
	LegacyAgc* stt = (LegacyAgc*)agcInst;

	framlen = frame_in_ms * stt->fs / 1000;
	if (stt->fs == 8000) {
		if (framlen != 80) {
			//printf(" Frame Length isn't 80. \n");
			return -1;
		}
	}
	else if (stt->fs == 16000 || stt->fs == 32000 || stt->fs == 48000) {
		if (framlen != 160) {
			//printf(" Frame Length isn't 160. \n");
			return -1;
		}
	}
	else {
		//printf(" Unknown sample rate:%d \n", stt->fs);
		return -1;
	}

	if (stt->agcMode == kAgcModeAdaptiveAnalog) {
		WebRtcAgc_AddMic(agcInst, in_near, num_bands, framlen);
	}
	else if (stt->agcMode == kAgcModeAdaptiveDigital) {
		WebRtcAgc_VirtualMic(agcInst, in_near, num_bands, framlen, inMicLevel, &outMicLevel);
	}

	WebRtcAgc_Process(agcInst, in_near, num_bands, out_agc, inMicLevel, &outMicLevel, 0, &saturationWarning);
	return 0;
}

void CskyAgc_Release(void* pInst) 
{ 
	WebRtcAgc_Free(pInst);
}