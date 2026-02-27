/******************************************************************************/
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\SmartPayHeaderHelpers.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
/******************************************************************************/
#include "PhotoIDListData.h"
/******************************************************************************/
#define nVERSION			1		// 6 fields
/******************************************************************************/
static const char* szDEFAULTLABEL = "SmartPhotoID";
/******************************************************************************/

CPhotoIDListData::CPhotoIDListData()
{
	m_strLabelName = szDEFAULTLABEL;
	m_strFilename = Filenames.GetPhotoIDListFilename();
	m_strBarmanFilename.Format("%s\\BarDat\\Barman.dat", Sysset.GetProgramPath());

	m_strFieldData = "";								// return value string
	ResetData();

	m_UserText.Open(Filenames.GetUserTypeFilename());
	Server.Read(Filenames.GetServerDataFilename());
	m_AlertText.Open(Filenames.GetAlertFilename());			// readonly

	ReadDefault();										// read default label data
}

/******************************************************************************/

void CPhotoIDListData::ResetData()
{
	m_strField1 = Account::UserID.Label;
	m_strField2 = Account::Username.Label;
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
//  m_nVersionNo		= csv->GetInt(1);
	m_strField1			= csv->GetString ( 2 );
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
		if ( ( bReply = file.Read ( csv ) ) == TRUE )
			ExtractCSVLine ( &csv );
	}

	return bReply;
}

/******************************************************************************/

bool CPhotoIDListData::MakeDefault ( const char* szLabel )
{
	if ( Load ( szLabel ) == TRUE )					// see if label exists
		return SaveDefault();						// make first in file & save

	return FALSE;
}

/******************************************************************************/

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

/******************************************************************************/

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

const char* CPhotoIDListData::GetFieldData(int nFieldNo, CSQLRowAccountFull& RowAccount)
{
	CString strFieldData = "";
	CString strFieldText = "";
	CString strField = "";

	switch (nFieldNo)
	{
	case 1:		
		strField = m_strField1;
		strFieldText = m_strFieldText1;
		break;

	case 2:		
		strField = m_strField2;
		strFieldText = m_strFieldText2;
		break;

	case 3:		
		strField = m_strField3;
		strFieldText = m_strFieldText3;
		break;

	case 4:		
		strField = m_strField4;
		strFieldText = m_strFieldText4;
		break;

	case 5:		
		strField = m_strField5;
		strFieldText = m_strFieldText5;
		break;

	case 6:		
		strField = m_strField6;
		strFieldText = m_strFieldText6;
		break;
	}

	int nFieldType = SmartPayHeaderHelpers.GetNumericField(strField);
	switch (nFieldType)
	{
	case SMARTPAYFIELD_USERID:
		strFieldData = FormatCardNo(RowAccount, FALSE);
		break;

	case SMARTPAYFIELD_MEMBERID:
		strFieldData = RowAccount.GetMemberID();
		break;

	case SMARTPAYFIELD_FULLUSERID:
		strFieldData = FormatCardNo(RowAccount, TRUE);
		break;

	case SMARTPAYFIELD_USERNAME:
		strFieldData = RowAccount.GetUsername();
		break;

	case SMARTPAYFIELD_FORENAME:
		strFieldData = RowAccount.GetForename();
		break;

	case SMARTPAYFIELD_SURNAME:
		strFieldData = RowAccount.GetSurname();
		break;

	case SMARTPAYFIELD_GROUPNO:
		strFieldData.Format("%2.2d", RowAccount.GetGroupNo());
		break;

	case SMARTPAYFIELD_GROUP_USERID:
		strFieldData.Format("%2.2d : %s", RowAccount.GetGroupNo(), FormatCardNo(RowAccount, FALSE));
		break;

	case SMARTPAYFIELD_GROUP_FULLUSERID:
		strFieldData.Format("%2.2d : %s", RowAccount.GetGroupNo(), FormatCardNo(RowAccount, TRUE));
		break;

	case SMARTPAYFIELD_EXPIRYDATE:
		strFieldData = RowAccount.GetExpiryDate();
		break;

	case SMARTPAYFIELD_DOB:
		strFieldData = RowAccount.GetDOB();
		break;

	case SMARTPAYFIELD_INFO1:
		strFieldData = RowAccount.GetInfo1();
		break;

	case SMARTPAYFIELD_INFO2:
		strFieldData = RowAccount.GetInfo2();
		break;

	case SMARTPAYFIELD_INFO3:
		strFieldData = RowAccount.GetInfo3();
		break;

	case SMARTPAYFIELD_INFO4:
		strFieldData = RowAccount.GetInfo4();
		break;

	case SMARTPAYFIELD_INFO5:
		strFieldData = RowAccount.GetInfo5();
		break;

	case SMARTPAYFIELD_INFO6:
		strFieldData = RowAccount.GetInfo6();
		break;

	case SMARTPAYFIELD_INFO7:
		strFieldData = RowAccount.GetInfo7();
		break;

	case SMARTPAYFIELD_INFO8:
		strFieldData = RowAccount.GetInfo8();
		break;

	case SMARTPAYFIELD_DIETARY:
		strFieldData = RowAccount.GetDietary();
		break;

	case SMARTPAYFIELD_FORENAME_SURNAME:
	{
		strFieldData = RowAccount.GetForename();

		if (strFieldData != "")
		{
			strFieldData += " ";
		}

		strFieldData += RowAccount.GetSurname();
	}
	break;

	case SMARTPAYFIELD_ALLERGYCODES:
		if (RowAccount.GetAllergyCodes() != 0)
		{
			m_AllergyText.SetAllergyCodes(RowAccount.GetAllergyCodes());	// tranlate codes to a string
			strFieldData = m_AllergyText.GetAllergyString();
		}
		break;

	case SMARTPAYFIELD_ALERTCODE:
		if (RowAccount.IsAlertSet() == TRUE)
		{
			strFieldData = m_AlertText.GetText(RowAccount.GetAlertCode());
		}
		break;

	case SMARTPAYFIELD_GROUP_TYPE:
	case SMARTPAYFIELD_GROUP_TYPE_USERID:
	case SMARTPAYFIELD_GROUP_TYPE_FULLUSERID:
		{
			CSQLRowGroupFull RowGroup;
			RowGroup.SetGroupNo(RowAccount.GetGroupNo());

			CSQLRepositoryGroup RepoGroup;

			if (SMARTPAYFIELD_GROUP_TYPE == nFieldType )
			{
				if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
				{
					strFieldData = m_UserText.GetText(RowGroup.GetUserType());
				}
			}
			else if (SMARTPAYFIELD_GROUP_TYPE_USERID == nFieldType)
			{
				if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
				{
					strFieldData.Format("%s : %s", m_UserText.GetText(RowGroup.GetUserType()), FormatCardNo(RowAccount, FALSE));
				}
			}
			else if (SMARTPAYFIELD_GROUP_TYPE_FULLUSERID == nFieldType)
			{
				if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
				{
					strFieldData.Format("%s : %s", m_UserText.GetText(RowGroup.GetUserType()), FormatCardNo(RowAccount, TRUE));
				}
			}
		}
		break;
	}
	
	m_strFieldData = "";
	if (strFieldData.GetLength() > 0)
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

CString CPhotoIDListData::FormatCardNo ( CSQLRowAccountFull& AccountRow, bool bFull )
{
	CString strCardNo = System.FormatCardNo(AccountRow.GetUserID(), bFull);
	return strCardNo;
}

//*********************************************************************

const char* CPhotoIDListData::GetTestFieldData(int nFieldNo)
{
	CString strFieldData = "";
	CString strFieldText = "";
	CString strField = "";

	switch (nFieldNo)
	{
	case 1:	
		strField = m_strField1;
		strFieldText = m_strFieldText1;
		break;

	case 2:
		strField = m_strField2;
		strFieldText = m_strFieldText2;
		break;

	case 3:
		strField = m_strField3;
		strFieldText = m_strFieldText3;
		break;

	case 4:	
		strField = m_strField4;
		strFieldText = m_strFieldText4;
		break;

	case 5:	
		strField = m_strField5;
		strFieldText = m_strFieldText5;
		break;

	case 6:	
		strField = m_strField6;
		strFieldText = m_strFieldText6;
		break;
	}

	switch (SmartPayHeaderHelpers.GetNumericField(strField))
	{
	case SMARTPAYFIELD_USERID:
		strFieldData = "999999";
		break;

	case SMARTPAYFIELD_MEMBERID:
		strFieldData = "<MemberID...>";
		break;

	case SMARTPAYFIELD_FULLUSERID:
		strFieldData = "999999999999";
		break;

	case SMARTPAYFIELD_USERNAME:
		strFieldData = "<UserName...>";
		break;

	case SMARTPAYFIELD_FORENAME:
		strFieldData = "<Forename...>";
		break;

	case SMARTPAYFIELD_SURNAME:
		strFieldData = "<Surname....>";
		break;

	case SMARTPAYFIELD_FORENAME_SURNAME:
		strFieldData = "<Forename...> <Surname....>";
		break;

	case SMARTPAYFIELD_GROUPNO:
		strFieldData = "99";
		break;

	case SMARTPAYFIELD_GROUP_TYPE:
		strFieldData = "<GroupType>";
		break;

	case SMARTPAYFIELD_GROUP_USERID:
		strFieldData = "99 : 999999";
		break;

	case SMARTPAYFIELD_GROUP_TYPE_USERID:
		strFieldData = "<GroupType> : 999999";
		break;

	case SMARTPAYFIELD_GROUP_FULLUSERID:
		strFieldData = "99 : 999999999999";
		break;

	case SMARTPAYFIELD_GROUP_TYPE_FULLUSERID:
		strFieldData = "<Group type> : 999999999999";
		break;

	case SMARTPAYFIELD_EXPIRYDATE:
	case SMARTPAYFIELD_DOB:
		strFieldData = "dd/mm/yyyy";
		break;

	case SMARTPAYFIELD_INFO1:
		strFieldData = "<Info1...>";
		break;

	case SMARTPAYFIELD_INFO2:
		strFieldData = "<Info2...>";
		break;

	case SMARTPAYFIELD_INFO3:
		strFieldData = "<Info3...>";
		break;

	case SMARTPAYFIELD_INFO4:
		strFieldData = "<Info4...>";
		break;

	case SMARTPAYFIELD_INFO5:
		strFieldData = "<Info5...>";
		break;

	case SMARTPAYFIELD_INFO6:
		strFieldData = "<Info6...>";

	case SMARTPAYFIELD_INFO7:
		strFieldData = "<Info7...>";
		break;

	case SMARTPAYFIELD_INFO8:
		strFieldData = "<Info8...>";
		break;

	case SMARTPAYFIELD_DIETARY:
		strFieldData = "<Dietary...>";
		break;

	case SMARTPAYFIELD_ALLERGYCODES:
		strFieldData = "<Allergies...>";
		break;

	case SMARTPAYFIELD_ALERTCODE:
		strFieldData = "<Alert...>";
		break;

	case SMARTPAYFIELD_TEXT:
		strFieldData = strFieldText;
		strFieldText = "";
		break;
	}
			
	m_strFieldData = "";
	if (strFieldText == "" || strFieldText == Account::Blank.Label)
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
	csv.Add(GetFieldData(1, RowAccount));		// description (Field1)
	csv.Add(GetFieldData(4, RowAccount));		// price (Field4)
	csv.Add(GetFieldData(2, RowAccount));		// pluno (Field2)
	csv.Add("");									// barcode
	csv.Add(GetFieldData(6, RowAccount));		// unit price (Field6)
	csv.Add(szImageFilename);					// image 
	csv.Add(GetFieldData(3, RowAccount));		// stockcode (Field3)
	csv.Add(GetFieldData(5, RowAccount));		// price2 (Field5)

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
		while ( fileLabels.ReadLine ( strBuf) == TRUE )
		{
			CCSV csv ( strBuf );
			CString strLabel = csv.GetString(nOffset);
			pCombo->AddString ( strLabel );
			++nCount;
	
			if ( strLabel == m_strLabelName )	nDefaultOpt = nOpt;
			else								++nOpt;
		}
		fileLabels.Close();
	}
	else
		pCombo->AddString ( szDEFAULTLABEL );

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

		if ( i >= arrayBarman.GetSize() )
			RemoveLabel ( strLabel );					// label does not exist anymore
	}
}

//**********************************************************************

