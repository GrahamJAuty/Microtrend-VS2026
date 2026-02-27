#pragma once
//*******************************************************************

class CAutoShutdownDlgStacker
{
public:
	CAutoShutdownDlgStacker(CSSAutoShutdownDialog* pHost);
	~CAutoShutdownDlgStacker();

private:
	CSSAutoShutdownDialog* m_pHost;
};

//*******************************************************************

class CAutoShutdownPropSheetStacker
{
public:
	CAutoShutdownPropSheetStacker(CSSAutoShutdownPropertySheet* pHost);
	~CAutoShutdownPropSheetStacker();

private:
	CSSAutoShutdownPropertySheet* m_pHost;
};

//*******************************************************************
