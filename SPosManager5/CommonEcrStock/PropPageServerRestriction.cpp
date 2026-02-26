/**********************************************************************/
#include "LocationCSVArray.h"
/**********************************************************************/
#include "PropPageServerRestriction.h"
/**********************************************************************/

CPropPageServerRestriction::CPropPageServerRestriction() : CSSPropertyPage(CPropPageServerRestriction::IDD)
{
	//{{AFX_DATA_INIT(CPropPageServerRestriction)
	//}}AFX_DATA_INIT
	m_bFullEdit = TRUE;
}

/**********************************************************************/

CPropPageServerRestriction::~CPropPageServerRestriction()
{
}

/**********************************************************************/

void CPropPageServerRestriction::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageServerRestriction)
	DDX_Control(pDX, IDC_EDIT_NAME_RECEIPT, m_editNameReceipt);
	DDX_Control(pDX, IDC_EDIT_NAME_FULL, m_editNameFull);
	DDX_Control(pDX, IDC_STATIC_LOCATION, m_staticLocation);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageServerRestriction, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageServerRestriction)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED( IDC_BUTTON_ALL, OnButtonAll )
	ON_BN_CLICKED( IDC_BUTTON_NONE, OnButtonNone )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageServerRestriction::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	m_editNameReceipt.LimitText( Server::ReceiptName.Max );
	m_editNameFull.LimitText( Server::FullName.Max );

	m_listTerminal.SubclassDlgItem ( IDC_LIST, this );
	m_listTerminal.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listTerminal.InsertColumn ( 0, "", LVCFMT_LEFT, 300 );
	m_listTerminal.SetLockItemZeroFlag( FALSE );

	int nLocIdx = DataManagerNonDb.ServerRestriction.GetCurrentLocIdx();
	for ( int nTermIdx = 0; nTermIdx < dbLocation.GetTerminalCount(nLocIdx); nTermIdx++ )
	{
		CSSListTaggedSelectItem item;
		item.m_strText = dbLocation.GetTerminalName( nLocIdx, nTermIdx );
		item.m_nData = 0;
		item.SetTagState(0);
		m_listTerminal.AddItemToList( item );
	}
	
	CString strLocation;
	strLocation.Format ( "Permitted Terminals (%s)", dbLocation.GetName(nLocIdx) );
	m_staticLocation.SetWindowText( strLocation );

	SetRecordControls();
	return TRUE;  
}

/**********************************************************************/

void CPropPageServerRestriction::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )
   {
	   m_listTerminal.AdjustRowHeight( lpMeasureItemStruct, 10 );
   }
}

/**********************************************************************/

BOOL CPropPageServerRestriction::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageServerRestriction::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPageServerRestriction::GetRecordData()
{
}

/**********************************************************************/

void CPropPageServerRestriction::SetRecordControls()
{
	m_editNameReceipt.SetWindowText(m_pServerRecord->GetReceiptEditName());
	m_editNameFull.SetWindowText(m_pServerRecord->GetFullName());

	CReportConsolidationArray<CSortedIntItem> arrayRestricted;

	CCSV csvRestriction(m_pRestrictionRecord->GetRestrictions(), ';');
	for (int n = 0; n < csvRestriction.GetSize(); n++)
	{
		CSortedIntItem item;
		item.m_nItem = csvRestriction.GetInt(n);
		arrayRestricted.Consolidate(item);
	}

	int nLocIdx = DataManagerNonDb.ServerRestriction.GetCurrentLocIdx();
	for (int nTermIdx = 0; (nTermIdx < dbLocation.GetTerminalCount(nLocIdx)) && (nTermIdx < m_listTerminal.GetItemCount()); nTermIdx++)
	{
		CSortedIntItem item;
		item.m_nItem = dbLocation.GetTNo(nLocIdx, nTermIdx);

		int nPos = 0;
		m_listTerminal.SetItemTagState(nTermIdx, arrayRestricted.Find(item, nPos) ? 0 : 1);
	}
	m_listTerminal.RedrawItems(0, m_listTerminal.GetItemCount() - 1);

	m_editNameReceipt.SetFocus();
	m_editNameReceipt.SetSel(0, -1);
}

/**********************************************************************/

bool CPropPageServerRestriction::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPageServerRestriction::SaveRecord()
{
	m_pServerRecord->SetReceiptName(GetEditBoxText(m_editNameReceipt));
	m_pServerRecord->SetFullName(GetEditBoxText(m_editNameFull));

	CCSV csv( ';' );
	int nLocIdx = DataManagerNonDb.ServerRestriction.GetCurrentLocIdx();
	for (int nTermIdx = 0; (nTermIdx < dbLocation.GetTerminalCount(nLocIdx)) && (nTermIdx < m_listTerminal.GetItemCount()); nTermIdx++)
	{
		if (m_listTerminal.GetItemTagState(nTermIdx) == 0)
		{
			csv.Add(dbLocation.GetTNo(nLocIdx, nTermIdx));
		}
	}

	m_pRestrictionRecord -> SetRestrictions( csv.GetLine() );
}

/**********************************************************************/

void CPropPageServerRestriction::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	SetRecordControls();
}

/**********************************************************************/

void CPropPageServerRestriction::OnButtonAll()
{
	m_listTerminal.SelectAll();
}

/**********************************************************************/

void CPropPageServerRestriction::OnButtonNone()
{
	m_listTerminal.SelectNone();
}

/**********************************************************************/
