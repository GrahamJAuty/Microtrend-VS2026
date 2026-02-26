/**********************************************************************/
#include "..\SPosEcrManager5\SPOSOfferGroupPluEditEcrman.h"
/**********************************************************************/
#include "..\SPosStockManager5\SPOSOfferGroupPluEditStockman.h"
/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "FilterTypes.h"
#include "ModifierSelectDlg.h"
#include "OfferGroupEntriesCSVArray.h"
#include "PluMiniBrowseDlg.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPageOfferGroupEntryPlu.h"
/**********************************************************************/

CPropPageOfferGroupEntryPlu::CPropPageOfferGroupEntryPlu() : CSSPropertyPage(CPropPageOfferGroupEntryPlu::IDD),
	m_FilterArray ( FILTERTYPE_BROWSE_PLU )
{
	//{{AFX_DATA_INIT(CPropPageOfferGroupEntryPlu)
	//}}AFX_DATA_INIT
	m_bDialogReady = FALSE;
	m_nEntryType = OFFERENTRY_PLU;
	m_nNodeType = NODE_OFFER_GROUP;
	m_bPluFilterMode = FALSE;
	m_bPluFilterSystem = FALSE;
	m_bWalletStampRule = FALSE;
	m_pOfferGroupRecord = NULL;
}

/**********************************************************************/

CPropPageOfferGroupEntryPlu::~CPropPageOfferGroupEntryPlu()
{
}

/**********************************************************************/

void CPropPageOfferGroupEntryPlu::SetPluFilterMode( bool bSystem )
{
	m_bPluFilterMode = TRUE;
	m_bPluFilterSystem = bSystem;
}

/**********************************************************************/

void CPropPageOfferGroupEntryPlu::SetEntryType( int nEntryType )
{
	switch( nEntryType )
	{
	case OFFERENTRY_PLU:
	case OFFERENTRY_PLU_EXCLUDE:
		m_nEntryType = nEntryType;
	}
}

/**********************************************************************/

void CPropPageOfferGroupEntryPlu::SetNodeType( int nNodeType )
{
	switch( nNodeType )
	{
	case NODE_OFFER_GROUP:
	case NODE_PLUFILTER_NAME:
		m_nNodeType = nNodeType;
	}
}

/**********************************************************************/

COfferGroupEntriesCSVArray* CPropPageOfferGroupEntryPlu::GetEntryArray()
{
	switch( m_nNodeType )
	{
	case NODE_PLUFILTER_NAME:
		return m_bPluFilterSystem ? &DataManagerNonDb.SystemPluFilterItems : &DataManager.DbPluFilterItems;

	case NODE_OFFER_GROUP:
	default:
		return &DataManager.OfferGroupEntry;
	}
}

/**********************************************************************/

void CPropPageOfferGroupEntryPlu::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageOfferGroupEntryPlu)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_EDIT_NAME, m_editName );
	DDX_Control(pDX, IDC_BUTTON_MODIFIER, m_buttonModifier);
	DDX_Control(pDX, IDC_BUTTON_SCAN, m_buttonScan);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_buttonBrowse);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_buttonAdd);
	DDX_Control(pDX, IDC_EDIT_PLU, m_editPluNo);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageOfferGroupEntryPlu, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageOfferGroupEntryPlu)
	//}}AFX_MSG_MAP
	ON_WM_MEASUREITEM()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	ON_BN_CLICKED(IDC_BUTTON_MODIFIER, OnButtonModifier)
	ON_BN_CLICKED(IDC_BUTTON_EDITPLU, OnButtonEditPlu)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_SCAN, OnButtonScan)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU, OnKillFocusPlu)
	ON_MESSAGE (WM_APP, OnAddPluMessage)
	ON_MESSAGE (WM_APP + 1, OnAddScannedPlu)
	ON_MESSAGE (WM_APP + 2, OnSetDefaultFocus)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageOfferGroupEntryPlu::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	if ( TRUE == m_bPluFilterSystem )
	{
		ShowAndEnableWindow( &m_buttonBrowse, FALSE );
		ShowAndEnableWindow( &m_buttonAdd, FALSE );
	}

	m_editPluNo.LimitText( Super.MaxPluLen() );
	m_editPluNo.SetMessageTarget( this, WM_APP + 1 );

	if ( SysInfo.GetMaxBasePluLen() == 0 )
	{
		ShowAndEnableWindow( &m_buttonModifier, FALSE );
	}

	m_editName.LimitText( OfferGroups::GroupName.Max );

	m_listPlu.SubclassDlgItem ( IDC_LIST, this );
	
	if ( FALSE == m_bPluFilterSystem )
	{
		if ( (TRUE == m_bWalletStampRule) && ( SysInfo.GetLoyaltyOrSmartPayType() == LOYALTY_TYPE_SMP_V4 ) )
		{
			m_listPlu.InsertColumn(0, "Plu No", LVCFMT_LEFT, 110);
			m_listPlu.InsertColumn(1, "Description", LVCFMT_LEFT, 180);
			m_listPlu.InsertColumn(2, "Root", LVCFMT_LEFT, 50);
			m_listPlu.InsertColumn(3, "Modifiers", LVCFMT_LEFT, 280);
			m_listPlu.InsertColumn(4, "Weight", LVCFMT_RIGHT, 60 );
		}
		else
		{
			m_listPlu.InsertColumn(0, "Plu No", LVCFMT_LEFT, 110);
			m_listPlu.InsertColumn(1, "Description", LVCFMT_LEFT, 200);
			m_listPlu.InsertColumn(2, "Root", LVCFMT_LEFT, 50);
			m_listPlu.InsertColumn(3, "Modifiers", LVCFMT_LEFT, 320);
		}
	}
	else
	{
		m_listPlu.InsertColumn ( 0, "Plu No", LVCFMT_LEFT, 160 );
		m_listPlu.InsertColumn ( 2, "Root", LVCFMT_LEFT, 70);
		m_listPlu.InsertColumn ( 3, "Modifiers", LVCFMT_LEFT, 450 );
	}

	m_listPlu.ModifyStyle( LVS_OWNERDRAWFIXED, 0, 0 );
	m_listPlu.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	
	m_bDialogReady = TRUE;
	SetRecordControls();

	return TRUE;  
}

/**********************************************************************/

void CPropPageOfferGroupEntryPlu::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )
   {
	   m_listPlu.AdjustRowHeight( lpMeasureItemStruct, 10 );
   }
}

/**********************************************************************/

long CPropPageOfferGroupEntryPlu::OnSetDefaultFocus( WPARAM wIndex, LPARAM lParam )
{
	m_editPluNo.SetFocus();
	m_editPluNo.SetSel(0,-1);
	return 0L;
}

/**********************************************************************/

BOOL CPropPageOfferGroupEntryPlu::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageOfferGroupEntryPlu::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	PostMessage( WM_APP + 2, 0, 0 );
	return bResult;
}

/**********************************************************************/

void CPropPageOfferGroupEntryPlu::Refresh()
{
	BuildPluArray();
	SetRecordControls();
}

/**********************************************************************/

void CPropPageOfferGroupEntryPlu::SetRecordControls()
{
	if ( TRUE == m_bDialogReady )
	{
		m_editName.SetWindowText( m_pOfferGroupRecord -> GetGroupName() );
		m_listPlu.SetItemCountEx( m_arrayPluNo.GetSize() );
		m_listPlu.Invalidate();
	}
}

/**********************************************************************/

void CPropPageOfferGroupEntryPlu::SaveRecord()
{
	CString strName;
	m_editName.GetWindowText( strName );
	m_pOfferGroupRecord -> SetGroupName( strName );
}

/**********************************************************************/

void CPropPageOfferGroupEntryPlu::BuildPluArray()
{
	m_arrayPluNo.RemoveAll();

	int nIdx1, nIdx2;
	GetEntryArray() -> GetGroupSubRange( m_pOfferGroupRecord -> GetGroupNo(), m_nEntryType, nIdx1, nIdx2 );

	for ( int n = nIdx1; n <= nIdx2; n++ )
	{
		COfferGroupEntriesCSVRecord entry;
		GetEntryArray() -> GetAt( n, entry );

		COfferGroupItem item;
		item.m_strPluNo.Format("%I64d", entry.GetEntryPluNo());
		item.m_nModifiers = entry.GetModifierFlags();
		m_arrayPluNo.Consolidate( item );
	}
}

/**********************************************************************/

void CPropPageOfferGroupEntryPlu::UpdateSelectedItemArray()
{
	m_arraySelected.RemoveAll();

	POSITION pos = m_listPlu.GetFirstSelectedItemPosition();
	
	while (pos)
	{
		CSortedIntItem indexItem;
		indexItem.m_nItem = m_listPlu.GetNextSelectedItem(pos);
		m_arraySelected.Consolidate( indexItem );
	}
}

/**********************************************************************/

void CPropPageOfferGroupEntryPlu::GetSelectedItem( int nIdx, COfferGroupItem& item )
{
	item.Reset();

	if ( ( nIdx >= 0 ) && ( nIdx < m_arraySelected.GetSize() ) )
	{
		CSortedIntItem indexItem;
		m_arraySelected.GetAt( nIdx, indexItem );

		if ((indexItem.m_nItem >= 0) && (indexItem.m_nItem < m_arrayPluNo.GetSize()))
		{
			m_arrayPluNo.GetAt(indexItem.m_nItem, item);
		}
	}
}

/**********************************************************************/

void CPropPageOfferGroupEntryPlu::SetSelectedItem( int nIdx, COfferGroupItem& item )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arraySelected.GetSize() ) )
	{
		CSortedIntItem indexItem;
		m_arraySelected.GetAt( nIdx, indexItem );

		if ((indexItem.m_nItem >= 0) && (indexItem.m_nItem < m_arrayPluNo.GetSize()))
		{
			m_arrayPluNo.SetAt(indexItem.m_nItem, item);
		}
	}
}

/**********************************************************************/

void CPropPageOfferGroupEntryPlu::GetPluDeletionList( CStringArray& arrayText )
{
	arrayText.RemoveAll();

	int nSize = m_arraySelected.GetSize();
	for ( int n = 0; ( n < 3 ) && ( n < nSize ); n++ )
	{
		COfferGroupItem arrayItem;
		GetSelectedItem( n, arrayItem );

		CString strDescription = "";

		if ( FALSE == m_bPluFilterSystem )
		{
			int nPluIdx;
			strDescription = "Unknown";
			if ( DataManager.Plu.FindPluByNumber( _atoi64(arrayItem.m_strPluNo), nPluIdx ) == TRUE )
			{
				CPluCSVRecord Plu;
				DataManager.Plu.GetAt( nPluIdx, Plu );
				strDescription = Plu.GetEposText();
			}
		}

		CString strLine = "";
		strLine += arrayItem.m_strPluNo;
		strLine += " ";
		strLine += strDescription;
		arrayText.Add( strLine );
	}

	if ( nSize > 3 )
	{
		CString strLine;
		CString strItems = ( 4 == nSize ) ? "item" : "items";
		strLine.Format( "<%d more %s>", nSize - 3, (const char*) strItems );
		arrayText.Add( "" );
		arrayText.Add( strLine );
	}
}

/**********************************************************************/

void CPropPageOfferGroupEntryPlu::SelectSingleItem( int nIndex )
{
	for ( int n = 0; n < m_listPlu.GetItemCount(); n++ )
	{
		if ( n != nIndex )
			m_listPlu.SetItemState(n, 0, LVIS_SELECTED);
		else
		{
			m_listPlu.SetItemState(n, LVIS_SELECTED, LVIS_SELECTED);
			m_listPlu.SetSelectionMark(n);
		}
	}
}

/**********************************************************************/

void CPropPageOfferGroupEntryPlu::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ( ( pDispInfo-> item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;
		
		if ( ( nIndex >= 0 ) && ( nIndex < m_arrayPluNo.GetSize() ) )
		{
			COfferGroupItem arrayItem;
			m_arrayPluNo.GetAt( nIndex, arrayItem );

			CString strDescription = "";
			int nWeight = 1;
			
			if ( FALSE == m_bPluFilterSystem )
			{
				int nPluIdx = 0;
				strDescription = "Unknown";
			
				if ( DataManager.Plu.FindPluByNumber( _atoi64(arrayItem.m_strPluNo), nPluIdx ) == TRUE )
				{
					CPluCSVRecord Plu;
					DataManager.Plu.GetAt( nPluIdx, Plu );
					strDescription = Plu.GetEposText();
					nWeight = Plu.GetStampWeight();
				}
			}

			int nMask = 2;
			CCSV csvModifiers;
			for ( int n = 1; n <= 9; n++ )
			{
				if ( ( arrayItem.m_nModifiers & nMask ) != 0 )
				{
					if ( FALSE == m_bPluFilterSystem )
						csvModifiers.Add( DataManager.Modifier.GetDisplayName( n ) );
					else
					{
						CString strModifier;
						strModifier.Format( "Modifier %d", n );
						csvModifiers.Add( strModifier );
					}
				}

				nMask *= 2;
			}
			
			CCSV csv( '\t' );
			csv.Add( arrayItem.m_strPluNo );

			if ( FALSE == m_bPluFilterSystem )
			{
				csv.Add( strDescription );
			}
			
			csv.Add( ( ( arrayItem.m_nModifiers & 1 ) == 1 ) ? "Yes" : "No" );
			csv.Add( csvModifiers.GetLine() );

			if ((TRUE == m_bWalletStampRule) && (SysInfo.GetLoyaltyOrSmartPayType() == LOYALTY_TYPE_SMP_V4))
			{
				csv.Add(nWeight);
			}

			lstrcpy ( pDispInfo->item.pszText, csv.GetString ( pDispInfo->item.iSubItem ) );
		}
	}
	*pResult = 0;
}

/**********************************************************************/

void CPropPageOfferGroupEntryPlu::OnButtonAdd() 
{
	CPluMiniBrowseDlg dlgFilter( m_nNodeType, m_FilterArray, this );
	dlgFilter.DoModal();
}

/**********************************************************************/

long CPropPageOfferGroupEntryPlu::OnAddPluMessage( WPARAM wIndex, LPARAM lParam )
{ 
	switch ( wIndex )
	{
	case ADDPLU_MESSAGE:
		{
			CPluMiniBrowseDlg* pDlg = (CPluMiniBrowseDlg*) lParam;
				
			int nPluIdx = pDlg -> GetPluIdx();

			if ( nPluIdx >= 0 && nPluIdx < DataManager.Plu.GetSize() )
			{
				CString strPluNo = DataManager.Plu.GetPluNoString( nPluIdx );
			
				COfferGroupItem item;
				item.m_strPluNo = strPluNo;
				item.m_nModifiers = 1;
					
				int nIdx;
				if ( m_arrayPluNo.Find( item, nIdx ) == FALSE )
				{
					m_arrayPluNo.Consolidate( item );
					m_listPlu.SetItemCountEx( m_arrayPluNo.GetSize() );
					SelectSingleItem( nIdx );
					m_listPlu.Invalidate();
				}
			}
		}
		break;
	}

	return 0L;
}

/**********************************************************************/

long CPropPageOfferGroupEntryPlu::OnAddScannedPlu( WPARAM wIndex, LPARAM lParam )
{
	AddScannedPlu();
	return 0L;
}

/**********************************************************************/

void CPropPageOfferGroupEntryPlu::OnButtonScan()
{
	AddScannedPlu();
}

/**********************************************************************/

void CPropPageOfferGroupEntryPlu::AddScannedPlu()
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
	else
	{
		int nPluIdx;
		if ( ( TRUE == m_bPluFilterSystem ) || ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == TRUE ) )
		{		
			COfferGroupItem item;
			item.m_strPluNo = strPluNo;
			item.m_nModifiers = 1;

			int nIdx;
			if ( m_arrayPluNo.Find( item, nIdx ) == FALSE )
			{
				m_arrayPluNo.Consolidate( item );
				m_listPlu.SetItemCountEx( m_arrayPluNo.GetSize() );
				SelectSingleItem( nIdx );
				m_listPlu.Invalidate();
			}
			else
			{
				CString strError;
				CString strEntity = ( NODE_PLUFILTER_NAME == m_nNodeType ) ? "Plu filter" : "offer group";
				strError.Format( "Plu number %s is already in this %s.", (const char*) strPluNo, (const char*) strEntity );
				Prompter.Error( strError );
			}
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

void CPropPageOfferGroupEntryPlu::OnButtonRemove() 
{
	UpdateSelectedItemArray();

	int nSize = m_arraySelected.GetSize();
	if ( nSize > 0 )
	{
		CStringArray arrayText;
		GetPluDeletionList( arrayText );

		CString strMsg = "";
		CString strThis = ( 1 == nSize ) ? "this" : "these";
		CString strItem = ( 1 == nSize ) ? "item" : "items";
		CString strEntity = ( NODE_PLUFILTER_NAME == m_nNodeType ) ? "Plu filter" : "offer group";
		
		strMsg.Format( "Are you sure you wish to remove %s\nPlu %s from this %s.\n",
			(const char*) strThis,
			(const char*) strItem,
			(const char*) strEntity );

		for ( int n = 0; n < arrayText.GetSize(); n++ )
		{
			strMsg += "\n";
			strMsg += arrayText.GetAt(n);
		}
	
		if ( Prompter.YesNo( strMsg, FALSE ) == IDYES )
		{
			for ( int n = m_arraySelected.GetSize() - 1; n >= 0; n-- )
			{
				CSortedIntItem indexItem;
				m_arraySelected.GetAt( n, indexItem );

				if ( ( indexItem.m_nItem >= 0 ) && ( indexItem.m_nItem < m_arrayPluNo.GetSize() ) )
					m_arrayPluNo.RemoveAt( indexItem.m_nItem );
			}

			m_listPlu.SetItemCountEx( m_arrayPluNo.GetSize() );
			m_listPlu.Invalidate();
		}
	}
}

/**********************************************************************/

void CPropPageOfferGroupEntryPlu::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnButtonModifier();
	*pResult = 0;
}

/**********************************************************************/

void CPropPageOfferGroupEntryPlu::OnButtonModifier() 
{
	UpdateSelectedItemArray();

	if (m_arraySelected.GetSize() == 0)
	{
		return;
	}

	for (int n = m_arraySelected.GetSize() - 1; n >= 0; n--)
	{
		COfferGroupItem arrayItem;
		GetSelectedItem(n, arrayItem);

		if (SysInfo.IsModifiableSSPluNo(_atoi64(arrayItem.m_strPluNo)) == FALSE)
		{
			m_arraySelected.RemoveAt(n);
		}
	}

	if ( m_arraySelected.GetSize() == 0 )
	{
		Prompter.Error( "You have not selected any modifiable items" );
		return;
	}

	int nMaskZeroBits = 0x3FF;
	int nMaskOneBits = 0x3FF;
	int nCommonBits = 0;

	for ( int n = 0; n < m_arraySelected.GetSize(); n++ )
	{
		COfferGroupItem arrayItem;
		GetSelectedItem( n, arrayItem );

		nCommonBits = arrayItem.m_nModifiers;
		int nFlags = nCommonBits;
		nMaskOneBits &= nFlags;
		nFlags = ~nFlags;
		nMaskZeroBits &= nFlags;
	}

	int nMask = nMaskZeroBits | nMaskOneBits;

	CString strPluNo = "";
	if ( m_arraySelected.GetSize() == 1 )
	{
		COfferGroupItem arrayItem;
		GetSelectedItem( 0, arrayItem );
		strPluNo = arrayItem.m_strPluNo;
	}

	CModifierSelectDlg dlg( m_nNodeType, nCommonBits, nMask, strPluNo, this );
	dlg.SetNoDescriptionFlag( m_bPluFilterSystem );

	if ( dlg.DoModal() == IDOK )
	{
		nMask = dlg.GetFinalMask();
		nCommonBits = dlg.GetFinalCommonBits();

		if ( nMask != 0 )
		{
			nMask = ~nMask;

			for ( int n = 0; n < m_arraySelected.GetSize(); n++ )
			{
				COfferGroupItem arrayItem;
				GetSelectedItem( n, arrayItem );
				arrayItem.m_nModifiers &= nMask;
				arrayItem.m_nModifiers += nCommonBits;
				SetSelectedItem( n, arrayItem );
			}

			m_listPlu.Invalidate();
		}
	}
}

/**********************************************************************/

void CPropPageOfferGroupEntryPlu::OnButtonEditPlu()
{
	POSITION p = m_listPlu.GetFirstSelectedItemPosition();
	int nListIdx = m_listPlu.GetNextSelectedItem(p);

	if ( ( nListIdx >= 0 ) && ( nListIdx < m_listPlu.GetItemCount() ))
	{ 		
		COfferGroupItem item;
		m_arrayPluNo.GetAt( nListIdx, item );

		int nPluIdx = 0;
		__int64 nPluNo = _atoi64( item.m_strPluNo );
		if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == TRUE )
		{

#ifndef STOCKMAN_SYSTEM
			CSPOSOfferGroupPluEditEcrman PluEdit(nPluNo, this);
#else
			CSPOSOfferGroupPluEditStockman PluEdit(nPluNo, this);
#endif
			if (PluEdit.LoadDatabase() == TRUE)
			{
				bool bEdited = PluEdit.EditPlu();

				if (TRUE == bEdited)
				{
					PluEdit.SaveChanges();
				}

				PluEdit.Finalise();
			}
			PluEdit.LoadReadOnlyDatabase();

			m_listPlu.Invalidate();
		}
	}
}

/**********************************************************************/

void CPropPageOfferGroupEntryPlu::OnButtonBrowse() 
{
	UpdateData( TRUE );

	int nDlgResult, nPluIdx;
	::ShowPluFilterDlg( m_FilterArray, this, nDlgResult, nPluIdx );

	if ( IDOK == nDlgResult )
	{
		if (nPluIdx >= 0 && nPluIdx < DataManager.Plu.GetSize())
		{
			m_editPluNo.SetWindowText(DataManager.Plu.GetPluNoString(nPluIdx));
		}
	}
}

/**********************************************************************/

void CPropPageOfferGroupEntryPlu::OnKillFocusPlu() 
{
	CString strPluNo;
	m_editPluNo.GetWindowText( strPluNo );
	strPluNo = BarcodePluNoTable.ConvertPlu( strPluNo );
	m_editPluNo.SetWindowText( strPluNo );
}

/**********************************************************************/

void CPropPageOfferGroupEntryPlu::SaveOfferGroupEntries()
{
	int nOfferNo = m_pOfferGroupRecord -> GetGroupNo();

	for ( int n = 0; n < m_arrayPluNo.GetSize(); n++ )
	{
		COfferGroupItem item;
		m_arrayPluNo.GetAt( n, item );

		__int64 nPluNo = _atoi64 ( item.m_strPluNo );

		if ( SysInfo.IsModifiableSSPluNo( nPluNo ) == TRUE )
		{
			int nFlags = item.m_nModifiers;
		
			if ( nFlags >= 1 && nFlags <= 1023 )
			{
				COfferGroupEntriesCSVRecord entry;
				entry.SetGroupNo( nOfferNo );
				entry.SetEntryPluNo( m_nEntryType, nPluNo );
				entry.SetModifierFlags( nFlags );
		
				int nIdx;
				GetEntryArray() -> InsertRecord( entry, nIdx );
			}
		}
		else
		{
			COfferGroupEntriesCSVRecord entry;
				
			__int64 nPluNo = _atoi64( item.m_strPluNo );
			entry.SetGroupNo( nOfferNo );
			entry.SetEntryPluNo( m_nEntryType, nPluNo );
				
			int nIdx;
			GetEntryArray() -> InsertRecord( entry, nIdx );
		}
	}
}	

/**********************************************************************/

BOOL CPropPageOfferGroupEntryPlu::OnApply()
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
