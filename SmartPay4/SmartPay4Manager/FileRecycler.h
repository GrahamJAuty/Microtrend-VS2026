#pragma once
//*********************************************************************

class CFileRecycler
{
public:
	CFileRecycler(){}
	static int SendToRecycleBin ( const char* szFileMask, bool bReportError = TRUE );
};

//*********************************************************************

