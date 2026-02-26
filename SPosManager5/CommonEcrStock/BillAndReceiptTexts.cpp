/**********************************************************************/
//#include "DataManager.h"
#include "FileRemover.h"
//#include "globalfunctions.h"
#include "LocationCSVArray.h"
#include "MaxLengths.h"
/**********************************************************************/
#include "BillAndReceiptTexts.h"
/**********************************************************************/

CBillAndReceiptTexts::CBillAndReceiptTexts() 
{
	m_strDataFilename = "";
	m_strLockFilename = "";
	m_strParentFilename = "";
	m_nParentConType = NODE_PLACEHOLDER;
	m_bDataFileExists = FALSE;

	m_BillText1.Create		( "BillText1", MAX_LENGTH_BILL_TEXT, "" );
	m_BillText2.Create		( "BillText2", MAX_LENGTH_BILL_TEXT, "" );
	m_BillText3.Create		( "BillText3", MAX_LENGTH_BILL_TEXT, "" );
	m_BillText4.Create		( "BillText4", MAX_LENGTH_BILL_TEXT, "" );
	m_BillText5.Create		( "BillText5", MAX_LENGTH_BILL_TEXT, "" );
	m_BillText6.Create		( "BillText6", MAX_LENGTH_BILL_TEXT, "" );

	m_ReceiptText1.Create	( "ReceiptText1", MAX_LENGTH_RECEIPT_TEXT, "" );
	m_ReceiptText2.Create	( "ReceiptText2", MAX_LENGTH_RECEIPT_TEXT, "" );
	m_ReceiptText3.Create	( "ReceiptText3", MAX_LENGTH_RECEIPT_TEXT, "" );
	m_ReceiptText4.Create	( "ReceiptText4", MAX_LENGTH_RECEIPT_TEXT, "" );
	m_ReceiptText5.Create	( "ReceiptText5", MAX_LENGTH_RECEIPT_TEXT, "" );
	m_ReceiptText6.Create	( "ReceiptText6", MAX_LENGTH_RECEIPT_TEXT, "" );

	Reset();
	m_bFatalReadError = FALSE;
}

/**********************************************************************/

bool CBillAndReceiptTexts::SetLocSelEntity( CLocationSelectorEntity& LocSelEntity, bool bAllowFallback )
{
	CString strDataFilenameOld = m_strDataFilename;

	m_strDataFilename = "";
	m_strLockFilename = "";
	m_strParentFilename = "";
	m_nParentConType = NODE_PLACEHOLDER;
	m_bDataFileExists = FALSE;

	CWordArray arrayToTry;
	arrayToTry.Add( NODE_LOCATION_TERMINAL );
	arrayToTry.Add( NODE_LOCATION );
	arrayToTry.Add( NODE_DATABASE );
	arrayToTry.Add( NODE_SYSTEM );

	int nStartPos = 3;
	switch( LocSelEntity.GetConType() )
	{
	case NODE_LOCATION_TERMINAL:	nStartPos = 0;	break;
	case NODE_LOCATION:				nStartPos = 1;	break;
	case NODE_DATABASE:				nStartPos = 2;	break;
	case NODE_SYSTEM:
	default:						nStartPos = 3;	break;
	}

	int nEndPos = 3;
	for ( int n = nStartPos; n <= nEndPos; n++ )
	{
		CString strDataFilename = "";
		CString strLockFilename = "";

		switch( arrayToTry.GetAt(n) )
		{
		case NODE_LOCATION_TERMINAL:
			{
				int nLocIdx = LocSelEntity.GetEntityIdx();
				int nTermIdx = LocSelEntity.GetTermIdx();
				int nTNo = dbLocation.GetTNo( nLocIdx, nTermIdx );
				CFilenameUpdater FnUpData( SysFiles::BillReceiptText, nLocIdx, nTNo, FNUP_FAMILY_TERM );
				strDataFilename = FnUpData.GetFilenameToUse();
				CFilenameUpdater FnUpLock( SysFiles::BillReceiptLock, nLocIdx, nTNo, FNUP_FAMILY_TERM );
				strLockFilename = FnUpLock.GetFilenameToUse();
			}
			break;

		case NODE_LOCATION:
			{
				int nLocIdx = LocSelEntity.GetEntityIdx();
				CFilenameUpdater FnUpData( SysFiles::BillReceiptText, nLocIdx, -1, FNUP_FAMILY_LOCATION );
				strDataFilename = FnUpData.GetFilenameToUse();
				CFilenameUpdater FnUpLock( SysFiles::BillReceiptLock, nLocIdx, -1, FNUP_FAMILY_LOCATION );
				strLockFilename = FnUpLock.GetFilenameToUse();
			}
			break;

		case NODE_DATABASE:
			{
				int nDbIdx = LocSelEntity.GetDbIdx();
				CFilenameUpdater FnUpData( SysFiles::BillReceiptText, nDbIdx, -1, FNUP_FAMILY_DATABASE );
				strDataFilename = FnUpData.GetFilenameToUse();
				CFilenameUpdater FnUpLock( SysFiles::BillReceiptLock, nDbIdx, -1, FNUP_FAMILY_DATABASE );
				strLockFilename = FnUpLock.GetFilenameToUse();
			}
			break;
		
		case NODE_SYSTEM:
		default:
			{
				CFilenameUpdater FnUpData( SysFiles::BillReceiptText, -1, -1, FNUP_FAMILY_SYSTEM );
				strDataFilename = FnUpData.GetFilenameToUse();
				CFilenameUpdater FnUpLock( SysFiles::BillReceiptLock, -1, -1, FNUP_FAMILY_SYSTEM );
				strLockFilename = FnUpLock.GetFilenameToUse();
			}
			break;
		}

		if ( FALSE == bAllowFallback )
		{
			if ( n == nStartPos )
			{
				m_strDataFilename = strDataFilename;
				m_strLockFilename = strLockFilename;
				m_bDataFileExists = ::FileExists( m_strDataFilename );
			}
			else
			{
				m_strParentFilename = strDataFilename;
				m_nParentConType = arrayToTry.GetAt(n);
				
				if ( ::FileExists( m_strParentFilename ) == TRUE )	
					break;
			}
		}
		else
		{
			m_strDataFilename = strDataFilename;
			m_strLockFilename = strLockFilename;
			m_bDataFileExists = ::FileExists( m_strDataFilename );

			if ( TRUE == m_bDataFileExists )
				break;
		}
	}

	return ( m_strDataFilename != strDataFilenameOld );
}

/**********************************************************************/

void CBillAndReceiptTexts::CopyFrom( CBillAndReceiptTexts& source )
{
	CEnhancedIniFile iniFile;
	source.PrepareIniFile( iniFile );
	ReloadFromIniFile( iniFile );
}

/**********************************************************************/

void CBillAndReceiptTexts::ReloadFromIniFile( CEnhancedIniFile& iniFile )
{
	iniFile.ReadString( m_BillText1 );
	iniFile.ReadString( m_BillText2 );
	iniFile.ReadString( m_BillText3 );
	iniFile.ReadString( m_BillText4 );
	iniFile.ReadString( m_BillText5 );
	iniFile.ReadString( m_BillText6 );

	iniFile.ReadString( m_ReceiptText1 );
	iniFile.ReadString( m_ReceiptText2 );
	iniFile.ReadString( m_ReceiptText3 );
	iniFile.ReadString( m_ReceiptText4 );
	iniFile.ReadString( m_ReceiptText5 );
	iniFile.ReadString( m_ReceiptText6 );
}

/**********************************************************************/

void CBillAndReceiptTexts::PrepareIniFile( CEnhancedIniFile& iniFile )
{
	iniFile.WriteString( m_BillText1 );
	iniFile.WriteString( m_BillText2 );
	iniFile.WriteString( m_BillText3 );
	iniFile.WriteString( m_BillText4 );
	iniFile.WriteString( m_BillText5 );
	iniFile.WriteString( m_BillText6 );

	iniFile.WriteString( m_ReceiptText1 );
	iniFile.WriteString( m_ReceiptText2 );
	iniFile.WriteString( m_ReceiptText3 );
	iniFile.WriteString( m_ReceiptText4 );
	iniFile.WriteString( m_ReceiptText5 );
	iniFile.WriteString( m_ReceiptText6 );
}

/**********************************************************************/

void CBillAndReceiptTexts::Reset()
{
	m_BillText1.Reset();
	m_BillText2.Reset();
	m_BillText3.Reset();
	m_BillText4.Reset();
	m_BillText5.Reset();
	m_BillText6.Reset();

	m_ReceiptText1.Reset();
	m_ReceiptText2.Reset();
	m_ReceiptText3.Reset();
	m_ReceiptText4.Reset();
	m_ReceiptText5.Reset();
	m_ReceiptText6.Reset();
}

/**********************************************************************/

void CBillAndReceiptTexts::SetDefaults()
{
	Reset();
}

/**********************************************************************/

bool CBillAndReceiptTexts::GotData()
{
	bool bGotData = FALSE;
	for ( int n = 0; n < 12; n++ )
	{
		CString str;
		switch(n)
		{
		case 0:		str = GetBillText1();		break;
		case 1:		str = GetBillText2();		break;
		case 2:		str = GetBillText3();		break;
		case 3:		str = GetBillText4();		break;
		case 4:		str = GetBillText5();		break;
		case 5:		str = GetBillText6();		break;
		case 6:		str = GetReceiptText1();	break;
		case 7:		str = GetReceiptText2();	break;
		case 8:		str = GetReceiptText3();	break;
		case 9:		str = GetReceiptText4();	break;
		case 10:	str = GetReceiptText5();	break;
		case 11:	str = GetReceiptText6();	break;
		}

		if ( str != "" )
		{
			bGotData = TRUE;
			break;
		}
	}
	return bGotData;
}

/**********************************************************************/

bool CBillAndReceiptTexts::Read()
{
	bool bResult = ReadInternal();
	return bResult;
}

/**********************************************************************/

bool CBillAndReceiptTexts::ReadInternal()
{
	if ( ::FileExists( m_strDataFilename ) == TRUE )
	{
		CEnhancedIniFile iniFile( SS_INI_AES );
		if ( iniFile.Read( m_strDataFilename ) == FALSE )
		{
			m_bFatalReadError = TRUE;
			return FALSE;
		}

		ReloadFromIniFile( iniFile );
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

bool CBillAndReceiptTexts::Write()
{
	bool bResult = FALSE;

	CEnhancedIniFile iniFile( SS_INI_AES );
	PrepareIniFile( iniFile );
		
	bResult = iniFile.Write( m_strDataFilename );

	return bResult;
}

/**********************************************************************/

void CBillAndReceiptTexts::DeleteFile()
{
	CFileRemover FileRemover( m_strDataFilename );
}

/**********************************************************************/
