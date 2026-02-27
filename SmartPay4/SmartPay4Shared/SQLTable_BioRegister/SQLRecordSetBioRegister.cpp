//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetBioRegister.h"
//**********************************************************************

CSQLRecordSetMetadataBioRegister::CSQLRecordSetMetadataBioRegister(bool bForExistenceCheck)
{
    if (FALSE == bForExistenceCheck)
    {
        AddInt64Field(&m_nUserID, BioRegister::UserID.Label);
        AddTextField(&m_strUsername, BioRegister::Username.Label);
        AddTextField(&m_strRegDate, BioRegister::RegDate.Label);
        AddTextField(&m_strRegTime, BioRegister::RegTime.Label);
        AddTextField(&m_strUnRegDate, BioRegister::UnRegDate.Label);
        AddTextField(&m_strUnRegTime, BioRegister::UnRegTime.Label);
        AddBOOLField(&m_bUnReg, BioRegister::UnRegFlag.Label);
        AddTextField(&m_strFirstDate, BioRegister::FirstDate.Label);
        AddTextField(&m_strFirstTime, BioRegister::FirstTime.Label);
        AddTextField(&m_strLastDate, BioRegister::LastDate.Label);
        AddTextField(&m_strLastTime, BioRegister::LastTime.Label);
        AddIntField(&m_nBioCounter, BioRegister::Counter.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataBioRegister::AddParamsByUserID(const CString strUserID)
{
    AddTextParam(BioRegister::UserID.Label, strUserID);
}

//**********************************************************************

CSQLRecordSetBioRegister::CSQLRecordSetBioRegister(CDatabase* pDatabase, RSParams_BioRegister_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetBioRegister::CSQLRecordSetBioRegister(CDatabase* pDatabase, RSParams_BioRegister_NormalByUserId params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
    m_MD.AddParamsByUserID(params.strUserId);
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetBioRegister::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetBioRegister::GetDefaultSQL()
{
    return SQLTableNames::BiometricRegister; 
}

//**********************************************************************

void CSQLRecordSetBioRegister::LoadFromBioRegisterRow(CSQLRowBioRegister& RowBio)
{
    m_MD.m_nUserID = RowBio.GetUserID();
    m_MD.m_strUsername = RowBio.GetUsername();
    m_MD.m_strRegDate = RowBio.GetRegDate();
    m_MD.m_strRegTime = RowBio.GetRegTime();
    m_MD.m_strUnRegDate = RowBio.GetUnRegDate();
    m_MD.m_strUnRegTime = RowBio.GetUnRegTime();
    m_MD.m_bUnReg = RowBio.GetUnRegFlag();
    m_MD.m_strFirstDate = RowBio.GetFirstDate();
    m_MD.m_strFirstTime = RowBio.GetFirstTime();
    m_MD.m_strLastDate = RowBio.GetLastDate();
    m_MD.m_strLastTime = RowBio.GetLastTime();
    m_MD.m_nBioCounter = RowBio.GetBioCounter();
}

//**********************************************************************

void CSQLRecordSetBioRegister::SaveToBioRegisterRow(CSQLRowBioRegister& RowBio)
{
    RowBio.SetUserID(m_MD.m_nUserID);
    RowBio.SetUsername(m_MD.m_strUsername);
    RowBio.SetRegDate(m_MD.m_strRegDate);
    RowBio.SetRegTime(m_MD.m_strRegTime);
    RowBio.SetUnRegDate(m_MD.m_strUnRegDate);
    RowBio.SetUnRegTime(m_MD.m_strUnRegTime);
    RowBio.SetUnRegFlag(m_MD.m_bUnReg);
    RowBio.SetFirstDate(m_MD.m_strFirstDate);
    RowBio.SetFirstTime(m_MD.m_strFirstTime);
    RowBio.SetLastDate(m_MD.m_strLastDate);
    RowBio.SetLastTime(m_MD.m_strLastTime);
    RowBio.SetBioCounter(m_MD.m_nBioCounter);
}

//**********************************************************************

bool CSQLRecordSetBioRegister::StepSelectAll(CSQLRowBioRegister& RowBio)
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

    SaveToBioRegisterRow(RowBio);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
