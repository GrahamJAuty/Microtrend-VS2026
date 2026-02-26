/**********************************************************************/
 
/**********************************************************************/
#include "CustomerStatementErrorInfo.h"
//#include "DataManagerNonDb.h"
#include "EposReportCustomerStatement.h"
#include "LocationCSVArray.h"
#include "SMTPEmailHelpers.h"
/**********************************************************************/
#include "CustomerStatementErrorDlg.h"
/**********************************************************************/

IMPLEMENT_DYNAMIC(CCustomerStatementErrorDlg, CDialog)

/**********************************************************************/

CCustomerStatementErrorDlg::CCustomerStatementErrorDlg( int nType, int nAttempted, CArray<CCustomerStatementErrorInfo,CCustomerStatementErrorInfo>& arrayErrors, CWnd* pParent)
	: CDialog(CCustomerStatementErrorDlg::IDD, pParent), m_arrayErrors( arrayErrors )
{
	m_nType = nType;
	m_nAttempted = nAttempted;
}

/**********************************************************************/

CCustomerStatementErrorDlg::~CCustomerStatementErrorDlg()
{
}

/**********************************************************************/

void CCustomerStatementErrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TEXT, m_staticText);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CCustomerStatementErrorDlg, CDialog)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CCustomerStatementErrorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	CString strText = "";
	
	int nErrorCount = 0;
	bool bGotOK = FALSE;
	for ( ; nErrorCount < m_arrayErrors.GetSize(); nErrorCount++ )
	{
		switch( m_arrayErrors[nErrorCount].m_nStatementError )
		{
		case CUSTOMER_STATEMENT_ERROR_NONE:
		case CUSTOMER_STATEMENT_ERROR_REDIRECT:
			bGotOK = TRUE;
			break;
		}

		if ( TRUE == bGotOK )
			break;
	}

	CString strAction = "";
	switch( m_nType )
	{
	case CUSTOMER_STATEMENT_DISPLAY:	strAction = "displayed";				break;
	case CUSTOMER_STATEMENT_PRINT:		strAction = "submitted for printing";	break;
	case CUSTOMER_STATEMENT_EMAIL:		strAction = "submitted for email";		break;
	default:							strAction = "processed";				break;
	}

	if ( 0 == nErrorCount )
	{	
		strText = "The requested statement";
		strText += ( 1 == m_nAttempted ) ? " was " : "s were ";
		strText += "successfully ";
		strText += strAction;
		strText += ".";
	}
	else
	{
		if ( 1 == m_nAttempted )
			strText = "The";
		else
			strText.Format( "%d of %d", nErrorCount, m_nAttempted );

		strText += " requested statement";
		
		if ( 1 != m_nAttempted )
			strText += "s";

		strText += " could not be ";
		strText += strAction;
		strText += ".";
	}
			
	m_staticText.SetWindowText( strText );
	
	m_listErrors.SubclassDlgItem ( IDC_LIST, this );
	m_listErrors.InsertColumn ( 0, "Location", LVCFMT_LEFT, 140 );
	m_listErrors.InsertColumn ( 1, "Customer", LVCFMT_LEFT, 130 );
	m_listErrors.InsertColumn ( 2, "Result", LVCFMT_LEFT, 200 );
	m_listErrors.SetExtendedStyle ( LVS_EX_FULLROWSELECT );

	for ( int n = 0; n < m_arrayErrors.GetSize(); n++ )
	{
		CCustomerStatementErrorInfo infoError = m_arrayErrors.GetAt( n );

		CCustomerSelectionListAccountInfo infoCustomer;
		DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( infoError.m_nNameTableIdx, infoCustomer );

		CString strError = "";
		infoError.GetErrorText( strError );

		m_listErrors.InsertItem ( n, "" );
		m_listErrors.SetItem ( n, 0, LVIF_TEXT, dbLocation.GetName( infoCustomer.m_nLocIdx ), 0, 0, 0, NULL );
		m_listErrors.SetItem ( n, 1, LVIF_TEXT, infoCustomer.GetReportName(), 0, 0, 0, NULL );
		m_listErrors.SetItem ( n, 2, LVIF_TEXT, strError, 0, 0, 0, NULL );
	}

	return TRUE;  
}

/**********************************************************************/