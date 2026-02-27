/*********************************************************************/
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\MembershipDatabase.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLHelpers.h"
/*********************************************************************/
#include "ImportWondeFile.h"
/*********************************************************************/

void CImportWondeFile::Init()
{
	m_bMembershipUpdated = FALSE;
	m_nRead = 0;
	m_nIgnored = 0;
	m_nUsed = 0;
	m_nAdded = 0;
	m_nSkipped = 0;

	m_ImportFlag.SetImportFlag( FALSE );

	m_strError = "";
	m_strImportFilename = "";
	m_strExceptionComment = "";

	m_strReportLabel = "R222";

	m_strPupilStartFrom	= m_pData->m_strPupilStartFrom;
	m_strStaffStartFrom	= m_pData->m_strStaffStartFrom;

	m_nImportMethod	= m_pData->m_nImportMethod;						// can set be externally
}

/*********************************************************************/

CImportWondeFile::~CImportWondeFile()
{
	if ( ( m_ImportFlag.GetImportFlag() == TRUE ) && ( m_nAdded > 0 ) )								// see if new data has been imported
	{
		bool bSaveReqd = FALSE;

		if ( m_pData->m_nNewPupilIntakeIndex == nWONDE_NEWINTAKE_NEXTFREE_INDEX	)
		{
			m_pData->m_strPupilStartFrom = m_strPupilStartFrom.TrimLeft ( '0' );	// save lastfree record used with no leading 0's
			bSaveReqd = TRUE;
		}

		if ( m_pData->m_nNewStaffIntakeIndex == nWONDE_NEWINTAKE_NEXTFREE_INDEX	)
		{
			m_pData->m_strStaffStartFrom = m_strStaffStartFrom.TrimLeft ( '0' );	// save last free record used with no leading 0's
			bSaveReqd = TRUE;
		}

		if (bSaveReqd == TRUE)
		{
			m_pData->Save();
		}
	}
}

//*******************************************************************
// excecute Wonde import

bool CImportWondeFile::Import(const char* szFilename)
{
	m_ImportFlag.SetImportFlag(TRUE);

	bool bReply = FALSE;

	m_strImportFilename = szFilename;
	if (m_strImportFilename == "")
	{
		m_strImportFilename = m_pData->m_strImportFilename;			// use default Wonde import filename if not set
	}

	if (ProcessFile() == nREPORT_NOERROR)							// import reqd					
	{
		if (m_pData->m_bDeleteImportFile == TRUE)					// see if delete file on manual import reqd
		{
			CFileRecycler::SendToRecycleBin(m_strImportFilename);
		}
	}

	bReply = (m_strError == "") ? TRUE : FALSE;

	if (bReply == TRUE)
	{
		ShowStatistics();											// show import stats
	}

	return bReply;
}

/*********************************************************************/
/*** process file for display or import*******************************/
/*********************************************************************/

int CImportWondeFile::ProcessFile()
{
	CString strWondeDatabase = Filenames.GetTempReportPath ( m_strReportLabel, "dat" );
	CFileRecycler::SendToRecycleBin ( strWondeDatabase );						// delete previous temp wonde database import file if found

	if ( ConvertJson ( m_strImportFilename, strWondeDatabase ) == FALSE )
	{
		return nREPORT_INVALIDSOURCE;
	}

	if ( m_wondeDbase.Open ( strWondeDatabase, DB_READONLY ) != DB_ERR_NONE )
	{
		m_strError = m_wondeDbase.GetError();
		return nREPORT_INVALIDSOURCE;
	}

// scan import file

	CWorkingDlg WorkingDlg ( GetProgressTitle(), FALSE );
	WorkingDlg.Create();

	if ( m_wondeDbase.MoveFirst() == TRUE )
	{
		do
		{
			WorkingDlg.SetCaption2PercentDone ( m_wondeDbase.GetPercentPosition() );

			CString strCardNo = "";
			m_strExceptionComment = "";								// clear any current exception

			CString strLabel = "";
			switch ( m_pData->m_nLocateOnIndex )					// locate account by Apno / UPN / Person_id
			{
			case nWONDE_LOCATEON_MIS_INDEX:		strLabel = m_wondeDbase.GetMISID();		break;
			case nWONDE_LOCATEON_ADNO_INDEX:	strLabel = m_wondeDbase.GetAdNo();		break;
			case nWONDE_LOCATEON_UPN_INDEX:		strLabel = m_wondeDbase.GetUPN();		break;
			}

			if ( strLabel != "" )
			{
				CSQLRepositoryAccount RepoAccount;
				__int64 nUserID = RepoAccount.LookupIndex(m_pData->GetLocateOnLabel(), strLabel, NULL).GetSQLResult();
			
				if (0 == nUserID)			// see if in temp Members file 
				{
					GetNewCardNo(strCardNo);							// get cardno for new record
				}
				else
				{
					strCardNo = FormatInt64Value(nUserID);				// cardno could be blank on invalid import line ( exception message set )
				}
			}

			if ( m_ImportFlag.GetImportFlag() == FALSE )	
			{
				ShowImportLine ( strCardNo );
			}
			else
			{
				ImportLine ( strCardNo );
			}
		}
		while ( m_wondeDbase.MoveNext() == TRUE );
	}

	m_nRead = m_wondeDbase.GetRecordCount();
	m_wondeDbase.Close();											// close Wonde file

	return nREPORT_NOERROR;
}

/*********************************************************************/
// get CardNo for new record,
// on entry exception string is blank, on exit exception string set if no cardno set

void CImportWondeFile::GetNewCardNo( CString& strCardNo )
{
	strCardNo = "";													// clear number

	if ( m_wondeDbase.GetTypeNo() == nWONDETYPE_STAFF )
	{
		if ( m_pData->m_bDisableStaffImport == FALSE )				// check import of staff is reqd
		{
			if (m_pData->m_nNewStaffIntakeIndex == nWONDE_NEWINTAKE_NEXTFREE_INDEX)
			{
				CSQLRepositoryAccount RepoAccount;
				__int64 nUserID = RepoAccount.GetNextFreeUserID(_atoi64(m_strStaffStartFrom), _atoi64(System.GetHighCardNo()),NULL).GetSQLResult();
					
				if ( 0 == nUserID )
				{
					m_strExceptionComment = "No more staff accounts";
				}
				else
				{
					strCardNo.Format("%I64d", nUserID);
					m_strStaffStartFrom = CSQLHelpers::IncrementUserID(strCardNo, System.GetHighCardNo());		// next new start from position
				}
			}
			else
			{
				CString strCardID = "";

				if (m_pData->m_nNewStaffIntakeIndex == nWONDE_NEWINTAKE_ADNO_INDEX)
				{
					strCardID = m_wondeDbase.GetAdNo();
				}
				else
				{
					strCardID = m_wondeDbase.GetMISID();
				}

				if (System.IsValidCardNo(strCardID) == FALSE)
				{
					m_strExceptionComment.Format("Invalid %s reference %s", 
						m_pData->GetNewIntakeText(m_pData->m_nNewStaffIntakeIndex), 
						(const char*) strCardID);
				}
				else
				{
					CSQLRepositoryAccount RepoAccount;
					if (RepoAccount.RowExists(_atoi64(strCardID), NULL).GetSQLResult() != 0 )
					{
						m_strExceptionComment.Format("Account %s already exists", 
							(const char*) System.FormatCardNo(strCardID));
					}
				}

				if (m_strExceptionComment == "")							// check for any errors
				{
					strCardNo = strCardID;
				}
			}
		}
	}
	else				//  Pupil line
	{
		if (m_pData->m_nNewPupilIntakeIndex == nWONDE_NEWINTAKE_NEXTFREE_INDEX)
		{
			CSQLRepositoryAccount RepoAccount;
			__int64 nUserID = RepoAccount.GetNextFreeUserID(_atoi64(m_strPupilStartFrom), _atoi64(System.GetHighCardNo()),NULL).GetSQLResult();
				
			if (0 == nUserID)
			{
				m_strExceptionComment = "No more student accounts";
			}
			else
			{
				strCardNo.Format("%I64d", nUserID);
				m_strPupilStartFrom = CSQLHelpers::IncrementUserID(strCardNo, System.GetHighCardNo());		// next new start from position
			}
		}
		else
		{
			CString strCardID = "";

			if (m_pData->m_nNewPupilIntakeIndex == nWONDE_NEWINTAKE_ADNO_INDEX)
			{
				strCardID = m_wondeDbase.GetAdNo();
			}
			else
			{
				strCardID = m_wondeDbase.GetMISID();
			}

			if (System.IsValidCardNo(strCardID) == FALSE)
			{
				m_strExceptionComment.Format("Invalid %s reference %s", 
					(const char*) m_pData->GetNewIntakeText(m_pData->m_nNewStaffIntakeIndex), strCardID);
			}
			else
			{
				CSQLRepositoryAccount RepoAccount;
				if (RepoAccount.RowExists(_atoi64(strCardID), NULL).GetSQLResult() != 0 )
				{
					m_strExceptionComment.Format("Account %s already exists", 
					(const char*) System.FormatCardNo(strCardID));
				}
			}

			if (m_strExceptionComment == "")							// check for any errors
			{
				strCardNo = strCardID;
			}
		}
	}
}

/*********************************************************************/

void CImportWondeFile::ImportLine ( const char* szUserID ) 
{
	CString strUserID = szUserID;

	if (strUserID == "" || ::IsStringNumeric(strUserID) == FALSE)
	{
		++m_nIgnored;
	}
	else
	{
		if ((m_wondeDbase.GetTypeNo() == nWONDETYPE_STAFF) && (m_pData->m_bDisableStaffImport == TRUE))
		{
			++m_nSkipped;
		}
		else
		{
			CSQLRepositoryAccount RepoAccount;

			bool bOk = TRUE;
			bool bExists = (RepoAccount.RowExists(_atoi64(strUserID), NULL).GetSQLResult() != 0);
			
			if ((FALSE == bExists) && (nIMPORT_METHOD_EXISTONLY == m_nImportMethod))		// see if must exist
			{
				bOk = FALSE;
			}
			else if (( TRUE == bExists ) && ( nIMPORT_METHOD_NEWONLY == m_nImportMethod))	// only new recod allowed
			{
				bOk = FALSE;
			}

			if ( bOk == TRUE )
			{
				++m_nUsed;													// inc number of valid lines 
	
				if ( bExists == FALSE )										// see if in database
				{
					++m_nAdded;
					CSQLRowAccountFull RowAccount;
					RowAccount.SetUserIDString(strUserID);
					RepoAccount.InsertRow(RowAccount,NULL);
					
					SetRecord ( RowAccount, TRUE );										// update new record
					RowAccount.CopyGroupMaxSpendSettings();
					RepoAccount.UpdateRow(RowAccount,NULL);
					
					CSQLAuditRecord atc;
					atc.Set( &RowAccount );
					atc.SetApplicationNo ( APPNO_INSERTRECORDWONDE );
					atc.SaveNewLine();
				}
				else
				{
					CSQLRowAccountFull RowAccount;
					RowAccount.SetUserIDString(strUserID);
					RepoAccount.SelectRow(RowAccount, NULL);
			
					int nOldGroupNo = RowAccount.GetGroupNo();

					SetRecord(RowAccount, FALSE);									// update existing record

					if (nOldGroupNo != RowAccount.GetGroupNo())
					{
						RowAccount.CopyGroupMaxSpendSettings();
					}

					RepoAccount.UpdateRow(RowAccount, NULL);
				}
			}
			else
			{
				++m_nSkipped;
			}
		}
	}
}
	
/*********************************************************************/

void CImportWondeFile::SetRecord ( CSQLRowAccountFull& RowAccount, bool bNewRecord )
{
	int nField;
	CString strField;

	int nTypeNo		 = m_wondeDbase.GetTypeNo();
	CString strMISID = m_wondeDbase.GetMISID();

	if ( bNewRecord == TRUE )
	{
		int nGroupNo = ( nTypeNo == nWONDETYPE_PUPIL ) ? m_pData->m_nFSMRevertGroupNo : m_pData->m_nStaffGroupNo;
		RowAccount.SetGroupNo ( nGroupNo );
	}

	if ( ( strField = m_wondeDbase.GetUserName() ) != "" )		RowAccount.SetUsername ( strField );
	if ( ( strField = m_wondeDbase.GetForename() ) != "" )		RowAccount.SetForename ( strField );
	if ( ( strField = m_wondeDbase.GetSurname() ) != "" )		RowAccount.SetSurname ( strField );
	if ( ( nField = m_pData->m_nMISIDUserTextNo ) > 0 )			RowAccount.SetInfo ( nField, strMISID );		// info1 - info8

	if ( nTypeNo == nWONDETYPE_PUPIL )
	{
		if ( m_pData->m_nWebPaymentIndex1 == nWONDE_WEBPAYMENT_MIS_INDEX )	// see if MIS_ID used as pupil WebPayment reference
		{
			if ( strMISID != "" )
			{
				RowAccount.SetMemberID ( strMISID );							// set WebPayment reference	
				m_bMembershipUpdated = TRUE;
			}
		}

		if ( ( strField = m_wondeDbase.GetUPN() ) != "" )				// see have have UPN field
		{
			if ((nField = m_pData->m_nUPNUserTextNo) > 0)				// see if saving it (0=not defined, Info1-8 )
			{
				RowAccount.SetInfo(nField, strField);					// info1 - info8
			}

			if ( m_pData->m_nWebPaymentIndex1 == nWONDE_WEBPAYMENT_UPN_INDEX )	// see if UPN used as pupil WebPayment reference
			{
				RowAccount.SetMemberID ( strField );								// set WebPayment reference	
				m_bMembershipUpdated = TRUE;
			}
		}
	
		if ( ( strField = m_wondeDbase.GetAdNo() ) != "" )				// see if have AdNo field
		{
			if ((nField = m_pData->m_nAdNoUserTextNo) > 0)				// see if saving it (0=not defined, Info1-8 )  
			{
				RowAccount.SetInfo(nField, strField);					// info1 - info8
			}

			if ( m_pData->m_nWebPaymentIndex1 == nWONDE_WEBPAYMENT_ADNO_INDEX )	// see if ADNO used as pupil WebPayment reference
			{
				RowAccount.SetMemberID ( strField );								// set WebPayment reference	
				m_bMembershipUpdated = TRUE;
			}
		}

		if ((strField = m_wondeDbase.GetYear()) != "")
		{
			if ((nField = m_pData->m_nYearUserTextNo) > 0)				// 0=not defined or Info1-8
			{
				RowAccount.SetInfo(nField, strField);
			}
		}

		if ((strField = m_wondeDbase.GetReg()) != "")
		{
			if ((nField = m_pData->m_nRegUserTextNo) > 0)				// 0=not defined or Info1-8
			{
				RowAccount.SetInfo(nField, strField);
			}
		}

		if ((strField = m_wondeDbase.GetHouse()) != "")
		{
			if ((nField = m_pData->m_nHouseUserTextNo) > 0)				// 0=not defined or Info1-8
			{
				RowAccount.SetInfo(nField, strField);
			}
		}

		if ( m_pData->m_bSavePupilEmail == TRUE )							// see if saving pupil home email
		{
			nField = m_pData->m_nEmailUserTextNo;							// 0 = email or Info1-8
			if ( ( strField = m_wondeDbase.GetEmail() ) != "" )			// get Priority 1 email
			{
				if (nField == 0)
				{
					RowAccount.SetEmail(strField);
				}
				else
				{
					RowAccount.SetInfo(nField, strField);
				}
			}
			else
			{
				if ( ( strField = m_wondeDbase.GetEmail2() ) != "" )	// get Priority 2 email
				{
					if (nField == 0)
					{
						RowAccount.SetEmail(strField);
					}
					else
					{
						RowAccount.SetInfo(nField, strField);
					}
				}
			}
		}

		if (m_wondeDbase.IsFSM() == TRUE)								// see is FSM set
		{
			if (m_pData->m_nFSMGroupNo != 0)
			{
				RowAccount.SetGroupNo(m_pData->m_nFSMGroupNo);		// yes - set to default FSM group
			}
		}
		else																// FSM not set
		{
			if (RowAccount.GetGroupNo() == m_pData->m_nFSMGroupNo)		// see if is was previoulsy FSM group
			{
				if (m_pData->m_nFSMRevertGroupNo != 0)
				{
					RowAccount.SetGroupNo(m_pData->m_nFSMRevertGroupNo);	// see to default not FSM group
				}
			}
		}

		if ( ( strField = m_wondeDbase.GetDOB() ) != "" )			RowAccount.SetDOB ( strField );
		if ( ( strField = m_wondeDbase.GetGender() ) != "" )		RowAccount.SetGender ( strField );
		if ( ( strField = m_wondeDbase.GetOutDate() ) != "" )		RowAccount.SetExpiryDate ( strField );
		if ( ( strField = m_wondeDbase.GetDietaryNeeds() ) != "" )	RowAccount.SetDietary ( strField );
	}
	else			// Staff ********************************************
	{
		if ( m_pData->m_nWebPaymentIndex2 == nWONDE_WEBPAYMENT_MIS_INDEX )	// see if MIS_ID used as WebPayment reference
		{
			if ( strMISID != "" )
			{
				RowAccount.SetMemberID ( strMISID );						// set WebPayment reference	
				m_bMembershipUpdated = TRUE;
			}
		}

		if ( m_pData->m_bSaveStaffEmail == TRUE )							// see if saving staff work email
		{
			nField = m_pData->m_nEmail2UserTextNo;							// 0 = email or Info1-8
			if ((strField = m_wondeDbase.GetEmail()) != "")			// get first email
			{
				if (nField == 0)
				{
					RowAccount.SetEmail(strField);
				}
				else
				{
					RowAccount.SetInfo(nField, strField);
				}
			}
		}
	}
}

/*********************************************************************/
/* STUDENT iNFO

	"MIS_ID": "8404",
	"AdmissionNumber": "003979",
	"PreferredFullName": "Gren Abbey",
	"LegalForename": "Grenetta",
	"LegalSurname": "Abbey",		  
	"DOB": "yyyy-MM-dd",
	"UPN": "B820432109001",		  
	"YearGroup": "10",
	"RegGroup": "10C",
	"House": "North",		  
	"Gender": "Female",
	"FSM": "T",
	"DietaryNeeds": { "Need": [ "Halal", "No pork" ] },
	"AdmissionDate": "yyyy-MM-dd"  		  
	"LeavingDate": "yyyy-MM-dd"
  
    "Staff": 
	"MIS_ID": "6913",
	"PreferredFullName": "Mia Asif",
	"LegalForename": "Mina",
	"LegalSurname": "Asif",
	"WorkEmail": "Asif@Hotmail.com" 

	
	Sharp.Students[0].MIS_ID=4472
	Sharp.Students[0].AdmissionNumber=000904
	Sharp.Students[0].PreferredFullName=Kerim Abbott
	Sharp.Students[0].LegalForename=Kerim
	Sharp.Students[0].LegalSurname=Abbott
	Sharp.Students[0].DOB=2003-10-13
	Sharp.Students[0].UPN=F309202206076
	Sharp.Students[0].YearGroup=8
	Sharp.Students[0].RegGroup=8Fo
	Sharp.Students[0].House=Fonteyn
	Sharp.Students[0].Gender=M
	Sharp.Students[0].FSM=F
	Sharp.Students[0].AdmissionDate=2015-09-01

	Sharp.ContactRelationships[0].RelationshipID=4472-4473
	Sharp.ContactRelationships[0].ContactID=4473
	Sharp.ContactRelationships[0].StudentID=4472
	Sharp.ContactRelationships[0].Relationship=Mother
	Sharp.ContactRelationships[0].Priority=1

	Sharp.Contacts[0].MIS_ID=4473
	Sharp.Contacts[0].LegalForename=Francine
	Sharp.Contacts[0].LegalSurname=Abbott
	Sharp.Contacts[0].HomeEmail=francineabbott@hotmail.co.uk
*/

bool CImportWondeFile::ConvertJson ( const char* szJsonFilename, const char* strWondeDatabase )
{
	if ( m_wondeDbase.Open ( strWondeDatabase, DB_READWRITE ) != DB_ERR_NONE )
	{
		m_strError.Format ( "Unable to open ' %s ' for read/write access", strWondeDatabase );
		return FALSE;
	}

	CSSJson jsonFile;
	jsonFile.ParseFile ( szJsonFilename, m_pData->m_bTestMode );		// test mode save working file for debugging

	CString strLabel, strValue;

// look for student records

	int nIndex = 0;
	while ( TRUE )
	{
		strLabel.Format ( "Sharp.Students[%d].MIS_ID", nIndex );
		strValue = jsonFile.GetValue ( strLabel );
		if ( strValue == "" )
			break;

		if (m_wondeDbase.FindRecord(strValue) == FALSE)
		{
			m_wondeDbase.AddBlankRecord(strValue);
		}

		m_wondeDbase.SetTypePupil();

		strLabel.Format ( "Sharp.Students[%d].AdmissionNumber", nIndex );
		strValue = jsonFile.GetValue ( strLabel );
		if ( strValue != "" )	m_wondeDbase.SetAdNo ( strValue );
		
		strLabel.Format ( "Sharp.Students[%d].PreferredFullName", nIndex );
		strValue = jsonFile.GetValue ( strLabel );
		if ( strValue != "" )	m_wondeDbase.SetUserName ( strValue );
		
		strLabel.Format ( "Sharp.Students[%d].LegalForename", nIndex );
		strValue = jsonFile.GetValue ( strLabel );
		if ( strValue != "" )	m_wondeDbase.SetForename ( strValue );
		
		strLabel.Format ( "Sharp.Students[%d].LegalSurname", nIndex );
		strValue = jsonFile.GetValue ( strLabel );
		if ( strValue != "" )	m_wondeDbase.SetSurname ( strValue );
		
		strLabel.Format ( "Sharp.Students[%d].DOB", nIndex );
		strValue = jsonFile.GetValue ( strLabel );
		if ( strValue != "" )	m_wondeDbase.SetDOB ( strValue );			// yyyy-MM-dd
		
		strLabel.Format ( "Sharp.Students[%d].UPN", nIndex );
		strValue = jsonFile.GetValue ( strLabel );
		if ( strValue != "" )	m_wondeDbase.SetUPN ( strValue );
		
		strLabel.Format ( "Sharp.Students[%d].YearGroup", nIndex );
		strValue = jsonFile.GetValue ( strLabel );
		if ( strValue != "" )	m_wondeDbase.SetYear ( strValue );
		
		strLabel.Format ( "Sharp.Students[%d].RegGroup", nIndex );
		strValue = jsonFile.GetValue ( strLabel );
		if ( strValue != "" )	m_wondeDbase.SetReg ( strValue );
	
		strLabel.Format ( "Sharp.Students[%d].House", nIndex );
		strValue = jsonFile.GetValue ( strLabel );
		if ( strValue != "" )	m_wondeDbase.SetHouse ( strValue );
		
		strLabel.Format ( "Sharp.Students[%d].Gender", nIndex );
		strValue = jsonFile.GetValue ( strLabel );
		if ( strValue != "" )	m_wondeDbase.SetGender ( strValue );
		
		strLabel.Format ( "Sharp.Students[%d].FSM", nIndex );
		strValue = jsonFile.GetValue ( strLabel );
		if ( strValue != "" )	m_wondeDbase.SetFSM ( strValue );

/*********************************************************************/
//		int nNeed = 0;
//		while ( TRUE )
//		{
//			strLabel.Format ( "Sharp.Students[%d].DietaryNeeds.Need[%d]", nIndex, nNeed );
//			strValue = jsonFile.GetValue ( strLabel );
//			if ( strValue == "" )
//				break;
//			m_wondeDbase.SetDietaryNeed ( strValue );
//			++nNeed;
//		}
/*********************************************************************/
		strLabel.Format ( "Sharp.Students[%d].DietaryNeeds.DietaryNeed", nIndex );
		strValue = jsonFile.GetValue ( strLabel );
		if ( strValue != "" )
			m_wondeDbase.SetDietaryNeed ( strValue );
/*********************************************************************/

		strLabel.Format ( "Sharp.Students[%d].HomeEmail", nIndex );
		strValue = jsonFile.GetValue ( strLabel );
		if ( strValue != "" )	m_wondeDbase.SetEmail ( strValue );

		strLabel.Format ( "Sharp.Students[%d].AdmissionDate", nIndex );
		strValue = jsonFile.GetValue ( strLabel );
		if ( strValue != "" )	m_wondeDbase.SetInDate ( strValue );	// yyyy-MM-dd

		strLabel.Format ( "Sharp.Students[%d].LeavingDate", nIndex );
		strValue = jsonFile.GetValue ( strLabel );
		if ( strValue != "" )	m_wondeDbase.SetOutDate ( strValue );	// yyyy-MM-dd

		++nIndex;
	}


// Look for staff records

	nIndex = 0;
	while ( TRUE )
	{
		strLabel.Format ( "Sharp.Staff[%d].MIS_ID", nIndex );
		strValue = jsonFile.GetValue ( strLabel );
		if ( strValue == "" )
			break;

		if (m_wondeDbase.FindRecord(strValue) == FALSE)
		{
			m_wondeDbase.AddBlankRecord(strValue);
		}

		m_wondeDbase.SetTypeStaff();

		strLabel.Format ( "Sharp.Staff[%d].PreferredFullName", nIndex );
		strValue = jsonFile.GetValue ( strLabel );
		if ( strValue != "" )	m_wondeDbase.SetUserName ( strValue );
		
		strLabel.Format ( "Sharp.Staff[%d].LegalForename", nIndex );
		strValue = jsonFile.GetValue ( strLabel );
		if ( strValue != "" )	m_wondeDbase.SetForename ( strValue );
		
		strLabel.Format ( "Sharp.Staff[%d].LegalSurname", nIndex );
		strValue = jsonFile.GetValue ( strLabel );
		if ( strValue != "" )	m_wondeDbase.SetSurname ( strValue );

		if ( m_pData->m_bSaveStaffEmail == TRUE )
		{
			strLabel.Format ( "Sharp.Staff[%d].WorkEmail", nIndex );
			strValue = jsonFile.GetValue ( strLabel );
			if ( strValue != "" )	m_wondeDbase.SetEmail ( strValue );
		}

		++nIndex;
	}

// look for student email address *****************************************
//	Sharp.ContactRelationships[564].RelationshipID=4401-4437
//	Sharp.ContactRelationships[564].ContactID=4437
//	Sharp.ContactRelationships[564].StudentID=4401
//	Sharp.ContactRelationships[564].Relationship=Father
//	Sharp.ContactRelationships[564].Priority=1
//
//	Sharp.Contacts[443].MIS_ID=4437
//	Sharp.Contacts[443].LegalForename=Besim
//	Sharp.Contacts[443].LegalSurname=Gerguri
//	Sharp.Contacts[443].HomeEmail=besgerguri@googlemail.com
//
//	Sharp.Students[306].MIS_ID=4401
//	Sharp.Students[306].AdmissionNumber=000935
//	Sharp.Students[306].PreferredFullName=Ulk Gerguri
//	Sharp.Students[306].LegalForename=Ulk
//	Sharp.Students[306].LegalSurname=Gerguri
//	Sharp.Students[306].DOB=2004-03-25
//	Sharp.Students[306].UPN=U302201006072
//	Sharp.Students[306].YearGroup=8
//	Sharp.Students[306].RegGroup=8Di
//	Sharp.Students[306].House=Dickens
//	Sharp.Students[306].Gender=M
//	Sharp.Students[306].FSM=F
//	Sharp.Students[306].AdmissionDate=2015-09-01
/*********************************************************************/

	if ( m_pData->m_bSavePupilEmail == TRUE )
	{
		nIndex = 0;
		while ( TRUE )
		{
			strLabel.Format ( "Sharp.ContactRelationships[%d].StudentID", nIndex );		// "Sharp.ContactRelationships[i].StudentID=nnnn"
			strValue = jsonFile.GetValue ( strLabel );
			if ( strValue == "" )														// not found exit	
				break;

			CString strContactID;														// parent contact MIS_ID
			if ( m_wondeDbase.FindRecord ( strValue ) == TRUE )						// see if have student record
			{
				strLabel.Format ( "Sharp.ContactRelationships[%d].ContactID", nIndex );	// get parent contact mis-id
				strContactID = jsonFile.GetValue ( strLabel );

				if ( strContactID == "" )												// not found
				{
					++nIndex;															// go to next student
					continue;
				}

				strLabel.Format ( "Sharp.ContactRelationships[%d].Priority", nIndex );	// Get parent priority
				strValue = jsonFile.GetValue ( strLabel );
				int nPriority = atoi ( strValue );										// save it for when email found

				int nIndex2 = 0;
				while ( TRUE )
				{
					strLabel.Format ( "Sharp.Contacts[%d].MIS_ID", nIndex2 );			// "Sharp.Contacts[i].MIS_ID"
					strValue = jsonFile.GetValue ( strLabel );
					if ( strValue == "" )
						break;															// end of contacts

					if ( strValue == strContactID )										// see if parent MIS_ID match found
					{
						strLabel.Format ( "Sharp.Contacts[%d].HomeEmail", nIndex2 );	// "Sharp.Contacts[i].HomeEmail"
						strValue = jsonFile.GetValue ( strLabel );
						if ( strValue != "" )											// get parent home email
						{
							if ( nPriority == 1 )
							{
								CString strEmail = m_wondeDbase.GetEmail();			// get first email ( Priority 1 )
								if ( strEmail == "" )
									m_wondeDbase.SetEmail ( strValue );
							}
							else
							{
								CString strEmail2 = m_wondeDbase.GetEmail2();		// get second email ( Priority 2, 3, 4 )
								if ( strEmail2 == "" )
									m_wondeDbase.SetEmail2( strValue );
							}
						}
						break;
					}
					++nIndex2;
				}
			}
			++nIndex;
		}
	}

//*** end of email extraction **********************************

	m_wondeDbase.Close();

	return TRUE;
}

/*********************************************************************/

