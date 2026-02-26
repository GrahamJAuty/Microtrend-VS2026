/**********************************************************************/
#include "PosTrayTask.h"
/**********************************************************************/
#include "PosTrayExportFieldDlg.h"
/**********************************************************************/

CPosTrayExportFieldDlg::CPosTrayExportFieldDlg( int nTaskFileType, int nFieldType, CWnd* pParent )
	: CSSDialog(CPosTrayExportFieldDlg::IDD, pParent)
{
	m_bAdd = FALSE;
	m_nTaskFileType = nTaskFileType;
	m_nFieldType = nFieldType;
	Init();
}

/**********************************************************************/

CPosTrayExportFieldDlg::CPosTrayExportFieldDlg( int nTaskFileType, CPosTrayExportFieldDlg** pParentToThis, CWnd* pParent )
	: CSSDialog(CPosTrayExportFieldDlg::IDD, pParent)
{
	m_bAdd = TRUE;
	m_nTaskFileType = nTaskFileType;
	m_nFieldType = 0;
	m_pParentToThis = pParentToThis;
	*m_pParentToThis = this;
	Init();
}

/**********************************************************************/

void CPosTrayExportFieldDlg::Init()
{
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
		m_arrayTypes.Add( POSTRAY_EXPORTFIELD_TRAN_SEQNO );
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

CPosTrayExportFieldDlg::~CPosTrayExportFieldDlg()
{
}

/**********************************************************************/

void CPosTrayExportFieldDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control( pDX, IDOK, m_buttonOK);
	DDX_Control( pDX, IDC_BUTTON_ADD, m_buttonAdd);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPosTrayExportFieldDlg, CDialog)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_TYPE, OnGetDispInfoListType)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_FIELD, OnGetDispInfoListField)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FIELD, OnDoubleClickListField)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TYPE, OnSelectType)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPosTrayExportFieldDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	{
		CString strAction = "";
		if ( TRUE == m_bAdd )
		{
			strAction = "Add Fields to";
			ShowAndEnableWindow( &m_buttonOK, FALSE );
		}
		else
		{
			strAction =	"Change Field in";
			ShowAndEnableWindow( &m_buttonAdd, FALSE );
		}

		CString strType = "";
		switch( m_nTaskFileType )
		{
		case POSTRAY_TASKFILE_EXPORT_SALES_TASK:		strType = "Plu Sales";	break;
		case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:		strType = "Payment";	break;
		case POSTRAY_TASKFILE_EXPORT_VOID_TASK:			strType = "Voids";		break;
		case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:		strType = "Discounts";	break;
		}

		CString strText = "";
		strText += strAction;
		strText += " CSV Export Line for ";
		strText += strType;
		SetWindowText( strText );
	}

	m_listType.SubclassDlgItem ( IDC_LIST_TYPE, this );
	m_listType.InsertColumn ( 1, "Type", LVCFMT_LEFT, 120 );
	m_listType.SetItemCountEx ( m_arrayTypes.GetSize() );
	m_listType.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listType.SetCurSel(0);

	m_listField.SubclassDlgItem ( IDC_LIST_FIELD, this );
	m_listField.InsertColumn ( 1, "Field", LVCFMT_LEFT, 280 );
	m_listField.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	
	bool bFound = FALSE;
	for ( int nTypeSel = 0; ( nTypeSel < m_arrayTypes.GetSize() ) && ( FALSE == bFound ); nTypeSel++ )
	{
		LoadTypeArray( nTypeSel );

		for ( int nFieldSel = 0; ( nFieldSel < m_arrayFields.GetSize() ) && ( FALSE == bFound ); nFieldSel++ )
		{
			if ( m_arrayFields.GetAt( nFieldSel ) == m_nFieldType )
			{
				m_nTypeSel = nTypeSel;
				m_listType.SetCurSel( nTypeSel );
				
				m_listField.SetItemCountEx( m_arrayFields.GetSize() );
				m_listField.Invalidate();
				m_listField.SetCurSel( nFieldSel );
				bFound = TRUE;
			}
		}
	}

	if ( FALSE == bFound )
	{
		LoadTypeArray( 0 );
		m_nTypeSel = 0;
		m_listType.SetCurSel( 0 );
		m_listField.SetItemCountEx( m_arrayFields.GetSize() );
		m_listField.Invalidate();
		m_listField.SetCurSel( 0 );
	}

	return TRUE;  
}

/**********************************************************************/

void CPosTrayExportFieldDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	switch( nIDCtl )
	{
	case IDC_LIST_TYPE:
	   m_listType.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
	   break;

	case IDC_LIST_FIELD:
		m_listField.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
		break;
	}
}

/**********************************************************************/

void CPosTrayExportFieldDlg::OnGetDispInfoListType(NMHDR* pNMHDR, LRESULT* pResult) 
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
			case POSTRAY_EXPORTFIELD_PROMONO		:	strType = "Offer Info";	break;
			case POSTRAY_EXPORTFIELD_TAXNO:				strType = "Tax Band";		break;
			case POSTRAY_EXPORTFIELD_PBANDNO:			strType = "Price Band";		break;
			case POSTRAY_EXPORTFIELD_QTY_BASE:			strType = "Totals";			break;
			}
		}

		lstrcpy ( pDispInfo->item.pszText, strType );
	}
	*pResult = 0;
}

/**********************************************************************/

void CPosTrayExportFieldDlg::OnGetDispInfoListField(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		CString strField = "";

		int nIndex = pDispInfo->item.iItem;
		if ( ( nIndex >= 0 ) && ( nIndex < m_arrayFields.GetSize() ))
		{
			CPosTrayExportFieldName Name;
			Name.m_nFieldType = m_arrayFields.GetAt(nIndex);
			DataManagerNonDb.PosTrayExportFieldNames.GetNameInfo( Name );
			strField = Name.GetSingleColumnName();

			if (POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK == m_nTaskFileType)
			{
				if (POSTRAY_EXPORTFIELD_DISCOUNTPREMIUM == Name.m_nFieldType)
				{
					strField = "Discount or Premium Amount";
				}
			}
		}

		lstrcpy ( pDispInfo->item.pszText, strField );
	}
	*pResult = 0;
}

/**********************************************************************/

void CPosTrayExportFieldDlg::OnSelectType(NMHDR* pNMHDR, LRESULT* pResult) 
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
			m_listField.SetItemCountEx( m_arrayFields.GetSize() );
			m_listField.Invalidate();
			m_listField.SetCurSel(0);
		}
    }
}

/**********************************************************************/

void CPosTrayExportFieldDlg::LoadTypeArray( int nSel )
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

		switch ( m_nTaskFileType )
		{
		case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
		case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_TBLGRPNO );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_TBLNO );
			break;
		}

		m_arrayFields.Add( POSTRAY_EXPORTFIELD_DBNAME );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_LOCNAME );
		m_arrayFields.Add( POSTRAY_EXPORTFIELD_RPNAME );

		switch ( m_nTaskFileType )
		{
		case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
		case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_TBLGRPNAME );
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_TBLNAME );
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

	case POSTRAY_EXPORTFIELD_TRAN_SEQNO:
		switch( m_nTaskFileType )
		{
		case POSTRAY_TASKFILE_EXPORT_SALES_TASK:
		case POSTRAY_TASKFILE_EXPORT_DISCOUNT_TASK:
		case POSTRAY_TASKFILE_EXPORT_PAYMENT_TASK:
			m_arrayFields.Add( POSTRAY_EXPORTFIELD_TRAN_SEQNO );
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
		break;
	}
}

/**********************************************************************/

void CPosTrayExportFieldDlg::OnDoubleClickListField(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	if ( TRUE == m_bAdd )
		OnButtonAdd();
	else
		OnOK();

	*pResult = 0;
}

/**********************************************************************/

void CPosTrayExportFieldDlg::OnButtonAdd()
{
	m_nFieldType = POSTRAY_EXPORTFIELD_DATE_CAL;

	int nSel = m_listField.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_arrayFields.GetSize() ) )
		m_nFieldType = m_arrayFields.GetAt( nSel );

	GetParent() -> PostMessage ( WM_APP, 0, (long) m_nFieldType );
}

/**********************************************************************/

void CPosTrayExportFieldDlg::OnOK()
{
	m_nFieldType = POSTRAY_EXPORTFIELD_DATE_CAL;

	int nSel = m_listField.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_arrayFields.GetSize() ) )
		m_nFieldType = m_arrayFields.GetAt( nSel );

	EndDialog( IDOK );
}

/**********************************************************************/

void CPosTrayExportFieldDlg::OnCancel()
{
	if ( m_bAdd == TRUE )
		DestroyWindow();
	else
		EndDialog( IDCANCEL );
}

/**********************************************************************/

void CPosTrayExportFieldDlg::PostNcDestroy() 
{
	if ( m_bAdd == TRUE )
	{
		*m_pParentToThis = NULL;
		delete this;
	}
}

/**********************************************************************/

