/**********************************************************************/
#include "LockManager.h"
#include "NodeTypes.h"
#include "MessageLogger.h"
#include "Password.h"
#include "StatusProgress.h"
#include "StockOptionsIni.h"
#include "StockmateLink.h"
#include "StocktakeImportDlg.h"
/**********************************************************************/
#include "StocktakeDlg.h"
/**********************************************************************/

CStocktakeDlg::CStocktakeDlg( CArray<int,int>& arrayEntityIdx, CWnd* pParent)
	: CSSDialog(CStocktakeDlg::IDD, pParent),
	m_arrayEntityIdx( arrayEntityIdx ),
	m_CacheCSV ( '\t' )
{
	//{{AFX_DATA_INIT(CStocktakeDlg)
	//}}AFX_DATA_INIT
	m_nCacheIndex = -1;
	m_nDbIdx = 0;
	m_nSpIdx = 0;
	m_nLastUpdateSpIdx = -1;
	m_nFirstUpdateState = 0;

	CSSDate dateNow;
	dateNow.SetCurrentDate();

	CSSTime timeNow;
	timeNow.SetCurrentTime();

	m_strDate.Format ( "%2.2d%2.2d%4.4d%2.2d%2.2d%2.2d",
		dateNow.GetDay(),
		dateNow.GetMonth(),
		dateNow.GetYear(),
		timeNow.GetHour(),
		timeNow.GetMinute(),
		timeNow.GetSecond() );

	m_bLogDate = TRUE;
}

/**********************************************************************/

void CStocktakeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStocktakeDlg)
	DDX_Control(pDX, IDC_BUTTON_ABANDON, m_buttonAbandon);
	DDX_Control(pDX, IDC_BUTTON_STOCKMATE, m_buttonStockmate);
	DDX_Control(pDX, IDC_BUTTON_ACTION1, m_buttonAction1);
	DDX_Control(pDX, IDC_BUTTON_ACTION2, m_buttonAction2);
	DDX_Control(pDX, IDC_BUTTON_ACTION3, m_buttonAction3);
	DDX_Control(pDX, IDC_BUTTON_ACTION4, m_buttonAction4);
	DDX_Control(pDX, IDC_BUTTON_ACTION5, m_buttonAction5);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStocktakeDlg, CDialog)
	//{{AFX_MSG_MAP(CStocktakeDlg)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_ABANDON, OnButtonAbandon)
	ON_BN_CLICKED(IDC_BUTTON_STOCKMATE, OnButtonStockmate)
	ON_BN_CLICKED(IDC_BUTTON_ACTION1, OnButtonAction1)
	ON_BN_CLICKED(IDC_BUTTON_ACTION2, OnButtonAction2)
	ON_BN_CLICKED(IDC_BUTTON_ACTION3, OnButtonAction3)
	ON_BN_CLICKED(IDC_BUTTON_ACTION4, OnButtonAction4)
	ON_BN_CLICKED(IDC_BUTTON_ACTION5, OnButtonAction5)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_STOCKPOINT, OnSelectStockpoint)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_NOTIFY(LVN_ODCACHEHINT, IDC_LIST, OnListCacheHint)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStocktakeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_listStockpoint.SubclassDlgItem( IDC_LIST_STOCKPOINT, this );
	m_listStockpoint.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listStockpoint.InsertColumn ( 0, "Stockpoint", LVCFMT_LEFT, 220 );
	
	m_listStocktakes.SubclassDlgItem( IDC_LIST, this );
	m_listStocktakes.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listStocktakes.InsertColumn ( 0, "Type", LVCFMT_LEFT, 60 );
	m_listStocktakes.InsertColumn ( 1, "Comment", LVCFMT_LEFT, 200 );
	m_listStocktakes.InsertColumn ( 2, "Date", LVCFMT_LEFT, 135 );
	m_listStocktakes.InsertColumn ( 3, "Items", LVCFMT_RIGHT, 40 );

	ShowAndEnableWindow( &m_buttonAbandon, PasswordArray.GetEnable( PasswordTicks::StocktakingAbandon ) );
	ShowAndEnableWindow( &m_buttonAction1, PasswordArray.GetEnable( PasswordTicks::StocktakingNew ) );
	ShowAndEnableWindow( &m_buttonAction2, PasswordArray.GetEnable( PasswordTicks::StocktakingEdit ) );
	ShowAndEnableWindow( &m_buttonAction4, PasswordArray.GetEnable( PasswordTicks::StocktakingReports ) );
	ShowAndEnableWindow( &m_buttonAction5, PasswordArray.GetEnable( PasswordTicks::StocktakingFinalise ) );

	for ( int n = 0; n < m_arrayEntityIdx.GetSize(); n++ )
	{
		int nSpIdx = m_arrayEntityIdx.GetAt(n);

		int nListPos = m_arrayStockpoint.GetSize();
		m_listStockpoint.InsertItem ( nListPos, "" );
		m_listStockpoint.SetItem ( nListPos, 0, LVIF_TEXT, dbStockpoint.GetName( nSpIdx ), 0, 0, 0, NULL );
	
		CLocationSelectorEntity LocSelEntity;
		LocSelEntity.SetStockpointNode( nSpIdx );
		m_arrayStockpoint.Add( LocSelEntity );
	}

	CLocationSelectorEntity LocSelEntity = m_arrayStockpoint.GetAt(0);
	m_nDbIdx = LocSelEntity.GetDbIdx();
	m_nSpIdx = LocSelEntity.GetEntityIdx();

	if ( m_arrayEntityIdx.GetSize() == 1 )
	{
		CString strTitle;
		GetWindowText( strTitle );

		CString strExtra;
		strExtra.Format( " (%s)", dbStockpoint.GetName( m_arrayEntityIdx.GetAt(0) ) );
		
		SetWindowText( strTitle + strExtra );

		ShowAndEnableWindow( &m_listStockpoint, FALSE );
		MoveControl( &m_listStocktakes, 7, 25 );
		MoveControl( &m_buttonAbandon, 7, 7 );
		MoveControl( &m_buttonStockmate, 271, 7);
		MoveControl( &m_buttonAction1, 330, 25 );
		MoveControl( &m_buttonAction2, 330, 60 );
		MoveControl( &m_buttonAction3, 330, 120 );
		MoveControl( &m_buttonAction4, 330, 155 );
		MoveControl( &m_buttonAction5, 330, 215 );
		MoveControl( &m_buttonCancel, 330, 289 );
		ResizeDialog( 387, 310 );
	}

	if (StockOptions.GetStockmateLinkFlag() == FALSE)
	{
		ShowAndEnableWindow(&m_buttonStockmate, FALSE);
	}

	m_nFirstUpdateState = 1;
	return TRUE;  
}

/**********************************************************************/

void CStocktakeDlg::OnPaint()
{
	CDialog::OnPaint();
	
	if ( 1 == m_nFirstUpdateState  )
	{
		m_nFirstUpdateState = 2;
		SelectStockpointLine(0);
	}
}

/**********************************************************************/

void CStocktakeDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	switch( nIDCtl )
	{
	case IDC_LIST:
		m_listStocktakes.AdjustRowHeight( lpMeasureItemStruct, 10 );
		break;

	case IDC_LIST_STOCKPOINT:
		m_listStockpoint.AdjustRowHeight( lpMeasureItemStruct, 10 );
		break;
	}
}

/**********************************************************************/

void CStocktakeDlg::SelectStockpointLine(int nLineIdx)
{
	m_listStockpoint.Invalidate();

	for (int n = 0; n < m_listStockpoint.GetItemCount(); n++)
	{
		if (n != nLineIdx)
		{
			m_listStockpoint.SetItemState(n, 0, LVIS_SELECTED | LVIS_FOCUSED);
		}
		else
		{
			m_listStockpoint.SetItemState(n, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			m_listStockpoint.SetSelectionMark(n);
		}
	}
}

/**********************************************************************/

void CStocktakeDlg::OnSelectStockpoint(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if ((pNMListView->uChanged & LVIF_STATE) 
        && (pNMListView->uNewState & LVNI_SELECTED))
    {
		int nSel = m_listStockpoint.GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < m_arrayEntityIdx.GetSize() ) )
		{
			CLocationSelectorEntity entity = m_arrayStockpoint.GetAt( nSel );
			m_nDbIdx = entity.GetDbIdx();
			m_nSpIdx = entity.GetEntityIdx();
			UpdateStocktakeList();
		}
    }
}

/**********************************************************************/

void CStocktakeDlg::OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCACHEHINT* pCacheHint = (NMLVCACHEHINT*)pNMHDR;
	m_nCacheIndex = -1;	
	*pResult = 0;
}

/**********************************************************************/

void CStocktakeDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;

		if ( nIndex != m_nCacheIndex  )
		{
			CStocktakeCSVRecord Stocktake;
			m_StocktakeArray.GetAt( nIndex, Stocktake );

			m_CacheCSV.RemoveAll();
			m_CacheCSV.Add ( Stocktake.GetStocktakeTypeText() );
			m_CacheCSV.Add ( Stocktake.GetComment() );
			m_CacheCSV.Add ( Stocktake.GetFormattedDate() );
			m_CacheCSV.Add ( Stocktake.GetItemCount() );
		}
		lstrcpy ( pDispInfo->item.pszText, m_CacheCSV.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

void CStocktakeDlg::UpdateStocktakeList( ) 
{
	m_StocktakeArray.Read( m_nSpIdx ); 
	
	CDataManagerInfo infoDM;
	DatabaseAction( 99, infoDM, TRUE );

	int nSize = m_StocktakeArray.GetSize();
	m_listStocktakes.SetItemCountEx( nSize );
	m_listStocktakes.SetCurSel(0);
	m_listStocktakes.Invalidate();

	m_nLastUpdateSpIdx = m_nSpIdx;
	
	m_nFirstUpdateState = 3;
}

/**********************************************************************/

int CStocktakeDlg::DatabaseAction(int nAction, CDataManagerInfo& infoDM, bool bLock)
{
	int nStocktakeSel = 0;

	switch (nAction)
	{
	case 2:	//EDIT
	case 3:	//LEVELS
	case 5:	//FINALISE
	case 6:	//ABANDON
	case 7:	//CLOSING
	case 8:	//STOCKMATE
	{
		if (m_StocktakeArray.GetSize() == 0)
		{
			Prompter.Error("You must create a stocktake before you can use this option.");
			return 2;
		}

		nStocktakeSel = m_listStocktakes.GetCurSel();
		if ((nStocktakeSel < 0) || (nStocktakeSel >= m_StocktakeArray.GetSize()))
		{
			Prompter.Error("You must select a stocktake from the list\nbefore you can use this option.");
			return 2;
		}
	}
	break;
	}

	DataManager.CloseDatabase();

	if (DataManager.OpenDatabaseReadOnly(m_nDbIdx, infoDM, FALSE, FALSE) == FALSE)
	{
		DataManager.CloseDatabase();
		return 2;
	}

	int nAccessType = bLock ? DATAMANAGER_OPEN_READWRITE : DATAMANAGER_OPEN_READONLY;

	if (DataManager.OpenStockLevels(m_nSpIdx, nAccessType, infoDM) == FALSE)
	{
		DataManager.CloseDatabase();
		return 1;
	}

	if (DataManager.OpenTransferDetail(m_nSpIdx, nAccessType, infoDM) == FALSE)
	{
		DataManager.CloseDatabase();
		return 1;
	}

	if (m_StocktakeArray.GetSize() > 0)
	{
		switch (nAction)
		{
		case 3: //LEVELS
		case 4: //REPORTS
		case 5: //FINALISE
		case 7: //CLOSING
		{
			CString strImportFilename;
			strImportFilename.Format("%s\\auto_closing_plu.dat",
				dbStockpoint.GetFolderPath(m_nSpIdx));

			if (::FileExists(strImportFilename) == TRUE)
			{
				CStocktakeImportDlg dlg(m_nSpIdx, strImportFilename, m_StocktakeArray, this);
				if (dlg.CheckImportFile() == TRUE)
				{
					dlg.DoModal();
				}
			}
		}

		if (StockOptions.GetStockmateLinkFlag() == TRUE)
		{
			CStocktakeCSVRecord Stocktake;
			m_StocktakeArray.GetAt(nStocktakeSel, Stocktake);

			CStockmateLink StockmateLink(m_nSpIdx, Stocktake);
			StockmateLink.ConvertStockmateCountFile();

			if (::FileExists(StockmateLink.GetStockmateClosingStockFilename()) == TRUE)
			{
				CStocktakeImportDlg dlg(m_nSpIdx, StockmateLink.GetStockmateClosingStockFilename(), m_StocktakeArray, this);
				dlg.SetDirectStockCodeFlag(TRUE);

				if (dlg.CheckImportFile() == TRUE)
				{
					dlg.DoModal();
				}
			}
		}
		break;
		}
	}

	switch (nAction)
	{
	case 1:
		HandleNew();
		break;

	case 2:
		HandleEdit();
		break;

	case 3:
		HandleLevels();
		break;

	case 4:
		HandleReports();
		break;

	case 5:
		HandleFinalise();
		break;

	case 6:
		HandleAbandon();
		break;

	case 7:
		HandleClosing();
		break;

	case 8:
		HandleStockmate();
		break;

	case 99:
		RebuildStocktakes();
		break;
	}

	DataManager.CloseDatabase();
	return 0;
}

/**********************************************************************/

void CStocktakeDlg::OnButtonAction1()
{
	if (3 == m_nFirstUpdateState)
	{
		CDataManagerInfo infoDM;
		if (DatabaseAction(1, infoDM, TRUE) == 1)
		{
			Prompter.ShareError(infoDM);
		}
	}
}

/**********************************************************************/

void CStocktakeDlg::OnButtonAction2()
{
	if (3 == m_nFirstUpdateState)
	{
		CDataManagerInfo infoDM;
		if (DatabaseAction(2, infoDM, TRUE) == 1)
		{
			Prompter.ShareError(infoDM);
		}
	}
}

/**********************************************************************/

void CStocktakeDlg::OnButtonAction3()
{
	if (3 == m_nFirstUpdateState)
	{
		CDataManagerInfo infoDM;
		if (DatabaseAction(3, infoDM, TRUE) == 1)
		{
			Prompter.ShareError(infoDM);
		}
	}
}

/**********************************************************************/

void CStocktakeDlg::OnButtonAction4()
{
	if (3 == m_nFirstUpdateState)
	{
		CDataManagerInfo infoDM;
		if (DatabaseAction(4, infoDM, FALSE) == 1)
		{
			Prompter.ShareError(infoDM);
		}
	}
}

/**********************************************************************/

void CStocktakeDlg::OnButtonAction5()
{
	if (3 == m_nFirstUpdateState)
	{
		CDataManagerInfo infoDM;
		if (DatabaseAction(5, infoDM, TRUE) == 1)
		{
			Prompter.ShareError(infoDM);
		}
	}
}

/**********************************************************************/

void CStocktakeDlg::OnButtonAbandon()
{
	if (3 == m_nFirstUpdateState)
	{
		CDataManagerInfo infoDM;
		if (DatabaseAction(6, infoDM, TRUE) == 1)
		{
			Prompter.ShareError(infoDM);
		}
	}
}

/**********************************************************************/

void CStocktakeDlg::OnButtonStockmate()
{
	if (3 == m_nFirstUpdateState)
	{
		CDataManagerInfo infoDM;
		if (DatabaseAction(8, infoDM, FALSE) == 1)
		{
			Prompter.ShareError(infoDM);
		}
	}
}

/**********************************************************************/

void CStocktakeDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (3 == m_nFirstUpdateState)
	{
		CDataManagerInfo infoDM;
		if (DatabaseAction(7, infoDM, TRUE) == 1)
		{
			Prompter.ShareError(infoDM);
		}
	}

	*pResult = 0;
}

/**********************************************************************/

void CStocktakeDlg::RebuildStocktakes()
{
	//CREATE ARRAYS TO COMPARE SAVED AND ACTUAL ITEM COUNTS
	DWORD arrayOldItemCount[MAX_STOCKTAKES + 1];
	DWORD arrayNewItemCount[MAX_STOCKTAKES + 1];

	for (int n = 0; n <= MAX_STOCKTAKES; n++)
	{
		arrayOldItemCount[n] = 0;
		arrayNewItemCount[n] = 0;
	}

	//CREATE AN ARRAY OF 20 RECOVERED STOCKTAKES
	CStocktakeCSVArray tempStocktakes;
	for (int nStocktakeNo = 1; nStocktakeNo <= MAX_STOCKTAKES; nStocktakeNo++)
	{
		CStocktakeCSVRecord Stocktake;
		Stocktake.SetStocktakeNo(nStocktakeNo);
		Stocktake.SetItemCount(0);
		Stocktake.SetStocktakeType(0);
		Stocktake.SetClosingStockZeroFlag(TRUE);
		Stocktake.SetCategory("");
		Stocktake.SetSupplier("");
		Stocktake.SetComment("Recovered Stocktake");
		Stocktake.SetDate(m_strDate);
		tempStocktakes.Add(Stocktake);
	}

	//MERGE EXISTING STOCK TAKE INFO WITH RECOVERED STOCKTAKES
	for (int n = 0; n < m_StocktakeArray.GetSize(); n++)
	{
		CStocktakeCSVRecord Stocktake;
		m_StocktakeArray.GetAt(n, Stocktake);

		int nStocktakeNo = Stocktake.GetStocktakeNo();
		if (nStocktakeNo >= 1 && nStocktakeNo <= MAX_STOCKTAKES)
		{
			tempStocktakes.SetAt(nStocktakeNo - 1, Stocktake);
			arrayOldItemCount[nStocktakeNo] = Stocktake.GetItemCount();
		}
	}

	//REPLACE EXISTING STOCK TAKE INFO WITH THE MERGED ARRAY
	m_StocktakeArray.RemoveAt(0, m_StocktakeArray.GetSize());
	for (int n = 0; n < tempStocktakes.GetSize(); n++)
	{
		CStocktakeCSVRecord Stocktake;
		tempStocktakes.GetAt(n, Stocktake);
		m_StocktakeArray.Add(Stocktake);
	}

	int nSize = DataManager.Stock.GetSize();
	int nSpNo = dbStockpoint.GetSpNo(m_nSpIdx);

	bool bWriteStock = FALSE;

	StatusProgress.Lock(TRUE, "Rebuilding stocktakes");
	for (int nStockIdx = 0; nStockIdx < nSize; nStockIdx++)
	{
		StatusProgress.SetPos(nStockIdx, nSize);

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt(nStockIdx, StockRecord);

		CStockLevelsCSVRecord StockLevels;
		DataManager.Stock.GetStockpointTotals(nStockIdx, StockRecord, nSpNo, StockLevels);

		int nStocktakeNo = StockLevels.GetStocktakeNo();
		if (nStocktakeNo >= 1 && nStocktakeNo <= MAX_STOCKTAKES)
		{
			arrayNewItemCount[nStocktakeNo]++;
		}
		else
		{
			StockLevels.SetStocktakeNo(0);
			DataManager.Stock.SetStockpointTotals(nStockIdx, StockRecord, nSpNo, StockLevels);
			bWriteStock = TRUE;
		}
	}
	StatusProgress.Unlock();

	if (bWriteStock == TRUE)
	{
		CDataManagerInfo info;
		DataManager.WriteStock(info, FALSE, TRUE);
	}

	for (int n = MAX_STOCKTAKES; n >= 1; n--)
	{
		if (arrayOldItemCount[n] != arrayNewItemCount[n])
		{
			CDWordArray dw;
			dw.Add(n);
			dw.Add(arrayOldItemCount[n]);
			dw.Add(arrayNewItemCount[n]);
			MessageLogger.LogSystem(8, 5, m_bLogDate, dw);
			m_bLogDate = FALSE;

			CMessageLoggerInfo info;
			info.m_strMessage.Format("Check stocktake %d %d %d", n, arrayOldItemCount[n], arrayNewItemCount[n]);
			info.m_nHideLevel = 2;
			MessageLogger.LogSchedulerMessage(info, FALSE, TRUE, FALSE);
		}

		if (arrayNewItemCount[n] > 0)
		{
			CStocktakeCSVRecord Stocktake;
			m_StocktakeArray.GetAt(n - 1, Stocktake);
			Stocktake.SetItemCount(arrayNewItemCount[n]);
			m_StocktakeArray.SetAt(n - 1, Stocktake);
		}
		else
		{
			m_StocktakeArray.RemoveAt(n - 1);
		}
	}

	m_StocktakeArray.Write(m_nSpIdx);
	m_listStocktakes.SetItemCountEx(m_StocktakeArray.GetSize());
	m_listStocktakes.Invalidate();
	m_listStocktakes.SetCurSel(0);
}

/**********************************************************************/

void CStocktakeDlg::OnCancel()
{
	if (3 == m_nFirstUpdateState)
	{
		m_StocktakeArray.Write(m_nSpIdx);
	}

	EndDialog(IDOK);
}

/**********************************************************************/

void CStocktakeDlg::OnOK()
{
	if (3 == m_nFirstUpdateState)
	{
		m_StocktakeArray.Write(m_nSpIdx);
	}

	EndDialog(IDOK);
}

/**********************************************************************/

