/**********************************************************************/
#include "..\SPosStockManager5\StockpointCSVArray.h"
/**********************************************************************/
#include "..\CommonEcrStock\DefEditDlg.h"
/**********************************************************************/
#include "ButtonDefines.h"
#include "ButtonManager.h"
#include "KeyboardSetCSVArray.h"
#include "ListDataDlg.h"
#include "LocationCSVArray.h"
#include "LocationSetCSVArray.h"
#include "ReportpointCSVArray.h"
/**********************************************************************/

CDefEditDlg::CDefEditDlg(CWnd* pParent)
	: CSSDialog(CDefEditDlg::IDD, pParent)
{
	m_strPrompt = "";
	m_bPrevious = FALSE;
	m_nMaxLen = 0;
}

/**********************************************************************/

CDefEditDlg::CDefEditDlg( const char* szTitle, const char* szName, int nMaxLen, const char* szPrompt, CWnd* pParent)
	: CSSDialog(CDefEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDefEditDlg)
	//}}AFX_DATA_INIT
	m_strTitle = szTitle;
	m_strName = szName;
	m_nMaxLen = nMaxLen;
	m_strPrompt = szPrompt;
	m_bPrevious = FALSE;
}

/**********************************************************************/

void CDefEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditDlg)
	DDX_Control(pDX, IDC_STATIC_REF, m_staticRef);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_RADIO_PREVIOUS, m_radioPrevious);
	DDX_Control(pDX, IDC_RADIO_NEXT, m_radioNext);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditDlg)
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_RADIO_PREVIOUS,OnRadioPrevious)
	ON_BN_CLICKED(IDC_RADIO_NEXT,OnRadioNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetWindowText ( m_strTitle );

	if (m_strPrompt != "")
	{
		m_staticRef.SetWindowText(m_strPrompt);
	}

	m_editName.LimitText( m_nMaxLen );
	m_editName.SetWindowText( m_strName );

	return (AdjustDialogControls() == FALSE);
}

/**********************************************************************/

bool CDefEditDlg::AdjustDialogControls()
{
	ShowAndEnableWindow(&m_radioPrevious, FALSE);
	ShowAndEnableWindow(&m_radioNext, FALSE);
	return FALSE;
}

/**********************************************************************/

void CDefEditDlg::OnRadioPrevious()
{
	m_radioPrevious.SetCheck(TRUE);
	m_radioNext.SetCheck(FALSE);
	m_bPrevious = TRUE;
}

/**********************************************************************/

void CDefEditDlg::OnRadioNext()
{
	m_radioPrevious.SetCheck(FALSE);
	m_radioNext.SetCheck(TRUE);
	m_bPrevious = FALSE;
}

/**********************************************************************/

void CDefEditDlg::HandleOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_editName.GetWindowText( m_strName );
		::TrimSpaces ( m_strName, FALSE );
		EndDialog ( IDOK );
	}
}

/**********************************************************************/

void CDefEditDlg::HandleCancel()
{
	EndDialog(IDCANCEL);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CChangePasswordDlg::CChangePasswordDlg(const char* szTitle, const char* szName, int nMaxLen, const char* szPrompt, CWnd* pParent) : CDefEditDlg(szTitle, szName, nMaxLen, szPrompt, pParent)
{
}

/**********************************************************************/

bool CChangePasswordDlg::AdjustDialogControls()
{
	CDefEditDlg::AdjustDialogControls();
	ResizeDialog(341, 66);
	ResizeControl(&m_editName, 250, 14);
	MoveControl(&m_buttonOK, 284, 29);
	MoveControl(&m_buttonCancel, 284, 45);
	return FALSE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataSearchDlg::CListDataSearchDlg(CListDataSearchDlg** pParentToThis, CString strTitle, CListDataDlg* pParent) : CDefEditDlg(pParent)
{
	m_pParent = pParent;
	m_pParentToThis = pParentToThis;
	*m_pParentToThis = this;
	m_strTitle = strTitle;
	m_strName = "";
	m_strPrompt = "Search text";
	m_nMaxLen = 30;
	m_bPrevious = FALSE;
}

/**********************************************************************/

void CListDataSearchDlg::LoadSettings(bool bPrevious, CString strText)
{
	m_bPrevious = bPrevious;
	m_radioPrevious.SetCheck(TRUE == m_bPrevious);
	m_radioNext.SetCheck(FALSE == m_bPrevious);
	m_editName.SetWindowText(strText);
	m_editName.SetSel(0, -1);
	m_editName.SetFocus();

}

/**********************************************************************/

bool CListDataSearchDlg::AdjustDialogControls()
{
	m_radioPrevious.SetCheck(TRUE == m_bPrevious);
	m_radioNext.SetCheck(FALSE == m_bPrevious);
	m_buttonOK.SetWindowText("Search");
	MoveControl(&m_buttonOK, 214, 45);
	m_buttonCancel.ShowWindow(SW_HIDE);
	m_editName.SetFocus();
	return TRUE;
}

/**********************************************************************/

void CListDataSearchDlg::HandleOK()
{
	m_strName = GetEditBoxText(m_editName);
	m_pParent->SaveSearchSettings(m_bPrevious, m_strName);
	m_pParent->SendMessage(WM_APP);
}

/**********************************************************************/

void CListDataSearchDlg::HandleCancel()
{
	m_strName = GetEditBoxText(m_editName);
	m_pParent->SaveSearchSettings(m_bPrevious, m_strName);
	DestroyWindow();
}

/**********************************************************************/

void CListDataSearchDlg::PostNcDestroy()
{
	*m_pParentToThis = NULL;
	delete this;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CChangeReportPointNameDlg::CChangeReportPointNameDlg( int nIndex, CWnd* pParent ) : CDefEditDlg ( pParent )
{
	m_strTitle = dbReportpoint.GetSystemName( nIndex );
	m_strName = dbReportpoint.GetName( nIndex );
	m_nMaxLen = 25;
}

/**********************************************************************/

CChangeKeyboardSetNameDlg::CChangeKeyboardSetNameDlg( int nIndex, CWnd* pParent ) : CDefEditDlg ( pParent )
{
	m_strTitle = dbKeyboardSet.GetSystemName( nIndex );
	m_strName = dbKeyboardSet.GetName( nIndex );
	m_nMaxLen = 25;
}

/**********************************************************************/

CChangeLocationSetNameDlg::CChangeLocationSetNameDlg( int nIndex, CWnd* pParent ) : CDefEditDlg ( pParent )
{
	m_strTitle = dbLocationSet.GetSystemName( nIndex );
	m_strName = dbLocationSet.GetName( nIndex );
	m_nMaxLen = 25;
}

/**********************************************************************/

CChangeSBLocationNameDlg::CChangeSBLocationNameDlg( int nIndex, CWnd* pParent ) : CDefEditDlg ( pParent )
{
	m_strTitle = dbLocation.GetSystemName( nIndex );
	m_strName = dbLocation.GetName( nIndex );
	m_nMaxLen = 25;
}

/**********************************************************************/

CChangePaymentNameDlg::CChangePaymentNameDlg( int nIndex, CWnd* pParent ) : CDefEditDlg ( pParent )
{
	CPaymentCSVRecord Payment;
	DataManager.Payment.GetAt( nIndex, Payment );
	m_strTitle.Format( "Payment %d", Payment.GetPaymentNo() );
	m_strName = Payment.GetEPOSName();
	m_nMaxLen = Payment::PaymentEPOSName.Max;
}

/**********************************************************************/

CChangeListNameDlg::CChangeListNameDlg( int nIndex, CWnd* pParent ) : CDefEditDlg ( pParent )
{
	m_strTitle.Format( "Set List Name (List %d)", nIndex );
	m_strName = ButtonManager.GetListName(nIndex);
	m_nMaxLen = BUTMGR_MAX_LISTNAME;
}

/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/

CChangeRoomGroupNameDlg::CChangeRoomGroupNameDlg( int nIndex, CWnd* pParent ) : CDefEditDlg ( pParent )
{
	m_strTitle.Format( "Set Room Group Name (Group %d)", nIndex );
	m_strName = DataManagerNonDb.PMSRoomGroup.GetRoomGroupName( nIndex );
	m_nMaxLen = 20;
}

/**********************************************************************/
#endif
/**********************************************************************/

CChangeSystemDepartmentNameDlg::CChangeSystemDepartmentNameDlg( int nIndex, CWnd* pParent ) : CDefEditDlg ( pParent )
{
	CDepartmentCSVRecord Dept;
	DataManagerNonDb.SystemDepartment.GetAt( nIndex, Dept );

	m_strTitle.Format( "Set System Dept Name (Dept %d)", Dept.GetDeptNo() );
	m_strName = Dept.GetReportText();
	m_nMaxLen = Dept::RepText.Max;
}

/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/

CChangeEcrmanSupplierNameDlg::CChangeEcrmanSupplierNameDlg( int nIndex, CWnd* pParent ) : CDefEditDlg ( pParent )
{
	CEcrmanSupplierCSVRecord Supplier;
	DataManagerNonDb.EcrmanSupplier.GetAt( nIndex, Supplier );

	m_strTitle.Format ( "Supplier %d", Supplier.GetSuppNo() );
	m_strName = Supplier.GetSuppName(); 
	m_nMaxLen = EcrmanSupplier::SupplierName.Max;
}

/**********************************************************************/

CFindEcrmanSuppRefDlg::CFindEcrmanSuppRefDlg( CWnd* pParent ) : CDefEditDlg ( pParent )
{
	m_strTitle = "Find Supplier Reference";
	m_strName = "";
	m_strPrompt = "Reference";
	m_nMaxLen = Plu::SupplierRef.Max;
}

/**********************************************************************/
#endif
/**********************************************************************/

