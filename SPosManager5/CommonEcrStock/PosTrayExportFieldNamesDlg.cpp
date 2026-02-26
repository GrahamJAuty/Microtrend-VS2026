/**********************************************************************/
#include "PosTrayTask.h"
/**********************************************************************/
#include "PosTrayExportFieldNamesDlg.h"
/**********************************************************************/

CPosTrayExportFieldNamesDlg::CPosTrayExportFieldNamesDlg( int nTaskFileType, CWnd* pParent )
	: CSSDialog(CPosTrayExportFieldNamesDlg::IDD, pParent)
{
	m_nTaskFileType = nTaskFileType;
	
	switch( m_nTaskFileType )
	{
	case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_YEARNO );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_TRAN_SEQNO );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_DBNO );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_PLUNO );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_DPTNO_DBASE );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_GRPNO_DBASE );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_ACATNO_DBASE );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_SERVER_TRAN );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_TAXNO );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_PBANDNO );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_QTY_BASE );
		break;

	case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_YEARNO );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_DBNO );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_PAYNO );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_SERVER_TRAN );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_QTY_BASE );
		break;

	case POSTRAY_TASKFILE_EXPORT_VOID_TASK:
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_YEARNO );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_DBNO );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_PLUNO );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_DPTNO_DBASE );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_GRPNO_DBASE );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_ACATNO_DBASE );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_VOID_SEQNO );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_DISCOUNT_USAGE );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_PROMONO );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_SERVER_TRAN );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_QTY_BASE );
		break;

	case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_YEARNO );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_TRAN_SEQNO );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_DBNO );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_PLUNO );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_DPTNO_DBASE );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_GRPNO_DBASE );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_ACATNO_DBASE );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_DISCOUNT_MODE );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_PROMONO );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_SERVER_TRAN );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_TAXNO );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_PBANDNO );
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_QTY_BASE );
		break;
	}

	m_nTypeSel = 0;
}

/**********************************************************************/

CPosTrayExportFieldNamesDlg::~CPosTrayExportFieldNamesDlg()
{
}

/**********************************************************************/

void CPosTrayExportFieldNamesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPosTrayExportFieldNamesDlg, CDialog)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_TYPE, OnGetDispInfoListType)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TYPE, OnSelectType)
	ON_MESSAGE(WM_VALIDATE, OnEndLabelEditVariableCriteria)
	ON_MESSAGE(WM_GET_CELLTYPE, GetCellType)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPosTrayExportFieldNamesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_listType.SubclassDlgItem ( IDC_LIST_TYPE, this );
	m_listType.InsertColumn ( 1, "Type", LVCFMT_LEFT, 120 );
	m_listType.SetItemCountEx ( m_arrayTypes.GetSize() );
	m_listType.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listType.SetCurSel(0);

	m_listFields.SubclassDlgItem ( IDC_LIST_FIELDS, this );
	m_listFields.InsertColumn ( 0, "Field Name", LVCFMT_LEFT, 230 );
	m_listFields.InsertColumn ( 1, "Header Text", LVCFMT_LEFT, 230 );
	m_listFields.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	LoadTypeArray( 0 );
	m_nTypeSel = 0;
	m_listType.SetCurSel( 0 );
	
	return TRUE;  
}

/**********************************************************************/

void CPosTrayExportFieldNamesDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
  switch( nIDCtl )
	{
	case IDC_LIST_TYPE:
	   m_listType.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
	   break;

	case IDC_LIST_FIELDS:
		m_listFields.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
		break;
	}
}

/**********************************************************************/

BOOL CPosTrayExportFieldNamesDlg::PreTranslateMessage(MSG* pMsg) 
{
  if(pMsg->message==WM_KEYDOWN)
  {
	  if(pMsg->wParam==VK_RETURN )
	  {
		  if ( GetFocus() == &m_listFields )
		  {
			  m_listFields.HandleReturn(1);	
			  pMsg->wParam=NULL ;
		  }
	  }
  }
  return CDialog::PreTranslateMessage(pMsg);
}

/**********************************************************************/

void CPosTrayExportFieldNamesDlg::OnGetDispInfoListType(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		CString strType = "";

		int nIndex = pDispInfo->item.iItem;
		if ( ( nIndex >= 0 ) && ( nIndex < m_arrayTypes.GetSize() ))
		{
			switch( m_arrayTypes.GetAt(nIndex) )
			{
			case POSTRAY_EXPORTFIELD_YEARNO:			strType = "Date & Time";	break;
			case POSTRAY_EXPORTFIELD_TRAN_SEQNO:		strType = "Transaction";	break;
			case POSTRAY_EXPORTFIELD_DBNO:				strType = "Location";		break;
			case POSTRAY_EXPORTFIELD_PLUNO:				strType = "Plu";			break;
			case POSTRAY_EXPORTFIELD_PAYNO:				strType = "Payment";		break;
			case POSTRAY_EXPORTFIELD_DPTNO_DBASE:		strType = "Department";		break;
			case POSTRAY_EXPORTFIELD_GRPNO_DBASE:		strType = "Dept Group";		break;
			case POSTRAY_EXPORTFIELD_ACATNO_DBASE:		strType = "Category";		break;
			case POSTRAY_EXPORTFIELD_SERVER_TRAN:		strType = "Servers";		break;
			case POSTRAY_EXPORTFIELD_VOID_SEQNO:		strType = "Void Info";		break;
			case POSTRAY_EXPORTFIELD_DISCOUNT_MODE:
			case POSTRAY_EXPORTFIELD_DISCOUNT_USAGE:	strType = "Discount Info";	break;
			case POSTRAY_EXPORTFIELD_PROMONO:			strType = "Offer Info";		break;
			case POSTRAY_EXPORTFIELD_TAXNO:				strType = EcrmanOptions.GetTaxBandString();	break;
			case POSTRAY_EXPORTFIELD_PBANDNO:			strType = "Price Band";		break;
			case POSTRAY_EXPORTFIELD_QTY_BASE:			strType = "Totals";			break;
			}
		}

		lstrcpy ( pDispInfo->item.pszText, strType );
	}
	*pResult = 0;
}

/**********************************************************************/

void CPosTrayExportFieldNamesDlg::OnSelectType(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if ((pNMListView->uChanged & LVIF_STATE) 
        && (pNMListView->uNewState & LVNI_SELECTED))
    {
		int nSel = m_listType.GetCurSel();

		if ( ( nSel >= 0 ) && ( nSel <= m_arrayTypes.GetSize() ) && ( nSel != m_nTypeSel ) )
		{
			LoadTypeArray( nSel );
			m_nTypeSel = nSel;
		}
    }
}

/**********************************************************************/

void CPosTrayExportFieldNamesDlg::LoadTypeArray( int nSel )
{
	int nType = m_arrayTypes.GetAt( nSel );

	m_arrayFields.RemoveAll();

	switch( nType )
	{
	case POSTRAY_EXPORTFIELD_DBNO:
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_DBNO );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_LOCNO );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_RPNO );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_TRMNO );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_DBNAME );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_LOCNAME );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_RPNAME );
		break;

	case POSTRAY_EXPORTFIELD_TRAN_SEQNO:
		switch( m_nTaskFileType )
		{
		case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
		case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_TRAN_SEQNO );
			break;
		}

		break;

	case POSTRAY_EXPORTFIELD_YEARNO:		
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_DATE_CAL );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_DATE_BUS );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_YEARNO );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_MONTHNO );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_MONTHNAME );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_DATE_WEEK );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_WEEKNO );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_WEEKDAYNO );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_WEEKDAYNAME );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_TIME_TRAN );
		
		switch( m_nTaskFileType )
		{
		case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_TIME_SALE );
			break;

		case POSTRAY_TASKFILE_EXPORT_VOID_TASK:
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_TIME_VOID );
			break;

		case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_TIME_DISCOUNT );
			break;
		}

		break;
		
	case POSTRAY_EXPORTFIELD_PLUNO:		
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_PLUNO );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_MODNO );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_REPTEXT_BASE );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_REPTEXT_MOD );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_MODNAME );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_SUPPREF );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_SORTCODE );
#ifdef STOCKMAN_SYSTEM
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_STOCKCODE );
#endif
		break;

	case POSTRAY_EXPORTFIELD_DPTNO_DBASE:
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_DPTNO_DBASE );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_DPTNO_SALE );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_DPTNAME_DBASE );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_DPTNAME_SALE );
		break;

	case POSTRAY_EXPORTFIELD_GRPNO_DBASE:
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_GRPNO_DBASE );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_GRPNO_SALE );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_GRPNAME_DBASE );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_GRPNAME_SALE );
		break;

	case POSTRAY_EXPORTFIELD_ACATNO_DBASE:
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_ACATNO_DBASE );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_ACATNO_SALE );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_ACATNAME_DBASE );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_ACATNAME_SALE );
		break;

	case POSTRAY_EXPORTFIELD_PAYNO:	
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_PAYNO );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_PAYNAME );
		break;

	case POSTRAY_EXPORTFIELD_VOID_SEQNO:	
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_VOID_SEQNO );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_VOID_TABLENO );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_VOID_LINE_TYPE );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_VOID_BEFOREAFTER );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_VOID_REASON );
		break;

	case POSTRAY_EXPORTFIELD_DISCOUNT_USAGE:
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_DISCOUNT_USAGE );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_DISCOUNT_TYPE );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_DISCOUNT_RATE );
		break;

	
	case POSTRAY_EXPORTFIELD_DISCOUNT_MODE:
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_DISCOUNT_LINE_TYPE );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_DISCOUNT_MODE );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_DISCOUNT_TYPE );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_DISCOUNT_RATE );
		break;

	case POSTRAY_EXPORTFIELD_PROMONO:
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_PROMONO );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_PROMONAME );

		switch( m_nTaskFileType )
		{
		case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_MIXTYPENO );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_MIXTYPENAME );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_MIXMATCHNO );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_MIXMATCHNAME );
			break;
		}
		break;

	case POSTRAY_EXPORTFIELD_SERVER_TRAN:	

		switch( m_nTaskFileType )
		{
		case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
		case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_SERVER_TRAN );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_SERVER_ITEM );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_SRVNAME_TRAN );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_SRVNAME_ITEM );
			break;

		case POSTRAY_TASKFILE_EXPORT_VOID_TASK:
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_SERVER_TRAN );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_SERVER_ITEM );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_SERVER_AUTH );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_SRVNAME_TRAN );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_SRVNAME_ITEM );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_SRVNAME_AUTH );
			break;

		case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_SERVER_TRAN );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_SRVNAME_TRAN );
			break;
		}

		break;

	case POSTRAY_EXPORTFIELD_TAXNO:
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_TAXNO );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_TAXNAME );
		break;

	case POSTRAY_EXPORTFIELD_PBANDNO:
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_PBANDNO );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_PBANDNAME );
		break;
		
	case POSTRAY_EXPORTFIELD_QTY_BASE:
		switch( m_nTaskFileType )
		{
		case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_QTY_BASE );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_QTY_MOD );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_VALUE_FULL );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_DISCOUNTPREMIUM );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_DISCOUNTONLY );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_PREMIUMONLY );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_VALUE_DISC );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_TAX );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_VALUE_NET );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_COST );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_WASTE_QTY_BASE );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_WASTE_QTY_MOD );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_WASTE_COST );
			break;

		case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_LINE_COUNT );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_DISCOUNTPREMIUM );
			break;

		case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_QTY_PAY );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_QTY_COVERS );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_VALUE_TENDER );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_VALUE_AMOUNT );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_VALUE_GRATUITY );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_VALUE_CASHBACK );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_VALUE_SURPLUS );
			break;

		case POSTRAY_TASKFILE_EXPORT_VOID_TASK:
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_LINE_COUNT );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_QTY_BASE );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_QTY_MOD );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_VALUE_VOID );
			break;
		}
	}

	m_listFields.DeleteAllItems();
	m_arrayCellTypes.RemoveAll();
	for ( int n = 0; n < m_arrayFields.GetSize(); n++ )
	{	
		CPosTrayExportFieldName Name;
		Name.m_nFieldType = m_arrayFields.GetAt(n);
		DataManagerNonDb.PosTrayExportFieldNames.GetNameInfo( Name );

		CString strFieldName = Name.GetSingleColumnName();
		if (POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK == m_nTaskFileType)
		{
			if (POSTRAY_EXPORTFIELD_DISCOUNTPREMIUM == Name.m_nFieldType)
			{
				strFieldName = "Discount or Premium Amount";
			}
		}

		CellTypeInfo info;
		info.m_nType =				CELL_TYPE_EDIT_TEXT;
		info.m_nSSEditType =		0xFFFF;
		info.m_nSSEditWidth =		POSTRAY_MAXLENGTH_FIELD_NAME;
		info.m_strSSEditFormat =	"";
		info.m_nMinInt =			Name.m_nFieldType;
		info.m_nMaxInt =			0;
		info.m_strCurrent =			Name.GetHeaderNameForExport();
	
		m_arrayCellTypes.Add( info );

		int nPos = m_listFields.GetItemCount();
		m_listFields.InsertItem ( nPos, "" );
		m_listFields.SetItem ( nPos, 0, LVIF_TEXT, strFieldName, 0, 0, 0, NULL );
		m_listFields.SetItem ( nPos, 1, LVIF_TEXT, Name.GetHeaderNameForExport(), 0, 0, 0, NULL );
	}
	m_listFields.SetSelectionMark( 0 );
	m_listFields.SetItemState( 0, LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
	m_listFields.EnsureVisible( 0, FALSE );
	m_listFields.Invalidate();
}

/**********************************************************************/

LRESULT CPosTrayExportFieldNamesDlg::OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam) 
{
	m_listFields.AllowListScroll( TRUE );
	m_listFields.SetEditCell( -1, -1 );

	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)lParam;
	
	if (wParam == IDC_LIST_FIELDS )
	{
		// Update the item text with the new text
		CString strText = pDispInfo->item.pszText;  
	
		int nRow = pDispInfo -> item.iItem;
		int nCol = pDispInfo -> item.iSubItem;

		if ( ( nRow >= 0 ) && ( nRow < m_arrayCellTypes.GetSize() ) )
		{
			CellTypeInfo info = m_arrayCellTypes.GetAt( nRow );

			switch( info.m_nType )
			{
			case CELL_TYPE_EDIT_TEXT:
				{
					if ( strText.GetLength() <= POSTRAY_MAXLENGTH_FIELD_NAME )
					{
						::TrimSpaces( strText, FALSE );
						
						CPosTrayExportFieldName Name;
						Name.m_nFieldType = info.m_nMinInt;
						DataManagerNonDb.PosTrayExportFieldNames.GetNameInfo( Name );
			
						if ( strText == Name.m_strHeaderNameDefault )
							strText = "";

						Name.m_strHeaderNameUser = strText;
						DataManagerNonDb.PosTrayExportFieldNames.SetNameInfo( Name );

						m_arrayCellTypes[nRow].m_strCurrent = Name.GetHeaderNameForExport();
					}
					
					m_listFields.SetItem ( nRow, 1, LVIF_TEXT, m_arrayCellTypes[nRow].m_strCurrent, 0, 0, 0, NULL );
				}
				break;
			}
		}
	}
	
	return 1;
}

/**********************************************************************/

LRESULT CPosTrayExportFieldNamesDlg::GetCellType(WPARAM wParam, LPARAM lParam)
{
	CellTypeInfo* pCellType = reinterpret_cast<CellTypeInfo*>(lParam);
	
	int nCol = pCellType -> m_nCol;
	int nRow = pCellType -> m_nRow;

	if ( nCol != 1 )
	{
		pCellType -> m_nType = 0;
		return 1;
	}

	if ( nRow < 0 || nRow >= m_arrayCellTypes.GetSize() )
	{
		pCellType -> m_nType = 0;
		return 1;
	}

	pCellType -> m_nType = m_arrayCellTypes[nRow].m_nType;
	pCellType -> m_nSSEditType = m_arrayCellTypes[nRow].m_nSSEditType;
	pCellType -> m_strSSEditFormat = m_arrayCellTypes[nRow].m_strSSEditFormat;
	pCellType -> m_nSSEditWidth = m_arrayCellTypes[nRow].m_nSSEditWidth;
	
	return 1;
}

/**********************************************************************/

void CPosTrayExportFieldNamesDlg::OnOK()
{
	EndDialog( IDOK );
}

/**********************************************************************/

void CPosTrayExportFieldNamesDlg::OnCancel()
{
	EndDialog( IDCANCEL );
}

/**********************************************************************/
