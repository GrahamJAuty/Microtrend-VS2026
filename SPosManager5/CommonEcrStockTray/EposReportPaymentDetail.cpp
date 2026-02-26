/**********************************************************************/
#include "EposReportConsolParams.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "PMSOptions.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportPaymentDetail.h"
/**********************************************************************/

CEposReportPaymentDetail::CEposReportPaymentDetail( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_PAYMENT_DETAIL;
	m_ReportSettings.SetShowTableNoFlag(FALSE);
	m_ReportSettings.SetShowTableNameFlag(FALSE);
	m_ReportSettings.SetShowSEQNoFlag(FALSE);
	m_ReportSettings.SetShowServerFlag(FALSE);
}

/**********************************************************************/

CEposReportPaymentDetail::CEposReportPaymentDetail(CEposSelectArray& SelectArray, const char* szCustomSettings) : CEposReport(SelectArray)
{
	m_nReportType = REPORT_TYPE_PAYMENT_DETAIL;

	CString strParams = szCustomSettings;
	m_ReportSettings.SetSettingsCSVLine(strParams);
}

/**********************************************************************/

CEposReportPaymentDetail::~CEposReportPaymentDetail()
{
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		m_arrayReportBlocks[n].m_pDetail -> RemoveAll();
		delete ( m_arrayReportBlocks[n].m_pDetail );
	}
}

/**********************************************************************/

void CEposReportPaymentDetail::InitialiseConsolidationBlocks()
{
	bool bAllowGlobal = ( ( NODE_SYSTEM == m_nConLevel ) && ( EcrmanOptions.GetGlobalPaymentFlag() == TRUE ) );

	CString strParams = "";
	switch( m_nPreferConsolType )
	{
	case EPOSREPORT_CONSOL_SAVED:
		strParams = m_EposReportSelectInfo.GetConsolPrefsSaved();
		break;

	case EPOSREPORT_CONSOL_ADHOC:
		strParams = m_EposReportSelectInfo.GetConsolPrefsAdhoc();
		break;
		
	case EPOSREPORT_CONSOL_DEFAULT:
		strParams = m_EposReportSelectInfo.GetConsolPrefsDefault();
		break;
		
	case EPOSREPORT_CONSOL_SYSTEM:
	default:
		strParams = m_EposReportSelectInfo.GetConsolPrefsSummary();
		break;
	}

	CEposReportConsolParamsStandard Params;
	Params.LoadSettings( strParams );

	if ( ( Params.m_bConsolSys | Params.m_bConsolDb | Params.m_bConsolLoc | Params.m_bConsolTerm ) == FALSE )
	{
		strParams = m_EposReportSelectInfo.GetConsolPrefsDefault();
		Params.LoadSettings( strParams );
	}

	if ( TRUE == Params.m_bConsolSys )
	{
		if ( ( NODE_SYSTEM != m_nConLevel ) || ( FALSE == bAllowGlobal ) )
		{
			Params.m_bConsolSys = FALSE;
			Params.m_bConsolDb = TRUE;
		}
	}
		
	m_SelectArray.MakeList();
	m_BlockMap.SetConsolidateByTerminalFlag( Params.m_bConsolTerm );
	m_BlockMap.SetConsolidateByLocationFlag( Params.m_bConsolLoc );
	m_BlockMap.SetConsolidateByDatabaseFlag( Params.m_bConsolDb );
	m_BlockMap.SetConsolidateBySystemFlag( Params.m_bConsolSys );
	m_BlockMap.BuildMap( m_SelectArray );
	
	for ( int n = 0; n < m_BlockMap.GetBlockMapEntityCount(); n++ )
	{
		CEposReportBlockMapEntity entity;
		m_BlockMap.GetBlockMapEntity( n, entity );

		CEposReportPaymentDetailBlock block;
		block.Copy( entity );
		block.m_pDetail = new CReportConsolidationArray<CEposReportConsolPaymentDetail>;
		
		m_arrayReportBlocks.Add( block );
	}
}

/**********************************************************************/

void CEposReportPaymentDetail::Consolidate()
{
	ConsolidateInternal();

	CDataManagerInfo infoDM;
	DataManagerNonDb.CloseTableNames(infoDM);
}

/**********************************************************************/

void CEposReportPaymentDetail::ConsolidateInternal()
{
	InitialiseConsolidationBlocks();

	int nTotalFileSize;

	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker(m_bEODMode, m_bSaleTime, m_PMSModes);
	FileListMaker.GetTermFileList(arrayFileInfo, m_SelectArray, TRUE, nTotalFileSize);

	int nProgress = 0;
	StatusProgress.Lock(TRUE, "Consolidating report data");

	bool bInclude = FALSE;
	COleDateTime oleTranDate;
	CString strTranDate = "";
	CString strTranTime = "";

	CReportConsolidationArray<CSortedIntItem> arrayPaymentNo;
	if (m_ReportSettings.GetPayNumFilterFlag() == TRUE)
	{
		CCSV csv(m_ReportSettings.GetPayNumFilter());

		for (int n = 0; n < csv.GetSize(); n++)
		{
			int nPaymentNo = csv.GetInt(n);
			if ((nPaymentNo >= 1) && (nPaymentNo <= 999))
			{
				CSortedIntItem item;
				item.m_nItem = nPaymentNo;
				arrayPaymentNo.Consolidate(item);
			}
		}
	}

	int nCurrentTableLocIdx = -1;

	for (int nFileIdx = 0; (nFileIdx < arrayFileInfo.GetSize()) && (m_CashRSPVersionChecker.GetCashRSPFormatFailFlag() == FALSE); nFileIdx++)
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt(nFileIdx, infoFile);

		m_BlockMap.GetBlockListForTerminal(infoFile.m_nSelectArrayIdx, m_arrayTerminalBlocks);

		int nPMSStatus = m_PMSModes.GetPMSLocationStatus(infoFile);

		if (0 == nPMSStatus)
		{
			continue;
		}

		bool bIsPMSLocation = (1 == nPMSStatus);

		CSSFile fileSales;
		if (fileSales.Open(infoFile.m_strFilename, "rb", _SH_DENYNO) == FALSE)
		{
			continue;
		}

		bool bTransactionServerMatch = FALSE;

		int nDbIdx;
		int nDbNo = dbLocation.GetDbNo(infoFile.m_nLocIdx);
		if (dbDatabase.FindDatabaseByNumber(nDbNo, nDbIdx) == FALSE)
		{
			nDbIdx = 0;
		}

		CDataManagerInfo infoDM;
		DataManager.OpenDatabaseReadOnly(nDbIdx, infoDM, FALSE);

		if (infoFile.m_nLocIdx != nCurrentTableLocIdx)
		{
			DataManagerNonDb.CloseTableNames(infoDM);
			DataManagerNonDb.OpenTableNames(DB_READONLY, infoFile.m_nLocIdx, infoDM);
			nCurrentTableLocIdx = infoFile.m_nLocIdx;
		}

		m_PMSModes.GetPMSTransactionDate(bIsPMSLocation, infoFile, strTranDate, strTranTime);

		int nConnectionType = CONNECTION_TYPE_STANDARD_NONE;
		{
			int nDummy = 0;
			dbLocation.GetLocSPOSVersion(infoFile.m_nLocIdx, nDummy, nConnectionType);
		}

		int nLinesToRead = 0;
		CString strBuffer = "";
		int nTableNo = 0;
		CString strTableName = "";
		int nSEQNo = 0;
		int nServerNo = 0;

		if (::FindFirstTermLine(fileSales, strBuffer, nLinesToRead) == TRUE)
		{
			m_CashRSPVersionChecker.ResetTransactionVersions();

			do
			{
				StatusProgress.SetPos(nProgress + fileSales.Tell(), nTotalFileSize);

				CCSVEposTermLine csvIn(strBuffer);

				int nCashRSPLineType = csvIn.GetLineTypeInt();

				if (m_PMSModes.CheckPMSTransactionType(bIsPMSLocation, nCashRSPLineType) == FALSE)
				{
					continue;
				}

				switch (nCashRSPLineType)
				{
				case CASHRSP_LINETYPE_TRAN_NORMAL:
				{
					m_CashRSPVersionChecker.ApplyPendingTransactionInfo();

					CString strDate = csvIn.GetTransactionDate();
					CString strTime = csvIn.GetTransactionTime();

					bInclude = FALSE;

					bool bAcceptDateTime = FALSE;

					if ((TRUE == bIsPMSLocation) && (m_PMSModes.GetDateMode() == PMS_DATEMODE_ACCOUNT))
					{
						bAcceptDateTime = TRUE;
					}
					else
					{
						CString strDate = csvIn.GetTransactionDate();
						CString strTime = csvIn.GetTransactionTime();

						//FILTER FOR DATE AND TIME
						if (strDate.GetLength() != 8 || strTime.GetLength() != 8)
						{
							continue;
						}

						strTranDate.Format("20%s%s%s",
							(const char*)strDate.Right(2),
							(const char*)strDate.Mid(3, 2),
							(const char*)strDate.Left(2));

						strTranTime.Format("%s%s%s",
							(const char*)strTime.Left(2),
							(const char*)strTime.Mid(3, 2),
							(const char*)strTime.Right(2));

						bAcceptDateTime = SimpleTimeCheck(infoFile.m_nLocIdx, strTranDate, strTranTime);
					}

					if (TRUE == bAcceptDateTime)
					{
						nTableNo = csvIn.GetTransactionTable();
						strTableName = "";

						oleTranDate = COleDateTime(
							atoi(strTranDate.Left(4)),
							atoi(strTranDate.Mid(4, 2)),
							atoi(strTranDate.Right(2)),
							0, 0, 0);

						if (m_ReportSettings.GetTableFilterFlag() == TRUE)
						{
							bInclude = (nTableNo >= m_ReportSettings.GetTableNoFrom()) && (nTableNo <= m_ReportSettings.GetTableNoTo());
						}
						else
						{
							bInclude = TRUE;
						}

						if (TRUE == bInclude)
						{
							if (m_ReportSettings.GetShowTableNameFlag() == TRUE)
							{
								int nTableIdx = 0;
								if (DataManagerNonDb.TableNames.FindTableByNumber(nTableNo, nTableIdx) == TRUE)
								{
									CTableNameCSVRecord TableName;
									DataManagerNonDb.TableNames.GetAt(nTableIdx, TableName);
									strTableName = TableName.GetTableName();
								}
							}

							nSEQNo = csvIn.GetTransactionNumber();
							nServerNo = csvIn.GetTransactionServer();
						}
					}
				}
				break;

				case CASHRSP_LINETYPE_TRAN_VOID:
				case CASHRSP_LINETYPE_WASTAGE:
				case CASHRSP_LINETYPE_NOSALE:
				case CASHRSP_LINETYPE_AGEVERIFY:
					m_CashRSPVersionChecker.ApplyPendingTransactionInfo();
					bInclude = FALSE;
					break;

				case CASHRSP_LINETYPE_PAYMENT:
				case CASHRSP_LINETYPE_DEPOSIT_PMS:
					if (TRUE == bInclude)
					{
						int nPaymentNo = csvIn.GetPaymentNumber();
						int nPaymentType = csvIn.GetPaymentType();

						switch (nConnectionType)
						{
						case CONNECTION_TYPE_SPTBOOK_NONE:
							nPaymentNo = DataManager.SBPaymentMap.GetSharpPosPaymentNo(nPaymentNo);
							break;

						case CONNECTION_TYPE_SMARTENT_NONE:
							nPaymentNo = DataManager.SEPaymentMap.GetSharpPosPaymentNo(nPaymentNo);
							break;

						case CONNECTION_TYPE_STANDARD_NONE:
						default:
							if (SysInfo.IsPMSSystem() == TRUE)
							{
								if ((m_PMSModes.GetEposFlag() == FALSE) && (FALSE == bIsPMSLocation))
								{
									if (DataManager.Payment.IsPMSPaymentType(csvIn.GetPaymentNumber()) == TRUE)
									{
										bInclude = FALSE;
									}
								}
							}
							break;
						}

						if (TRUE == bInclude)
						{
							if (m_ReportSettings.GetPayNumFilterFlag() == TRUE)
							{
								CSortedIntItem item;
								item.m_nItem = nPaymentNo;

								int nPos = 0;
								if (arrayPaymentNo.Find(item, nPos) == FALSE)
								{
									bInclude = FALSE;
								}
							}
						}

						if (TRUE == bInclude)
						{
							if (m_ReportSettings.IsPaymentTypeEnabled(nPaymentType) == FALSE)
							{
								bInclude = FALSE;
							}
						}

						if (TRUE == bInclude)
						{
							if ((EcrmanOptions.GetReportsMultiCashMethod() == REPORTCASH_METHOD1_CONSOLIDATE) && (nPaymentNo != 1))
							{
								int nType, nDummy32;
								__int64 nDummy64;
								bool bIsCash;
								csvIn.GetPaymentAccountTypeAndID(m_CashRSPVersionChecker, nType, nDummy32, nDummy64, bIsCash);

								if (TRUE == bIsCash)
								{
									nPaymentNo = 1;
								}
							}

							CEposReportConsolPaymentDetail info;
							info.m_nPaymentNo = nPaymentNo;
							info.m_strDate = strTranDate;
							info.m_strTime = strTranTime;
							info.m_nLocIdx = infoFile.m_nLocIdx;
							info.m_nTableNo = nTableNo;
							info.m_strTableName = strTableName;
							info.m_nSEQNo = nSEQNo;
							info.m_nServerNo = nServerNo;
							info.m_nAmount = csvIn.GetPaymentAmount();
							info.m_nTender = csvIn.GetPaymentTender();
							info.m_nGratuity = csvIn.GetPaymentGratuity();
							info.m_nCashback = csvIn.GetPaymentCashback();
							info.m_nSurplus = csvIn.GetPaymentSurplus(m_CashRSPVersionChecker.GetCurrentTransactionVersionNo());

							if (EcrmanOptions.GetReportsShowCashChangeFlag() == TRUE)
							{
								info.m_nChange = info.m_nTender - (info.m_nAmount + info.m_nGratuity + info.m_nSurplus);
							}
							else
							{
								info.m_nChange = 0;
							}

							for (int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++)
							{
								int nBlock = m_arrayTerminalBlocks.GetAt(n);
								info.m_bIsBlockTotal = FALSE;
								info.m_bIsPaymentTotal = FALSE;
								info.m_nLocIdx = infoFile.m_nLocIdx;
								info.m_nTableNo = nTableNo;
								info.m_strTableName = strTableName;
								m_arrayReportBlocks[nBlock].m_pDetail->Consolidate(info);
								info.m_nLocIdx = 0;
								info.m_nSEQNo = 0;
								info.m_nServerNo = 0;
								info.m_nTableNo = 0;
								info.m_strTableName = "";
								info.m_bIsPaymentTotal = TRUE;
								m_arrayReportBlocks[nBlock].m_pDetail->Consolidate(info);
								info.m_bIsPaymentTotal = FALSE;
								info.m_bIsBlockTotal = TRUE;
								m_arrayReportBlocks[nBlock].m_pDetail->Consolidate(info);
							}

							m_SelectArray.ProcessStartDate(infoFile.m_nSelectArrayIdx, oleTranDate);
							m_SelectArray.ProcessEndDate(infoFile.m_nSelectArrayIdx, oleTranDate);
						}
					}
					break;
				}

				switch (nCashRSPLineType)
				{
				case CASHRSP_LINETYPE_VERSION:
					m_CashRSPVersionChecker.ProcessCashRSPVersionLine(csvIn);
					break;

				case CASHRSP_LINETYPE_APPLICATION:
					m_CashRSPVersionChecker.ProcessAppTypeLine(csvIn);
					break;

				default:
					m_CashRSPVersionChecker.ResetPendingTransactionInfo();
					break;
				}
			} 
			while ((fileSales.ReadString(strBuffer) == TRUE) && (--nLinesToRead != 0) && (m_CashRSPVersionChecker.GetCashRSPFormatFailFlag() == FALSE));
		}
		
		nProgress += fileSales.GetLength();
		fileSales.Close();
	}
	StatusProgress.Unlock();
}

/**********************************************************************/

bool CEposReportPaymentDetail::CreateReport()
{
	if (m_ReportFile.Open(Super.ReportFilename()) == FALSE)
	{
		return FALSE;
	}

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	
	m_ReportFile.AddColumn ( "Date", TA_LEFT, 250 );
	m_ReportFile.AddColumn ( "Time", TA_LEFT, 200 );

	if (m_ReportSettings.GetShowSEQNoFlag() == TRUE)
	{
		m_ReportFile.AddColumn ( "SEQ No", TA_LEFT, 200 );
	}

	if (m_ReportSettings.GetShowServerFlag() == TRUE)
	{
		m_ReportFile.AddColumn("Srvr No", TA_LEFT, 200);
		m_ReportFile.AddColumn("Srvr Name", TA_LEFT, 300);
	}

	if (m_ReportSettings.GetShowTableNoFlag() == TRUE)
	{
		m_ReportFile.AddColumn ( "Tbl.No", TA_LEFT, 150 );
	}

	if (m_ReportSettings.GetShowTableNameFlag() == TRUE)
	{
		m_ReportFile.AddColumn ( "Table Name", TA_LEFT, 340 );
	}

	m_ReportFile.AddColumn ( "Tender", TA_RIGHT, 200 );
	m_ReportFile.AddColumn ( "Amount", TA_RIGHT, 200 );
	m_ReportFile.AddColumn ( "Gratuity", TA_RIGHT, 200 );
	m_ReportFile.AddColumn ( "Cashback", TA_RIGHT, 200 );
	m_ReportFile.AddColumn ( "Surplus", TA_RIGHT, 200 );

	if (EcrmanOptions.GetReportsShowCashChangeFlag() == TRUE)
	{
		m_ReportFile.AddColumn("Change", TA_RIGHT, 200);
	}
	
	m_nCreateReportCount = 0;
	m_nCreateReportTarget = 0;
	m_nCreateReportMiniCount = 0;
	m_nCreateReportMiniTarget = 0;
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		CEposReportPaymentDetailBlock ReportBlock = m_arrayReportBlocks.GetAt( n );
		m_nCreateReportTarget += ReportBlock.m_pDetail -> GetSize();
	}
	
	m_nCreateReportMiniTarget = m_nCreateReportTarget / 100;
	if (m_nCreateReportMiniTarget < 1)
	{
		m_nCreateReportMiniTarget = 1;
	}

	StatusProgress.Lock( TRUE, "Creating report" );

	int nLastDbIdx = -1;
	m_BlockMap.BuildSortArray();
	for ( int n = 0; n < m_BlockMap.GetSortArraySize(); n++ )
	{
		int nIdx = m_BlockMap.GetSortArrayIdx(n);
		CEposReportPaymentDetailBlock ReportBlock = m_arrayReportBlocks.GetAt( nIdx );

		if (ReportBlock.GotData() == FALSE)
		{
			continue;
		}

		{
			int nDbIdx = ReportBlock.m_nDbIdx;

			if (nDbIdx != nLastDbIdx)
			{
				nLastDbIdx = nDbIdx;

				CDataManagerInfo info;
				DataManager.OpenDatabaseReadOnly(nDbIdx, info, FALSE);
			}
		}

		m_ReportFile.HandleNewPage();

		CString strHeader;
		ReportBlock.GetHeaderText( strHeader );
		m_ReportFile.WriteReportMiscLine( strHeader );
		m_ReportFile.WriteReportMiscLine( "<LI>" );
		m_ReportFile.LockBlankLines(1);

		CreateSalesSection( ReportBlock );
	}

	StatusProgress.Unlock();

	ShowEposDetails();
	m_ReportFile.Close();
	
	return TRUE;
}

/**********************************************************************/

void CEposReportPaymentDetail::CreateSalesSection( CEposReportPaymentDetailBlock& Block )
{
	CString strUnderline = "\t\t";
	
	if ( m_ReportSettings.GetShowSEQNoFlag() == TRUE )
	{
		strUnderline += "\t";
	}

	if ( m_ReportSettings.GetShowServerFlag() == TRUE )
	{
		strUnderline += "\t\t";
	}

	if (m_ReportSettings.GetShowTableNoFlag() == TRUE)
	{
		strUnderline += "\t";
	}

	if (m_ReportSettings.GetShowTableNameFlag() == TRUE)
	{
		strUnderline += "\t";
	}

	strUnderline += "<LI>\t<LI>\t<LI>\t<LI>\t<LI>";
	
	if (EcrmanOptions.GetReportsShowCashChangeFlag() == TRUE)
	{
		strUnderline += "\t<LI>";
	}

	bool bWantHeader = TRUE;

	for ( int s = 0; s < Block.m_pDetail -> GetSize(); s++ )
	{
		CEposReportConsolPaymentDetail infoConsol;
		Block.m_pDetail -> GetAt( s, infoConsol );
		UpdateCreateReportProgress();

		int nPaymentNo = infoConsol.m_nPaymentNo;

		if ( TRUE == infoConsol.m_bIsBlockTotal )
		{
			CString strBlock;
			Block.GetHeaderText( strBlock, FALSE );
			
			CString strTotal = "";
			strTotal += "<..>Total ";
			strTotal += strBlock;
			
			m_ReportFile.WriteReportMiscLine( strTotal );
			m_ReportFile.WriteReportMiscLine( "<LI>" );
			WritePaymentLine( infoConsol, TRUE );
		}
		else if ( TRUE == infoConsol.m_bIsPaymentTotal )
		{
			m_ReportFile.WriteReportMiscLine( strUnderline );
			WritePaymentLine( infoConsol, TRUE );
			m_ReportFile.RequestBlankLines( 2 );
			bWantHeader = TRUE;
		}
		else
		{
			if ( TRUE == bWantHeader )
			{
				CString strHeader;
				strHeader.Format ( "<..>P%3.3d, %s", nPaymentNo, DataManager.Payment.GetDisplayName( nPaymentNo ) );
				m_ReportFile.WriteReportMiscLine( strHeader );
				m_ReportFile.WriteReportMiscLine( "<LI>" );
				bWantHeader = FALSE;
			}

			WritePaymentLine( infoConsol, FALSE );
		}
	}
}

/**********************************************************************/

void CEposReportPaymentDetail::WritePaymentLine(CEposReportConsolPaymentDetail& infoConsol, bool bTotal )
{
	CString strDate = "";
	CString strTime = "";

	if ((FALSE == infoConsol.m_bIsBlockTotal) && (FALSE == infoConsol.m_bIsPaymentTotal))
	{
		strDate.Format("%s/%s/%s",
			(const char*) infoConsol.m_strDate.Right(2),
			(const char*) infoConsol.m_strDate.Mid(4, 2),
			(const char*) infoConsol.m_strDate.Left(4));

		strTime.Format("%s:%s:%s",
			(const char*) infoConsol.m_strTime.Left(2),
			(const char*) infoConsol.m_strTime.Mid(2, 2),
			(const char*) infoConsol.m_strTime.Right(2));
	}
	else if (TRUE == infoConsol.m_bIsPaymentTotal)
	{
		strDate.Format("<..>Total %s", DataManager.Payment.GetDisplayName(infoConsol.m_nPaymentNo));
	}

	int nDP = SysInfo.GetDPValue();

	CCSV csv('\t');
	csv.Add(strDate);
	csv.Add(strTime);

	if (m_ReportSettings.GetShowSEQNoFlag() == TRUE)
	{
		if ( FALSE == bTotal )
		{
			CString strSEQNo = "";
			strSEQNo.Format("%6.6d", infoConsol.m_nSEQNo % 1000000);
			csv.Add(strSEQNo);
		}
		else
		{
			csv.Add("");
		}
	}

	if ( m_ReportSettings.GetShowServerFlag() == TRUE )
	{
		if ( FALSE == bTotal )
		{
			CString strServerNo = "";
			strServerNo.Format("%4.4d", infoConsol.m_nServerNo);
			csv.Add(strServerNo);
			csv.Add(DataManagerNonDb.ServerNameTable.GetServerName(NODE_LOCATION, infoConsol.m_nLocIdx, infoConsol.m_nServerNo, FALSE));
		}
		else
		{
			csv.Add("");
			csv.Add("");
		}
	}

	if ( m_ReportSettings.GetShowTableNoFlag() == TRUE )
	{
		if ( (infoConsol.m_nTableNo != 0) && ( FALSE == bTotal ) )
		{
			csv.Add(infoConsol.m_nTableNo);
		}
		else
		{
			csv.Add("");
		}
	}

	if (m_ReportSettings.GetShowTableNameFlag() == TRUE)
	{
		CString strTableName = "";

		if ( (infoConsol.m_nTableNo != 0) && ( FALSE == bTotal ) )
		{
			strTableName = infoConsol.m_strTableName;
		}
		
		csv.Add(strTableName);
	}

	csv.Add(infoConsol.GetReportTender(), nDP);
	csv.Add(infoConsol.GetReportAmount(), nDP);
	csv.Add(infoConsol.GetReportGratuity(), nDP);
	csv.Add(infoConsol.GetReportCashback(), nDP);
	csv.Add(infoConsol.GetReportSurplus(), nDP);

	if (EcrmanOptions.GetReportsShowCashChangeFlag() == TRUE)
	{
		csv.Add(infoConsol.GetReportChange(), nDP);
	}

	m_ReportFile.WriteReportDataLine(csv.GetLine());
}

/**********************************************************************/
