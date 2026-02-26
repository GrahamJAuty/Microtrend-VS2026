/**********************************************************************/
#include "..\SPosStockManager5\SiteCSVArray.h"
#include "..\SPosStockManager5\StockpointCSVArray.h"
/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "KeyboardSetCSVArray.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "PMSOptions.h"
#include "SysInfo.h"
/**********************************************************************/
#include "FilenameUpdater.h"
/**********************************************************************/

CFilenameUpdater::CFilenameUpdater( SysFileInfo info, int nIndex, int nSubFile, int nFolderFamily )
{
	m_strFilenameToUse = "";

	if (nFolderFamily == -1)
	{
		nFolderFamily = info.m_nFamily;
	}

	CString strFamily = "";
	switch( info.m_nFamily )
	{
	case FNUP_FAMILY_SYSTEM:			strFamily = "0";	break;
	case FNUP_FAMILY_REPORT:			strFamily = "1";	break;
	case FNUP_FAMILY_DATABASE:			strFamily = "2";	break;
	case FNUP_FAMILY_LOCATION:			strFamily = "3";	break;
	case FNUP_FAMILY_STKPOINT:			strFamily = "4";	break;
	case FNUP_FAMILY_PC_HOST:			strFamily = "5";	break;
	case FNUP_FAMILY_PC_CLIENT:			strFamily = "5";	break;
	case FNUP_FAMILY_INSTANCE:			strFamily = "6";	break;
	case FNUP_FAMILY_ECRREP:			strFamily = "7";	break;
	case FNUP_FAMILY_STOCKAUDIT:		strFamily = "8";	break;
	case FNUP_FAMILY_LOC_COMMS:			strFamily = "9";	break;
	//UNUSED FAMILY A
	case FNUP_FAMILY_SITE:				strFamily = "B";	break;
	case FNUP_FAMILY_NETWORK:			strFamily = "C";	break;
	case FNUP_FAMILY_SYSSET:			strFamily = "D";	break;
	case FNUP_FAMILY_PMS_SYSFILE:		strFamily = "E";	break;
	case FNUP_FAMILY_PMS_BOOKING:		strFamily = "F";	break;
	case FNUP_FAMILY_KBSET:				strFamily = "G";	break;
	case FNUP_FAMILY_PMS_CUSTOMER:		strFamily = "H";	break;
	//UNUSED FAMILY I
	case FNUP_FAMILY_IDRAUGHT:			strFamily = "J";	break;
	case FNUP_FAMILY_SAGE:				strFamily = "K";	break;
	case FNUP_FAMILY_EMAIL:				strFamily = "L";	break;
	case FNUP_FAMILY_FNB:				strFamily = "M";	break;
	case FNUP_FAMILY_TERM:				strFamily = "N";	break;
	case FNUP_FAMILY_S4LABOUR:			strFamily = "P";	break;
	case FNUP_FAMILY_LIPA:				strFamily = "Q";	break;
	case FNUP_FAMILY_CHARTWELLS:		strFamily = "R";	break;
	case FNUP_FAMILY_LAYOUT:			strFamily = "Z";	break;
	default:							strFamily = "-";	break;
	}

	CString strFilename;
	CString strFileNo;

	switch( info.m_nFamily )
	{
	case FNUP_FAMILY_ECRREP:
		strFilename = "FL";
		strFileNo.Format ( "%2.2d%3.3d", nIndex, nSubFile );
		break;

	case FNUP_FAMILY_STOCKAUDIT:
		strFilename = "FILE";
		strFileNo.Format( "%3.3d", nSubFile );
		break;

	default:
		if ( ( nSubFile >= 0 ) && ( nFolderFamily != FNUP_FAMILY_TERM ) )
		{
			strFilename = "FL";
			strFileNo.Format ( "%3.3d%2.2X", info.m_nFileNo, nSubFile );
		}
		else
		{
			strFilename = "FILE";
			strFileNo.Format ( "%3.3d", info.m_nFileNo );
		}
		break;
	}

	switch( SysInfo.GetMaxBasePluLen() )
	{
	case 0:		
		//LEGACY MODE LITERAL DATA
		if (SysInfo.GetAltFilenamesIfNoModifiersFlag() == TRUE)
		{
			strFilename += "8";
		}
		break; 
	}
	
	strFilename += strFamily;
	strFilename += strFileNo;

	if (nFolderFamily != FNUP_FAMILY_TERM)
	{
		strFilename += ".dat";
	}
	else
	{
		CString strExtra = "";
		strExtra.Format( ".%3.3d", nSubFile );
		strFilename += strExtra;
	}

	switch( nFolderFamily )
	{
	case FNUP_FAMILY_SYSTEM:
	case FNUP_FAMILY_REPORT:
	case FNUP_FAMILY_ECRREP:
	case FNUP_FAMILY_STOCKAUDIT:
		m_strFilenameToUse = "SysFiles\\";
		m_strFilenameToUse += strFilename;
		GetDataProgramPath( m_strFilenameToUse );
		break;

	case FNUP_FAMILY_SYSSET:
		m_strFilenameToUse = "SysFiles\\";
		m_strFilenameToUse += strFilename;
		GetSyssetProgramPath( m_strFilenameToUse );
		break;

	case FNUP_FAMILY_LAYOUT:
		m_strFilenameToUse = "Layout\\";
		m_strFilenameToUse += strFilename;
		GetSyssetProgramPath( m_strFilenameToUse );
		break;

	case FNUP_FAMILY_DATABASE:
		if ( nIndex != -1 )
		{
			m_strFilenameToUse = dbDatabase.GetFolderPathData( nIndex );
			m_strFilenameToUse += "\\";
			m_strFilenameToUse += strFilename;
		}
		else
		{
			m_strFilenameToUse = "SysFiles\\";
			m_strFilenameToUse += strFilename;
			GetDataProgramPath( m_strFilenameToUse );
		}
		break;

	case FNUP_FAMILY_LOCATION:
		m_strFilenameToUse = dbLocation.GetFolderPathData( nIndex );
		m_strFilenameToUse += "\\";
		m_strFilenameToUse += strFilename;
		break;
		
	case FNUP_FAMILY_LOC_COMMS:
		m_strFilenameToUse = dbLocation.GetFolderPathSysset( nIndex );
		m_strFilenameToUse += "\\";
		m_strFilenameToUse += strFilename;
		break;

	case FNUP_FAMILY_PC_HOST:
		m_strFilenameToUse = strFilename;
		GetPCPathHost( m_strFilenameToUse );
		break;

	case FNUP_FAMILY_PC_CLIENT:
		m_strFilenameToUse = strFilename;
		GetPCPathClient(m_strFilenameToUse);
		break;

	case FNUP_FAMILY_INSTANCE:
		m_strFilenameToUse = strFilename;
		GetTempPath( m_strFilenameToUse );
		break;

#ifdef STOCKMAN_SYSTEM

	case FNUP_FAMILY_STKPOINT:
		m_strFilenameToUse = dbStockpoint.GetFolderPath( nIndex );
		m_strFilenameToUse += "\\";
		m_strFilenameToUse += strFilename;
		break;

	case FNUP_FAMILY_SITE:
		m_strFilenameToUse = dbSite.GetFolderPath( nIndex );
		m_strFilenameToUse += "\\";
		m_strFilenameToUse += strFilename;
		break;

#endif

	case FNUP_FAMILY_NETWORK:
	case FNUP_FAMILY_TERM:
		if (nIndex >= 0)
		{
			m_strFilenameToUse = dbNetwork.GetFolderPathData(nIndex);
		}
		else
		{
			m_strFilenameToUse = dbNetwork.GetFolderPathSysset(nIndex);
		}

		m_strFilenameToUse += "\\";
		m_strFilenameToUse += strFilename;
		break;

	case FNUP_FAMILY_KBSET:
		m_strFilenameToUse = dbKeyboardSet.GetFolderPath( nIndex );
		m_strFilenameToUse += "\\";
		m_strFilenameToUse += strFilename;
		break;

#ifdef COMPILE_PMS

	case FNUP_FAMILY_PMS_SYSFILE:
		m_strFilenameToUse = PMSOptions.GetPMSFolderSysFiles();
		m_strFilenameToUse += "\\";
		m_strFilenameToUse += strFilename;
		break;

	case FNUP_FAMILY_PMS_BOOKING:
		m_strFilenameToUse = PMSOptions.GetPMSFolderBooking();
		m_strFilenameToUse += "\\";
		m_strFilenameToUse += strFilename;
		break;

	case FNUP_FAMILY_PMS_CUSTOMER:
		m_strFilenameToUse = PMSOptions.GetPMSFolderCustomer();
		m_strFilenameToUse += "\\";
		m_strFilenameToUse += strFilename;
		break;

#endif

	case FNUP_FAMILY_IDRAUGHT:
		m_strFilenameToUse = "IDraught\\";
		m_strFilenameToUse += strFilename;
		GetDataProgramPath( m_strFilenameToUse );
		break;

	case FNUP_FAMILY_SAGE:
		m_strFilenameToUse = "Sage\\";
		m_strFilenameToUse += strFilename;
		GetDataProgramPath( m_strFilenameToUse );
		break;

	case FNUP_FAMILY_EMAIL:
		m_strFilenameToUse = strFilename;
		break;

	case FNUP_FAMILY_FNB:
		m_strFilenameToUse = dbDatabase.GetFolderPathSysset( nIndex );
		m_strFilenameToUse += "\\FNB\\";
		m_strFilenameToUse += strFilename;
		break;

	case FNUP_FAMILY_CHARTWELLS:
		m_strFilenameToUse = "CHARTWELLS\\";
		m_strFilenameToUse += strFilename;
		GetSyssetProgramPath(m_strFilenameToUse);
		break;

	case FNUP_FAMILY_S4LABOUR:
		m_strFilenameToUse = dbLocation.GetFolderPathSysset( nIndex );
		m_strFilenameToUse += "\\S4LABOUR\\";
		m_strFilenameToUse += strFilename;
		break;

	case FNUP_FAMILY_LIPA:
		m_strFilenameToUse = "LIPA\\";
		::GetSyssetProgramPath( m_strFilenameToUse );
		m_strFilenameToUse += strFilename;
		break;

	default:
		m_strFilenameToUse = "";
		GetDataProgramPath( m_strFilenameToUse );
		break;
	}
}

/**********************************************************************/

const char* CFilenameUpdater::GetInstanceFilenameToUse()
{
	return GetInstanceFilenameToUse( Sysset.GetInstanceNo() );
}

/**********************************************************************/

const char* CFilenameUpdater::GetInstanceFilenameToUse( int nInstanceNo )
{
	int nLength = m_strFilenameToUse.GetLength();
	
	if ((1 == nInstanceNo) || (nLength < 5))
	{
		m_strInstanceFilenameToUse = m_strFilenameToUse;
	}
	else
	{
		m_strInstanceFilenameToUse = m_strFilenameToUse.Left( nLength - 4 );

		CString strNum = "";
		strNum.Format( "%2.2d", nInstanceNo );
		
		m_strInstanceFilenameToUse += strNum;
		m_strInstanceFilenameToUse += ".dat";
	}

	return m_strInstanceFilenameToUse;
}

/**********************************************************************/


