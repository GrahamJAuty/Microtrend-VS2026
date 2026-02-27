#pragma once
//$$******************************************************************

struct CEditAccountBuffer
{
public:
	CEditAccountBuffer() {}
	
public:
	CString m_strCardName = "";
	CString m_strNewPINNumber = "";
	int m_nGroupNo = 1;
	bool m_bOwnMaxSpend = FALSE;
};

//$$******************************************************************
