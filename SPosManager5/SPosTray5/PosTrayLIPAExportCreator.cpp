/**********************************************************************/
#include "EPOSReportFileListMaker.h"
#include "EPOSReportPMSModes.h"
#include "EPOSSelectArray.h"
#include "FileRemover.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "PosTrayOptions.h"
#include "PriceHelpers.h"
#include "ReportHelpers.h"
#include "SessionCSVArray.h"
#include "TermFileListInfo.h"
/**********************************************************************/
#include "PosTrayLIPAExportCreator.h"
/**********************************************************************/

CLIPAExportBufferSale::CLIPAExportBufferSale()
{
	Reset();
}

/**********************************************************************/

void CLIPAExportBufferSale::Reset()
{
	m_nLocIdx = 0;
	m_nPriceLevel = 0;
	m_nPaymentType = 0;
	m_nPluNo = 0;
	m_nGLCode = 0;
	m_strCostCentre = "";
	m_strProject = "";
	m_bIsType5Line = FALSE;
	m_strTaxBand = "";
	m_dTaxRate = 0.0;
	m_nLineType = 0;
	m_dVal = 0.0;
}

/**********************************************************************/

void CLIPAExportBufferSale::Add ( CLIPAExportBufferSale& source )
{
	m_dVal += source.m_dVal;
}

/**********************************************************************/

int CLIPAExportBufferSale::Compare( CLIPAExportBufferSale& source, int nHint )
{
	if (m_nLocIdx != source.m_nLocIdx)
	{
		return (m_nLocIdx > source.m_nLocIdx ? 1 : -1);
	}

	if (m_nPriceLevel != source.m_nPriceLevel)
	{
		return (m_nPriceLevel > source.m_nPriceLevel ? 1 : -1);
	}

	if (m_nPaymentType != source.m_nPaymentType)
	{
		return (m_nPaymentType > source.m_nPaymentType ? 1 : -1);
	}

	if (m_nPluNo != source.m_nPluNo)
	{
		return (m_nPluNo > source.m_nPluNo ? 1 : -1);
	}

	if (m_nGLCode != source.m_nGLCode)
	{
		return (m_nGLCode > source.m_nGLCode ? 1 : -1);
	}

	if (m_strCostCentre != source.m_strCostCentre)
	{
		return (m_strCostCentre > source.m_strCostCentre ? 1 : -1);
	}

	if (m_strProject != source.m_strProject)
	{
		return (m_strProject > source.m_strProject ? 1 : -1);
	}

	//LJMU
	if (DealerFlags.Get_LIPA_LJMU_ExportType() == 2)
	{
		if (m_nLineType != source.m_nLineType)
		{
			return (m_nLineType > source.m_nLineType ? 1 : -1);
		}

		if (m_strTaxBand != source.m_strTaxBand)
		{
			return (m_strTaxBand > source.m_strTaxBand ? 1 : -1);
		}
		else
		{
			return 0;
		}
	}
	//LIPA
	else
	{
		if (m_bIsType5Line != source.m_bIsType5Line)
		{
			return m_bIsType5Line ? 1 : -1;
		}

		if (TRUE == m_bIsType5Line)
		{
			return 0;
		}

		if (m_strTaxBand != source.m_strTaxBand)
		{
			return (m_strTaxBand > source.m_strTaxBand ? 1 : -1);
		}

		if (m_dTaxRate != source.m_dTaxRate)
		{
			return (m_dTaxRate > source.m_dTaxRate ? 1 : -1);
		}

		if (m_nLineType != source.m_nLineType)
		{
			return (m_nLineType > source.m_nLineType ? 1 : -1);
		}
		else
		{
			return 0;
		}
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CLIPAExportBufferPayment::CLIPAExportBufferPayment()
{
	Reset();
}

/**********************************************************************/

void CLIPAExportBufferPayment::Reset()
{
	m_nPaymentType = 0;
	m_dVal = 0.0;
}

/**********************************************************************/

void CLIPAExportBufferPayment::Add ( CLIPAExportBufferPayment& source )
{
	m_dVal += source.m_dVal;
}

/**********************************************************************/

int CLIPAExportBufferPayment::Compare( CLIPAExportBufferPayment& source, int nHint )
{
	if (m_nPaymentType != source.m_nPaymentType)
	{
		return (m_nPaymentType > source.m_nPaymentType ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CLIPAExportBufferPluItem::CLIPAExportBufferPluItem()
{
	Reset();
}

/**********************************************************************/

void CLIPAExportBufferPluItem::Reset()
{
	m_nPluNo = 0;
	m_nGLCode = 0;
	m_strCostCentre = "";
	m_strProject = "";
}

/**********************************************************************/

void CLIPAExportBufferPluItem::Add ( CLIPAExportBufferPluItem& source )
{
}

/**********************************************************************/

int CLIPAExportBufferPluItem::Compare( CLIPAExportBufferPluItem& source, int nHint )
{
	if (m_nPluNo != source.m_nPluNo)
	{
		return (m_nPluNo > source.m_nPluNo ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPosTrayLIPAExportCreator::CPosTrayLIPAExportCreator( CPosTrayLIPAScheduleInfo& infoExport ) : m_infoExport( infoExport )
{
	m_nTransactionRAType = CASHRSP_ITEMPAYTYPE_NORMAL;
	m_strTranHeader = "";
	m_bGotTranError = FALSE;
	m_nTaxDateCode = 0;
	m_nTransactionAccountID = 0;
}

/**********************************************************************/

void CPosTrayLIPAExportCreator::LogError( const char* szError )
{
	CString strError = "";
	strError += m_strTranHeader;
	strError += ",";
	strError += szError;

	m_arrayErrors.Add( strError );
	m_bGotTranError = TRUE;
}

/**********************************************************************/

bool CPosTrayLIPAExportCreator::ProcessExport(COleDateTime& dateReport)
{
	CEposSelectArray SelectArray;
	SelectArray.SelectSystem();

	CEposReportPMSModes PMSModes;
	PMSModes.SetAllowMode(PMS_ALLOWMODE_NONE);

	int nSODHour = EcrmanOptions.GetReportsDayStartHour();
	int nSODMinute = EcrmanOptions.GetReportsDayStartHour();

	int nEODHour = nSODHour;
	int nEODMinute = nSODMinute - 1;

	if (nEODMinute < 0)
	{
		nEODMinute = 59;
		nEODHour--;

		if (nEODHour < 0)
		{
			nEODHour = 23;
		}
	}

	CSessionCSVRecord ReportSession;
	ReportSession.SetSessionStartHour(nSODHour);
	ReportSession.SetSessionStartMinute(nSODMinute);
	ReportSession.SetSessionEndHour(nEODHour);
	ReportSession.SetSessionEndMinute(nEODMinute);
	DataManagerNonDb.SessionDateTimeFilter.SetDates(dateReport, dateReport, ReportSession);

	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker(FALSE, FALSE, PMSModes);
	FileListMaker.GetFileList(arrayFileInfo, HISTORYFILE_TYPE_ITEMPAY, SelectArray, FALSE, nTotalFileSize);

	CString strReportDate;
	strReportDate.Format("%4.4d%2.2d%2.2d",
		dateReport.GetYear(),
		dateReport.GetMonth(),
		dateReport.GetDay());

	m_nTaxDateCode = DataManager.HistoricalTaxRates.GetDateCode(strReportDate);

	int nLastDbIdx = -1;
	for (int nFileIdx = 0; nFileIdx < arrayFileInfo.GetSize(); nFileIdx++)
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt(nFileIdx, infoFile);

		int nNetWkLocNo = PosTrayOptions.GetLIPAExportLocation();
		if (nNetWkLocNo > 0)
		{
			if (dbLocation.GetNetworkLocNo(infoFile.m_nLocIdx) != nNetWkLocNo)
			{
				continue;
			}
		}

		if (infoFile.m_nDbIdx != nLastDbIdx)
		{
			CDataManagerInfo info;
			DataManager.OpenDatabaseReadOnly(infoFile.m_nDbIdx, info, FALSE);
			nLastDbIdx = infoFile.m_nDbIdx;
			m_PluInfoTable.RemoveAll();
		}

		if (ProcessEPOSFile(infoFile) == FALSE)
		{
			return FALSE;
		}
	}

	CString strLIPAFolder = PosTrayOptions.GetLIPAExportFolder();
	::TrimSpaces(strLIPAFolder, FALSE);

	if (strLIPAFolder == "")
	{
		strLIPAFolder = "LIPA";
		GetSyssetProgramPath(strLIPAFolder);
		::CreateSubdirectory(strLIPAFolder);
		strLIPAFolder += "\\EXPORT";
	}

	::CreateSubdirectory(strLIPAFolder);

	CString strExportFile;
	strExportFile.Format("%s\\%s.txt",
		(const char*)strLIPAFolder,
		(const char*)strReportDate);

	CString strErrorFile;
	strErrorFile.Format("%s\\%s_XXX.txt",
		(const char*)strLIPAFolder,
		(const char*)strReportDate);

	CString strExportDate;
	strExportDate.Format("%2.2d/%2.2d/%4.4d",
		dateReport.GetDay(),
		dateReport.GetMonth(),
		dateReport.GetYear());

	bool bResult = FALSE;
	CSSFile fileExport;
	if (fileExport.Open(strExportFile, "wb") == TRUE)
	{
		for (int n = 0; n < m_ConsolArray.GetSize(); n++)
		{
			CLIPAExportBufferSale infoSale;
			m_ConsolArray.GetAt(n, infoSale);

			CCSV csv('|');
			csv.Add(dbLocation.GetAgressoLocationGroup(infoSale.m_nLocIdx));
			csv.Add(dbLocation.GetName(infoSale.m_nLocIdx));
			csv.Add(DataManager.PriceText.GetReportText(infoSale.m_nPriceLevel));

			switch (infoSale.m_nPaymentType)
			{
			case 0:
				csv.Add("Cash");
				break;

			case 1:
				csv.Add("Cheque");
				break;

			case 2:
			case 3:
			case 9:
			case 15:
				csv.Add("Card");
				break;

			case 4:
			case 5:
				csv.Add("Room");
				break;

			case 6:
				csv.Add("Loyalty");
				break;

			case 7:
				csv.Add("Currency");
				break;

			case 8:
				csv.Add("Deposit");
				break;

			case 10:
				csv.Add("Customer");
				break;

			case 11:
				csv.Add("Credit Note");
				break;

			case 12:
				csv.Add("Gift Card");
				break;

			case 13:
				csv.Add("External");
				break;

			default:
				csv.Add(infoSale.m_nPaymentType);
				break;
			}

			csv.Add(strExportDate);
			csv.Add(infoSale.m_nLineType);

			if (DealerFlags.Get_LIPA_LJMU_ExportType() == 1)
			{
				csv.Add(infoSale.m_strTaxBand);
				csv.Add(infoSale.m_dTaxRate, 2);
			}

			csv.Add(m_nTransactionAccountID);

			if (infoSale.m_nGLCode != 0)
			{
				csv.Add(infoSale.m_nGLCode);
			}
			else
			{
				csv.Add("");
			}

			csv.Add(infoSale.m_strCostCentre);
			csv.Add(infoSale.m_strProject);
			csv.Add(infoSale.m_dVal, 2);
			fileExport.WriteLine(csv.GetLine());
		}

		bResult = TRUE;
	}

	if (TRUE == bResult)
	{
		if (m_arrayErrors.GetSize() != 0)
		{
			CSSFile fileError;
			if (fileError.Open(strErrorFile, "wb") == TRUE)
			{
				CString strDate = strExportDate + ",";
				for (int n = 0; n < m_arrayErrors.GetSize(); n++)
				{
					fileError.WriteLine(strDate + m_arrayErrors.GetAt(n));
				}
			}
			else
			{
				bResult = FALSE;
			}
		}
		else
		{
			CFileRemover FileRemover(strErrorFile);
		}
	}

	return bResult;
}

/**********************************************************************/

bool CPosTrayLIPAExportCreator::ProcessEPOSFile( CTermFileListInfo& infoFile )
{
	CSSFile fileSales;
	if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
		return FALSE;

	ResetTransaction();
	bool bInclude = FALSE;

	CString strBuffer = "";
	while( fileSales.ReadString( strBuffer ) == TRUE )
	{
		CCSV csvIn( strBuffer );

		switch( csvIn.GetInt(0) )
		{
		case 0:	//HEADER
			
			if (TRUE == bInclude)
			{
				ProcessTransaction(infoFile.m_nLocIdx);
			}
			
			ResetTransaction();
			bInclude = FALSE;
					
			if ( csvIn.GetInt(1) == 1 )
			{
				CString strCheckDate = infoFile.m_strDateTran;
				CString strCheckTime = csvIn.GetString(2);

				if ( DataManagerNonDb.SessionDateTimeFilter.CheckTime( strCheckDate, strCheckTime  ) != 0 )	
				{
					bInclude = TRUE;

					COleDateTime oleDate = COleDateTime (
					atoi(strCheckDate.Left(4)),
					atoi(strCheckDate.Mid(4,2)),
					atoi(strCheckDate.Right(2)),
					0, 0, 0 );

					m_strTranHeader = strBuffer;
					m_CashRSPVersionChecker.ForceSetVersionInfo( csvIn.GetInt(3), csvIn.GetInt(4) );
				}
			}
			break;
		
		case 1: //ITEM
			if ( TRUE == bInclude )
			{
				__int64 nPluNo = -1;
				int nPriceLevel = 0;
				CString strTaxBand = "";
				double dVal = 0.0;
					
				switch( csvIn.GetInt(1) )
				{
				case CSVPLUDATA_PLU_SALE:
					if ( csvIn.GetInt(2) == 1 )
					{
						nPluNo = csvIn.GetInt64(3);
						dVal = csvIn.GetDouble(6);
						nPriceLevel = csvIn.GetInt(7);
						strTaxBand = csvIn.GetString(8);
					}
					break;

				case CSVPLUDATA_PLU_DISCOUNT:
				case CSVPLUDATA_PLU_MIXMATCH:
					m_bufferDiscounts.Add( strBuffer );
					break;

				case CSVPLUDATA_PROMO_TAXABLE:
				case CSVPLUDATA_PROMO_NONTAXABLE:
				case CSVPLUDATA_MIXMATCH_NONTAXABLE:
					LogError( "Discount assigned to separate PLU" );
					break;
				}
				
				if ( nPluNo != -1 )
				{
					CSQLPluNoInfo infoPluNo;
					infoPluNo.m_nEposPluNo = nPluNo;
					::ProcessPluNo( infoPluNo, FALSE, TRUE );

					CLIPAExportBufferPluItem PluItem;
						
					if ( TRUE == infoPluNo.m_bValid )
					{
						PluItem.m_nPluNo = infoPluNo.m_nBasePluNo;

						int nPos;
						if ( m_PluInfoTable.Find( PluItem, nPos ) == TRUE )
						{
							m_PluInfoTable.GetAt( nPos, PluItem );
						}
						else 
						{
							if ( DataManager.Plu.FindPluByNumber( infoPluNo.m_nBasePluNo, nPos ) == TRUE )
							{
								CPluCSVRecord PluRecord;
								DataManager.Plu.GetAt( nPos, PluRecord );
								PluItem.m_nGLCode = PluRecord.GetAgressoGLCode();
								PluItem.m_strCostCentre = PluRecord.GetAgressoCostCentre();
								PluItem.m_strProject = PluRecord.GetAgressoProject();
							}

							m_PluInfoTable.Consolidate( PluItem );
						}
					
						CLIPAExportBufferSale infoItem;
						infoItem.m_nPluNo = PluItem.m_nPluNo;
						infoItem.m_nGLCode = PluItem.m_nGLCode;
						infoItem.m_strCostCentre = PluItem.m_strCostCentre;
						infoItem.m_strProject = PluItem.m_strProject;
						infoItem.m_nPriceLevel = nPriceLevel;
						infoItem.m_strTaxBand = strTaxBand;
						infoItem.m_dVal = dVal;
						m_TransactionBufferSale.Consolidate( infoItem );
					}
					else
					{
						LogError( "Invalid PLU Number" );
					}
				}
			}
			break;

		case 2: //PAYMENT 
			if ( TRUE == bInclude )
			{
				CCSVEposTermLine csvPayment ( strBuffer );

				int nPaymentNo = csvPayment.GetPaymentNumber();

				double dAmount = ( ( double ) csvPayment.GetPaymentAmount() ) / 100.0;
						
				int nType, nDummy32;
				__int64 nDummy64;
				bool bIsCash;
				csvPayment.GetPaymentAccountTypeAndID( m_CashRSPVersionChecker, nType, nDummy32, nDummy64, bIsCash );
							
				CLIPAExportBufferPayment info;
				info.m_nPaymentType = csvPayment.GetPaymentType();
				info.m_dVal = dAmount;
				m_TransactionBufferPayment.Consolidate( info );

				if (csvPayment.GetPaymentCashback() != 0)
				{
					LogError("Transaction includes cashback");
				}

				if (m_nTransactionAccountID == 0)
				{
					m_nTransactionAccountID = nDummy32;
				}
			}
			break;

		case 3:
			if ( TRUE == bInclude )
			{
				if ( ( csvIn.GetInt(1) == 1 ) && ( CASHRSP_ITEMPAYTYPE_NORMAL == m_nTransactionRAType ) )
				{
					int nNewType = csvIn.GetInt(2);
					switch( nNewType )
					{
					case CASHRSP_ITEMPAYTYPE_DEPOSIT:	
					case CASHRSP_ITEMPAYTYPE_CUSTOMER:		
					case CASHRSP_ITEMPAYTYPE_ROOM:			
					case CASHRSP_ITEMPAYTYPE_LOYALTY:		
					case CASHRSP_ITEMPAYTYPE_SMARTPAY:		
					case CASHRSP_ITEMPAYTYPE_SMARTPHONE:	
					case CASHRSP_ITEMPAYTYPE_SPTBOOK:	
						m_nTransactionRAType = nNewType;
						break;
					}
				}
			}
			break;
		}
	}

	fileSales.Close();

	if (TRUE == bInclude)
	{
		ProcessTransaction(infoFile.m_nLocIdx);
	}
	
	ResetTransaction();

	return TRUE;
}

/**********************************************************************/

void CPosTrayLIPAExportCreator::ResetTransaction()
{
	m_TransactionBufferPayment.RemoveAll();
	m_TransactionBufferSale.RemoveAll();
	m_nTransactionRAType = CASHRSP_ITEMPAYTYPE_NORMAL;
	m_bufferDiscounts.RemoveAll();
	m_strTranHeader = "";
	m_bGotTranError = FALSE;
	m_nTransactionAccountID = 0;
}

/**********************************************************************/

void CPosTrayLIPAExportCreator::ProcessTransaction( int nLocIdx )
{
	if (TRUE == m_bGotTranError)
	{
		return;
	}

	if ( m_TransactionBufferPayment.GetSize() == 0 )
	{
		LogError( "Transaction contains no payments" );
		return;
	}

	//ASSIGN DISCOUNTS TO PLU ITEMS
	for ( int n = 0; n < m_bufferDiscounts.GetSize(); n++ )
	{
		CCSV csvDiscount( m_bufferDiscounts.GetAt(n) );

		if ( csvDiscount.GetInt(2) == 1 )
		{
			CSQLPluNoInfo infoPluNo;
			infoPluNo.m_nEposPluNo = csvDiscount.GetInt64(3);
			::ProcessPluNo( infoPluNo, FALSE, TRUE );
			
			if ( TRUE == infoPluNo.m_bValid )
			{
				bool bFound = FALSE;
				for ( int x = 0; ( x < m_TransactionBufferSale.GetSize() ) && ( FALSE == bFound ); x++ )
				{
					CLIPAExportBufferSale infoItem;
					m_TransactionBufferSale.GetAt( x, infoItem );

					if ( infoItem.m_nPluNo == infoPluNo.m_nBasePluNo )
					{
						infoItem.m_dVal -= csvDiscount.GetDouble(6);
						m_TransactionBufferSale.SetAt( x, infoItem );
						bFound = TRUE;
					}
				}

				if ( FALSE == bFound )
				{
					LogError( "No matching PLU item for discount" );
					return;
				}
			}
		}					
	}
	
	if ( CASHRSP_ITEMPAYTYPE_NORMAL != m_nTransactionRAType )
	{
		CString strRAType = "";
		switch( m_nTransactionRAType )
		{
		case CASHRSP_ITEMPAYTYPE_DEPOSIT:		strRAType = "Deposit Purchase";		break;
		case CASHRSP_ITEMPAYTYPE_CUSTOMER:		strRAType = "Customer RA";			break;
		case CASHRSP_ITEMPAYTYPE_ROOM:			strRAType = "Room Settlement";		break;
		case CASHRSP_ITEMPAYTYPE_LOYALTY:		strRAType = "Loyalty Topup";		break;
		case CASHRSP_ITEMPAYTYPE_SMARTPAY:		strRAType = "SmartPay Topup";		break;
		case CASHRSP_ITEMPAYTYPE_SMARTPHONE:	strRAType = "SmartPhone Topup";		break;
		case CASHRSP_ITEMPAYTYPE_SPTBOOK:		strRAType = "SportsBooker RA";		break;
		default:								strRAType = "Payment on Account";	break; 
		}

		CString strError = "";
		strError += "Transaction contains ";
		strError += strRAType;
		LogError( strError );
		return;
	}
	
	//CALCULATE ITEM TOTAL
	double dItemTotal = 0.0;
	for ( int n = 0; n < m_TransactionBufferSale.GetSize(); n++ )
	{
		CLIPAExportBufferSale item;
		m_TransactionBufferSale.GetAt( n, item );
		dItemTotal += item.m_dVal;
	}

	//CALCULATE PAYMENT TOTAL
	double dPaymentTotal = 0.0;
	for ( int n = 0; n < m_TransactionBufferPayment.GetSize(); n++ )
	{
		CLIPAExportBufferPayment item;
		m_TransactionBufferPayment.GetAt( n, item );
		dPaymentTotal += item.m_dVal;
	}

	if ( CPriceHelpers::CompareDoubles( dItemTotal, dPaymentTotal, 3 ) != 0 )
	{
		LogError( "Payment total does not match item total" );
		return;
	}
	
	//IF ITEM TOTAL IS ZERO THEN HANDLE AS UNASSIGNED PAYMENT
	if ( CPriceHelpers::CompareDoubles( dItemTotal, 0.0, 3 ) == 0 )
	{
		LogError( "Item total value is zero" );
		return;
	}

	CReportConsolidationArray<CLIPAExportBufferSale> arrayAssignedPayments;

	//ASSIGN PAYMENTS TO GROUPS BY RATIO TO GROUP TOTAL
	for ( int p = 0; p < m_TransactionBufferPayment.GetSize(); p++ )
	{
		CLIPAExportBufferPayment infoPayment;
		m_TransactionBufferPayment.GetAt( p, infoPayment );

		for ( int g = 0; g < m_TransactionBufferSale.GetSize(); g++ )
		{
			CLIPAExportBufferSale infoSale;
			m_TransactionBufferSale.GetAt( g, infoSale );
				
			if (1 == m_TransactionBufferSale.GetSize())
			{
				infoSale.m_dVal = infoPayment.m_dVal;
			}
			else	
			{
				infoSale.m_dVal = ( infoPayment.m_dVal * infoSale.m_dVal ) / dItemTotal;
			}

			infoSale.m_nPaymentType = infoPayment.m_nPaymentType;
			arrayAssignedPayments.Consolidate( infoSale );
		}
	}

	//COPY RESULTS TO MAIN ARRAY LJMU
	if (DealerFlags.Get_LIPA_LJMU_ExportType() == 2)
	{
		for (int n = 0; n < arrayAssignedPayments.GetSize(); n++)
		{
			CLIPAExportBufferSale infoSale;
			arrayAssignedPayments.GetAt(n, infoSale);

			int nTaxBand = CTaxArray::GetNumericTaxBand(infoSale.m_strTaxBand, TRUE);

			infoSale.m_nLocIdx = nLocIdx;
			infoSale.m_nPluNo = 0;
			infoSale.m_strTaxBand = "";

			double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate(m_nTaxDateCode, nTaxBand);
			if (CPriceHelpers::CompareDoubles(dTaxRate, 0.0, 3) == 0)
			{
				infoSale.m_nLineType = 1;
				infoSale.m_dVal *= -1;
				m_ConsolArray.Consolidate(infoSale);
				infoSale.m_nLineType = 5;
				infoSale.m_dVal *= -1;
				m_ConsolArray.Consolidate(infoSale);
			}
			else
			{
				CReportHelpers ReportHelpers;
				double dGross = infoSale.m_dVal;
				double dNet = ReportHelpers.CalcNonTax(dGross, dTaxRate);
				double dTaxAmount = dGross - dNet;

				infoSale.m_nLineType = 1;
				infoSale.m_dVal = dNet * -1;
				m_ConsolArray.Consolidate(infoSale);
				infoSale.m_nLineType = 2;
				infoSale.m_dVal = dTaxAmount * -1;
				m_ConsolArray.Consolidate(infoSale);
				infoSale.m_nLineType = 5;
				infoSale.m_dVal = dGross;
				m_ConsolArray.Consolidate(infoSale);
			}
		}

	}
	//COPY RESULTS TO MAIN ARRAY LIPA
	else
	{
		for (int n = 0; n < arrayAssignedPayments.GetSize(); n++)
		{
			CLIPAExportBufferSale infoSale;
			arrayAssignedPayments.GetAt(n, infoSale);

			int nTaxBand = CTaxArray::GetNumericTaxBand(infoSale.m_strTaxBand, TRUE);

			infoSale.m_nLocIdx = nLocIdx;
			infoSale.m_nPluNo = 0;
			infoSale.m_dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate(m_nTaxDateCode, nTaxBand);

			if (CPriceHelpers::CompareDoubles(infoSale.m_dTaxRate, 0.0, 3) == 0)
			{
				infoSale.m_bIsType5Line = FALSE;
				infoSale.m_nLineType = 1;
				infoSale.m_dVal *= -1;
				m_ConsolArray.Consolidate(infoSale);

				infoSale.m_bIsType5Line = TRUE;
				infoSale.m_nLineType = 5;
				infoSale.m_dVal *= -1;
				infoSale.m_strTaxBand = "";
				infoSale.m_dTaxRate = 0.0;
				m_ConsolArray.Consolidate(infoSale);
			}
			else
			{
				CReportHelpers ReportHelpers;
				double dGross = infoSale.m_dVal;
				double dNet = ReportHelpers.CalcNonTax(dGross, infoSale.m_dTaxRate);
				double dTaxAmount = dGross - dNet;

				infoSale.m_bIsType5Line = FALSE;
				infoSale.m_nLineType = 1;
				infoSale.m_dVal = dNet * -1;
				m_ConsolArray.Consolidate(infoSale);

				infoSale.m_nLineType = 2;
				infoSale.m_dVal = dTaxAmount * -1;
				m_ConsolArray.Consolidate(infoSale);

				infoSale.m_bIsType5Line = TRUE;
				infoSale.m_nLineType = 5;
				infoSale.m_dVal = dGross;
				infoSale.m_strTaxBand = "";
				infoSale.m_dTaxRate = 0.0;
				m_ConsolArray.Consolidate(infoSale);
			}
		}
	}
}

/**********************************************************************/