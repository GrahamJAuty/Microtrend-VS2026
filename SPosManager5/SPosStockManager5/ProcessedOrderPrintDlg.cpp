/**********************************************************************/
 
/**********************************************************************/
#include "DateRangeHelpers.h"
/**********************************************************************/
#include "ProcessedOrderPrintDlg.h"
/**********************************************************************/

CProcessedOrderPrintDlg::CProcessedOrderPrintDlg( const char* szTitle, bool bAllDates, CWnd* pParent)
	: CDialog(CProcessedOrderPrintDlg::IDD, pParent),
	m_DateSelectorRange( m_comboDates, m_DatePickerFrom, m_DatePickerTo )
{
	//{{AFX_DATA_INIT(CProcessedOrderPrintDlg)
	//}}AFX_DATA_INIT
	m_strTitle = szTitle;
	m_strDateFrom = "";
	m_strDateTo = "";
	m_bAllDates = bAllDates;
}

/**********************************************************************/

void CProcessedOrderPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessedOrderPrintDlg)
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CProcessedOrderPrintDlg, CDialog)
	//{{AFX_MSG_MAP(CProcessedOrderPrintDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DATES, OnSetDates)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CProcessedOrderPrintDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetWindowText ( m_strTitle );

	m_DateSelectorRange.InitialiseDateControls();
	m_DateSelectorRange.AddStandardReportRanges();

	if ( FALSE == m_bAllDates )
		m_DateSelectorRange.SetComboSelection( 0, szRANGE_TODAY );
	else
		m_DateSelectorRange.SetComboSelection( 0, szRANGE_ALL );

	return TRUE;  
}

/**********************************************************************/

void CProcessedOrderPrintDlg::OnSetDates() 
{
	m_DateSelectorRange.UpdateDateControls();
}

/**********************************************************************/

void CProcessedOrderPrintDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_DatePickerFrom.GetTime ( m_oleDateFrom );
		int nYear = m_oleDateFrom.GetYear();
		int nMonth = m_oleDateFrom.GetMonth();
		int nMDay = m_oleDateFrom.GetDay();

		m_strDateFrom.Format ( "%2.2d/%2.2d/%4.4d",
			nMDay, nMonth, nYear );
		
		m_DatePickerTo.GetTime ( m_oleDateTo );
		nYear = m_oleDateTo.GetYear();
		nMonth = m_oleDateTo.GetMonth();
		nMDay = m_oleDateTo.GetDay();
		
		m_strDateTo.Format ( "%2.2d/%2.2d/%4.4d",
			nMDay, nMonth, nYear );
		
		EndDialog ( IDOK );
	}
}

/**********************************************************************/

void CProcessedOrderPrintDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CProcessedOrderPrintDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/
