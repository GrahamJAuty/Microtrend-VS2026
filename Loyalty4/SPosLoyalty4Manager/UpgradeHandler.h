#pragma once
//$$******************************************************************

class CUpgradeHandler
{
public:
	CUpgradeHandler( CWnd* pParent = NULL);
	bool DefaultUpgradePossible();
	bool Upgrade();

private:
	CString GetSourceFolder();

private:
	CWnd* m_pParent;
	CString m_strSourcePath;
};

//$$******************************************************************
