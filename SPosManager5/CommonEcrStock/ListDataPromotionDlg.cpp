/**********************************************************************/
#include "CommsBatchDlg.h"
#include "NewRecordNumberDlg.h"
#include "DownloadDateDlg.h"
#include "NetworkCSVArray.h"
#include "Password.h"
#include "PromoDownloadDlg.h"
#include "PropertySheetPromotion.h"
#include "ReportDataTablePromoDlg.h"
#include "SelectMultipleLocationDlg.h"
/**********************************************************************/
#include "ListDataPromotionDlg.h"
/**********************************************************************/

CSingleListContentHelpersPromotion::CSingleListContentHelpersPromotion() 
{
}

/**********************************************************************/

int CSingleListContentHelpersPromotion::GetArraySize()
{
	return DataManager.Promotion.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersPromotion::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CPromotionsCSVRecord Promo;
		DataManager.Promotion.GetAt ( nIndex, Promo );

		CCSV csv( '\t' );
		csv.Add( Promo.GetPromoNo() );
		csv.Add( Promo.GetDisplayName() );
		csv.Add( Promo.GetPromoTypeName() );
		
		switch( Promo.GetUsage() )
		{
		case 1:		
			csv.Add( "Item" );	
			break;

		case 2:		
			csv.Add( "Subtotal" );	
			break;

		case 3:		
			csv.Add( "Item or Subtotal" );	
			break;

		case 6:		
			csv.Add( "Deferred Subtotal" );
			break;

		default:	
			{
				CString str = "";
				str.Format( "Unknown (%d)", Promo.GetUsage() );
				csv.Add( str );				
			}
			break;
		}

		csv.Add(Promo.GetSPOSApplyTaxFlag() ? "Yes" : "No");

		if (Promo.GetFlexType() == 1)
		{
			csv.Add("Flex (Value)");
		}
		else if (Promo.GetFlexType() == 2)
		{
			csv.Add("Flex (Covers)");
		}
		else if ( Promo.GetPromoType() != PROMOTION_TYPE_MARKDOWN )
		{
			CString strPercent = "";
			strPercent.Format( "%.2f%%", Promo.GetPercent() );
			csv.Add( strPercent );
		}
		else
		{
			CString strMoney = "";
			strMoney.Format( "£%.2f", Promo.GetMarkdown() );
			csv.Add( strMoney );
		}
		
		strLine = csv.GetLine();

		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersPromotion::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < DataManager.Promotion.GetSize() ) )
	{
		DataManager.Promotion.RemoveAt( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersPromotion::SetColumns(CSSListCtrlVirtual& list, CWnd* pParent)
{
	list.SubclassDlgItem(IDC_LIST, pParent);
	list.InsertColumn(0, "No", LVCFMT_LEFT, 50);
	list.InsertColumn(1, "Name", LVCFMT_LEFT, 170);
	list.InsertColumn(2, "Type", LVCFMT_LEFT, 110);
	list.InsertColumn(3, "Usage", LVCFMT_LEFT, 110);
	list.InsertColumn(4, "Taxable", LVCFMT_LEFT, 80);
	list.InsertColumn(5, "Amount", LVCFMT_RIGHT, 80);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataPromotionDlg::CListDataPromotionDlg( CWnd* pParent) : CListDataDlg ( pParent )
{
	m_pContentHelpers = new CSingleListContentHelpersPromotion();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "Promotion" );
	m_pDisplayHelpers -> SetTopPluralText( "Promotions" );
	m_pDisplayHelpers -> SetBottomSingleText( "promotion" );
	m_pDisplayHelpers -> SetBottomPluralText( "promotions" );

	m_bSearchPrevious = FALSE;
	m_strSearchText = "";
	m_strSearchPrompt = "Find Promotion by Name";
	m_nSearchColumn = 1;
}

/**********************************************************************/

CListDataPromotionDlg::~CListDataPromotionDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataPromotionDlg::SetTitle()
{
	CString strTitle = "Promotions Database ";
	strTitle += dbDatabase.GetDatabaseTitle( DataManager.GetActiveDbIdx(), DealerFlags.GetGlobalPromotionFlag() ); 
	SetWindowText ( strTitle );
}

/**********************************************************************/

void CListDataPromotionDlg::SetButtonTexts()
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

void CListDataPromotionDlg::SetColumns()
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

void CListDataPromotionDlg::HandleButton1()
{
	int nPromoNo = DataManager.Promotion.FindFirstFreeNumber();

	if ( nPromoNo < 1 )
	{
		Prompter.Error ( "All available promotions are already in use." );
		return;
	}

	CNewRecordNumberDlg dlg ( NODE_PROMOTION, this );
	dlg.m_nNumber = nPromoNo;
	if ( dlg.DoModal() != IDOK )
		return;

	int nPromoIdx;
	nPromoNo = dlg.m_nNumber;
	if ( DataManager.Promotion.FindTableByNumber ( nPromoNo, nPromoIdx ) )
	{
		Prompter.Error ( "The selected promotion is already in use." );
		return;
	}

	CPromotionsCSVRecord Promo;
	Promo.SetPromoNo( nPromoNo );
	Promo.SetDescription ( "" );
	DataManager.Promotion.InsertAt ( nPromoIdx, Promo );

	m_listData.SetItemCountEx ( GetArraySize() );
	SelectLine ( nPromoIdx );

	DataManager.Department.SetDeptZero( "None" );

	CPropertySheetPromotion propSheet( IDC_SPIN, TRUE, nPromoIdx, this );
	
	propSheet.SetListDataDlg( this );
	
	if ( propSheet.DoModal() == IDOK )
	{
		propSheet.SaveRecord();
	}

	DataManager.Department.SetDeptZero( "PROMO" );	
	SelectLine ( propSheet.GetIndex() );
	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataPromotionDlg::HandleButton2() 
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 )
		return;
	
	DataManager.Department.SetDeptZero( "None" );
	
	CPropertySheetPromotion propSheet( IDC_SPIN, TRUE, nIndex, this );
	
	propSheet.SetListDataDlg( this );
	
	if ( propSheet.DoModal() == IDOK )
	{
		propSheet.SaveRecord();
	}
	
	DataManager.Department.SetDeptZero( "PROMO" );
	SelectLine ( propSheet.GetIndex() );
	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataPromotionDlg::HandleButton3()
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

void CListDataPromotionDlg::HandleButton4() 
{
	if ( DataManager.Promotion.GetSize() == 0 )
	{
		Prompter.Error ( "You must create some promotions\nbefore you can print this report" );
	}
	else
	{
		CReportDataTablePromoDlg dlgReport( this );
		dlgReport.DoModal();
	}
}
		
/**********************************************************************/

void CListDataPromotionDlg::HandleButton5() 
{
	int	nDbIdx = DataManager.GetActiveDbIdx();

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

	HandleDownload( arrayLocIdx );
}

/**********************************************************************/

void CListDataPromotionDlg::HandleDownload( CArray<int,int>& arrayLocIdx )
{
	CPromoDownloadDlg dlgDownload( this );
	if ( dlgDownload.DoModal() != IDOK )
		return;

	bool bMixMatch = ( dlgDownload.m_nDownloadType == 2 );
	CString strTitle = (bMixMatch) ? "Download Mix Match" : "Download Promotions";

	if ( CSelectMultipleLocationDlg::FilterLocListForDownload( arrayLocIdx, strTitle, this ) == TRUE )
	{
		CEposTaskDatabase TaskDatabase;
		
		if (FALSE == bMixMatch)
		{
			TaskDatabase.CreateSimpleTask(DN_PROMO, arrayLocIdx);
		}
		else if (EcrmanOptions.GetFeaturesTimePlanDownloadFlag() == FALSE)
		{
			TaskDatabase.CreateSimpleTask(DN_MIX, arrayLocIdx);
		}
		else
		{
			CWordArray arrayJobs;
			arrayJobs.Add( DN_MIX );
			arrayJobs.Add( DN_TPLAN );
			TaskDatabase.CreateSimpleTask( arrayJobs, arrayLocIdx );
		}

		CString strDate = "";
		{
			CEposTaskHeader TaskHeader;
			TaskDatabase.GetEditableTaskHeader( TaskHeader );

			CString strTitle;
			TaskHeader.SetName( bMixMatch ? "Download Mix Match" : "Download Promotions" );
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

bool CListDataPromotionDlg::SearchText(int nIndex, CString strSearchText)
{
	bool bResult = FALSE;

	CPromotionsCSVRecord Promo;
	DataManager.Promotion.GetAt(nIndex, Promo);

	CString strText = Promo.GetDisplayName();
	strText.MakeUpper();

	if (strText.Find(strSearchText) != -1)
	{
		SelectLine(nIndex);
		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

bool CListDataPromotionDlg::HandleOK()
{
	int nNonVATPromos = 0;

	for (int nIndex = 0; nIndex < DataManager.Promotion.GetSize(); nIndex++)
	{
		CPromotionsCSVRecord Promo;
		DataManager.Promotion.GetAt(nIndex, Promo);

		if (Promo.GetPromoType() != PROMOTION_TYPE_SURCHARGE)
		{
			if (Promo.GetSPOSApplyTaxFlag() == FALSE)
			{
				nNonVATPromos++;
			}
		}
	}

	bool bResult = TRUE;

	if (nNonVATPromos != 0)
	{
		CString strPlural1 = (nNonVATPromos > 1) ? "s" : "";
		CString strPlural2 = (nNonVATPromos > 1) ? "these discounts" : "this discount";

		CString strMsg = "";
		strMsg.Format("You have created %d non taxable discount promotion%s.\n\n",
			nNonVATPromos,
			(const char*)strPlural1);

		CString strLine = "";
		strLine.Format("Applying %s will not reduce the calculated %%T amount for reports.\n\n",
			(const char*)strPlural2);
		strLine = EcrmanOptions.GetTaxString(strLine);
		strMsg += strLine;

		strLine = "This may lead to a higher than expected estimate of total %T payable.\n\n";
		strLine = EcrmanOptions.GetTaxString(strLine);
		strMsg += strLine;

		strMsg += "Are you sure you wish to save these settings ?";

		if (Prompter.YesNo(strMsg) != IDYES)
		{
			bResult = FALSE;
		}
	}

	if (TRUE == bResult)
	{
		bResult = CListDataDlg::HandleOK();
	}

	return bResult;
}

/**********************************************************************/
