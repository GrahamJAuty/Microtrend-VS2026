#pragma once
/**********************************************************************/

class CDbExportHelpers
{
public:
	CDbExportHelpers();

public:
	CString GetExportConditionField(CString strKey);

private:
	void AddExportConditionField(CString strKey, CString strValue);

private:
	CMap <CString, LPCTSTR, CString, LPCTSTR> m_ExportConditionFieldMap;
};

/**********************************************************************/
extern CDbExportHelpers DbExportHelpers;
/**********************************************************************/
