/**********************************************************************/
#include "SmartPay4Manager.h"
/**********************************************************************/
#include "ExportPropertySheet.h"
/**********************************************************************/

CExportPropertySheet::CExportPropertySheet(CDbExportHandler* pDbExportHandler, CWnd* pParentWnd, UINT iSelectPage)
	: CSSAutoShutdownPropertySheet(AutoShutdownHelper, "Export Database", pParentWnd, iSelectPage)
{
	m_pDbExportHandler = pDbExportHandler;

	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags &= ~PSH_HASHELP;

	if (pDbExportHandler->m_nExportFlag != nDBEXPORTFIELDS_NONE)
	{
		AddPage(&m_Page1);										// Fields
		m_Page1.m_psp.dwFlags &= ~PSP_HASHELP;
		m_Page1.SetPointer(pDbExportHandler);
	}

	if (pDbExportHandler->m_strLabel.Left(3) != "DEL" && pDbExportHandler->m_strLabel.Left(3) != "EXP")		// not reqd if export on delete
	{
		AddPage(&m_Page2);										// Filters
		m_Page2.m_psp.dwFlags &= ~PSP_HASHELP;
		m_Page2.SetPointer(pDbExportHandler);

		AddPage(&m_Page3);										// conditional
		m_Page3.m_psp.dwFlags &= ~PSP_HASHELP;
		m_Page3.SetPointer(pDbExportHandler);

		if (pDbExportHandler->m_bSecondaryOutput == TRUE)
		{
			AddPage(&m_Page4);									// Output
			m_Page4.m_psp.dwFlags &= ~PSP_HASHELP;
			m_Page4.SetPointer(pDbExportHandler);
		}

		if (pDbExportHandler->m_nExportFlag == nDBEXPORTFIELDS_REPORT)
		{
			SetActivePage(1);
		}
	}
}

/**********************************************************************/
BEGIN_MESSAGE_MAP(CExportPropertySheet, CSSAutoShutdownPropertySheet)
END_MESSAGE_MAP()
/**********************************************************************/

BOOL CExportPropertySheet::CSSAutoShutdownPostInitDialog()
{
	SetWindowText ( m_pDbExportHandler->m_strTitle );
	return TRUE;
}

/**********************************************************************/
