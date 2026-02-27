//$$******************************************************************
// DbReportHandler.cpp: implementation of the CDbReportHandler class.
//
// Derived from CDbExportHandler -  ( CDbExportHandler see for list of report labels )
//
//$$******************************************************************
#include "..\SmartPay4Shared\AlertText.h"
#include "..\SmartPay4Shared\AllergyText.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_PINNumber\SQLRepositoryPINNumber.h"
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\DbReportFields.h"
//$$******************************************************************
#include "DbReportHandler.h"
//$$******************************************************************

CDbReportHandler::CDbReportHandler()
{
	ClearTotals();						
	DbReportFields.ResetColumnsFromUserText();
}

//$$******************************************************************

void CDbReportHandler::ClearTotals()
{
	ClearTotals(nREPORT_SORTON);
	ClearTotals(nREPORT_GROUP);
	ClearTotals(nREPORT_TOTAL);
}

//$$******************************************************************

void CDbReportHandler::ClearTotals(int nIndex)
{
	if ((nIndex >= 0) && (nIndex < nREPORT_MAX))
	{
		m_nTotalPoints[nIndex] = 0;
		m_nTotalPointsTD[nIndex] = 0;
		m_dTotalPurse1Liability[nIndex] = 0.0;
		m_dTotalPurse1Credit[nIndex] = 0.0;
		m_dTotalPurse2[nIndex] = 0.0;
		m_dTotalPurse3Liability[nIndex] = 0.0;
		m_dTotalPurse3Credit[nIndex] = 0.0;
		m_dTotalPurse1Spend[nIndex] = 0.0;
		m_dTotalPurse2Spend[nIndex] = 0.0;
		m_dTotalPurse3Spend[nIndex] = 0.0;
		m_dTotalPurse1SpendTD[nIndex] = 0.0;
		m_dTotalPurse2SpendTD[nIndex] = 0.0;
		m_dTotalPurse3SpendTD[nIndex] = 0.0;
		m_dTotalCashSpendTD[nIndex] = 0.0;
		m_dTotalCardSpendTD[nIndex] = 0.0;
		m_dTotalSpendTD[nIndex] = 0.0;
	}
}

//$$******************************************************************

void CDbReportHandler::AddSpecialColumn(CString strColumn)
{
	DbReportFields.AddColumn(strColumn, DBREPORTFIELD_COLTYPE_SPECIAL);
}

//$$******************************************************************
// always adds GRP column

void CDbReportHandler::AddOrderByColumnText()
{
	switch ( GetSortOn() )
	{
	case nSORTONUSERID:		DbReportFields.AddColumn(Account::UserID.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							DbReportFields.AddColumn(Account::Username.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							break;

	case nSORTONUSERNAME:	DbReportFields.AddColumn(Account::Username.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							DbReportFields.AddColumn(Account::UserID.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							break;

	case nSORTONSURNAME:	DbReportFields.AddColumn(Account::Surname.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							DbReportFields.AddColumn(Account::UserID.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							break;
	
	case nSORTONMEMBERID:	DbReportFields.AddColumn(Account::MemberID.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							DbReportFields.AddColumn(Account::UserID.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							break;
		
	case nSORTONEXTREF1:	DbReportFields.AddColumn(Account::ExternalRef1.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							DbReportFields.AddColumn(Account::UserID.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							break;

	case nSORTONEXTREF2:	DbReportFields.AddColumn(Account::ExternalRef2.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							DbReportFields.AddColumn(Account::UserID.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							break;

	case nSORTONINFO1:		DbReportFields.AddColumn(Account::Info1.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							DbReportFields.AddColumn(Account::UserID.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							break;

	case nSORTONINFO2:		DbReportFields.AddColumn(Account::Info2.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							DbReportFields.AddColumn(Account::UserID.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							break;

	case nSORTONINFO3:		DbReportFields.AddColumn(Account::Info3.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							DbReportFields.AddColumn(Account::UserID.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							break;

	case nSORTONINFO4:		DbReportFields.AddColumn(Account::Info4.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							DbReportFields.AddColumn(Account::UserID.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							break;

	case nSORTONINFO5:		DbReportFields.AddColumn(Account::Info5.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							DbReportFields.AddColumn(Account::UserID.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							break;

	case nSORTONINFO6:		DbReportFields.AddColumn(Account::Info6.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							DbReportFields.AddColumn(Account::UserID.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							break;

	case nSORTONINFO7:		DbReportFields.AddColumn(Account::Info7.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							DbReportFields.AddColumn(Account::UserID.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							break;

	case nSORTONINFO8:		DbReportFields.AddColumn(Account::Info8.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							DbReportFields.AddColumn(Account::UserID.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
							break;
	}

	DbReportFields.AddColumn(Account::GroupNo.Label, DBREPORTFIELD_COLTYPE_ORDERBY);
}

//$$******************************************************************

void CDbReportHandler::AddCardNo(CCSV* pCsv, __int64 nCardNo, bool bExists)
{
	CString strCardNo = System.FormatCardNo(nCardNo, TRUE);
	if (FALSE == bExists)
	{
		strCardNo += " *";
	}
	pCsv->Add(strCardNo);
}

//$$******************************************************************
// szText can be additiona text to include in report title before database name

CString CDbReportHandler::GetReportTitle ( const char* szText )
{
	CString strMsg = "";
	if (m_strLabel.Left(2) == "UR")
	{
		strMsg = m_strFilename;		//"Custom Database Report" text;
	}
	else
	{
		strMsg = m_strTitle;		// standard report title;
	}

	CString strReportTitle= "";
	strReportTitle.Format ( "%s%s", 
		(const char*) strMsg,
		szText );

	return strReportTitle;
}

//$$******************************************************************
// get filename to hold report --p16\temp01\Rnnn.rep

CString CDbReportHandler::GetReportFilename()
{
	CString strFilename	= Filenames.GetReportFilename ( m_strLabel );
	return strFilename;
}

//$$******************************************************************
// get filename to hold report parameters ..p16\cardnnn\reports\Rnnn.prm

CString CDbReportHandler::GetParamsFilename()
{
	CString strFilename	= Filenames.GetReportParamsFilename ( m_strLabel );
	return strFilename;
}

//$$******************************************************************
// create report key depending on filters used

CString CDbReportHandler::GetReportKey()
{
	CString strReportKey = "";
	strReportKey.Format ( "%s%s", 
		(const char*) m_strLabel, 
		(const char*) m_csvFilters.GetLine() );

	return strReportKey;
}

//$$******************************************************************
// add additional fields

void CDbReportHandler::CreateReportHeader ( CReportFileWithEndText* pReport )
{
	CCSV csvHeader ( m_strHeader );						// fields for report
	
	for (int n = 0; n < csvHeader.GetSize(); n++)
	{
		CString strHeader = csvHeader.GetString(n);
		
		if (strHeader == Account::GroupShiftStatus.Label)
		{
			if (System.GetAllowGroupShiftDateRangeFlag() == TRUE)
			{
				DbReportFields.AddColumn(strHeader, DBREPORTFIELD_COLTYPE_NORMAL);
				DbReportFields.AddColumn(Account::GroupShiftStartDate.Label, DBREPORTFIELD_COLTYPE_NORMAL);
				DbReportFields.AddColumn(Account::GroupShiftEndDate.Label, DBREPORTFIELD_COLTYPE_NORMAL);
				DbReportFields.AddColumn(Account::GroupShiftFutureGroup.Label, DBREPORTFIELD_COLTYPE_NORMAL);
			}
		}
		else
		{
			DbReportFields.AddColumn(strHeader, DBREPORTFIELD_COLTYPE_NORMAL);
		}
	}

	DbReportFields.CreateReportHeader(pReport);
}

//$$******************************************************************

void CDbReportHandler::AppendDbFieldsToCSVLine(CSQLRowAccountFull& AccountRow, CCSV* pCsv, bool bExists, __int64 nUnknownCardNo, CString strUnknownCardName, int nFixGroupNo)
{
	int nSpecialField = 0;

	if (bExists == FALSE)
	{
		for (int n = 0; n < DbReportFields.GetColumnCount(); n++)
		//for (int n = pCsv->GetSize(); n < DbReportFields.GetColumnCount(); n++)
		{
			if (DbReportFields.GetColType(n) == DBREPORTFIELD_COLTYPE_SPECIAL)
			{
				pCsv->Add(GetSpecialField(nSpecialField++));
			}
			else
			{
				switch (DbReportFields.GetFieldType(n))
				{
				case DBREPORTFIELD_USERID:
					AddCardNo(pCsv, nUnknownCardNo, FALSE);
					break;

				case DBREPORTFIELD_USERNAME:
					if (strUnknownCardName != "")
					{
						pCsv->Add(strUnknownCardName);
					}
					else
					{
						pCsv->Add("unknown");
					}
					break;

				case DBREPORTFIELD_GROUPNO:
					if (nFixGroupNo != -1)
					{
						pCsv->Add(nFixGroupNo);
					}
					else
					{
						pCsv->Add("-");
					}
					break;

				default:
					pCsv->Add("");
					break;
				}
			}
		}
	}
	else
	{
		for (int n = 0; n < DbReportFields.GetColumnCount(); n++)
		//for (int n = pCsv->GetSize(); n < DbReportFields.GetColumnCount(); n++)
		{
			if (DbReportFields.GetColType(n) == DBREPORTFIELD_COLTYPE_SPECIAL)
			{
				pCsv->Add(GetSpecialField(nSpecialField++));
			}
			else
			{
				switch (DbReportFields.GetFieldType(n))
				{
				case DBREPORTFIELD_USERID:
					AddCardNo(pCsv, AccountRow.GetUserID(), bExists);
					break;

				case DBREPORTFIELD_USERNAME:
					pCsv->Add(AccountRow.GetUsername());
					break;

				case DBREPORTFIELD_FORENAME:
					pCsv->Add(AccountRow.GetForename());
					break;

				case DBREPORTFIELD_SURNAME:
					pCsv->Add(AccountRow.GetSurname());
					break;

				case DBREPORTFIELD_GROUPNO:
					if ( nFixGroupNo != -1 )
					{
						pCsv->Add(nFixGroupNo);
					}
					else
					{
						pCsv->Add(AccountRow.GetGroupNo());
					}
					break;

				case DBREPORTFIELD_GROUPSHIFT_STATUS:
					switch (AccountRow.GetGroupShiftStatus())
					{
					case 0:
						pCsv->Add("None");
						break;

					case 1:
						pCsv->Add("Current");
						break;

					case 2:
						pCsv->Add("Future");
						break;

					case 3:
						pCsv->Add("???");
						break;
					}
					break;

				case DBREPORTFIELD_GROUPSHIFT_STARTDATE:
					switch (AccountRow.GetGroupShiftStatus())
					{
					case 1:
					case 2:
						pCsv->Add(AccountRow.GetGroupShiftStartDate());
						break;

					default:
						pCsv->Add("---");
						break;
					}
					break;

				case DBREPORTFIELD_GROUPSHIFT_ENDDATE:
					switch (AccountRow.GetGroupShiftStatus())
					{
					case 1:
					case 2:
						pCsv->Add(AccountRow.GetGroupShiftEndDate());
						break;

					default:
						pCsv->Add("---");
						break;
					}
					break;

				case DBREPORTFIELD_GROUPSHIFT_FUTUREGROUP:
					switch (AccountRow.GetGroupShiftStatus())
					{
					case 1:
					case 2:
						pCsv->Add(AccountRow.GetGroupShiftFutureGroup());
						break;

					default:
						pCsv->Add("---");
						break;
					}
					break;

				case DBREPORTFIELD_EMPTYFIELD:
					pCsv->Add("");
					break;

				case DBREPORTFIELD_DOB:
					pCsv->Add(AccountRow.GetDOB());
					break;

				case DBREPORTFIELD_GENDER:
				{
					CString strGender = AccountRow.GetGender();
					if (strGender == "")
					{
						strGender = "-";
					}
					pCsv->Add(strGender);
				}
				break;

				case DBREPORTFIELD_EXPIRY:
					pCsv->Add(AccountRow.GetExpiryDate());
					break;

				case DBREPORTFIELD_DAYGROUPS:
				{
					CString strData = AccountRow.GetDayGroups();
					if (strData.GetLength() > 0 && strData != ",,,,,,")		// check have some data
					{
						pCsv->Add(strData);
					}
					else
					{
						pCsv->Add("");										//30/09/2016 - v1.13b
					}
				}
				break;

				case DBREPORTFIELD_ALERTCODE:
					pCsv->Add(AccountRow.GetAlertCode());
					break;

				case DBREPORTFIELD_ALERTDATE:
					pCsv->Add(AccountRow.GetAlertDate());
					break;

				case DBREPORTFIELD_ALERTTIME:
					pCsv->Add(AccountRow.GetAlertTime());
					break;

				case DBREPORTFIELD_INACTIVE:
					pCsv->Add(GetBoolText(AccountRow.GetInactive()));
					break;

				case DBREPORTFIELD_ALLERGYCODES:
					pCsv->Add(AccountRow.GetAllergyCodes());
					break;

				case DBREPORTFIELD_DIETARY:
					pCsv->Add(AccountRow.GetDietary());
					break;

				case DBREPORTFIELD_OWNMAXSPEND:
					pCsv->Add(AccountRow.GetOwnMaxSpend());
					break;

				case DBREPORTFIELD_P1ALLOW:
					pCsv->Add(AccountRow.GetPurse1Allow());
					break;

				case DBREPORTFIELD_P2ALLOW:
					pCsv->Add(AccountRow.GetPurse2Allow());
					break;

				case DBREPORTFIELD_P3ALLOW:
					pCsv->Add(AccountRow.GetPurse3Allow());
					break;

				case DBREPORTFIELD_P1MAXSPEND1:
					pCsv->Add(AccountRow.GetMaxSpendPurse1Period1(), System.GetDPValue());
					break;

				case DBREPORTFIELD_P1MAXSPEND2:
					pCsv->Add(AccountRow.GetMaxSpendPurse1Period2(), System.GetDPValue());
					break;

				case DBREPORTFIELD_P1MAXSPEND3:
					pCsv->Add(AccountRow.GetMaxSpendPurse1Period3(), System.GetDPValue());
					break;

				case DBREPORTFIELD_P1MAXSPEND4:
					pCsv->Add(AccountRow.GetMaxSpendPurse1Period4(), System.GetDPValue());
					break;

				case DBREPORTFIELD_P1MAXSPEND5:
					pCsv->Add(AccountRow.GetMaxSpendPurse1Period5(), System.GetDPValue());
					break;

				case DBREPORTFIELD_P1MAXSPEND6:
					pCsv->Add(AccountRow.GetMaxSpendPurse1Period6(), System.GetDPValue());
					break;

				case DBREPORTFIELD_P1OVERSPEND1:
					pCsv->Add(AccountRow.GetMaxOverdraftPurse1Period1(), System.GetDPValue());
					break;

				case DBREPORTFIELD_P1OVERSPEND2:
					pCsv->Add(AccountRow.GetMaxOverdraftPurse1Period2(), System.GetDPValue());
					break;

				case DBREPORTFIELD_P1OVERSPEND3:
					pCsv->Add(AccountRow.GetMaxOverdraftPurse1Period3(), System.GetDPValue());
					break;

				case DBREPORTFIELD_P1OVERSPEND4:
					pCsv->Add(AccountRow.GetMaxOverdraftPurse1Period4(), System.GetDPValue());
					break;

				case DBREPORTFIELD_P1OVERSPEND5:
					pCsv->Add(AccountRow.GetMaxOverdraftPurse1Period5(), System.GetDPValue());
					break;

				case DBREPORTFIELD_P1OVERSPEND6:
					pCsv->Add(AccountRow.GetMaxOverdraftPurse1Period6(), System.GetDPValue());
					break;

				case DBREPORTFIELD_P3MAXSPEND1:
					pCsv->Add(AccountRow.GetMaxSpendPurse3Period1(), System.GetDPValue());
					break;

				case DBREPORTFIELD_P3MAXSPEND2:
					pCsv->Add(AccountRow.GetMaxSpendPurse3Period2(), System.GetDPValue());
					break;

				case DBREPORTFIELD_P3MAXSPEND3:
					pCsv->Add(AccountRow.GetMaxSpendPurse3Period3(), System.GetDPValue());
					break;

				case DBREPORTFIELD_P3MAXSPEND4:
					pCsv->Add(AccountRow.GetMaxSpendPurse3Period4(), System.GetDPValue());
					break;

				case DBREPORTFIELD_P3MAXSPEND5:
					pCsv->Add(AccountRow.GetMaxSpendPurse3Period5(), System.GetDPValue());
					break;

				case DBREPORTFIELD_P3MAXSPEND6:
					pCsv->Add(AccountRow.GetMaxSpendPurse3Period6(), System.GetDPValue());
					break;

				case DBREPORTFIELD_P3OVERSPEND1:
					pCsv->Add(AccountRow.GetMaxOverdraftPurse3Period1(), System.GetDPValue());
					break;

				case DBREPORTFIELD_P3OVERSPEND2:
					pCsv->Add(AccountRow.GetMaxOverdraftPurse3Period2(), System.GetDPValue());
					break;

				case DBREPORTFIELD_P3OVERSPEND3:
					pCsv->Add(AccountRow.GetMaxOverdraftPurse3Period3(), System.GetDPValue());
					break;

				case DBREPORTFIELD_P3OVERSPEND4:
					pCsv->Add(AccountRow.GetMaxOverdraftPurse3Period4(), System.GetDPValue());
					break;

				case DBREPORTFIELD_P3OVERSPEND5:
					pCsv->Add(AccountRow.GetMaxOverdraftPurse3Period5(), System.GetDPValue());
					break;

				case DBREPORTFIELD_P3OVERSPEND6:
					pCsv->Add(AccountRow.GetMaxOverdraftPurse3Period6(), System.GetDPValue());
					break;

				case DBREPORTFIELD_P1LIABILITY:
				{
					double dPurse = AccountRow.GetPurse1Liability();
					pCsv->Add(dPurse, System.GetDPValue());
					m_dTotalPurse1Liability[nREPORT_SORTON] += dPurse;
					m_dTotalPurse1Liability[nREPORT_GROUP] += dPurse;
					m_dTotalPurse1Liability[nREPORT_TOTAL] += dPurse;
				}
				break;

				case DBREPORTFIELD_P1CREDIT:
				{
					double dPurse = AccountRow.GetPurse1Credit();
					pCsv->Add(dPurse, System.GetDPValue());
					m_dTotalPurse1Credit[nREPORT_SORTON] += dPurse;
					m_dTotalPurse1Credit[nREPORT_GROUP] += dPurse;
					m_dTotalPurse1Credit[nREPORT_TOTAL] += dPurse;
				}
				break;

				case DBREPORTFIELD_P1DATE:
					pCsv->Add(AccountRow.GetPurse1LastSpendDate());
					break;

				case DBREPORTFIELD_P1TIME:
					pCsv->Add(AccountRow.GetPurse1LastSpendTime());
					break;

				case DBREPORTFIELD_P1LASTSPEND:
				{
					double dSpend = AccountRow.GetPurse1LastSpend();
					pCsv->Add(dSpend, System.GetDPValue());
					m_dTotalPurse1Spend[nREPORT_SORTON] += dSpend;
					m_dTotalPurse1Spend[nREPORT_GROUP] += dSpend;
					m_dTotalPurse1Spend[nREPORT_TOTAL] += dSpend;
				}
				break;

				case DBREPORTFIELD_P1SPENDTD:
				{
					double dSpendTD = AccountRow.GetPurse1SpendToDate();
					pCsv->Add(dSpendTD, System.GetDPValue());
					m_dTotalPurse1SpendTD[nREPORT_SORTON] += dSpendTD;
					m_dTotalPurse1SpendTD[nREPORT_GROUP] += dSpendTD;
					m_dTotalPurse1SpendTD[nREPORT_TOTAL] += dSpendTD;
				}
				break;

				case DBREPORTFIELD_P2BALANCE:
				{
					double dPurse = AccountRow.GetPurse2Balance();
					pCsv->Add(dPurse, System.GetDPValue());
					m_dTotalPurse2[nREPORT_SORTON] += dPurse;
					m_dTotalPurse2[nREPORT_GROUP] += dPurse;
					m_dTotalPurse2[nREPORT_TOTAL] += dPurse;
				}
				break;

				case DBREPORTFIELD_P2DATE:
					pCsv->Add(AccountRow.GetPurse2LastSpendDate());
					break;

				case DBREPORTFIELD_P2TIME:
					pCsv->Add(AccountRow.GetPurse2LastSpendTime());
					break;

				case DBREPORTFIELD_P2LASTSPEND:
				{
					double dSpend = AccountRow.GetPurse2LastSpend();
					pCsv->Add(dSpend, System.GetDPValue());
					m_dTotalPurse2Spend[nREPORT_SORTON] += dSpend;
					m_dTotalPurse2Spend[nREPORT_GROUP] += dSpend;
					m_dTotalPurse2Spend[nREPORT_TOTAL] += dSpend;
				}
				break;

				case DBREPORTFIELD_P2SPENDTD:
				{
					double dSpendTD = AccountRow.GetPurse2SpendToDate();
					pCsv->Add(dSpendTD, System.GetDPValue());
					m_dTotalPurse2SpendTD[nREPORT_SORTON] += dSpendTD;
					m_dTotalPurse2SpendTD[nREPORT_GROUP] += dSpendTD;
					m_dTotalPurse2SpendTD[nREPORT_TOTAL] += dSpendTD;
				}
				break;

				case DBREPORTFIELD_REFRESHREF:
					pCsv->Add(AccountRow.GetRefreshTextDays());
					break;

				case DBREPORTFIELD_REFRESHDATE:
					pCsv->Add(AccountRow.GetPurse2RefreshedDate());
					break;

				case DBREPORTFIELD_REFRESHTIME:
					pCsv->Add(AccountRow.GetPurse2RefreshedTime());
					break;

				case DBREPORTFIELD_P3LIABILITY:
				{
					double dPurse = AccountRow.GetPurse3Liability();
					pCsv->Add(dPurse, System.GetDPValue());
					m_dTotalPurse3Liability[nREPORT_SORTON] += dPurse;
					m_dTotalPurse3Liability[nREPORT_GROUP] += dPurse;
					m_dTotalPurse3Liability[nREPORT_TOTAL] += dPurse;
				}
				break;

				case DBREPORTFIELD_P3CREDIT:
				{
					double dPurse = AccountRow.GetPurse3Credit();
					pCsv->Add(dPurse, System.GetDPValue());
					m_dTotalPurse3Credit[nREPORT_SORTON] += dPurse;
					m_dTotalPurse3Credit[nREPORT_GROUP] += dPurse;
					m_dTotalPurse3Credit[nREPORT_TOTAL] += dPurse;
				}
				break;

				case DBREPORTFIELD_P3DATE:
					pCsv->Add(AccountRow.GetPurse3LastSpendDate());
					break;

				case DBREPORTFIELD_P3TIME:
					pCsv->Add(AccountRow.GetPurse3LastSpendTime());
					break;

				case DBREPORTFIELD_P3LASTSPEND:
				{
					double dSpend = AccountRow.GetPurse3LastSpend();
					pCsv->Add(dSpend, System.GetDPValue());
					m_dTotalPurse3Spend[nREPORT_SORTON] += dSpend;
					m_dTotalPurse3Spend[nREPORT_GROUP] += dSpend;
					m_dTotalPurse3Spend[nREPORT_TOTAL] += dSpend;
				}
				break;

				case DBREPORTFIELD_P3SPENDTD:
				{
					double dSpendTD = AccountRow.GetPurse3SpendToDate();
					pCsv->Add(dSpendTD, System.GetDPValue());
					m_dTotalPurse3SpendTD[nREPORT_SORTON] += dSpendTD;
					m_dTotalPurse3SpendTD[nREPORT_GROUP] += dSpendTD;
					m_dTotalPurse3SpendTD[nREPORT_TOTAL] += dSpendTD;
				}
				break;

				case DBREPORTFIELD_POINTS:
				{
					int nPoints = AccountRow.GetPoints();
					pCsv->Add(nPoints);
					m_nTotalPoints[nREPORT_SORTON] += nPoints;
					m_nTotalPoints[nREPORT_GROUP] += nPoints;
					m_nTotalPoints[nREPORT_TOTAL] += nPoints;
				}
				break;

				case DBREPORTFIELD_POINTSTD:
				{
					int nPoints = AccountRow.GetPointsToDate();
					pCsv->Add(nPoints);
					m_nTotalPointsTD[nREPORT_SORTON] += nPoints;
					m_nTotalPointsTD[nREPORT_GROUP] += nPoints;
					m_nTotalPointsTD[nREPORT_TOTAL] += nPoints;
				}
				break;

				case DBREPORTFIELD_CASHTD:
				{
					double dSpendTD = AccountRow.GetCashSpendToDate();
					pCsv->Add(dSpendTD, System.GetDPValue());
					m_dTotalCashSpendTD[nREPORT_SORTON] += dSpendTD;
					m_dTotalCashSpendTD[nREPORT_GROUP] += dSpendTD;
					m_dTotalCashSpendTD[nREPORT_TOTAL] += dSpendTD;
				}
				break;

				case DBREPORTFIELD_MEMBERID:
					pCsv->Add(AccountRow.GetMemberID());
					break;

				case DBREPORTFIELD_EMAIL:
					pCsv->Add(AccountRow.GetEmail());
					break;

				case DBREPORTFIELD_INFO1:
					pCsv->Add(AccountRow.GetInfo1());
					break;

				case DBREPORTFIELD_INFO2:
					pCsv->Add(AccountRow.GetInfo2());
					break;

				case DBREPORTFIELD_INFO3:
					pCsv->Add(AccountRow.GetInfo3());
					break;

				case DBREPORTFIELD_INFO4:
					pCsv->Add(AccountRow.GetInfo4());
					break;

				case DBREPORTFIELD_INFO5:
					pCsv->Add(AccountRow.GetInfo5());
					break;

				case DBREPORTFIELD_INFO6:
					pCsv->Add(AccountRow.GetInfo6());
					break;

				case DBREPORTFIELD_INFO7:
					pCsv->Add(AccountRow.GetInfo7());
					break;

				case DBREPORTFIELD_INFO8:
					pCsv->Add(AccountRow.GetInfo8());
					break;

				case DBREPORTFIELD_TICK1:
					pCsv->Add(GetBoolText(AccountRow.GetTick1()));
					break;

				case DBREPORTFIELD_TICK2:
					pCsv->Add(GetBoolText(AccountRow.GetTick2()));
					break;

				case DBREPORTFIELD_TICK3:
					pCsv->Add(GetBoolText(AccountRow.GetTick3()));
					break;

				case DBREPORTFIELD_TICK4:
					pCsv->Add(GetBoolText(AccountRow.GetTick4()));
					break;

				case DBREPORTFIELD_TOTALSPENDTD:
				{
					double dSpendTD = AccountRow.GetTotalSpendToDate();
					pCsv->Add(dSpendTD, System.GetDPValue());
					m_dTotalSpendTD[nREPORT_SORTON] += dSpendTD;
					m_dTotalSpendTD[nREPORT_GROUP] += dSpendTD;
					m_dTotalSpendTD[nREPORT_TOTAL] += dSpendTD;
				}
				break;

				case DBREPORTFIELD_DOB_NEXTAGE:
					pCsv->Add(AccountRow.GetAgeNextBirthday());
					break;

				case DBREPORTFIELD_DOB_BDAY:
					pCsv->Add(AccountRow.GetDOBDayOfWeek());
					break;

				case DBREPORTFIELD_ALERTTEXT:
					pCsv->Add(GetAlertText(AccountRow));
					break;

				case DBREPORTFIELD_ALLERGYTEXT:
					pCsv->Add(GetAllergyText(AccountRow));
					break;

				case DBREPORTFIELD_EXTREF:
					pCsv->Add(AccountRow.GetExternalRef1());
					break;

				case DBREPORTFIELD_EXTREF2:
					pCsv->Add(AccountRow.GetExternalRef2());
					break;

				case DBREPORTFIELD_DELETEDDATE:
					pCsv->Add(CSSDate::GetCurrentDate().GetDate());
					break;

				case DBREPORTFIELD_DELETEDTIME:
					pCsv->Add(CSSTime::GetCurrentTime().GetTime());
					break;

				case DBREPORTFIELD_PIN:
					{
						CString strPIN = "";

						CSQLRowPINNumber rowPIN;
						rowPIN.SetUserID(FormatInt64Value(AccountRow.GetUserID()));

						CSQLRepositoryPINNumber repoPIN;
						if ( repoPIN.SelectRowByUserID(rowPIN, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
						{
							strPIN = rowPIN.GetPINNumber();
						}
						
						pCsv->Add(strPIN);
					}
					break;

				case DBREPORTFIELD_BLANK:
				default:
					pCsv->Add("");
					break;
				}
			}
		}
	}
}

//$$******************************************************************

const char* CDbReportHandler::GetBoolText ( bool bFlag )
{
	return ( bFlag == TRUE ) ? "X" : "-";
}

//$$******************************************************************

CString CDbReportHandler::GetAllergyText(CSQLRowAccountFull& AccountRow)
{
	CString strText = "";
	
	if ( AccountRow.HaveAllergies() == TRUE )
	{
		CAllergyText allergy;
		allergy.Read();
		allergy.SetAllergyCodes (AccountRow.GetAllergyCodes() );	// translate codes to a string
		strText = allergy.GetAllergyString();
	}

	return strText;
}

//$$******************************************************************

CString CDbReportHandler::GetAlertText( CSQLRowAccountFull& AccountRow )
{
	CString strText = "";

	if ( AccountRow.IsAlertSet() == TRUE )
	{
		CAlertText alert{};
		alert.Open ( Filenames.GetAlertFilename() );	// readonly
		strText = alert.GetText ( AccountRow.GetAlertCode() );
	}
	return strText;
}

//$$******************************************************************

void CDbReportHandler::CreateTotalLine(int nTextPos, CString strText, CCSV* pCsv, CCSV* pCsvTabLine, int nIndex, int nGroupCount)
{
	int nSpecialIndex = 0;

	for (int n = 0; n < DbReportFields.GetColumnCount(); n++)
	{
		if (nTextPos == n)
		{
			CString strFullLine = "<..>";
			if (strText.Left(4) != strFullLine)
			{
				strFullLine += strText;
				strText = strFullLine;
			}
			pCsv->Add(strText);
			pCsvTabLine->Add("");
		}
		else
		{
			if (DbReportFields.GetColType(n) == DBREPORTFIELD_COLTYPE_SPECIAL)
			{
				CString strField = GetSpecialField(nSpecialIndex++);
				pCsv->Add(strField);
				pCsvTabLine->Add( (strField == "") ? "" : "<LI>" );
			}
			else
			{
				bool bHaveTotal = FALSE;

				switch (DbReportFields.GetFieldType(n))
				{
				case DBREPORTFIELD_GROUPNO:
					pCsv->Add(nGroupCount);
					bHaveTotal = TRUE;
					break;

				case DBREPORTFIELD_P1LIABILITY:
					pCsv->Add(m_dTotalPurse1Liability[nIndex], System.GetDPValue());
					bHaveTotal = TRUE;
					break;

				case DBREPORTFIELD_P1CREDIT:
					pCsv->Add(m_dTotalPurse1Credit[nIndex], System.GetDPValue());
					bHaveTotal = TRUE;
					break;

				case DBREPORTFIELD_P1LASTSPEND:
					pCsv->Add(m_dTotalPurse1Spend[nIndex], System.GetDPValue());
					bHaveTotal = TRUE;
					break;

				case DBREPORTFIELD_P1SPENDTD:
					pCsv->Add(m_dTotalPurse1SpendTD[nIndex], System.GetDPValue());
					bHaveTotal = TRUE;
					break;

				case DBREPORTFIELD_P2BALANCE:
					pCsv->Add(m_dTotalPurse2[nIndex], System.GetDPValue());
					bHaveTotal = TRUE;
					break;

				case DBREPORTFIELD_P2LASTSPEND:
					pCsv->Add(m_dTotalPurse2Spend[nIndex], System.GetDPValue());
					bHaveTotal = TRUE;
					break;

				case DBREPORTFIELD_P2SPENDTD:
					pCsv->Add(m_dTotalPurse2SpendTD[nIndex], System.GetDPValue());
					bHaveTotal = TRUE;
					break;

				case DBREPORTFIELD_P3LIABILITY:
					pCsv->Add(m_dTotalPurse3Liability[nIndex], System.GetDPValue());
					bHaveTotal = TRUE;
					break;

				case DBREPORTFIELD_P3CREDIT:
					pCsv->Add(m_dTotalPurse3Credit[nIndex], System.GetDPValue());
					bHaveTotal = TRUE;
					break;

				case DBREPORTFIELD_P3LASTSPEND:
					pCsv->Add(m_dTotalPurse3Spend[nIndex], System.GetDPValue());
					bHaveTotal = TRUE;
					break;

				case DBREPORTFIELD_P3SPENDTD:
					pCsv->Add(m_dTotalPurse3SpendTD[nIndex], System.GetDPValue());
					bHaveTotal = TRUE;
					break;

				case DBREPORTFIELD_POINTS:
					pCsv->Add(m_nTotalPoints[nIndex]);
					bHaveTotal = TRUE;
					break;

				case DBREPORTFIELD_POINTSTD:
					pCsv->Add(m_nTotalPointsTD[nIndex]);
					bHaveTotal = TRUE;
					break;

				case DBREPORTFIELD_CASHTD:
					pCsv->Add(m_dTotalCashSpendTD[nIndex], System.GetDPValue());
					bHaveTotal = TRUE;
					break;

				case DBREPORTFIELD_TOTALSPENDTD:
					pCsv->Add(m_dTotalSpendTD[nIndex], System.GetDPValue());
					bHaveTotal = TRUE;
					break;
				}

				if (TRUE == bHaveTotal)
				{
					pCsvTabLine->Add("<LI>");
				}
				else
				{
					pCsv->Add("");
					pCsvTabLine->Add("");
				}
			}
		}
	}
}

//$$******************************************************************

void CDbReportHandler::ResetSpecialFields()
{
	m_arraySpecialFields.RemoveAll();
}

//$$******************************************************************

void CDbReportHandler::AddSpecialField( CString str )
{
	m_arraySpecialFields.Add(str);
}

//$$******************************************************************

void CDbReportHandler::AddSpecialField(double d, int nDPs)
{
	CString str = "";
	str.Format("%.*f", nDPs, d);
	m_arraySpecialFields.Add(str);
}

//$$******************************************************************

void CDbReportHandler::AddSpecialField(int n)
{
	CString str = "";
	str.Format("%d", n);
	m_arraySpecialFields.Add(str);
}

//$$******************************************************************

CString CDbReportHandler::GetSpecialField(int n)
{
	CString str = "";
	if ((n >= 0) && (n < m_arraySpecialFields.GetSize()))
	{
		str = m_arraySpecialFields.GetAt(n);
	}
	return str;
}

//$$******************************************************************
