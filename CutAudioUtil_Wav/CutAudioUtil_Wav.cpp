// CutAudioUtil_Wav.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iterator>
#include "FileCuter.h"

#define PARAMS_COUNT 3
#define DOT_SYMBOL _T('.')
#define WAV_EXTENTION _T("wav")

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < PARAMS_COUNT) {
		_tprintf_s(_T("\nPlease use format <audio_file> <time_in_sec>"));
		return 0;
	}

	// check audio file
	String audioFileName = argv[1];
	if (audioFileName.empty())  {
		_tprintf_s(_T("\nEmpty audio file name"));
		return 0;
	}

	const int dotPos = audioFileName.find_last_of(DOT_SYMBOL);
	if (dotPos == ::std::string::npos || (dotPos == audioFileName.length() - 1) || (dotPos == 0)) {
		_tprintf_s(_T("\nInvalid file format"));
		return 0;
	}

	String fileExtantion = audioFileName.substr(dotPos + 1);
	::std::transform(fileExtantion.begin(), fileExtantion.end(), fileExtantion.begin(), ::tolower);
	if (fileExtantion.compare(WAV_EXTENTION)) {
		_tprintf_s(_T("\nInvalid file format. Use wav audio file"));
		return 0;
	}

	// check time
	int time = _wtoi(argv[2]);
	if (time < 1) {
		_tprintf_s(_T("\nInvalid time interval"));
		return 0;
	}

	String cutFileName = _T("");
	cutFileName.append(audioFileName.substr(0, dotPos)); 
	cutFileName.append(_T("_Cut."));
	cutFileName.append(WAV_EXTENTION);

	CFileCuter wavFileCuter;
	wavFileCuter.setTime(time);
	bool bRes = wavFileCuter.CreateWAVCutFile(audioFileName, cutFileName);
	if (bRes) {
		_tprintf_s(_T("\nNew cut file %s is created successful.\n"), cutFileName);
	} else {
		_tprintf_s(_T("\nCan't create cut file.\n"));
	}

	return 0;
}