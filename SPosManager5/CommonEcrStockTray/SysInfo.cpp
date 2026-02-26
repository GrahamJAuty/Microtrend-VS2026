/**********************************************************************/
#include "SalesHistoryFields.h"
/**********************************************************************/
#include <math.h>
/**********************************************************************/
#include "SysInfo.h"
/**********************************************************************/

CSysInfo::CSysInfo()
{
	m_dwStackBase = 0;
	SetDefaults();
	m_bFatalReadError = FALSE;
	m_nPluMode = 0;
	m_nMaxBasePluLen = -1;
	m_bAltFilenamesIfNoModifiers = TRUE;
	m_nLoyaltyOrSmartPayType = LOYALTY_TYPE_NONE;
	m_bIsPosTrayStockSystem = FALSE;
}

/**********************************************************************/

void CSysInfo::SetDefaults()
{
	m_nDPQty = 3;
	m_nDPValue = 2;
	m_nDataPathNo = 0;
	m_bScriptMode = TRUE;
	m_bTaskMode = FALSE;
	m_strTaskMode = "";
	m_bProcessCashRSPBuffer = FALSE;
	m_nCommsThreadSpeed = 1;
	m_bNewBarcodePlu = FALSE;
	SetCustomTaxFilter( TAXBAND_FILTER_ALL );
	SetCustomPriceLevelFilter( PRICELEVEL_FILTER_ALL );
	SetCustomModifierFilter( MODIFIER_FILTER_ALL );
}

/**********************************************************************/

bool CSysInfo::Read()
{
	CFilenameUpdater FnUp ( SysFiles::System );

	CSSFile fileSysInfo;
	if ( fileSysInfo.Open ( FnUp.GetFilenameToUse(), "rb" ) == FALSE )
	{		
		if (GetLastError() != ERROR_FILE_NOT_FOUND)
		{
			m_bFatalReadError = TRUE;
		}
			
		return FALSE;
	}
	
	CString strBuffer;
	if (fileSysInfo.ReadString(strBuffer) == FALSE)
	{
		return FALSE;
	}
	
	CCSVSysInfo csv ( strBuffer );

	SetDPQty ( csv.GetDPQty() );
	SetDPValue ( csv.GetDPValue() );
	SetCommsThreadSpeed( csv.GetCommsThreadSpeed() );
	SetProcessCashRSPBufferFlag( csv.GetProcessCashRSPBufferFlag() );
	
	return TRUE;
}
	
/**********************************************************************/

bool CSysInfo::Write()
{
	bool bWrite = FALSE;

	if ( DataManager.LockAllUsers() == TRUE )
	{
		CFilenameUpdater FnUp ( SysFiles::System );

		CSSFile fileSysInfo;
		if ( fileSysInfo.Open ( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
		{
			CCSV csv;
			csv.Add ( m_nDPQty );
			csv.Add ( m_nDPValue );
			csv.Add ( 60 );
			csv.Add ( 0 );
			csv.Add ( m_nCommsThreadSpeed );
			csv.Add ( m_bProcessCashRSPBuffer );
			fileSysInfo.WriteLine ( csv.GetLine() );
			fileSysInfo.Close();

			bWrite = TRUE;
		}
		
		DataManager.UnlockAllUsers();
	}

	return bWrite;
}

/**********************************************************************/

int CSysInfo::GetEcrLiteSystemType()
{
#ifdef STOCKMAN_SYSTEM
	return ECRLITE_TYPE_NONE;
#endif

#ifdef POSTRAY_UTILITY
	return ECRLITE_TYPE_NONE;
#endif

	if ( ( GetExternalLink() & 0x4 ) == 0x4 )
	{
		return ECRLITE_TYPE_NEW;
	}

	if ( ( GetExternalLink() & 0x100 ) == 0x100 )
	{
		return ECRLITE_TYPE_NEW;
	}

	if ( Sysset.GetSystemType() == 1 )
	{
		return ECRLITE_TYPE_NEW;
	}

	return ECRLITE_TYPE_NONE;
}

/**********************************************************************/

bool CSysInfo::IsEcrLiteSystem()
{
	return ( GetEcrLiteSystemType() == ECRLITE_TYPE_NEW );
}

/**********************************************************************/

CString CSysInfo::GetEcrLiteVersionDetail()
{
	CString strDetail = "Lite type. ";
	
	switch ( GetEcrLiteSystemType() )
	{
	case ECRLITE_TYPE_NEW:	
		strDetail += "2025";	
		break;

	case ECRLITE_TYPE_NONE:		
	default:
		strDetail += "";
		break;
	}

	return strDetail;
}

/**********************************************************************/

void CSysInfo::SetDPQty(int n)
{
	if (n >= 0 && n <= 3)
	{
		m_nDPQty = n;
	}
}

/**********************************************************************/

void CSysInfo::SetDPValue(int n)
{
	if ((n >= 0) && (n <= 3))
	{
		m_nDPValue = n;
	}
}

/**********************************************************************/

void CSysInfo::SetCommsThreadSpeed(int n)
{
	if ((n >= 1) && (n <= 10))
	{
		m_nCommsThreadSpeed = n;
	}
}

/**********************************************************************/

void CSysInfo::SetTaskModeString( const char* sz )
{
	CString str = sz;
	str.MakeUpper();

	if ( str.Left(7) == "RUNTASK" )
	{
		m_bTaskMode = TRUE;
		m_strTaskMode = str.Right( str.GetLength() - 7 );
	}
	else
	{
		m_bTaskMode = FALSE;
		m_strTaskMode = "";
	}
}

/**********************************************************************/

void CSysInfo::RememberSettings()
{
	m_nStoredDPQty = m_nDPQty;
	m_nStoredDPValue = m_nDPValue;
	m_nStoredCommsThreadSpeed = m_nCommsThreadSpeed;
	m_bStoredProcessCashRSPBuffer = m_bProcessCashRSPBuffer;
}

/**********************************************************************/

void CSysInfo::RestoreSettings()
{
	m_nDPQty = m_nStoredDPQty;
	m_nDPValue = m_nStoredDPValue;
	m_nCommsThreadSpeed = m_nStoredCommsThreadSpeed;
	m_bProcessCashRSPBuffer = m_bStoredProcessCashRSPBuffer;
}

/**********************************************************************/

bool CSysInfo::IsStockSystem()
{
#ifdef POSTRAY_UTILITY
	return m_bIsPosTrayStockSystem;
#endif

#ifdef STOCKMAN_SYSTEM
	return TRUE;
#endif

	return FALSE;
}

/**********************************************************************/

bool CSysInfo::IsSageExportSystem()
{
	if (IsEcrLiteSystem() == TRUE)
	{
		return FALSE;
	}
	else
	{
		return DealerFlags.GetSageExportFlag();
	}
}

/**********************************************************************/

bool CSysInfo::IsIDraughtExportSystem()
{
	if (IsEcrLiteSystem() == TRUE)
	{
		return FALSE;
	}
	else
	{
		return DealerFlags.GetIDraughtExportFlag();
	}
}

/**********************************************************************/

bool CSysInfo::IsPosTraySystem()
{
	bool bResult = FALSE;
	if (Sysset.IsSchedulerSystem())
	{
		bResult = TRUE;
	}
	else if (IsPosTrayNoTaskSystem())
	{
		bResult = TRUE;
	}
	return bResult;
}

/**********************************************************************/

bool CSysInfo::IsPosTrayTimeAndAttendanceSystem()
{
	return TRUE;
	//return ((GetExternalLink() & 0x1000) == 0x1000);
}

/**********************************************************************/

bool CSysInfo::IsSPOSSMTPServerSystem()
{
	return ((GetExternalLink() & 0x2000) == 0x2000);
}

/**********************************************************************/

bool CSysInfo::IsPosTrayNoTaskSystem()
{
	return ((GetExternalLink() & 0x800) == 0x800);
}

/**********************************************************************/

bool CSysInfo::IsPosTraySocketSystem()
{
#ifndef POSTRAY_UTILITY
	return FALSE;
#endif

#ifndef POSTRAY_SOCKET
	return FALSE;
#endif

	return TRUE;
}

/**********************************************************************/

bool CSysInfo::IsSportsBookerSystem()
{
	return ( ( GetExternalLink() & 0x3 ) == 1 ); 
}

/**********************************************************************/

bool CSysInfo::IsSmartEntertainmentSystem()
{
	return ( ( GetExternalLink() & 0x3 ) == 2 );
}

/**********************************************************************/

bool CSysInfo::IsPMSSystem()
{
#ifndef COMPILE_PMS
	return FALSE;
#else
	return ( GetMaxPMSRooms() != 0 );
#endif
}

/**********************************************************************/

int CSysInfo::GetMaxPMSRooms()
{
	return ( Sysset.GetPMS() );
}

/**********************************************************************/

int CSysInfo::GetMaxDatabases()
{
	return Sysset.GetMaxDatabases();
}

/**********************************************************************/

int CSysInfo::GetMaxLocations()
{
	return Sysset.GetMaxLocations();
}

/**********************************************************************/

int CSysInfo::GetMaxRemoteLocations()
{
	return Sysset.GetMaxRemoteLocations();
}

/**********************************************************************/

int CSysInfo::GetMaxBasePluLen(bool bForceRefresh)
{
	if (FALSE == bForceRefresh)
	{
		if ((m_nMaxBasePluLen >= 0) && (m_nMaxBasePluLen <= 13))
		{
			return m_nMaxBasePluLen;
		}
	}

	m_bAltFilenamesIfNoModifiers = TRUE;

	CString strPath = "SysFiles\\";
	GetDataProgramPath(strPath);

	if (::FileExists(strPath + "FILED0078.DAT"))
	{
		m_nMaxBasePluLen = 13;
	}
	else if (::FileExists(strPath + "FILEC0078.DAT"))
	{
		m_nMaxBasePluLen = 12;
	}
	else if (::FileExists(strPath + "FILEB0078.DAT"))
	{
		m_nMaxBasePluLen = 11;
	}
	else if (::FileExists(strPath + "FILEA0078.DAT"))
	{
		m_nMaxBasePluLen = 10;
	}
	else if (::FileExists(strPath + "FILE90078.DAT"))
	{
		m_nMaxBasePluLen = 9;
	}
	else if (::FileExists(strPath + "FILE80078.DAT"))
	{
		m_nMaxBasePluLen = 8;
	}
	else if (::FileExists(strPath + "FILE70078.DAT"))
	{
		m_nMaxBasePluLen = 7;
	}
	else if (::FileExists(strPath + "FILE60078.DAT"))
	{
		m_nMaxBasePluLen = 6;
	}
	else if (::FileExists(strPath + "FILE50078.DAT"))
	{
		m_nMaxBasePluLen = 5;
	}
	else if (::FileExists(strPath + "FILE40078.DAT"))
	{
		m_nMaxBasePluLen = 4;
	}
	else if (::FileExists(strPath + "FILE30078.DAT"))
	{
		m_nMaxBasePluLen = 3;
	}
	else if (::FileExists(strPath + "FILE20078.DAT"))
	{
		m_nMaxBasePluLen = 2;
	}
	else if (::FileExists(strPath + "FILE10078.DAT"))
	{
		m_nMaxBasePluLen = 1;
	}
	else if (::FileExists(strPath + "FILE00078.DAT"))
	{
		m_bAltFilenamesIfNoModifiers = TRUE;
		m_nMaxBasePluLen = 0;
	}
	else if (::FileExists(strPath + "FILEX0078.DAT"))
	{
		m_bAltFilenamesIfNoModifiers = FALSE;
		m_nMaxBasePluLen = 0;
	}
	else
	{
		m_nMaxBasePluLen = 6;
	}

	return m_nMaxBasePluLen;
}

/**********************************************************************/

bool CSysInfo::IsModifiableSSPluNo( __int64 nPluNo )
{
	int nMaxBaseLen = GetMaxBasePluLen();
	
	if ( 0 == nMaxBaseLen )
		return FALSE;
	else
	{
		CString strPluNo;
		strPluNo.Format( "%I64d", nPluNo );
		return ( strPluNo.GetLength() <= nMaxBaseLen );
	}
}

/**********************************************************************/

bool CSysInfo::IsLiteralEposPluNo( __int64 nPluNo )
{
	if ( nPluNo > Plu::PluNo.Max )
		return FALSE;

	switch( GetMaxBasePluLen() )
	{
	case 0:		return ( nPluNo >= 1 );
	case 1:		return ( nPluNo >= 100 );
	case 2:		return ( nPluNo >= 1000 );
	case 3:		return ( nPluNo >= 10000 );
	case 4:		return ( nPluNo >= 100000 );
	case 5:		return ( nPluNo >= 1000000 );
	case 6:		return ( nPluNo >= 10000000 );
	case 7:		return ( nPluNo >= 100000000 );
	case 8:		return ( nPluNo >= 1000000000 );
	case 9:		return ( nPluNo >= 10000000000 );
	case 10:	return ( nPluNo >= 100000000000 );
	case 11:	return ( nPluNo >= 1000000000000 );
	case 12:	return ( nPluNo >= 10000000000000 );
	default:	return FALSE;
	}
}

/**********************************************************************/

bool CSysInfo::IsValidSSPluNo( __int64 nPluNo )
{
	CString strPluNo;
	strPluNo.Format( "%I64d", nPluNo );

	int nMaxBaseLen = GetMaxBasePluLen();

	switch( strPluNo.GetLength() )
	{
	case 1:		return ( nPluNo != 0 );
	case 2:		return ( nMaxBaseLen != 1 );
	case 3:		return ( nMaxBaseLen != 2 );
	case 4:		return ( nMaxBaseLen != 3 );
	case 5:		return ( nMaxBaseLen != 4 );
	case 6:		return ( nMaxBaseLen != 5 );
	case 7:		return ( nMaxBaseLen != 6 );
	case 8:		return ( nMaxBaseLen != 7 );
	case 9:		return ( nMaxBaseLen != 8 );
	case 10:	return ( nMaxBaseLen != 9 );
	case 11:	return ( nMaxBaseLen != 10 );
	case 12:	return ( nMaxBaseLen != 11 );
	case 13:	return ( nMaxBaseLen != 12 );
	case 14:	return ( nMaxBaseLen != 13 );
	default:	return FALSE;
	}
}

/**********************************************************************/

void CSysInfo::SetCustomTaxFilter( int n )
{
	m_nCustomTaxFilter = n;
	m_strCustomTaxFilter = "";

	int nMask = 1;
	CString strBands = "0ABCDEFGHIJKL";

	CString strFilter = "";
	CString strFilterNoSpace = "";

	for (int nBand = 0; nBand <= MAX_TAX_BANDS; nBand++)
	{
		if ((m_nCustomTaxFilter & nMask) != 0)
		{
			if (strFilter != "")
			{
				strFilter += " ";
			}

			strFilter += strBands.GetAt(nBand);
			strFilterNoSpace += strBands.GetAt(nBand);
		}
		nMask *= 2;
	}

	if (strFilterNoSpace.GetLength() > 8)
	{
		m_strCustomTaxFilter = strFilterNoSpace;
	}
	else
	{
		m_strCustomTaxFilter = strFilter;
	}

	if (m_strCustomTaxFilter == "")
	{
		m_strCustomTaxFilter = "None";
	}
}

/**********************************************************************/

void CSysInfo::SetCustomPriceLevelFilter( int n )
{
	m_nCustomPriceLevelFilter = n;
	m_strCustomPriceLevelFilter = "";

	if ( ( m_nCustomPriceLevelFilter & 1 ) != 0 )	m_strCustomPriceLevelFilter += "1 ";
	if ( ( m_nCustomPriceLevelFilter & 2 ) != 0 )	m_strCustomPriceLevelFilter += "2 ";
	if ( ( m_nCustomPriceLevelFilter & 4 ) != 0 )	m_strCustomPriceLevelFilter += "3 ";
	if ( ( m_nCustomPriceLevelFilter & 8 ) != 0 )	m_strCustomPriceLevelFilter += "4 ";
	if ( ( m_nCustomPriceLevelFilter & 16 ) != 0 )	m_strCustomPriceLevelFilter += "5 ";
	if ( ( m_nCustomPriceLevelFilter & 32 ) != 0 )	m_strCustomPriceLevelFilter += "6 ";
	if ( ( m_nCustomPriceLevelFilter & 64 ) != 0 )	m_strCustomPriceLevelFilter += "7 ";
	if ( ( m_nCustomPriceLevelFilter & 128 ) != 0 )	m_strCustomPriceLevelFilter += "8 ";
	if ( ( m_nCustomPriceLevelFilter & 256 ) != 0 )	m_strCustomPriceLevelFilter += "9 ";
	if ( ( m_nCustomPriceLevelFilter & 512 ) != 0 )	m_strCustomPriceLevelFilter += "10 ";
	
	if ( m_strCustomPriceLevelFilter == "" ) m_strCustomPriceLevelFilter = "None";
}

/**********************************************************************/

void CSysInfo::SetCustomModifierFilter( int n )
{
	m_nCustomModifierFilter = n;
	m_strCustomModifierFilter = "";

	if ( ( m_nCustomModifierFilter & 1 ) != 0 )		m_strCustomModifierFilter += "0 ";
	if ( ( m_nCustomModifierFilter & 2 ) != 0 )		m_strCustomModifierFilter += "1 ";
	if ( ( m_nCustomModifierFilter & 4 ) != 0 )		m_strCustomModifierFilter += "2 ";
	if ( ( m_nCustomModifierFilter & 8 ) != 0 )		m_strCustomModifierFilter += "3 ";
	if ( ( m_nCustomModifierFilter & 16 ) != 0 )	m_strCustomModifierFilter += "4 ";
	if ( ( m_nCustomModifierFilter & 32 ) != 0 )	m_strCustomModifierFilter += "5 ";
	if ( ( m_nCustomModifierFilter & 64 ) != 0 )	m_strCustomModifierFilter += "6 ";
	if ( ( m_nCustomModifierFilter & 128 ) != 0 )	m_strCustomModifierFilter += "7 ";
	if ( ( m_nCustomModifierFilter & 256 ) != 0 )	m_strCustomModifierFilter += "8 ";
	if ( ( m_nCustomModifierFilter & 512 ) != 0 )	m_strCustomModifierFilter += "9 ";
	
	if ( m_strCustomModifierFilter == "" ) m_strCustomModifierFilter = "None";
}

/**********************************************************************/

void CSysInfo::SetLoyaltyOrSmartPayType( int n )
{
	switch( n )
	{
	case LOYALTY_TYPE_NONE:
	case LOYALTY_TYPE_UNKNOWN:
	case LOYALTY_TYPE_LOY_V2:
	case LOYALTY_TYPE_LOY_V3:
	case LOYALTY_TYPE_LOY_V4:
	case LOYALTY_TYPE_SMP_V1:
	case LOYALTY_TYPE_SMP_V2:
	case LOYALTY_TYPE_SMP_V4:
		m_nLoyaltyOrSmartPayType = n;
		break;
	}
}

/**********************************************************************/

int CSysInfo::GetLoyaltyOrSmartPayType()
{
	if (Sysset.IsSmartcardSystem() == FALSE)
	{
		return LOYALTY_TYPE_NONE;
	}

	return m_nLoyaltyOrSmartPayType;
}

/**********************************************************************/

bool CSysInfo::GotLoyaltyLink()
{
	switch( GetLoyaltyOrSmartPayType() )
	{
	case LOYALTY_TYPE_LOY_V2:
	case LOYALTY_TYPE_LOY_V3:
	case LOYALTY_TYPE_LOY_V4:
		return TRUE;

	default:
		return FALSE;
	}
}

/**********************************************************************/

bool CSysInfo::GotSmartPayLink()
{
	switch( GetLoyaltyOrSmartPayType() )
	{
	case LOYALTY_TYPE_SMP_V1:
	case LOYALTY_TYPE_SMP_V2:
	case LOYALTY_TYPE_SMP_V4:
		return TRUE;

	default:
		return FALSE;
	}
}

/**********************************************************************/

bool CSysInfo::IsRetailStockSystem()
{
	return Sysset.IsRetailStockSystem();
}

/**********************************************************************/

void CSysInfo::EstimateStackUsage()
{
	char c;

	DWORD dwSize = ( m_dwStackBase - DWORD(&c) );

	CString str;
	str.Format( "Estimated stack size %dk", dwSize / 1024 );
	Prompter.Diagnostic( str );
}

/**********************************************************************/

int CSysInfo::GetExternalLink()
{
	return Sysset.GetExternalLink();
}

/**********************************************************************/

int CSysInfo::GetMaxPresetTasks()
{
	if ( IsEcrLiteSystem() )
		return 2;
	else
		return 20;
}

/**********************************************************************/
