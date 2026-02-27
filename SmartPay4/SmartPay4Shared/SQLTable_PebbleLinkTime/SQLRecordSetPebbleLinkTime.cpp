//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetPebbleLinkTime.h"
//**********************************************************************

CSQLRecordSetMetadataPebbleLinkTime::CSQLRecordSetMetadataPebbleLinkTime(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddInt64Field(&m_nLineID, PebbleLinkTime::LineID.Label);
        AddTextField(&m_strDate, PebbleLinkTime::Date.Label);
		AddTextField(&m_strTime, PebbleLinkTime::Time.Label);
        AddInt64Field(&m_nUserID, PebbleLinkTime::UserID.Label);
        AddTextField(&m_strUsername, PebbleLinkTime::Username.Label);
        AddTextField(&m_strForename, PebbleLinkTime::Forename.Label);
        AddTextField(&m_strSurname, PebbleLinkTime::Surname.Label);
		AddIntField(&m_nGroupNo, PebbleLinkTime::GroupNo.Label);
        AddTextField(&m_strPebbleFirstName, PebbleLinkTime::PebbleFirstName.Label);
		AddTextField(&m_strPebbleLastName, PebbleLinkTime::PebbleLastName.Label);
        AddIntField(&m_nProcessType, PebbleLinkTime::ProcessType.Label);
        AddIntField(&m_nMatchType, PebbleLinkTime::MatchType.Label);
        AddTextField(&m_strMIS, PebbleLinkTime::MIS.Label);
        AddTextField(&m_strUPN, PebbleLinkTime::UPN.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataPebbleLinkTime::AddParamsByLineID(const CString strLineID)
{
    AddTextParam(PebbleLinkTime::LineID.Label, strLineID);
}

//**********************************************************************

void CSQLRecordSetMetadataPebbleLinkTime::AddTextParam(CString strLabel, const CString strDef)
{
    CSQLRecordSetMetadata::AddTextParam(strLabel, strDef);
}

//**********************************************************************

void CSQLRecordSetMetadataPebbleLinkTime::AddIntParam(CString strLabel, const int nDef)
{
    CSQLRecordSetMetadata::AddIntParam(strLabel, nDef);
}

//**********************************************************************

CSQLRecordSetPebbleLinkTime::CSQLRecordSetPebbleLinkTime(CDatabase* pDatabase, RSParams_PebbleLinkTime_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
	m_MD()
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetPebbleLinkTime::CSQLRecordSetPebbleLinkTime(CDatabase* pDatabase, RSParams_PebbleLinkTime_NormalByLineId params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
	m_MD.AddParamsByLineID(params.strLineId);
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetPebbleLinkTime::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetPebbleLinkTime::GetDefaultSQL()
{
    return SQLTableNames::PebbleLinkTimes;
}

//**********************************************************************

void CSQLRecordSetPebbleLinkTime::LoadFromPebbleLinkTimeRow(CSQLRowPebbleLinkTime& RowBC)
{
    m_MD.m_nLineID = RowBC.GetLineID();
    m_MD.m_strDate = RowBC.GetDate();
	m_MD.m_strTime = RowBC.GetTime();
    m_MD.m_nUserID = RowBC.GetUserID();
    m_MD.m_strUsername = RowBC.GetUsername();
    m_MD.m_strForename = RowBC.GetForename();
    m_MD.m_strSurname = RowBC.GetSurname();
	m_MD.m_nGroupNo = RowBC.GetGroupNo();
    m_MD.m_strPebbleFirstName = RowBC.GetPebbleFirstName();
	m_MD.m_strPebbleLastName = RowBC.GetPebbleLastName();
    m_MD.m_nProcessType = RowBC.GetProcessType();
    m_MD.m_nMatchType = RowBC.GetMatchType();
    m_MD.m_strMIS = RowBC.GetMIS();
    m_MD.m_strUPN = RowBC.GetUPN();
}

//**********************************************************************

void CSQLRecordSetPebbleLinkTime::SaveToPebbleLinkTimeRow(CSQLRowPebbleLinkTime& RowBC)
{
    RowBC.SetLineID(m_MD.m_nLineID);
    RowBC.SetDate(m_MD.m_strDate);
	RowBC.SetTime(m_MD.m_strTime);
    RowBC.SetUserID(m_MD.m_nUserID);
    RowBC.SetUsername(m_MD.m_strUsername);
    RowBC.SetForename(m_MD.m_strForename);
    RowBC.SetSurname(m_MD.m_strSurname);
	RowBC.SetGroupNo(m_MD.m_nGroupNo);
	RowBC.SetPebbleFirstName(m_MD.m_strPebbleFirstName);
	RowBC.SetPebbleLastName(m_MD.m_strPebbleLastName);
	RowBC.SetProcessType(m_MD.m_nProcessType);
	RowBC.SetMatchType(m_MD.m_nMatchType);
    RowBC.SetMIS(m_MD.m_strMIS);
    RowBC.SetUPN(m_MD.m_strUPN);
}

//**********************************************************************

bool CSQLRecordSetPebbleLinkTime::StepSelectAll(CSQLRowPebbleLinkTime& RowBC)
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

    SaveToPebbleLinkTimeRow(RowBC);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
