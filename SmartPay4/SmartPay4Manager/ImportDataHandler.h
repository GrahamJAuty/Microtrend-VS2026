#pragma once
//*******************************************************************

class CImportDataHandler
{
public:
	CImportDataHandler ( CWnd* pParent = NULL );
	void SetBackgroundMode ( bool bMode ) { m_bBackgroundMode = bMode; }
	void ScanAll();

private:
	CWnd* m_pParent;
	bool m_bBackgroundMode;
};

//*******************************************************************
