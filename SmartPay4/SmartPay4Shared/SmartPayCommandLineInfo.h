#pragma once
//$$******************************************************************

class CSmartPayCommandLineInfo : public CCommandLineInfo
{
public:
	CSmartPayCommandLineInfo(bool bAllowEmailMode);

public:
	virtual void ParseParam(const char* szParam, BOOL bFlag, BOOL bLast);
	int GetSize() { return m_arrayParam.GetSize(); }
	const char* GetAt(int n) { return m_arrayParam.GetAt(n); }

public:
	bool GetEmailModeFlag() { return m_bEmailMode; }
	
private:
	CStringArray m_arrayParam;
	bool m_bAllowEmailMode;
	bool m_bEmailMode;
};

//$$******************************************************************

