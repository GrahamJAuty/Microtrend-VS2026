/**********************************************************************/
 
/**********************************************************************/
#include "FilterTypes.h"
//#include "globalfunctions.h"
#include "MaxLengths.h"
/**********************************************************************/
#include "ProcessedOrderDefineFilterDlg.h"
/**********************************************************************/

CProcessedOrderDefineFilterDlg::CProcessedOrderDefineFilterDlg( CProcessedOrderFilterArray& FilterArray, CWnd* pParent )
	: CDialog(CProcessedOrderDefineFilterDlg::IDD, pParent), m_OrderFilterArray ( FilterArray )
{
	//{{AFX_DATA_INIT(CProcessedOrderDefineFilterDlg)
	//}}AFX_DATA_INIT
	m_nSearchType = m_OrderFilterArray.GetSearchType();
	m_strSearchText = m_OrderFilterArray.GetDescription();
	m_bMatchCase = m_OrderFilterArray.GetMatchCase();
	m_nSearchType = m_OrderFilterArray.GetSearchType();
	m_bMatchDescription =  ( ( m_nSearchType & STOCK_FILTER_DESCRIPTION ) == 0 ) ? FALSE : TRUE;
	m_bMatchStockCode =  ( ( m_nSearchType & STOCK_FILTER_STOCKCODE ) == 0 ) ? FALSE : TRUE;
	m_bMatchSuppRef =  ( ( m_nSearchType & STOCK_FILTER_SUPPREF ) == 0 ) ? FALSE : TRUE;
}

/**********************************************************************/

void CProcessedOrderDefineFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessedOrderDefineFilterDlg)
	DDX_Check(pDX, IDC_CHECK_MATCHCASE, m_bMatchCase);
	DDX_Check(pDX, IDC_CHECK_DESCRIPTION, m_bMatchDescription);
	DDX_Check(pDX, IDC_CHECK_STOCKCODE, m_bMatchStockCode);
	DDX_Check(pDX, IDC_CHECK_SUPPREF, m_bMatchSuppRef);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_PLUNAME, m_strSearchText);
	DDV_MaxChars(pDX, m_strSearchText, MAX_LENGTH_SEARCH_TEXT);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CProcessedOrderDefineFilterDlg, CDialog)
	//{{AFX_MSG_MAP(CProcessedOrderDefineFilterDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CProcessedOrderDefineFilterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	return TRUE;  
}

/**********************************************************************/

void CProcessedOrderDefineFilterDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		::TrimSpaces ( m_strSearchText, FALSE );
		
		m_nSearchType = 0;
		if ( m_bMatchDescription == TRUE ) m_nSearchType |= STOCK_FILTER_DESCRIPTION;
		if ( m_bMatchStockCode == TRUE ) m_nSearchType |= STOCK_FILTER_STOCKCODE;
		if ( m_bMatchSuppRef == TRUE ) m_nSearchType |= STOCK_FILTER_SUPPREF;

		m_OrderFilterArray.SetDescriptionFilter ( m_strSearchText, m_nSearchType, ( m_bMatchCase != 0 ) );
		EndDialog ( IDOK );
	}
}

/**********************************************************************/

