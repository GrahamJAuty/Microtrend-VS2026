/**********************************************************************/
#include "CCSVReportLine.h"
#include "CommsBatchDlg.h"
#include "DefEditDlg.h"
#include "DownloadDateDlg.h"
#include "NewRecordNumberDlg.h"
#include "NetworkCSVArray.h"
#include "Password.h"
#include "PriceHelpers.h"
#include "PropertySheetDepartment.h"
#include "RepSpawn.h"
#include "SelectMultipleLocationDlg.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ListDataDepartmentDlg.h"
/**********************************************************************/

CSingleListContentHelpersDepartment::CSingleListContentHelpersDepartment( int& nGroupListMode ) : m_nGroupListMode( nGroupListMode )
{
}

/**********************************************************************/

int CSingleListContentHelpersDepartment::GetArraySize()
{
	return DataManager.Department.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersDepartment::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CDepartmentCSVRecord DeptRecord;
		DataManager.Department.GetAt ( nIndex, DeptRecord );
	
		CString strGroup = "";
		if ( NODE_DEPT_GROUP_REPORT == m_nGroupListMode )
			strGroup = DataManager.ReportGroup.GetGroupDisplayText( DeptRecord.GetReportGroup() );
		else
			strGroup = DataManager.EposGroup.GetGroupDisplayText( DeptRecord.GetEposGroup() );

		strLine.Format ( "%d\t%s\t%s\t%s",
			DeptRecord.GetDeptNo(),
			DeptRecord.GetEposText(),
			DeptRecord.GetRepText(),
			(const char*) strGroup );

		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersDepartment::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < DataManager.Department.GetSize() ) )
	{
		CDepartmentCSVRecord DeptRecord;
		DataManager.Department.GetAt( nIndex, DeptRecord );
		DataManager.ListKeyImageWrapperDept.FlagDeletedDept( DeptRecord.GetDeptNo() );
		DataManager.Department.RemoveAt( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersDepartment::FilterSelectedItemArray( CReportConsolidationArray<CSortedIntItem>& arraySelected )
{
	m_bPreventDeletion = FALSE;

	for ( int n = 0; n < arraySelected.GetSize(); n++ )
	{
		CSortedIntItem item;
		arraySelected.GetAt( n, item );

		CDepartmentCSVRecord dept;
		DataManager.Department.GetAt( item.m_nItem, dept );

		if ( dept.GetDeptNo() == 0 )
		{
			arraySelected.RemoveAt( n );
			m_bPreventDeletion = TRUE;
			break;
		}
	}

	m_bPreventDeletion &= ( arraySelected.GetSize() == 0 );
}

/**********************************************************************/

void CSingleListContentHelpersDepartment::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "No", LVCFMT_LEFT, 50 );
	list.InsertColumn ( 1, "EPOS Text", LVCFMT_LEFT, 140 );
	list.InsertColumn ( 2, "Report Text", LVCFMT_LEFT, 140 );
	list.InsertColumn ( 3, "EPOS Group", LVCFMT_LEFT, 150 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataDepartmentDlg::CListDataDepartmentDlg( CWnd* pParent) : CListDataDlg ( pParent )
{
	m_nGroupListMode = NODE_DEPT_GROUP_EPOS;

	m_pContentHelpers = new CSingleListContentHelpersDepartment( m_nGroupListMode );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "Department" );
	m_pDisplayHelpers -> SetTopPluralText( "Departments" );
	m_pDisplayHelpers -> SetBottomSingleText( "department" );
	m_pDisplayHelpers -> SetBottomPluralText( "departments" );

	DataManager.ListKeyImageWrapperDept.Reset();

	m_bSearchPrevious = FALSE;
	m_strSearchText = "";
	m_strSearchPrompt = "Find Department by Text";
	m_nSearchColumn = 1;
}

/**********************************************************************/

CListDataDepartmentDlg::~CListDataDepartmentDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataDepartmentDlg::SetTitle()
{
	CString strTitle = "Department Database ";
	strTitle += dbDatabase.GetDatabaseTitle( DataManager.GetActiveDbIdx(), EcrmanOptions.GetGlobalDepartmentFlag() ); 
	SetWindowText ( strTitle );
}

/**********************************************************************/

void CListDataDepartmentDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Add" );
	m_button2.SetWindowText ( "&Edit" );
	m_button3.SetWindowText ( "&Delete" );
	m_button4.SetWindowText ( "&Print" );
	m_button5.SetWindowText ( "Do&wnload" );
	m_button6.ShowWindow ( SW_HIDE );

	if ( PasswordArray.GetEnable( PasswordTicks::CommsDownloads ) == FALSE )
	{
		m_button5.EnableWindow( FALSE );
		m_button5.ShowWindow( SW_HIDE );
	}

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 47 );
	MoveControl( &m_button3, 368, 87 );
	MoveControl( &m_button4, 368, 147 );
	MoveControl( &m_button5, 368, 187 );
}

/**********************************************************************/

void CListDataDepartmentDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataDepartmentDlg::HandleButton1()
{
	bool bAdd = FALSE;
	int nDeptIdx, nDeptNo;
		
	for ( nDeptNo = 1; nDeptNo <= Dept::DeptNo.Max; nDeptNo++ )
	{
		if ( DataManager.Department.FindDeptByNumber ( nDeptNo, nDeptIdx ) == FALSE )
		{
			bAdd = TRUE;
			break;
		}
	}

	if ( bAdd == FALSE )
	{
		Prompter.Error ( "All available department numbers are already in use." );
		return;
	}

	CNewRecordNumberDlg dlg ( NODE_DEPARTMENT, this );
	dlg.m_nNumber = nDeptNo;
	if ( dlg.DoModal() != IDOK )
		return;

	nDeptNo = dlg.m_nNumber;
	if ( DataManager.Department.FindDeptByNumber ( nDeptNo, nDeptIdx ) )
	{
		Prompter.Error ( "The selected department number is already in use." );
		return;
	}

	CDepartmentCSVRecord Department;
	Department.SetDeptNo( nDeptNo );
	Department.SetEposText ( Department.GetDefaultText() );
	Department.SetRepText( "" );
	Department.SetEposGroup( 0 );
	Department.SetReportGroup( 0 );
	DataManager.Department.InsertAt ( nDeptIdx, Department );

	m_WarnCancelAfterChange.SetFlag();

	CPropertySheetDepartment propSheet( IDC_SPIN, nDeptIdx, m_WarnCancelAfterChange, this );
	propSheet.SetListDataDlg( this );
	
	if ( propSheet.DoModal() == IDOK )
		propSheet.SaveRecord();
	else
		propSheet.UndoChanges();

	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( propSheet.GetIndex() );
}

/**********************************************************************/

void CListDataDepartmentDlg::HandleButton2() 
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 )
		return;

	if ( IsDepartmentZero( nIndex ) == TRUE )
		return;

	CPropertySheetDepartment propSheet( IDC_SPIN, nIndex, m_WarnCancelAfterChange, this );
	propSheet.SetListDataDlg( this );
	
	if ( propSheet.DoModal() == IDOK )
		propSheet.SaveRecord();
	else
		propSheet.UndoChanges();

	SelectLine ( propSheet.GetIndex() );
}

/**********************************************************************/

void CListDataDepartmentDlg::HandleButton3()
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );

	if ( m_pContentHelpers -> GetPreventDeletionFlag() == TRUE )
	{
		DepartmentZeroMessage();
	}
}

/**********************************************************************/

void CListDataDepartmentDlg::HandleButton4() 
{
	CReportFile ReportFile;
	if (ReportFile.Open(Super.ReportFilename()) == FALSE)
	{
		return;
	}

	ReportFile.SetStyle1 ( "Department Texts" );
		
	ReportFile.AddColumn ( "No", TA_LEFT, 120 );
	ReportFile.AddColumn ( "EPOS Text", TA_LEFT, 500 );
	ReportFile.AddColumn ( "Report Text", TA_LEFT, 500 );
	ReportFile.AddColumn ( "EPOS Group", TA_LEFT, 500 );
	ReportFile.AddColumn ( "Report Group", TA_LEFT, 500 );
	ReportFile.AddColumn ( "Image Filename", TA_LEFT, 500 );
	ReportFile.AddColumn ( "Tickets Mode", TA_LEFT, 400 );
	ReportFile.AddColumn ( "Tickets", TA_RIGHT, 180 );
	ReportFile.AddColumn ( "DP Qty", TA_RIGHT, 180 );	
	ReportFile.AddColumn ( "Neg Val", TA_RIGHT, 180 );
	ReportFile.AddColumn ( "Non Add", TA_RIGHT, 180 );	

	if (SysInfo.IsIDraughtExportSystem() == TRUE)
	{
		ReportFile.AddColumn("IDraught", TA_RIGHT, 180);
	}

	ReportFile.AddColumn ( "Learning Plu", TA_LEFT, 400 );
	ReportFile.AddColumn ( "Plu Description", TA_LEFT, 600 );

	if (SysInfo.GetLoyaltyOrSmartPayType() == LOYALTY_TYPE_LOY_V4)
	{
		ReportFile.AddColumn("Bonus Trip", TA_RIGHT, 300);
		ReportFile.AddColumn("Bonus Type", TA_LEFT, 300);
		ReportFile.AddColumn("Bonus Award", TA_RIGHT, 300);
		ReportFile.AddColumn("Spend Bonus", TA_RIGHT, 300);
	}
			
	for ( int nDeptIdx = 0; nDeptIdx < DataManager.Department.GetSize(); nDeptIdx++ )
	{
		CDepartmentCSVRecord DepartmentRecord;
		DataManager.Department.GetAt ( nDeptIdx, DepartmentRecord );

		CString strEposGroup = DataManager.EposGroup.GetGroupDisplayText( DepartmentRecord.GetEposGroup() );
		CString strReportGroup = DataManager.ReportGroup.GetGroupDisplayText( DepartmentRecord.GetReportGroup() );

		CCSVReportLine csvOut;
		csvOut.AppendInt ( DepartmentRecord.GetDeptNo() );
		csvOut.AppendString ( DepartmentRecord.GetEposText() );
		csvOut.AppendString ( DepartmentRecord.GetRepText() );
		csvOut.AppendString ( strEposGroup );
		csvOut.AppendString ( strReportGroup );
		csvOut.AppendString ( DepartmentRecord.GetImageFilename() );

		switch( DepartmentRecord.GetTicketsMode() )
		{
		case 0:		csvOut.AppendString( "No" );					break;
		case 1:		csvOut.AppendString( "At finalisation" );		break;
		case 2:		csvOut.AppendString( "At lay-away" );			break;
		default:	csvOut.AppendString( "Unknown" );				break;
		}

		csvOut.AppendInt ( DepartmentRecord.GetTickets() );
		csvOut.AppendYesOrBlank ( DepartmentRecord.GetDPQtyFlag() );
		csvOut.AppendYesOrBlank ( DepartmentRecord.GetNegativeFlag() );
		csvOut.AppendYesOrBlank ( DepartmentRecord.GetNonAddFlag() );
	
		if (SysInfo.IsIDraughtExportSystem() == TRUE)
		{
			csvOut.AppendYesOrBlank(DepartmentRecord.GetIDraughtExportFlag());
		}

		CSQLPluNoInfo infoPluNo;
		infoPluNo.m_nEposPluNo = DepartmentRecord.GetSpecimenLearnPlu();
		::ProcessPluNo( infoPluNo );

		if (infoPluNo.m_nBasePluNo != 0)
		{
			CString strPluNo;
			::FormatPluNo(infoPluNo.m_nBasePluNo, strPluNo);

			if (infoPluNo.m_nModifier != 0)
			{
				CString strExtra;
				strExtra.Format("+%.d", infoPluNo.m_nModifier);
				strPluNo += strExtra;
			}

			int nPluIdx;
			CString strDescription = "Unknown";
			if (DataManager.Plu.FindPluByNumber(infoPluNo.m_nBasePluNo, nPluIdx) == TRUE)
			{
				CPluCSVRecord PluRecord;
				DataManager.Plu.GetAt(nPluIdx, PluRecord);
				strDescription = PluRecord.GetReportText();
			}

			csvOut.AppendString(strPluNo);
			csvOut.AppendString(strDescription);
		}
		else
		{
			csvOut.AppendBlanks(2);
		}

		if (SysInfo.GetLoyaltyOrSmartPayType() == LOYALTY_TYPE_LOY_V4)
		{
			double dTrip = DepartmentRecord.GetLoyaltyBonusTrip();

			if (CPriceHelpers::CompareDoubles(dTrip, 0.0, 3) != 0)
			{
				csvOut.AppendVal(dTrip, 2);

				switch (DepartmentRecord.GetLoyaltyBonusType())
				{
				case 0:
					csvOut.AppendString("Points");
					csvOut.AppendInt(DepartmentRecord.GetLoyaltyBonusPoints());
					break;

				case 1:
					csvOut.AppendString("Value");
					csvOut.AppendVal(DepartmentRecord.GetLoyaltyBonusValue(), 2);
					break;

				case 2:
					csvOut.AppendString("Percent");
					csvOut.AppendPercent(DepartmentRecord.GetLoyaltyBonusPercent(), 2);
					break;

				default:
					csvOut.AppendBlanks(2);
					break;
				}
			}
			else
			{
				csvOut.AppendBlanks(3);
			}

			csvOut.AppendYesOrBlank(DepartmentRecord.GetLoyaltyGeneralSpendBonus());
		}
	
		ReportFile.WriteLine ( csvOut.GetLine() );
	}
	ReportFile.Close();

	CReportManager ReportManager;
	CFilenameUpdater FnUp ( SysFiles::DepartmentPrm );
	ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
}
		
/**********************************************************************/
	
void CListDataDepartmentDlg::HandleButton5() 
{
	int nDbIdx;
	if ( EcrmanOptions.GetGlobalDepartmentFlag() )
		nDbIdx = -1;
	else
		nDbIdx = DataManager.GetActiveDbIdx();

	bool bBlocked = FALSE;
	CArray<int,int> arrayLocIdx;
	dbNetwork.GetDatabaseCommsLocArray( nDbIdx, arrayLocIdx, bBlocked );

	if ( arrayLocIdx.GetSize() == 0 )
	{
		if ( FALSE == bBlocked )
			Prompter.Error ( "There are no locations available for\ndownload on this database." );
		else
			Prompter.Error ( "You are not authorised to download files\nfrom this database." );

		return;
	}

	HandleDownload( nDbIdx, arrayLocIdx );
}

/**********************************************************************/

void CListDataDepartmentDlg::HandleClickHeader( int nColumn )
{
	if (m_nSearchColumn == nColumn)
	{
		if ((nColumn >= 0) && (GetArraySize() > 1))
		{
			if (m_pSearchDlg == NULL)
			{
				m_pSearchDlg = new CListDataSearchDlg(&m_pSearchDlg, m_strSearchPrompt, this);
				m_pSearchDlg->Create(IDD_DEF_EDIT, this);
			}

			m_pSearchDlg->PostMessage(WM_SETFOCUS);
			m_pSearchDlg->LoadSettings(m_bSearchPrevious, m_strSearchText);
		}
	}
	else if ( 3 == nColumn )
	{
		LVCOLUMN lvCol;
		::ZeroMemory((void *)&lvCol, sizeof(LVCOLUMN));
		lvCol.mask=LVCF_TEXT;
		m_listData.GetColumn(3,&lvCol);

		CString strColumnText = "";
		if ( NODE_DEPT_GROUP_EPOS == m_nGroupListMode )
		{
			m_nGroupListMode = NODE_DEPT_GROUP_REPORT;
            lvCol.pszText = const_cast<char*>("Report Group");
		}
		else
		{
			m_nGroupListMode = NODE_DEPT_GROUP_EPOS;
			lvCol.pszText = const_cast<char*>("EPOS Group");
		}

		m_listData.SetColumn(3,&lvCol);
		m_listData.Invalidate();
	}
}

/**********************************************************************/

bool CListDataDepartmentDlg::IsDepartmentZero( int nIndex )
{
	CDepartmentCSVRecord dept;
	DataManager.Department.GetAt( nIndex, dept );

	if ( dept.GetDeptNo() == 0 )
	{
		DepartmentZeroMessage();
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

void CListDataDepartmentDlg::DepartmentZeroMessage()
{
	Prompter.Error( "Department zero is for internal EPOS use only and cannot be edited or deleted." );
}

/**********************************************************************/

void CListDataDepartmentDlg::HandleDownload( int nDbIdx, CArray<int,int>& arrayLocIdx )
{
	if ( CSelectMultipleLocationDlg::FilterLocListForDownload( arrayLocIdx, "Download Departments", this ) == TRUE )
	{
		CEposTaskDatabase TaskDatabase;
		TaskDatabase.CreateSimpleTask( DN_DEPT, arrayLocIdx );
		CString strDate = "";
		
		{
			CEposTaskHeader TaskHeader;
			TaskDatabase.GetEditableTaskHeader( TaskHeader );

			CString strTitle;
			TaskHeader.SetName( "Download Departments" );
			strTitle.Format ( "Epos Import Date (%s)", TaskHeader.GetName() );

			CDownloadDateDlg dlgDate( strTitle, this );
			if ( dlgDate.RunIfNeeded() == FALSE )
				return;

			if ( dlgDate.m_bImmediate == TRUE )
				TaskHeader.SetImportType( IMPORT_IMMEDIATE );
			else
			{
				TaskHeader.SetImportType( IMPORT_SPECIFIED );
				strDate = dlgDate.m_strDate;
			}

			TaskDatabase.SetEditableTaskHeader( TaskHeader );
		}

		CKeyboardMap KeyboardMap;
		CCommsBatchDlg dlgComms ( TaskDatabase, strDate, TRUE, KeyboardMap, -1, 0, 0, this );
		dlgComms.DoModal();
		m_bWarnCancelAfterDownload = TRUE;
	}
}

/**********************************************************************/

bool CListDataDepartmentDlg::SearchText(int nIndex, CString strSearchText)
{
	bool bResult = FALSE;

	CDepartmentCSVRecord Dept{};
	DataManager.Department.GetAt(nIndex, Dept);

	{
		CString strText = Dept.GetEposText();
		strText.MakeUpper();

		if (strText.Find(strSearchText) != -1)
		{
			SelectLine(nIndex);
			bResult = TRUE;
		}
	}

	if (FALSE == bResult)
	{
		CString strText = Dept.GetReportText();
		strText.MakeUpper();

		if (strText.Find(strSearchText) != -1)
		{
			SelectLine(nIndex);
			bResult = TRUE;
		}
	}

	return bResult;
}

/**********************************************************************/
