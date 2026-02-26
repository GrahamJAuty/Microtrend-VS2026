//**********************************************************************
#include "..\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetStaffGiftException.h"
//**********************************************************************

CSQLRecordSetMetadataStaffGiftException::CSQLRecordSetMetadataStaffGiftException() : CSQLRecordSetMetadata()
{
    AddInt64Field(&m_nLineID, StaffGiftExceptions::LineID.Label);
    AddInt64Field(&m_nCustomerID, StaffGiftExceptions::CustomerID.Label);
    AddIntField(&m_nFolderIndexDbNo, StaffGiftExceptions::FolderIndexDbNo.Label);
    AddIntField(&m_nFolderIndexSetNo, StaffGiftExceptions::FolderIndexSetNo.Label);
    AddIntField(&m_nServerNo, StaffGiftExceptions::ServerNo.Label, 1);
    AddInt64Field(&m_nServerID, StaffGiftExceptions::ServerID.Label);
    AddTextField(&m_strDate, StaffGiftExceptions::Date.Label);
    AddTextField(&m_strTime, StaffGiftExceptions::Time.Label);
    AddDecimalField(&m_strValue, StaffGiftExceptions::Value.Label);
    AddIntField(&m_nErrorCode, StaffGiftExceptions::ErrorCode.Label);
}

//**********************************************************************

void CSQLRecordSetMetadataStaffGiftException::AddParamsByLineId(const CString strLineId)
{
    AddTextParam(StaffGiftExceptions::LineID.Label, strLineId);
}

//**********************************************************************

CSQLRecordSetStaffGiftException::CSQLRecordSetStaffGiftException(CDatabase* pDatabase, RSParams_StaffGiftException_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
	CommonInit();
}

//**********************************************************************

CSQLRecordSetStaffGiftException::CSQLRecordSetStaffGiftException(CDatabase* pDatabase, RSParams_StaffGiftException_NormalByLineId params ) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD()
{
    m_MD.AddParamsByLineId(params.strLineId);
	CommonInit();
}

//**********************************************************************

void CSQLRecordSetStaffGiftException::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetStaffGiftException::GetDefaultSQL()
{
    return SQLTableNames::StaffGiftExceptions;
}

//**********************************************************************

void CSQLRecordSetStaffGiftException::DoFieldExchange(CFieldExchange* pFX)
{
    if ((0 == m_MD.m_nFolderIndexDbNo) && (m_MD.m_nFolderIndexSetNo != 0))
    {
        m_MD.m_nFolderIndexDbNo = 1;
    }

	CSQLRecordSetConnectionPool::DoFieldExchange(pFX);

    if ((0 == m_MD.m_nFolderIndexDbNo) && (m_MD.m_nFolderIndexSetNo != 0))
    {
        m_MD.m_nFolderIndexDbNo = 1;
    }
}

//**********************************************************************

void CSQLRecordSetStaffGiftException::LoadFromStaffGiftExceptionRow(CSQLRowStaffGiftException& RowSC)
{
    m_MD.m_nLineID = RowSC.GetLineID();
    m_MD.m_nCustomerID = RowSC.GetCustomerID();
    m_MD.m_nFolderIndexDbNo = RowSC.GetFolderIndexDbNo();
    m_MD.m_nFolderIndexSetNo = RowSC.GetFolderIndexSetNo();
    m_MD.m_nServerNo = RowSC.GetServerNo();
    m_MD.m_nServerID = RowSC.GetServerID();
    m_MD.m_strDate = RowSC.GetDate();
    m_MD.m_strTime = RowSC.GetTime();
	SetDoubleString(m_MD.m_strValue, RowSC.GetValue());
    m_MD.m_nErrorCode = RowSC.GetErrorCode();
}

//**********************************************************************

void CSQLRecordSetStaffGiftException::SaveToStaffGiftExceptionRow(CSQLRowStaffGiftException& RowSC)
{
    RowSC.SetLineID(m_MD.m_nLineID);
    RowSC.SetCustomerID(m_MD.m_nCustomerID);
    RowSC.SetFolderIndexDbNo(m_MD.m_nFolderIndexDbNo);
    RowSC.SetFolderIndexSetNo(m_MD.m_nFolderIndexSetNo);
    RowSC.SetServerNo(m_MD.m_nServerNo);
    RowSC.SetServerID(m_MD.m_nServerID);
    RowSC.SetDate(m_MD.m_strDate);
    RowSC.SetTime(m_MD.m_strTime);
    RowSC.SetValue(atof(m_MD.m_strValue));
    RowSC.SetErrorCode(m_MD.m_nErrorCode);
}

//**********************************************************************

bool CSQLRecordSetStaffGiftException::StepSelectAll(CSQLRowStaffGiftException& RowBC)
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

    SaveToStaffGiftExceptionRow(RowBC);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
