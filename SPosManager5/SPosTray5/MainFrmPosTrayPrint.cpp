/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//#include "DealerFlags.h"
#include "FileRemover.h"
//#include "globalfunctions.h"
 
#include "RepSpawn.h"
/**********************************************************************/
#include "MainFrmPosTrayPrint.h"
/**********************************************************************/

IMPLEMENT_DYNAMIC(CMainFramePosTrayPrint, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFramePosTrayPrint, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFramePosTrayPrint)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

CMainFramePosTrayPrint::CMainFramePosTrayPrint()
{
	m_hIcon = AfxGetApp()->LoadIcon ( IDR_MAINFRAME_POSTRAY_PRINT );
}

/**********************************************************************/

CMainFramePosTrayPrint::~CMainFramePosTrayPrint()
{
}

/**********************************************************************/

int CMainFramePosTrayPrint::OnCreate ( LPCREATESTRUCT lpCreateStruct )
{
	return CFrameWnd::OnCreate(lpCreateStruct);
}

/**********************************************************************/

void CMainFramePosTrayPrint::PrintReports()
{
	CString strPrintFolder = "PosTray\\Print";
	::GetSyssetProgramPath( strPrintFolder );
	strPrintFolder += "\\";

	CStringArray arrayFilesToDelete;
	CStringArray arrayFilesToPrint;

	CFileFind FileFinder;	
	bool bWorking = ( FileFinder.FindFile ( strPrintFolder + "*.*" ) != 0 );

	int nFileCount = 0;
	while ( ( bWorking ) && ( nFileCount < 100 ) )   
	{
		( bWorking = FileFinder.FindNextFile() != 0 );

		if ( FileFinder.IsDirectory() == TRUE )
			continue;

		if ( FileFinder.IsDots() == TRUE )
			continue;

		CString strFilename = FileFinder.GetFileName();
		strFilename.MakeUpper();

		bool bValidFilename = TRUE;
		
		if ( strFilename.GetLength() != 31 )
			bValidFilename = FALSE;
		else if ( ::TestNumeric( strFilename.Left(8) ) == FALSE )
			bValidFilename = FALSE;
		else if ( strFilename.Mid(8,1) != "_" )
			bValidFilename = FALSE;
		else if ( ::TestNumeric( strFilename.Mid(9,6) ) == FALSE )
			bValidFilename = FALSE;
		else if ( strFilename.Mid(15,1) != "_" )
			bValidFilename = FALSE;
		else if ( ::TestNumeric( strFilename.Mid(16,4) ) == FALSE )
			bValidFilename = FALSE;
		else if ( strFilename.Mid(20,1) != "_" )
			bValidFilename = FALSE;
		else if ( ::TestNumeric( strFilename.Mid(21,6) ) == FALSE )
			bValidFilename = FALSE;

		if ( FALSE == bValidFilename )
			arrayFilesToDelete.Add( strFilename );
		else if ( strFilename.Right(4) == ".REP" )
			arrayFilesToPrint.Add( strFilename.Left(27) );
		else if ( strFilename.Right(4) != ".DAT" )
			arrayFilesToDelete.Add( strFilename );

		nFileCount++;
	}

	for ( int n = 0; n < arrayFilesToDelete.GetSize(); n++ )
		CFileRemover FileRemover( strPrintFolder + arrayFilesToDelete.GetAt(n) );

	arrayFilesToDelete.RemoveAll();

	for ( int n = 0; n < arrayFilesToPrint.GetSize(); n++ )
	{
		CReportManager ReportManager;
		
		CString strReportFilename = arrayFilesToPrint.GetAt(n);
		CString strParamsFilename = "";
		CString strKey = "";

		{
			CSSFile fileHelper;
			if ( fileHelper.Open( strPrintFolder + strReportFilename + ".dat", "rb" ) == TRUE )
			{
				fileHelper.ReadString( strParamsFilename );
				fileHelper.ReadString( strKey );
			}
		}

		if ( ReportManager.PrintSilent( strPrintFolder + strReportFilename + ".rep", strParamsFilename, strKey, NULL ) == TRUE )
		{
			CFileRemover FileRemover;
			FileRemover.RemoveFile( strPrintFolder + strReportFilename + ".rep" );
			FileRemover.RemoveFile( strPrintFolder + strReportFilename + ".dat" );
		}
	}

	if ( DealerFlags.GetPrinterRedirectMode() == 2 )
	{
		if ( arrayFilesToPrint.GetSize() != 0 )
		{
			CString strMsg;
			strMsg.Format ( "PosTray AutoPrint redirected to display (%d)", GetTickCount() );
			Prompter.Info( strMsg );
		}
	}
}

/**********************************************************************/