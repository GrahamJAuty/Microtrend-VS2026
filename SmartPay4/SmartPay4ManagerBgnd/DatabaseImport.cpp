/**********************************************************************/
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\ImageHandler.h"
#include "..\SmartPay4Shared\SmartPayAuditFile.h"
#include "..\SmartPay4Shared\SmartPayHeaderHelpers.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\TLogFile.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_PeriodRefresh\SQLRepositoryPeriodRefresh.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
/**********************************************************************/
#include "SQLTranImportDatabase.h"
/**********************************************************************/
#include "DatabaseImport.h"
/**********************************************************************/
#define	nMEMBER_PREFIXLEN			4
/**********************************************************************/
static const char* szCLEAR_FIELD = "CLEAR";
/**********************************************************************/

CImportDatabase::CImportDatabase ( CWnd* pParent )
{
	m_pParent = pParent;
	m_strTitle = "";
	m_strError = "";
	Init();
}

//**************************************************************************

void CImportDatabase::Init()
{
	m_nIndexCardNo = -1;
	m_nIndexLocateOn = -1;
	m_nImportMethod = 0;
	m_nImportAppNo = APPNO_IMPORTUPDATE;
	
	m_strExceptionPrefix = "";

	m_data.Read();
	m_strNextNewUserID = m_data.m_strStandardNextNewUserID;
	m_nLocateOn = m_data.m_nLocateOn;		
	m_strLocateOnLabel = "";
}

/**********************************************************************/

CImportDatabase::~CImportDatabase()
{
	::SendToRecycleBin ( &m_arrayDeleteList );						// delete any files selected for delete
}

//***************************************************************************
// auto csv record import ***************************************************
// file format standard csvv \ Custom ***************************************
//***************************************************************************

bool CImportDatabase::AutoImportDatabase ( int nDataFormat, const char* szCsvFilename, int nImportMethod, CSQLImportCounters& Counters )
{
	bool bDeleteFile = TRUE;												// delete import file when done
	m_nImportMethod	= nImportMethod;

	m_strTitle = "Import Automatic Import File";
	LogMessage( "Auto Import Started" );

	if ( nDataFormat == nIMPORT_FORMAT_CUSTOM )
	{
		if ( m_data.m_strCustomMenuText == "" )
		{
			DisplayMessage ( "CSV Custom Import option has been inhibited!" );
			LogMessage( "Custom import requested but not enabled" );
			return FALSE;
		}

		{
			// convert custom header line to database field names in tmp file 
			CString strResult = m_data.ConvertCustomHeader ( szCsvFilename ); 
			
			if ( strResult != "" )
			{
				LogMessage( strResult );
				return FALSE;
			}
		}

		m_nImportAppNo = APPNO_IMPORTCUSTOM;
		m_strNextNewUserID = m_data.m_strCustomNextNewUserID;		// set next start from custom start from user id
		m_nLocateOn	= m_data.m_nCustomLocateOn;
	}

	bool bReply = FALSE;
	if ((bReply = ValidateFile(szCsvFilename, "")) == TRUE)				// check import file ok
	{
		CSSFile fileImport;
		if (fileImport.Open(szCsvFilename, "rb") == TRUE)
		{
			CSQLTranImportDatabase Tran;
			Tran.BeginTrans();
			Tran.DoWork(m_nImportMethod, m_nImportAppNo, fileImport, m_arrayImportHeader, m_nIndexCardNo);

			switch (Tran.EndTrans())
			{
			case SQLTRAN_STATE_COMMITOK:
				Tran.AfterTransaction();

				if (bDeleteFile == TRUE)
				{
					m_arrayDeleteList.Add(szCsvFilename);
				}

				Tran.GetImportCounters(Counters);

				if (Counters.GetAdded() > 0)
				{
					switch (nDataFormat)
					{
					case nIMPORT_FORMAT_CSV:
						SaveStandardNextNewUserID();
						break;

					case nIMPORT_FORMAT_SIMS:
						break;	// no used by this routine

					case nIMPORT_FORMAT_CUSTOM:
						SaveCustomNextNewUserID();
						break;

					case nIMPORT_FORMAT_WONDE:
						break;	// no used by this routine
					}
				}

				break;
			}
		}
	}

	DisplayMessage ( m_strError );
	return bReply;
}

//**********************************************************************
//**********************************************************************
// check csv import file is ok to use

bool CImportDatabase::ValidateFile ( CString strImportFilename, CString strMandatoryHeader )
{
	int nLineCount;
	if ( ( nLineCount = ::GetFileLineCount ( strImportFilename ) ) < 2 )
	{
		m_strError = "Insufficent data lines found in import file";
		LogMessage( m_strError );
		return FALSE;
	}
	--nLineCount;													// number of data lines - ignore header line in count

// read header line from importfile **********************************

	CSSFile file1;
	if ( file1.Open ( strImportFilename, "rb" ) == FALSE )
	{
		m_strError.Format ( "Unable to open import file %s", 
			(const char*) strImportFilename );

		LogMessage( m_strError );
		return FALSE;
	}

// read header line from importfile **********************************

	bool bReply = FALSE;
	CString strHeader = "";

	{
		CCSV csvImportHeader;
		bReply = file1.Read(csvImportHeader);					// read header line 
		strHeader = csvImportHeader.GetLine();
		CreateNumericHeader(csvImportHeader);
	}

	file1.Close();													// close import file

	if ( strMandatoryHeader != "" )
	{
		if (CompareNoCase(strHeader, strMandatoryHeader) == FALSE)
		{
			m_strError = "Header line does not match mandatory format";
			LogMessage( m_strError );
			return FALSE;
		}
	}

	if ( bReply == FALSE )
	{
		m_strError = "Unable to read the header line in the csv import file";
		LogMessage( m_strError );
		return FALSE;
	}

// scan header line *************************************************

	m_strLocateOnLabel = m_data.GetLocateOnLabel(m_nLocateOn); 
	m_nIndexCardNo = -1;
	m_nIndexLocateOn = -1;

	int nLocateOnField = SmartPayHeaderHelpers.GetNumericField(m_strLocateOnLabel);
	
	for ( int n = 0 ; n < m_arrayImportHeader.GetSize() ; n++ )
	{
		int nField = m_arrayImportHeader.GetAt(n);

		if (SMARTPAYFIELD_USERID == nField)
		{
			m_nIndexCardNo = n;
		}

		if ( ( nLocateOnField > 0 ) && ( nField == nLocateOnField ) )
		{
			m_nIndexLocateOn = n;

		}
	}

	if ((m_nIndexCardNo != -1) && (m_nIndexLocateOn != -1))
	{
		CString strHeaderError = "Header cannot contain UserID and Locate On label";
		CString strDetail = "Locate On label = ";
		strDetail += m_strLocateOnLabel;

		LogMessage(strHeaderError);
		LogMessage(strDetail);
		m_strError = strHeaderError + "\n\n";
		m_strError += strDetail;
		return FALSE;
	}

	{
		CString strMsg;		
		strMsg.Format ( "Number of lines in the import file = %d.\n\n", nLineCount );

		if ( ( m_nIndexCardNo == -1 ) && ( m_strNextNewUserID != "" ) )
		{
			if ( ( m_nLocateOn != 0 ) || ( m_nImportMethod != nIMPORT_METHOD_EXISTONLY ) )
			{
				CString strMsg1 = "";
				strMsg1.Format ( "Any new records will be added starting from account %s.\n\n", 
					(const char*) System.FormatCardNo(m_strNextNewUserID) ); 
				strMsg += strMsg1;	
			}
		}

		strMsg += "Do you wish to continue?";

		CModuleGlobals::MessageBeepQuestion();
		if ( Prompter.YesNo( strMsg, m_strTitle, FALSE, TRUE ) == IDNO )
		{
			return FALSE;
		}
	}

	if (m_nIndexCardNo != -1)										// see if have a CardNo field
	{
		return TRUE;												// yes - so normal import
	}

// to get here don't have a CardNo field in header *************************************************************

	CString strHeaderError = "Missing Header Element";
	
	if ( 0 == m_nLocateOn )											// standard import
	{
		if (nIMPORT_METHOD_EXISTONLY == m_nImportMethod)
		{
			CString strDetail = "There is no UserID field in the file header line";
			LogMessage(strHeaderError);
			LogMessage(strDetail);
			m_strError = strHeaderError + "\n\n";
			m_strError += strDetail;
			return FALSE;
		}
	}
	else															// LocateOn required
	{
		if (m_nIndexLocateOn == -1)
		{
			CString strDetail = "";
			strDetail.Format("There is no %s field in the file header line!",
				(const char*)m_strLocateOnLabel);

			LogMessage(strHeaderError);
			LogMessage(strDetail);
			m_strError = strHeaderError + "\n\n";
			m_strError += strDetail;
			return FALSE;
		}
	}

// ** insert card number in csv field *************************************************************************

	m_strError = "";												// clear error message;

	m_arrayImportHeader.InsertAt(0, SMARTPAYFIELD_USERID );			// modify header to insert cardno field in first position
	m_nIndexCardNo = 0;												// set index to cardno field

	CReportConsolidationArray<CSortedInt64ByString> NewMemberMap;

	CFileStringArray file;
	if ( file.Read ( strImportFilename ) == TRUE )					// read import file
	{
		{
			CString strHeader = "";
			strHeader += Account::UserID.Label;
			strHeader += ",";
			strHeader += file.GetAt(0);
			file.SetAt(0, strHeader);
		}
		
		for ( int nLine = 1 ; nLine < file.GetCount() ; nLine++ )
		{
			CString strLine = file.GetAt ( nLine );					// get csv line
			if (strLine == "")
			{
				continue;											// ignore blank line
			}

			CCSV csvLine ( strLine );								// extract csv data fields
			CString strLocateOnField = "";
			bool bLocateOnFound = FALSE;

			CString strUserID = "";									// clear UserID
			if ( m_nLocateOn != 0 )									// see if LocateOn required
			{
				strLocateOnField = csvLine.GetString ( m_nIndexLocateOn );	// LocateOn member record to find

				if (strLocateOnField != "")
				{
					CSQLRepositoryAccount RepoAccount;
					__int64 nUserID = RepoAccount.LookupIndex(m_strLocateOnLabel, strLocateOnField, NULL).GetSQLResult();

					if (0 == nUserID)
					{
						CSortedInt64ByString item;
						item.m_strKey = strLocateOnField;

						int nPos = 0;
						if (NewMemberMap.Find(item, nPos) == TRUE)
						{
							NewMemberMap.GetAt(nPos, item);
							nUserID = item.m_nData;
						}
					}

					if (nUserID != 0)
					{
						strUserID = System.FormatCardNo(nUserID, TRUE);	// Prefix Cardno to show CardNo is in (or will be) in LocateOn members database
						bLocateOnFound = TRUE;
					}
				}
			}
			
			if ( ( strUserID == "" ) && ( strLocateOnField != "" ) )
			{														// not in members database - get next free cardno ( may be blank if max cardno reached )
				if ( m_strNextNewUserID != "" )					// check can add records ( ie if topup no add )
				{
					CSQLRepositoryAccount RepoAccount;
					__int64 nUserID = RepoAccount.GetNextFreeUserID(_atoi64(m_strNextNewUserID), _atoi64(System.GetHighCardNo()), NULL).GetSQLResult();
						
					if ( nUserID != 0 )
					{
						strUserID = FormatInt64Value(nUserID);
						m_strNextNewUserID = CSQLHelpers::IncrementUserID ( strUserID, System.GetHighCardNo() );		// next new start from position

						if ( FALSE == bLocateOnFound )	// see if new member will be added
						{
							CSortedInt64ByString item;
							item.m_strKey = strLocateOnField;
							item.m_nData = nUserID;
							NewMemberMap.Consolidate(item);
						}
					}
				}
			}

			csvLine.InsertAt ( 0, strUserID );						// add card number ( may be blank if max cardno reached )
			file.SetAt ( nLine, csvLine.GetLine() );
		}

		file.Write ( strImportFilename );

		++m_nIndexLocateOn;											// UserID field has been added to start of header\line so to move LocateOn index down 1 place
	}

	return TRUE;
}

//**********************************************************************

void CImportDatabase::CreateNumericHeader(CCSV& csvImportHeader)
{
	SmartPayHeaderHelpers.CreateNumericHeader(csvImportHeader, m_arrayImportHeader);

	CSQLRepositoryUserText Repo;
	for (int i = 0; i < m_arrayImportHeader.GetSize(); i++)
	{
		int nFieldType = m_arrayImportHeader.GetAt(i);
		
		if (nFieldType < 0)
		{
			CString strLabel = csvImportHeader.GetString(i);

			if (CompareLabel(strLabel, Repo.GetMemberIDText()))
			{
				nFieldType = SMARTPAYFIELD_MEMBERID;
			}
			else if (CompareLabel(strLabel, Repo.GetExtRef1Text()))
			{
				nFieldType = SMARTPAYFIELD_EXTERNALREF1;
			}
			else if (CompareLabel(strLabel, Repo.GetExtRef2Text()))
			{
				nFieldType = SMARTPAYFIELD_EXTERNALREF2;
			}
			else if (CompareLabel(strLabel, Repo.GetInfo1Text()))
			{
				nFieldType = SMARTPAYFIELD_INFO1;
			}
			else if (CompareLabel(strLabel, Repo.GetInfo2Text()))
			{
				nFieldType = SMARTPAYFIELD_INFO2;
			}
			else if (CompareLabel(strLabel, Repo.GetInfo3Text()))
			{
				nFieldType = SMARTPAYFIELD_INFO3;
			}
			else if (CompareLabel(strLabel, Repo.GetInfo4Text()))
			{
				nFieldType = SMARTPAYFIELD_INFO4;
			}
			else if (CompareLabel(strLabel, Repo.GetInfo5Text()))
			{
				nFieldType = SMARTPAYFIELD_INFO5;
			}
			else if (CompareLabel(strLabel, Repo.GetInfo6Text()))
			{
				nFieldType = SMARTPAYFIELD_INFO6;
			}
			else if (CompareLabel(strLabel, Repo.GetInfo7Text()))
			{
				nFieldType = SMARTPAYFIELD_INFO7;
			}
			else if (CompareLabel(strLabel, Repo.GetInfo8Text()))
			{
				nFieldType = SMARTPAYFIELD_INFO8;
			}
			else if (CompareLabel(strLabel, Repo.GetTick1Text()))
			{
				nFieldType = SMARTPAYFIELD_TICK1;
			}
			else if (CompareLabel(strLabel, Repo.GetTick2Text()))
			{
				nFieldType = SMARTPAYFIELD_TICK2;
			}
			else if (CompareLabel(strLabel, Repo.GetTick3Text()))
			{
				nFieldType = SMARTPAYFIELD_TICK3;
			}
			else if (CompareLabel(strLabel, Repo.GetTick4Text()))
			{
				nFieldType = SMARTPAYFIELD_TICK4;
			}
			else if (CompareLabel(strLabel, Repo.GetDOBText()))
			{
				nFieldType = SMARTPAYFIELD_DOB;
			}

			m_arrayImportHeader.SetAt(i, nFieldType);
		}
	}
}

//**********************************************************************

bool CImportDatabase::CompareLabel(CString strLabel1, CString strLabel2)
{
	strLabel1.MakeUpper();
	strLabel2.MakeUpper();
	return ((strLabel1 == strLabel2) && (strLabel1 != ""));
}

//***********************************************************************

void CImportDatabase::CopyPhotoFile ( const char* szFromCardNo, const char* szToCardNo )
{
	CImageHandler image(FALSE);
	image.CopyFile ( szFromCardNo, szToCardNo );				// copy first photo file if found
}

//*********************************************************************

void CImportDatabase::CopyPurchaseFiles ( const char* szFromCardNo, const char* szToCardNo )
{
	CTLogFile tlogFile ( szFromCardNo );							// will merge in any outstanding updates

	CString strFromFilename = Filenames.GetPurchaseHistoryFilename ( szFromCardNo );
	CString strToFilename   = Filenames.GetPurchaseHistoryFilename ( szToCardNo );

	::CopyFile ( strFromFilename, strToFilename, FALSE );
}

//*********************************************************************
// copy refresh database record from old to new

void CImportDatabase::CopyRefreshDatabase ( const char* szOldCardNo, const char* szNewCardNo )
{
	CSQLRowPeriodRefresh RowRefresh;
	RowRefresh.SetUserID(_atoi64(szOldCardNo));

	CSQLRepositoryPeriodRefresh Repo;
	if ( Repo.SelectRow(RowRefresh,NULL).GetSQLError() == SQLCRUD_ERR_NONE )
	{
		RowRefresh.SetUserID(_atoi64(szNewCardNo));
		Repo.UpsertRow(RowRefresh, NULL);
	}
}

//********************************************************************

void CImportDatabase::SaveStandardNextNewUserID()
{
	m_data.m_strStandardNextNewUserID = m_strNextNewUserID.TrimLeft('0');
	m_data.Save();
}

//***********************************************************************

void CImportDatabase::SaveTopupNextNewUserID()
{
	m_data.m_strTopupNextNewUserID = m_strNextNewUserID.TrimLeft('0');
	m_data.Save();
}

//***********************************************************************

void CImportDatabase::SaveCustomNextNewUserID()
{
	m_data.m_strCustomNextNewUserID = m_strNextNewUserID.TrimLeft('0');
	m_data.Save();
}

//***********************************************************************
