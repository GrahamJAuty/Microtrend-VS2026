/**********************************************************************/
#include "DateRangeHelpers.h"
#include "RepSpawn.h"
#include "SysInfo.h"
/**********************************************************************/
#include "CustomerPrintDlg.h"
/**********************************************************************/

CCustomerPrintDlg::CCustomerPrintDlg( int nLocIdx, bool bOneOff, CWnd* pParent)
	: CSSDialog(CCustomerPrintDlg::IDD, pParent),
	m_DateSelectorRange( m_comboDates, m_DatePickerFrom, m_DatePickerTo )
{
	//{{AFX_DATA_INIT(CCustomerPrintDlg)
	//}}AFX_DATA_INIT
	m_strTitle = "Select Date Range";
	m_strRangeText = "Date Range";
	m_nLocIdx = nLocIdx;
	m_bOneOff = bOneOff;
}

/**********************************************************************/

void CCustomerPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomerPrintDlg)
	DDX_Control(pDX, IDC_CHECK_SPOS, m_checkSPos);
	DDX_Control(pDX, IDC_CHECK_ECRMAN, m_checkEcrman);
	DDX_Control(pDX, IDC_STATIC_RANGE, m_staticRange);
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CCustomerPrintDlg, CDialog)
	//{{AFX_MSG_MAP(CCustomerPrintDlg)
	ON_BN_CLICKED( IDC_CHECK_SPOS, OnToggleDateTick)
	ON_BN_CLICKED( IDC_CHECK_ECRMAN, OnToggleDateTick)
	ON_CBN_SELCHANGE(IDC_COMBO_DATES, OnSelectDate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CCustomerPrintDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowText( m_strTitle );
	m_staticRange.SetWindowText( m_strRangeText );

	m_DateSelectorRange.InitialiseDateControls();
	m_DateSelectorRange.AddCustomerCreationDateRanges();
	m_DateSelectorRange.SetComboSelection( 0, szRANGE_ALL );
	OnSelectDate();

	m_checkSPos.SetCheck( TRUE );
	m_checkEcrman.SetCheck( FALSE );

	if ( SysInfo.IsStockSystem() == FALSE )
	{
		m_checkEcrman.SetWindowText( "Show Ecrman import date" );
	}
	else
	{
		m_checkEcrman.SetWindowText( "Show Stockman import date" );
	}

	return TRUE;  
}

/**********************************************************************/

void CCustomerPrintDlg::OnToggleDateTick() 
{
	if ( IsTicked( m_checkSPos ) || IsTicked( m_checkEcrman ) )
	{
		m_comboDates.EnableWindow( TRUE );
		OnSelectDate();
	}
	else
	{
		m_comboDates.EnableWindow( FALSE );
		m_DatePickerFrom.EnableWindow( FALSE );
		m_DatePickerTo.EnableWindow( FALSE );
	}
}

/**********************************************************************/

void CCustomerPrintDlg::OnSelectDate() 
{
	m_DateSelectorRange.UpdateDateControls();
}

/**********************************************************************/

void CCustomerPrintDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CCustomerPrintDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CCustomerPrintDlg::OnOK() 
{
	COleDateTime oleDateFrom, oleDateTo;
	m_DatePickerFrom.GetTime( oleDateFrom );
	m_DatePickerTo.GetTime( oleDateTo );
	
	if ( TRUE == m_bOneOff )
	{
		CReportFile ReportFile;
		if ( ReportFile.Open ( Super.ReportFilename() ) == TRUE )
		{
			DataManagerNonDb.CustomerOneOff.CreateCustomerReport( TRUE, m_nLocIdx, oleDateFrom, oleDateTo, IsTicked( m_checkSPos), IsTicked( m_checkEcrman ), ReportFile );
			ReportFile.Close();

			CFilenameUpdater FnUp ( SysFiles::CustomerOneOffPrm );
			CString strParamsFile = FnUp.GetFilenameToUse();
			
			CReportManager ReportManager;
			ReportManager.DisplayReport ( Super.ReportFilename(), strParamsFile, "", this );
		}
	}
	else
	{
		CReportFile ReportFile;
		if ( ReportFile.Open ( Super.ReportFilename() ) == TRUE )
		{
			DataManagerNonDb.CustomerAccount.CreateCustomerReport( FALSE, m_nLocIdx, oleDateFrom, oleDateTo, IsTicked( m_checkSPos), IsTicked( m_checkEcrman ), ReportFile );
			ReportFile.Close();

			CFilenameUpdater FnUp ( SysFiles::CustomerAccountPrm );
			CString strParamsFile = FnUp.GetFilenameToUse();
			
			CReportManager ReportManager;
			ReportManager.DisplayReport ( Super.ReportFilename(), strParamsFile, "", this );
		}
	}
}

/**********************************************************************/
