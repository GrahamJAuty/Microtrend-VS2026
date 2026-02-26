//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetDeptInfo.h"
//**********************************************************************

CSQLRecordSetMetadataDeptInfo::CSQLRecordSetMetadataDeptInfo()
{
    AddIntField(&m_nDbNo, DeptInfo::DbNo.Label);
    AddIntField(&m_nDeptNo, DeptInfo::DeptNo.Label);
    AddTextField(&m_strName, DeptInfo::Name.Label);
    AddDecimalField(&m_strLoyaltyBonusTrip, DeptInfo::LoyaltyBonusTrip.Label);
    AddIntField(&m_nLoyaltyBonusType, DeptInfo::LoyaltyBonusType.Label);
    AddIntField(&m_nLoyaltyBonusPoints, DeptInfo::LoyaltyBonusPoints.Label);
    AddDecimalField(&m_strLoyaltyBonusValue, DeptInfo::LoyaltyBonusValue.Label);
    AddIntField(&m_nLoyaltyBonusPercent, DeptInfo::LoyaltyBonusPercent.Label);
    AddBOOLField(&m_bGeneralSpendBonusFlag, DeptInfo::GeneralSpendBonusFlag.Label, TRUE);
}
   
//**********************************************************************

void CSQLRecordSetMetadataDeptInfo::AddParamsByDbNo(const int nDbNo)
{
    AddIntParam(DeptInfo::DbNo.Label, nDbNo);
}

//**********************************************************************

void CSQLRecordSetMetadataDeptInfo::AddParamsByDbNoDeptNo(const int nDbNo, const int nDeptNo)
{
    AddParamsByDbNo(nDbNo);
    AddIntParam(DeptInfo::DeptNo.Label, nDeptNo);
}

//**********************************************************************

CSQLRecordSetDeptInfo::CSQLRecordSetDeptInfo(CDatabase* pDatabase, RSParams_DeptInfo_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
	m_MD()
{
    CommonInit();
}

//**********************************************************************

CSQLRecordSetDeptInfo::CSQLRecordSetDeptInfo(CDatabase* pDatabase, RSParams_DeptInfo_NormalByDbNo params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
	m_MD.AddParamsByDbNo(params.nDbNo);
	CommonInit();
}

//**********************************************************************

CSQLRecordSetDeptInfo::CSQLRecordSetDeptInfo(CDatabase* pDatabase, RSParams_DeptInfo_NormalByDbNoDeptNo params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
    m_MD.AddParamsByDbNoDeptNo(params.nDbNo,params.nDeptNo);
    CommonInit();
}

//**********************************************************************

void CSQLRecordSetDeptInfo::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetDeptInfo::GetDefaultSQL()
{
    return SQLTableNames::DeptInfo; 
}

//**********************************************************************

void CSQLRecordSetDeptInfo::LoadFromDeptInfoRow(CSQLRowDeptInfo& RowDept)
{
	m_MD.m_nDbNo = RowDept.GetDbNo();
    m_MD.m_nDeptNo = RowDept.GetDeptNo();
    m_MD.m_strName = RowDept.GetName();
	SetDoubleString(m_MD.m_strLoyaltyBonusTrip, RowDept.GetLoyaltyBonusTrip());
    m_MD.m_nLoyaltyBonusType = RowDept.GetLoyaltyBonusType();
    m_MD.m_nLoyaltyBonusPoints = RowDept.GetLoyaltyBonusPoints();
	SetDoubleString(m_MD.m_strLoyaltyBonusValue, RowDept.GetLoyaltyBonusValue());
    m_MD.m_nLoyaltyBonusPercent = RowDept.GetLoyaltyBonusPercent();
    m_MD.m_bGeneralSpendBonusFlag = RowDept.GetGeneralSpendBonusFlag();
}

//**********************************************************************

void CSQLRecordSetDeptInfo::SaveToDeptInfoRow(CSQLRowDeptInfo& RowDept)
{
	RowDept.SetDbNo(m_MD.m_nDbNo);
    RowDept.SetDeptNo(m_MD.m_nDeptNo);
    RowDept.SetName(m_MD.m_strName);
    RowDept.SetLoyaltyBonusTrip(atof(m_MD.m_strLoyaltyBonusTrip));
    RowDept.SetLoyaltyBonusType(m_MD.m_nLoyaltyBonusType);
    RowDept.SetLoyaltyBonusPoints(m_MD.m_nLoyaltyBonusPoints);
    RowDept.SetLoyaltyBonusValue(atof(m_MD.m_strLoyaltyBonusValue));
    RowDept.SetLoyaltyBonusPercent(m_MD.m_nLoyaltyBonusPercent);
	RowDept.SetGeneralSpendBonusFlag(m_MD.m_bGeneralSpendBonusFlag);
}

//**********************************************************************

bool CSQLRecordSetDeptInfo::StepSelectAll(CSQLRowDeptInfo& RowDept)
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

    SaveToDeptInfoRow(RowDept);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
