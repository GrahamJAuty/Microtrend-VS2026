#pragma once
/**********************************************************************/
 
struct CEODFileInfo
{
public:
	CEODFileInfo();
	void Reset();
	
public:
	int Compare ( CEODFileInfo& source, int nHint = 0 );
	void Add	( CEODFileInfo& source );
	
public:
	CString m_strFirstLine;
	CString m_strFilename;
	CString m_strFilenameToDelete;
	int m_nFileLength;
};

/**********************************************************************/

class CEODFileMover
{
public:
	CEODFileMover();

public:
	void MoveReasonFilesNetwork( int nNetworkIdx, int nParam1 );	
	void MoveEODPluFileLocation( int nNwkIdx, int nLocIdx );
	void MoveEODCustomerFileLocation( int nNwkIdx, int nLocIdx );
	
public:
	void TidyReasonFiles( int nNetworkIdx );

public:
	int GetFileCount(){ return m_nFileCount; }

private:
	int m_nFileCount;
};

/**********************************************************************/
