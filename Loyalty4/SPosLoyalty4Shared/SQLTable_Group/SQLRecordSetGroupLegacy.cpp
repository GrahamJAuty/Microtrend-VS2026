//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetGroupLegacy.h"
//**********************************************************************

CSQLRecordSetMetadataGroupLegacy::CSQLRecordSetMetadataGroupLegacy() : CSQLRecordSetMetadata()
{
	AddIntField(&m_nGroupNo, Group::GroupNo.Label);
	/*****/
	AddBOOLField(&m_bRevalueFlag[1], Group::LegacyRevalueFlag1.Label, FALSE, TRUE);
	AddBOOLField(&m_bRevalueDisable[1], Group::LegacyRevalueDisable1.Label, FALSE, TRUE);
	AddIntField(&m_nRevalueType[1], Group::LegacyRevalueType1.Label, 1, TRUE);
	AddDecimalField(&m_strRevalueTrip[1], Group::LegacyRevalueTrip1.Label, "0.00", TRUE);
	AddIntField(&m_nRevaluePoints[1], Group::LegacyRevaluePoints1.Label, 0, TRUE);
	AddDecimalField(&m_strRevalueBonus[1], Group::LegacyRevalueBonus1.Label, "0.00", TRUE);
	AddIntField(&m_nRevaluePercent[1], Group::LegacyRevaluePercent1.Label, 0, TRUE);
	/*****/
	AddBOOLField(&m_bRevalueFlag[2], Group::LegacyRevalueFlag2.Label, FALSE, TRUE);
	AddBOOLField(&m_bRevalueDisable[2], Group::LegacyRevalueDisable2.Label, FALSE, TRUE);
	AddIntField(&m_nRevalueType[2], Group::LegacyRevalueType2.Label, 1, TRUE);
	AddDecimalField(&m_strRevalueTrip[2], Group::LegacyRevalueTrip2.Label, "0.00", TRUE);
	AddIntField(&m_nRevaluePoints[2], Group::LegacyRevaluePoints2.Label, 0, TRUE);
	AddDecimalField(&m_strRevalueBonus[2], Group::LegacyRevalueBonus2.Label, "0.00", TRUE);
	AddIntField(&m_nRevaluePercent[2], Group::LegacyRevaluePercent2.Label, 0, TRUE);
	/*****/
	AddBOOLField(&m_bRevalueFlag[3], Group::LegacyRevalueFlag3.Label, FALSE, TRUE);
	AddBOOLField(&m_bRevalueDisable[3], Group::LegacyRevalueDisable3.Label, FALSE, TRUE);
	AddIntField(&m_nRevalueType[3], Group::LegacyRevalueType3.Label, 1, TRUE);
	AddDecimalField(&m_strRevalueTrip[3], Group::LegacyRevalueTrip3.Label, "0.00", TRUE);
	AddIntField(&m_nRevaluePoints[3], Group::LegacyRevaluePoints3.Label, 0, TRUE);
	AddDecimalField(&m_strRevalueBonus[3], Group::LegacyRevalueBonus3.Label, "0.00", TRUE);
	AddIntField(&m_nRevaluePercent[3], Group::LegacyRevaluePercent3.Label, 0, TRUE);
	/*****/
	AddBOOLField(&m_bRevalueFlag[4], Group::LegacyRevalueFlag4.Label, FALSE, TRUE);
	AddBOOLField(&m_bRevalueDisable[4], Group::LegacyRevalueDisable4.Label, FALSE, TRUE);
	AddIntField(&m_nRevalueType[4], Group::LegacyRevalueType4.Label, 1, TRUE);
	AddDecimalField(&m_strRevalueTrip[4], Group::LegacyRevalueTrip4.Label, "0.00", TRUE);
	AddIntField(&m_nRevaluePoints[4], Group::LegacyRevaluePoints4.Label, 0, TRUE);
	AddDecimalField(&m_strRevalueBonus[4], Group::LegacyRevalueBonus4.Label, "0.00", TRUE);
	AddIntField(&m_nRevaluePercent[4], Group::LegacyRevaluePercent4.Label, 0, TRUE);
}

//**********************************************************************

CSQLRecordSetGroupLegacy::CSQLRecordSetGroupLegacy(CDatabase* pDatabase, RSParams_GroupLegacy_NormalNoParams params) :
	CSQLRecordSetConnectionPool(pDatabase),
	m_MD()
{
	m_nFields = m_MD.GetFieldCount();
	m_nParams = m_MD.GetParamCount();
	SetMetadata(&m_MD);
	SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetGroupLegacy::GetDefaultSQL()
{ 
    return SQLTableNames::AccountGroups; 
}

//**********************************************************************

void CSQLRecordSetGroupLegacy::LoadFromGroupRow(CSQLRowGroupLegacy& RowGroup)
{
    m_MD.m_nGroupNo = RowGroup.GetGroupNo();
    /*****/
    for (int n = 1; n <= LEGACY_REVALUE_PLUNO_COUNT; n++)
    {
		m_MD.m_bRevalueFlag[n] = RowGroup.GetRevalueFlag(n);
		m_MD.m_bRevalueDisable[n] = RowGroup.GetRevalueDisable(n);
		m_MD.m_nRevalueType[n] = RowGroup.GetRevalueType(n);
		SetDoubleString(m_MD.m_strRevalueTrip[n], RowGroup.GetRevalueTrip(n));
		m_MD.m_nRevaluePoints[n] = RowGroup.GetRevaluePoints(n);
		SetDoubleString(m_MD.m_strRevalueBonus[n], RowGroup.GetRevalueBonus(n));
		m_MD.m_nRevaluePercent[n] = RowGroup.GetRevaluePercent(n);
    } 
}

//**********************************************************************

void CSQLRecordSetGroupLegacy::SaveToGroupRow(CSQLRowGroupLegacy& RowGroup)
{
    RowGroup.SetGroupNo(m_MD.m_nGroupNo);
    /*****/
    for (int n = 1; n <= LEGACY_REVALUE_PLUNO_COUNT; n++)
    {
        RowGroup.SetRevalueFlag(n, m_MD.m_bRevalueFlag[n]);
        RowGroup.SetRevalueDisable(n, m_MD.m_bRevalueDisable[n]);
        RowGroup.SetRevalueType(n, m_MD.m_nRevalueType[n]);
        RowGroup.SetRevalueTrip(n, atof(m_MD.m_strRevalueTrip[n]));
        RowGroup.SetRevaluePoints(n, m_MD.m_nRevaluePoints[n]);
        RowGroup.SetRevalueBonus(n, atof(m_MD.m_strRevalueBonus[n]));
        RowGroup.SetRevaluePercent(n, m_MD.m_nRevaluePercent[n]);
    }
}

//**********************************************************************

bool CSQLRecordSetGroupLegacy::StepSelectAll(CSQLRowGroupLegacy& RowGroup)
{
    if (IsOpen() == FALSE)
    {
        if (Open(CRecordset::forwardOnly, NULL, CRecordset::readOnly) == FALSE)
        {
            return FALSE;
        }
    }

    if (IsEOF() == TRUE)
    {
        return FALSE;
    }

    SaveToGroupRow(RowGroup);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
