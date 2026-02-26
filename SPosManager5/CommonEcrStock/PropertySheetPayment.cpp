/**********************************************************************/
#include "PropertySheetPayment.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetPayment, CSpinPropertySheet)
/**********************************************************************/

CPropertySheetPayment::CPropertySheetPayment( WORD wSpinID, int nPaymentIdx, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pWndParent)
	 : CSpinPropertySheet( TRUE, wSpinID, "", pWndParent ), m_WarnCancelAfterChange( WarnCancelAfterChange )
{	
	m_pListDataDlg = NULL;

	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	m_Page1.SetPaymentRecord ( &m_PaymentRecord );
	AddPage( &m_Page1 );
	m_PageArray.Add ( 1 );

	m_Page2.SetPaymentRecord ( &m_PaymentRecord );
	AddPage( &m_Page2 );
	m_PageArray.Add ( 2 );

	m_nPaymentIdx = nPaymentIdx;
	DataManager.Payment.GetAt ( m_nPaymentIdx, m_PaymentRecord );
	UpdateTitle();
}

/**********************************************************************/

CPropertySheetPayment::~CPropertySheetPayment()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetPayment, CSpinPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetPayment)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CPropertySheetPayment::SpinNext()
{
	if ( m_nPaymentIdx < DataManager.Payment.GetSize() - 1 )
	{
		int nPage = m_PageArray.GetAt ( GetActiveIndex() );

		bool bUpdate = FALSE;

		switch ( nPage )
		{
		case 1:	bUpdate = m_Page1.UpdateRecord();	break;
		case 2:	bUpdate = m_Page2.UpdateRecord();	break;
		}
		
		if ( bUpdate == FALSE )
			return;

		SaveRecord();
		m_nPaymentIdx++;
		DataManager.Payment.GetAt ( m_nPaymentIdx, m_PaymentRecord );
		UpdateTitle();

		switch ( nPage )
		{
		case 1:	m_Page1.Refresh();	break;
		case 2:	m_Page2.Refresh();	break;
		}

		if ( m_pListDataDlg != NULL )
			m_pListDataDlg -> SelectLine ( m_nPaymentIdx );
	}	
}

/**********************************************************************/

void CPropertySheetPayment::SpinPrevious()
{
	if ( m_nPaymentIdx >= 1 )
	{
		int nPage = m_PageArray.GetAt ( GetActiveIndex() );

		bool bUpdate = FALSE;

		switch ( nPage )
		{
		case 1:	bUpdate = m_Page1.UpdateRecord();	break;
		case 2:	bUpdate = m_Page2.UpdateRecord();	break;
		}
		
		if ( bUpdate == FALSE )
			return;

		SaveRecord();
		m_nPaymentIdx--;
		DataManager.Payment.GetAt ( m_nPaymentIdx, m_PaymentRecord );
		UpdateTitle();

		switch ( nPage )
		{
		case 1:	m_Page1.Refresh();	break;
		case 2:	m_Page2.Refresh();	break;
		}

		if ( m_pListDataDlg != NULL )
			m_pListDataDlg -> SelectLine ( m_nPaymentIdx );
	}
}

/**********************************************************************/

void CPropertySheetPayment::SaveRecord()
{
	DataManager.Payment.SetAt ( m_nPaymentIdx, m_PaymentRecord );
	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CPropertySheetPayment::UpdateTitle()
{
	CString strTitle;
	strTitle.Format ( "P%2.2d %s", m_PaymentRecord.GetPaymentNo(), m_PaymentRecord.GetDisplayName() );
	SetTitle ( strTitle, 0 );
}

/**********************************************************************/
