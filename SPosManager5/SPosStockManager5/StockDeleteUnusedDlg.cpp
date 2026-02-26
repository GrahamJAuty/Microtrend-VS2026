/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
#include "DateRangeHelpers.h"
#include "MessageLogger.h"
#include "NodeTypes.h"
#include "Password.h"
 
#include "SalesHistoryReportStock.h"
#include "StockDeleteDatabaseDlg.h"
/**********************************************************************/
#include "StockDeleteUnusedDlg.h"
/**********************************************************************/

CStockDeleteUnusedDlg::CStockDeleteUnusedDlg( CWnd* pParent )
	: CDialog(CStockDeleteUnusedDlg::IDD, pParent),
	m_DateSelectorSingle( m_comboDates, m_DatePickerTo )
{
	//{{AFX_DATA_INIT(CStockDeleteUnusedDlg)
	m_bKeepSales = FALSE;
	//}}AFX_DATA_INIT
	m_strDate = "";
	m_bKeepApparent = TRUE;
	m_bKeepOnOrder = TRUE;
	m_bKeepSales = TRUE;
}

/**********************************************************************/

void CStockDeleteUnusedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockDeleteUnusedDlg)
	DDX_Control(pDX, IDC_COMBO_DATABASE, m_comboDatabase);
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	DDX_Check(pDX, IDC_CHECK_KEEPAPPARENT, m_bKeepApparent);
	DDX_Check(pDX, IDC_CHECK_KEEPONORDER, m_bKeepOnOrder);
	DDX_Check(pDX, IDC_CHECK_KEEPSALES, m_bKeepSales);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockDeleteUnusedDlg, CDialog)
	//{{AFX_MSG_MAP(CStockDeleteUnusedDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DATES, OnSelectDate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockDeleteUnusedDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CWordArray arrayTypes;
	arrayTypes.Add ( NODE_DATABASE );

	if ( PasswordArray.GetLocationAccessType() != LOCATION_ACCESS_SYSTEM )
		m_LocationSelector.SetSingleDbIdx( PasswordArray.GetLocationAccessDbIdx() );

	m_LocationSelector.BuildList( arrayTypes );
	m_LocationSelector.FillLocationCombo ( &m_comboDatabase );

	m_DateSelectorSingle.InitialiseDateControl();
	m_DateSelectorSingle.AddStandardReportRanges( FALSE );
	
	CString strType;
	CDateRangeHelpers DateRangeHelpers;
	DateRangeHelpers.GetEarliestReportDateString ( strType );
	m_DateSelectorSingle.SetComboSelection( 0, strType );
	
	return TRUE;  
}

/**********************************************************************/

void CStockDeleteUnusedDlg::OnSelectDate() 
{
	m_DateSelectorSingle.UpdateDateControl();
}

/**********************************************************************/

void CStockDeleteUnusedDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		COleDateTime date;
		m_DatePickerTo.GetTime ( date );
		
		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetSelectedEntity( LocSelEntity );
		
		CWordArray arrayReadWrite;
		arrayReadWrite.Add( NODE_PLU );
		arrayReadWrite.Add( NODE_STOCK );

		CDataManagerInfo info;
		if ( DataManager.OpenDatabaseReadWrite( LocSelEntity.GetDbIdx(), arrayReadWrite, info, TRUE, TRUE ) == FALSE )
			Prompter.ShareError( info );
		else
		{
			int nResult = DeleteStockInternal( LocSelEntity.GetDbIdx(), date );
			DataManager.CloseDatabase();

			switch( nResult )
			{
			case 1:
				Prompter.Error ( "No unused Stock records were found." );
				break;

			case 2:
				Prompter.Info ( "Stock records were deleted as requested." );
				break;
			}
		}
	}
}

/**********************************************************************/

int CStockDeleteUnusedDlg::DeleteStockInternal( int nDbIdx, COleDateTime& date )
{
	DataManager.CustomFieldManager.Clear();

	CByteArray FlagArray;
	CSalesHistoryReportStock Report;
	Report.GetSoldItemsList( nDbIdx, date, FlagArray );

	CStockDeleteDatabaseDlg dlgDelete ( FlagArray, ( m_bKeepOnOrder != 0 ), ( m_bKeepApparent != 0 ), ( m_bKeepSales != 0 ), this );
			
	if ( dlgDelete.GetFilterSize() == 0 )
		return 1;
			
	CDWordArray dw;
	dw.Add(dlgDelete.GetFilterSize());
	dw.Add(DataManager.Stock.GetSize() );
	MessageLogger.LogSystem( 4, 3, TRUE, dw );

	dlgDelete.DoModal();

	if ( dlgDelete.GetDoneDeleteFlag() == TRUE )
		return 2;
	else
		return 0;
}

/**********************************************************************/

void CStockDeleteUnusedDlg::OnChangeDate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorSingle.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/