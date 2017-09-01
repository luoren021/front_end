/**********************************************************************************
*  Copyright (c) 2017 The Csky authors. All Rights Reserved.
*
**********************************************************************************/

#ifndef AGC_H_
#define AGC_H_

#include <stddef.h>

/**********************************************************************************
* This function sets work mode of AGC, sample rate of source data,
* and prepares NS structure before NS process
*
* Input:
*      - mode          : 0: Unchanged,       1: AdaptiveAnalog , 
*                        2: AdaptiveDigital, 3: FixedDigital
*      - fs            : sampling frequency of source data
* Output:
*      - void *        : pointer to instance of AGC
**********************************************************************************/
void* CskyAgc_init(int mode, int fs);

/**********************************************************************************
* The input and output signals should always be 10ms (80 or 160 samples).
*
* This functions does AGC function
*
* Input
*      - agcInst       : pointer to AGC instance
*      - in_near       : Pointer to input data frame for mic
*      - num_bands     : Number of bands: 8K,16k ~1, 32k ~2
*      - frame_in_ms   : frame size in ms
*
* Output:
*      - out_agc       : Pointer to output data frame for each band
* return: 0:success, 1:failure
**********************************************************************************/
int CskyAgc_Process(void* agcInst,
	signed short* const* in_near,
	unsigned short num_bands,
	unsigned short frame_in_ms,
	signed short* const* out_agc);

/**********************************************************************************
* This function releases the memory of AGC instance
*
* Input:
*      - pInst       : Pointer to AGC instance
**********************************************************************************/
void CskyAgc_Release(void* pInst);

#endif