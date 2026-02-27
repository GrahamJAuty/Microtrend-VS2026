#pragma once
//*******************************************************************
#include "AutoImportData.h"
//*******************************************************************

class CAutoImportHandler
{
public:
	CAutoImportHandler(CWnd* pParent );
	int ImportAccounts();
	int ImportPhotoIDs();

private:
	bool m_bBackgroundMode;
	CWnd* m_pParent;

	CAutoImportData m_data;
};

//*******************************************************************
