#pragma once
//$$******************************************************************

struct CEditAccountBuffer
{
public:
	CEditAccountBuffer() { m_strCardName = ""; m_nGroupNo = 1; }

public:
	CString m_strCardName;
	int m_nGroupNo;
};

//$$******************************************************************
