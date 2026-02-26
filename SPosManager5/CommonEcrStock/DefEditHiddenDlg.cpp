/**********************************************************************/
#include "DefEditHiddenDlg.h"
/**********************************************************************/

CDefEditHiddenDlg::CDefEditHiddenDlg( CWnd* pParent )
	: CSSDialog(CDefEditHiddenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDefEditHiddenDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CDefEditHiddenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditHiddenDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_HIDDEN, m_editHidden);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditHiddenDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditHiddenDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditHiddenDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_editHidden.LimitText(200);

	CString strHidden = "";
	AppendFlag(strHidden, DealerFlags.GetIDraughtExportFlag(), 3);
	AppendFlag(strHidden, DealerFlags.GetBatchExportFlag(), 4);
	AppendNum(strHidden, DealerFlags.GetPrinterRedirectMode(), 1, 2, 7);
	AppendFlag(strHidden, DealerFlags.GetSageExportFlag(), 8);
	AppendFlag(strHidden, DealerFlags.GetCascadeReportFlag(), 9);
	AppendFlag(strHidden, DealerFlags.GetCostleyTransactionLogFlag(), 15);
	AppendFlag(strHidden, DealerFlags.GetWeeklyReportNoZeroSkipFlag(), 16);
	AppendNum(strHidden, DealerFlags.GetSortCodeRenameType(), 1, 2, 17);
	AppendFlag(strHidden, DealerFlags.GetLogFileWritesFlag(), 18);
	AppendFlag(strHidden, DealerFlags.GetReportCostPriceFlag(), 19);
	AppendFlag(strHidden, DealerFlags.GetHoltsDiscountFlag(), 21);
	AppendFlag(strHidden, DealerFlags.GetLocationNotesFlag(), 25);
	AppendFlag(strHidden, DealerFlags.GetSpawnRepmanFlag(), 26);
	AppendNum(strHidden, DealerFlags.Get_LIPA_LJMU_ExportType(), 1, 2, 27);
	AppendFlag(strHidden, DealerFlags.GetLogNonDuplicatesFlag(), 28);
	AppendFlag(strHidden, DealerFlags.GetPMSExtendedRoomRateFlag(), 29);
	AppendFlag(strHidden, DealerFlags.GetLogWebAPICallsFlag(), 30);

	if ( DealerFlags.GetRemoveLoyaltyTopupFlag() == TRUE )
	{
		strHidden += DealerFlags.GetMisc32String();
		strHidden += "\r\n";
	}

	AppendFlag(strHidden, DealerFlags.GetGlobalPromotionFlag(), 33);
	AppendFlag(strHidden, DealerFlags.GetPosTraySetupNoLockAllUsersFlag(), 34);
	AppendFlag(strHidden, DealerFlags.GetReasonTicksNoDownloadFlag(), 35);
	AppendFlag(strHidden, DealerFlags.GetReasonTicksVoidAfterFlag(), 36);
	AppendFlag(strHidden, DealerFlags.GetServerHomeLocationFlag(), 37);
	AppendFlag(strHidden, DealerFlags.GetServerImportExportFlag(), 38);
	AppendFlag(strHidden, DealerFlags.GetRefundFixFlag(), 39);
	AppendFlag(strHidden, DealerFlags.GetIgnoreUnpaidTransactionsFlag(), 40);
	AppendFlag(strHidden, DealerFlags.GetRefund2FixFlag(), 41);
	AppendFlag(strHidden, DealerFlags.GetGenRecExcludeLoyaltyFlag(), 42);
	AppendFlag(strHidden, DealerFlags.GetUseDatabaseVATBandFlag(), 43);
	AppendFlag(strHidden, DealerFlags.GetAllDiscountsVATableFlag(), 44);
	AppendFlag(strHidden, DealerFlags.Get15DigitKludgeDeptGroupFlag(), 45);
	AppendFlag(strHidden, DealerFlags.GetGratuityFixFlag(), 46);
	AppendFlag(strHidden, DealerFlags.GetGratuityFix2Flag(), 47);
	AppendFlag(strHidden, DealerFlags.GetLinkPluFixFlag(), 48);
	AppendFlag(strHidden, DealerFlags.GetDuplicateCheckIgnoreTimeFlag(), 49);
	
	DealerFlags.SetFixDepositRecycle(
		DealerFlags.GetFixDepositRecycleFlag(),
		DealerFlags.GetFixDepositRedeemPayNo(),
		DealerFlags.GetFixDepositRecyclePayNo(),
		DealerFlags.GetFixDepositRecyclePluNo());

	if (DealerFlags.GetFixDepositRecycleFlag() == TRUE)
	{
		CString strLine = "";
		strLine.Format("MISC50,%d,%d,%I64d\r\n",
			DealerFlags.GetFixDepositRedeemPayNo(),
			DealerFlags.GetFixDepositRecyclePayNo(),
			DealerFlags.GetFixDepositRecyclePluNo());

		strHidden += strLine;
	}
	
	AppendFlag(strHidden, DealerFlags.GetCOFAllDiscountFlag(), 51);

#ifdef STOCKMAN_SYSTEM
	AppendFlag(strHidden, DealerFlags.GetRebuildStockSalesHistoryFlag(), 20);
	AppendFlag(strHidden, DealerFlags.GetDorbiereImportFlag(), 22);
#else
	AppendFlag(strHidden, DealerFlags.GetApparentStockDateFlag(), 23);
#endif

	m_editHidden.SetWindowText(strHidden);

	return TRUE;
}

/**********************************************************************/

void CDefEditHiddenDlg::AppendFlag(CString& strHidden, bool b, int nFlagNum)
{
	if (TRUE == b)
	{
		CString strFlagNum = "";
		strFlagNum.Format("%d", nFlagNum);
		strHidden += "MISC";
		strHidden += strFlagNum;
		strHidden += "\r\n";
	}
}

/**********************************************************************/

void CDefEditHiddenDlg::AppendNum(CString& strHidden, int nNum, int nMin, int nMax, int nFlagNum)
{
	if ( (nNum >= nMin ) && ( nNum <= nMax) )
	{
		CString strFlag = "";
		strFlag.Format("MISC%d,%d\r\n",
			nFlagNum,
			nNum);

		strHidden += strFlag;
	}
}

/**********************************************************************/

void CDefEditHiddenDlg::OnOK()
{
	if (UpdateData(TRUE) == TRUE)
	{
		DealerFlags.SetIDraughtExportFlag(FALSE);
		DealerFlags.SetBatchExportFlag(FALSE);
		DealerFlags.SetPrinterRedirectMode(0);
		DealerFlags.SetSageExportFlag(FALSE);
		DealerFlags.SetCascadeReportFlag(FALSE);
		DealerFlags.SetCostleyTransactionLogFlag(FALSE);
		DealerFlags.SetWeeklyReportNoZeroSkipFlag(FALSE);
		DealerFlags.SetSortCodeRenameType(0);
		DealerFlags.SetLogFileWritesFlag(FALSE);
		DealerFlags.SetReportCostPriceFlag(0);
		DealerFlags.SetHoltsDiscountFlag(FALSE);
		DealerFlags.SetLocationNotesFlag(FALSE);
		DealerFlags.SetSpawnRepmanFlag(FALSE);
		DealerFlags.Set_LIPA_LJMU_ExportType(0);
		DealerFlags.SetLogNonDuplicatesFlag(FALSE);
		DealerFlags.SetPMSExtendedRoomRateFlag(FALSE);
		DealerFlags.SetLogWebAPICallsFlag(FALSE);
		DealerFlags.SetRemoveLoyaltyTopupFlag(FALSE);
		DealerFlags.SetGlobalPromotionFlag(FALSE);
		DealerFlags.SetPosTraySetupNoLockAllUsersFlag(FALSE);
		DealerFlags.SetReasonTicksNoDownloadFlag(FALSE);
		DealerFlags.SetReasonTicksVoidAfterFlag(FALSE);
		DealerFlags.SetServerHomeLocationFlag(FALSE);
		DealerFlags.SetServerImportExportFlag(FALSE);
		DealerFlags.SetRefundFixFlag(FALSE);
		DealerFlags.SetIgnoreUnpaidTransactionsFlag(FALSE);
		DealerFlags.SetRefund2FixFlag(FALSE);
		DealerFlags.SetGenRecExcludeLoyaltyFlag(FALSE);
		DealerFlags.SetUseDatabaseVATBandFlag(FALSE);
		DealerFlags.SetAllDiscountsVATableFlag(FALSE);
		DealerFlags.Set15DigitKludgeDeptGroupFlag(FALSE);
		DealerFlags.SetGratuityFixFlag(FALSE);
		DealerFlags.SetGratuityFix2Flag(FALSE);
		DealerFlags.SetLinkPluFixFlag(FALSE);
		DealerFlags.SetDuplicateCheckIgnoreTimeFlag(FALSE);
		DealerFlags.SetFixDepositRecycle(FALSE);
		DealerFlags.SetCOFAllDiscountFlag(FALSE);

#ifdef STOCKMAN_SYSTEM
		DealerFlags.SetDorbiereImportFlag(FALSE);
		DealerFlags.SetRebuildStockSalesHistoryFlag(FALSE);
#else
		DealerFlags.SetApparentStockDateFlag(FALSE);
#endif

		CString strHidden;
		m_editHidden.GetWindowText(strHidden);

		int nLength = strHidden.GetLength();

		CString strFlag = "";
		for (int nPos = 0; nPos < nLength; nPos++)
		{
			if (nPos == nLength - 1)
			{
				strFlag += strHidden.GetAt(nPos);
			}
			else
			{
				if (strHidden.Mid(nPos, 2) == "\r\n")
				{
					ProcessFlag(strFlag);
					strFlag = "";
					nPos++;
				}
				else
				{
					strFlag += strHidden.GetAt(nPos);
				}
			}
		}

		ProcessFlag(strFlag);
	}

	EndDialog(IDOK);
}

/**********************************************************************/

void CDefEditHiddenDlg::ProcessFlag(CString& strFlag)
{
	strFlag.MakeUpper();
	if (strFlag == "MISC3")		DealerFlags.SetIDraughtExportFlag(TRUE);
	if (strFlag == "MISC4")		DealerFlags.SetBatchExportFlag(TRUE);
	if (strFlag == "MISC8")		DealerFlags.SetSageExportFlag(TRUE);
	if (strFlag == "MISC9")		DealerFlags.SetCascadeReportFlag(TRUE);
	if (strFlag == "MISC15")	DealerFlags.SetCostleyTransactionLogFlag(TRUE);
	if (strFlag == "MISC16")	DealerFlags.SetWeeklyReportNoZeroSkipFlag(TRUE);
	if (strFlag == "MISC7,1")	DealerFlags.SetPrinterRedirectMode(1);
	if (strFlag == "MISC7,2")	DealerFlags.SetPrinterRedirectMode(2);
	if (strFlag == "MISC17,1")	DealerFlags.SetSortCodeRenameType(1);
	if (strFlag == "MISC17,2")	DealerFlags.SetSortCodeRenameType(2);
	if (strFlag == "MISC18")	DealerFlags.SetLogFileWritesFlag(TRUE);
	if (strFlag == "MISC19")	DealerFlags.SetReportCostPriceFlag(TRUE);
	if (strFlag == "MISC21")	DealerFlags.SetHoltsDiscountFlag(TRUE);
	if (strFlag == "MISC25")	DealerFlags.SetLocationNotesFlag(TRUE);
	if (strFlag == "MISC26")	DealerFlags.SetSpawnRepmanFlag(TRUE);
	if (strFlag == "MISC27")	DealerFlags.Set_LIPA_LJMU_ExportType(1);
	if (strFlag == "MISC27,1")	DealerFlags.Set_LIPA_LJMU_ExportType(1);
	if (strFlag == "MISC27,2")	DealerFlags.Set_LIPA_LJMU_ExportType(2);
	if (strFlag == "MISC28")	DealerFlags.SetLogNonDuplicatesFlag(TRUE);
	if (strFlag == "MISC29")	DealerFlags.SetPMSExtendedRoomRateFlag(TRUE);
	if (strFlag == "MISC30")	DealerFlags.SetLogWebAPICallsFlag(TRUE);
	if (strFlag == "MISC33")	DealerFlags.SetGlobalPromotionFlag(TRUE);
	if (strFlag == "MISC34")	DealerFlags.SetPosTraySetupNoLockAllUsersFlag(TRUE);
	if (strFlag == "MISC35")	DealerFlags.SetReasonTicksNoDownloadFlag(TRUE);
	if (strFlag == "MISC36")	DealerFlags.SetReasonTicksVoidAfterFlag(TRUE);
	if (strFlag == "MISC37")	DealerFlags.SetServerHomeLocationFlag(TRUE);
	if (strFlag == "MISC38")	DealerFlags.SetServerImportExportFlag(TRUE);
	if (strFlag == "MISC39")	DealerFlags.SetRefundFixFlag(TRUE);
	if (strFlag == "MISC40")	DealerFlags.SetIgnoreUnpaidTransactionsFlag(TRUE);
	if (strFlag == "MISC41")	DealerFlags.SetRefund2FixFlag(TRUE);
	if (strFlag == "MISC42")	DealerFlags.SetGenRecExcludeLoyaltyFlag(TRUE);
	if (strFlag == "MISC43")	DealerFlags.SetUseDatabaseVATBandFlag(TRUE);
	if (strFlag == "MISC44")	DealerFlags.SetAllDiscountsVATableFlag(TRUE);
	if (strFlag == "MISC45")	DealerFlags.Set15DigitKludgeDeptGroupFlag(TRUE);
	if (strFlag == "MISC46")	DealerFlags.SetGratuityFixFlag(TRUE);
	if (strFlag == "MISC47")	DealerFlags.SetGratuityFix2Flag(TRUE);
	if (strFlag == "MISC48")	DealerFlags.SetLinkPluFixFlag(TRUE);
	if (strFlag == "MISC49")	DealerFlags.SetDuplicateCheckIgnoreTimeFlag(TRUE);
	if (strFlag == "MISC51")	DealerFlags.SetCOFAllDiscountFlag(TRUE);

#ifdef STOCKMAN_SYSTEM
	if (strFlag == "MISC20")	DealerFlags.SetRebuildStockSalesHistoryFlag(TRUE);
	if (strFlag == "MISC22")	DealerFlags.SetDorbiereImportFlag(TRUE);
#else
	if (strFlag == "MISC23")	DealerFlags.SetApparentStockDateFlag(TRUE);
#endif

	if (strFlag.Left(6) == "MISC50")
	{
		CCSV csv(strFlag);
		DealerFlags.SetFixDepositRecycle(TRUE, csv.GetInt(1), csv.GetInt(2), csv.GetInt64(3));
	}

	if (strFlag.Left(6) == "MISC32")
	{
		CCSV csv(strFlag);
		DealerFlags.SetRemoveLoyaltyTopupFlag(TRUE);
		DealerFlags.ClearNoLoyaltyTopupPLUs();

		for (int i = 1; i < csv.GetSize(); i++)
		{
			DealerFlags.AddNoLoyaltyTopupPLU(csv.GetInt64(i));
			
			if ( DealerFlags.GetNoLoyaltyTopupPLUCount() >= MAX_NOTOPUP_PLU )
			{
				break;
			}
		}
	}
}

/**********************************************************************/
