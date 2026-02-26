#pragma once
//**********************************************************************

class CAccountIdentifier
{
public:
	CAccountIdentifier( const char* szID, int nIDType );

public:
	const char* GetAccountID(){ return m_strID; }
	int GetAccountIDType(){ return m_nIDType; }

private:
	CString m_strID;
	int m_nIDType;
};

//**********************************************************************
