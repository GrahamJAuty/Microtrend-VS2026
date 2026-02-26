/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "ExportDate.h"
#include "LocationCSVArray.h"
#include "ServerDatabaseHeader.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ServerExportDlg.h"
/**********************************************************************/

CServerExportDlg::CServerExportDlg( int nServerDbIdx, int nServerLocIdx, CWnd* pParent)
: CSSDialog(CServerExportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerExportDlg)
	//}}AFX_DATA_INIT
	m_strServerFrom = "1";
	m_strServerTo = "9999";
	m_nServerDbIdx = nServerDbIdx;
	m_nServerLocIdx = nServerLocIdx;
}

/**********************************************************************/

void CServerExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerExportDlg)
	DDX_Control(pDX, IDC_EDIT_DESTINATION, m_editDestination);
	//}}AFX_DATA_MAP
	DDX_IntegerString(pDX, IDC_EDIT_SERVER_RANGE_START, m_strServerFrom);
	DDV_IntegerString(pDX, m_strServerFrom, "1", "9999" );
	DDX_IntegerString(pDX, IDC_EDIT_SERVER_RANGE_END, m_strServerTo);
	DDV_IntegerString(pDX, m_strServerTo, "1", "9999" );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CServerExportDlg, CDialog)
	//{{AFX_MSG_MAP(CServerExportDlg)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_STATIC_FROM, OnStaticFrom)
	ON_BN_CLICKED(IDC_STATIC_TO, OnStaticTo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CServerExportDlg::GetEditServerFrom() { return GetEdit( IDC_EDIT_SERVER_RANGE_START ); }
CEdit* CServerExportDlg::GetEditServerTo() { return GetEdit( IDC_EDIT_SERVER_RANGE_END ); }
/**********************************************************************/

void CServerExportDlg::ConsolidateString( CReportConsolidationArray<CSortedStringItem>& arraySorted, CString strField )
{
	CSortedStringItem item;
	item.m_strItem = strField;
	arraySorted.Consolidate( item );
}

/**********************************************************************/

void CServerExportDlg::ConsolidateString( CStringArray& arrayStrings, CReportConsolidationArray<CSortedStringItem>& arraySorted, CString strField )
{
	arrayStrings.Add( strField );

	CSortedStringItem item;
	item.m_strItem = strField.MakeUpper();
	arraySorted.Consolidate( item );
}

/**********************************************************************/

void CServerExportDlg::CreateAvailableFieldsArray( CStringArray& arrayFields, CReportConsolidationArray<CSortedStringItem>& arrayAvailableFields )
{
	arrayFields.RemoveAll();
	arrayAvailableFields.RemoveAll();

	CReportConsolidationArray<CSortedStringItem> arrayExcludedFields;
	ConsolidateString( arrayExcludedFields, Server::ServerNo.Label );
	ConsolidateString(arrayExcludedFields, "");

	if (DealerFlags.GetServerHomeLocationFlag() == FALSE)
	{
		ConsolidateString(arrayExcludedFields, Server::HomeNwkLocNo.Label);
	}
	
	//USE PLU DATABASE HEADER TO POPULATE LIST BOX
	CCSV csvHeaderServer ( ServerDatabaseHeader.GetDatabaseHeader() );

	CSortedStringItem item;
	
	for ( int nPos = 0; nPos < csvHeaderServer.GetSize(); nPos++ )
	{
		int nIdx;
		item.m_strItem = csvHeaderServer.GetString ( nPos ); 
		if ( arrayExcludedFields.Find( item, nIdx ) == FALSE )
		{
			ConsolidateString( arrayFields, arrayAvailableFields, item.m_strItem );
		}
	}
}

/**********************************************************************/

BOOL CServerExportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SubclassEdit ( IDC_EDIT_SERVER_RANGE_START, SS_NUM, MAX_SERVERNO_LEN, "%s" );
	SubclassEdit ( IDC_EDIT_SERVER_RANGE_END, SS_NUM, MAX_SERVERNO_LEN, "%s" );

	m_listFields.SubclassDlgItem ( IDC_LIST, this );
	m_listFields.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listFields.InsertColumn ( 0, "Fields", LVCFMT_LEFT, 200 );
	m_listFields.SetLockItemZeroFlag( TRUE );

	CStringArray arrayFields;
	CReportConsolidationArray<CSortedStringItem> arrayAvailableFields;
	CreateAvailableFieldsArray( arrayFields, arrayAvailableFields );
	
	//READ LINE OF SELECTED FIELDS IF AVAILABLE
	CString strBuffer;
	CSSFile fileFields;
	CFilenameUpdater FnUp( SysFiles::ServerCSVDef );
	if ( fileFields.Open ( FnUp.GetFilenameToUse(), "rb" ) == TRUE ) 
	{
		fileFields.ReadString ( strBuffer );
		fileFields.Close();
	}
	else
	{
		strBuffer = "";
	}

	AddFieldToList( Server::ServerNo.Label, 1 );

	CCSV csvFields ( strBuffer ); 
	for (int nPos = 0; nPos < csvFields.GetSize(); nPos++)
	{
		CString strField = csvFields.GetString(nPos);
		CString strUpper = strField;
		strUpper.MakeUpper();

		CSortedStringItem item;
		item.m_strItem = strUpper;

		int nFindPos;
		if (arrayAvailableFields.Find(item, nFindPos) == TRUE)
		{
			AddFieldToList(strField, 1);
			arrayAvailableFields.RemoveAt(nFindPos);
		}
	}

	for ( int n = 0; n < arrayFields.GetSize(); n++ )
	{
		CString strField = arrayFields.GetAt(n);
		CString strUpper = strField;
		strUpper.MakeUpper();

		CSortedStringItem item;
		item.m_strItem = strUpper;

		int nFindPos;
		if ( arrayAvailableFields.Find( item, nFindPos ) == TRUE )
		{
			AddFieldToList( strField, 0 );
			arrayAvailableFields.RemoveAt( nFindPos );
		}

	}

	CString strExportPath = "";
	{
		CSSFile fileExportPath;
		if (fileExportPath.Open(DataManagerNonDb.GetFilePathServerExport(m_nServerDbIdx, m_nServerLocIdx), "rb") == TRUE)
		{
			fileExportPath.ReadString(strExportPath);
		}
	}

	if ( strExportPath == "" )
	{
		strExportPath = DataManagerNonDb.GetDefaultServerExportPath(m_nServerDbIdx, m_nServerLocIdx);
	}
	
	m_editDestination.SetWindowText ( strExportPath );
	return TRUE;  
}

/**********************************************************************/

void CServerExportDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )  
   {
	   m_listFields.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
   }
}

/**********************************************************************/

void CServerExportDlg::AddFieldToList( const char* szText, int nTagState )
{
	CSSListTaggedSelectItem item;
	item.m_strText = szText;
	item.m_nData = 0;
	item.SetTagState(nTagState);
	m_listFields.AddItemToList( item );
}

/**********************************************************************/

void CServerExportDlg::OnButtonSave() 
{
	UpdateData( TRUE, FALSE );

	CSSFile fileFields;
	CFilenameUpdater FnUp( SysFiles::ServerCSVDef );
	if ( fileFields.Open ( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
	{
		CreateSavedSettingsLine();
		fileFields.WriteLine ( m_strSavedSettingsLine );
	}
}

/**********************************************************************/

void CServerExportDlg::OnButtonSet() 
{
	char buffer [ 512 ];
	
	CString strTitle = "Export Server CSV File";
	CFileDialog dlgFile ( FALSE, "csv", "serverxfer", OFN_HIDEREADONLY|OFN_NOCHANGEDIR, "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||", this );
	dlgFile.m_ofn.lpstrTitle = strTitle;
	
	CString strLastFile = "";
	m_editDestination.GetWindowText( strLastFile );
	strcpy_s ( buffer, strLastFile );
	dlgFile.m_ofn.lpstrFile = buffer;
	dlgFile.m_ofn.nMaxFile = 512;
	
	if ( dlgFile.DoModal() == IDOK )
		m_editDestination.SetWindowText ( dlgFile.GetPathName() );
}

/**********************************************************************/

void CServerExportDlg::AppendServerHeader ( CCSV& csv )
{
	for ( int nPos = 0; nPos < m_listFields.GetItemCount(); nPos++ )
	{
		CSSListTaggedSelectItem ListItem;
		m_listFields.GetListItem( nPos, ListItem );
		
		if ( ListItem.GetTagState() == 1 )
			csv.Add( ListItem.m_strText );
	}
}

/**********************************************************************/

void CServerExportDlg::CreateServerHeaderLine()
{
	CCSV csv;
	AppendServerHeader ( csv );
	m_strServerHeaderLine = csv.GetLine();
}

/**********************************************************************/

void CServerExportDlg::CreateSavedSettingsLine()
{
	CCSV csv;
	AppendServerHeader ( csv );
	m_strSavedSettingsLine = csv.GetLine();
}

/**********************************************************************/

void CServerExportDlg::OnButtonDown() 
{
	int nSel = m_listFields.GetCurSel();
	if ( nSel > 0 && nSel < m_listFields.GetItemCount() - 1 )
		m_listFields.SwapItems( nSel, nSel + 1 );
}

/**********************************************************************/

void CServerExportDlg::OnButtonUp() 
{
	int nSel = m_listFields.GetCurSel();
	if ( nSel > 1 && nSel < m_listFields.GetItemCount() )
		m_listFields.SwapItems( nSel, nSel - 1 );
}

/**********************************************************************/

void CServerExportDlg::OnButtonAll() 
{
	m_listFields.SelectAll();
	m_strServerFrom = "1";
	m_strServerTo = "";
	::AddLeading(m_strServerTo, MAX_SERVERNO_LEN, '9');
	UpdateData ( FALSE );
}

/**********************************************************************/

void CServerExportDlg::OnButtonNone() 
{
	m_listFields.SelectNone();
}

/**********************************************************************/

void CServerExportDlg::OnStaticFrom() 
{
	GetEditServerFrom()->SetWindowText("1");
}

/**********************************************************************/

void CServerExportDlg::OnStaticTo() 
{
	CString strEnd = "";
	::AddLeading ( strEnd, MAX_SERVERNO_LEN, '9' );
	GetEditServerTo() -> SetWindowText( strEnd );
}

/**********************************************************************/

void CServerExportDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		CString strExportPath;
		m_editDestination.GetWindowText( strExportPath );
		
		{
			CSSFile fileExportPath;
			if (fileExportPath.Open(DataManagerNonDb.GetFilePathServerExport(m_nServerDbIdx, m_nServerLocIdx), "wb") == TRUE)
			{
				fileExportPath.WriteLine(strExportPath);
			}
		}

		CreateServerHeaderLine();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/

