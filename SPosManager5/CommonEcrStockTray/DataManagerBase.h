#pragma once
/**********************************************************************/
#define DATAMANAGER_CLOSED 0
#define DATAMANAGER_OPEN_READONLY 1
#define DATAMANAGER_OPEN_READWRITE 2
/**********************************************************************/
#define DATAMANAGER_ERROR_NONE 0
#define DATAMANAGER_ERROR_ALREADYOPEN 1001
#define DATAMANAGER_ERROR_NOTOPEN 1002
#define DATAMANAGER_ERROR_BADSUPPIDX 1003
#define DATAMANAGER_ERROR_READONLY 1004
#define DATAMANAGER_ERROR_WRITE 1005
//1006 WAS ERROR_FORCEREAD
#define DATAMANAGER_ERROR_SPIDX 1007
/**********************************************************************/

struct CDataManagerInfo
{
	int m_nNodeType;
	int m_nErrorType;
};

/**********************************************************************/

class CDataManagerBase
{
public:
	CDataManagerBase();
	~CDataManagerBase();

public:
	void SetNoLogWriteFlag( bool b ){ m_bNoLogWrite = b; }

protected:
	virtual bool OpenStringArray( CSharedStringArray& array, CDataManagerInfo& info, const char* szFilename, int& nCurrentState, int nNewState, bool bNoFileIsOK );
	virtual bool CloseStringArray( CSharedStringArray& array, CDataManagerInfo& info, int& nCurrentState );
	virtual bool WriteStringArray( CSharedStringArray& array, CDataManagerInfo& info, int& nCurrentState );
	
	virtual bool OpenCSVArray( CSharedCSVArray& array, CDataManagerInfo& info, const char* szFilename, int& nCurrentState, int nNewState, bool bNoFileIsOK );
	virtual bool CloseCSVArray( CSharedCSVArray& array, CDataManagerInfo& info, int& nCurrentState );
	virtual bool WriteCSVArray( CSharedCSVArray& array, CDataManagerInfo& info, int& nCurrentState );
	
	virtual void ResetInfo( CDataManagerInfo& info, int nNodeType );

private:
	bool m_bNoLogWrite;
};

/**********************************************************************/
