//******************************************************************************
#include "ParentMailData.h"
//******************************************************************************

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParentMailData::CParentMailData()
{
	m_bKeepXmlLogin			= FALSE;
	m_bKeepXmlFiles			= FALSE;
	m_strSoapVersion		= "1.1";
	m_strUsername			= "";
	m_strPassword			= "";
	m_strSchoolID			= "";
//	m_nServiceLevel			= 1;			// o=ParentMail1, 1=ParentMail2
	m_nLastPaymentID		= 0;
	m_nLastTransactionID	= 0;
	m_nAccountPurse			= 1;
	m_nFormNameRefField		= 0;			// 0=not defined, Info1-8
	m_nAdditionalRefField	= 0;					
	m_nMaxTransactionLines	= 200;
	m_strStaffGroupList		= "";

	CString strPath = System.GetWebPaymentPath ( "", nWEBPAYMENT_PARENTMAIL );	// create "P44\ParentPay" folder if not found
	if ( ::ExistSubdirectory ( strPath ) == FALSE )
		::MakeSubdirectory ( strPath );

	m_strFilename = System.GetWebPaymentPath ( "ParentMail.dat", nWEBPAYMENT_PARENTMAIL );
	m_arrayStaffList.RemoveAll();
}

//******************************************************************

void CParentMailData::Read()
{
	CSSIniFile file ( SS_INI_AES );
	if ( file.Read ( m_strFilename ) == TRUE )
	{
		m_bKeepXmlLogin			= file.GetBool  ( "KeepXmlLogin", m_bKeepXmlLogin );
		m_bKeepXmlFiles			= file.GetBool  ( "DemoSystem", m_bKeepXmlFiles );
		m_strSoapVersion		= file.GetString( "SoapVersion", m_strSoapVersion );
		m_strUsername			= file.GetString ( "Username", m_strUsername );
		m_strPassword			= file.GetString ( "Password", m_strPassword );
		m_strSchoolID			= file.GetString ( "SchoolID", m_strSchoolID );
		m_nLastPaymentID		= file.GetInt64 ( "LastPaymentID", m_nLastPaymentID );
		m_nLastTransactionID	= file.GetInt64 ( "LastTransactionID", m_nLastTransactionID );
		m_nAccountPurse			= file.GetInt  ( "AccountPurse", m_nAccountPurse );
		m_nFormNameRefField		= file.GetInt  ( "FormNameRefField", m_nFormNameRefField );		// 0=Not defined, Info1-8
		m_nAdditionalRefField	= file.GetInt  ( "AdditionalRefField", m_nAdditionalRefField );		// 0=Not defined, Info1-8
		m_nMaxTransactionLines	= file.GetInt ( "MaxTransactionLines", m_nMaxTransactionLines);
		m_strStaffGroupList		= file.GetString ( "StaffList", m_strStaffGroupList );
	}
}

//*********************************************************************

void CParentMailData::Save()
{
	CSSIniFile file ( SS_INI_AES );

	file.Set ( "KeepXmlLogin", m_bKeepXmlLogin );
	file.Set ( "DemoSystem", m_bKeepXmlFiles );
	file.Set ( "SoapVersion", m_strSoapVersion );
	file.Set ( "Username", m_strUsername );
	file.Set ( "Password", m_strPassword );
	file.Set ( "SchoolID", m_strSchoolID );
	file.Set ( "LastPaymentID", m_nLastPaymentID );	
	file.Set ( "LastTransactionID", m_nLastTransactionID );
	file.Set ( "AccountPurse", m_nAccountPurse );
	file.Set ( "FormNameRefField", m_nFormNameRefField );
	file.Set ( "AdditionalRefField", m_nAdditionalRefField );
	file.Set ( "MaxTransactionLines", m_nMaxTransactionLines );
	file.Set ( "StaffList", m_strStaffGroupList );

	file.Write ( m_strFilename );
}

//******************************************************************

bool CParentMailData::IsSet()
{
	if ( m_strSchoolID == "" )	return FALSE;
	if ( m_strUsername == "" )	return FALSE;
	if ( m_strPassword == "" )	return FALSE;
	return TRUE;
}

//******************************************************************
// get full path of where to save demo system login file

CString CParentMailData::GetXmlLoginPath ( const char* szFilename )
{
	CString strPathname = System.GetWebPaymentPath ( szFilename, nWEBPAYMENT_PARENTMAIL );
	return strPathname;	
}

//*********************************************************************
// 20,21,22-30

bool CParentMailData::ValidateStaffList ( const char* szText )
{
	bool bReply = TRUE;
	CString strList = szText;

	for ( int i = 0 ; i < strList.GetLength() ; i++ )
	{
		unsigned char uc = strList.GetAt(i);

		if ( uc == ',' || uc == '-' || uc == ' ' )				// see if separator
			continue;

		if ( uc < '0' || uc > '9' )
		{
			bReply = FALSE;
			break;
		}
	}
	return bReply;
}

//*********************************************************************

bool CParentMailData::IsStaffGroup ( int nGroupNo )
{
	if ( m_strStaffGroupList == "" )						// no staff list setup
		return FALSE;

	if ( m_arrayStaffList.GetSize() == 0 )					// on firsttim etrough setup array
	{
		CCSV csv ( m_strStaffGroupList );
	
		for ( int i = 0 ; i < csv.GetSize() ; i++ )
		{
			if ( csv.IsEmpty(i) == FALSE )
			{
				CString strItem = csv.GetString ( i );

				int nPtr;
				if ( ( nPtr = strItem.Find ( '-' ) ) != -1 )		// index to '-' in '1-12' ie range 1 to 12
				{
					CString strStart = strItem.Left ( nPtr );
					++nPtr;
					CString strEnd = strItem.Mid ( nPtr, strItem.GetLength()-nPtr ); 

					if ( strStart != "" && strEnd != "" )
					{
						for ( int nNo = atoi(strStart) ; nNo <= atoi(strEnd) ; nNo++ )
						{
							if ( nNo > 0 )
								m_arrayStaffList.Add ( nNo ) ;
						}
					}
				}
				else
				{
					int nNo = csv.GetInt(i);
					if ( nNo > 0 )
						m_arrayStaffList.Add ( nNo );
				}
			}
		}
	}

	bool bReply = FALSE;
	for ( int i = 0 ; i < m_arrayStaffList.GetSize() ; i++ )
	{
		if ( nGroupNo == m_arrayStaffList.GetAt(i) )
		{
			bReply = TRUE;
			break;
		}
	}
	return bReply;
}

//*********************************************************************
