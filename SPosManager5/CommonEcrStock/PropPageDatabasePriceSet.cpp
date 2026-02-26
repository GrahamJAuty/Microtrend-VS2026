/**********************************************************************/
#include "PluDatabaseHeader.h"
#include "PriceSetItemManager.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPageDatabasePriceSet.h"
/**********************************************************************/

CPropPageDatabasePriceSet::CPropPageDatabasePriceSet() : CPropertyPage(CPropPageDatabasePriceSet::IDD)
{
	//{{AFX_DATA_INIT(CPropPageDatabasePriceSet)
	//}}AFX_DATA_INIT
	m_nDbIdx = 0;
	m_pDatabaseBuffer = NULL;
	m_pPriceSetHeaderManager = NULL;
}

/**********************************************************************/

CPropPageDatabasePriceSet::~CPropPageDatabasePriceSet()
{
}

/**********************************************************************/

void CPropPageDatabasePriceSet::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageDatabasePriceSet)
	DDX_Control(pDX, IDC_COMBO_PRICESET, m_comboPriceSet);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageDatabasePriceSet, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageDatabasePriceSet)
	ON_WM_MEASUREITEM()
	ON_CBN_SELCHANGE(IDC_COMBO_PRICESET, OnSelectPriceSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageDatabasePriceSet::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	SetLocalPluControls();
	SelectPriceSetFields();
	return TRUE;    
}

/**********************************************************************/

void CPropPageDatabasePriceSet::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )
   {
	   m_listFields.AdjustRowHeight( lpMeasureItemStruct, 10 );
   }
}

/**********************************************************************/

BOOL CPropPageDatabasePriceSet::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	UpdateNewHeader();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageDatabasePriceSet::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CPropPageDatabasePriceSet::SetLocalPluControls()
{
	m_listFields.SubclassDlgItem ( IDC_LIST, this );
	m_listFields.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listFields.InsertColumn ( 0, "Fields", LVCFMT_LEFT, 250 );
	m_listFields.SetLockItemZeroFlag( TRUE );

	CCSV csvHeader( PluDatabaseHeader.GetPriceSetHeader() );
	for ( int n = 0; n < csvHeader.GetSize(); n++ )
	{
		CSSListTaggedSelectItem item;
		item.m_strText = csvHeader.GetString(n);
		item.m_nData = 0;
		item.SetTagState(0);
		m_listFields.AddItemToList( item );
	}

	for ( int n = 1; n <= 20; n++ )
	{
		CString strLine;
		strLine.Format( "DB Price Set %d", n );
		m_comboPriceSet.AddString( strLine );
	}

	m_comboPriceSet.SetCurSel(0);
	m_nPriceSet = 1;
}

/**********************************************************************/

void CPropPageDatabasePriceSet::SelectPriceSetFields()
{
	m_listFields.SelectNone();
	m_listFields.SetItemTagState( 0, 1 );

	m_pPriceSetHeaderManager -> LoadHeaderForDatabaseEdit( m_nDbIdx, m_comboPriceSet.GetCurSel() + 1 );
	
	CCSV csvIn ( PluDatabaseHeader.GetUpdatedHeader( m_pPriceSetHeaderManager -> GetNewHeader() ) );
	
	CReportConsolidationArray<CSortedStringItem> arrayTicked;
	for ( int n = 0; n < csvIn.GetSize(); n++ )
	{
		CSortedStringItem item;
		item.m_strItem = csvIn.GetString(n);
		arrayTicked.Consolidate( item );
	}

	for ( int n = 1; n < m_listFields.GetItemCount(); n++ )
	{
		CSSListTaggedSelectItem ListItem;
		m_listFields.GetListItem( n, ListItem );

		CSortedStringItem headerItem;
		headerItem.m_strItem = ListItem.m_strText;
		
		int nIdx;
		if ( arrayTicked.Find( headerItem, nIdx ) == TRUE )
		{
			m_listFields.SetItemTagState( n, 1 );
		}
	}

	m_listFields.RedrawWindow();
}

/**********************************************************************/

void CPropPageDatabasePriceSet::OnSelectPriceSet() 
{
	int nPriceSet = m_comboPriceSet.GetCurSel() + 1;

	if ( nPriceSet != m_nPriceSet )
	{
		UpdateNewHeader();
		
		bool bAddedPluFields, bRemovedPluFields, bWasEmpty;
		m_pPriceSetHeaderManager -> CheckHeaderChanges( bAddedPluFields, bRemovedPluFields, bWasEmpty );

		CStringArray arrayWarning;

		CString strTitle;
		strTitle.Format ( "Select Price Set Fields (%s, Set %d)", dbDatabase.GetName(m_nDbIdx), m_nPriceSet );
		
		bool bNeedUpdate = FALSE;

		//MUST UPDATE IF WE ADD OR REMOVE FIELDS
		if ( bAddedPluFields == TRUE || bRemovedPluFields == TRUE )
			bNeedUpdate = TRUE;

		//MUST WARN IF WE REMOVE FIELDS UNLESS OLD PRICE SETS WERE EMPTY
		if ( ( bRemovedPluFields == TRUE ) && ( bWasEmpty == FALSE ) )
		{
			arrayWarning.Add( strTitle );
			arrayWarning.Add( "The fields that you removed will not be editable in this price set." );
			arrayWarning.Add( "" );
			arrayWarning.Add( "Any price set specific values in these fields will be deleted." );
		}
		
		//MUST WARN IF WE ADD FIELDS UNLESS OLD PRICE SETS WERE EMPTY
		if ( ( bAddedPluFields == TRUE ) && ( bWasEmpty == FALSE ) )
		{	
			if ( arrayWarning.GetSize() == 0 )
				arrayWarning.Add( strTitle );
			else
				arrayWarning.Add( "" );
			
			arrayWarning.Add ( "The fields that you added will be editable in this price set." );
			arrayWarning.Add ( "" );
			arrayWarning.Add ( "You must specify values for these fields in all the Plu items" );
			arrayWarning.Add ( "+that are linked to this price set." );
		}
		
		//WARNING
		if ( arrayWarning.GetSize() != 0 )
		{
			if ( Prompter.Warning( arrayWarning ) != IDYES )
				bNeedUpdate = FALSE;
		}

		if ( bNeedUpdate == TRUE )
		{
			m_pPriceSetHeaderManager -> UpdateDatabasePriceSetFile( m_nDbIdx, m_nPriceSet );
			m_pPriceSetHeaderManager -> CreateDatabasePriceSetFile( m_nDbIdx, m_nPriceSet );
		}

		m_nPriceSet = nPriceSet;
		SelectPriceSetFields();
	}	
}

/**********************************************************************/

void CPropPageDatabasePriceSet::UpdateNewHeader()
{
	CCSV csvOut;
	
	for ( int nIndex = 0; nIndex < m_listFields.GetItemCount(); nIndex++ )
	{
		CSSListTaggedSelectItem ListItem;
		m_listFields.GetListItem( nIndex, ListItem );
		
		if ( ListItem.GetTagState() == 1 )
		{
			csvOut.Add( ListItem.m_strText );
		}
	}
	
	m_pPriceSetHeaderManager -> SetNewHeader ( csvOut.GetLine() );
}

/**********************************************************************/
