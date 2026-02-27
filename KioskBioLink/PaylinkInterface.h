#pragma once
//*******************************************************************
#include "Aesimhei.h"
//*******************************************************************

class CPaylinkInterface
{
public:
	CPaylinkInterface();

	bool Enable(CWnd* pParentWnd);
	bool IsStartupOK() { return m_bStartLevelOK; }

	void Disable() { DisableInterface(); }
	long GetCurrentValue() { return CurrentValue(); }

	CString GetError() { return m_strError; }

private:
	bool CheckStartLevel(CWnd* pParentWnd);
	bool CheckSerialNumber();

private:
	CString m_strError = "";
	bool m_bStartLevelOK = TRUE;
};

//*******************************************************************
