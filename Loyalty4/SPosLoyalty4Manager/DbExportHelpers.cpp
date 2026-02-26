//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
//**********************************************************************
#include "DbExportHandler.h"
//**********************************************************************
#include "DbExportHelpers.h"
//**********************************************************************

CDbExportHelpers::CDbExportHelpers()
{
	m_ExportConditionFieldMap.InitHashTable(103);

	AddExportConditionField(szPURSE1, Account::Purse1Balance.Label);
	AddExportConditionField(szPURSE2, Account::Purse2Balance.Label);
	AddExportConditionField(szPOINTS, Account::Points.Label);
	AddExportConditionField(szPURSE1SPENDTD, Account::Purse1SpendToDate.Label);
	AddExportConditionField(szPURSE2SPENDTD, Account::Purse2SpendToDate.Label);
	AddExportConditionField(szCASHSPENDTD, Account::CashSpendToDate.Label);
	AddExportConditionField(szTOTALSPENDTD, Account::TotalSpendToDate.Label);
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
