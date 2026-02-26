/**********************************************************************/
#include "DateRangeHelpers.h"
#include "StockReportNames.h"
/**********************************************************************/
#include "StockReportReorderDateDlg.h"
/**********************************************************************/

CStockReportReorderDateDlg::CStockReportReorderDateDlg( CWnd* pParent)
	: CDialog(CStockReportReorderDateDlg::IDD, pParent),
	m_DateSelectorRange( m_comboDates, m_DatePickerFrom, m_DatePickerTo )
{
	//{{AFX_DATA_INIT(CStockReportReorderDateDlg)
	m_bReorderItems = FALSE;
	//}}AFX_DATA_INIT
	m_strDateTo = "";
	m_strDateFrom = "";
}

/**********************************************************************/

void CStockReportReorderDateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockReportReorderDateDlg)
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	DDX_Check(pDX, IDC_CHECK_INCLUDE, m_bReorderItems);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockReportReorderDateDlg, CDialog)
	//{{AFX_MSG_MAP(CStockReportReorderDateDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DATES, OnSelectDate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockReportReorderDateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_DateSelectorRange.InitialiseDateControls();
	m_DateSelectorRange.AddStandardReportRanges();
	m_DateSelectorRange.SetComboSelection( 0, szRANGE_TODAY );

	{
		int nResult = m_comboDates.FindStringExact( 0, EcrmanOptions.GetReportsDefaultDateRange() );
			
		if ( nResult != CB_ERR )
			m_comboDates.SetCurSel( nResult );
		else
			m_comboDates.SetCurSel( 0 );

		OnSelectDate();
	}
	
	return TRUE;  
}

/**********************************************************************/

void CStockReportReorderDateDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		CTime time;
		
		m_DatePickerFrom.GetTime ( time );
		int nYear = time.GetYear();
		int nMonth = time.GetMonth();
		int nMDay = time.GetDay();

		m_strDateFrom.Format ( "%2.2d/%2.2d/%4.4d",
			nMDay, nMonth, nYear );
		
		m_DatePickerTo.GetTime ( time );
		nYear = time.GetYear();
		nMonth = time.GetMonth();
		nMDay = time.GetDay();
		
		m_strDateTo.Format ( "%2.2d/%2.2d/%4.4d",
			nMDay, nMonth, nYear );

		GetParent() -> PostMessage ( WM_APP, STOCK_REORDER, (long) this );
	}
}

/**********************************************************************/

void CStockReportReorderDateDlg::OnSelectDate() 
{
	m_DateSelectorRange.UpdateDateControls();
}

/**********************************************************************/

void CStockReportReorderDateDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CStockReportReorderDateDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/