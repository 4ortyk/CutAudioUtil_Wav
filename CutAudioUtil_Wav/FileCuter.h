#pragma once

#include <iostream>

#ifndef UNICODE  
typedef ::std::string String;
#else
typedef ::std::wstring String;
#endif

class CFileCuter {
public:
	void setTime(const int time);	//set cut file time length in sec
	int getTime() const;			//get cut file time length in sec

	bool CreateWAVCutFile (const String& inputFileName, const String& outputFileName);

private:
	struct  WavHeaderInfo {
	char                RIFF[4];        /* RIFF Header      */ 	//Magic header 
	unsigned long       ChunkSize;      /* RIFF Chunk Size  */    
	char                WAVE[4];        /* WAVE Header      */    
	char                fmt[4];         /* FMT header       */    
	unsigned long       Subchunk1Size;  /* Size of the fmt chunk                                */   
	unsigned short      AudioFormat;    /* Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM */    
	unsigned short      NumOfChannels;  /* Number of channels 1=Mono 2=Sterio                   */    
	unsigned long       SamplesPerSec;  /* Sampling Frequency in Hz                             */   
	unsigned long       bytesPerSec;    /* bytes per second */   
	unsigned short      blockAlign;     /* 2=16-bit mono, 4=16-bit stereo */ 
	unsigned short      bitsPerSample;  /* Number of bits per sample      */  
	char                Subchunk2ID[4]; /* "data"  string   */   
	unsigned long       Subchunk2Size;  /* Sampled data length    */
};

private:
	// Create wav-file header
	WavHeaderInfo InitWavHeader(const long int bytes);

private:
	int m_TimeInSec;	// how long should be new cut file

	// this data takes from original file header
	unsigned long  m_Hz;
	unsigned short m_Mode;
	unsigned short m_BytesPerSample;
};