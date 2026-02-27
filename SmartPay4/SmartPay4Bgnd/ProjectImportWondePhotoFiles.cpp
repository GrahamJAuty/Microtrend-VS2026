/*********************************************************************/
#include "..\SmartPay4ManagerBgnd\ImportWondePhotoFiles.h"
/*********************************************************************/

CImportWondePhotoFiles::CImportWondePhotoFiles( CWondeData* pData, CWnd* pParent )
{
	m_pData = pData;
	m_pParent = pParent;
	Init();
}

/*********************************************************************/

