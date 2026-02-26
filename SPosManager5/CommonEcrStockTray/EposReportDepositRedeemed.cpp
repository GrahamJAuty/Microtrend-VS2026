/**********************************************************************/
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "NodeTypes.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "EposReportDepositRedeemed.h"
/**********************************************************************/

CEposReportConsolDepositRedeemed::CEposReportConsolDepositRedeemed()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolDepositRedeemed::Reset()
{
	m_nLocSort = 0;
	m_strDate = "";
	m_strTime = "";
	m_nDepositID = 0;
	m_dTender = 0.0;
	m_dAmount = 0.0;
}

/**********************************************************************/

void CEposReportConsolDepositRedeemed::Add ( CEposReportConsolDepositRedeemed& source )
{
	m_dTender += source.m_dTender;
	m_dAmount += source.m_dAmount;
}

/**********************************************************************/

int CEposReportConsolDepositRedeemed::Compare(CEposReportConsolDepositRedeemed& source, int nHint)
{
	if (m_nLocSort != source.m_nLocSort)
	{
		return (m_nLocSort > source.m_nLocSort ? 1 : -1);
	}

	if (m_strDate != source.m_strDate)
	{
		return (m_strDate > source.m_strDate ? 1 : -1);
	}

	if (m_strTime != source.m_strTime)
	{
		return (m_strTime > source.m_strTime ? 1 : -1);
	}

	if (m_nDepositID != source.m_nDepositID)
	{
		return (m_nDepositID > source.m_nDepositID ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportDepositRedeemed::CEposReportDepositRedeemed( int nSpecialItemType, CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nSpecialItemType = nSpecialItemType;

	switch( m_nSpecialItemType )
	{
	case CASHRSP_ITEMPAYTYPE_CUSTOMER:
		m_nReportType = REPORT_TYPE_CUSTOMER_SPEND;
		break;

	case CASHRSP_ITEMPAYTYPE_DEPOSIT:
	default:
		m_nReportType = REPORT_TYPE_DEPOSIT_REDEEMED;
		break;
	}

	SetPMSAllowMode( PMS_ALLOWMODE_NONE );
}

/**********************************************************************/

void CEposReportDepositRedeemed::WriteTotal( double dTotalTender, double dTotalAmount, int nCustomerColumns )
{
	CString strUnderline = "";
	if ( nCustomerColumns >= 1 ) strUnderline += "\t";
	if ( nCustomerColumns >= 2 ) strUnderline += "\t";
	strUnderline += "\t\t\t<LI>\t<LI>";
	m_ReportFile.WriteReportMiscLine( strUnderline );

	CCSV csv( '\t' );
	csv.Add( "" );
	csv.Add( "" );
	if ( nCustomerColumns >= 1 ) csv.Add( "" );
	if ( nCustomerColumns >= 2 ) csv.Add( "" );
	csv.Add( "Total" );
	csv.Add( dTotalTender, 2 );
	csv.Add( dTotalAmount, 2 );
	m_ReportFile.WriteReportMiscLine( csv.GetLine() );
}

/**********************************************************************/

void CEposReportDepositRedeemed::Consolidate()
{
	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
	FileListMaker.GetFileList ( arrayFileInfo, HISTORYFILE_TYPE_ACCOUNT, m_SelectArray, FALSE, nTotalFileSize );

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Consolidating report data" );

	for ( int nFileIdx = 0; nFileIdx < arrayFileInfo.GetSize(); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt( nFileIdx, infoFile );

		CSSFile fileSales;
		if (fileSales.Open(infoFile.m_strFilename, "rb", _SH_DENYNO) == FALSE)
		{
			continue;
		}

		COleDateTime timeTransaction = COleDateTime( 2011, 1, 1, 0, 0, 0 );
		COleDateTime dateTransaction = COleDateTime( 2011, 1, 1, 0, 0, 0 );
		CString strDate = "";
		CString strTime = "";
		bool bValidTime = FALSE;

		CString strBuffer;
		while( fileSales.ReadString( strBuffer ) == TRUE )
		{
			StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );

			CCSVAccountHistory csvIn ( strBuffer );

			switch( csvIn.GetAccountLineType() )
			{
			case ACCOUNT_LINETYPE_HEADER:
				strDate = infoFile.m_strDateTran;
				strTime = csvIn.GetHeaderLineTime();
				bValidTime = ValidateAccountDate( infoFile.m_nLocIdx, strDate, strTime, timeTransaction, dateTransaction );
				break;

			case ACCOUNT_LINETYPE_CHARGE:
				if ((TRUE == bValidTime) && (csvIn.GetChargeType() == m_nSpecialItemType))
				{
					CEposReportConsolDepositRedeemed info;
					info.m_nLocSort = dbLocation.GetSortIdxFromLocIdx(infoFile.m_nLocIdx);
					info.m_strDate = strDate;
					info.m_strTime = strTime;
					info.m_nDepositID = csvIn.GetChargeID();
					info.m_dTender = csvIn.GetChargeTender();
					info.m_dAmount = csvIn.GetChargeAmount();

					if ( (DEPOSIT_ADJUST_ID == info.m_nDepositID) && ( REPORT_TYPE_DEPOSIT_REDEEMED == m_nReportType ) )
					{
						info.m_strDate = "99999999";
						info.m_strTime = "999999";
					}

					m_ConsolArray.Consolidate( info );
		
					m_SelectArray.ProcessStartDate ( infoFile.m_nSelectArrayIdx, dateTransaction );
					m_SelectArray.ProcessEndDate( infoFile.m_nSelectArrayIdx, dateTransaction );
				}
				break;
			}
		}

		nProgress += fileSales.GetLength();
		fileSales.Close();
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

bool CEposReportDepositRedeemed::CreateReport()
{
	if (m_ReportFile.Open(Super.ReportFilename()) == FALSE)
	{
		return FALSE;
	}

	int nCustomerColumns = 1;
	if (CASHRSP_ITEMPAYTYPE_CUSTOMER == m_nSpecialItemType)
	{
		if (EcrmanOptions.GetFeaturesCustomerBalanceFlag() == TRUE)
		{
			nCustomerColumns = 2;
		}
		else
		{
			nCustomerColumns = 0;
		}
	}

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "Date", TA_LEFT, 250 );
	m_ReportFile.AddColumn ( "Time", TA_LEFT, 200 );

	switch( nCustomerColumns )
	{
	case 0:
		m_ReportFile.AddColumn ( "Customer ID", TA_LEFT, 250 );
		break;

	case 2:
		m_ReportFile.AddColumn ( "Customer ID", TA_LEFT, 250 );
		m_ReportFile.AddColumn ( "Customer Type", TA_LEFT, 300 );
		m_ReportFile.AddColumn ( "Customer Name", TA_LEFT, 500 );
		break;

	case 1:
	default:
		m_ReportFile.AddColumn ( "Deposit ID", TA_LEFT, 200 );
		m_ReportFile.AddColumn ( "Purchaser Name", TA_LEFT, 500 );
		break;
	}
	
	m_ReportFile.AddColumn ( "Tender", TA_RIGHT, 200 );
	m_ReportFile.AddColumn ( "Amount", TA_RIGHT, 200 );
	
	int nDepositPos = 0;
	int nNextBlockPos = 0;

	int nSize = m_ConsolArray.GetSize();
	StatusProgress.Lock( TRUE, "Creating report" );
	
	while ( nDepositPos < m_ConsolArray.GetSize() )
	{
		CEposReportConsolDepositRedeemed infoThisBlock;
		m_ConsolArray.GetAt( nDepositPos, infoThisBlock );

		CEposReportConsolDepositRedeemed infoNextBlock;
		infoNextBlock.m_nLocSort = infoThisBlock.m_nLocSort + 1;
		infoNextBlock.m_strDate = "";
		m_ConsolArray.Find( infoNextBlock, nNextBlockPos );

		CString strLocation;
		int nLocIdx = dbLocation.GetLocIdxFromSortIdx( infoThisBlock.m_nLocSort );
		strLocation.Format( "<..>%s", dbLocation.GetName( nLocIdx ) );

		if (0 != nDepositPos)
		{
			m_ReportFile.RequestBlankLines(1);
		}

		m_ReportFile.WriteReportMiscLine( strLocation );
		m_ReportFile.WriteReportMiscLine( "<LI>" );

		double dTotalTender = 0.0;
		double dTotalAmount = 0.0;

		CDataManagerInfo infoDM;
		DataManagerNonDb.OpenDepositNames( DB_READONLY, nLocIdx, infoDM );

		for ( int nPos = nDepositPos; nPos < nNextBlockPos; nPos++ )
		{
			CEposReportConsolDepositRedeemed infoLine;
			m_ConsolArray.GetAt( nPos, infoLine );
			
			dTotalTender += infoLine.m_dTender;
			dTotalAmount += infoLine.m_dAmount;

			CString strDate;
			strDate.Format( "%s/%s/%s",
				(const char*) infoLine.m_strDate.Right(2),
				(const char*) infoLine.m_strDate.Mid(4,2),
				(const char*) infoLine.m_strDate.Left(4) );

			CString strTime;
			strTime.Format( "%s:%s:%s",
				(const char*) infoLine.m_strTime.Left(2),
				(const char*) infoLine.m_strTime.Mid(2,2),
				(const char*) infoLine.m_strTime.Right(2) );

			CString strID;
			strID.Format( "%8.8d", infoLine.m_nDepositID );

			CCSV csv( '\t' );
			
			bool bRecycledDeposits = FALSE;
			if ((DEPOSIT_ADJUST_ID == infoLine.m_nDepositID) && (infoLine.m_strDate == "99999999") && (REPORT_TYPE_DEPOSIT_REDEEMED == m_nReportType))
			{
				bRecycledDeposits = TRUE;
				csv.Add("");
				csv.Add("");
				csv.Add("<..>Recycled Deposits");
				csv.Add("");
			}
			else
			{
				csv.Add(strDate);
				csv.Add(strTime);
				csv.Add(strID);

				switch (nCustomerColumns)
				{
				case 0:
					break;

				case 2:
				{
					CCustomerSelectionListAccountInfo infoSelect;
					infoSelect.m_nLocIdx = nLocIdx;
					infoSelect.m_nCustomerID = infoLine.m_nDepositID;

					int nNodeType, nDummy;
					DataManagerNonDb.CustomerNameTable.GetCustomerStatus(infoSelect, nNodeType, nDummy);

					switch (nNodeType)
					{
					case NODE_CUSTOMER_ACCOUNT:
						csv.Add("Account");
						break;

					case NODE_CUSTOMER_ONEOFF:
						csv.Add("One-off");
						break;

					case NODE_PLACEHOLDER:
					default:
						csv.Add("Unknown");
						break;
					}

					csv.Add(infoSelect.GetReportName());
				}
				break;

				case 1:
				default:
					csv.Add(DataManagerNonDb.DepositNames.GetBuyerNameByDepositID(infoLine.m_nDepositID));
					break;
				}
			}
			
			csv.Add( infoLine.m_dTender, 2 );
			csv.Add( infoLine.m_dAmount, 2 );
			
			if ( TRUE == bRecycledDeposits)
			{
				m_ReportFile.WriteReportMiscLine("\t\t\t\t<LI>\t<LI>");
			}
			
			m_ReportFile.WriteReportDataLine( csv.GetLine() );
		}

		WriteTotal( dTotalTender, dTotalAmount, nCustomerColumns );
		m_ReportFile.RequestBlankLines( 1 );

		DataManagerNonDb.CloseDepositNames( infoDM );

		nDepositPos = nNextBlockPos;
	}

	StatusProgress.Unlock();
				
	ShowEposDetails();
	m_ReportFile.Close();

	return TRUE;
}

/**********************************************************************/
