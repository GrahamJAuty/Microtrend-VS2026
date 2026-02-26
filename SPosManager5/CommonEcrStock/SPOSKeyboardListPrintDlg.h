#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSPOSKeyboardListPrintDlg : public CDialog
{
	DECLARE_DYNAMIC(CSPOSKeyboardListPrintDlg)

public:
	CSPOSKeyboardListPrintDlg(CWnd* pParent = NULL);
	virtual ~CSPOSKeyboardListPrintDlg();

	enum { IDD = IDD_SPOS_KEYBOARD_LIST_PRINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

	DECLARE_MESSAGE_MAP()

public:
	BOOL m_bListStandard;
	BOOL m_bListLarge;
	BOOL m_bFunctionStandard;
	BOOL m_bFunctionLarge;
	BOOL m_bFunctionBottom;

protected:
	virtual void OnOK();

private:
	void WriteLists( CReportFile& ReportFile, int nType );
};

/**********************************************************************/
