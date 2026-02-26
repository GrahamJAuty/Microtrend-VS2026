/**********************************************************************/
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
/**********************************************************************/
#include "LoyaltyManager.h"
#include "RepmanHandler.h"
/**********************************************************************/
#include "ReportCSVFile.h"
/**********************************************************************/

CCSVFileReport::CCSVFileReport ( const char* szLabel, const char* szFilename, CWnd* pParent )
{
	m_strFilename = szFilename;
	m_strReportLabel = szLabel;
	m_pParent = pParent;
	m_strReportTitle = "";
	m_nMode = P_NOWAIT;
}

/**********************************************************************/

void CCSVFileReport::Show(const char* szTitle)
{
	m_strReportTitle = szTitle;

	if (m_strReportTitle == "")
	{
		m_strReportTitle = "CSV File";
	}

	int nErrorNo;
	CString strReportFile = Filenames.GetReportFilename(m_strReportLabel);
	CString strParams = Filenames.GetReportParamsFilename(m_strReportLabel);

	CRepmanHandler repman(m_strReportTitle, m_pParent);
	repman.SetMode(m_nMode);

	if ((nErrorNo = Create(strReportFile)) == nREPORT_NOERROR)
	{
		nErrorNo = repman.DisplayReport(strReportFile, strParams);
	}

	repman.DisplayError(nErrorNo);
}

/**********************************************************************/

int CCSVFileReport::Create ( const char* szReportFile )
{
	CSSFile csvFile{};
	if (csvFile.Open(m_strFilename, "rb") == FALSE)
	{
		return nREPORT_NOSOURCE;
	}

	if (m_Report.Open(szReportFile) == FALSE)
	{
		return nREPORT_CREATEFAIL;
	}

// read csv file 

	CCSV csvIn{};
	if (csvFile.Read(csvIn) == FALSE)
	{
		return nREPORT_INVALIDSOURCE;
	}

	m_Report.SetStyle1 ( m_strReportTitle );

// scan header to create report header

	CUIntArray aiDps{};
	int nCardNoIndex = -1;
	int nFieldCount	= csvIn.GetSize();

	for (int i = 0; i < nFieldCount; i++)
	{
		CString strLabel = csvIn.GetString(i);

		int nDps = DecimalPlacesReqd(strLabel);
		aiDps.Add(nDps);

		int nAlignment = TA_RIGHT;
		int nWidth = 250;

		if (nDps == -1)
		{
			nAlignment = TA_LEFT;
			nWidth = 350;
		}
		m_Report.AddColumn(strLabel, nAlignment, nWidth);

		if (nCardNoIndex == -1)
		{
			if (strLabel == Account::UserID.Label)
			{
				nCardNoIndex = i;
			}
		}
	}

// add lines

	while ( csvFile.Read ( csvIn ) == TRUE )
	{
		CCSV csvOut ( '\t' );
		for ( int i = 0 ; i < nFieldCount ; i++ )
		{
			if (csvIn.IsEmpty(i) == TRUE)
			{
				csvOut.Add("");
			}
			else
			{
				CString strField = csvIn.GetString(i);
				if ( nCardNoIndex == i )					// see if CardNo column
				{
					CSQLRepositoryAccount repoAccount;
					if (repoAccount.RowExists(_atoi64(strField), NULL).GetSQLResult() == 0)
					{
						strField += " *";
					}
				}

				int nDps = aiDps.GetAt(i);
	
				if (nDps == -1)
				{
					csvOut.Add(strField);
				}
				else
				{
					double dValue = atof ( strField );
					csvOut.Add ( dValue, nDps );
				}
			}
		}
		m_Report.WriteLine ( csvOut );
	}
	m_Report.Close();

	return nREPORT_NOERROR;
}

/**********************************************************************/

int CCSVFileReport::DecimalPlacesReqd ( const char* szLabel )
{
	switch (LoyaltyHeaderHelpers.GetNumericField(szLabel))
	{
	case LOYALTYFIELD_POINTS:
	case LOYALTYFIELD_POINTSTODATE:
		return 0;

	case LOYALTYFIELD_PURSE1_BALANCE:
	case LOYALTYFIELD_PURSE1_LASTSPEND:
	case LOYALTYFIELD_PURSE1_SPENDTODATE:
	case LOYALTYFIELD_PURSE2_BALANCE:
	case LOYALTYFIELD_PURSE2_LASTSPEND:
	case LOYALTYFIELD_PURSE2_SPENDTODATE:
	case LOYALTYFIELD_CASH_LASTSPEND:
	case LOYALTYFIELD_CASH_SPENDTODATE:
	case LOYALTYFIELD_MAXSPEND:
	case LOYALTYFIELD_MAXOVERDRAFT:
		return System.GetDPValue();

	default:
		return -1;
	}
}

/**********************************************************************/
