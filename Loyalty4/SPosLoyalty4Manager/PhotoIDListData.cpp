/******************************************************************************/
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
/******************************************************************************/
#include "LoyaltyManager.h"
/******************************************************************************/
#include "PhotoIDListData.h"
/******************************************************************************/
#define nVERSION 2		// 6 fields
/******************************************************************************/

static const char* szDEFAULTLABEL = "SmartPhotoID";

/******************************************************************************/

CPhotoIDListData::CPhotoIDListData()
{
	m_strLabelName	= szDEFAULTLABEL;
	m_strFilename	= Filenames.GetPhotoIDListFilename();
	m_strBarmanFilename.Format ( "%s\\BarDat\\Barman.dat", Sysset.GetProgramPath() );

	m_strFieldData = "";								// return value string
	ResetData();

	ReadDefault();										// read default label data
}

/******************************************************************************/

void CPhotoIDListData::ResetData()
{
	m_strField1 = Account::UserID.Label;
	m_strField2 = Account::Fullname.Label;
	m_strField3 = Account::Blank.Label;
	m_strField4 = Account::Blank.Label;
	m_strField5 = Account::Blank.Label;
	m_strField6 = Account::Blank.Label;

	m_strFieldText1 = "";
	m_strFieldText2 = "";
	m_strFieldText3 = "";
	m_strFieldText4 = "";
	m_strFieldText5 = "";
	m_strFieldText6 = "";
}

//**********************************************************************

void CPhotoIDListData::ExtractCSVLine ( CCSV* csv )
{
	m_strLabelName		= csv->GetString ( 0 );

	switch ( csv->GetInt ( 1 ) )
	{
	case 1:		m_strField1			= csv->GetString ( 2 );
				m_strFieldText1		= csv->GetString ( 3 );
				m_strField2			= csv->GetString ( 4 );
				m_strFieldText2		= csv->GetString ( 5 );
				m_strField3			= csv->GetString ( 6 );
				m_strFieldText3		= csv->GetString ( 7 );
				m_strField4			= csv->GetString ( 8 );
				m_strFieldText4		= csv->GetString ( 9 );
				break;

	case 2:		m_strField1			= csv->GetString ( 2 );
				m_strFieldText1		= csv->GetString ( 3 );
				m_strField2			= csv->GetString ( 4 );
				m_strFieldText2		= csv->GetString ( 5 );
				m_strField3			= csv->GetString ( 6 );
				m_strFieldText3		= csv->GetString ( 7 );
				m_strField4			= csv->GetString ( 8 );
				m_strFieldText4		= csv->GetString ( 9 );
				m_strField5			= csv->GetString ( 10 );
				m_strFieldText5		= csv->GetString ( 11 );
				m_strField6			= csv->GetString ( 12 );
				m_strFieldText6		= csv->GetString ( 13 );
				break;
	}
}

/**********************************************************************/

bool CPhotoIDListData::SaveDefault()
{
	RemoveLabel ( m_strLabelName );					// delete old line if found

	CFileStringArray fileArray;
	fileArray.Read( m_strFilename );

	CCSV csv;
	csv.Add ( m_strLabelName );
	csv.Add ( nVERSION );
	csv.Add ( m_strField1 );
	csv.Add ( m_strFieldText1 );
	csv.Add ( m_strField2 );
	csv.Add ( m_strFieldText2 );
	csv.Add ( m_strField3 );
	csv.Add ( m_strFieldText3 );
	csv.Add ( m_strField4 );
	csv.Add ( m_strFieldText4 );
	csv.Add ( m_strField5 );
	csv.Add ( m_strFieldText5 );
	csv.Add ( m_strField6 );
	csv.Add ( m_strFieldText6 );

	fileArray.InsertAt ( 0, csv.GetLine() );		// insert at beginning of file
	return fileArray.Write( m_strFilename );
}

/**********************************************************************/
// first one in file is default to use

bool CPhotoIDListData::ReadDefault()
{
	bool bReply = FALSE;

	CSSFile file;
	if ( file.Open ( m_strFilename, "rb" ) == TRUE )		// may not exist
	{
		CCSV csv;
		if ((bReply = file.Read(csv)) == TRUE)
		{
			ExtractCSVLine(&csv);
		}
	}

	return bReply;
}

//**********************************************************************

bool CPhotoIDListData::MakeDefault ( const char* szLabel )
{
	if (Load(szLabel) == TRUE)					// see if label exists
	{
		return SaveDefault();					// make first in file & save
	}

	return FALSE;
}

/**********************************************************************/

bool CPhotoIDListData::Load ( const char* szLabel )
{
	CFileStringArray fileArray;
	fileArray.Read( m_strFilename );

	int nIndex;
	for ( nIndex = 0; nIndex < fileArray.GetSize(); nIndex++ )
	{
		CCSV csv ( fileArray.GetAt( nIndex ) );
		CString strLabel = csv.GetString(0);
		
		if ( strLabel == szLabel )
		{
			ExtractCSVLine ( &csv );
			return TRUE;
		}
	}

	ResetData();								// not found
	return FALSE;
}

/**********************************************************************/

bool CPhotoIDListData::RemoveLabel ( const char* szLabel )
{
	CFileStringArray fileArray;
	fileArray.Read( m_strFilename );

	for ( int nIndex = 0; nIndex < fileArray.GetSize(); nIndex++ )
	{
		CCSV csv ( fileArray.GetAt( nIndex ) );
		CString strLabel = csv.GetString(0);
		
		if ( strLabel == szLabel )
		{
			fileArray.RemoveAt ( nIndex );				// delete old line if found
			return fileArray.Write( m_strFilename );
		}
	}

	return FALSE;
}

//*********************************************************************
//*********************************************************************

const char* CPhotoIDListData::GetFieldData ( CSQLRowAccountFull& RowAccount, int nFieldNo )
{
	CString strFieldData = "";
	CString strFieldText = "";
	CString strField = "";

	switch ( nFieldNo )
	{
	case 1:		strField		= m_strField1;
				strFieldText	= m_strFieldText1;
				break;

	case 2:		strField		= m_strField2;
				strFieldText	= m_strFieldText2;
				break;

	case 3:		strField		= m_strField3;
				strFieldText	= m_strFieldText3;
				break;

	case 4:		strField		= m_strField4;
				strFieldText	= m_strFieldText4;
				break;

	case 5:		strField		= m_strField5;
				strFieldText	= m_strFieldText5;
				break;

	case 6:		strField		= m_strField6;
				strFieldText	= m_strFieldText6;
				break;
	}

	switch (LoyaltyHeaderHelpers.GetNumericField(strField))
	{
	case LOYALTYFIELD_USERID:
		strFieldData = FormatCardNo(RowAccount.GetUserID(), FALSE);
		break;

	case LOYALTYFIELD_FULLCARDNO:
		strFieldData = FormatCardNo(RowAccount.GetUserID(), TRUE);
		break;

	case LOYALTYFIELD_FULLNAME:
		strFieldData = RowAccount.GetFullname();
		break;

	case LOYALTYFIELD_GROUPNO:
		strFieldData.Format("%2.2d", RowAccount.GetGroupNo());
		break;

	case LOYALTYFIELD_GROUPCARDNO:
		strFieldData.Format("%2.2d : %s", RowAccount.GetGroupNo(), FormatCardNo(RowAccount.GetUserID(), FALSE));
		break;

	case LOYALTYFIELD_GROUPFULLCARDNO:
		strFieldData.Format("%2.2d : %s", RowAccount.GetGroupNo(), FormatCardNo(RowAccount.GetUserID(), TRUE));
		break;

	case LOYALTYFIELD_EXPIRYDATE:
		strFieldData = RowAccount.GetExpiryDate();
		break;

	case LOYALTYFIELD_DOB:
		strFieldData = RowAccount.GetDOB();
		break;

	case LOYALTYFIELD_ADDRESS1:
		strFieldData = RowAccount.GetAddress1();
		break;

	case LOYALTYFIELD_ADDRESS2:
		strFieldData = RowAccount.GetAddress2();
		break;

	case LOYALTYFIELD_ADDRESS3:
		strFieldData = RowAccount.GetAddress3();
		break;

	case LOYALTYFIELD_ADDRESS4:
		strFieldData = RowAccount.GetAddress4();
		break;

	case LOYALTYFIELD_ADDRESS5:
		strFieldData = RowAccount.GetAddress5();
		break;

	case LOYALTYFIELD_PHONE1:
		strFieldData = RowAccount.GetPhone1();
		break;

	case LOYALTYFIELD_PHONE2:
		strFieldData = RowAccount.GetPhone2();
		break;

	case LOYALTYFIELD_INFO1:
		strFieldData = RowAccount.GetInfo1();
		break;

	case LOYALTYFIELD_INFO2:
		strFieldData = RowAccount.GetInfo2();
		break;

	case LOYALTYFIELD_INFO3:
		strFieldData = RowAccount.GetInfo3();
		break;

	case LOYALTYFIELD_INFO4:
		strFieldData = RowAccount.GetInfo4();
		break;
	}
	m_strFieldData = "";
	if ( strFieldData.GetLength() > 0 )
	{
		if (strFieldText == "" || strFieldText == Account::Blank.Label)
		{
			m_strFieldData = strFieldData;
		}
		else
		{
			m_strFieldData.Format("%s%s", strFieldText, strFieldData);
		}
	}
	
	return m_strFieldData;
}

//*********************************************************************

CString CPhotoIDListData::FormatCardNo ( __int64 nUserID, bool bFullCardNo )
{
	CString strCardNo = "";
	if (bFullCardNo == TRUE)
	{
		strCardNo = System.FormatFullCardNo(SimpleFormatInt64Value(nUserID));
	}
	else
	{
		strCardNo = System.FormatCardNo(SimpleFormatInt64Value(nUserID));
	}

	return strCardNo;
}

//*********************************************************************

const char* CPhotoIDListData::GetTestFieldData ( int nFieldNo )
{
	CString strFieldData	= "";
	CString strFieldText	= "";
	CString strField		= "";

	switch ( nFieldNo )
	{
	case 1:	strField		= m_strField1;
			strFieldText	= m_strFieldText1;
			break;

	case 2:	strField		= m_strField2;
			strFieldText	= m_strFieldText2;
			break;

	case 3:	strField		= m_strField3;
			strFieldText	= m_strFieldText3;
			break;

	case 4:	strField		= m_strField4;
			strFieldText	= m_strFieldText4;
			break;

	case 5:	strField		= m_strField5;
			strFieldText	= m_strFieldText5;
			break;

	case 6:	strField		= m_strField6;
			strFieldText	= m_strFieldText6;
			break;
	}

	switch (LoyaltyHeaderHelpers.GetNumericField(strField))
	{
	case LOYALTYFIELD_USERID:
		strFieldData = System.GetHighCardNo();
		break;

	case LOYALTYFIELD_FULLCARDNO:
		strFieldData = "999999999999";
		break;

	case LOYALTYFIELD_FULLNAME:
		strFieldData = "<cardname here>";
		break;

	case LOYALTYFIELD_GROUPNO:
		strFieldData = "99";
		break;

	case LOYALTYFIELD_GROUPCARDNO:
		strFieldData.Format("99 : %s", System.GetHighCardNo());
		break;

	case LOYALTYFIELD_GROUPFULLCARDNO:
		strFieldData = "99 : 999999999999";
		break;

	case LOYALTYFIELD_EXPIRYDATE:
		strFieldData = "dd/mm/yyyy";
		break;

	case LOYALTYFIELD_DOB:
		strFieldData = "dd/mm/yyyy";
		break;

	case LOYALTYFIELD_ADDRESS1:
		strFieldData = "<address1 here>";
		break;

	case LOYALTYFIELD_ADDRESS2:
		strFieldData = "<address2 here>";
		break;

	case LOYALTYFIELD_ADDRESS3:
		strFieldData = "<address3 here>";
		break;

	case LOYALTYFIELD_ADDRESS4:
		strFieldData = "<address4 here>";
		break;

	case LOYALTYFIELD_ADDRESS5:
		strFieldData = "<address5 here>";
		break;

	case LOYALTYFIELD_PHONE1:
		strFieldData = "<phone1 here>";
		break;

	case LOYALTYFIELD_PHONE2:
		strFieldData = "<phone2 here>";
		break;

	case LOYALTYFIELD_INFO1:
		strFieldData = "<info1 here>";
		break;

	case LOYALTYFIELD_INFO2:
		strFieldData = "<info2 here>";
		break;

	case LOYALTYFIELD_INFO3:
		strFieldData = "<info3 here>";
		break;

	case LOYALTYFIELD_INFO4:
		strFieldData = "<info4 here>";
		break;

	case LOYALTYFIELD_TEXT:
		{
			strFieldData = strFieldText;
			strFieldText = "";
		}
		break;
	}

	m_strFieldData = "";
	if ((strFieldText == "" )|| (strFieldText == Account::Blank.Label))
	{
		m_strFieldData = strFieldData;
	}
	else
	{
		m_strFieldData.Format("%s%s", strFieldText, strFieldData);
	}

	return m_strFieldData;
}

//*********************************************************************

const char* CPhotoIDListData::GetReportData(CSQLRowAccountFull& RowAccount, const char* szImageFilename)
{
	CCSV csv;
	csv.Add(GetFieldData(RowAccount, 1));		// description (Field1)
	csv.Add(GetFieldData(RowAccount, 4));		// price (Field4)
	csv.Add(GetFieldData(RowAccount, 2));		// pluno (Field2)
	csv.Add("");								// barcode
	csv.Add(GetFieldData(RowAccount, 6));		// unit price (Field6)
	csv.Add(szImageFilename);					// image 
	csv.Add(GetFieldData(RowAccount, 3));		// stockcode (Field3)
	csv.Add(GetFieldData(RowAccount, 5));		// price2 (Field5)

	m_strFieldData = csv.GetLine();
	return m_strFieldData;
}

//*********************************************************************

const char* CPhotoIDListData::GetTestReportData()
{
	CCSV csv;
	csv.Add ( GetTestFieldData ( 1 ) );				// description
	csv.Add ( GetTestFieldData ( 4 ) );				// price
	csv.Add ( GetTestFieldData ( 2 ) );				// pluno
	csv.Add ( "" );									// barcode
	csv.Add ( GetTestFieldData ( 6 ) );				// unit price
	csv.Add ( "" );									// image
	csv.Add ( GetTestFieldData ( 3 ) );				// stockcode
	csv.Add ( GetTestFieldData ( 5 ) );				// price2

	m_strFieldData = csv.GetLine();
	return m_strFieldData;
}

//**********************************************************************
//**********************************************************************

int CPhotoIDListData::CompileLabelCombo ( CSSComboBox* pCombo, bool bFullList )
{
	if (bFullList == FALSE)
	{
		ValidateSmartcardList();
	}

	pCombo->ResetContent();
	int nDefaultOpt = 0;
	int nCount = 0;

	int nOffset = 1;
	CString strFilename = m_strBarmanFilename;	// use full list of barman labels
	if ( bFullList == FALSE )
	{
		nOffset = 0;
		strFilename = m_strFilename;			// use only those setup by smartcard
	}

	CSSFile fileLabels;
	if ( fileLabels.Open ( strFilename, "rb" ) == TRUE )
	{
		int nOpt = 0;
		CString strBuf;
		while ( fileLabels.ReadString ( strBuf) == TRUE )
		{
			CCSV csv ( strBuf );
			CString strLabel = csv.GetString(nOffset);
			pCombo->AddString ( strLabel );
			++nCount;
	
			if (strLabel == m_strLabelName)
			{
				nDefaultOpt = nOpt;
			}
			else
			{
				++nOpt;
			}
		}
		fileLabels.Close();
	}
	else
	{
		pCombo->AddString(szDEFAULTLABEL);
	}

	pCombo->SetCurSel(nDefaultOpt);
	return ( nCount );
}

//**********************************************************************
// work thru smartcard list & remove entries that no longer exist in barman label list

void CPhotoIDListData::ValidateSmartcardList()
{
	CFileStringArray arraySmart;					// array of smartcard labels
	arraySmart.Read( m_strFilename );

	CFileStringArray arrayBarman;					// full array of barman labels
	arrayBarman.Read( m_strBarmanFilename );

	for ( int nIndex = 0; nIndex < arraySmart.GetSize(); nIndex++ )
	{
		CCSV csvSmart ( arraySmart.GetAt( nIndex ) );
		CString strLabel = csvSmart.GetString(0);
		
		int i;
		for ( i = 0; i < arrayBarman.GetSize(); i++ )
		{
			CCSV csvBarman ( arrayBarman.GetAt( i ) );

			if ( strLabel == csvBarman.GetString(1) )
				break;
		}

		if (i >= arrayBarman.GetSize())
		{
			RemoveLabel(strLabel);					// label does not exist anymore
		}
	}
}

//**********************************************************************

