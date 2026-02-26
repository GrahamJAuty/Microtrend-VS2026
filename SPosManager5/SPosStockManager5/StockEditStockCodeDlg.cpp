/*********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
 
/*********************************************************************/
#include "StockEditStockCodeDlg.h"
/*********************************************************************/

CStockEditStockCodeDlg::CStockEditStockCodeDlg( CSSListCtrlVirtual& StockList, CStockFilterArray* pFilterArray, CWnd* pParent)
	: CDialog(CStockEditStockCodeDlg::IDD, pParent), m_StockList ( StockList )
{
	//{{AFX_DATA_INIT(CStockEditStockCodeDlg)
	m_strNewStockCode = _T("");
	//}}AFX_DATA_INIT
	m_pFilterArray = pFilterArray;
}

/*********************************************************************/

void CStockEditStockCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockEditStockCodeDlg)
	DDX_Control(pDX, IDC_EDIT_NEWSTOCKCODE, m_editNewStockCode);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_NEWSTOCKCODE, m_strNewStockCode);
	DDV_MaxChars(pDX, m_strNewStockCode, Stock::StockCode.Max);
}

/*********************************************************************/

BEGIN_MESSAGE_MAP(CStockEditStockCodeDlg, CDialog)
	//{{AFX_MSG_MAP(CStockEditStockCodeDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*********************************************************************/

BOOL CStockEditStockCodeDlg::OnInitDialog() 
{
	GetRecordData();
	CDialog::OnInitDialog();
	UpdateTitle();
	m_editNewStockCode.SetFocus();	
	m_editNewStockCode.SetSel ( 0, -1 );
	return TRUE;  
}

/*********************************************************************/

void CStockEditStockCodeDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if ( pNMUpDown -> iDelta > 0 )
		SpinPrevious();
	else
		SpinNext();
	
	*pResult = 0;
}

/*********************************************************************/

void CStockEditStockCodeDlg::SpinNext() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_pFilterArray -> HasNext() == TRUE )
		{
			if ( ProcessStockCode() == TRUE )
			{
				m_pFilterArray -> Next();
				GetRecordData();
				UpdateTitle();
				UpdateData ( FALSE );
				m_editNewStockCode.SetFocus();
				m_editNewStockCode.SetSel ( 0, -1 );
				m_StockList.SetCurSel( m_pFilterArray -> GetArrayIdx() );
			}
		}
	}
}

/*********************************************************************/

void CStockEditStockCodeDlg::SpinPrevious() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_pFilterArray -> HasPrevious() == TRUE )
		{
			if ( ProcessStockCode() == TRUE )
			{
				m_pFilterArray -> Previous();
				GetRecordData();
				UpdateTitle();
				UpdateData ( FALSE );
				m_editNewStockCode.SetFocus();
				m_editNewStockCode.SetSel ( 0, -1 );
				m_StockList.SetCurSel( m_pFilterArray -> GetArrayIdx() );
			}
		}
	}
	
}

/*********************************************************************/

void CStockEditStockCodeDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( ProcessStockCode() == FALSE )
			return;

		ProcessStockCode();
		EndDialog ( IDOK );
	}
}

/*********************************************************************/

void CStockEditStockCodeDlg::GetRecordData()
{
	CStockCSVRecord StockRecord;
	int nStockIdx = m_pFilterArray -> GetStockItemIndex();
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );
	m_strNewStockCode = m_pFilterArray -> GetNewStockCode( nStockIdx );
}

/*********************************************************************/

bool CStockEditStockCodeDlg::ProcessStockCode()
{
	::TrimSpaces ( m_strNewStockCode, FALSE );
	
	if ( m_strNewStockCode == "" )
	{
		Prompter.Error ( "The new stock code for this\nitem cannot be blank" );
		return FALSE;
	}
	
	int nStockIdx = m_pFilterArray -> GetStockItemIndex();
	
	for ( int nCheckIdx = 0; nCheckIdx < DataManager.Stock.GetSize(); nCheckIdx++ )
	{
		if ( nCheckIdx != nStockIdx && m_strNewStockCode == m_pFilterArray -> GetNewStockCode ( nCheckIdx ) )
		{
			CString strMsg = "You have already assigned the stock code\n";
			strMsg += m_strNewStockCode;
			strMsg += " to another item";

			Prompter.Error ( strMsg );
			return FALSE;
		}
	}
	m_pFilterArray -> SetNewStockCode ( nStockIdx, m_strNewStockCode );
	return TRUE;
}

/*********************************************************************/

void CStockEditStockCodeDlg::UpdateTitle()
{
	CStockCSVRecord StockRecord;
	int nStockIdx = m_pFilterArray -> GetStockItemIndex();
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );
	
	CString strText; 
	strText.Format ( "Stock Code %s %s",
		StockRecord.GetStockCode(),
		StockRecord.GetDescription() );
		
	SetWindowText(strText);
}

/*********************************************************************/

void CStockEditStockCodeDlg::OnDefaultButton() 
{
	SpinNext();	
}

/*********************************************************************/
