//******************************************************************************
#include "AuditRecord.h"
#include "RepmanHandler.h"
#include "DateRangeDlg.h"
#include "ReportInfoDateRange.h"
#include "WorkingDlg.h"
//******************************************************************************
#include "ReportVisitTotaliser.h"
//******************************************************************************

static const char* szHEADER			= "CardNo,Name,Count,D1,D2,D3,D4,D5,D6,D7,D8,D9,D10,D11,D12";
static const char* szSORTFIELD		= "CardNo,ascending";

static const char* szLABEL_CARDNO	= "CardNo";
static const char* szLABEL_NAME 	= "Name";
static const char* szLABEL_COUNT 	= "Count";

#define nRETRIES		10
#define nMAX_DAYS		31
#define nMAX_MONTHS		12
#define szDEFAULTDAYS	"0000000000000000000000000000000"			// 31 day indicators

//******************************************************************************

CReportVisitTotaliser::CReportVisitTotaliser(CWnd* pParent)
{
	m_pParent = pParent;
	m_strReportLabel = "R121";
	m_strCaption = "Visit Totaliser";
	m_strReportKey = "";									// gets set when date range selected
	Reset();
}

//******************************************************************************

void CReportVisitTotaliser::Reset()
{
	for (int i = 0; i <= nMAX_MONTHS; i++)				// Month[0] = report total
	{
		m_nMonthTotals[i] = 0;
	}
}

//******************************************************************************

void CReportVisitTotaliser::Show()
{
	CRepmanHandler repman(m_strCaption, m_pParent);

	int nErrorNo = m_auditFile.ValidateFile();				// check main audit file ok to report on
	if (nErrorNo == nREPORT_NOERROR)						// audit file ok to report on
	{
		CString strReportFile = Filenames.GetReportFilename(m_strReportLabel);
		CString strParams = Filenames.GetReportParamsFilename(m_strReportLabel);

		do
		{
			if ((nErrorNo = Create(strReportFile)) == nREPORT_NOERROR)
			{
				nErrorNo = repman.DisplayReport(strReportFile, strParams, m_strReportKey);
				repman.DisplayError(nErrorNo);
			}
		} 
		while (nErrorNo != nREPORT_CANCEL);
	}

	repman.DisplayError(nErrorNo);
}

//******************************************************************************

int CReportVisitTotaliser::Create ( const char* szReportFile )
{
	CReportInfoDateRange ReportInfo( m_strReportLabel, m_strCaption );
	CDateRangeDlg dateRange ( m_strReportLabel, m_pParent );
	dateRange.SetReportInfo(&ReportInfo);

	if (dateRange.DoModal() == IDCANCEL)
	{
		return nREPORT_CANCEL;
	}

	if (m_auditFile.OpenReadOnly() == FALSE)								// open main audit file
	{
		return nREPORT_OPENERROR;
	}

	m_auditFile.JumpIn ( ReportInfo.GetDateFromString() );

	CAuditRecord atcRecord;
	if ( m_auditFile.ReadLine ( &atcRecord ) == FALSE )
	{
		m_auditFile.Close();
		return nREPORT_NOMATCH;
	}

//*** start by creating a temp database 

	Reset();

	m_strReportKey = ReportInfo.GetReportKey();			// set report key for when report displayed

	int nDbCount = 0;									// used to read data into
	CString strDbLabel = "";							
	CString strDbField = "";							
	
	CString strDbFilename;
	strDbFilename.Format ( "%s\\%s.$$$", 
		(const char*) Sysset.GetTempPath(),
		(const char*) m_strReportLabel );

	CSSStringDatabase dbData;
	dbData.CreateSSDatabase( strDbFilename, "", szHEADER, szSORTFIELD, nRETRIES );	// create a temp empty db
	dbData.Close();

	int nReply = nREPORT_NOERROR;

	if ( dbData.OpenSSDatabaseReadWrite ( strDbFilename, "", szSORTFIELD, nRETRIES ) == DB_ERR_NONE )
	{
		CWorkingDlg WorkingDlg ( "Creating report", TRUE );
		WorkingDlg.Create();
	
		while ( m_auditFile.ReadLine ( &atcRecord ) == TRUE )
		{
			if ( WorkingDlg.SetCaption2PercentDone(m_auditFile.GetPercentPosition(), TRUE) == TRUE )
			{
				nReply = nREPORT_CANCEL;
				break;
			}

			if ( ReportInfo.IsValid ( atcRecord.GetDateLastUsed() ) == TRUE ) // see if date in range
			{
				if ( atcRecord.GetApplicationNo() == APPNO_SALE )			// see if audit of epos sale
				{
					double dSales = -atcRecord.GetPurse1Transaction();		// sales are negative
					dSales -= atcRecord.GetPurse2Transaction();
					dSales -= atcRecord.GetCashTransaction();

					if (dSales > 0)										// check for a posoitive sale ( ie not a refund )
					{
						CString strCardNo = System.FormatCardNo(atcRecord.GetCardNo());

						if (dbData.FindRecord(strCardNo) == FALSE)			// see if cardno already found
						{
							dbData.AddBlankRecord(strCardNo);						// no add new record
						}

						strDbLabel.Format("D%d", atcRecord.m_dateLastUsed.GetMonth());
						dbData.Get(strDbLabel, strDbField);					// get current day flags for card record

						if (strDbField == "")									// see if any set so far
						{
							strDbField = szDEFAULTDAYS;							// no - add 31 blank day indicators
						}

						int nDayPtr = atcRecord.m_dateLastUsed.GetDay() - 1;
						if (strDbField.GetAt(nDayPtr) == '0')					// see if day set
						{
							strDbField.SetAt(nDayPtr, 'x');					// no - set it
							dbData.Set(strDbLabel, strDbField);				// save it in dbase

							dbData.Get(szLABEL_COUNT, nDbCount);				// inc count of number of days
							dbData.Set(szLABEL_COUNT, nDbCount + 1);
						}

						dbData.Get(szLABEL_NAME, strDbField);					// get name set in database
						if (strDbField == "")										// see if set
						{
							dbData.Set(szLABEL_NAME, atcRecord.GetCardName());	// no - set name from audit line
						}
					}
				}
			}
		}

		dbData.Close();
		WorkingDlg.DestroyWindow();
	}
	else
	{
		nReply = nREPORT_DBFILEERROR;
	}

	m_auditFile.Close();

//*** sort database if required & display

	if (nReply == nREPORT_NOERROR)
	{
		if (m_Report.Open(szReportFile) == TRUE)
		{
			CSSUnsortedDatabase dbSort;
			if (dbSort.OpenSSDatabaseReadOnly(strDbFilename, nRETRIES) == DB_ERR_NONE)
			{
				CString strTitle = "";
				if (ReportInfo.GetSummaryFlag() == TRUE)				// summary flag used to Rank Entries
				{
					dbSort.ClearSort();
					dbSort.AddSort(szLABEL_COUNT, DB_SORT_INT, DB_SORT_DESCENDING);
					dbSort.AddSort(szLABEL_CARDNO, DB_SORT_TEXT, DB_SORT_ASCENDING);
					dbSort.Sort();

					strTitle = "Ranked ";
				}

				strTitle += m_strCaption;
				strTitle += ReportInfo.GetDateRangeString();
				m_Report.SetStyle1(strTitle);

				AddColumnText();

				dbSort.MoveFirst();
				do
				{
					CCSV csv('\t');

					dbSort.Get(szLABEL_CARDNO, strDbField);
					csv.Add(strDbField);

					dbSort.Get(szLABEL_NAME, strDbField);
					csv.Add(strDbField);

					for (int nMonthNo = 1; nMonthNo <= nMAX_MONTHS; nMonthNo++)
					{
						strDbLabel.Format("D%d", nMonthNo);
						dbSort.Get(strDbLabel, strDbField);				// get current day flags

						int nCount = 0;
						if (strDbField.GetLength() == nMAX_DAYS)			// check have some entries
						{
							for (int i = 0; i < nMAX_DAYS; i++)			// count number of days
							{
								if (strDbField.GetAt(i) == 'x')			// see if day of month been set
								{
									++nCount;
								}
							}
						}

						AddCount(&csv, nCount);
						m_nMonthTotals[nMonthNo] += nCount;					// add to report total for given month
					}

					dbSort.Get(szLABEL_COUNT, nDbCount);					// get total numer of days for given card no
					csv.Add(nDbCount);									// total number of days

					m_nMonthTotals[0] += nDbCount;							// add count to report total

					m_Report.WriteLine(csv);
				} while (dbSort.MoveNext() == TRUE);
				dbSort.Close();

				SaveTotals();												// show report totals
			}
			else
			{
				nReply = nREPORT_DBFILEERROR;
			}

			m_Report.Close();
		}
		else
		{
			nReply = nREPORT_CREATEFAIL;
		}
	}

	::SendToRecycleBin ( strDbFilename );

	return nReply;
}

//**************************************************************************

void CReportVisitTotaliser::AddColumnText()
{
	m_Report.AddColumn(System.GetCardNoText(), TA_LEFT, 190);
	m_Report.AddColumn("Name", TA_LEFT, 250);
	m_Report.AddColumn("Jan", TA_RIGHT, 150);
	m_Report.AddColumn("Feb", TA_RIGHT, 150);
	m_Report.AddColumn("Mar", TA_RIGHT, 150);
	m_Report.AddColumn("Apr", TA_RIGHT, 150);
	m_Report.AddColumn("May", TA_RIGHT, 150);
	m_Report.AddColumn("Jun", TA_RIGHT, 150);
	m_Report.AddColumn("Jul", TA_RIGHT, 150);
	m_Report.AddColumn("Aug", TA_RIGHT, 150);
	m_Report.AddColumn("Sep", TA_RIGHT, 150);
	m_Report.AddColumn("Oct", TA_RIGHT, 150);
	m_Report.AddColumn("Nov", TA_RIGHT, 150);
	m_Report.AddColumn("Dec", TA_RIGHT, 150);
	m_Report.AddColumn("Total", TA_RIGHT, 200);
}

//*********************************************************************

void CReportVisitTotaliser::SaveTotals()
{
	CCSV csv('\t');

	csv.Add("Totals");
	csv.Add("");

	for (int nMonthNo = 1; nMonthNo <= nMAX_MONTHS; nMonthNo++)
	{
		AddCount(&csv, m_nMonthTotals[nMonthNo]);
	}

	csv.Add(m_nMonthTotals[0]);							// report total

	m_Report.WriteLine("\t\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>");
	m_Report.WriteLine(csv);
}

//*********************************************************************

void CReportVisitTotaliser::AddCount(CCSV* pCsv, int nCount)
{
	if (0 == nCount)
	{
		pCsv->Add("-");
	}
	else
	{
		pCsv->Add(nCount);
	}
}

//*********************************************************************
