/**********************************************************************/
#include "Consolidation.h"
#include "IDraughtOptions.h"
#include "LocationCSVArray.h"
#include "MaxLengths.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
#include "PropPageIDraughtOptionsLocation.h"
/**********************************************************************/

CPropPageIDraughtOptionsLocation::CPropPageIDraughtOptionsLocation() : CSSPropertyPage(CPropPageIDraughtOptionsLocation::IDD)
{
	//{{AFX_DATA_INIT(CPropPageIDraughtOptionsLocation)
	//}}AFX_DATA_INIT

	for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
	{	
		CellTypeInfo info;
		info.m_nType =				CELL_TYPE_EDIT_TEXT;
		info.m_nSSEditType =		0xFFFF;
		info.m_nSSEditWidth =		MAX_LENGTH_IDRAUGHT_ID;
		info.m_strSSEditFormat =	"";
		info.m_nMinInt =			0;	//USED AS DUPLICATION FLAG
		info.m_nMaxInt =			0;
		info.m_strCurrent =			dbLocation.GetIDraughtID( nLocIdx );
		
		m_arrayCellTypes.Add( info );
	}

	m_bFilter = FALSE;
}

/**********************************************************************/

CPropPageIDraughtOptionsLocation::~CPropPageIDraughtOptionsLocation()
{
}

/**********************************************************************/

void CPropPageIDraughtOptionsLocation::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageIDraughtOptionsLocation)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_FILTER, m_buttonFilter);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageIDraughtOptionsLocation, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageIDraughtOptionsLocation)
	//}}AFX_MSG_MAP
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_FILTER, OnButtonFilter)
	ON_MESSAGE(WM_VALIDATE, OnEndLabelEditVariableCriteria)
	ON_MESSAGE(WM_GET_CELLTYPE, GetCellType)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageIDraughtOptionsLocation::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	m_listLocations.SubclassDlgItem ( IDC_LIST, this );
	m_listLocations.InsertColumn ( 0, "Location", LVCFMT_LEFT, 200 );
	m_listLocations.InsertColumn ( 1, "Site ID", LVCFMT_LEFT, 200 );
	m_listLocations.InsertColumn ( 2, "Duplicate", LVCFMT_LEFT, 100 );

	m_listLocations.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	m_buttonFilter.SetWindowText ( "Show Duplicate IDs" );

	MarkDuplicates( FALSE );
	ToggleFilter( FALSE );
	
	return TRUE;  
}

/**********************************************************************/

void CPropPageIDraughtOptionsLocation::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )
   {
	   m_listLocations.AdjustRowHeight( lpMeasureItemStruct, 10 );
   }
}

/**********************************************************************/

BOOL CPropPageIDraughtOptionsLocation::PreTranslateMessage(MSG* pMsg) 
{
  if(pMsg->message==WM_KEYDOWN)
  {
	  if(pMsg->wParam==VK_RETURN )
	  {
		  if ( GetFocus() == &m_listLocations )
		  {
			  m_listLocations.HandleReturn(1);	
			  pMsg->wParam=NULL ;
		  }
	  }
  }
  return CPropertyPage::PreTranslateMessage(pMsg);
}

/**********************************************************************/

BOOL CPropPageIDraughtOptionsLocation::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageIDraughtOptionsLocation::OnSetActive() 
{
	GetRecordData();
	return ( CPropertyPage::OnSetActive() != 0 );
}

/**********************************************************************/

void CPropPageIDraughtOptionsLocation::OnButtonFilter()
{
	if ( TRUE == m_bFilter )
	{
		m_bFilter = FALSE;
		m_buttonFilter.SetWindowText ( "Show Duplicate IDs" );
	}
	else
	{
		m_bFilter = TRUE;
		m_buttonFilter.SetWindowText ( "Show All Locations" );
	}

	MarkDuplicates( FALSE );
	ToggleFilter( m_bFilter );
}

/**********************************************************************/

LRESULT CPropPageIDraughtOptionsLocation::OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam) 
{
	m_listLocations.AllowListScroll( TRUE );
	m_listLocations.SetEditCell( -1, -1 );

	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)lParam;
	
	if (wParam == IDC_LIST)
	{
		// Update the item text with the new text
		CString strText = pDispInfo->item.pszText;  
	
		int nRow = pDispInfo -> item.iItem;
		int nCol = pDispInfo -> item.iSubItem;

		if ( ( nRow >= 0 ) && ( nRow < m_arrayLineIndex.GetSize() ) )
		{
			int nLocIdx = m_arrayLineIndex[ nRow ];

			if ( ( nLocIdx >= 0 ) && ( nLocIdx < m_arrayCellTypes.GetSize() ) )
			{
				CellTypeInfo info = m_arrayCellTypes.GetAt( nLocIdx );

				switch( info.m_nType )
				{
				case CELL_TYPE_EDIT_TEXT:
					{
						if ( strText.GetLength() <= MAX_LENGTH_IDRAUGHT_ID )
							m_arrayCellTypes[nLocIdx].m_strCurrent = strText;
						else
							m_listLocations.SetItem ( nRow, 2, LVIF_TEXT, info.m_strCurrent, 0, 0, 0, NULL );
					}
					MarkDuplicates( TRUE );
					break;
				}
			}
		}
	}

	return 1;
}

/**********************************************************************/

LRESULT CPropPageIDraughtOptionsLocation::GetCellType(WPARAM wParam, LPARAM lParam)
{
	CellTypeInfo* pCellType = reinterpret_cast<CellTypeInfo*>(lParam);
	
	int nCol = pCellType -> m_nCol;
	int nRow = pCellType -> m_nRow;

	if ( nCol != 1 )
	{
		pCellType -> m_nType = 0;
		return 1;
	}

	if ( nRow < 0 || nRow >= m_arrayLineIndex.GetSize() )
	{
		pCellType -> m_nType = 0;
		return 1;
	}

	int nLocIdx = m_arrayLineIndex[ nRow ];

	if ( nLocIdx < 0 || nLocIdx >= m_arrayCellTypes.GetSize() )
	{
		pCellType -> m_nType = 0;
		return 1;
	}

	pCellType -> m_nType = m_arrayCellTypes[nLocIdx].m_nType;
	pCellType -> m_nSSEditType = m_arrayCellTypes[nLocIdx].m_nSSEditType;
	pCellType -> m_strSSEditFormat = m_arrayCellTypes[nLocIdx].m_strSSEditFormat;
	pCellType -> m_nSSEditWidth = m_arrayCellTypes[nLocIdx].m_nSSEditWidth;
	
	return 1;
}

/**********************************************************************/

void CPropPageIDraughtOptionsLocation::ToggleFilter( bool bFilter )
{
	m_arrayLineIndex.RemoveAll();

	for ( int nLocIdx = 0; nLocIdx < m_arrayCellTypes.GetSize(); nLocIdx++ )
		if ( ( FALSE == bFilter ) || ( 1 == m_arrayCellTypes[ nLocIdx ].m_nMinInt ) )
			m_arrayLineIndex.Add( nLocIdx );

	m_listLocations.DeleteAllItems();

	for ( int nLineIdx = 0; nLineIdx < m_arrayLineIndex.GetSize(); nLineIdx++ )
	{
		int nLocIdx = m_arrayLineIndex.GetAt(nLineIdx);

		CString strName = dbLocation.GetName( nLocIdx );
		CString strID = m_arrayCellTypes[ nLocIdx ].m_strCurrent;
		CString strDup = ( m_arrayCellTypes[ nLocIdx ].m_nMinInt ) ? "*****" : "";

		int nPos = m_listLocations.GetItemCount();
		m_listLocations.InsertItem ( nPos, "" );
		m_listLocations.SetItem ( nPos, 0, LVIF_TEXT, strName, 0, 0, 0, NULL );
		m_listLocations.SetItem ( nPos, 1, LVIF_TEXT, strID, 0, 0, 0, NULL );
		m_listLocations.SetItem ( nPos, 2, LVIF_TEXT, strDup, 0, 0, 0, NULL );
	}
}

/**********************************************************************/

void CPropPageIDraughtOptionsLocation::MarkDuplicates( bool bRedrawList )
{
	CReportConsolidationArray<CSortedStringCounter> arrayIDs;

	for ( int n = 0; n < m_arrayCellTypes.GetSize(); n++ )
	{
		if ( m_arrayCellTypes[n].m_strCurrent != "" )
		{
			CSortedStringCounter item;
			item.m_strItem = m_arrayCellTypes[n].m_strCurrent;
			arrayIDs.Consolidate( item );
		}
	}

	for ( int n = 0; n < m_arrayCellTypes.GetSize(); n++ )
	{
		bool bDuplicate = FALSE;

		int nPos;
		if ( m_arrayCellTypes[n].m_strCurrent != "" )
		{
			CSortedStringCounter item;
			item.m_strItem = m_arrayCellTypes[n].m_strCurrent;

			if ( arrayIDs.Find( item, nPos ) == TRUE )
			{
				arrayIDs.GetAt( nPos, item );
				if ( item.m_nCount > 1 )
					bDuplicate = TRUE;
			}
		}
					
		m_arrayCellTypes[n].m_nMinInt = ( bDuplicate ) ? 1 : 0;
	}

	if ( TRUE == bRedrawList )
	{
		for ( int nLineIdx = 0; nLineIdx < m_arrayLineIndex.GetSize(); nLineIdx++ )
		{
			int nLocIdx = m_arrayLineIndex.GetAt(nLineIdx);
			CString strDup = ( m_arrayCellTypes[ nLocIdx ].m_nMinInt ) ? "*****" : "";
			m_listLocations.SetItem ( nLineIdx, 2, LVIF_TEXT, strDup, 0, 0, 0, NULL );
		}
	}
}

/**********************************************************************/

void CPropPageIDraughtOptionsLocation::GetRecordData()
{
}

/**********************************************************************/

void CPropPageIDraughtOptionsLocation::SaveRecord()
{
	CCSV csv;

	for ( int n = 0; n < m_arrayCellTypes.GetSize(); n++ )
		csv.Add( m_arrayCellTypes[n].m_strCurrent );

	IDraughtOptions.SetSiteIDBuffer( csv.GetLine() );
}

/**********************************************************************/
