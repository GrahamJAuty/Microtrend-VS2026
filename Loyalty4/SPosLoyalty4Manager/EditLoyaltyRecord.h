#pragma once
/******************************************************/
#define ACCOUNTEDIT_DATABASE 0
#define ACCOUNTEDIT_FIND 1
#define ACCOUNTEDIT_HOTLIST 2
/******************************************************/

class CEditLoyaltyRecord
{
public:
	CEditLoyaltyRecord();
	const char* GetNewDisplayLine(){ return m_strNewDisplayLine; }
	int GetNewColour(){ return m_nNewColour; }
	
public:
	bool EditRecord( int nAction, __int64 nUserID, CWnd* pParent );
	
private:
	//FIDDLE : THESE WERE USED FOR FIND ACTION
	CString m_strNewDisplayLine;
	int m_nNewColour;
};

/******************************************************/
