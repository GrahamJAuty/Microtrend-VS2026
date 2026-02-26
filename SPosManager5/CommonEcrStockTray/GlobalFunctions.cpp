/**********************************************************************/
#include <io.h>
#include <ckglobal.h>
/**********************************************************************/
#include "..\SPosEcrManager5\PluFilterDlgEcrman.h"
/**********************************************************************/
#include "..\SPosStockManager5\PluFilterDlgStockman.h"
/**********************************************************************/
#include "CashRSPImporter.h"
#include "FileRemover.h"
#include "FontTable.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "PluCSVArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "GlobalFunctions.h"
/**********************************************************************/

bool FileExists ( const char* szFile )
{
	return ( _access ( szFile, 0 ) == 0 );
}

/**********************************************************************/

void TrimSpaces ( CString& string, bool bRemoveLeadingZeros )
{
	string.TrimLeft();
	string.TrimRight();
	if (bRemoveLeadingZeros == TRUE)
	{
		string.TrimLeft("0");
	}
}

/**********************************************************************/

void CreateSubdirectory(const char* szDir)
{
	CString strDir = szDir;
	if (::ExistSubdirectory(strDir) == FALSE)
	{
		::MakeSubdirectory(strDir);
	}
}

/**********************************************************************/

void CreateDataSubdirectory(const char* szDir)
{
	CString strDir = szDir;
	GetDataProgramPath(strDir);

	if (::ExistSubdirectory(strDir) == FALSE)
	{
		::MakeSubdirectory(strDir);
	}
}

/**********************************************************************/

void CreateSyssetSubdirectory ( const char* szDir )
{
	CString strDir = szDir;
	GetSyssetProgramPath( strDir );
	
	if (::ExistSubdirectory(strDir) == FALSE)
	{
		::MakeSubdirectory(strDir);
	}
}

/**********************************************************************/

bool TestNumeric ( const CString& strText, int nMaxLen )
{
	if ((nMaxLen > 0) && ((strText.GetLength() == 0) || (strText.GetLength() > nMaxLen)))
	{
		return FALSE;
	}

	for (int nPos = 0; nPos < strText.GetLength(); nPos++)
	{
		char c = strText.GetAt(nPos);
		if (c < '0' || c > '9')
		{
			return FALSE;
		}
	}

	return TRUE;
}

/**********************************************************************/

bool TestNumericHex(const CString& strText, int nMaxLen)
{
	if ((nMaxLen > 0) && ((strText.GetLength() == 0) || (strText.GetLength() > nMaxLen)))
	{
		return FALSE;
	}

	for (int nPos = 0; nPos < strText.GetLength(); nPos++)
	{
		char c = strText.GetAt(nPos);

		if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
		{
			continue;
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}

/**********************************************************************/

bool GetStringFromOleDate( COleDateTime& date, CString& strDate )
{
	strDate = "";
	if (date.GetStatus() != COleDateTime::valid)
	{
		return FALSE;
	}

	strDate.Format ( "%4.4d%2.2d%2.2d", 
		date.GetYear(), 
		date.GetMonth(), 
		date.GetDay() );

	return TRUE;
}

/**********************************************************************/

bool GetStringFromOleDateTime(COleDateTime& dateTime, CString& strDateTime)
{
	strDateTime = "";
	if (dateTime.GetStatus() != COleDateTime::valid)
	{
		return FALSE;
	}

	strDateTime.Format("%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d", dateTime.GetYear(), dateTime.GetMonth(), dateTime.GetDay(), dateTime.GetHour(), dateTime.GetMinute(), dateTime.GetSecond());
	return TRUE;
}

/**********************************************************************/

bool GetOleDateFromString(CString& strDate, COleDateTime& date)
{
	if (strDate.GetLength() != 8)
	{
		return FALSE;
	}

	if (::TestNumeric(strDate) == FALSE)
	{
		return FALSE;
	}

	date = COleDateTime(atoi(strDate.Left(4)), atoi(strDate.Mid(4, 2)), atoi(strDate.Right(2)), 0, 0, 0);

	return date.GetStatus() == COleDateTime::valid;
}

/**********************************************************************/

bool GetOleDateTimeFromString(CString& strDateTime, COleDateTime& dateTime)
{
	if (strDateTime.GetLength() != 14)
	{
		return FALSE;
	}

	if (::TestNumeric(strDateTime) == FALSE)
	{
		return FALSE;
	}

	dateTime = COleDateTime(atoi(strDateTime.Left(4)), atoi(strDateTime.Mid(4, 2)), atoi(strDateTime.Mid(6, 2)), atoi(strDateTime.Mid(8, 2)), atoi(strDateTime.Mid(10, 2)), atoi(strDateTime.Right(2)));

	return dateTime.GetStatus() == COleDateTime::valid;
}

/**********************************************************************/

void GetReportStringFromOleDate(const COleDateTime date, CString& strDate)
{
	strDate = "";
	if (date.GetStatus() == COleDateTime::valid)
	{
		strDate.Format("%2.2d/%2.2d/%4.4d", date.GetDay(), date.GetMonth(), date.GetYear());
	}
}

/**********************************************************************/

void GetDataProgramPath(CString& strPath)
{
	CString strTemp = Sysset.GetProgramPath();
	strTemp += "\\";
	strTemp += strPath;
	strPath = strTemp;
}

/**********************************************************************/

void GetSyssetProgramPath(CString& strPath)
{
	CString strTemp = Sysset.GetSyssetProgramPath();
	strTemp += "\\";
	strTemp += strPath;
	strPath = strTemp;
}

/**********************************************************************/

void GetTempPath(CString& strPath)
{
	CString strTemp = Sysset.GetTempPath();
	strTemp += "\\";
	strTemp += strPath;
	strPath = strTemp;
}

/**********************************************************************/

void GetPCPathHost(CString& strPath)
{
	CString strTemp = Sysset.GetPCPathHost();
	strTemp += "\\";
	strTemp += strPath;
	strPath = strTemp;
}

/**********************************************************************/

void GetPCPathClient(CString& strPath)
{
	CString strTemp = Sysset.GetPCPathClient();
	strTemp += "\\";
	strTemp += strPath;
	strPath = strTemp;
}

/**********************************************************************/

bool IsBeforeStartOfDay(COleDateTime& time)
{
	if (time.GetHour() < EcrmanOptions.GetReportsDayStartHour())
	{
		return TRUE;
	}

	if (time.GetHour() > EcrmanOptions.GetReportsDayStartHour())
	{
		return FALSE;
	}

	if (time.GetMinute() < EcrmanOptions.GetReportsDayStartMinute())
	{
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

CString FilterEposText ( const char* szText )
{
	CString strEposText;

	for (int i = 0; i < (int)strlen(szText); i++)
	{
		if (szText[i] >= 0x20 && szText[i] < 0x7f)
		{
			strEposText += szText[i];
		}
	}

	return strEposText;
}

/**********************************************************************/

void GetTermSaleFilename( int nCashRSPMode, int nTerminalNo, CString& strFilename )
{
	CString strExt = "";
	if (nTerminalNo >= 1)
	{
		strExt.Format(".%3.3d", nTerminalNo);
	}
	else
	{
		strExt = "";
	}

	strFilename = "";
	switch( nCashRSPMode )
	{
	case CASHRSP_SALES_PENDING:		
		strFilename = "TERM";		
		break;

	case CASHRSP_OBSOLETE_TERMSAVE:
		strFilename = "TERMSAVE";	
		break;

	case CASHRSP_SALES_TRADING:	
		strFilename = "TRMSAVE1";
		break;

	case CASHRSP_OBSOLETE_TRMSAVE2:	
		strFilename = "TRMSAVE2";	
		break;

	case CASHRSP_SALES_NONDUPLICATE:	
		strFilename = "TRMSAVE3";	
		break;

	case CASHRSP_SALES_TIMEATTEND:
		strFilename = "TIMESAVE";
		break;

	case CASHRSP_SALES_PMS:		
		strFilename = "PMSTERM";
		break;

	case CASHRSP_SALES_DEPOSIT:	
		strFilename = "TERMDPST";
		break;

	case CASHRSP_SALES_EOD:				
		strFilename = "TERMEOD";	
		break;
	}

	strFilename += strExt;
}

/**********************************************************************/

CString GetLastRasCommsErrorMessage()
{
	LPVOID lpMessageBuffer{};

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	// the user default language
		(LPTSTR)&lpMessageBuffer,
		0,
		NULL);

	CString strError = (LPTSTR)lpMessageBuffer;
	strError.TrimRight("\r\n");

	LocalFree(lpMessageBuffer);

	return strError;
}

/**********************************************************************/

int HexToInt ( const char* szHex )
{
	int nResult = 0;

	CString strHex = szHex;
	::TrimSpaces ( strHex, TRUE );

	for (int nPos = 0; nPos < strHex.GetLength(); nPos++)
	{
		nResult *= 16;

		char c = strHex.GetAt(nPos);

		if (c >= '0' && c <= '9')
		{
			nResult += c - '0';
		}
		else if (c >= 'A' && c <= 'F')
		{
			nResult += (c + 10 - 'A');
		}
		else if (c >= 'a' && c <= 'f')
		{
			nResult += (c + 10 - 'a');
		}
	}

	return nResult;
}

/**********************************************************************/

__int64 HexToInt64 ( const char* szHex )
{
	__int64 nResult = 0;

	CString strHex = szHex;
	::TrimSpaces ( strHex, TRUE );

	for ( int nPos = 0; nPos < strHex.GetLength(); nPos++ )
	{
		nResult *= 16;

		char c = strHex.GetAt ( nPos );

		if (c >= '0' && c <= '9')
		{
			nResult += c - '0';
		}

		if (c >= 'A' && c <= 'F')
		{
			nResult += (c + 10 - 'A');
		}
	}
	return nResult;
}

/**********************************************************************/

void FormatPluNo(__int64 nPluNo, CString& strPluNo, bool bNoEan, bool bAllowZero)
{
	int nMinPluNo = (bAllowZero) ? 0 : 1;
	if ((nPluNo >= nMinPluNo) && (nPluNo <= Super.MaxPluInt()))
	{
		if (FALSE == bNoEan)
		{
			strPluNo.Format("%*.*I64d",
				Super.MaxPluLen(),
				Super.MaxPluLen(),
				nPluNo);
		}
		else
		{
			strPluNo.Format("%*.*I64d",
				SysInfo.GetMaxBasePluLen(),
				SysInfo.GetMaxBasePluLen(),
				nPluNo);
		}
	}
	else
	{
		strPluNo = "";
	}
}

/**********************************************************************/

void FormatBarcodeNo(__int64 nBarcodeNo, CString& strBarcodeNo)
{
	if ((nBarcodeNo >= 1) && (nBarcodeNo <= Super.MaxBarcodeInt()))
	{
		strBarcodeNo.Format("%*.*I64d",
			Super.MaxBarcodeLen(),
			Super.MaxBarcodeLen(),
			nBarcodeNo);
	}
	else
	{
		strBarcodeNo = "";
	}
}

/**********************************************************************/

void ProcessPluNo( CSQLPluNoInfo& info, bool bFiddleLongPluNo, bool bFiddleShortPluNo )
{
	info.m_nBasePluNo = 0;
	info.m_nModifier = 0;
	info.m_bModifiable = FALSE;
	info.m_nImageFileNo = 0;
	info.m_nImageFileNoLen = 0;
	info.m_bValid = FALSE;

	CString strPluNo;
	strPluNo.Format( "%I64d", info.m_nEposPluNo );

	if ( TRUE == bFiddleLongPluNo )
	{
		if ( strPluNo.GetLength() > Super.MaxPluLen() )
		{
			CString strTemp = strPluNo.Right( Super.MaxPluLen() - 1 );
			strPluNo = "9";
			strPluNo += strTemp;
			info.m_nEposPluNo = _atoi64( strPluNo );
		}
	}

	int nPluNoLen = strPluNo.GetLength();
	int nMaxBaseLen = SysInfo.GetMaxBasePluLen();

	//ALLOW SINGLE DIGIT PLU NUMBERS IF MODIFIERS ARE DISABLED
	int nMinEposPluLen = ( ( ( nMaxBaseLen == 0 ) || ( TRUE == bFiddleShortPluNo ) ) ? 1 : 2 );
	if ((nPluNoLen < nMinEposPluLen) || (nPluNoLen > Super.MaxPluLen()))
	{
		return;
	}

	if (0 == info.m_nEposPluNo)
	{
		return;
	}

	if ( ( nMaxBaseLen != 0 ) && ( nPluNoLen <= nMaxBaseLen + 1 ) )
	{
		info.m_nBasePluNo = info.m_nEposPluNo / 10;							
		info.m_nImageFileNo = info.m_nBasePluNo * 10;
		info.m_nModifier = (int) info.m_nEposPluNo % 10;
		info.m_bModifiable = TRUE;
		info.m_bValid = TRUE;
	}
	else
	{
		info.m_nBasePluNo = info.m_nEposPluNo;							
		info.m_nImageFileNo = info.m_nEposPluNo;
		info.m_nModifier = 0;
		info.m_bModifiable = FALSE;
		info.m_bValid = TRUE;
	}
	
	if ( nPluNoLen <= 8 )
		info.m_nImageFileNoLen = 8;
	else
		info.m_nImageFileNoLen = 14;
}

/**********************************************************************/

void FormatAuditReference(CString& strRef1, CString& strRef2, CString& strRef3)
{
	::TrimSpaces(strRef1, FALSE);
	::TrimSpaces(strRef2, FALSE);

	if ((strRef1 == "") && (strRef2 == ""))
	{
		strRef1 = "none";
		strRef3 = "none";
	}
	else
	{
		strRef3 = strRef1;

		if (strRef2 != "")
		{
			if (strRef3 != "")
			{
				strRef3 += " ";
			}

			strRef3 += strRef2;
		}
	}
}

/**********************************************************************/

void MicrotrendDebugMessageBox( CString& str )
{
#ifdef _DEBUG
	Prompter.Diagnostic( str );
#endif
}

/**********************************************************************/

void GetPMSRoomText( int nRoomNo, const char* szRoomName, CString& strRoomText, bool bWithNumber )
{
	CString strRoomName = szRoomName;
	strRoomName.MakeUpper();

	CString strRoomNumber;
	strRoomNumber.Format( "ROOM %d", nRoomNo );

	if ( ( strRoomName == "" ) || ( strRoomName == strRoomNumber ) )
	{
		strRoomText.Format( "Room %d",
			nRoomNo );
	}
	else
	{
		if (TRUE == bWithNumber)
		{
			strRoomText.Format("Room %d, %s",
				nRoomNo,
				szRoomName);
		}
		else
		{
			strRoomText = szRoomName;
		}
	}
}

/**********************************************************************/

void FixAmpersandsForWindowsText( CString& strText )
{
	for (int n = 0; n < strText.GetLength(); n++)
	{
		char c = strText[n];

		if (c == '&')
		{
			strText.Insert(n++, '&');
		}
	}
}

/**********************************************************************/

bool FindFirstTranLine( CSSFile& fileSales, CString& strBuffer, int& nLinesToRead )
{
	nLinesToRead = -1;

	//FAIL IF THE FILE IS EMPTY
	if (fileSales.ReadString(strBuffer) == FALSE)
	{
		return FALSE;
	}

	//SUCCEED IF THIS IS NOT A PMS FILE
	{
		CString strCheck = strBuffer.Left(5);
		strCheck.MakeUpper();

		if (strCheck != "#DATE")
		{
			return TRUE;
		}
	}

	//FAIL IF WE DON'T FIND #TRAN ON SECOND LINE
	{
		fileSales.ReadString(strBuffer);

		CString strCheck = strBuffer.Left(5);
		strCheck.MakeUpper();

		if (strCheck != "#TRAN")
		{
			return FALSE;
		}
	}

	CCSV csv( strBuffer );
	nLinesToRead = csv.GetInt(1);

	//FILE IF THERE ARE NO TRAN LINES
	if (0 == nLinesToRead)
	{
		return FALSE;
	}

	//FAIL IF THERE ARE NO MORE LINES IN THE FILE
	if (fileSales.ReadString(strBuffer) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

bool FindFirstPluLine(CSSFile& fileSales, CString& strBuffer, int& nLinesToRead)
{
	nLinesToRead = -1;

	//FAIL IF THE FILE IS EMPTY
	if (fileSales.ReadString(strBuffer) == FALSE)
	{
		return FALSE;
	}

	//SUCCEED IF THIS IS NOT A PMS FILE
	{
		CString strCheck = strBuffer.Left(5);
		strCheck.MakeUpper();

		if (strCheck != "#DATE")
		{
			return TRUE;
		}
	}

	//FAIL IF WE DON'T FIND #TRAN ON SECOND LINE
	{
		fileSales.ReadString(strBuffer);

		CString strCheck = strBuffer.Left(5);
		strCheck.MakeUpper();

		if (strCheck != "#TRAN")
		{
			return FALSE;
		}
	}

	//SKIP TRANSACTION SUMMARY LINES
	{
		CCSV csv(strBuffer);
		int nLinesToSkip = csv.GetInt(1);

		while (nLinesToSkip-- >= 0)
		{
			if (fileSales.ReadString(strBuffer) == FALSE)
			{
				return FALSE;
			}
		}
	}

	//FAIL IF THE NEXT LINE IS NOT A PLU HEADER
	{
		CString strCheck = strBuffer.Left(4);

		strCheck.MakeUpper();
		if (strCheck != "#PLU")
		{
			return FALSE;
		}
	}

	CCSV csv(strBuffer);
	nLinesToRead = csv.GetInt(1);

	//FILE IF THERE ARE NO PLU LINES
	if (0 == nLinesToRead)
	{
		return FALSE;
	}

	//FAIL IF THERE ARE NO MORE LINES IN THE FILE
	if (fileSales.ReadString(strBuffer) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

bool FindFirstTermLine(CSSFile& fileSales, CString& strBuffer, int& nLinesToRead)
{
	nLinesToRead = -1;

	//FAIL IF THE FILE IS EMPTY
	if (fileSales.ReadString(strBuffer) == FALSE)
	{
		return FALSE;
	}

	//SUCCEED IF THIS IS NOT A PMS FILE
	{
		CString strCheck = strBuffer.Left(5);
		strCheck.MakeUpper();

		if (strCheck != "#DATE")
		{
			return TRUE;
		}
	}

	//FAIL IF WE DON'T FIND #TRAN ON SECOND LINE
	{
		fileSales.ReadString(strBuffer);

		CString strCheck = strBuffer.Left(5);
		strCheck.MakeUpper();

		if (strCheck != "#TRAN")
		{
			return FALSE;
		}
	}

	//SKIP TRANSACTION SUMMARY LINES
	{
		CCSV csv(strBuffer);
		int nLinesToSkip = csv.GetInt(1);

		while (nLinesToSkip-- >= 0)
		{
			if (fileSales.ReadString(strBuffer) == FALSE)
			{
				return FALSE;
			}
		}
	}

	//FAIL IF THE NEXT LINE IS NOT A PLU HEADER
	{
		CString strCheck = strBuffer.Left(4);
		strCheck.MakeUpper();

		if (strCheck != "#PLU")
		{
			return FALSE;
		}
	}

	//SKIP PLU HISTORY LINES
	{
		CCSV csv(strBuffer);
		int nLinesToSkip = csv.GetInt(1);

		while (nLinesToSkip-- >= 0)
		{
			if (fileSales.ReadString(strBuffer) == FALSE)
			{
				return FALSE;
			}
		}
	}

	CCSV csv(strBuffer);
	nLinesToRead = csv.GetInt(1);

	//FILE IF THERE ARE NO TERM LINES
	if (0 == nLinesToRead)
	{
		return FALSE;
	}

	//FAIL IF THERE ARE NO MORE LINES IN THE FILE
	if (fileSales.ReadString(strBuffer) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

void CopyGlobalData(int nNodeType)
{
	CString strGlobalFile1 = "";
	CString strGlobalFile2 = "";

	switch (nNodeType)
	{
	case NODE_DEPARTMENT:
		strGlobalFile1 = DataManager.GetFilePathDepartment(-1);
		strGlobalFile2 = DataManager.GetFilePathDepartmentSet(-1);
		break;

	case NODE_DEPT_GROUP_REPORT:
		strGlobalFile1 = DataManager.GetFilePathReportGroup(-1);
		break;

	case NODE_DEPT_GROUP_CONSOL:
		strGlobalFile1 = DataManager.GetFilePathConsolGroup(-1);
		break;

	case NODE_DEPT_GROUP_EPOS:
		strGlobalFile1 = DataManager.GetFilePathEposGroup(-1);
		break;

	case NODE_TAX:
		strGlobalFile1 = DataManager.GetFilePathTaxRates(-1);
		strGlobalFile2 = DataManager.GetFilePathHistoricalTax(-1);
		break;

	case NODE_MODIFIER:
		strGlobalFile1 = DataManager.GetFilePathModifier(-1);
		break;

	case NODE_ACAT:
		strGlobalFile1 = DataManager.GetFilePathAnalysisCategory(-1);
		break;

	case NODE_PAYMENT:
		strGlobalFile1 = DataManager.GetFilePathPayment(-1);
		strGlobalFile2 = DataManager.GetFilePathSptBookPaymentMap(-1);
		break;

	case NODE_SERVER:
		strGlobalFile1 = DataManagerNonDb.GetFilePathServer(-1, -1);
		break;

#ifdef STOCKMAN_SYSTEM

	case NODE_CATEGORY:
		strGlobalFile1 = DataManager.GetFilePathCategory(-1);
		strGlobalFile2 = DataManager.GetFilePathCategorySet(-1);
		break;

	case NODE_ALLOWANCE:
		strGlobalFile1 = DataManager.GetFilePathAllowance(-1);
		break;

#endif

	default:
		return;
	}

	if (::FileExists(strGlobalFile1) == FALSE)
	{
		return;
	}

	for (int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++)
	{
		CString strDbFile1 = "";
		CString strDbFile2 = "";

		switch (nNodeType)
		{
		case NODE_DEPARTMENT:
			strDbFile1 = DataManager.GetFilePathDepartment(nDbIdx);
			strDbFile2 = DataManager.GetFilePathDepartmentSet(nDbIdx);
			break;

		case NODE_DEPT_GROUP_REPORT:
			strDbFile1 = DataManager.GetFilePathReportGroup(nDbIdx);
			break;

		case NODE_DEPT_GROUP_CONSOL:
			strDbFile1 = DataManager.GetFilePathConsolGroup(nDbIdx);
			break;

		case NODE_DEPT_GROUP_EPOS:
			strDbFile1 = DataManager.GetFilePathEposGroup(nDbIdx);
			break;

		case NODE_TAX:
			strDbFile1 = DataManager.GetFilePathTaxRates(nDbIdx);
			strDbFile2 = DataManager.GetFilePathHistoricalTax(nDbIdx);
			break;

		case NODE_MODIFIER:
			strDbFile1 = DataManager.GetFilePathModifier(nDbIdx);
			break;

		case NODE_ACAT:
			strDbFile1 = DataManager.GetFilePathAnalysisCategory(nDbIdx);
			break;

		case NODE_PAYMENT:
			strDbFile1 = DataManager.GetFilePathPayment(nDbIdx);
			strDbFile2 = DataManager.GetFilePathSptBookPaymentMap(nDbIdx);
			break;

		case NODE_SERVER:
			strDbFile1 = DataManagerNonDb.GetFilePathServer(nDbIdx, -1);
			break;

#ifdef STOCKMAN_SYSTEM

		case NODE_CATEGORY:
			strDbFile1 = DataManager.GetFilePathCategory(nDbIdx);
			strDbFile2 = DataManager.GetFilePathCategorySet(nDbIdx);
			break;

		case NODE_ALLOWANCE:
			strDbFile1 = DataManager.GetFilePathAllowance(nDbIdx);
			break;

#endif

		default:
			return;
		}

		if (::FileExists(strDbFile1) == FALSE)
		{
			CopyFile(strGlobalFile1, strDbFile1, FALSE);

			if (strGlobalFile2 != "")
			{
				CopyFile(strGlobalFile2, strDbFile2, FALSE);
			}
		}
	}
}

/**********************************************************************/

void CopyServerData( int nSourceNodeType )
{
	switch( nSourceNodeType )
	{
	case NODE_SYSTEM:
		{
			CString strGlobalFile = DataManagerNonDb.GetFilePathServer(-1, -1);

			if (::FileExists(strGlobalFile) == TRUE)
			{
				for (int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++)
				{
					CFilenameUpdater FnUp2(SysFiles::Servers, nLocIdx, -1, FNUP_FAMILY_LOCATION);
					CString strLocFile = FnUp2.GetFilenameToUse();

					if (::FileExists(strLocFile) == FALSE)
					{
						::CopyFile(strGlobalFile, strLocFile, FALSE);
					}
				}
			}
		}
		break;

	case NODE_DATABASE:
		{
			for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
			{
				CFilenameUpdater FnUp( SysFiles::Servers, nDbIdx, -1, FNUP_FAMILY_DATABASE );
				CString strDbFile = FnUp.GetFilenameToUse();

				if (::FileExists(strDbFile) == FALSE)
				{
					continue;
				}

				CArray<int,int> arrayLocIdx;
				dbLocation.GetDatabaseLocIdxSet( nDbIdx, arrayLocIdx );

				for (int n = 0; n < arrayLocIdx.GetSize(); n++)
				{
					int nLocIdx = arrayLocIdx.GetAt(n);

					CFilenameUpdater FnUp2(SysFiles::Servers, nLocIdx, -1, FNUP_FAMILY_LOCATION);
					CString strLocFile = FnUp2.GetFilenameToUse();

					if (::FileExists(strLocFile) == FALSE)
					{
						::CopyFile(strDbFile, strLocFile, FALSE);
					}
				}
			}
		}
		break;
	}
}

/**********************************************************************/

void AddPMSPluItem(int nItem, const char* szEcrText, const char* szRepText, int nDeptNo)
{
	int nPluIdx = 0;
	CPluCSVRecord PluRecord;

	PluRecord.SetPluNo(99999999999900 + nItem);
	PluRecord.SetEposText(szEcrText);
	PluRecord.SetRepText(szRepText);
	PluRecord.SetBaseDeptNo(nDeptNo);

	if (DataManager.Plu.FindPluByNumber(PluRecord.GetPluNoInt(), nPluIdx) == FALSE)
	{
#ifdef STOCKMAN_SYSTEM
		DataManager.Plu.InsertRecord(PluRecord, "", TRUE);
#else
		DataManager.Plu.InsertRecord(PluRecord, TRUE);
#endif
	}
	else
	{
		DataManager.Plu.SetAt(nPluIdx, PluRecord);
	}
}
	
/**********************************************************************/
#endif
/**********************************************************************/

void StockMovementError ( int nType, bool bPlu )
{
	CString strMsg = "";
	
	strMsg += "You must set up some ";
	strMsg += ( bPlu ) ? "plu" : "stock";
	strMsg += " records before you can enter ";

	switch( nType )
	{
	case NODE_DELIVERY:
		strMsg += " deliveries";
		break;

	case NODE_RETURN:
		strMsg += " returns";
		break;

	case NODE_ADJUSTMENT_PLU:
	case NODE_ADJUSTMENT_STOCK:
		strMsg += " adjustments";
		break;

	case NODE_MANUALSALE_STOCK:
	case NODE_MANUALSALE_PLU:
		strMsg += " manual sales";
		break;
	}

	Prompter.Error( strMsg );
}

/**********************************************************************/

void SetHexStringFlag( CString& strHex, int nBit, bool bFlag )
{
	if ( ( nBit >= 1 ) && ( nBit <= 32 ) )
	{
		int x = HexToInt( strHex );

		int nMask = 1;
		nMask <<= nBit - 1;

		if ( TRUE == bFlag )
		{
			if ( ( x & nMask ) == 0 )
			{
				x += nMask;
				strHex.Format( "%8.8X", x );
			}
		}
		else
		{
			if ( ( x & nMask ) != 0 )
			{
				x -= nMask;
				strHex.Format( "%8.8X", x );
			}
		}
	}
}

/**********************************************************************/

bool GetHexStringFlag( CString& strHex, int nBit )
{
	bool bResult = FALSE;

	if ( ( nBit >= 1 ) && ( nBit <= 32 ) )
	{
		int x = HexToInt( strHex );

		int nMask = 1;
		nMask <<= nBit - 1;

		bResult = ( ( x & nMask ) != 0 );
	}

	return bResult;
}

/**********************************************************************/

#ifndef POSTRAY_UTILITY
	#ifndef STOCKMAN_SYSTEM
		void ShowPluFilterDlg( CPluFilterArray& filter, CWnd* pParent, int& nDlgResult, int& nPluIdx )
		{
			CPluFilterDlgEcrman dlgBrowse( filter, pParent );
			nDlgResult = dlgBrowse.DoModal();
			nPluIdx = dlgBrowse.GetPluIdx();
		}
	#else
		void ShowPluFilterDlg( CPluFilterArray& filter, CWnd* pParent, int& nDlgResult, int& nPluIdx )
		{
			CPluFilterDlgStockman dlgBrowse( filter, pParent );
			nDlgResult = dlgBrowse.DoModal();
			nPluIdx = dlgBrowse.GetPluIdx();
		}
	#endif
#endif

/**********************************************************************/

int GetImageType(const char* szFile, CString& strImageData, bool bFullRead)
{
	unsigned char buffer[1024];

	CSSFile file;
	if (file.Open(szFile, "rb") == FALSE)
	{
		return IMAGE_TYPE_NONE;
	}

	strImageData = "0x";

	if (FALSE == bFullRead)
	{
		int nBytes = file.Read(buffer, 1, 10);

		for (int n = 0; n < nBytes; n++)
		{
			CString strHex;
			strHex.Format("%2.2X", buffer[n]);
			strImageData += strHex;
		}
	}
	else
	{
		int nBytes = file.Read(buffer, 1, 1024);

		while (nBytes != 0)
		{
			for (int n = 0; n < nBytes; n++)
			{
				CString strHex;
				strHex.Format("%2.2X", buffer[n]);
				strImageData += strHex;
			}

			nBytes = file.Read(buffer, 1, 1024);
		}
	}

	int nLen = strImageData.GetLength();

	if ((nLen >= 6) && (strImageData.Mid(2, 4) == "424D"))
	{
		return IMAGE_TYPE_BMP;
	}

	if ((nLen >= 8) && (strImageData.Mid(2, 6) == "474946"))
	{
		return IMAGE_TYPE_GIF;
	}

	if ((nLen >= 8) && (strImageData.Mid(2, 6) == "FFD8FF"))
	{
		return IMAGE_TYPE_JPG;
	}

	return IMAGE_TYPE_NONE;
}

/**********************************************************************/

void GetSimpleHexTimeStamp(CString& strTimeStamp)
{
	COleDateTime timeNow = COleDateTime::GetCurrentTime();

	int nYear = timeNow.GetYear() - 2000;

	if (nYear < 0)
	{
		nYear = 0;
	}

	nYear %= 16;

	strTimeStamp.Format("%X%X%2.2X%2.2X%3.3X",
		nYear,
		timeNow.GetMonth(),
		timeNow.GetDay(),
		timeNow.GetHour(),
		(timeNow.GetMinute() * 60) + timeNow.GetSecond());
}

/**********************************************************************/

void TokeniseSpacedLine(CString& strText, CStringArray& arrayTokens)
{
	arrayTokens.RemoveAll();
	CString strToken = "";

	for (int i = 0; i < strText.GetLength(); i++)
	{
		char c = strText.GetAt(i);

		if (c == ' ')
		{
			if (strToken != "")
			{
				arrayTokens.Add(strToken);
				strToken = "";
			}
		}
		else
		{
			strToken += c;
		}
	}

	if (strToken != "")
	{
		arrayTokens.Add(strToken);
	}
}

/**********************************************************************/

void GetStockLevelString ( double dQty, int nSubUnits, CString& strQty )
{
	if ( nSubUnits <= 1 )
	{
		strQty.Format( "%.*f", SysInfo.GetDPQty(), dQty );
	}
	else
	{
		strQty = "";
			
		if ( dQty < 0.0 )
		{
			strQty = "-";
			dQty = -dQty;
		}

		int nStock = ( int ) dQty;
		int nSub = ( int ) ( ( dQty * nSubUnits ) - ( nStock * nSubUnits ) + 0.5 );
		
		CString strTemp = "";
		strTemp.Format ( "%d/%d", nStock, nSub );
		strQty += strTemp;
	}
}

/**********************************************************************/

void ShowLocationError()
{
	Prompter.Error ( "You have not setup any locations that support this option" );
}

/**********************************************************************/

void ShowStockpointError()
{
	Prompter.Error ( "You must create a stockpoint before you can use this option" );
}

/**********************************************************************/

void MakeYearFirstDate( CString& strDate )
{
	if ( strDate.GetLength() == 8 )
	{
		CString strTest = strDate.Mid(4,2);
		if ( ( strTest == "19" ) || ( strTest == "20" ) )
		{
			CString strTemp = strDate;
			strDate = strTemp.Right(4) + strTemp.Mid(2,2) + strTemp.Left(2);
		}
	}
}

/**********************************************************************/

void HTMLEuroConvert(CString& strLine)
{
	CString strTemp = strLine;
	strLine = "";

	for (int n = 0; n < strTemp.GetLength(); n++)
	{
		char c = strTemp.GetAt(n);

		if (c != '€')
		{
			strLine += c;
		}
		else
		{
			strLine += "&euro;";
		}
	}
}

/**********************************************************************/

void GetMixMatchOfferTypeName( int nType, CString& strType, bool bFullLine, const char* szUnknown )
{
	strType = bFullLine ? "<..>" : "";

	switch( nType )
	{
	case 1:		
		strType += "Deduct cost of one item";	
		break;

	case 2:		
		strType += "Selling price by cost";	
		break;

	case 3:	
		strType += "Apply promotion";	
		break;

	case 4:		
		strType += "Cheapest one free";	
		break;

	case 5:		
		strType += "Selling price by PLU";
		break;

	case 6:		
		strType += "One bucket free";		
		break;

	case 7:		
		strType += "Selling price by PLU 2";
		break;

	case 8:		
		strType += "Set bucket price";
		break;

	default:	
		strType += szUnknown;
		break;
	}
}

/**********************************************************************/

bool ChangeFileExtension( CString& strInput, const char* szNewExtension, CString& strOutput )
{
	strOutput = "";
	bool bResult = FALSE;
	
	int nInputLen = strInput.GetLength();
	if ( nInputLen > 4 )
	{	
		if ( strInput.Mid( nInputLen -4, 1 ) == "." )
		{
			strOutput = strInput.Left( nInputLen - 3 );
			strOutput += szNewExtension;
			bResult = TRUE;
		}
	}

	return bResult;
}

/**********************************************************************/

bool CompareFiles( const char* szFilename1, const char* szFilename2 )
{
	CFileStatus FileStatus1, FileStatus2;
	CFile::GetStatus( szFilename1, FileStatus1 );
	CFile::GetStatus( szFilename2, FileStatus2 );
			
	if (FileStatus1.m_size != FileStatus2.m_size)
	{
		return FALSE;
	}

	CSSFile file1, file2;
				
	if (file1.Open(szFilename1, "rb") == FALSE)
	{
		return FALSE;
	}

	if (file2.Open(szFilename2, "rb") == FALSE)
	{
		return FALSE;
	}

	unsigned char buffer1[1024];		
	unsigned char buffer2[1024];
		
	int nBytes1 = file1.Read( buffer1, 1, 1024 );
	int nBytes2 = file2.Read( buffer2, 1, 1024 );
			
	while( ( nBytes2 != 0 ) && ( nBytes2 == nBytes1 ) )			
	{				
		for (int n = 0; n < nBytes1; n++)
		{
			if (buffer1[n] != buffer2[n])
			{
				return FALSE;
			}
		}
		
		nBytes1 = file1.Read( buffer1, 1, 1024 );
		nBytes2 = file2.Read( buffer2, 1, 1024 );		
	}

	return ( nBytes1 == nBytes2 );
}

/**********************************************************************/

void GetOleDateSince2011( int nDayNumber, COleDateTime& date )
{
	date = COleDateTime( 2011, 1, 1, 0, 0, 0 ) + COleDateTimeSpan( nDayNumber, 0, 0, 0 );
}

/**********************************************************************/

int GetDayNumberSince2011( COleDateTime& date )
{
	COleDateTime dateToCheck = COleDateTime(
		date.GetYear(),
		date.GetMonth(),
		date.GetDay(),
		0, 0, 0 );

	COleDateTime dateZero = COleDateTime( 2011, 1, 1, 0, 0, 0 );

	if (dateToCheck.m_status != COleDateTime::valid)
	{
		return 0;
	}

	COleDateTimeSpan span = dateToCheck - dateZero;

	return span.GetDays();
}

/**********************************************************************/

void DrawTextInDeviceContext( CDC* pDC, CStringArray& Texts, CRect rect, UINT nHAlign, int nFontSize, COLORREF crText )
{
	int nLines = Texts.GetSize();
	if (nLines == 0)
	{
		return;
	}

	pDC -> SetBkMode ( TRANSPARENT );
	pDC -> SetTextColor ( crText );

	CFont Font;
	FontTable.GetKeyDisplayFont( pDC, &Font, nFontSize, FALSE );
	
	UINT nOldAlign = pDC-> SetTextAlign ( nHAlign );
	CFont* pOldFont = pDC -> SelectObject ( &Font );

	TEXTMETRIC tm;
	pDC -> GetTextMetrics ( &tm );

	int nLineHeight = tm.tmHeight + tm.tmExternalLeading;
	int nLineSpace = ( nLineHeight * nLines ) - tm.tmExternalLeading;
	int nHeight = rect.Height();

	//height and y axis are negative
	int nYStart = rect.top + ( ( rect.Height() - nLineSpace ) / 2 );

	int nXPos = 0;
	switch( nHAlign )
	{
	case TA_LEFT:
		pDC -> SetTextAlign( TA_LEFT );
		nXPos = rect.left;
		break;

	case TA_RIGHT:
		pDC -> SetTextAlign( TA_RIGHT );
		nXPos = rect.right;
		break;

	case TA_CENTER:
	default:
		pDC -> SetTextAlign( TA_CENTER );
		nXPos = rect.left + ( rect.Width() / 2 );
		break;
	}
		
	for ( int nToken = 0; nToken < nLines; nToken++ )
	{			
		CString strToken = Texts.GetAt ( nToken );
			
		if (strToken != "")
		{
			pDC->ExtTextOut(nXPos, nYStart, ETO_CLIPPED, rect, strToken, strToken.GetLength(), NULL);
		}

		nYStart += nLineHeight;
	}
	
	pDC -> SetTextAlign ( nOldAlign );
	pDC -> SelectObject ( pOldFont );
}

/**********************************************************************/

bool ValidateFolderAccess(const char* szPath)
{
	bool bReply = FALSE;
	CString strFilename = "";
	strFilename.Format("%s\\%s",
		(const char*)szPath,
		(const char*)GetUniqueTempFilename("$$$"));			// cretae a temp file

	CSSFile file;
	if ((bReply = file.Open(strFilename, "wb")) == TRUE)
	{
		file.Close();
		SendToRecycleBin(strFilename);
	}
	return bReply;
}

//*******************************************************************

CString GetUniqueTempFilename(const char* szFileExtension, const char* szPrefix)
{
	CString strFilename = "";

	CTime t = CTime::GetCurrentTime();
	strFilename.Format("%s-%d%2.2d%2.2d%2.2d%2.2d%2.2d.%s", szPrefix, t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(), szFileExtension);

	return strFilename;
}

//*******************************************************************

int CountFiles(const char* szFileMask)
{
	int nCount = 0;

	CFileFind FileFinder;
	BOOL bWorking = FileFinder.FindFile(szFileMask);
	while (bWorking)
	{
		bWorking = FileFinder.FindNextFile();
		if (FileFinder.IsDots() == TRUE || FileFinder.IsDirectory() == TRUE)
		{
			continue;
		}

		++nCount;
	}

	return nCount;
}

//*******************************************************************

void SendToRecycleBin(CStringArray* pArray)
{
	for (int i = 0; i < pArray->GetSize(); i++)
	{
		remove(pArray->GetAt(i));
	}
}

/******************************************************************************/

void SendToRecycleBin(const char* szFileMask)
{
	CFileFind FileFinder;
	BOOL bWorking = FileFinder.FindFile(szFileMask);

	while (bWorking)
	{
		bWorking = FileFinder.FindNextFile();
		if (FileFinder.IsDots() == TRUE)
		{
			continue;
		}

		if (FileFinder.IsDirectory() == TRUE)
		{
			continue;
		}

		remove(FileFinder.GetFilePath());
	}
}

/******************************************************************************/

void CKUnlock()
{
	CkGlobal glob;
	bool success = glob.UnlockBundle("SPOSUK.CB1082028_DzA3mERU2EoF");
	if (success != true) {
		AfxMessageBox(glob.lastErrorText());
		return;
	}

	int status = glob.get_UnlockStatus();
	if (status == 2)
	{
		//UNLOCKED WITH PURCHASED CODE;
	}
	else
	{
		//UNLOCKED IN TRIAL MODE;
	}
}

/**********************************************************************/