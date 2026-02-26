/**********************************************************************/
#pragma once
/**********************************************************************/
#include "PluCSVArray.h"
/**********************************************************************/

class CSpecialChangesHelper
{
public:
	CSpecialChangesHelper(void);
	
public:
	void InitialiseDownloadJob( int nLocIdx, int nTerminalNo, int nParam1 );
	void FinaliseDownloadJob();
	
public:
	bool HasPluChanged( CPluCSVRecord& PluRecord );
	void NoteDownloadedPlu( CPluCSVRecord& PluRecord );

private:
	const char* GetLocPluFilename();

private:
	int m_nLocIdx;
	int m_nTerminalNo;
	int m_nParam1;
	CPluCSVArray m_arrayEcrPlu;
	CPluCSVArray m_arrayCurrentDownload;
	CString m_strLocPluFilename;
};

/**********************************************************************/
extern CSpecialChangesHelper SpecialChangesHelper;
/**********************************************************************/
