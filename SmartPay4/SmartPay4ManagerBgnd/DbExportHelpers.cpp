//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "DbExportHandler.h"
//**********************************************************************
#include "DbExportHelpers.h"
//**********************************************************************

CDbExportHelpers::CDbExportHelpers()
{
	m_ExportConditionFieldMap.InitHashTable(103);

	CString strPurse1Balance = "Purse1Balance";
	CString strPurse3Balance = "Purse3Balance";
	
	AddExportConditionField(szPURSE1_BALANCE, strPurse1Balance);
	AddExportConditionField("Purse1", strPurse1Balance);
	AddExportConditionField(szPURSE1_CREDIT, Account::Purse1Credit.Label);
	AddExportConditionField(szPURSE1_LIABILITY, Account::Purse1Liability.Label);

	AddExportConditionField("Purse2", Account::Purse2Balance.Label);
	AddExportConditionField(szPURSE2_BALANCE, Account::Purse2Balance.Label);

	AddExportConditionField(szPURSE3_BALANCE, strPurse3Balance);
	AddExportConditionField("Purse3", strPurse3Balance);
	AddExportConditionField(szPURSE3_CREDIT, Account::Purse3Credit.Label);
	AddExportConditionField(szPURSE3_LIABILITY, Account::Purse3Liability.Label);

	AddExportConditionField(szPOINTS, Account::Points.Label);
	AddExportConditionField(szCASHSPENDTD, Account::CashSpendToDate.Label);
	AddExportConditionField(szPURSE1SPENDTD, Account::Purse1SpendToDate.Label);
	AddExportConditionField(szPURSE2SPENDTD, Account::Purse2SpendToDate.Label);
	AddExportConditionField(szPURSE3SPENDTD, Account::Purse3SpendToDate.Label);
	AddExportConditionField(szTOTALSPENDTD, "TotalSpendToDate");
	AddExportConditionField(szPOINTSTD, Account::PointsToDate.Label);
}

//**********************************************************************

void CDbExportHelpers::AddExportConditionField(CString strKey, CString strValue)
{
	strKey.MakeUpper();
	m_ExportConditionFieldMap.SetAt(strKey, strValue);
}

//**********************************************************************

CString CDbExportHelpers::GetExportConditionField(CString strKey)
{
	CString strResult = "";

	strKey.MakeUpper();

	int nIndex = 0;
	if (m_ExportConditionFieldMap.Lookup(strKey, strResult) == 0)
	{
		strResult = "";
	}
	
	return strResult;
}

//**********************************************************************
