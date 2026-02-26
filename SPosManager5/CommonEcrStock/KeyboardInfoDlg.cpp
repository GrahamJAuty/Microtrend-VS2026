/**********************************************************************/
 
/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "SPOSVersions.h"
//#include "globalfunctions.h"
#include "KeyboardSetCSVArray.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
/**********************************************************************/
#include "KeyboardInfoDlg.h"
/**********************************************************************/

CKeyboardInfoDlg::CKeyboardInfoDlg( CDataNodeInfoKeyboard& infoNode, CString& strName, CWnd* pParent )
	: CDialog(CKeyboardInfoDlg::IDD, pParent), m_infoNode( infoNode ), m_strName( strName )
{
	//{{AFX_DATA_INIT(CKeyboardInfoDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CKeyboardInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyboardInfoDlg)
	DDX_Control(pDX, IDC_EDIT_MODIFIED, m_editModified);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 20);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CKeyboardInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CKeyboardInfoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CKeyboardInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strTitle;
	CString strPath;

	if ( NODE_LOCATION == m_infoNode.m_nNodeType )
	{
		strTitle.Format( "Edit Keyboard Name (%s, Kbd %2.2d)",
			dbLocation.GetName( m_infoNode.m_nLocIdx ),
			m_infoNode.m_nKbNo );

		strPath = dbLocation.GetFolderPathDataKeyboard ( m_infoNode.m_nLocIdx, m_infoNode.m_nKbNo );
	}
	else
	{
		strTitle.Format( "Edit Keyboard Name (%s, %s, Kbd %2.2d)",
			dbDatabase.GetName( m_infoNode.m_nDbIdx ),
			dbKeyboardSet.GetName( m_infoNode.m_nKbsIdx ),
			m_infoNode.m_nKbNo );

		strPath = dbKeyboardSet.GetFolderPathKeyboard ( m_infoNode.m_nKbsIdx, m_infoNode.m_nKbNo );
	}

	SetWindowText( strTitle );

	CString strDate = "";
	
	if ( ::ExistSubdirectory ( strPath ) == TRUE )
	{
		CCSV csvFiles;

		if ( CSPOSVersions::CheckSPOSVersion( m_infoNode.m_nSPOSVersion, SPOS_V4 ) == TRUE )
		{
			csvFiles.Add( "\\KbdV4.sql" );
			csvFiles.Add( "\\KbdV3_4.sql" );
			csvFiles.Add( "\\KbdV3.sql" );
		}
		
		for ( int n = 0; n < csvFiles.GetSize(); n++ )
		{
			CString strFilename = strPath + csvFiles.GetString(n);

			if ( ::FileExists ( strFilename ) == TRUE )	
			{
				CFileStatus FileStatus;
				CFile::GetStatus ( strFilename, FileStatus );
				CTime time = FileStatus.m_mtime;				
				strDate = time.Format ( "%d/%m/%Y at %H:%M:%S" );
				break;
			}
		}
	}
	
	m_editModified.SetWindowText( strDate );	
	m_editName.SetWindowText( m_strName );
	
	return TRUE;
}

/**********************************************************************/

void CKeyboardInfoDlg::OnOK() 
{
	if ( UpdateData( TRUE ) == TRUE )
		EndDialog( IDOK );
}

/**********************************************************************/
