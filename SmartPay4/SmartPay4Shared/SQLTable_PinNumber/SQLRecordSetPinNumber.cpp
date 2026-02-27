//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetPINNumber.h"
//**********************************************************************

CSQLRecordSetMetadataPINNumber::CSQLRecordSetMetadataPINNumber(bool bForExistenceCheck)
{
    if (FALSE == bForExistenceCheck)
    {
		AddFixLenTextField(&m_strPINNumber, 4, PINNumber::PINNumber.Label);
        AddTextField(&m_strUserID, PINNumber::UserID.Label);
        AddTextField(&m_strUsername, PINNumber::Username.Label);
        AddTextField(&m_strFirstDate, PINNumber::FirstDate.Label);
        AddTextField(&m_strFirstTime, PINNumber::FirstTime.Label);
        AddTextField(&m_strLastDate, PINNumber::LastDate.Label);
        AddTextField(&m_strLastTime, PINNumber::LastTime.Label);
        AddIntField(&m_nUseCounter, PINNumber::UseCounter.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataPINNumber::AddParamsByPINNumber(const CString strPINNumber)
{
    AddFixLenTextParam(PINNumber::PINNumber.Label, 4, strPINNumber);
}

//**********************************************************************

void CSQLRecordSetMetadataPINNumber::AddParamsByUserID(const CString strUserID)
{
    AddTextParam(PINNumber::UserID.Label, strUserID);
}

//**********************************************************************

void CSQLRecordSetMetadataPINNumber::AddParamsByRange(const CString strPINNumberFrom, const CString strPINNumberTo)
{
    AddParamsByPINNumber(strPINNumberFrom);
    AddFixLenTextParam(PINNumber::PINNumber.Label, 4, strPINNumberTo);
}

//**********************************************************************

CSQLRecordSetPINNumber::CSQLRecordSetPINNumber(CDatabase* pDatabase, RSParams_PINNumber_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetPINNumber::CSQLRecordSetPINNumber(CDatabase* pDatabase, RSParams_PINNumber_NormalByPINNumber params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
    m_MD.AddParamsByPINNumber(params.strPINNumber);
	CommonInit();
}

//**********************************************************************

CSQLRecordSetPINNumber::CSQLRecordSetPINNumber(CDatabase* pDatabase, RSParams_PINNumber_NormalByUserID params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
    m_MD.AddParamsByUserID(params.strUserID);
    CommonInit();
}

//**********************************************************************

CSQLRecordSetPINNumber::CSQLRecordSetPINNumber(CDatabase* pDatabase, RSParams_PINNumber_CountRowsByRange params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(TRUE)
{
    m_bRangeFilter = TRUE;
    m_MD.AddParamsByRange(params.strPINNumberFrom, params.strPINNumberTo);
    CommonInit();
}

//**********************************************************************

void CSQLRecordSetPINNumber::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetPINNumber::GetDefaultSQL()
{
    return SQLTableNames::PINNumber; 
}

//**********************************************************************

void CSQLRecordSetPINNumber::LoadFromPINNumberRow(CSQLRowPINNumber& RowPINNumber)
{
    m_MD.m_strPINNumber = RowPINNumber.GetPINNumber();
    m_MD.m_strUserID = RowPINNumber.GetUserID();
    m_MD.m_strUsername = RowPINNumber.GetUsername();
    m_MD.m_strFirstDate = RowPINNumber.GetFirstDate();
    m_MD.m_strFirstTime = RowPINNumber.GetFirstTime();
    m_MD.m_strLastDate = RowPINNumber.GetLastDate();
    m_MD.m_strLastTime = RowPINNumber.GetLastTime();
    m_MD.m_nUseCounter = RowPINNumber.GetUseCounter();
}

//**********************************************************************

void CSQLRecordSetPINNumber::SaveToPINNumberRow(CSQLRowPINNumber& RowPINNumber)
{         
    RowPINNumber.SetPINNumber(m_MD.m_strPINNumber);
    RowPINNumber.SetUserID(m_MD.m_strUserID);
    RowPINNumber.SetUsername(m_MD.m_strUsername);
    RowPINNumber.SetFirstDate(m_MD.m_strFirstDate);
    RowPINNumber.SetFirstTime(m_MD.m_strFirstTime);
    RowPINNumber.SetLastDate(m_MD.m_strLastDate);
    RowPINNumber.SetLastTime(m_MD.m_strLastTime);
    RowPINNumber.SetUseCounter(m_MD.m_nUseCounter);
}

//**********************************************************************

bool CSQLRecordSetPINNumber::StepSelectAll(CSQLRowPINNumber& RowPINNumber)
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

    SaveToPINNumberRow(RowPINNumber);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
