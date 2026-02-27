/*********************************************************************/
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\MembershipDatabase.h"
/*********************************************************************/
#include "..\SmartPay4ManagerBgnd\ImportSimsFile.h"
/*********************************************************************/

CImportSimsFile::CImportSimsFile ( CSimsData* pSims, CWnd* pParent )
{
	m_pSims = pSims;
	Init();
}

/*********************************************************************/
