/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "DateRangeHelpers.h"
#include "MessageLogger.h"
#include "NodeTypes.h"
#include "Password.h"
#include "PluDeleteDatabaseDlg.h"
#include "SalesHistoryReportPlu.h"
/**********************************************************************/
#include "PluDeleteUnusedDlg.h"
/**********************************************************************/

CPluDeleteUnusedDlg::CPluDeleteUnusedDlg( CWnd* pParent )
	: CDialog(CPluDeleteUnusedDlg::IDD, pParent ),
	m_DateSelectorSingle( m_comboDates, m_DatePickerTo )
{
	//{{AFX_DATA_INIT(CPluDeleteUnusedDlg)
	//}}AFX_DATA_INIT
	m_strDate = "";
	m_nAction = 0;
}

/**********************************************************************/

void CPluDeleteUnusedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluDeleteUnusedDlg)
	DDX_Control(pDX, IDC_COMBO_DATABASE, m_comboDatabase);
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	DDX_Radio(pDX, IDC_RADIO_ACTION, m_nAction);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluDeleteUnusedDlg, CDialog)
	//{{AFX_MSG_MAP(CPluDeleteUnusedDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DATES, OnSelectDate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPluDeleteUnusedDlg::OnInitDialog() 
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

void CPluDeleteUnusedDlg::OnSelectDate() 
{
	m_DateSelectorSingle.UpdateDateControl();
}

/**********************************************************************/

void CPluDeleteUnusedDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		COleDateTime date;
		m_DatePickerTo.GetTime ( date );
		
		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetSelectedEntity( LocSelEntity );

		CWaitCursor wait;

		CWordArray arrayReadWrite;
		arrayReadWrite.Add( NODE_PLU );

		CDataManagerInfo info;
		if ( DataManager.OpenDatabaseReadWrite( LocSelEntity.GetDbIdx(), arrayReadWrite, info, FALSE ) == FALSE )
			Prompter.ShareError( info );
		else
		{
			int nResult = DeletePluInternal( LocSelEntity.GetDbIdx(), date );
			DataManager.CloseDatabase();

			switch( nResult )
			{
			case 1:
				Prompter.Error ( "No matching items were found." );
				break;

			case 2:
				{
					if ( 1 == m_nAction )
						Prompter.Info( "Selected items were deleted as requested" );
					else
						Prompter.Info( "Selected items were marked as inactive" );
				}
				break;
			}
		}
	}
}

/**********************************************************************/

int CPluDeleteUnusedDlg::DeletePluInternal( int nDbIdx, COleDateTime& date )
{
	CByteArray FlagArray;
	CSalesHistoryReportPlu Report;
	Report.GetSoldItemsList( nDbIdx, date, FlagArray );

	CPluDeleteDatabaseDlg dlgDelete ( ( 1 == m_nAction ), FlagArray, this );

	if ( dlgDelete.GetFilterSize() == 0 )
		return 1;
	
	CDWordArray dw;
	dw.Add( dlgDelete.GetFilterSize() );
	dw.Add( DataManager.Plu.GetSize() );
	MessageLogger.LogSystem( 5, 2, TRUE, dw );
		
	dlgDelete.DoModal();

	if ( dlgDelete.GetDoneDeleteFlag() == TRUE )
		return 2;
	else
		return 0;

	return 9;
}

/**********************************************************************/

void CPluDeleteUnusedDlg::OnChangeDate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorSingle.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/
