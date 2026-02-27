//*******************************************************************
#ifdef SYSTEMTYPE_MANAGER
//*******************************************************************
#include "..\SmartPay4Shared\PasswordData.h"
//*******************************************************************
#endif
//*******************************************************************
#include "SQLTable_CCNo\SQLRepositoryCCNo.h"
//*******************************************************************
#include "SQLAuditRecord_base.h"
//*******************************************************************
// used by SmartPayServer & Audit reporting

CSQLAuditRecord_base::CSQLAuditRecord_base()
{
	Set(AUDIT_SERVER);
}

//*******************************************************************

void CSQLAuditRecord_base::Set(int nSourceType, int nAppNo)
{
	m_dateFiled.SetDate("");			// date audit line was first saved
	m_timeFiled.SetTime("");			// time audit line was first saved
	m_dateLastUsed.SetDate("");			// date last used @ ecr
	m_timeLastUsed.SetTime("");			// time last used @ ecr
	m_strUserID = "";					// User ID
	m_strXFerUserID = "";				// User ID for transfer, if any
	m_strUserName = "";					// User name
	m_nGroupNo = 1;						// default group
	m_nAlternateGroupNo = 0;			// set no alternate group set

	m_nCurrentPoints = 0;
	m_dCurrentPurse1Liability = 0.0;
	m_dCurrentPurse1Credit = 0.0;
	m_dCurrentPurse2Balance = 0.0;
	m_dCurrentPurse3Liability = 0.0;
	m_dCurrentPurse3Credit = 0.0;

	m_nTransactionPoints = 0;
	m_dTransactionPurse1Liability = 0.0;
	m_dTransactionPurse1Credit = 0.0;
	m_dTransactionPurse2Balance = 0.0;
	m_dTransactionPurse3Liability = 0.0;
	m_dTransactionPurse3Credit = 0.0;
	m_dTransactionCash = 0.0;

	m_nToDatePoints = 0;
	m_dToDatePurse1Spend = 0.0;
	m_dToDatePurse2Spend = 0.0;
	m_dToDatePurse3Spend = 0.0;
	m_dToDateCashSpend = 0.0;

	m_nSourceType = nSourceType;			// source of audit info
	m_nApplicationNo = nAppNo;				// application no
	m_nCCNoFull = 0;
	m_nTerminalNo = 0;
	m_strOperatorID = "";
	m_strAuditLineExtraText = "";		// additional text
	m_strComment = "";					// audit comments

	m_nFolderSet = 0;
	m_nREQFileType = REQFILETYPE_INTERNAL;
	m_nPasswordNo = -1;
}

//*******************************************************************
//** used by SmartPay manager to save a card database audit line
// & by SmartPayServer to audit Purse2 Refreshes

void CSQLAuditRecord_base::Set(CSQLRowAccountFull* pRow, int nSourceType)
{
	Set(nSourceType);								// reset variables

	if (nSourceType == AUDIT_PC)
	{
		//		m_dateLastUsed.SetCurrentDate();
		//		m_timeLastUsed.SetCurrentTime();
		CSSDate date;
		CSSTime time;
		pRow -> GetLastUsed(date, time);
		m_dateLastUsed.SetDate(date.GetDate());
		m_timeLastUsed.SetTime(time.GetTime());
	}

	m_strUserID = pRow->GetUserIDString();			// card no
	m_strUserName = pRow->GetUsername();		// name
	m_nGroupNo = pRow->GetGroupNo();		// Group number
	
	m_nCurrentPoints = pRow->GetPoints();;
	m_dCurrentPurse1Liability = pRow->GetPurse1Liability(); // current purse
	m_dCurrentPurse1Credit = pRow->GetPurse1Credit();	// current purse credit
	m_dCurrentPurse2Balance = pRow->GetPurse2Balance();			// current purse
	m_dCurrentPurse3Liability = pRow->GetPurse3Liability();// current purse
	m_dCurrentPurse3Credit = pRow->GetPurse3Credit();	// current purse credit
	
	m_nToDatePoints = pRow->GetPointsToDate();		// points todate
	m_dToDatePurse1Spend = pRow->GetPurse1SpendToDate();	// p1 spend todate
	m_dToDatePurse2Spend = pRow->GetPurse2SpendToDate();	// p2 spend todate
	m_dToDatePurse3Spend = pRow->GetPurse3SpendToDate();	// p2 spend todate
	m_dToDateCashSpend = pRow->GetCashSpendToDate();	// cash spend todate
}

//**********************************************************************

bool CSQLAuditRecord_base::AuditChanges(CSQLRowAccountFull* pRow, bool bForceAudit)
{
	bool bAuditReqd = bForceAudit;

	//CALCULATE TRANSACTION CHANGES
	int nTransactionPoints = pRow->GetPoints() - m_nCurrentPoints;
	double dTransactionPurse1Liability = pRow->GetPurse1Liability() - m_dCurrentPurse1Liability;
	double dTransactionPurse1Credit = pRow->GetPurse1Credit() - m_dCurrentPurse1Credit;
	double dTransactionPurse2Balance = pRow->GetPurse2Balance() - m_dCurrentPurse2Balance;
	double dTransactionPurse3Liability = pRow->GetPurse3Liability() - m_dCurrentPurse3Liability;
	double dTransactionPurse3Credit = pRow->GetPurse3Credit() - m_dCurrentPurse3Credit;

	if (bForceAudit == FALSE)
	{
		bAuditReqd |= (nTransactionPoints != 0);
		bAuditReqd |= (dTransactionPurse1Liability != 0.0);
		bAuditReqd |= (dTransactionPurse1Credit != 0.0);
		bAuditReqd |= (dTransactionPurse2Balance != 0.0);
		bAuditReqd |= (dTransactionPurse3Liability != 0.0);
		bAuditReqd |= (dTransactionPurse3Credit != 0.0);
		bAuditReqd |= (m_nToDatePoints != pRow->GetPointsToDate());
		bAuditReqd |= (m_dToDatePurse1Spend != pRow->GetPurse1SpendToDate());
		bAuditReqd |= (m_dToDatePurse2Spend != pRow->GetPurse2SpendToDate());
		bAuditReqd |= (m_dToDatePurse3Spend != pRow->GetPurse3SpendToDate());
		bAuditReqd |= (m_dToDateCashSpend != pRow->GetCashSpendToDate());
		bAuditReqd |= (m_strUserName != pRow->GetUsername());
		bAuditReqd |= (m_nGroupNo != pRow->GetGroupNo());
	}

	if (bAuditReqd == TRUE)
	{
		m_strUserName = pRow->GetUsername();
		m_nGroupNo = pRow->GetGroupNo();

		CSSDate date;
		CSSTime time;
		pRow->GetLastUsed(date, time);
		m_dateLastUsed.SetDate(date.GetDate());
		m_timeLastUsed.SetTime(time.GetTime());

		m_nTransactionPoints = nTransactionPoints;
		m_dTransactionPurse1Liability = dTransactionPurse1Liability;
		m_dTransactionPurse1Credit = dTransactionPurse1Credit;
		m_dTransactionPurse2Balance = dTransactionPurse2Balance;
		m_dTransactionPurse3Liability = dTransactionPurse3Liability;
		m_dTransactionPurse3Credit = dTransactionPurse3Credit;

		m_nCurrentPoints = pRow->GetPoints();			// current points
		m_dCurrentPurse1Liability = pRow->GetPurse1Liability();// current purse1
		m_dCurrentPurse1Credit = pRow->GetPurse1Credit();	// current purse1 credit
		m_dCurrentPurse2Balance = pRow->GetPurse2Balance();			// current purse2
		m_dCurrentPurse3Liability = pRow->GetPurse3Liability();// current purse3
		m_dCurrentPurse3Credit = pRow->GetPurse3Credit();	// current purse3 credit

		m_nToDatePoints = pRow->GetPointsToDate();		// points todate
		m_dToDatePurse1Spend = pRow->GetPurse1SpendToDate();	// p1 spend todate
		m_dToDatePurse2Spend = pRow->GetPurse2SpendToDate();	// p2 spend todate
		m_dToDatePurse3Spend = pRow->GetPurse3SpendToDate();	// p3 spend todate
		m_dToDateCashSpend = pRow->GetCashSpendToDate();	// cash spend todate

		return TRUE;
	}

	return FALSE;
}

//**********************************************************************
// save new audit line to an audit file

bool CSQLAuditRecord_base::SaveNewLine()
{ 
	return Save ( Filenames.GetAuditFilename(), FALSE );
}

//**********************************************************************

bool CSQLAuditRecord_base::SaveNewLine ( const char* szAuditFile, bool bNoWebUpdateForPebble )
{ 
	return Save ( szAuditFile, FALSE, bNoWebUpdateForPebble ); 
}

//**********************************************************************

bool CSQLAuditRecord_base::SaveNewLine ( CSSFile *pFile )				
{ 
	return SaveLineToFile ( pFile, FALSE, FALSE ); 
}

//**********************************************************************

bool CSQLAuditRecord_base::SaveSessionLine ( const char* szFilename )	
{ 
	return Save ( szFilename, TRUE ); 
}

//**********************************************************************

void CSQLAuditRecord_base::BufferNewLine()
{
	m_dateFiled.SetCurrentDate();						// audit filed date
	m_timeFiled.SetCurrentTime();						// audit filed time

	CSQLRowCCNo rowCC;

	CString strCCNoLabel = "";
	switch (m_nSourceType)
	{
	case AUDIT_SERVER:		
		rowCC.SetType(CCNO_TYPE_SERVER);
		break;

	case AUDIT_PC:		
		rowCC.SetType(CCNO_TYPE_PC);
		break;

	case AUDIT_POS:	
		rowCC.SetType(CCNO_TYPE_POS);	
		break;

	case AUDIT_KIOSKLINK:
		rowCC.SetType(CCNO_TYPE_KIOSK);
		break;

	case AUDIT_EODBALANCE:
		rowCC.SetType(CCNO_TYPE_EODBALANCE);
		break;

	case AUDIT_VENDING:	
		rowCC.SetType(CCNO_TYPE_VENDING);
		break;

	case AUDIT_EXT:			
		rowCC.SetType(CCNO_TYPE_EXTERNAL);	
		break;

	default:
		rowCC.SetType(CCNO_TYPE_UNKNOWN);
		break;
	}

	//ROW MAY NOT EXIST
	CSQLRepositoryCCNo repoCC;
	repoCC.SelectRow(rowCC, NULL);
	m_nCCNoFull = rowCC.GetNumber();
	rowCC.IncrementCCNo();
	repoCC.UpsertRow(rowCC, NULL);
	
	WriteLineToArray(m_arrayPendingAudits);
}

//**********************************************************************
// private function

bool CSQLAuditRecord_base::Save(const char* szFilename, bool bIsTemporaryFile, bool bNoWebUpdateForPebble)
{
	bool bReply = FALSE;

	CSSFile file;
	if ((bReply = file.Open(szFilename, "ab")) == TRUE)
	{
		bReply = SaveLineToFile(&file, bIsTemporaryFile, bNoWebUpdateForPebble);				// temp file - no increment of ccno's
		file.Close();
	}

	return bReply;
}

//**********************************************************************
// private function
// if TempFile doesn't increment CCNo or save to webupdates ( also used by audit leavers )

bool CSQLAuditRecord_base::SaveLineToFile()
{
	return SaveLineToFile(Filenames.GetAuditFilename(), FALSE);
}

//**********************************************************************

bool CSQLAuditRecord_base::SaveLineToFile(const char* szFilename, bool bIsTemporaryFile)
{
	bool bReply = FALSE;

	CSSFile file;
	if ((bReply = file.Open(szFilename, "ab")) == TRUE)
	{
		bReply = SaveLineToFile(&file, bIsTemporaryFile, FALSE);
		file.Close();
	}

	return bReply;
}

//**********************************************************************

bool CSQLAuditRecord_base::SaveLineToFile(CSSFile* pFile, bool bIsTemporaryFile, bool bNoWebUpdateForPebble)
{
	m_dateFiled.SetCurrentDate();						// audit filed date
	m_timeFiled.SetCurrentTime();						// audit filed time

	CSQLRowCCNo rowCC;

	CString strCCNoLabel;
	switch (m_nSourceType)
	{
	case AUDIT_SERVER:
		rowCC.SetType(CCNO_TYPE_SERVER);
		break;

	case AUDIT_PC:
		rowCC.SetType(CCNO_TYPE_PC);
		break;

	case AUDIT_POS:
		rowCC.SetType(CCNO_TYPE_POS);
		break;

	case AUDIT_KIOSKLINK:
		rowCC.SetType(CCNO_TYPE_KIOSK);
		break;

	case AUDIT_EODBALANCE:
		rowCC.SetType(CCNO_TYPE_EODBALANCE);
		break;

	case AUDIT_VENDING:
		rowCC.SetType(CCNO_TYPE_VENDING);
		break;

	case AUDIT_EXT:
		rowCC.SetType(CCNO_TYPE_EXTERNAL);
		break;
	}

	m_nCCNoFull = 0;
	CSQLRepositoryCCNo repoCC;
	repoCC.SelectRow(rowCC, NULL);
	m_nCCNoFull = rowCC.GetNumber();

	if (bIsTemporaryFile == FALSE)						// don't inc ccno if its a temp file
	{
		rowCC.IncrementCCNo(); 
		repoCC.UpsertRow(rowCC, NULL);
	}

	bool bReply = WriteLineToFile(pFile);						// save audit record line

	if ((TRUE == bReply) && (FALSE == bIsTemporaryFile))		// save audit line for web updating & sales exporting
	{
		SaveWebUpdates(bNoWebUpdateForPebble);
	}

	return bReply;
}

//********************************************************************

void CSQLAuditRecord_base::SaveWebUpdates(bool bNoWebUpdateForPebble)
{
	if ((System.GetWebPaymentType() != nWEBPAYMENT_PEBBLE) || (FALSE == bNoWebUpdateForPebble))
	{
		CString strUpdates = Filenames.GetWebPaymentUpdatesFilename("dat");		// ..\{web}\webupdates.dat
		
		if (strUpdates != "")
		{
			CSSFile file;
			if (file.Open(strUpdates, "ab") == TRUE)
			{
				WriteLineToFile(&file);
				file.Close();
			}
		}
	}

	if ( System.GetEnableExportSalesFlag() == TRUE)									// special export of sales \ topups 
	{
		CString strUpdates = Filenames.GetWebPaymentUpdatesFilename ( nWEBPAYMENT_EXPORTSALES, "dat" );	// ..\{web}\webupdates.dat

		CSSFile file;
		if ( file.Open ( strUpdates, "ab" ) == TRUE )
		{
			WriteLineToFile( &file );
			file.Close();
		}
	}
}

//********************************************************************

void CSQLAuditRecord_base::ClearPendingLines()
{
	m_arrayPendingAudits.RemoveAll();
}

//********************************************************************

void CSQLAuditRecord_base::WritePendingLines()
{
	CSSFile fileAudit, fileWebUpdate, fileWebSales,fileChartwells;

	fileAudit.Open(Filenames.GetAuditFilename(), "ab");

	// ..\{web}\webupdates.dat
	CString strUpdates = Filenames.GetWebPaymentUpdatesFilename("dat");		
	if (strUpdates != "")
	{
		fileWebUpdate.Open(strUpdates, "ab");
	}

	// special export of sales \ topups 
	CString strSales = "";
	if (System.GetEnableExportSalesFlag() == TRUE)
	{
		// ..\{web}\webupdates.dat
		strSales = Filenames.GetWebPaymentUpdatesFilename(nWEBPAYMENT_EXPORTSALES, "dat");	
	}

	if ( strSales != "" )
	{
		fileWebSales.Open(strSales, "ab");
	}

	for (int n = 0; n < m_arrayPendingAudits.GetSize(); n++)
	{
		CString strLine = m_arrayPendingAudits.GetAt(n);
		fileAudit.WriteLine(strLine);

		if (strUpdates != "")
		{
			fileWebUpdate.WriteLine(strLine);
		}

		if (strSales != "")
		{
			fileWebSales.WriteLine(strLine);
		}
	}
}

//********************************************************************
//write audit data to file

bool CSQLAuditRecord_base::WriteLineToFile(CSSFile* pFile)
{
	CCSV csv(',', '"', TRUE, TRUE, TRUE);
	CreateWriteableLine(csv);
	return pFile->Write(csv);						// audit record line
}

//*********************************************************************

bool CSQLAuditRecord_base::WriteLineToArray(CStringArray& array)
{
	CCSV csv(',', '"', TRUE, TRUE, TRUE);
	CreateWriteableLine(csv);
	array.Add(csv.GetLine());
	return TRUE;
}

//*********************************************************************

void CSQLAuditRecord_base::CreateWriteableLine(CCSV& csv)
{
	CString strUserID = m_strUserID;
	strUserID.TrimLeft('0');

	CString strXFerUserID = m_strXFerUserID;
	strXFerUserID.TrimLeft('0');

	CString strPasswordNo = "";

#ifdef SYSTEMTYPE_MANAGER

	switch (m_nSourceType)
	{
	case AUDIT_PC:
		strPasswordNo.Format("%d", Passwords.GetPasswordInUse() + 1);
		break;
	}

#endif

	csv.Add(m_dateFiled.GetCSVDate());				// audit filed date
	csv.Add(m_timeFiled.GetCSVTime());				// audit filed time

	if (m_nSourceType == AUDIT_EODBALANCE)			// Internal EOD Balance line
	{
		csv.Add("");									// date
		csv.Add("");									// time
		csv.Add("");									// user  no
		csv.Add("");									// user name
		csv.Add("");									// group no
		csv.Add("");									// points added
		csv.Add("");									// cash spend
		csv.Add("");									// purse added
		csv.Add("");									// transaction total
		csv.Add("");									// current points
	}
	else
	{
		csv.Add(m_dateLastUsed.GetCSVDate());		// date
		csv.Add(m_timeLastUsed.GetCSVTime());		// time
		csv.Add(strUserID);							// user  no
		csv.Add(m_strUserName);						// user name

		int nGroupNo = (m_nAlternateGroupNo == 0) ? m_nGroupNo : m_nAlternateGroupNo;
		csv.Add(nGroupNo);							// group no

		csv.Add(m_nTransactionPoints);				// points added
		csv.Add(m_dTransactionCash, 2);				// cash spend
		csv.Add(m_dTransactionPurse1Liability, 2);	// purse added
		csv.Add(m_dTransactionPurse2Balance, 2);	// transaction total
		csv.Add(m_nCurrentPoints);					// current points
	}

	csv.Add(m_dCurrentPurse1Liability, 2);			// current purse
	csv.Add(m_dCurrentPurse2Balance, 2);			// current purse
	csv.Add(m_nToDatePoints);						// points todate
	csv.Add(m_dToDateCashSpend, 2);					// cash spend todate
	csv.Add(m_dToDatePurse1Spend, 2);				// p1 spend todate
	csv.Add(m_dToDatePurse2Spend, 2);				// p2 spend todate
	csv.Add(m_nSourceType);							// source of audit info
	csv.Add(m_nApplicationNo);						// application no
	csv.Add(m_nCCNoFull);
	csv.Add(m_nTerminalNo);
	csv.Add(m_strOperatorID);
	csv.Add(m_strAuditLineExtraText);
	csv.Add(m_strComment);
	csv.Add(m_dTransactionPurse3Liability, 2);		// transaction total
	csv.Add(m_dCurrentPurse3Liability, 2);			// current purse
	csv.Add(m_dToDatePurse3Spend, 2);				// p3 spend todate
	csv.Add(m_dCurrentPurse1Credit, 2);				// p1 credit
	csv.Add(m_dCurrentPurse3Credit, 2);				// p3 credit
	csv.Add(m_dTransactionPurse1Credit, 2);			// p3 credit
	csv.Add(m_dTransactionPurse3Credit, 2);			// p3 credit
	csv.Add(m_nFolderSet);
	csv.Add(m_nREQFileType);
	csv.Add(strPasswordNo);
	csv.Add(m_strXFerUserID);
}

//*******************************************************************

bool CSQLAuditRecord_base::ReadLine(CSSFile* file)
{
	CCSV csv;
	while (TRUE)
	{
		if (file->Read(csv) == FALSE)
		{
			return FALSE;							// end of file
		}

		if (csv.GetSize() > 0)					// lose blank lines, just in case
		{
			break;
		}
	}

	return ExtractData(&csv);
}

//*******************************************************************

bool CSQLAuditRecord_base::ReadLineFromArray(CStringArray* pArray, int& nNextArrayLineNo)
{
	CCSV csv;
	while (TRUE)
	{
		if (nNextArrayLineNo >= pArray->GetSize())
		{
			return FALSE;
		}

		csv.ParseLine(pArray->GetAt(nNextArrayLineNo++));

		if (csv.GetSize() > 0)					// lose blank lines, just in case
		{
			break;
		}
	}

	return ExtractData(&csv);
}

//*******************************************************************

bool CSQLAuditRecord_base::ExtractData(CCSV* pCsv)
{
	m_dateFiled.SetDate(pCsv->GetString(0));			// date filed
	m_timeFiled.SetTime(pCsv->GetString(1));			// time filed
	m_dateLastUsed.SetDate(pCsv->GetString(2));		// date used
	m_timeLastUsed.SetTime(pCsv->GetString(3));		// time used

	pCsv->Get(4, m_strUserID);
	pCsv->Get(5, m_strUserName);					// card name
	pCsv->Get(6, m_nGroupNo);						// group number
	pCsv->Get(7, m_nTransactionPoints);				// points added
	pCsv->Get(8, m_dTransactionCash);				// cash spend
	pCsv->Get(9, m_dTransactionPurse1Liability);	// purse1 transaction value
	pCsv->Get(10, m_dTransactionPurse2Balance);		// purse2 transaction value
	pCsv->Get(11, m_nCurrentPoints);				// current points
	pCsv->Get(12, m_dCurrentPurse1Liability);		// current purse1
	pCsv->Get(13, m_dCurrentPurse2Balance);			// current purse2
	pCsv->Get(14, m_nToDatePoints);					// points todate
	pCsv->Get(15, m_dToDateCashSpend);				// cash spend todate
	pCsv->Get(16, m_dToDatePurse1Spend);			// p1 spend todate
	pCsv->Get(17, m_dToDatePurse2Spend);			// p2 spend todate
	pCsv->Get(18, m_nSourceType);					// source of audit info
	pCsv->Get(19, m_nApplicationNo);				// application no
	pCsv->Get(20, m_nCCNoFull);
	pCsv->Get(21, m_nTerminalNo);
	pCsv->Get(22, m_strOperatorID);
	pCsv->Get(23, m_strAuditLineExtraText);			// additional info
	pCsv->Get(24, m_strComment);					// audit comment
	pCsv->Get(25, m_dTransactionPurse3Liability);	// transaction total
	pCsv->Get(26, m_dCurrentPurse3Liability);		// current purse
	pCsv->Get(27, m_dToDatePurse3Spend);			// p3 spend todate
	pCsv->Get(28, m_dCurrentPurse1Credit);			// p1 credit
	pCsv->Get(29, m_dCurrentPurse3Credit);			// p3 credit
	pCsv->Get(30, m_dTransactionPurse1Credit);		// p1 credit
	pCsv->Get(31, m_dTransactionPurse3Credit);		// p3 credit
	pCsv->Get(32, m_nFolderSet);
	pCsv->Get(33, m_nREQFileType);
	pCsv->Get(34, m_nPasswordNo);
	pCsv->Get(35, m_strXFerUserID);					// transfer user ID

	return TRUE;
}

//*******************************************************************

void CSQLAuditRecord_base::SetWebPaymentInfo ( __int64 nPaymentID )
{
	CString strText = ::FormatInt64Value( nPaymentID );
	SetAuditLineExtraText ( strText );
}

//**********************************************************************

void CSQLAuditRecord_base::SetOperatorID ( int nOperatorNo )
{
	CString strID = "";
	strID.Format ( "%d", nOperatorNo );
	SetOperatorID ( strID );
}

//**********************************************************************

CString CSQLAuditRecord_base::GetREQFileTypeText()
{
	CString strResult = "";
	switch (m_nREQFileType)
	{
	case REQFILETYPE_CARDSCAN:
		strResult = "Card scan";
		break;

	case REQFILETYPE_BIO:
		strResult = "Bio";
		break;

	case REQFILETYPE_MANUAL:
		strResult = "Manual";
		break;

	case REQFILETYPE_DEFAULT:
		strResult = "Default";
		break;

	case REQFILETYPE_INTERNAL:
		strResult = "Internal";
		break;

	case REQFILETYPE_PIN:
		strResult = "PIN";
		break;

	case REQFILETYPE_UNKNOWN:
		strResult = "Unknown";
		break;

	case REQFILETYPE_NONE:
		strResult = "None";
		break;

	default:
		strResult.Format( "Other (%d)", m_nREQFileType);
		break;
	}

	return strResult;
}

//**********************************************************************

CString CSQLAuditRecord_base::GetLineOrDefaultComment()
{
	CString strComment = m_strComment;
	
	if (strComment == "")
	{
		strComment = CAuditReportTexts::GetDefaultComment(m_nApplicationNo);
	}

	return strComment;
}

//**********************************************************************

double CSQLAuditRecord_base::GetTransactionTotal()
{
	double dTotal = 0.0;
	dTotal += m_dTransactionPurse1Liability;
	dTotal += m_dTransactionPurse1Credit;
	dTotal += m_dTransactionPurse2Balance;
	dTotal += m_dTransactionPurse3Liability;
	dTotal += m_dTransactionPurse3Credit;
	dTotal += m_dTransactionCash;
	return dTotal;
}

//**********************************************************************

bool CSQLAuditRecord_base::SaveLineToArray(CStringArray& array)
{
	m_dateFiled.SetCurrentDate();						// audit filed date
	m_timeFiled.SetCurrentTime();						// audit filed time
	return WriteLineToArray(array);						// save audit record line
}

//*********************************************************************

bool CSQLAuditRecord_base::ReadFromString(CString str)
{
	CCSV csv(str);
	return ExtractData(&csv);
}

//*********************************************************************

CString CSQLAuditRecord_base::GetReportPasswordNo()
{
	CString strPasswordNo = "--";

	if (m_nPasswordNo >= 0)
	{
		strPasswordNo.Format("%d", m_nPasswordNo);
	}
	return strPasswordNo;
}

//*********************************************************************
