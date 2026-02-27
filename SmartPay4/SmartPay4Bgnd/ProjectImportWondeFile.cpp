/*********************************************************************/
#include "..\SmartPay4ManagerBgnd\ImportWondeFile.h"
/*********************************************************************/

CImportWondeFile::CImportWondeFile( CWondeData* pData, CWnd* pParent )
{
	m_pData = pData;
	m_pParent = pParent;
	Init();
}

/*********************************************************************/
