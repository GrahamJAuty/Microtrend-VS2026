//*******************************************************
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRowGroupFull.h"
#include "..\SmartPay4Shared\SQLWhereBuilder.h"
//*******************************************************
#include "..\SmartPay4ManagerBgnd\DbExportHandler.h"
//*******************************************************
#include "StandardCombos.h"
//*******************************************************

CStandardCombos::CStandardCombos(void)
{
}

//*******************************************************

void CStandardCombos::AddGenderTexts( CSSComboBox& combo )
{
	combo.ResetContent();
	combo.AddString( szUNKNOWN );
	combo.AddString( szMALE );
	combo.AddString( szFEMALE );
}

//*******************************************************
//*******************************************************
//*******************************************************

void CStandardCombos::FillGenderCombo( CSSComboBox& combo, int nSel )
{
	AddGenderTexts( combo );
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::FillAutoRefreshCombo( CSSComboBox& combo, int nSel )
{
	combo.ResetContent();
	combo.AddString( "Daily" );
	combo.AddString( "Weekly" );
	combo.AddString( "Monthly" );
	combo.AddString( "Yearly" );
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::FillGroupMaxSpendCombo( CSSComboBox& combo, int nMaxSpendType )
{
	combo.ResetContent();
	combo.AddString( "Single Daily Limit" );
	combo.AddString( "Use Audit Periods" );
	
	int nSel = 0;
	switch( nMaxSpendType )
	{
	case nGRPSPENDTYPE_PERIOD_LOCKED:
	case nGRPSPENDTYPE_PERIOD_RECORD1:
	case nGRPSPENDTYPE_PERIOD_RECORD2:
		nSel = 1;
		break;
	}
	
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::FillGroupMaxSpendOverrideCombo( CSSComboBox& combo, int nMaxSpendType )
{
	combo.ResetContent();
	combo.AddString( "Never" );
	combo.AddString( "Always" );
	combo.AddString( "Set by Account" );
	
	int nSel = 0;
	switch( nMaxSpendType )
	{
	case nGRPSPENDTYPE_DAILY_RECORD1:
	case nGRPSPENDTYPE_PERIOD_RECORD1:
		nSel = 1;
		break;
	
	case nGRPSPENDTYPE_DAILY_RECORD2:
	case nGRPSPENDTYPE_PERIOD_RECORD2:
		nSel = 2;
		break;
	}
	
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::FillFieldNamesCombo( CSSComboBox& combo, int nSel )
{
	combo.ResetContent();
	combo.AddString( "Database Fields" );
	combo.AddString( "EOD Texts" );
	combo.AddString( "Audit Periods" );
	combo.AddString( "Alert Texts" );
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::FillWeekdayCombo( CSSComboBox& combo, int nSel )
{
	combo.ResetContent();
	combo.AddString( "Sunday" );
	combo.AddString( "Monday" );
	combo.AddString( "Tuesday" );
	combo.AddString( "Wednesday" );
	combo.AddString( "Thursday" );
	combo.AddString( "Friday" );
	combo.AddString( "Saturday" );
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::FillFormatCombo( CSSComboBox& combo, int nSel )
{
	combo.ResetContent();
	combo.AddString( "Format1" );
	combo.AddString( "Format2" );
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::FillBioRegisterCombo( CSSComboBox& combo, int nSel )
{
	combo.ResetContent();
	combo.AddString( "No" );
	combo.AddString( "Include if found" );
	combo.AddString( "Exclude if found" );
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::FillServiceProviderCombo( CSSTabbedComboBox& combo, int nSel )
{
	combo.ResetContent();

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(100);
	combo.SetTabStops(arrayStops);

	combo.ResetContent();
	combo.AddItem( "None" );
	combo.AddItem( "ParentPay" );
	combo.AddItem( "Pebble,(Arbor)");
	combo.AddItem( "BromCom");
	combo.AddItem( "Tucasi,(SOAP)" );
	combo.AddItem( "Tucasi,(JSON)");
	combo.AddItem( "WisePay" );
	combo.AddItem( "Schoolcomms" );
	combo.AddItem( "sQuid" );
	combo.AddItem( "ParentMail,(+Pay)" );
	
	combo.SetItemData(0, nWEBPAYMENT_NONE);
	combo.SetItemData(1, nWEBPAYMENT_PARENTPAY);
	combo.SetItemData(2, nWEBPAYMENT_PEBBLE);
	combo.SetItemData(3, nWEBPAYMENT_BROMCOM);
	combo.SetItemData(4, nWEBPAYMENT_TUCASI);
	combo.SetItemData(5, nWEBPAYMENT_TUCASIv2);
	combo.SetItemData(6, nWEBPAYMENT_WISEPAY);
	combo.SetItemData(7, nWEBPAYMENT_SCHOOLCOMMS);
	combo.SetItemData(8, nWEBPAYMENT_SQUID);
	combo.SetItemData(9, nWEBPAYMENT_PARENTMAIL);
	
	SetComboSel(combo, nSel);
}

//*******************************************************

void CStandardCombos::FillDatabaseImportCombo( CSSComboBox& combo, int nSel )
{
	combo.ResetContent();
	combo.AddString( "Standard CSV" );
	combo.AddString( "SIMS Interface" );
	combo.AddString( "Custom CSV" );
	combo.AddString( "Wonde (JSON)" );
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::FillInterfaceCombo( CSSComboBox& combo, int nSel )
{
	combo.ResetContent();
	combo.AddString( "Biometric" );
	combo.AddString( "MiFare-1" );
	combo.AddString( "MiFare-2" );
	combo.AddString( "Dual" );
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::FillBackgroundOptionsCombo( CSSComboBox& combo, int nSel )
{
	combo.ResetContent();
	combo.AddString( "No" );
	combo.AddString( "Yes" );
	combo.AddString( "Auto Start" );
	combo.AddString( "Auto Stop" );
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::FillExtRefCombo( CSSComboBox& combo, int nSel )
{
	combo.ResetContent();
	combo.AddString( "MiFare" );
	combo.AddString( "Paxton" );
	combo.AddString( "Innerrange" );
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::FillDefaultExtRefCombo( CSSComboBox& combo, int nSel )
{
	combo.ResetContent();
	combo.AddString( "Ext Ref1" );
	combo.AddString( "Ext Ref2" );
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::FillOutputTypeCombo( CSSComboBox& combo, int nSel )
{
	combo.ResetContent();
	combo.AddString( "None" );
	combo.AddString( "CSV" );
	combo.AddString( "HTML" );
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::FillSortOnCombo( CSSComboBox& combo, int nSel )
{
	combo.ResetContent();
	combo.AddString( "Surname" );
	combo.AddString( "User ID" );
	combo.AddString( "User Name" );
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::FillPaymentOrderCombo( CSSComboBox& combo, int nSel )
{
	combo.ResetContent();
	combo.AddString( "Date" );
	combo.AddString( "Type" );
	combo.AddString( "User ID" );
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::FillRefreshPurseCombo( CSSComboBox& combo, int nSel )
{
	combo.ResetContent();
	combo.AddString( "Disabled" );
	combo.AddString( "Refresh Daily" );
	combo.AddString( "Refresh Weekly" );
	combo.AddString( "Refresh Monthly" );
	combo.AddString( "Refresh Yearly" );
	combo.AddString( "Topup Manual" );
	combo.AddString( "Period Refresh" );
	combo.AddString( "EOD Refresh");
	SetComboSel( combo, nSel );
}

//*******************************************************

void CStandardCombos::SetComboSel( CSSComboBox& combo, int nSel, int nDefSel )
{
	if ((nSel >= 0) && (nSel < combo.GetCount()))
	{
		combo.SetCurSel(nSel);
	}
	else if ((nDefSel >= 0) && (nDefSel < combo.GetCount()))
	{
		combo.SetCurSel(nDefSel);
	}
}

//*******************************************************

void CStandardCombos::SetComboSel(CSSTabbedComboBox& combo, int nSel, int nDefSel)
{
	if ((nSel >= 0) && (nSel < combo.GetCount()))
	{
		combo.SetCurSel(nSel);
	}
	else if ((nDefSel >= 0) && (nDefSel < combo.GetCount()))
	{
		combo.SetCurSel(nDefSel);
	}
}

//*******************************************************
//*******************************************************
//*******************************************************

void CStandardCombos::FillGenderCombo( CSSComboBox& combo, const char* sz )
{
	AddGenderTexts( combo );
	SelectComboString( combo, sz );
}

//*******************************************************

void CStandardCombos::FillGenderConditionCombo( CSSComboBox& combo, const char* sz )
{
	combo.ResetContent();
	combo.AddString( szDONTCARE );
	combo.AddString( szMALE );
	combo.AddString( szFEMALE );
	combo.AddString( szUNKNOWN );
	SelectComboString( combo, sz, 0 );
}

//*******************************************************

void CStandardCombos::FillAndOrCombo( CSSComboBox& combo, const char* sz )
{
	combo.ResetContent();
	combo.AddString( szAND );
	combo.AddString( szOR );
	SelectComboString( combo, sz, 0 );
}

//*******************************************************

void CStandardCombos::FillConditionCombo( CSSComboBox& combo, const char* sz )
{
	combo.ResetContent();
	combo.AddString( szDONTCARE );
	combo.AddString( szLESSTHAN );
	combo.AddString( szGREATER );
	combo.AddString( szEQUAL );
	combo.AddString( szNOTEQUAL );
	SelectComboString( combo, sz, 0 );
}

//*******************************************************

void CStandardCombos::FillExportFieldCombo(CSSComboBox& combo, const char* sz)
{
	combo.ResetContent();
	combo.AddString(szPURSE1_BALANCE);
	combo.AddString(szPURSE1_LIABILITY);
	combo.AddString(szPURSE1_CREDIT);
	combo.AddString(szPURSE2_BALANCE);
	combo.AddString(szPURSE3_BALANCE);
	combo.AddString(szPURSE3_LIABILITY);
	combo.AddString(szPURSE3_CREDIT);
	combo.AddString(szPOINTS);
	combo.AddString(szCASHSPENDTD);
	combo.AddString(szPURSE1SPENDTD);
	combo.AddString(szPURSE2SPENDTD);
	combo.AddString(szPURSE3SPENDTD);
	combo.AddString(szTOTALSPENDTD);
	combo.AddString(szPOINTSTD);
	SelectComboString(combo, sz, 0);
}

//*******************************************************

void CStandardCombos::FillBaudCombo( CSSComboBox& combo, const char* sz )
{
	combo.ResetContent();
	combo.AddString( "9600" );
	combo.AddString( "19200" );
	combo.AddString( "38400" );
	combo.AddString( "57600" );
	SelectComboString( combo, sz );
}

//*******************************************************

void CStandardCombos::FillSoapCombo( CSSComboBox& combo, const char* sz )
{
	combo.ResetContent();
	combo.AddString( "1.1" );
	combo.AddString( "1.2" );
	SelectComboString( combo, sz );
}

//*******************************************************

void CStandardCombos::FillPortCombo( CSSComboBox& combo, const char* sz )
{
	combo.ResetContent();
	
	for ( int n = 1; n <= 16; n++ )
	{
		CString str;
		str.Format( "COM%d", n );
		combo.AddString( str );
	}

	SelectComboString( combo, sz );
}

//*******************************************************

void CStandardCombos::SelectComboString( CSSComboBox& combo, const char* sz, int nDefSel )
{
	if (combo.SelectString(-1, sz) == CB_ERR)
	{
		if ((nDefSel >= 0) && (nDefSel < combo.GetCount()))
		{
			combo.SetCurSel(nDefSel);
		}
		else
		{
			combo.SetWindowText(sz);
		}
	}
}

//*******************************************************

void CStandardCombos::SelectComboStringWithDefaultPos(CSSComboBox& combo, const char* sz, int nPos)
{
	if (combo.SelectString(-1, sz) == CB_ERR)
	{
		if ((nPos >= 0) && (nPos < combo.GetCount()))
		{
			combo.SetCurSel(nPos);
		}
		else if (combo.GetCount() > 0)
		{
			combo.SetCurSel(0);
		}
	}
}

//*******************************************************

int CStandardCombos::GetItemDataOfSelection(CSSTabbedComboBox& combo, int nDefault)
{
	int nResult = nDefault;
	
	int nSel = combo.GetCurSel();
	if ((nSel >= 0) && (nSel <= combo.GetCount()))
	{
		nResult = combo.GetItemData(nSel);
	}

	return nResult;
}

//*******************************************************

void CStandardCombos::FillFolderSetCombo(CSSTabbedComboBox& combo, int nTargetSetNo)
{
	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(50);
	combo.SetTabStops(arrayStops);

	combo.ResetContent();
	combo.AddItem("--,All");
	combo.SetItemData(0, 0);

	for (int nSetNo = 1; nSetNo <= EPOSREQPATH_COUNT; nSetNo++)
	{
		CString strName = Server.GetEposPathLabel(nSetNo);
		TrimSpacesFromString(strName);

		if (strName == "")
		{
			strName.Format("Set %d", nSetNo);
		}

		CString strComboLine = "";
		strComboLine.Format("%2.2d,%s",
			nSetNo,
			(const char*)strName);

		combo.AddItem(strComboLine);
		combo.SetItemData(nSetNo, nSetNo);
	}

	int nSel = nTargetSetNo;

	if ((nSel < 0) || (nSel >= combo.GetCount()))
	{
		nSel = 0;
	}

	combo.SetCurSel(nSel);
}

//*******************************************************