/**********************************************************************/
#include "..\CommonEcrStockTray\SysInfo.h"
/**********************************************************************/
#include "CommsBatchDlg.h"
#include "DefEditWalletStampRuleDlg.h"
#include "NewRecordNumberDlg.h"
#include "DownloadDateDlg.h"
#include "NetworkCSVArray.h"
#include "Password.h"
#include "PromoDownloadDlg.h"
#include "ReportDataTableWalletStampRuleDlg.h"
#include "SelectMultipleLocationDlg.h"
/**********************************************************************/
#include "ListDataWalletStampRuleDlg.h"
/**********************************************************************/

CSingleListContentHelpersWalletStampRule::CSingleListContentHelpersWalletStampRule()
{
}

/**********************************************************************/

int CSingleListContentHelpersWalletStampRule::GetArraySize()
{
	return DataManager.WalletStampRule.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersWalletStampRule::PrepareLine(int nIndex, CString& strLine)
{
	strLine = "";

	bool bResult = FALSE;
	if ((nIndex >= 0) && (nIndex < GetArraySize()))
	{
		CWalletStampRuleCSVRecord Rule;
		DataManager.WalletStampRule.GetAt(nIndex, Rule);

		int nExpiry = Rule.GetExpiry();

		CCSV csv('\t');
		csv.Add(Rule.GetRuleID());
		csv.Add(Rule.GetDisplayName());
		csv.Add(Rule.GetEnableFlag() ? "Yes" : "No");
		csv.Add(Rule.GetThreshold());

		if (0 == nExpiry)
		{
			csv.Add("----");
		}
		else
		{
			csv.Add(nExpiry);
		}

		strLine = csv.GetLine();

		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersWalletStampRule::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < DataManager.WalletStampRule.GetSize() ) )
	{
		DataManager.WalletStampRule.RemoveAt( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersWalletStampRule::SetColumns(CSSListCtrlVirtual& list, CWnd* pParent)
{
	list.SubclassDlgItem(IDC_LIST, pParent);
	list.InsertColumn(0, "ID", LVCFMT_LEFT, 50);
	list.InsertColumn(1, "Name", LVCFMT_LEFT, 330);
	list.InsertColumn(2, "Enable", LVCFMT_LEFT, 60);
	list.InsertColumn(3, "Threshold", LVCFMT_RIGHT, 80);
	list.InsertColumn(4, "Expiry", LVCFMT_RIGHT, 80);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataWalletStampRuleDlg::CListDataWalletStampRuleDlg( CWnd* pParent) : CListDataDlg ( pParent )
{
	m_pContentHelpers = new CSingleListContentHelpersWalletStampRule();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	switch (SysInfo.GetLoyaltyOrSmartPayType())
	{
	case LOYALTY_TYPE_SMP_V4:
		m_pDisplayHelpers->SetTopSingleText("Purchase Control");
		m_pDisplayHelpers->SetTopPluralText("Purchase Controls");
		m_pDisplayHelpers->SetBottomSingleText("purchase control");
		m_pDisplayHelpers->SetBottomPluralText("purchase controls");
		m_strSearchPrompt = "Find Purchase Control by Name";
		break;

	case LOYALTY_TYPE_LOY_V4:
	default:
		m_pDisplayHelpers->SetTopSingleText("Stamp Offer");
		m_pDisplayHelpers->SetTopPluralText("Stamp Offers");
		m_pDisplayHelpers->SetBottomSingleText("stamp offer");
		m_pDisplayHelpers->SetBottomPluralText("stamp offers");
		m_strSearchPrompt = "Find Stamp Offer by Name";
		break;
	}

	m_bSearchPrevious = FALSE;
	m_strSearchText = "";
	m_nSearchColumn = 1;

	for (int n = 0; n < DataManager.WalletStampRule.GetSize(); n++)
	{
		CWalletStampRuleCSVRecord Rule;
		DataManager.WalletStampRule.GetAt(n, Rule);

		CSortedIntByInt item;
		item.m_nKey = Rule.GetRuleID();
		item.m_nVal = Rule.GetEdition();
		m_arrayOriginalOfferEdition.Consolidate(item);
	}
}

/**********************************************************************/

CListDataWalletStampRuleDlg::~CListDataWalletStampRuleDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataWalletStampRuleDlg::SetTitle()
{
	CString strTitle = "";
	
	switch (SysInfo.GetLoyaltyOrSmartPayType())
	{
	case LOYALTY_TYPE_SMP_V4:
		strTitle = "Purchase Controls ";
		break;

	case LOYALTY_TYPE_LOY_V4:
		strTitle = "Stamp Offers ";
		break;
	}
	
	if (DealerFlags.GetGlobalPromotionFlag() == TRUE)
	{
		strTitle += dbDatabase.GetDatabaseTitle(DataManager.GetActiveDbIdx(), TRUE);
	}
	else if ( EcrmanOptions.GetFeaturesGlobalWalletStampRuleFlag() == FALSE )
	{
		strTitle += dbDatabase.GetDatabaseTitle(DataManager.GetActiveDbIdx(), FALSE);
	}
	else
	{
		CString strExtra = "";
		strExtra.Format("(Global Offer Definitions, %s Plu items)",
			dbDatabase.GetName(DataManager.GetActiveDbIdx()));

		strTitle += strExtra;
	}

	SetWindowText ( strTitle );
}

/**********************************************************************/

void CListDataWalletStampRuleDlg::SetButtonTexts()
{
	m_button1.ShowWindow(SW_HIDE);
	m_button2.SetWindowText ( "&Edit" );
	m_button3.ShowWindow(SW_HIDE);
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

void CListDataWalletStampRuleDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );

	ResizeDialog(500, 348);
	ResizeControl(&m_listData, 420, 334);
	MoveControl(&m_button1, 443, 7);
	MoveControl(&m_button2, 443, 42);
	MoveControl(&m_button3, 443, 77);
	MoveControl(&m_button4, 443, 137);
	MoveControl(&m_button5, 443, 172);
	MoveControl(&m_button6, 443, 207);
	MoveControl(&m_buttonOK, 443, 311);
	MoveControl(&m_buttonCancel, 443, 327);
}

/**********************************************************************/

void CListDataWalletStampRuleDlg::HandleButton1()
{
	/*
	int nRuleID = DataManager.WalletStampRule.FindFirstFreeRuleID();

	if ( nRuleID < 1 )
	{
		Prompter.Error ( "All available stamp offers are already in use." );
		return;
	}

	CNewRecordNumberDlg dlg ( NODE_WALLETSTAMPRULE, this );
	dlg.m_nNumber = nRuleID;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	int nRuleIdx = 0;
	nRuleID = dlg.m_nNumber;
	if ( DataManager.WalletStampRule.FindRuleByID ( nRuleID, nRuleIdx ) )
	{
		Prompter.Error ( "The selected stamp offer is already in use." );
		return;
	}

	CWalletStampRuleCSVRecord Rule;
	Rule.SetRuleID( nOfferID );
	Rule.SetDescription ( "" );
	DataManager.WalletStampRule.InsertAt ( nRuleIdx, Rule );

	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( nRuleIdx );

	CDefEditWalletStampRuleDlg dlgEdit(nRuleIdx, m_arrayOriginalOfferEdition, TRUE, m_WarnCancelAfterChange, this);
	dlgEdit.SetListDataDlg(this);
	dlgEdit.DoModal();
	SelectLine(dlgEdit.GetIndex());
	*/
}

/**********************************************************************/

void CListDataWalletStampRuleDlg::HandleButton2()
{
	int nOfferIdx = GetSafeSelection();
	if (nOfferIdx == -1)
	{
		return;
	}

	if ((nOfferIdx >= 0) && (nOfferIdx < DataManager.WalletStampRule.GetSize()))
	{
		CDefEditWalletStampRuleDlg dlgEdit(nOfferIdx, m_arrayOriginalOfferEdition, TRUE, m_WarnCancelAfterChange, this);
		dlgEdit.SetListDataDlg(this);
		dlgEdit.DoModal();
		SelectLine(dlgEdit.GetIndex());
	}
}

/**********************************************************************/

void CListDataWalletStampRuleDlg::HandleButton3()
{
	/*
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
	*/
}

/**********************************************************************/

void CListDataWalletStampRuleDlg::HandleButton4()
{
	if ( DataManager.WalletStampRule.GetSize() == 0 )
	{
		switch( SysInfo.GetLoyaltyOrSmartPayType() )
		{
		case LOYALTY_TYPE_SMP_V4:
			Prompter.Error ( "You must create some purchase controls\nbefore you can print this report" );
			break;

		case LOYALTY_TYPE_LOY_V4:
		default:
			Prompter.Error ( "You must create some stamp offers\nbefore you can print this report" );
			break;
		}
	}
	else
	{
		CReportDataTableWalletStampRuleDlg dlgReport( this );
		dlgReport.DoModal();
	}
}
		
/**********************************************************************/

void CListDataWalletStampRuleDlg::HandleButton5()
{
	int	nDbIdx = DataManager.GetActiveDbIdx();

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

	HandleDownload( arrayLocIdx );
}

/**********************************************************************/

void CListDataWalletStampRuleDlg::HandleDownload( CArray<int,int>& arrayLocIdx )
{
	CString strType = "Download ";
	if (SysInfo.GetLoyaltyOrSmartPayType() == LOYALTY_TYPE_SMP_V4)
	{
		strType += "Purchase Controls";
	}
	else
	{
		strType += "Stamp Offers";
	}

	if (CSelectMultipleLocationDlg::FilterLocListForDownload(arrayLocIdx, strType, this) == TRUE)
	{
		CEposTaskDatabase TaskDatabase;
		TaskDatabase.CreateSimpleTask(DN_WALLETSTAMPRULE, arrayLocIdx);
		
		CString strDate = "";
		{
			CEposTaskHeader TaskHeader;
			TaskDatabase.GetEditableTaskHeader( TaskHeader );

			TaskHeader.SetName(strType);

			CString strTitle = "";
			strTitle.Format ( "Epos Import Date (%s)", TaskHeader.GetName() );

			CDownloadDateDlg dlgDate( strTitle, this );
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

bool CListDataWalletStampRuleDlg::SearchText(int nIndex, CString strSearchText)
{
	bool bResult = FALSE;
	
	CWalletStampRuleCSVRecord Rule;
	DataManager.WalletStampRule.GetAt(nIndex, Rule);

	CString strText = Rule.GetDisplayName();
	strText.MakeUpper();

	if (strText.Find(strSearchText) != -1)
	{
		SelectLine(nIndex);
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

bool CListDataWalletStampRuleDlg::HandleOK()
{
	return CListDataDlg::HandleOK();
}

/**********************************************************************/
