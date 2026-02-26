/**********************************************************************/
#include "PluCSVArray.h"
#include "ReportPointCSVArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "LocationCSVArray.h"
/**********************************************************************/

void CLocationCSVRecord::ConvertToCSV(CCSV& csv)
{
	CString strImportMask;
	strImportMask.Format("%X", m_nProdCSVImportTypeMask);

	CString strFieldMask;
	strFieldMask.Format("%X", m_nProdCSVProtectFieldMask);

	csv.Add(m_nLocNo);
	csv.Add(m_nNetworkNo);
	csv.Add(LOCATION_VERSION);
	csv.Add(m_nDbNo);
	csv.Add(m_strName);
	csv.Add(m_strIDraughtID);
	csv.Add(m_strRemoteID);
	csv.Add(m_nDefaultPriceSet);
	csv.Add(m_nPluPriceSetCount);
	csv.Add(m_nDbPriceSet1);
	csv.Add(m_nDbPriceSet2);
	csv.Add(m_nDbPriceSet3);
	csv.Add(m_nPriceSetKeyboard1);
	csv.Add(m_nPriceSetKeyboard2);
	csv.Add(m_nPriceSetKeyboard3);
	csv.Add(m_nDbKeyboardSet);
	csv.Add(m_strTerminalExportPath);
	csv.Add(m_strDownloadPath);
	csv.Add(m_strDownloadNowPath);
	csv.Add(m_bExportControl);
	csv.Add(m_strExportControlPath);
	csv.Add(m_nExportControlDelay);
	csv.Add(m_nProdCSVImportFolderType);
	csv.Add(m_nProdCSVImportExistingMode);
	csv.Add(strImportMask);
	csv.Add(strFieldMask);
	csv.Add(m_nProdCSVImportModifierMode);
	csv.Add(m_nCustomerImportFolderType);
	csv.Add(m_nBritanniaDefaultPriceBand);
	csv.Add(m_strAgressoLocationGroup);
	csv.Add(m_nLoyaltyFolderSet);
	csv.Add(m_nPluDownloadRangeType);

	if (m_nPluDownloadRangeType != 0)
	{
		for (int n = 0; n < 5; n++)
		{
			csv.Add(GetPluDownloadRangeStart(n));
			csv.Add(GetPluDownloadRangeEnd(n));
		}
	}

	int nCount = m_arrayTerminals.GetSize();
	csv.Add(nCount);

	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		CString strRpFlags;
		strRpFlags.Format("%15.15I64X", m_arrayTerminals[nIndex].m_nRpFlags);

		csv.Add(m_arrayTerminals[nIndex].m_nTNo);
		csv.Add(m_arrayTerminals[nIndex].m_nSpNo);
		csv.Add(m_arrayTerminals[nIndex].m_nRpNoLegacy);
		csv.Add(strRpFlags);
	}
}

/**********************************************************************/

void CLocationCSVRecord::ConvertFromCSV(CCSV& csv)
{
	ClearRecord();

	SetLocNo(csv.GetInt(0));
	SetNetworkNo(csv.GetInt(1));
	int nVer = csv.GetInt(2);

	//LOCATION DATABASE VERSION NUMBER
	switch (nVer)
	{
	case 1:
		V1ConvertFromCSV(csv);
		break;

	case 2:
		V2ConvertFromCSV(csv);
		break;

	case 3:
		V3ConvertFromCSV(csv);
		break;

	case 4:
		V4ConvertFromCSV(csv);
		break;

	case 5:
	case 6:
	case 7:
		V5To7ConvertFromCSV(csv, nVer);
		break;

	case 8:
	case 9:
		V8To9ConvertFromCSV(csv, nVer);
		break;

	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		V10To15ConvertFromCSV(csv, nVer);
		break;

	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
		V16To21ConvertFromCSV(csv, nVer);
		break;

	case 22:
	case 23:
		V22To23ConvertFromCSV(csv, nVer);
		break;
	}

	Validate(m_nLoyaltyFolderSet, 0, 10, 0);
	Validate(m_nDefaultPriceSet, 1, MAX_PRICE_SETS, 1);
	Validate(m_nPluPriceSetCount, 1, MAX_PRICE_SETS, 1);
	Validate(m_nBritanniaDefaultPriceBand, 1, MAX_PLU_PRICES, 1);
	Validate(m_nDbPriceSet1, 0, 20, 0);
	Validate(m_nDbPriceSet2, 0, 20, 0);
	Validate(m_nDbPriceSet3, 0, 20, 0);
	Validate(m_nPriceSetKeyboard1, 0, 14, 0);
	Validate(m_nPriceSetKeyboard2, 0, 14, 0);
	Validate(m_nPriceSetKeyboard3, 0, 14, 0);
	Validate(m_nDbKeyboardSet, 0, 20, 0);
	Validate(m_nExportControlDelay, 1, 30, 1);
	Validate(m_nProdCSVImportFolderType, 0, 2, 0);
	Validate(m_nProdCSVImportTypeMask, 0, 0x7FFFFFFF, 0x7FFFFFFF);
	Validate(m_nProdCSVProtectFieldMask, 0, 0x7FFFFFFF, 0x0);
	Validate(m_nCustomerImportFolderType, 1, 2, 1);
}

/**********************************************************************/

void CLocationCSVRecord::V1ConvertFromCSV(CCSV& csv)
{
	SetObsoleteRecordFlag(TRUE);

	SetDbNo(csv.GetInt(3));
	SetObsoleteSiteNo(csv.GetInt(4));
	SetName(csv.GetString(5));
	SetDefaultPriceSet(csv.GetInt(6));
	SetPluPriceSetCount(csv.GetInt(7));

	int nCount = csv.GetInt(8);

	if (nCount > MAX_TERMINALS)
	{
		nCount = MAX_TERMINALS;
	}

	int nTNo = 1;
	int nOffset = 9;
	for (int nTerminalIdx = 0; nTerminalIdx < nCount; nTerminalIdx++)
	{
		int nObsoleteEposNo = csv.GetInt(nOffset++);
		int nObsoleteTerminalNo = csv.GetInt(nOffset++);

		if (nTerminalIdx == 0)
		{
			nTNo = nObsoleteTerminalNo;
		}

		int nIdx = 0;
		int nSpNo = csv.GetInt(nOffset++);
		int nRpNoLegacy = csv.GetInt(nOffset++);

		if (AddTerminal(nTNo++, nRpNoLegacy, 0, nSpNo, nIdx) == TRUE)
		{
			if ((nRpNoLegacy >= 1) && (nRpNoLegacy <= MAX_SHARED_RPNO))
			{
				SetRpFlag(nIdx, nRpNoLegacy);
				SetRpNoLegacy(nIdx, 0);
			}
		}
	}
}

/**********************************************************************/

void CLocationCSVRecord::V2ConvertFromCSV(CCSV& csv)
{
	SetObsoleteRecordFlag(TRUE);

	SetDbNo(csv.GetInt(3));
	SetObsoleteSiteNo(csv.GetInt(4));
	SetName(csv.GetString(5));
	SetDefaultPriceSet(csv.GetInt(6));
	SetPluPriceSetCount(csv.GetInt(7));
	SetObsoleteUploadPathFlag(csv.GetBool(8));
	SetObsoleteUploadPath(csv.GetString(9));

	int nCount = csv.GetInt(10);

	if (nCount > MAX_TERMINALS)
	{
		nCount = MAX_TERMINALS;
	}

	int nTNo = 1;
	int nOffset = 11;
	for (int nTerminalIdx = 0; nTerminalIdx < nCount; nTerminalIdx++)
	{
		int nObsoleteEposNo = csv.GetInt(nOffset++);
		int nObsoleteTerminalNo = csv.GetInt(nOffset++);

		if (nTerminalIdx == 0)
		{
			nTNo = nObsoleteTerminalNo;
		}

		int nIdx = 0;
		int nSpNo = csv.GetInt(nOffset++);
		int nRpNoLegacy = csv.GetInt(nOffset++);

		if (AddTerminal(nTNo++, nRpNoLegacy, 0, nSpNo, nIdx) == TRUE)
		{
			if ((nRpNoLegacy >= 1) && (nRpNoLegacy <= MAX_SHARED_RPNO))
			{
				SetRpFlag(nIdx, nRpNoLegacy);
				SetRpNoLegacy(nIdx, 0);
			}
		}
	}
}

/**********************************************************************/

void CLocationCSVRecord::V3ConvertFromCSV(CCSV& csv)
{
	SetDbNo(csv.GetInt(3));
	SetName(csv.GetString(4));
	SetDefaultPriceSet(csv.GetInt(5));
	SetPluPriceSetCount(csv.GetInt(6));
	SetTerminalExportPath(csv.GetString(7));
	SetDownloadPath(csv.GetString(8));

	int nCount = csv.GetInt(9);

	if (nCount > MAX_TERMINALS)
	{
		nCount = MAX_TERMINALS;
	}

	int nTNo = 1;
	int nOffset = 10;
	for (int nTerminalIdx = 0; nTerminalIdx < nCount; nTerminalIdx++)
	{
		int nObsoleteEposNo = csv.GetInt(nOffset++);
		int nObsoleteTerminalNo = csv.GetInt(nOffset++);

		if (nTerminalIdx == 0)
		{
			nTNo = nObsoleteTerminalNo;
		}

		int nIdx = 0;
		int nSpNo = csv.GetInt(nOffset++);
		int nRpNoLegacy = csv.GetInt(nOffset++);

		if (AddTerminal(nTNo++, nRpNoLegacy, 0, nSpNo, nIdx) == TRUE)
		{
			if ((nRpNoLegacy >= 1) && (nRpNoLegacy <= MAX_SHARED_RPNO))
			{
				SetRpFlag(nIdx, nRpNoLegacy);
				SetRpNoLegacy(nIdx, 0);
			}
		}
	}
}

/**********************************************************************/

void CLocationCSVRecord::V4ConvertFromCSV(CCSV& csv)
{
	SetDbNo(csv.GetInt(3));
	SetName(csv.GetString(4));
	SetDefaultPriceSet(csv.GetInt(5));
	SetPluPriceSetCount(csv.GetInt(6));
	SetDbPriceSet1(csv.GetInt(7));
	SetDbPriceSet2(csv.GetInt(8));
	SetDbPriceSet3(csv.GetInt(9));
	SetTerminalExportPath(csv.GetString(10));
	SetDownloadPath(csv.GetString(11));
	SetDownloadNowPath(csv.GetString(12));

	int nCount = csv.GetInt(13);

	if (nCount > MAX_TERMINALS)
	{
		nCount = MAX_TERMINALS;
	}

	int nTNo = 1;
	int nOffset = 14;
	for (int nTerminalIdx = 0; nTerminalIdx < nCount; nTerminalIdx++)
	{
		int nObsoleteEposNo = csv.GetInt(nOffset++);
		int nObsoleteTerminalNo = csv.GetInt(nOffset++);

		if (nTerminalIdx == 0)
		{
			nTNo = nObsoleteTerminalNo;
		}

		int nIdx = 0;
		int nSpNo = csv.GetInt(nOffset++);
		int nRpNoLegacy = csv.GetInt(nOffset++);

		if (AddTerminal(nTNo++, nRpNoLegacy, 0, nSpNo, nIdx) == TRUE)
		{
			if ((nRpNoLegacy >= 1) && (nRpNoLegacy <= MAX_SHARED_RPNO))
			{
				SetRpFlag(nIdx, nRpNoLegacy);
				SetRpNoLegacy(nIdx, 0);
			}
		}
	}
}

/**********************************************************************/

void CLocationCSVRecord::V5To7ConvertFromCSV(CCSV& csv, int nVer)
{
	SetDbNo(csv.GetInt(3));
	int nMasterTNo = csv.GetInt(4);
	SetName(csv.GetString(5));
	SetDefaultPriceSet(csv.GetInt(6));
	SetPluPriceSetCount(csv.GetInt(7));
	SetDbPriceSet1(csv.GetInt(8));
	SetDbPriceSet2(csv.GetInt(9));
	SetDbPriceSet3(csv.GetInt(10));

	int nOffset = 11;

	if (nVer != 5)
	{
		SetPriceSetKeyboard1(csv.GetInt(nOffset++));
		SetPriceSetKeyboard2(csv.GetInt(nOffset++));
		SetPriceSetKeyboard3(csv.GetInt(nOffset++));
	}

	SetTerminalExportPath(csv.GetString(nOffset++));
	SetDownloadPath(csv.GetString(nOffset++));
	SetDownloadNowPath(csv.GetString(nOffset++));

	if (7 == nVer)
	{
		nOffset++; //WAS EOD UPLOAD PATH
	}

	int nCount = csv.GetInt(nOffset++);

	if (nCount > MAX_TERMINALS)
	{
		nCount = MAX_TERMINALS;
	}

	for (int nTerminalIdx = 0; nTerminalIdx < nCount; nTerminalIdx++)
	{
		int nIdx = 0;
		int nSpNo = csv.GetInt(nOffset++);
		int nRpNoLegacy = csv.GetInt(nOffset++);

		if (AddTerminal(nMasterTNo + nTerminalIdx, nRpNoLegacy, 0, nSpNo, nIdx) == TRUE)
		{
			if ((nRpNoLegacy >= 1) && (nRpNoLegacy <= MAX_SHARED_RPNO))
			{
				SetRpFlag(nIdx, nRpNoLegacy);
				SetRpNoLegacy(nIdx, 0);
			}
		}
	}
}

/**********************************************************************/

void CLocationCSVRecord::V8To9ConvertFromCSV(CCSV& csv, int nVer)
{
	int nOffset = 3;

	SetDbNo(csv.GetInt(nOffset++));
	int nMasterTNo = csv.GetInt(nOffset++);
	SetName(csv.GetString(nOffset++));

	if (nVer >= 9)
	{
		SetIDraughtID(csv.GetString(nOffset++));
	}

	SetDefaultPriceSet(csv.GetInt(nOffset++));
	SetPluPriceSetCount(csv.GetInt(nOffset++));
	SetDbPriceSet1(csv.GetInt(nOffset++));
	SetDbPriceSet2(csv.GetInt(nOffset++));
	SetDbPriceSet3(csv.GetInt(nOffset++));
	SetPriceSetKeyboard1(csv.GetInt(nOffset++));
	SetPriceSetKeyboard2(csv.GetInt(nOffset++));
	SetPriceSetKeyboard3(csv.GetInt(nOffset++));
	SetDbKeyboardSet(csv.GetInt(nOffset++));
	SetTerminalExportPath(csv.GetString(nOffset++));
	SetDownloadPath(csv.GetString(nOffset++));
	SetDownloadNowPath(csv.GetString(nOffset++));

	int nCount = csv.GetInt(nOffset++);

	if (nCount > MAX_TERMINALS)
	{
		nCount = MAX_TERMINALS;
	}

	for (int nTerminalIdx = 0; nTerminalIdx < nCount; nTerminalIdx++)
	{
		int nIdx = 0;
		int nSpNo = csv.GetInt(nOffset++);
		int nRpNoLegacy = csv.GetInt(nOffset++);

		if (AddTerminal(nMasterTNo + nTerminalIdx, nRpNoLegacy, 0, nSpNo, nIdx) == TRUE)
		{
			if ((nRpNoLegacy >= 1) && (nRpNoLegacy <= MAX_SHARED_RPNO))
			{
				SetRpFlag(nIdx, nRpNoLegacy);
				SetRpNoLegacy(nIdx, 0);
			}
		}
	}
}

/**********************************************************************/

void CLocationCSVRecord::V10To15ConvertFromCSV(CCSV& csv, int nVer)
{
	int nOffset = 3;

	SetDbNo(csv.GetInt(nOffset++));
	SetName(csv.GetString(nOffset++));
	SetIDraughtID(csv.GetString(nOffset++));

	if (nVer >= 13)
		SetRemoteID(csv.GetString(nOffset++));

	SetDefaultPriceSet(csv.GetInt(nOffset++));
	SetPluPriceSetCount(csv.GetInt(nOffset++));
	SetDbPriceSet1(csv.GetInt(nOffset++));
	SetDbPriceSet2(csv.GetInt(nOffset++));
	SetDbPriceSet3(csv.GetInt(nOffset++));
	SetPriceSetKeyboard1(csv.GetInt(nOffset++));
	SetPriceSetKeyboard2(csv.GetInt(nOffset++));
	SetPriceSetKeyboard3(csv.GetInt(nOffset++));
	SetDbKeyboardSet(csv.GetInt(nOffset++));
	SetTerminalExportPath(csv.GetString(nOffset++));
	SetDownloadPath(csv.GetString(nOffset++));
	SetDownloadNowPath(csv.GetString(nOffset++));

	if (nVer >= 11)
	{
		SetExportControlFlag(csv.GetBool(nOffset++));
		SetExportControlPath(csv.GetString(nOffset++));
		SetExportControlDelay(csv.GetInt(nOffset++));
	}

	if ((12 == nVer) || (13 == nVer))
	{
		SetProdCSVImportFolderType(csv.GetBool(nOffset++) ? EODIMPORT_FOLDERTYPE_ROOT : EODIMPORT_FOLDERTYPE_NONE);
	}
	else if (nVer >= 14)
	{
		SetProdCSVImportFolderType(csv.GetInt(nOffset++));
	}

	SetProdCSVImportExistingMode(csv.GetInt(nOffset++));

	if (nVer >= 15)
	{
		SetBritanniaDefaultPriceBand(csv.GetInt(nOffset++));
	}

	int nCount = csv.GetInt(nOffset++);

	if (nCount > MAX_TERMINALS)
	{
		nCount = MAX_TERMINALS;
	}

	for (int nTerminalIdx = 0; nTerminalIdx < nCount; nTerminalIdx++)
	{
		int nIdx = 0;
		int nTNo = csv.GetInt(nOffset++);
		int nSpNo = csv.GetInt(nOffset++);
		int nRpNoLegacy = csv.GetInt(nOffset++);

		if (AddTerminal(nTNo, nRpNoLegacy, 0, nSpNo, nIdx) == TRUE)
		{
			if ((nRpNoLegacy >= 1) && (nRpNoLegacy <= MAX_SHARED_RPNO))
			{
				SetRpFlag(nIdx, nRpNoLegacy);
				SetRpNoLegacy(nIdx, 0);
			}
		}
	}

	if (nVer < 14)
	{
		if (EODIMPORT_FOLDERTYPE_ROOT == m_nProdCSVImportFolderType)
		{
			int nLength = m_strTerminalExportPath.GetLength();

			if (nLength > 10)
			{
				CString str = m_strTerminalExportPath.Right(10);
				str.MakeUpper();

				if (str == "\\INTRADING")
				{
					m_strTerminalExportPath = m_strTerminalExportPath.Left(nLength - 10);
					m_nProdCSVImportFolderType = EODIMPORT_FOLDERTYPE_INTRADING;
				}
			}
		}
		else
		{
			m_nProdCSVImportFolderType = EODIMPORT_FOLDERTYPE_ROOT;
		}
	}
}

/**********************************************************************/

void CLocationCSVRecord::V16To21ConvertFromCSV(CCSV& csv, int nVer)
{
	int nOffset = 3;

	SetDbNo(csv.GetInt(nOffset++));
	SetName(csv.GetString(nOffset++));
	SetIDraughtID(csv.GetString(nOffset++));
	SetRemoteID(csv.GetString(nOffset++));
	SetDefaultPriceSet(csv.GetInt(nOffset++));
	SetPluPriceSetCount(csv.GetInt(nOffset++));
	SetDbPriceSet1(csv.GetInt(nOffset++));
	SetDbPriceSet2(csv.GetInt(nOffset++));
	SetDbPriceSet3(csv.GetInt(nOffset++));
	SetPriceSetKeyboard1(csv.GetInt(nOffset++));
	SetPriceSetKeyboard2(csv.GetInt(nOffset++));
	SetPriceSetKeyboard3(csv.GetInt(nOffset++));
	SetDbKeyboardSet(csv.GetInt(nOffset++));
	SetTerminalExportPath(csv.GetString(nOffset++));
	SetDownloadPath(csv.GetString(nOffset++));
	SetDownloadNowPath(csv.GetString(nOffset++));
	SetExportControlFlag(csv.GetBool(nOffset++));
	SetExportControlPath(csv.GetString(nOffset++));
	SetExportControlDelay(csv.GetInt(nOffset++));
	SetProdCSVImportFolderType(csv.GetInt(nOffset++));
	SetProdCSVImportExistingMode(csv.GetInt(nOffset++));

	if (nVer >= 18)
	{
		SetProdCSVImportTypeMask(::HexToInt(csv.GetString(nOffset++)));
	}

	if (nVer >= 19)
	{
		SetProdCSVProtectFieldMask(::HexToInt(csv.GetString(nOffset++)));
	}

	if (nVer >= 20)
	{
		SetProdCSVImportModifierMode(csv.GetInt(nOffset++));
	}

	SetCustomerImportFolderType(csv.GetInt(nOffset++));
	SetBritanniaDefaultPriceBand(csv.GetInt(nOffset++));

	if (nVer >= 21)
	{
		SetAgressoLocationGroup(csv.GetString(nOffset++));
	}

	int nCount = csv.GetInt(nOffset++);

	if (nCount > MAX_TERMINALS)
	{
		nCount = MAX_TERMINALS;
	}

	for (int nTerminalIdx = 0; nTerminalIdx < nCount; nTerminalIdx++)
	{
		int nIdx = 0;
		int nTNo = csv.GetInt(nOffset++);
		int nSpNo = csv.GetInt(nOffset++);
		int nRpNoLegacy = csv.GetInt(nOffset++);

		__int64 nRpFlags = 0;
		if (nVer >= 17)
		{
			CString strRpFlags = csv.GetString(nOffset++);
			nRpFlags = ::HexToInt64(strRpFlags);
		}

		if (AddTerminal(nTNo, nRpNoLegacy, nRpFlags, nSpNo, nIdx) == TRUE)
		{
			if ((nRpNoLegacy >= 1) && (nRpNoLegacy <= MAX_SHARED_RPNO))
			{
				SetRpFlag(nIdx, nRpNoLegacy);
				SetRpNoLegacy(nIdx, 0);
			}
		}
	}
}

/**********************************************************************/

void CLocationCSVRecord::V22To23ConvertFromCSV(CCSV& csv, int nVer)
{
	int nOffset = 3;

	SetDbNo(csv.GetInt(nOffset++));
	SetName(csv.GetString(nOffset++));
	SetIDraughtID(csv.GetString(nOffset++));
	SetRemoteID(csv.GetString(nOffset++));
	SetDefaultPriceSet(csv.GetInt(nOffset++));
	SetPluPriceSetCount(csv.GetInt(nOffset++));
	SetDbPriceSet1(csv.GetInt(nOffset++));
	SetDbPriceSet2(csv.GetInt(nOffset++));
	SetDbPriceSet3(csv.GetInt(nOffset++));
	SetPriceSetKeyboard1(csv.GetInt(nOffset++));
	SetPriceSetKeyboard2(csv.GetInt(nOffset++));
	SetPriceSetKeyboard3(csv.GetInt(nOffset++));
	SetDbKeyboardSet(csv.GetInt(nOffset++));
	SetTerminalExportPath(csv.GetString(nOffset++));
	SetDownloadPath(csv.GetString(nOffset++));
	SetDownloadNowPath(csv.GetString(nOffset++));
	SetExportControlFlag(csv.GetBool(nOffset++));
	SetExportControlPath(csv.GetString(nOffset++));
	SetExportControlDelay(csv.GetInt(nOffset++));
	SetProdCSVImportFolderType(csv.GetInt(nOffset++));
	SetProdCSVImportExistingMode(csv.GetInt(nOffset++));
	SetProdCSVImportTypeMask(::HexToInt(csv.GetString(nOffset++)));
	SetProdCSVProtectFieldMask(::HexToInt(csv.GetString(nOffset++)));
	SetProdCSVImportModifierMode(csv.GetInt(nOffset++));
	SetCustomerImportFolderType(csv.GetInt(nOffset++));
	SetBritanniaDefaultPriceBand(csv.GetInt(nOffset++));
	SetAgressoLocationGroup(csv.GetString(nOffset++));

	if (nVer >= 23)
	{
		SetLoyaltyFolderSet(csv.GetInt(nOffset++));
	}

	SetPluDownloadRangeType(csv.GetInt(nOffset++));

	if (m_nPluDownloadRangeType != 0)
	{
		for (int n = 0; n < 5; n++)
		{
			SetPluDownloadRangeStart(n, csv.GetInt64(nOffset++));
			SetPluDownloadRangeEnd(n, csv.GetInt64(nOffset++));
		}
	}

	int nCount = csv.GetInt(nOffset++);

	if (nCount > MAX_TERMINALS)
	{
		nCount = MAX_TERMINALS;
	}

	for (int nTerminalIdx = 0; nTerminalIdx < nCount; nTerminalIdx++)
	{
		int nIdx = 0;
		int nTNo = csv.GetInt(nOffset++);
		int nSpNo = csv.GetInt(nOffset++);
		int nRpNoLegacy = csv.GetInt(nOffset++);

		__int64 nRpFlags = 0;
		{
			CString strRpFlags = csv.GetString(nOffset++);
			nRpFlags = ::HexToInt64(strRpFlags);
		}

		if (AddTerminal(nTNo, nRpNoLegacy, nRpFlags, nSpNo, nIdx) == TRUE)
		{
			if ((nRpNoLegacy >= 1) && (nRpNoLegacy <= MAX_SHARED_RPNO))
			{
				SetRpFlag(nIdx, nRpNoLegacy);
				SetRpNoLegacy(nIdx, 0);
			}
		}
	}

	TidyPluDownloadRanges();
}

/**********************************************************************/

void CLocationCSVRecord::Validate( int& nTarget, int nMin, int nMax, int nDefault )
{
	if ((nTarget < nMin) || (nTarget > nMax))
	{
		nTarget = nDefault;
	}
}

/**********************************************************************/
