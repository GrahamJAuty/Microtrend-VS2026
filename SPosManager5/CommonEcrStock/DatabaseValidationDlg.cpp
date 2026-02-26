/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "LocationCSVArray.h"
#include "MessageLogger.h"
#include "NetworkCSVArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "DatabaseValidationDlg.h"
/**********************************************************************/

CDatabaseValidationDlg::CDatabaseValidationDlg( bool bFullValidation, CWnd* pParent)
	: CDialog(CDatabaseValidationDlg::IDD, pParent)
{
	m_bFullValidation = bFullValidation;
	m_bValidAtStartup = TRUE;
	m_bCancelAtStartup = FALSE;
	m_strError = "";
}

/**********************************************************************/

void CDatabaseValidationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDatabaseValidationDlg)
	DDX_Control(pDX, IDC_STATIC_INFO, m_staticInfo);
	DDX_Control(pDX, IDC_STATIC_INFO2, m_staticInfo2);
    DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDatabaseValidationDlg, CDialog)
	//{{AFX_MSG_MAP(CDatabaseValidationDlg)
	//}}AFX_MSG_MAP
	ON_MESSAGE ( WM_APP + 1, RunValidation)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDatabaseValidationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Progress.SetRange(0, 100);
	m_Progress.SetPos(0);
	m_nProgressPos = 0;

	CString strTitle = "";
	strTitle += "Validating ";
	strTitle += AfxGetAppName();
	strTitle += " Product Files";
	SetWindowText(strTitle);

	m_staticInfo.SetWindowText("");
	m_staticInfo2.SetWindowText("");

	PostMessage(WM_APP + 1, 0, 0);

	return TRUE;
}

/**********************************************************************/

long CDatabaseValidationDlg::RunValidation(WPARAM wIndex, LPARAM lParam)
{
	m_bValidAtStartup = TRUE;

	for (int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++)
	{
		if (RunInternal(nDbIdx) == FALSE)
		{
			m_bValidAtStartup = FALSE;
			EndDialog(IDCANCEL);
			return 0l;
		}
	}

	if ((TRUE == m_bFullValidation) && (DealerFlags.GetSpecialChangesOnlyFlag() == TRUE))
	{
		for (int nNwkIdx = 0; nNwkIdx < dbNetwork.GetSize(); nNwkIdx++)
		{
			if (ValidateLocationPluFiles(nNwkIdx) == FALSE)
			{
				m_bValidAtStartup = FALSE;
				EndDialog(IDCANCEL);
				return 0l;
			}
		}
	}

	EndDialog(IDCANCEL);
	return 0L;
}

/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/

bool CDatabaseValidationDlg::RunInternal(int nDbIdx)
{
	//FIND OUT TOTAL PROGRESS SIZE
	__int64 nTarget = 0;
	__int64 nProgress = 0;

	CString strPluFilename = DataManager.GetFilePathPlu(nDbIdx);
	{
		CSSFile filePlu;
		filePlu.Open(strPluFilename, "rb");
		nTarget += filePlu.GetLength();
	}

	SetInfoText(nDbIdx, 1);
	return (ValidatePluFile(strPluFilename, nProgress, nTarget, 1, dbDatabase.GetDbNo(nDbIdx), 0));
}
	
/**********************************************************************/
#else
/**********************************************************************/

bool CDatabaseValidationDlg::RunInternal( int nDbIdx )
{
	//FIND OUT TOTAL PROGRESS SIZE
	__int64 nTarget = 0;
	__int64 nProgress = 0;

	SetInfoText( nDbIdx, 6 );

	CString strPluFilename = DataManager.GetFilePathPlu( nDbIdx );
	CString strRecipeFilename = DataManager.GetFilePathRecipe( nDbIdx );
	CString strStockFilename = DataManager.GetFilePathStock( nDbIdx );
		
	{
		CSSFile filePlu;
		filePlu.Open( strPluFilename, "rb" );
		nTarget += filePlu.GetLength();
	}

	{
		CSSFile fileRecipe;
		fileRecipe.Open( strRecipeFilename, "rb" );
		nTarget += fileRecipe.GetLength();
	}

	{
		CSSFile fileStock;
		fileStock.Open( strStockFilename, "rb" );
		nTarget += fileStock.GetLength();
	}
		
	CSupplierCSVArray LocalSupplierArray;
	CString strSupplierFilename = DataManager.GetFilePathSupplier();
	LocalSupplierArray.Open( strSupplierFilename, DB_READONLY );

	CCustomFieldValueCSVArray LocalCustomField[ MAX_ITEM_CUSTOM_FIELDS + 1 ];
	for ( int nFieldNo = 1; nFieldNo <= MAX_ITEM_CUSTOM_FIELDS; nFieldNo++ )
	{
		CString strFilename = DataManager.GetFilePathCustomFieldValue( nDbIdx, nFieldNo );
		LocalCustomField[nFieldNo].Open( strFilename, DB_READONLY );
		LocalCustomField[nFieldNo].PullNextNumberFromArray();
	}
	
	int nStartSpIdx, nEndSpIdx;
	dbStockpoint.GetSpIdxRange( nDbIdx, nStartSpIdx, nEndSpIdx );

	if ( TRUE == m_bFullValidation )
	{
		SetInfoText( nDbIdx, 6 );

		for ( int nSpIdx = nStartSpIdx; nSpIdx <= nEndSpIdx; nSpIdx++ )
		{
			SetProgressPos( nSpIdx - nStartSpIdx, nEndSpIdx + 1 - nStartSpIdx, 0, 100 );

			CSSFile fileLevels;
			CFilenameUpdater FnUp( SysFiles::StockLevels, nSpIdx );
			fileLevels.Open( FnUp.GetFilenameToUse(), "rb" );
			nTarget += fileLevels.GetLength();
		}
	}

	m_nProgressPart = -1;
	SetInfoText( nDbIdx, 1 );
	if (ValidatePluFile(strPluFilename, nProgress, nTarget, 1, dbDatabase.GetDbNo(nDbIdx), 0) == FALSE)
	{
		return FALSE;
	}

	m_nProgressPart = -1;
	SetInfoText( nDbIdx, 2 );
	if (ValidatePluFile(strRecipeFilename, nProgress, nTarget, 2, dbDatabase.GetDbNo(nDbIdx), 0) == FALSE)
	{
		return FALSE;
	}

	m_nProgressPart = -1;
	SetInfoText( nDbIdx, 3 );
	if (ValidateStockFile(strStockFilename, nProgress, nTarget, 3, dbDatabase.GetDbNo(nDbIdx), 0) == FALSE)
	{
		return FALSE;
	}

	if ( TRUE == m_bFullValidation )
	{
		m_nProgressPart = -1;
		for (int nSpIdx = nStartSpIdx; nSpIdx <= nEndSpIdx; nSpIdx++)
		{
			SetInfoText(nSpIdx, 4);

			CFilenameUpdater FnUp(SysFiles::StockLevels, nSpIdx);
			CString	strLevelsFilename = FnUp.GetFilenameToUse();

			if (ValidateStockFile(strLevelsFilename, nProgress, nTarget, 4, dbStockpoint.GetDbNo(nSpIdx), dbStockpoint.GetSpNo(nSpIdx)) == FALSE)
			{
				return FALSE;
			}
		}
	}
	
	LocalSupplierArray.ValidateSuppliers();
	if ( LocalSupplierArray.GetErrorType() != 0 )
	{
		LogError( 0, 0, 6, LocalSupplierArray.GetErrorType(), LocalSupplierArray.GetErrorLine(), strSupplierFilename );
		return FALSE;
	}

	for ( int nFieldNo = 1; nFieldNo <= MAX_ITEM_CUSTOM_FIELDS; nFieldNo++ )
	{
		LocalCustomField[nFieldNo].ValidateFieldValues();

		if ( LocalCustomField[nFieldNo].GetErrorType() != 0 )
		{
			CString strFilename = DataManager.GetFilePathCustomFieldValue( nDbIdx, nFieldNo );
			LogError( dbDatabase.GetDbNo( nDbIdx ), nFieldNo, 7, LocalCustomField[nFieldNo].GetErrorType(), LocalCustomField[nFieldNo].GetErrorLine(), strFilename );
			return FALSE;
		}
	}
	
	return TRUE;
}

/**********************************************************************/
#endif
/**********************************************************************/

bool CDatabaseValidationDlg::ValidateLocationPluFiles(int nNwkIdx)
{
	//FIND OUT TOTAL PROGRESS SIZE
	__int64 nTarget = 0;
	__int64 nProgress = 0;

	SetInfoText(nNwkIdx, 6);

	int nStartIdx, nEndIdx;
	dbLocation.GetNetworkLocIdxRange(nNwkIdx, nStartIdx, nEndIdx);

	for (int nLocIdx = nStartIdx; nLocIdx <= nEndIdx; nLocIdx++)
	{
		SetProgressPos(nLocIdx - nStartIdx, nEndIdx + 1 - nStartIdx, 0, 100);

		CSSFile filePlu;
		filePlu.Open(dbLocation.GetFilePathSyssetPluEcr(nLocIdx), "rb");
		nTarget += filePlu.GetLength();
	}

	m_nProgressPart = -1;
	for (int nLocIdx = nStartIdx; nLocIdx <= nEndIdx; nLocIdx++)
	{
		SetInfoText(nLocIdx, 5);
		CString strPluFilename = dbLocation.GetFilePathSyssetPluEcr(nLocIdx);

		if (ValidatePluFile(strPluFilename, nProgress, nTarget, 5, dbLocation.GetNetworkNo(nLocIdx), dbLocation.GetLocNo(nLocIdx)) == FALSE)
		{
			return FALSE;
		}
	}

	return TRUE;
}

/**********************************************************************/

bool CDatabaseValidationDlg::ValidatePluFile(CString& strFilename, __int64& nProgress, __int64 nTarget, int nContext, int nEntityNo1, int nEntityNo2)
{
	int nPluIdx = 0;
	__int64 nLastPluNo = 0;
	int nMaxBaseLen = SysInfo.GetMaxBasePluLen();

	CSSFile filePlu;
	filePlu.Open(strFilename, "rb");

	CString strBuffer = "";

	while (filePlu.ReadString(strBuffer) == TRUE)
	{
		if (((nPluIdx + 1) % 100) == 0)
		{
			SetProgressPos(nProgress + filePlu.Tell(), nTarget, 0, 100);
			PumpWaitingMessages();
		}

		CCSV csv(strBuffer, ',', '"', 1);
		CString strThisPluNo = csv.GetString(0);
		__int64 nThisPluNo = _atoi64(strThisPluNo);

		int nError = 0;
		if ((nThisPluNo < 1) || (nThisPluNo > Super.MaxPluInt()))
		{
			nError = 1;
		}
		else if (::TestNumeric(strThisPluNo) == FALSE)
		{
			nError = 2;
		}
		else if (nThisPluNo == nLastPluNo)
		{
			nError = 3;
		}
		else if (nThisPluNo < nLastPluNo)
		{
			nError = 4;
		}
		else if ((0 != nMaxBaseLen) && (strThisPluNo.GetLength() == nMaxBaseLen + 1))
		{
			nError = 5;

			if (SysInfo.GetMaxBasePluLen() == 6)
			{
				if (strThisPluNo.Left(5) == "99999")
				{
					nError = 0;
				}
			}
		}

		if (nError != 0)
		{
			LogError(nEntityNo1, nEntityNo2, nContext, nError, nPluIdx, strFilename);
			return FALSE;
		}

		nLastPluNo = nThisPluNo;
		nPluIdx++;
	}

	nProgress += filePlu.GetLength();
	return TRUE;
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

bool CDatabaseValidationDlg::ValidateStockFile( CString& strFilename, __int64& nProgress, __int64 nTarget, int nContext, int nEntityNo1, int nEntityNo2 )
{
	int nStockIdx = 0;
	CString strLastStockCode = "";
	
	CSSFile fileStock;
	fileStock.Open( strFilename, "rb" );

	CString strBuffer = "";
	while( fileStock.ReadString( strBuffer ) == TRUE )
	{
		if ( ( ( nStockIdx + 1 ) % 100 ) == 0 )
		{
			SetProgressPos( nProgress + fileStock.Tell(), nTarget, 0, 100 );
			PumpWaitingMessages();
		}

		CCSV csv ( strBuffer, ',', '"', 1 );
		CString strThisStockCode = csv.GetString(0);

		int nError = 0;
		if (strThisStockCode.GetLength() > Stock::StockCode.Max)
		{
			nError = 2;
		}
		else if (strThisStockCode == strLastStockCode)
		{
			nError = 3;
		}
		else if (strThisStockCode < strLastStockCode)
		{
			nError = 4;
		}

		if ( nError != 0 )
		{
			LogError( nEntityNo1, nEntityNo2, nContext, nError, nStockIdx, strFilename );
			return FALSE;
		}
			
		strLastStockCode = strThisStockCode;
		nStockIdx++;
	}
	nProgress += fileStock.GetLength();
	return TRUE;
}

/**********************************************************************/
#endif
/**********************************************************************/

void CDatabaseValidationDlg::SetProgressPos( __int64 nProgress, __int64 nTarget, int nOffset, int nMultiplyBy )
{
	if (nTarget == 0)
	{
		return;
	}

	int nPos = int ( ( nProgress * nMultiplyBy ) / nTarget ) + nOffset;
	if ( nPos < 0 || nPos > 100 ) nPos = 0;
	
	if ( nPos != m_nProgressPos )
	{
		m_nProgressPos = nPos;
		m_Progress.SetPos(nPos);
	}
}

/**********************************************************************/

void CDatabaseValidationDlg::SetProgressPos( __int64 nProgress, __int64 nTarget )
{
	if (nTarget == 0)
	{
		return;
	}

	int nPos = int ( ( nProgress * 100 ) / nTarget );
	if ( nPos < 0 || nPos > 100 ) nPos = 0;
	
	if ( nPos != m_nProgressPos )
	{
		m_nProgressPos = nPos;
		m_Progress.SetPos(nPos);
	}
}

/**********************************************************************/

void CDatabaseValidationDlg::SetProgressPos( CSSFile& file )
{
	int nPos = file.GetPercentPosition();
	
	if ( nPos != m_nProgressPos )
	{
		m_nProgressPos = nPos;
		m_Progress.SetPos(nPos);
	}
}

/**********************************************************************/

void CDatabaseValidationDlg::SetInfoText( int nDbIdx, int nType )
{
	if ( nType != m_nProgressType )
	{
		CString strText = "";
		switch ( nType )
		{
		case 1:	
			strText = "Checking Plu File";	
			break;

		case 2:	
			strText = "Checking Recipe File";
			break;

		case 3:	
			strText = "Checking Stock File";
			break;

		case 4:	
			strText = "Checking Stock Levels";
			break;

		case 5:	
			strText = "Checking Ecr Plu File";	
			break;

		case 6:	
			strText = "Checking File Sizes";		
			break;

		default:	
			strText = "";
			break;
		}
		
		m_staticInfo.SetWindowText( strText );
		m_nProgressType = nType;
	}

	if ( nDbIdx != m_nProgressPart )
	{
		CString strDbName = "";
		switch ( m_nProgressType )
		{
		case 1:
		case 2:
		case 3:
			strDbName.Format ( "(%s)", dbDatabase.GetName(nDbIdx) );
			break;

#ifdef STOCKMAN_SYSTEM
		case 4:
			strDbName.Format ( "(%s)", dbStockpoint.GetName(nDbIdx) );
			break;
#endif
	
		case 5:
			strDbName.Format ( "(%s)", dbLocation.GetName(nDbIdx) );
			break;
		}

		m_staticInfo2.SetWindowText( strDbName );
		m_nProgressPart = nDbIdx;
	}
}

/**********************************************************************/

void CDatabaseValidationDlg::LogError( int nDbNo, int nSpNo, int nFileType, int nError, int nIndex, const char* szFilename )
{ 
	switch( nFileType )
	{
	case 4:
	case 5:
	case 7:
		{
			m_strError.Format ( "%3.3d-%3.3d-%1.1d%2.2d-%6.6X",
				nDbNo,
				nSpNo,
				nFileType,
				nError,
				nIndex );
		}
		break;

	default:
		{
			m_strError.Format ( "%3.3d-%1.1d%2.2d-%6.6X",
				nDbNo,
				nFileType,
				nError,
				nIndex );
		}
		break;
	}

	CString strMsg;
	strMsg.Format ( "Invalid Product File (%s)",
		(const char*) m_strError );

	CMessageLoggerInfo info;
	info.m_strMessage = strMsg;
	info.m_nHideLevel = 2;
	
	MessageLogger.LogSchedulerMessage( info, FALSE, TRUE, FALSE );

	CSSFile fileError;
	CFilenameUpdater FnUp( SysFiles::ValidationErrors );
	fileError.Open( FnUp.GetFilenameToUse(), "ab" );
	{
		CCSV csv;
		csv.Add( m_strError );
		csv.Add( szFilename );
		fileError.WriteLine( csv.GetLine() );
	}
}

/**********************************************************************/

void CDatabaseValidationDlg::PumpWaitingMessages()
{
	MSG msg;
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if(!IsDialogMessage(&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg); 
		}
	}
}

/**********************************************************************/

void CDatabaseValidationDlg::OnOK() 
{
	OnCancel();
}

/**********************************************************************/

void CDatabaseValidationDlg::OnCancel() 
{
	if ( FALSE == m_bCancelAtStartup )
	{
		m_bCancelAtStartup = TRUE;
		m_bValidAtStartup = FALSE;
		EndDialog( IDCANCEL );
	}
}

/**********************************************************************/