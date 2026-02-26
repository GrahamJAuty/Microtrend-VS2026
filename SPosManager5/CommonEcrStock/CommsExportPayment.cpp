/**********************************************************************/
#include "CommsErrors.h"
#include "DatabaseCSVArray.h"
#include "SPOSVersions.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
#include "PaymentCSVArray.h"
#include "PriceHelpers.h"
#include "SQLOut.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "CommsExportPayment.h"
/**********************************************************************/

CCommsExportPayment::CCommsExportPayment(CJobListCSVRecord& JobInfo, bool bFromMemory) : CCommsExportBase(JobInfo, bFromMemory)
{
}

/**********************************************************************/

int CCommsExportPayment::PrepareFile()
{
	if (m_nNetworkIdx < 0 || m_nNetworkIdx >= dbNetwork.GetSize())
	{
		return COMMS_ERROR_BADNETIDX;
	}

	switch (dbNetwork.GetConnectionType(m_nNetworkIdx))
	{
	case CONNECTION_TYPE_STANDARD_NONE:
	case CONNECTION_TYPE_SPTBOOK_NONE:
	case CONNECTION_TYPE_SPTBOOK_WEB:
		return COMMS_ERROR_NOCOMMS;
	}

	if (m_nLocIdx < 0 || m_nLocIdx >= dbLocation.GetSize())
	{
		return COMMS_ERROR_BADLOCIDX;
	}

	m_strSourceFolder = dbNetwork.GetFolderPathData(m_nNetworkIdx);
	m_strSourceFolder += "\\";

	m_strDestFolder = LocationMasterMap.GetDownloadFolder(m_nLocIdx, m_nTerminalNo, m_bImmediate);

	int nResult = COMMS_ERROR_NODATA;

	int nDbIdx = 0;
	int nDbNo = dbLocation.GetDbNo(m_nLocIdx);
	if (dbDatabase.FindDatabaseByNumber(nDbNo, nDbIdx) == FALSE)
	{
		return COMMS_ERROR_BADDBLINK;
	}

	if (FALSE == m_bFromMemory)
	{
		CDataManagerInfo info;
		DataManager.OpenDatabaseQuickNoFiles(nDbIdx, TRUE);
		DataManager.ReopenPayment(info);
	}

	if (DataManager.Payment.GetSize() != 0)
	{
		int nSPOSVersion = dbNetwork.GetSPOSVersion(m_nNetworkIdx);

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4) == TRUE)
		{
			nResult = PrepareSPOSFile(nSPOSVersion);
		}
		else
		{
			nResult = COMMS_ERROR_CREATEFILE;
		}
	}

	return nResult;
}

/**********************************************************************/

int CCommsExportPayment::PrepareSPOSFile(int nSPOSVersion)
{
	CSSFile filePayment;
	m_strSourceFilename = "PayMeths.sql";
	if (filePayment.Open(m_strSourceFolder + m_strSourceFilename, "wb") == FALSE)
	{
		return COMMS_ERROR_CREATEFILE;
	}

	CCSV csvHeader;
	csvHeader.Add("[PymNo]");
	csvHeader.Add("[Name]");
	csvHeader.Add("[PaymentType]");
	csvHeader.Add("[Floor]");
	csvHeader.Add("[Tender]");
	csvHeader.Add("[OverTender]");
	csvHeader.Add("[Change]");
	csvHeader.Add("[Gratuity]");
	csvHeader.Add("[Drawer]");
	csvHeader.Add("[Eft]");
	csvHeader.Add("[Cashback]");
	csvHeader.Add("[NonBank]");
	csvHeader.Add("[PayText]");
	csvHeader.Add("[RctPrint]");
	csvHeader.Add("[FuncNo]");
	csvHeader.Add("[ForexNo]");
	csvHeader.Add("[ForHH]");

	if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_634) == TRUE)
	{
		csvHeader.Add("[WithCashLimitDrawer]");
	}

	if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_780) == TRUE)
	{
		csvHeader.Add("[DepositChange]");
		csvHeader.Add("[DepositChangeAsVoucherMin]");
	}

	if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_1300) == TRUE)
	{
		csvHeader.Add("[ExternAppParam1]");
	}

	if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_1392) == TRUE)
	{
		csvHeader.Add("[AccountAllow]");
	}

	if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_1977) == TRUE)
	{
		csvHeader.Add("[CashDeclaration]");
	}

	if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_631) == TRUE)
	{
		csvHeader.Add("[ChangeAsCashback]");
	}

	CSQLOut SQLOut(filePayment, "PayMeths", csvHeader.GetLine());
	SQLOut.WriteFileBegin();
	SQLOut.WriteDelete();

	StatusProgress.Lock();
	int nSize = DataManager.Payment.GetSize();
	for (int nPayIdx = 0; nPayIdx < nSize; nPayIdx++)
	{
		StatusProgress.SetPos(nPayIdx, nSize);

		CPaymentCSVRecord Payment;
		DataManager.Payment.GetAt(nPayIdx, Payment);

		CMinimumSPOSVersion Version;
		Payment.CheckMinimumSPOSVersion(Version);

		int nErrorType = -1;
		Version.CheckSPOSVersion(nSPOSVersion, nErrorType);

		if (nErrorType != -1)
		{
			TallyDownloadExclusion(NODE_PAYMENT, nErrorType);
			continue;
		}

		SQLOut.Add(Payment.GetPaymentNo());
		SQLOut.Add(Payment.GetEPOSName());
		SQLOut.Add(Payment.GetPaymentType());
		SQLOut.Add(Payment.GetLimit(), SysInfo.GetDPValue());
		SQLOut.Add(Payment.GetTender());
		SQLOut.Add(Payment.GetOverTender());
		SQLOut.Add(Payment.GetChange());
		SQLOut.Add(Payment.GetGratuity());
		SQLOut.Add(Payment.GetDrawer());

		//EFT ALSO CONTROLS CASHBACK
		switch (Payment.GetPaymentType())
		{
		case PAYMENT_TYPE_EFT:
		case PAYMENT_TYPE_EFT_CNP:
			SQLOut.Add(TRUE);
			SQLOut.Add(Payment.GetEFTCashback());
			break;

		case PAYMENT_TYPE_EXTERNAL_PAY:
			SQLOut.Add(FALSE);
			SQLOut.Add(Payment.GetEFTCashback());
			break;

		default:
			SQLOut.Add(FALSE);
			SQLOut.Add(FALSE);
			break;
		}

		SQLOut.Add(Payment.GetNonBnk());
		SQLOut.Add(Payment.GetSQLText());
		SQLOut.Add(Payment.GetReceipt());
		SQLOut.Add(Payment.GetFunctionNo());
		SQLOut.Add(Payment.GetForexNo());
		SQLOut.Add(Payment.GetHandheld());

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_634) == TRUE)
		{
			if (Payment.GetPaymentNo() == 1)
			{
				SQLOut.Add(TRUE);
			}
			else
			{
				SQLOut.Add(Payment.GetCashLimitFlag());
			}
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_780) == TRUE)
		{
			SQLOut.Add(Payment.GetDepositChangeType());
			SQLOut.Add(Payment.GetDepositChangeVoucherMin());
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_1300) == TRUE)
		{
			SQLOut.Add(Payment.GetExternalParams());
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_1392) == TRUE)
		{
			SQLOut.Add(Payment.GetAccountAllow());
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_1977) == TRUE)
		{
			SQLOut.Add(Payment.GetCashDeclarationFlag());
		}

		if (CSPOSVersions::CheckSPOSVersion(nSPOSVersion, SPOS_V4_2_631) == TRUE)
		{
			if (Payment.GetPaymentType() == PAYMENT_TYPE_OTHER)
			{
				SQLOut.Add(Payment.GetChangeAsCashbackFlag());
			}
			else
			{
				SQLOut.Add(FALSE);
			}
		}

		SQLOut.WriteInsert();
		TallyDownloadAccept();
	}
	StatusProgress.Unlock();

	SQLOut.WriteFileEnd();
	filePayment.Close();
	return COMMS_ERROR_NONE;
}

/**********************************************************************/
