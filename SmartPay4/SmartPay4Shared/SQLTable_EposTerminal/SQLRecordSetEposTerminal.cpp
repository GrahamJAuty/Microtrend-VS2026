//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetEposTerminal.h"
//**********************************************************************

CSQLRecordSetMetadataEposTerminal::CSQLRecordSetMetadataEposTerminal(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
	if (FALSE == bForExistenceCheck)
	{
		AddIntField(&m_nTerminalNo, EposTerminal::TerminalNo.Label, 1);
		AddIntField(&m_nTerminalType, EposTerminal::TerminalType.Label);
		AddTextField(&m_strTerminalName, EposTerminal::TerminalName.Label);
		AddTextField(&m_strLastUsedDate, EposTerminal::LastUsedDate.Label);
		AddTextField(&m_strLastUsedTime, EposTerminal::LastUsedTime.Label);
		AddIntField(&m_nCCNo, EposTerminal::CCNo.Label);
		AddInt64Field(&m_nUserID, EposTerminal::UserID.Label, -1);
		AddDecimalField(&m_strCardValue, EposTerminal::CardValue.Label);
		AddIntField(&m_nTotalCounter, EposTerminal::TotalCounter.Label);
		AddDecimalField(&m_strTotalInDrawer, EposTerminal::TotalInDrawer.Label);
		AddBOOLField(&m_bDisable, EposTerminal::Disabled.Label, FALSE);
		AddBOOLField(&m_bTerminalOn, EposTerminal::TerminalOn.Label, FALSE);
		AddBOOLField(&m_bInService, EposTerminal::InService.Label, FALSE);
		AddBOOLField(&m_bDoorOpen, EposTerminal::DoorOpen.Label, FALSE);
	}
}

//**********************************************************************

void CSQLRecordSetMetadataEposTerminal::AddParamsByTerminalNo(const int nTerminalNo)
{
	AddIntParam(EposTerminal::TerminalNo.Label, nTerminalNo);
}

//**********************************************************************

CSQLRecordSetEposTerminal::CSQLRecordSetEposTerminal(CDatabase* pDatabase, RSParams_EposTerminal_NormalNoParams params) :
	CSQLRecordSetConnectionPool( pDatabase),
	m_MD()
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetEposTerminal::CSQLRecordSetEposTerminal(CDatabase* pDatabase, RSParams_EposTerminal_NormalByTerminalNo params, bool bForExistenceCheck) :
	CSQLRecordSetConnectionPool(pDatabase),
	m_MD(bForExistenceCheck)
{
	m_MD.AddParamsByTerminalNo(params.nTerminalNo);
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetEposTerminal::CommonInit()
{
	m_nFields = m_MD.GetFieldCount();
	m_nParams = m_MD.GetParamCount();
	SetMetadata(&m_MD);
	SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetEposTerminal::GetDefaultSQL()
{
    return SQLTableNames::EposTerminals;
}

//**********************************************************************

void CSQLRecordSetEposTerminal::LoadFromEposTerminalRow(CSQLRowEposTerminal& RowEP)
{
    m_MD.m_nTerminalNo = RowEP.GetTerminalNo();
	m_MD.m_nTerminalType = RowEP.GetTerminalType();
	m_MD.m_strTerminalName = RowEP.GetTerminalName();
	m_MD.m_strLastUsedDate = RowEP.GetLastUsedDate();
	m_MD.m_strLastUsedTime = RowEP.GetLastUsedTime();
	m_MD.m_nCCNo = RowEP.GetCCNo();
	m_MD.m_nUserID = RowEP.GetUserID();
	SetDoubleString(m_MD.m_strCardValue, RowEP.GetCardValue());
	m_MD.m_nTotalCounter = RowEP.GetTotalCounter();
	SetDoubleString(m_MD.m_strTotalInDrawer, RowEP.GetTotalInDrawer());
	m_MD.m_bDisable = RowEP.GetDisableFlag();
	m_MD.m_bTerminalOn = RowEP.GetTerminalOnFlag();
	m_MD.m_bInService = RowEP.GetInServiceFlag();
	m_MD.m_bDoorOpen = RowEP.GetDoorOpenFlag();
}

//**********************************************************************

void CSQLRecordSetEposTerminal::SaveToEposTerminalRow(CSQLRowEposTerminal& RowEP)
{
    RowEP.SetTerminalNo(m_MD.m_nTerminalNo);
    RowEP.SetTerminalType(m_MD.m_nTerminalType);
	RowEP.SetTerminalName(m_MD.m_strTerminalName);
	RowEP.SetLastUsedDate(m_MD.m_strLastUsedDate);
	RowEP.SetLastUsedTime(m_MD.m_strLastUsedTime);
	RowEP.SetCCNo(m_MD.m_nCCNo);
	RowEP.SetUserID(m_MD.m_nUserID);
	RowEP.SetCardValue(atof(m_MD.m_strCardValue));
	RowEP.SetTotalCounter(m_MD.m_nTotalCounter);
	RowEP.SetTotalInDrawer(atof(m_MD.m_strTotalInDrawer));
	RowEP.SetDisableFlag(m_MD.m_bDisable);
	RowEP.SetTerminalOnFlag(m_MD.m_bTerminalOn);
	RowEP.SetInServiceFlag(m_MD.m_bInService);
	RowEP.SetDoorOpenFlag(m_MD.m_bDoorOpen);
}

//**********************************************************************

bool CSQLRecordSetEposTerminal::StepSelectAll(CSQLRowEposTerminal& RowEP)
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

    SaveToEposTerminalRow(RowEP);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
