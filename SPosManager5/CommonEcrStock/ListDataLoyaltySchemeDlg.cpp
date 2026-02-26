/**********************************************************************/
#include "CommsBatchDlg.h"
#include "NewRecordNumberDlg.h"
#include "DownloadDateDlg.h"
#include "EposTaskDatabase.h"
#include "NetworkCSVArray.h"
#include "Password.h"
#include "ReportLoyaltySchemeDlg.h"
#include "PropertySheetLoyaltyScheme.h"
#include "SelectMultipleLocationDlg.h"
/**********************************************************************/
#include "ListDataLoyaltySchemeDlg.h"
/**********************************************************************/

CSingleListContentHelpersLoyaltyScheme::CSingleListContentHelpersLoyaltyScheme() 
{
}

/**********************************************************************/

int CSingleListContentHelpersLoyaltyScheme::GetArraySize()
{
	return DataManager.LoyaltyScheme.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersLoyaltyScheme::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CLoyaltySchemeCSVRecord LoyaltySchemeRecord;
		DataManager.LoyaltyScheme.GetAt ( nIndex, LoyaltySchemeRecord );
	
		strLine.Format ( "%d\t%s",
			LoyaltySchemeRecord.GetSchemeNo(),
			LoyaltySchemeRecord.GetDisplayName() );
		
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersLoyaltyScheme::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < DataManager.LoyaltyScheme.GetSize() ) )
	{
		DataManager.LoyaltyScheme.RemoveAt( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersLoyaltyScheme::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "No", LVCFMT_LEFT, 50 );
	list.InsertColumn ( 1, "Name", LVCFMT_LEFT, 430 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataLoyaltySchemeDlg::CListDataLoyaltySchemeDlg( CWnd* pParent) : CListDataDlg ( pParent )
{
	DataManagerNonDb.CCMacroActive.Reset();
	DataManagerNonDb.CCMacroActive.LoadParentMacrosDatabase(DataManager.GetActiveDbIdx());
	DataManagerNonDb.CCMacroActive.LoadParentMacrosSystem();

	m_pContentHelpers = new CSingleListContentHelpersLoyaltyScheme();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "Loyalty Scheme" );
	m_pDisplayHelpers -> SetTopPluralText( "Loyalty Schemes" );
	m_pDisplayHelpers -> SetBottomSingleText( "Loyalty Scheme" );
	m_pDisplayHelpers -> SetBottomPluralText( "Loyalty Schemes" );
}

/**********************************************************************/

CListDataLoyaltySchemeDlg::~CListDataLoyaltySchemeDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataLoyaltySchemeDlg::SetTitle()
{
	CString strTitle = "Loyalty Schemes ";
	strTitle += dbDatabase.GetDatabaseTitle(DataManager.GetActiveDbIdx(), FALSE);
	SetWindowText(strTitle);
}

/**********************************************************************/

void CListDataLoyaltySchemeDlg::SetButtonTexts()
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

void CListDataLoyaltySchemeDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataLoyaltySchemeDlg::HandleButton1()
{
	int nLoyaltySchemeNo = DataManager.LoyaltyScheme.FindFirstFreeNumber();

	if ( nLoyaltySchemeNo == 0 )
	{
		Prompter.Error ( "All available Loyalty Schemes are already in use." );
		return;
	}

	CNewRecordNumberDlg dlg ( NODE_LOYALTY_SCHEME, this );
	dlg.m_nNumber = nLoyaltySchemeNo;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	int nLoyaltySchemeIdx = 0;
	nLoyaltySchemeNo = dlg.m_nNumber;
	if ( DataManager.LoyaltyScheme.FindSchemeByNumber ( nLoyaltySchemeNo, nLoyaltySchemeIdx ) )
	{
		Prompter.Error ( "The selected Loyalty Scheme is already in use." );
		return;
	}

	CLoyaltySchemeCSVRecord LoyaltyScheme;
	LoyaltyScheme.SetSchemeNo( nLoyaltySchemeNo );
	LoyaltyScheme.SetSchemeName ( LoyaltyScheme.GetDisplayName() );
	DataManager.LoyaltyScheme.InsertAt ( nLoyaltySchemeIdx, LoyaltyScheme );
	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( nLoyaltySchemeIdx );

	CPropertySheetLoyaltyScheme propsheet ( IDC_SPIN, nLoyaltySchemeIdx, m_WarnCancelAfterChange, this );
	propsheet.SetListDataDlg( this );

	if ( propsheet.DoModal() == IDOK )
	{
		propsheet.SaveRecord();
	}

	SelectLine ( propsheet.GetIndex() );
}

/**********************************************************************/

void CListDataLoyaltySchemeDlg::HandleButton2()
{
	int nIndex = GetSafeSelection();
	if (nIndex == -1)
	{
		return;
	}

	CPropertySheetLoyaltyScheme propsheet(IDC_SPIN, nIndex, m_WarnCancelAfterChange, this);
	propsheet.SetListDataDlg(this);

	if (propsheet.DoModal() == IDOK)
	{
		propsheet.SaveRecord();
	}

	SelectLine(propsheet.GetIndex());
}

/**********************************************************************/

void CListDataLoyaltySchemeDlg::HandleButton3()
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

void CListDataLoyaltySchemeDlg::HandleButton4()
{
	if (DataManager.LoyaltyScheme.GetSize() == 0)
	{
		Prompter.Error("You must create some Loyalty Schemes\nbefore you can print this report");
		return;
	}
	else
	{
		CReportLoyaltySchemeDlg dlg(m_reportInfo, this);
		dlg.DoModal();
	}
}
		
/**********************************************************************/

void CListDataLoyaltySchemeDlg::HandleButton5()
{
	bool bBlocked = FALSE;
	CArray<int, int> arrayLocIdx;
	dbNetwork.GetDatabaseCommsLocArray(DataManager.GetActiveDbIdx(), arrayLocIdx, bBlocked);

	if (arrayLocIdx.GetSize() == 0)
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

	HandleDownload(arrayLocIdx);
}

/**********************************************************************/

void CListDataLoyaltySchemeDlg::HandleDownload(CArray<int, int>& arrayLocIdx)
{
	if (CSelectMultipleLocationDlg::FilterLocListForDownload(arrayLocIdx, "Download Loyalty Schemes", this) == TRUE)
	{
		CEposTaskDatabase TaskDatabase;
		TaskDatabase.CreateSimpleTask(DN_LOYALTY_SCHEME, arrayLocIdx);

		CString strDate = "";
		{
			CEposTaskHeader TaskHeader;
			TaskDatabase.GetEditableTaskHeader(TaskHeader);

			CString strTitle;
			TaskHeader.SetName("Download Loyalty Schemes");
			strTitle.Format("Epos Import Date (%s)", TaskHeader.GetName());

			CDownloadDateDlg dlgDate(strTitle, this);
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
				TaskHeader.SetImportType(IMPORT_SPECIFIED);
				strDate = dlgDate.m_strDate;
			}

			TaskDatabase.SetEditableTaskHeader(TaskHeader);
		}

		CKeyboardMap KeyboardMap;
		CCommsBatchDlg dlgComms(TaskDatabase, strDate, TRUE, KeyboardMap, -1, 0, 0, this);
		dlgComms.DoModal();
		m_bWarnCancelAfterDownload = TRUE;
	}
}

/**********************************************************************/
