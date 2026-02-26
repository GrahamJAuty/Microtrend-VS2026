/**********************************************************************/
#include "RepSpawn.h"
#include "SPOSKeyboardKeyHelpers.h"
/**********************************************************************/
#include "SPOSKeyboardListPrintDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CSPOSKeyboardListPrintDlg, CDialog)
/**********************************************************************/

CSPOSKeyboardListPrintDlg::CSPOSKeyboardListPrintDlg(CWnd* pParent)
	: CDialog(CSPOSKeyboardListPrintDlg::IDD, pParent)
{
	m_bListStandard = TRUE;
	m_bListLarge = TRUE;
	m_bFunctionStandard = TRUE;
	m_bFunctionLarge = TRUE;
	m_bFunctionBottom = TRUE;
}

/**********************************************************************/

CSPOSKeyboardListPrintDlg::~CSPOSKeyboardListPrintDlg()
{
}

/**********************************************************************/

void CSPOSKeyboardListPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_LIST_STANDARD, m_bListStandard );
	DDX_Check(pDX, IDC_CHECK_LIST_LARGE, m_bListLarge );
	DDX_Check(pDX, IDC_CHECK_FUNCTION_STANDARD, m_bFunctionStandard );
	DDX_Check(pDX, IDC_CHECK_FUNCTION_LARGE, m_bFunctionLarge );
	DDX_Check(pDX, IDC_CHECK_FUNCTION_BOTTOM, m_bFunctionBottom );
}

/**********************************************************************/
BEGIN_MESSAGE_MAP(CSPOSKeyboardListPrintDlg, CDialog)
END_MESSAGE_MAP()
/**********************************************************************/

void CSPOSKeyboardListPrintDlg::OnOK()
{
	if ( UpdateData( TRUE ) == FALSE )
		return;

	if ( ( 0 == m_bListStandard ) && 
			( 0 == m_bListLarge ) && 
			( 0 == m_bFunctionStandard ) && 
			( 0 == m_bFunctionLarge ) && 
			( 0 == m_bFunctionBottom ) )
	{
		Prompter.Error( "You must select at least one list type\n\nbefore you can create this report." );
		return;
	}

	CString strFolder1 = SPOSKeyboardManager.GetLayoutOptions() -> GetReportFolder();
	CString strFolder2 = SPOSKeyboardManager.GetLayoutOptions() -> GetReportSettingsFolder();
	CString strReportPath = strFolder1 + "\\REPORT.REP";
	CString strParamPath = strFolder2 + "\\KEYBDLST.PRM";

	CReportFile ReportFile;
	if ( ReportFile.Open ( strReportPath ) == FALSE )
	{
		Prompter.Error ( "Unable to create report." );
		return;
	}

	ReportFile.SetStyle1 ( "Keyboard Lists" );
	ReportFile.AddColumn ( "Page", TA_LEFT, 120 );
	ReportFile.AddColumn ( "Pos", TA_LEFT, 200 );
	ReportFile.AddColumn ( "Size", TA_LEFT, 200 );
	ReportFile.AddColumn ( "Key Type", TA_LEFT, 300 );
	ReportFile.AddColumn ( "Key Num", TA_LEFT, 200 );
	ReportFile.AddColumn ( "Key Detail", TA_LEFT, 500 );
	ReportFile.AddColumn ( "Key Text", TA_LEFT, 800 );
	
	if ( 0 != m_bListStandard )
		WriteLists( ReportFile, LISTTYPE_ITEM_STANDARD );

	if ( 0 != m_bListLarge )
		WriteLists( ReportFile, LISTTYPE_ITEM_LARGE );

	if ( 0 != m_bFunctionStandard )
		WriteLists( ReportFile, LISTTYPE_FUNCTION_STANDARD );

	if ( 0 != m_bFunctionLarge )
		WriteLists( ReportFile, LISTTYPE_FUNCTION_LARGE );

	if ( 0 != m_bFunctionBottom )
		WriteLists( ReportFile, LISTTYPE_FUNCTION_BOTTOM );

	ReportFile.WriteLine( "" );
	ReportFile.Close();

	CReportManager ReportManager;
	ReportManager.DisplayReport( strReportPath, strParamPath, "", this );
}

/**********************************************************************/

void CSPOSKeyboardListPrintDlg::WriteLists( CReportFile& ReportFile, int nType )
{
	for ( int nListIdx = 0; nListIdx < SPOSKeyboardManager.GetListManager() -> GetListCount(); nListIdx++ )
	{
		CSPOSKeyboardListRecord ListRecord;
		SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx, ListRecord );

		if ( ListRecord.GetListType() != nType )
			continue;

		CString strType;
		switch( nType )
		{
		case LISTTYPE_ITEM_STANDARD:
			strType = "(Standard List Area)";
			break;

		case LISTTYPE_ITEM_LARGE:
			strType = "(Large List Area)";
			break;

		case LISTTYPE_FUNCTION_STANDARD:
			strType = "(Standard Function Area)";
			break;

		case LISTTYPE_FUNCTION_LARGE:
			strType = "(Large Function Area)";
			break;

		case LISTTYPE_FUNCTION_BOTTOM:
			strType = "(Bottom Function Bar)";
			break;

		default:
			strType = "(Unknown List Type)";
			break;
		}

		ReportFile.WriteLine( "<LI>" );
		
		CString str;
		str.Format( "\t%5.5d\t<..>",
			ListRecord.GetListNo() );
		ReportFile.WriteLine( str + ListRecord.GetName() );

		str.Format( "\t\t<..>%s",
			(const char*) strType );
		ReportFile.WriteLine( str );

		ReportFile.WriteLine( "<LI>" );

		int nPageCount = ListRecord.GetPageCount();

		for ( int nPageNo = 1; nPageNo <= nPageCount; nPageNo++ )
		{
			CArray<CSPOSKeyboardKeyExtent,CSPOSKeyboardKeyExtent> map;
			SPOSKeyboardManager.GetListManager() -> GetKeyExtentMap( ListRecord.GetListNo(), nPageNo, map );

			if ( nPageNo != 1 )
				ReportFile.WriteLine( "" );

			for ( int nIdx = 0; nIdx < map.GetSize(); nIdx++ )
			{
				CSPOSKeyboardKeyExtent KeyExtent = map.GetAt( nIdx );

				int nKeyIdx;
				if ( SPOSKeyboardManager.GetListManager() -> FindKey( ListRecord.GetListNo(), nPageNo, KeyExtent.m_nKeyNo, nKeyIdx ) == TRUE )
				{
					CSPOSKeyboardKeyRecord KeyRecord;
					SPOSKeyboardManager.GetListManager() -> GetKeyRecord( nKeyIdx, KeyRecord );

					CString strPos;
					strPos.Format( "%d , %d",
						KeyExtent.m_nRowStart,
						KeyExtent.m_nColStart );

					CString strSize;
					strSize.Format( "%d x %d",
						KeyExtent.m_nRowSize,
						KeyExtent.m_nColSize );

					CString strType = "";
			
					CString strNum;
					strNum.Format( "%I64d", KeyRecord.GetValue() );

					switch( KeyRecord.GetType() )
					{
					case KEY_TYPE_EMPTY:
						strType = "EMPTY";
						strNum = "";
						break;

					case KEY_TYPE_FUNCTION:		
						strType = "FUNCTION";
						break;

					case KEY_TYPE_PAYMENT:
						strType = "PAYMENT";
						break;

					case KEY_TYPE_MODIFIER:
						strType = "MODIFIER";
						break;

					case KEY_TYPE_CONTROL:
						strType = "CONTROL";
						break;

					case KEY_TYPE_LIST:
						strType = "LIST";
						break;

					case KEY_TYPE_SVRPRESET:	
						strType = "SVR PRESET";
						break;

					case KEY_TYPE_MACRO:
						strType = "MACRO";
						break;

					case KEY_TYPE_PLUNUMBER:	
						strType = "PLU";
						break;
					}

					CSPOSKeyboardKeyHelpers KeyHelpers;
					CString strText;
					KeyHelpers.GetDefaultText( KeyRecord.GetType(), KeyRecord.GetValue(), strText, FALSE );
					CString strContent = strText;
					
					CString strKeyText = KeyRecord.GetKeyText();
					
					CCSV csv('\t');
					csv.Add( nPageNo );
					csv.Add( strPos );
					csv.Add( strSize );
					csv.Add( strType );
					csv.Add( strNum );
					csv.Add( strContent );
					csv.Add( strKeyText );
					ReportFile.WriteLine( csv.GetLine() );

					if ( KeyRecord.GetType() == KEY_TYPE_LIST )
					{
						__int64 nListNo = KeyRecord.GetValue();
						
						int nListIdx;
						if ( SPOSKeyboardManager.GetListManager() -> FindList( nListNo, nListIdx ) == TRUE )
						{
							CSPOSKeyboardListRecord ListRecord;
							SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx, ListRecord );

							int nNextListNo = ListRecord.GetNextList();

							if ( nNextListNo != 0 )
							{
								CSPOSKeyboardKeyHelpers KeyHelpers;
								CString strText;
								KeyHelpers.GetDefaultText( KEY_TYPE_LIST, nNextListNo, strText, FALSE );
								CString strContent = ">>> ";
								strContent += strText;
					
								CCSV csv('\t');
								csv.Add( "" );
								csv.Add( "" );
								csv.Add( ">>> NEXT LIST" );
								csv.Add( nNextListNo );
								csv.Add( strContent );
								ReportFile.WriteLine( csv.GetLine() );
							}
						}
					}
				}
			}
		}
		ReportFile.WriteLine( "" );
		ReportFile.WriteLine( "" );
	}
}

/**********************************************************************/
