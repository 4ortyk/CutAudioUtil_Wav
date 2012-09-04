#include "stdafx.h"
#include "FileCuter.h"

#define WAV_HEADER_SIZE 44

void CFileCuter::setTime(const int time)
{
	m_TimeInSec = (time < 0) ? 0 : time;
}

int CFileCuter::getTime() const
{
	return m_TimeInSec;
}

bool CFileCuter::CreateWAVCutFile (const String& inputFileName, const String& outputFileName)
{
	if (inputFileName.empty() || outputFileName.empty())
		return false;

	///////////////////////////////////////////////////////////////////////
	// read wave data into buffer
	::std::ifstream inputFile(inputFileName, ::std::ios::binary|::std::ios::in);
	if (!inputFile)
		return false;

	std::streamoff begin = inputFile.tellg();
	inputFile.seekg (0, std::ios::end);
	std::streamoff end = inputFile.tellg();

	// read input file header
	inputFile.seekg(0, std::ios::beg);
	WavHeaderInfo inputHeader;
	inputFile.read((char*)&inputHeader, sizeof(inputHeader));
	m_Hz = inputHeader.SamplesPerSec;
	m_Mode = inputHeader.NumOfChannels;
	m_BytesPerSample = inputHeader.bitsPerSample / 8;
	
	unsigned long byteCount = m_TimeInSec * m_Hz;
	switch (m_BytesPerSample) {
	case 1:
		byteCount *= sizeof(unsigned char);
		break;
	case 2:
		byteCount *= sizeof(unsigned short);
		break;
	case 4:
		byteCount *= sizeof(unsigned);
		break;
	}
	
	switch (m_Mode) {
	case 2:
		byteCount *= 2;
		break;
	}

	if ((end-begin) < byteCount + WAV_HEADER_SIZE)
		return false;
	
	// read data
	char *buff = new char[byteCount];
	inputFile.read(buff, byteCount);
	inputFile.close();
	///////////////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////////////////
	::std::ofstream outputFile(outputFileName, ::std::ios::binary|::std::ios::out);
	if (!outputFile)
		return false;

	// header
	WavHeaderInfo header = InitWavHeader(byteCount);
	outputFile.write((char*)&header, sizeof(header));
	// data
	outputFile.write(buff, byteCount);
	outputFile.close();
	///////////////////////////////////////////////////////////////////////

	return true;
}

CFileCuter::WavHeaderInfo CFileCuter::InitWavHeader(const long int bytes)
{   
	/* quick and dirty */
	WavHeaderInfo header;
	header.RIFF[0] = 'R';
	header.RIFF[1] = 'I';
	header.RIFF[2] = 'F';
	header.RIFF[3] = 'F';									/*  0-3 */    

	header.ChunkSize = bytes + WAV_HEADER_SIZE - 8;			/*  4-7 */    

	header.WAVE[0] = 'W';
	header.WAVE[1] = 'A';
	header.WAVE[2] = 'V';
	header.WAVE[3] = 'E';
	header.fmt[0] = 'f';
	header.fmt[1] = 'm';
	header.fmt[2] = 't';
	header.fmt[3] = ' ';									/*  8-15 */   

	header.Subchunk1Size = 16;								/* 16-19 */ 
	header.AudioFormat = 1;									/* 20-21 */ 
	header.NumOfChannels = m_Mode;							/* 22-23 */ 
	header.SamplesPerSec = m_Hz;							/* 24-27 */ 
	header.bytesPerSec = m_Hz * m_Mode * m_BytesPerSample;  /* 28-31 */ 
	header.blockAlign = m_Mode * m_BytesPerSample;			/* 32-33 */ 
	header.bitsPerSample = 8 * m_BytesPerSample;			/* 34-35 */ 

	header.Subchunk2ID[0] = 'd';
	header.Subchunk2ID[1] = 'a';
	header.Subchunk2ID[2] = 't';
	header.Subchunk2ID[3] = 'a';							/* 36-39 */  
	
	header.Subchunk2Size = bytes;							/* 40-43 */

	return header;
}
