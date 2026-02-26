/******************************************************************************/
#include "SQLTable_CCNo\\SQLRepositoryCCNo.h"
/******************************************************************************/
#include "AuditRecord_base.h"
/******************************************************************************/

// used by LoyaltyServer & Audit reporting
CAuditRecord_base::CAuditRecord_base()
{
	Set ( AUDIT_SERVER );
}

//********************************************************************
//** used by Loyalty manager to save a card database audit line
// & by LoyaltyServer to audit Purse2 Refreshes
void CAuditRecord_base::Set(int nSourceType, bool bSameEposTran)
{
	m_dateFiled.SetDate("");						// date audit line was first saved
	m_timeFiled.SetTime("");						// time audit line was first saved
	m_dateLastUsed.SetDate("");						// date last used @ ecr
	m_timeLastUsed.SetTime("");						// time last used @ ecr
	m_strCardNo = "";					// card no
	m_strCardName = "";					// card name
	m_nGroupNo = 1;
	m_nPointsTransaction = 0;					// amount of points in transaction
	m_dPurse1Transaction = 0.0;					// Purse1 deduction
	m_dPurse2Transaction = 0.0;					// Purse2 deduction
	m_dCashTransaction = 0.0;					// amount of cash in transaction
	m_dTotalDiscount = 0.0;					// total discount in transaction
	m_nPoints = 0;					// current points
	m_nPointsTD = 0;					// points todate
	m_dPurse1 = 0.0;					// current purse1
	m_dPurse2 = 0.0;					// current purse2
	m_dPurse1SpendTD = 0;					// Purse spend todate
	m_dPurse2SpendTD = 0;					// Purse spend todate
	m_dCashSpendTD = 0;					// cash spend todate
	m_nSourceType = nSourceType;			// source of audit info
	m_nApplicationNo = 0;					// application no
	m_nCCNo = 0;
	m_strComment = "";					// audit comment
	
	if (FALSE == bSameEposTran)
	{
		m_nTerminalNo = 0;
		m_nOperatorNo = 0;
		m_strEposTranNo = "";				// epos transaction number
		m_FolderSetIndex.m_nDbNo = 0;
		m_FolderSetIndex.m_nSetNo = 0;
	}
}

//**********************************************************************

void CAuditRecord_base::SetSQL(CSQLRowAccountFull& RowAccount, int nSourceType, bool bSameEposTran)
{
	Set(nSourceType,bSameEposTran);								// reset variables

	if (nSourceType == AUDIT_PC)
	{
		CSSDate date;
		CSSTime time;
		RowAccount.GetLastUsed(date, time);
		m_dateLastUsed.SetDate(date.GetDate());
		m_timeLastUsed.SetTime(time.GetTime());
	}

	m_strCardNo = SimpleFormatInt64Value(RowAccount.GetUserID());		// card no
	m_strCardName = RowAccount.GetFullname();		// card name
	m_nGroupNo = RowAccount.GetGroupNo();		// Group number
	m_nPoints = RowAccount.GetPoints();		// current points
	m_nPointsTD = RowAccount.GetPointsToDate();		// points todate
	m_dPurse1 = RowAccount.GetPurse1Balance();	// current purse1
	m_dPurse2 = RowAccount.GetPurse2Balance();	// current purse2
	m_dPurse1SpendTD = RowAccount.GetPurse1SpendToDate();	// p1 spend todate
	m_dPurse2SpendTD = RowAccount.GetPurse2SpendToDate();	// p2 spend todate
	m_dCashSpendTD = RowAccount.GetCashSpendToDate();	// cash spend todate
}

//**********************************************************************
// save line to main audit file

bool CAuditRecord_base::SaveLineToFile()
{
	return SaveLineToFile(Filenames.GetAuditFilename(), FALSE);
}

//**********************************************************************

// save line to specified audit filename
bool CAuditRecord_base::SaveLineToFile(const char* szFilename, bool bIsTemporaryFile)
{
	bool bReply;

	CSSFile file;
	if ((bReply = file.Open(szFilename, "ab")) == TRUE)
	{
		bReply = SaveLineToFile(&file, bIsTemporaryFile);
		file.Close();
	}

	return bReply;
}

//**********************************************************************

//save line to specified file pointer
bool CAuditRecord_base::SaveLineToFile ( CSSFile *pFile, bool bIsTemporaryFile )
{
	m_dateFiled.SetCurrentDate();						// audit filed date
	m_timeFiled.SetCurrentTime();						// audit filed time

	m_nCCNo = 0;
	if ( bIsTemporaryFile == FALSE )					// don't inc ccno if its a temp file
	{													// ccno will be set when actually save in final audit file
		CSQLRowCCNo RowCC;
		
		switch (m_nSourceType)
		{
		case AUDIT_SERVER:
			RowCC.SetType(CCNO_TYPE_SERVER);
			break;

		case AUDIT_PC:
			RowCC.SetType(CCNO_TYPE_PC);
			break;

		case AUDIT_POS:
			RowCC.SetType(CCNO_TYPE_POS);
			break;

		case AUDIT_EXT:
			RowCC.SetType(CCNO_TYPE_EXTERNAL);
			break;
		}

		//ROW MAY NOT EXIST
		CSQLRepositoryCCNo repoCC;
		repoCC.SelectRow(RowCC, NULL);
		m_nCCNo = RowCC.GetNumber();
		RowCC.IncrementCCNo();
		repoCC.UpsertRow(RowCC, NULL);
	}

	return WriteLineToFile( pFile );							// save audit record line
}

//*********************************************************************

bool CAuditRecord_base::SaveLineToArray( CStringArray& array)
{
	m_dateFiled.SetCurrentDate();						// audit filed date
	m_timeFiled.SetCurrentTime();						// audit filed time
	return WriteLineToArray(array);						// save audit record line
}

//*********************************************************************

bool CAuditRecord_base::WriteLineToFile(CSSFile* pFile)
{
	CCSV csv(',', '"', TRUE, TRUE, TRUE);
	CreateWriteableLine(csv);
	return pFile->Write(csv);						// audit record line
}

//*********************************************************************

bool CAuditRecord_base::WriteLineToArray(CStringArray& array)
{
	CCSV csv(',', '"', TRUE, TRUE, TRUE);
	CreateWriteableLine(csv);
	array.Add(csv.GetLine());
	return TRUE;
}

//*********************************************************************

void CAuditRecord_base::CreateWriteableLine(CCSV& csv)
{
	CString strCardNo = m_strCardNo;
	strCardNo.TrimLeft('0');

	csv.Add(m_dateFiled.GetCSVDate());				// audit filed date
	csv.Add(m_timeFiled.GetCSVTime());				// audit filed time
	csv.Add(m_dateLastUsed.GetCSVDate());			// date
	csv.Add(m_timeLastUsed.GetCSVTime());			// time
	csv.Add(strCardNo);								// card no
	csv.Add(m_strCardName);							// card name
	csv.Add(m_nGroupNo);
	csv.Add(m_nPointsTransaction);					// points added
	csv.Add(m_dPurse1Transaction, 2);				// purse1 added
	csv.Add(m_dCashTransaction, 2);					// transaction total
	csv.Add(m_nPoints);								// current points
	csv.Add(m_dPurse1, 2);							// current purse1
	csv.Add(m_nPointsTD);							// points todate
	csv.Add(m_dPurse1SpendTD, 2);					// p1 spend todate
	csv.Add(m_dCashSpendTD, 2);						// cash spend todate
	csv.Add(m_nSourceType);							// source of audit info
	csv.Add(m_nApplicationNo);						// application no
	csv.Add(m_nCCNo);
	csv.Add(m_nTerminalNo);
	csv.Add(m_nOperatorNo);
	csv.Add(m_strEposTranNo);
	csv.Add(m_strComment);
	csv.Add(m_dPurse2Transaction, 2);				// purse2 added
	csv.Add(m_dPurse2, 2);							// current purse2
	csv.Add(m_dPurse2SpendTD, 2);					// p2 spend todate
	csv.Add(m_FolderSetIndex.m_nSetNo);
	csv.Add(m_FolderSetIndex.m_nDbNo);
	csv.Add(m_dTotalDiscount, 2);					// total discount in transaction
}

//*********************************************************************

bool CAuditRecord_base::ReadLine ( CSSFile* file )
{
	CCSV csv;
	while ( TRUE )
	{
		// end of file
		if (file->Read(csv) == FALSE)
		{
			return FALSE;							
		}

		// lose blank lines, just in case
		if (csv.GetSize() > 0)
		{
			break;
		}
	}

	return ExtractData ( &csv );
}

//*********************************************************************

bool CAuditRecord_base::ReadFromString(CString str)
{
	CCSV csv(str);
	return ExtractData(&csv);
}

//*********************************************************************

bool CAuditRecord_base::ExtractData(CCSV* pCsv)
{
	m_dateFiled.SetDate(pCsv->GetString(0));			// date filed
	m_timeFiled.SetTime(pCsv->GetString(1));			// time filed
	m_dateLastUsed.SetDate(pCsv->GetString(2));		// date
	m_timeLastUsed.SetTime(pCsv->GetString(3));		// time  

	pCsv->Get(4, m_strCardNo);
	pCsv->Get(5, m_strCardName);					// card name
	pCsv->Get(6, m_nGroupNo);						// group number
	pCsv->Get(7, m_nPointsTransaction);				// points added
	pCsv->Get(8, m_dPurse1Transaction);				// P1
	pCsv->Get(9, m_dCashTransaction);				// cash 
	pCsv->Get(10, m_nPoints);						// current points
	pCsv->Get(11, m_dPurse1);						// current purse1
	pCsv->Get(12, m_nPointsTD);						// points todate
	pCsv->Get(13, m_dPurse1SpendTD);					// p1 spend todate
	pCsv->Get(14, m_dCashSpendTD);					// cash spend todate
	pCsv->Get(15, m_nSourceType);					// source of audit info
	pCsv->Get(16, m_nApplicationNo);					// application no
	pCsv->Get(17, m_nCCNo);
	pCsv->Get(18, m_nTerminalNo);
	pCsv->Get(19, m_nOperatorNo);
	pCsv->Get(20, m_strEposTranNo);
	pCsv->Get(21, m_strComment);
	pCsv->Get(22, m_dPurse2Transaction);			// purse2 added
	pCsv->Get(23, m_dPurse2);						// current purse2
	pCsv->Get(24, m_dPurse2SpendTD);				// p2 spend todate
	pCsv->Get(25, m_FolderSetIndex.m_nSetNo);
	pCsv->Get(26, m_FolderSetIndex.m_nDbNo);
	pCsv->Get(27, m_dTotalDiscount);				// total discount in transaction
	return TRUE;
}

//**********************************************************************

