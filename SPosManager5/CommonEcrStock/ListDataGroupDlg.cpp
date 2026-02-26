/**********************************************************************/
#include "CommsBatchDlg.h"
#include "DefEditGroupDlg.h"
#include "DefEditGroupEposDlg.h"
#include "DefEditGroupReportDlg.h"
#include "DownloadDateDlg.h"
#include "NewRecordNumberDlg.h"
#include "NetworkCSVArray.h"
#include "Password.h"
#include "RepSpawn.h"
#include "SageOptions.h"
#include "SelectMultipleLocationDlg.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ListDataGroupDlg.h"
/**********************************************************************/

CSingleListContentHelpersGroup::CSingleListContentHelpersGroup( int nGroupType, CGroupCSVArrayBase* pGroupArray ) 
{
	m_nGroupType = nGroupType;
	m_pGroupArray = pGroupArray;
}

/**********************************************************************/

int CSingleListContentHelpersGroup::GetArraySize()
{
	return m_pGroupArray -> GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersGroup::PrepareLine( int nIndex, CString& strLine )
{
	return m_pGroupArray -> GetListDataLine( nIndex, strLine );
}

/**********************************************************************/

void CSingleListContentHelpersGroup::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < m_pGroupArray -> GetSize() ) )
	{
		m_pGroupArray -> RemoveAt( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersGroup::FilterSelectedItemArray( CReportConsolidationArray<CSortedIntItem>& arraySelected )
{
	m_bPreventDeletion = FALSE;

	for ( int n = 0; n < arraySelected.GetSize(); n++ )
	{
		CSortedIntItem item;
		arraySelected.GetAt( n, item );

		CBaseGroupInfo GroupInfo;
		m_pGroupArray -> GetGroupInfo ( item.m_nItem, GroupInfo );

		if ( m_pGroupArray ->CanEditGroupZero() == FALSE )
		{
			if ( 0 == GroupInfo.m_nGroupNo )
			{
				arraySelected.RemoveAt( n );
				m_bPreventDeletion = TRUE;
				break;
			}
		}
	}

	m_bPreventDeletion &= ( arraySelected.GetSize() == 0 );
}

/**********************************************************************/

void CSingleListContentHelpersGroup::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );

	switch( m_nGroupType )
	{
	case NODE_DEPT_GROUP_REPORT:
		list.InsertColumn ( 0, "No", LVCFMT_LEFT, 60 );
		list.InsertColumn ( 1, "Report Text", LVCFMT_LEFT, 210 );
		list.InsertColumn ( 2, "Consolidation Group", LVCFMT_LEFT, 210 );
		break;

	case NODE_DEPT_GROUP_CONSOL:
	case NODE_PAYMENT_GROUP:
		list.InsertColumn ( 0, "No", LVCFMT_LEFT, 60 );
		list.InsertColumn ( 1, "Report Text", LVCFMT_LEFT, 420 );
		break;

	case NODE_ALLERGEN:
		list.InsertColumn ( 0, "No", LVCFMT_LEFT, 60 );
		list.InsertColumn ( 1, "Description", LVCFMT_LEFT, 420 );
		break;

	case NODE_DEPT_GROUP_EPOS:
	default:
		list.InsertColumn ( 0, "No", LVCFMT_LEFT, 60 );
		list.InsertColumn ( 1, "EPOS Text", LVCFMT_LEFT, 210 );
		list.InsertColumn ( 2, "Report Text", LVCFMT_LEFT, 210 );
		break;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataGroupDlg::CListDataGroupDlg( int nGroupType, int nGroupNoSel, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_nGroupType = nGroupType;
	m_nGroupNoSel = nGroupNoSel;

	switch( m_nGroupType )
	{
	case NODE_DEPT_GROUP_REPORT:
		m_pGroupArray = &DataManager.ReportGroup;
		break;

	case NODE_DEPT_GROUP_CONSOL:
		m_pGroupArray = &DataManager.ConsolGroup;
		break;

	case NODE_PAYMENT_GROUP:
		m_pGroupArray = &DataManager.PaymentGroup;
		break;

	case NODE_ALLERGEN:
		m_pGroupArray = &DataManagerNonDb.Allergen;
		break;

	case NODE_DEPT_GROUP_EPOS:
	default:
		m_pGroupArray = &DataManager.EposGroup;
		break;
	}

	m_pContentHelpers = new CSingleListContentHelpersGroup( m_nGroupType, m_pGroupArray );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	if ( NODE_ALLERGEN != m_nGroupType )
	{
		m_pDisplayHelpers -> SetTopSingleText( "group" );
		m_pDisplayHelpers -> SetTopPluralText( "groups" );
		m_pDisplayHelpers -> SetBottomSingleText( "group" );
		m_pDisplayHelpers -> SetBottomPluralText( "groups" );

		m_bSearchPrevious = FALSE;
		m_strSearchText = "";
		m_strSearchPrompt = "Find Group by Text";
		m_nSearchColumn = 1;
	}
	else
	{
		m_pDisplayHelpers -> SetTopSingleText( "allergen" );
		m_pDisplayHelpers -> SetTopPluralText( "allergens" );
		m_pDisplayHelpers -> SetBottomSingleText( "allergen" );
		m_pDisplayHelpers -> SetBottomPluralText( "allergens" );
	}
}

/**********************************************************************/

CListDataGroupDlg::~CListDataGroupDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataGroupDlg::SetTitle()
{
	CString strTitle = "";

	bool bGlobal = FALSE;
	switch( m_nGroupType )
	{
	case NODE_DEPT_GROUP_REPORT:
		bGlobal = EcrmanOptions.GetGlobalDeptGroupReportFlag();
		strTitle = "Report Group Database ";
		break;

	case NODE_DEPT_GROUP_CONSOL:
		bGlobal = EcrmanOptions.GetGlobalDeptGroupConsolFlag();
		strTitle = "Consolidation Group Database ";
		break;

	case NODE_PAYMENT_GROUP:
		bGlobal = EcrmanOptions.GetGlobalPaymentGroupFlag();
		strTitle = "Payment Group Database ";
		break;

	case NODE_ALLERGEN:
		bGlobal = TRUE;
		strTitle = "Allergen List ";
		break;

	case NODE_DEPT_GROUP_EPOS:
	default:
		bGlobal = EcrmanOptions.GetGlobalDeptGroupEposFlag();
		strTitle = "EPOS Group Database ";
		break;
	}

	strTitle += dbDatabase.GetDatabaseTitle( DataManager.GetActiveDbIdx(), bGlobal ); 
	SetWindowText ( strTitle );
}

/**********************************************************************/

void CListDataGroupDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Add" );
	m_button2.SetWindowText ( "&Edit" );
	m_button3.SetWindowText ( "&Delete" );
	m_button4.SetWindowText ( "&Print" );
	m_button5.SetWindowText ( "Do&wnload" );
	m_button6.ShowWindow ( SW_HIDE );

	bool bCanDownload = FALSE;

	switch( m_nGroupType )
	{
	case NODE_DEPT_GROUP_EPOS:
	case NODE_ALLERGEN:
		bCanDownload = PasswordArray.GetEnable( PasswordTicks::CommsDownloads );
		break;
	}
	
	if ( FALSE == bCanDownload )
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

void CListDataGroupDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataGroupDlg::PrepareList()
{
	CListDataDlg::PrepareList();
	
	int nSel = 0;
	for ( int n = 0; n < m_pGroupArray -> GetSize(); n++ )
	{
		CBaseGroupInfo GroupInfo;
		m_pGroupArray -> GetGroupInfo( n, GroupInfo );

		if ( GroupInfo.m_nGroupNo == m_nGroupNoSel )
		{
			nSel = n;
			break;
		}
	}

	SelectLine(nSel);
}

/**********************************************************************/

void CListDataGroupDlg::HandleButton1()
{
	int nGroupIdx, nGroupNo;
	bool bAdd = m_pGroupArray -> FindNextFreeGroupNumber( nGroupIdx, nGroupNo ); 
		
	if ( FALSE == bAdd )
	{
		CString strError;
		strError.Format( "All available %s numbers are already in use.", m_pDisplayHelpers -> GetBottomSingleText() );
		Prompter.Error ( strError );
		return;
	}

	CNewRecordNumberDlg dlg ( m_nGroupType, this );
	dlg.m_nNumber = nGroupNo;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	nGroupNo = dlg.m_nNumber;
	if ( m_pGroupArray -> FindGroupByNumber ( nGroupNo, nGroupIdx ) )
	{
		CString strError;
		strError.Format( "The selected %s number is already in use.", m_pDisplayHelpers -> GetBottomSingleText() );
		Prompter.Error ( strError );
		return;
	}

	switch( m_nGroupType )
	{
	case NODE_DEPT_GROUP_REPORT:
		{
			CGroupCSVRecordReport ReportGroup;
			ReportGroup.SetDefaultGroup( nGroupNo );
			m_pGroupArray -> InsertAt( nGroupIdx, ReportGroup );
		}
		break;

	case NODE_DEPT_GROUP_CONSOL:
		{
			CGroupCSVRecordConsol ConsolGroup;
			ConsolGroup.SetDefaultGroup( nGroupNo );
			m_pGroupArray -> InsertAt( nGroupIdx, ConsolGroup );
		}
		break;

	case NODE_PAYMENT_GROUP:
		{
			CGroupCSVRecordPayment PaymentGroup;
			PaymentGroup.SetDefaultGroup( nGroupNo );
			m_pGroupArray -> InsertAt( nGroupIdx, PaymentGroup );
		}
		break;

	case NODE_ALLERGEN:
		{
			CGroupCSVRecordAllergen Allergen;
			Allergen.SetDefaultGroup( nGroupNo );
			m_pGroupArray -> InsertAt( nGroupIdx, Allergen );
		}
		break;

	case NODE_DEPT_GROUP_EPOS:
	default:
		{
			CGroupCSVRecordEpos EposGroup;
			EposGroup.SetDefaultGroup( nGroupNo );
			m_pGroupArray -> InsertAt( nGroupIdx, EposGroup );
		}
		break;
	}

	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( nGroupIdx );

	m_WarnCancelAfterChange.SetFlag();

	switch( m_nGroupType )
	{
	case NODE_DEPT_GROUP_REPORT:
		{
			CDefEditGroupReportDlg dlgEdit ( nGroupIdx, m_WarnCancelAfterChange, this );
			dlgEdit.SetListDataDlg( this );
			dlgEdit.DoModal();
			SelectLine ( dlgEdit.GetIndex() );
		}
		break;

	case NODE_DEPT_GROUP_CONSOL:
	case NODE_PAYMENT_GROUP:
	case NODE_ALLERGEN:
		{
			CDefEditGroupDlg dlgEdit ( m_pGroupArray, m_nGroupType, nGroupIdx, m_WarnCancelAfterChange, this );
			dlgEdit.SetListDataDlg( this );
			dlgEdit.DoModal();
			SelectLine ( dlgEdit.GetIndex() );
		}
		break;

	case NODE_DEPT_GROUP_EPOS:
	default:
		{
			CDefEditGroupEposDlg dlgEdit ( nGroupIdx, m_WarnCancelAfterChange, this );
			dlgEdit.SetListDataDlg( this );
			dlgEdit.DoModal();
			SelectLine ( dlgEdit.GetIndex() );
		}
		break;
	}
}

/**********************************************************************/

void CListDataGroupDlg::HandleButton2() 
{
	int nIndex = GetSafeSelection();

	if (nIndex == -1)
	{
		return;
	}

	if (m_pGroupArray->CanEditGroupZero() == FALSE)
	{
		if (IsGroupZero(nIndex) == TRUE)
		{
			return;
		}
	}

	switch( m_nGroupType )
	{
	case NODE_DEPT_GROUP_REPORT:
		{
			CDefEditGroupReportDlg dlgEdit ( nIndex, m_WarnCancelAfterChange, this );
			dlgEdit.SetListDataDlg( this );
			dlgEdit.DoModal();
			SelectLine ( dlgEdit.GetIndex() );
		}
		break;

	case NODE_DEPT_GROUP_CONSOL:
	case NODE_PAYMENT_GROUP:
	case NODE_ALLERGEN:
		{
			CDefEditGroupDlg dlgEdit ( m_pGroupArray, m_nGroupType, nIndex, m_WarnCancelAfterChange, this );
			dlgEdit.SetListDataDlg( this );
			dlgEdit.DoModal();
			SelectLine ( dlgEdit.GetIndex() );
		}
		break;

	case NODE_DEPT_GROUP_EPOS:
	default:
		{
			CDefEditGroupEposDlg dlgEdit ( nIndex, m_WarnCancelAfterChange, this );
			dlgEdit.SetListDataDlg( this );
			dlgEdit.DoModal();
			SelectLine ( dlgEdit.GetIndex() );
		}
		break;
	}
}

/**********************************************************************/

void CListDataGroupDlg::HandleButton3()
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );

	if ( m_pContentHelpers -> GetPreventDeletionFlag() == TRUE )
	{
		GroupZeroMessage();
	}
}

/**********************************************************************/

void CListDataGroupDlg::HandleButton4() 
{
	switch( m_nGroupType )
	{
	case NODE_DEPT_GROUP_REPORT:
		PrintReportGroupReport();
		break;

	case NODE_DEPT_GROUP_CONSOL:
		PrintConsolGroupReport();
		break;

	case NODE_PAYMENT_GROUP:
		PrintPaymentGroupReport();
		break;

	case NODE_ALLERGEN:
		PrintAllergenReport();
		break;

	case NODE_DEPT_GROUP_EPOS:
	default:
		PrintEposGroupReport();
		break;
	}
}

/**********************************************************************/

void CListDataGroupDlg::PrintEposGroupReport()
{
	CReportFile ReportFile;
	if (ReportFile.Open(Super.ReportFilename()) == FALSE)
	{
		return;
	}
	ReportFile.SetStyle1 ( "EPOS Group Texts" );
	ReportFile.AddColumn ( "No", TA_LEFT, 120 );
	ReportFile.AddColumn ( "EPOS Text", TA_LEFT, 500 );
	ReportFile.AddColumn ( "Rep Text", TA_LEFT, 500 );

	if (SysInfo.IsSageExportSystem() == TRUE)
	{
		ReportFile.AddColumn("Sage Nominal Code", TA_LEFT, 400);
	}

	for ( int nGroupIdx = 0; nGroupIdx < m_pGroupArray -> GetSize(); nGroupIdx++ )
	{
		CGroupCSVRecordEpos GroupRecord;
		m_pGroupArray -> GetAt ( nGroupIdx, GroupRecord );

		CCSV csvOut ( '\t' );
		csvOut.Add ( GroupRecord.GetGroupNo() );
		csvOut.Add ( GroupRecord.GetEposText() );
		csvOut.Add ( GroupRecord.GetRepText() );

		if ( SysInfo.IsSageExportSystem() == TRUE )
		{
			int nCode = GroupRecord.GetSageNominalCode();
			int nMaxLen = SageOptions.GetNominalCodeLength();

			if (0 == nCode)
			{
				csvOut.Add("");
			}
			else
			{
				CString str;
				str.Format( "%*.*d", nMaxLen, nMaxLen, nCode );
				csvOut.Add ( str );
			}
		}

		ReportFile.WriteLine ( csvOut.GetLine() );
	}
	ReportFile.Close();

	CFilenameUpdater FnUp ( SysFiles::EposGroupPrm );
	
	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
}
		
/**********************************************************************/

void CListDataGroupDlg::PrintReportGroupReport()
{
	CReportFile ReportFile;
	if (ReportFile.Open(Super.ReportFilename()) == FALSE)
	{
		return;
	}

	ReportFile.SetStyle1 ( "Report Group Texts" );
	ReportFile.AddColumn ( "No", TA_LEFT, 120 );
	ReportFile.AddColumn ( "Rep Text", TA_LEFT, 500 );
	ReportFile.AddColumn ( "Consolidation Group", TA_LEFT, 500 );

	for ( int nGroupIdx = 0; nGroupIdx < m_pGroupArray -> GetSize(); nGroupIdx++ )
	{
		CGroupCSVRecordReport GroupRecord;
		m_pGroupArray -> GetAt ( nGroupIdx, GroupRecord );

		CCSV csvOut ( '\t' );
		csvOut.Add ( GroupRecord.GetGroupNo() );
		csvOut.Add ( GroupRecord.GetRepText() );
		csvOut.Add ( DataManager.ConsolGroup.GetGroupDisplayText( GroupRecord.GetParentGroupNo() ) );

		ReportFile.WriteLine ( csvOut.GetLine() );
	}
	ReportFile.Close();

	CFilenameUpdater FnUp ( SysFiles::ReportGroupPrm );
	
	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
}
		
/**********************************************************************/

void CListDataGroupDlg::PrintConsolGroupReport()
{
	CReportFile ReportFile;
	if (ReportFile.Open(Super.ReportFilename()) == FALSE)
	{
		return;
	}

	ReportFile.SetStyle1 ( "Consolidation Group Texts" );
	ReportFile.AddColumn ( "No", TA_LEFT, 120 );
	ReportFile.AddColumn ( "Rep Text", TA_LEFT, 500 );

	for ( int nGroupIdx = 0; nGroupIdx < m_pGroupArray -> GetSize(); nGroupIdx++ )
	{
		CGroupCSVRecordConsol GroupRecord;
		m_pGroupArray -> GetAt ( nGroupIdx, GroupRecord );

		CCSV csvOut ( '\t' );
		csvOut.Add ( GroupRecord.GetGroupNo() );
		csvOut.Add ( GroupRecord.GetRepText() );

		ReportFile.WriteLine ( csvOut.GetLine() );
	}
	ReportFile.Close();

	CFilenameUpdater FnUp ( SysFiles::ConsolGroupPrm );
	
	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
}
		
/**********************************************************************/

void CListDataGroupDlg::PrintPaymentGroupReport()
{
	CReportFile ReportFile;
	if (ReportFile.Open(Super.ReportFilename()) == FALSE)
	{
		return;
	}

	ReportFile.SetStyle1 ( "Payment Group Texts" );
	ReportFile.AddColumn ( "No", TA_LEFT, 120 );
	ReportFile.AddColumn ( "Rep Text", TA_LEFT, 500 );

	for ( int nGroupIdx = 0; nGroupIdx < m_pGroupArray -> GetSize(); nGroupIdx++ )
	{
		CGroupCSVRecordPayment GroupRecord;
		m_pGroupArray -> GetAt ( nGroupIdx, GroupRecord );

		CCSV csvOut ( '\t' );
		csvOut.Add ( GroupRecord.GetGroupNo() );
		csvOut.Add ( GroupRecord.GetRepText() );

		ReportFile.WriteLine ( csvOut.GetLine() );
	}
	ReportFile.Close();

	CFilenameUpdater FnUp ( SysFiles::PaymentGroupPrm );
	
	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
}
		
/**********************************************************************/

void CListDataGroupDlg::PrintAllergenReport()
{
	CReportFile ReportFile;
	if (ReportFile.Open(Super.ReportFilename()) == FALSE)
	{
		return;
	}

	ReportFile.SetStyle1 ( "Allergen Texts" );
	ReportFile.AddColumn ( "No", TA_LEFT, 120 );
	ReportFile.AddColumn ( "Rep Text", TA_LEFT, 500 );

	for ( int nGroupIdx = 0; nGroupIdx < m_pGroupArray -> GetSize(); nGroupIdx++ )
	{
		CGroupCSVRecordAllergen GroupRecord;
		m_pGroupArray -> GetAt ( nGroupIdx, GroupRecord );

		CCSV csvOut ( '\t' );
		csvOut.Add ( GroupRecord.GetGroupNo() );
		csvOut.Add ( GroupRecord.GetRepText() );

		ReportFile.WriteLine ( csvOut.GetLine() );
	}
	ReportFile.Close();

	CFilenameUpdater FnUp ( SysFiles::AllergenPrm );
	
	CReportManager ReportManager;
	ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
}
		
/**********************************************************************/

void CListDataGroupDlg::HandleButton5() 
{
	int nDbIdx = -1;
	if ((NODE_DEPT_GROUP_EPOS == m_nGroupType) && (EcrmanOptions.GetGlobalDeptGroupEposFlag() == FALSE))
	{
		nDbIdx = DataManager.GetActiveDbIdx();
	}

	bool bBlocked = FALSE;
	CArray<int,int> arrayLocIdx;
	dbNetwork.GetDatabaseCommsLocArray( nDbIdx, arrayLocIdx, bBlocked );

	if ( arrayLocIdx.GetSize() == 0 )
	{
		if (FALSE == bBlocked)
		{
			Prompter.Error("There are no locations available for\ndownload on this database.");
		}
		else
		{
			Prompter.Error("You are not authorised to download files\nfrom this database.");
		}

		return;
	}

	HandleDownload( nDbIdx, arrayLocIdx );
}

/**********************************************************************/

bool CListDataGroupDlg::IsGroupZero( int nIndex )
{
	CBaseGroupInfo GroupInfo;
	m_pGroupArray -> GetGroupInfo( nIndex, GroupInfo );

	if ( 0 == GroupInfo.m_nGroupNo )
	{
		GroupZeroMessage();
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

void CListDataGroupDlg::GroupZeroMessage()
{
	Prompter.Error( "Group zero is for internal EPOS use only and cannot be edited or deleted." );
}

/**********************************************************************/

void CListDataGroupDlg::HandleDownload( int nDbIdx, CArray<int,int>& arrayLocIdx )
{
	CString strTitle = ( NODE_DEPT_GROUP_EPOS == m_nGroupType ) ? "Download Dept Groups" : "Download Allergens";

	if ( CSelectMultipleLocationDlg::FilterLocListForDownload( arrayLocIdx, strTitle, this ) == TRUE )
	{
		CEposTaskDatabase TaskDatabase;
		TaskDatabase.CreateSimpleTask( ( NODE_DEPT_GROUP_EPOS == m_nGroupType ) ? DN_GROUP : DN_ALLERGEN, arrayLocIdx );
		CString strDate = "";

		{
			CEposTaskHeader TaskHeader;
			TaskDatabase.GetEditableTaskHeader( TaskHeader );

			CString strDlgTitle;
			TaskHeader.SetName( strTitle );
			strDlgTitle.Format ( "Epos Import Date (%s)", TaskHeader.GetName() );

			CDownloadDateDlg dlgDate( strDlgTitle, this );
			if (dlgDate.RunIfNeeded() == FALSE)
			{
				return;
			}

			if (dlgDate.m_bImmediate == TRUE)
			{
				TaskHeader.SetImportType(IMPORT_IMMEDIATE);
			}
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

bool CListDataGroupDlg::SearchText(int nIndex, CString strSearchText)
{
	bool bResult = FALSE;

	CString strEPOSText = "";
	CString strReportText = "";

	switch (m_nGroupType)
	{
	case NODE_DEPT_GROUP_REPORT:
		{
			CGroupCSVRecordReport Group;
			m_pGroupArray->GetAt(nIndex, Group);
			strEPOSText = Group.GetEposText();
			strReportText = Group.GetReportText();
		}
		break;

	case NODE_DEPT_GROUP_CONSOL:
		{
			CGroupCSVRecordConsol Group;
			m_pGroupArray->GetAt(nIndex, Group);
			strEPOSText = Group.GetEposText();
			strReportText = Group.GetReportText();
		}
		break;

	case NODE_PAYMENT_GROUP:
		{
			CGroupCSVRecordPayment Group;
			m_pGroupArray->GetAt(nIndex, Group);
			strEPOSText = Group.GetEposText();
			strReportText = Group.GetReportText();
		}
		break;

	case NODE_DEPT_GROUP_EPOS:
	default:
		{
			CGroupCSVRecordEpos Group;
			m_pGroupArray->GetAt(nIndex, Group);
			strEPOSText = Group.GetEposText();
			strReportText = Group.GetReportText();
		}
		break;
	}

	{
		strEPOSText.MakeUpper();

		if (strEPOSText.Find(strSearchText) != -1)
		{
			SelectLine(nIndex);
			bResult = TRUE;
		}
	}

	if (FALSE == bResult)
	{
		strReportText.MakeUpper();

		if (strReportText.Find(strSearchText) != -1)
		{
			SelectLine(nIndex);
			bResult = TRUE;
		}
	}

	return bResult;
}

/**********************************************************************/

bool CListDataGroupDlg::HandleOK()
{
	m_nGroupNoSel = 0;
	
	int nIndex = GetSafeSelection();
	if ( nIndex != -1 )
	{
		CBaseGroupInfo GroupInfo;
		m_pGroupArray -> GetGroupInfo( nIndex, GroupInfo );
		m_nGroupNoSel = GroupInfo.m_nGroupNo;
	}

	return TRUE;
}

/**********************************************************************/


