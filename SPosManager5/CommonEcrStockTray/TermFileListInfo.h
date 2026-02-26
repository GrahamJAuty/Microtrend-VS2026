#pragma once
/**********************************************************************/

struct CTermFileListInfo
{
public:
	CTermFileListInfo();
	void Reset();

public:
	int Compare ( CTermFileListInfo& source, int nHint = 0 );
	void Add	( CTermFileListInfo& source );
	
public:
	bool IsLegacySalesFolder();

public:
	int m_nSelectArrayIdx;
	int m_nCashRSPMode;
	int m_nDbIdx;
	int m_nLocIdx;
	int m_nTermIdx;
	int m_nRange;
	bool m_bIsActivePMSFile;
	CString m_strFilename;
	CString m_strDateFolder;
	CString m_strDateTran;
	CString m_strDateSale;
};

/**********************************************************************/