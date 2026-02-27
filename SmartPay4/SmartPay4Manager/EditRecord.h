#pragma once
//*******************************************************************

class CEditRecord
{
public:
	CEditRecord ( const char* szLeaversYear, CWnd* pParent );
	bool Edit ( __int64 nUserID );

private:
	bool EditLeaverRecord(__int64 nUserID);
	bool EditNormalRecord(__int64 nUserID);
	void LogEdit (__int64 nUserID, const char* szProcess );

private:
	void SelectRowForEdit(CSQLRowAccountFull& RowAccount, CString& strMsg);

private:
	CString m_strLeaversYear;
	CWnd* m_pParent;
};

//*******************************************************************
