//*******************************************************************
#include "..\SmartPay4ManagerBgnd\ImportSimsPhotoFiles.h"
/*********************************************************************/

CImportSimsPhotoFiles::CImportSimsPhotoFiles( CSimsData* pSims, CWnd* pParent )
{
	m_pSims = pSims;
	m_pParent = NULL;
	Init();
}

//*******************************************************************
