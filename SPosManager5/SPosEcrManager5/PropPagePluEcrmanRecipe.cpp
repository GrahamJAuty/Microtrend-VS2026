/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "FilterTypes.h"
#include "..\SPosEcrManager5\PluFilterDlgEcrman.h"
#include "..\CommonEcrStock\PluMiniBrowseDlg.h"
#include "..\CommonEcrStock\PriceSetItemManager.h"
#include "ReportHelpers.h"
#include "..\CommonEcrStock\ReportPluCustomList.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPagePluEcrmanRecipe.h"
/**********************************************************************/

CPropPagePluEcrmanRecipe::CPropPagePluEcrmanRecipe() : CSSPropertyPage(CPropPagePluEcrmanRecipe::IDD), 
	m_FilterArray ( FILTERTYPE_BROWSE_PLU ) 
{
	//{{AFX_DATA_INIT(CPropPagePluEcrmanRecipe)
	//}}AFX_DATA_INIT
	m_pPluRecord = NULL;
	m_bReadOnly = FALSE;
}

/**********************************************************************/

CPropPagePluEcrmanRecipe::~CPropPagePluEcrmanRecipe()
{
}

/**********************************************************************/

void CPropPagePluEcrmanRecipe::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePluEcrmanRecipe)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_ADD, m_buttonAdd);
	DDX_Control(pDX, IDC_BUTTON_REMOVE, m_buttonRemove);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_buttonBrowse);
	DDX_Control(pDX, IDC_BUTTON_SCAN, m_buttonScan);
	DDX_Control(pDX, IDC_EDIT_PLU, m_editPluNo);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePluEcrmanRecipe, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePluEcrmanRecipe)
	//}}AFX_MSG_MAP
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_SCAN, OnButtonScan)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU, OnKillFocusPlu)
	ON_MESSAGE (WM_APP, OnAddPluMessage)
	ON_MESSAGE (WM_APP + 1, OnAddScannedPlu)
	ON_MESSAGE (WM_APP + 2, OnSetDefaultFocus)
	ON_MESSAGE(WM_VALIDATE, OnEndLabelEditVariableCriteria)
	ON_MESSAGE(WM_GET_CELLTYPE, GetCellType)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePluEcrmanRecipe::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_editPluNo.LimitText( Super.MaxPluLen() );
	m_editPluNo.SetMessageTarget( this, WM_APP + 1 );

	SetRecordControls();

	m_listRecipeItems.SubclassDlgItem ( IDC_LIST, this );
	m_listRecipeItems.InsertColumn ( 0, "", LVCFMT_LEFT, 30 );
	m_listRecipeItems.InsertColumn ( 1, "Plu No", LVCFMT_LEFT, 130 );
	m_listRecipeItems.InsertColumn ( 2, "Description", LVCFMT_LEFT, 380 );
	m_listRecipeItems.InsertColumn ( 3, "Qty", LVCFMT_LEFT, 200 );
	m_listRecipeItems.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listRecipeItems.EnableVScroll();

	Refresh();
	
	return TRUE;  
}

/**********************************************************************/

void CPropPagePluEcrmanRecipe::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )
   {
	   m_listRecipeItems.AdjustRowHeight( lpMeasureItemStruct, 10 );
   }
}

/**********************************************************************/

BOOL CPropPagePluEcrmanRecipe::PreTranslateMessage(MSG* pMsg) 
{
  if(pMsg->message==WM_KEYDOWN)
  {
	  if(pMsg->wParam==VK_RETURN )
	  {
		  if ( GetFocus() == &m_listRecipeItems )
		  {
			  m_listRecipeItems.HandleReturn(3);	
			  pMsg->wParam=NULL ;
		  }
	  }
  }
  return CDialog::PreTranslateMessage(pMsg);
}

/**********************************************************************/

void CPropPagePluEcrmanRecipe::AddList()
{
	m_arrayCellTypes.RemoveAll();
	m_listRecipeItems.DeleteAllItems();

	for ( int n = 0; n < m_arrayRecipeItems.GetSize(); n++ )
	{
		CEcrmanRecipeItem infoItem;
		m_arrayRecipeItems.GetAt( n, infoItem );

		if ( infoItem.m_dStockQty <= 0.0 )
		{
			m_arrayRecipeItems.RemoveAt(n--);
			continue;
		}

		AddRecipeListLine( -1, infoItem );
	}

	RenumberListLines();
}

/**********************************************************************/

void CPropPagePluEcrmanRecipe::AddRecipeListLine( int nPos, CEcrmanRecipeItem& infoItem )
{
	if ( nPos < 0 )
		nPos = m_listRecipeItems.GetItemCount();

	CString strPluNo;
	::FormatPluNo( infoItem.m_nPluNo, strPluNo );

	int nPluIdx;
	CString strDescription = "Unknown";
	if ( DataManager.Plu.FindPluByNumber( infoItem.m_nPluNo, nPluIdx ) == TRUE )
	{
		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt( nPluIdx, PluRecord );
		strDescription = ReportHelpers.GetReportText( PluRecord );
	}

	CString strStockQty;
	strStockQty.Format( "%.3f", infoItem.m_dStockQty );

	m_listRecipeItems.InsertItem ( nPos, "" );
	m_listRecipeItems.SetItem ( nPos, 0, LVIF_TEXT, "", 0, 0, 0, NULL );
	m_listRecipeItems.SetItem ( nPos, 1, LVIF_TEXT, strPluNo, 0, 0, 0, NULL );
	m_listRecipeItems.SetItem ( nPos, 2, LVIF_TEXT, strDescription, 0, 0, 0, NULL );
	m_listRecipeItems.SetItem ( nPos, 3, LVIF_TEXT, strStockQty, 0, 0, 0, NULL );

	{			
		CellTypeInfo infoEdit;
		infoEdit.m_nType =				CELL_TYPE_EDIT;
		infoEdit.m_nSSEditType =		SS_NUM_DP;
		infoEdit.m_nSSEditWidth =		8;
		infoEdit.m_strSSEditFormat =	"%.3f";
		infoEdit.m_dMinDouble =			0.001;
		infoEdit.m_dMaxDouble =			9999.999;
		infoEdit.m_strCurrent =			strStockQty;
		
		m_arrayCellTypes.InsertAt( nPos, infoEdit );
	}
}

/**********************************************************************/

void CPropPagePluEcrmanRecipe::AddRecipeItem( __int64 nPluNo )
{
	CEcrmanRecipeItem infoItem;
	infoItem.m_nPluNo = nPluNo;
	infoItem.m_dStockQty = 1.0;

	int nItemIdx;
	if ( m_arrayRecipeItems.Find( infoItem, nItemIdx ) == FALSE )
	{
		m_arrayRecipeItems.InsertAt( nItemIdx, infoItem );
		AddRecipeListLine( nItemIdx, infoItem );
		RenumberListLines();
	}

	m_listRecipeItems.SetSelectionMark( nItemIdx );
	m_listRecipeItems.SetItemState( nItemIdx, LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
	m_listRecipeItems.EnsureVisible( nItemIdx, FALSE );
}

/**********************************************************************/

void CPropPagePluEcrmanRecipe::RenumberListLines()
{
	for ( int n = 0; n < m_listRecipeItems.GetItemCount(); n++ )
	{
		CString strNum;
		strNum.Format( "%d", n + 1 );
		m_listRecipeItems.SetItem ( n, 0, LVIF_TEXT, strNum, 0, 0, 0, NULL );
	}
}

/**********************************************************************/

long CPropPagePluEcrmanRecipe::OnSetDefaultFocus( WPARAM wIndex, LPARAM lParam )
{
	m_editPluNo.SetFocus();
	m_editPluNo.SetSel(0,-1);
	return 0L;
}

/**********************************************************************/

BOOL CPropPagePluEcrmanRecipe::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePluEcrmanRecipe::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	PostMessage( WM_APP + 2, 0, 0 );
	return bResult;
}

/**********************************************************************/

void CPropPagePluEcrmanRecipe::Refresh()
{
	m_pPluRecord -> GetEcrmanRecipeTableArray( m_arrayRecipeItems );
	AddList();
}

/**********************************************************************/

bool CPropPagePluEcrmanRecipe::UpdateRecord()
{
	SaveRecord();
	return TRUE;
}

/**********************************************************************/

void CPropPagePluEcrmanRecipe::SetRecordControls()
{
	if ( ( PriceSetItemManager.IsPluMaster() == FALSE ) || ( m_bReadOnly == TRUE ) )
	{
		int nPluStatus = PriceSetItemManager.GetPluStatus();

		if ( ( nPluStatus != LOCALPLU_LOCAL ) || ( m_bReadOnly == TRUE ) )
		{
			m_editPluNo.EnableWindow( FALSE );
			m_buttonBrowse.EnableWindow( FALSE );
			m_buttonAdd.EnableWindow( FALSE );
			m_buttonRemove.EnableWindow( FALSE );
			m_buttonScan.EnableWindow( FALSE );
		}
		else
		{
			bool bTable = PriceSetItemManager.CheckField( Plu::EcrmanRecipeTable.Label );
			m_editPluNo.EnableWindow( bTable );
			m_buttonBrowse.EnableWindow( bTable );
			m_buttonAdd.EnableWindow( bTable );
			m_buttonRemove.EnableWindow( bTable );
			m_buttonScan.EnableWindow( bTable );
		}
	}
}

/**********************************************************************/

void CPropPagePluEcrmanRecipe::SaveRecord()
{
	if ( m_bReadOnly == TRUE )
		return;

	UpdateRecipeItemArray();
	m_pPluRecord -> SetEcrmanRecipeTableArray( m_arrayRecipeItems );
}

/**********************************************************************/

void CPropPagePluEcrmanRecipe::OnButtonAdd() 
{
	CPluMiniBrowseDlg dlgFilter( NODE_PLU, m_FilterArray, this );
	dlgFilter.DoModal();
}

/**********************************************************************/

long CPropPagePluEcrmanRecipe::OnAddPluMessage( WPARAM wIndex, LPARAM lParam )
{
	switch ( wIndex )
	{
	case ADDPLU_MESSAGE:
		{
			CPluMiniBrowseDlg* pDlg = (CPluMiniBrowseDlg*) lParam;
				
			int nPluIdx = pDlg -> GetPluIdx();

			if ( nPluIdx >= 0 && nPluIdx < DataManager.Plu.GetSize() )
			{
				if ( m_arrayRecipeItems.GetSize() >= MAX_RECIPE_PLU_ITEMS )
				{
					CString strMsg;
					strMsg.Format( "You can only add a maximum of\n%d items to a recipe table", MAX_RECIPE_PLU_ITEMS );
					Prompter.Error( strMsg );
				}
				else
				{
					__int64 nPluNo = DataManager.Plu.GetPluNoInt( nPluIdx );
					AddRecipeItem( nPluNo );
				}
			}
		}
		break;
	}
	
	return 0L;
}

/**********************************************************************/

long CPropPagePluEcrmanRecipe::OnAddScannedPlu( WPARAM wIndex, LPARAM lParam )
{
	AddScannedPlu();
	return 0L;
}

/**********************************************************************/

void CPropPagePluEcrmanRecipe::OnButtonScan()
{
	AddScannedPlu();
}

/**********************************************************************/

void CPropPagePluEcrmanRecipe::AddScannedPlu()
{
	CString strPluNo;
	m_editPluNo.GetWindowText( strPluNo );
	strPluNo = BarcodePluNoTable.ConvertPlu( strPluNo );
	m_editPluNo.SetWindowText( strPluNo );

	__int64 nPluNo = _atoi64( strPluNo );

	if ( nPluNo == 0 )
	{
		m_editPluNo.SetWindowText( "" );
		Prompter.Error( "Please enter a Plu number" );
	}
	else if ( m_arrayRecipeItems.GetSize() >= MAX_RECIPE_PLU_ITEMS )
	{
		CString strMsg;
		strMsg.Format( "You can only add a maximum of\n%d items to a recipe table", MAX_RECIPE_PLU_ITEMS );
		Prompter.Error( strMsg );
	}
	else
	{
		int nPluIdx;
		if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == TRUE )
		{	
			AddRecipeItem( nPluNo );
		}
		else
		{
			CString strError;
			strError.Format( "Plu number %s does not exist in your database.", (const char*) strPluNo );
			Prompter.Error( strError );
		}
	}

	m_editPluNo.SetSel( 0, -1 );
	m_editPluNo.SetFocus();
}

/**********************************************************************/

void CPropPagePluEcrmanRecipe::OnButtonRemove() 
{
	int nSel = m_listRecipeItems.GetSelectionMark();
	if ( ( nSel >= 0 ) && ( nSel < m_arrayRecipeItems.GetSize() ) )
	{
		m_arrayRecipeItems.RemoveAt( nSel );
		m_arrayCellTypes.RemoveAt( nSel );
		m_listRecipeItems.DeleteItem( nSel );
		RenumberListLines();

		if ( nSel < m_arrayRecipeItems.GetSize() )
		{
			m_listRecipeItems.SetSelectionMark( nSel );
			m_listRecipeItems.SetItemState( nSel, LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
			m_listRecipeItems.EnsureVisible( nSel, FALSE );
		}
		else if ( nSel > 0 )
		{
			m_listRecipeItems.SetSelectionMark( nSel - 1 );
			m_listRecipeItems.SetItemState( nSel - 1, LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
			m_listRecipeItems.EnsureVisible( nSel - 1, FALSE );
		}
	}
}

/**********************************************************************/

void CPropPagePluEcrmanRecipe::OnButtonBrowse() 
{
	UpdateData( TRUE );

	int nDlgResult, nPluIdx;
	::ShowPluFilterDlg( m_FilterArray, this, nDlgResult, nPluIdx );

	if ( IDOK == nDlgResult )
	{
		if ( nPluIdx >= 0 && nPluIdx < DataManager.Plu.GetSize() )
			m_editPluNo.SetWindowText ( DataManager.Plu.GetPluNoString ( nPluIdx ) );
	}
}

/**********************************************************************/

void CPropPagePluEcrmanRecipe::OnKillFocusPlu() 
{
	CString strPluNo;
	m_editPluNo.GetWindowText( strPluNo );
	strPluNo = BarcodePluNoTable.ConvertPlu( strPluNo );
	m_editPluNo.SetWindowText( strPluNo );
}

/**********************************************************************/

LRESULT CPropPagePluEcrmanRecipe::OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam) 
{
	m_listRecipeItems.AllowListScroll( TRUE );
	m_listRecipeItems.SetEditCell( -1, -1 );

	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)lParam;
	
	if (wParam == IDC_LIST)
	{
		// Update the item text with the new text
		CString strText = pDispInfo->item.pszText;  
	
		int nRow = pDispInfo -> item.iItem;
		int nCol = pDispInfo -> item.iSubItem;

		if ( ( nRow >= 0 ) && ( nRow < m_arrayCellTypes.GetSize() ) )
		{
			if ( 3 == nCol )
			{
				CellTypeInfo infoCell = m_arrayCellTypes[ nRow ];

				double d = atof( strText );

				if ( ( d >= infoCell.m_dMinDouble ) && ( d <= infoCell.m_dMaxDouble ) )
					m_arrayCellTypes[nRow].m_strCurrent = strText;	
				else
					strText = m_arrayCellTypes[nRow].m_strCurrent;

				m_listRecipeItems.SetItem ( nRow, 3, LVIF_TEXT, strText, 0, 0, 0, NULL );
			}
		}
	}

	return 1;
}

/**********************************************************************/

LRESULT CPropPagePluEcrmanRecipe::GetCellType(WPARAM wParam, LPARAM lParam)
{
	CellTypeInfo* pCellType = reinterpret_cast<CellTypeInfo*>(lParam);
	
	int nCol = pCellType -> m_nCol;
	int nRow = pCellType -> m_nRow;

	if ( nCol != 3 )
	{
		pCellType -> m_nType = 0;
		return 1;
	}

	if ( ( nRow < 0 ) || ( nRow >= m_arrayCellTypes.GetSize() ) )
	{
		pCellType -> m_nType = 0;
		return 1;
	}
	
	if ( ( m_bReadOnly == TRUE ) || ( PriceSetItemManager.IsPluMaster() == FALSE ) )
	{
		pCellType -> m_nType = 0;
		return 1;
	}

	CellTypeInfo infoCell = m_arrayCellTypes[nRow];

	pCellType -> m_nType =				infoCell.m_nType;
	pCellType -> m_nSSEditType =		infoCell.m_nSSEditType;
	pCellType -> m_strSSEditFormat =	infoCell.m_strSSEditFormat;
	pCellType -> m_nSSEditWidth =		infoCell.m_nSSEditWidth;
	pCellType -> m_nComboSelect =		infoCell.m_nComboSelect;
	
	return 1;
}

/**********************************************************************/

void CPropPagePluEcrmanRecipe::UpdateRecipeItemArray()
{
	if ( m_arrayCellTypes.GetSize() == m_arrayRecipeItems.GetSize() )
	{
		for ( int n = 0; n < m_arrayRecipeItems.GetSize(); n++ )
		{
			CEcrmanRecipeItem infoItem;
			m_arrayRecipeItems.GetAt( n, infoItem );
			infoItem.m_dStockQty = atof( m_arrayCellTypes[n].m_strCurrent );
			m_arrayRecipeItems.SetAt( n, infoItem );
		}
	}
}

/**********************************************************************/

BOOL CPropPagePluEcrmanRecipe::OnApply()
{
	CWnd* pWnd = GetFocus();

	if ( pWnd == &m_buttonScan )
	{
		AddScannedPlu();
		return FALSE;
	}
	else
	{
		OnOK();
		return TRUE;
	}
}

/**********************************************************************/

void CPropPagePluEcrmanRecipe::OnOK()
{
	CPropertyPage::OnOK();
}

/**********************************************************************/

void CPropPagePluEcrmanRecipe::OnCancel()
{
	CPropertyPage::OnCancel();
}

/**********************************************************************/

