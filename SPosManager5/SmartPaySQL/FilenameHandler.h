#pragma once
//*******************************************************************

class CFilenameHandler
{

public:
	CFilenameHandler() {};

public:
	CString GetSQLConnectionOptionsFilename();
	CString GetSQLDatabaseFilename();

private:
	CString GetFilename(const CString strFolder, const CString strFilename);
};

//*******************************************************************
extern CFilenameHandler SQLFilenames;
//*******************************************************************
