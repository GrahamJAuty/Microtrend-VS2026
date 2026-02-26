#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************

class CImportDataHandler
{
public:
	CImportDataHandler ( CWnd* pParent = NULL );
	void ScanAll();

private:
	void ImportDataRecords();

private:
	CWnd* m_pParent;
	bool m_bBackgroundMode;
};

//*******************************************************************
