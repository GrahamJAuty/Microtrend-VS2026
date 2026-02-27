#pragma once
//*********************************************************************

class CFileRecycler
{
public:
	CFileRecycler(){}
	static void SendToRecycleBin ( const char* szFileMask );
};

//*********************************************************************

