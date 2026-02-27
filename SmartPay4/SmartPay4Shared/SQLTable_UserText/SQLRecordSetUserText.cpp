//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "SQLRecordSetUserText.h"
//**********************************************************************

CSQLRecordSetMetadataUserText::CSQLRecordSetMetadataUserText(bool bForExistenceCheck) : CSQLRecordSetMetadata()
{
    if (FALSE == bForExistenceCheck)
    {
        AddTextField(&m_strFieldLabel, UserText::FieldLabel.Label);
        AddTextField(&m_strUserText, UserText::UserText.Label);
        AddBOOLField(&m_bUseFlag, UserText::UseFlag.Label);
    }
}

//**********************************************************************

void CSQLRecordSetMetadataUserText::AddParamsByFieldLabel(const CString strFieldLabel)
{
    AddTextParam(UserText::FieldLabel.Label, strFieldLabel);
}

//**********************************************************************

CSQLRecordSetUserText::CSQLRecordSetUserText(CDatabase* pDatabase, RSParams_UserText_NormalNoParams params) :
    CSQLRecordSetConnectionPool(pDatabase),
	m_MD()
{
    CommonInit();
}

//**********************************************************************

CSQLRecordSetUserText::CSQLRecordSetUserText(CDatabase* pDatabase, RSParams_UserText_NormalByFieldLabel params, bool bForExistenceCheck) :
    CSQLRecordSetConnectionPool(pDatabase),
    m_MD(bForExistenceCheck)
{
    m_MD.AddParamsByFieldLabel(params.strFieldLabel);
    CommonInit();
}

//**********************************************************************

void CSQLRecordSetUserText::CommonInit()
{
    m_nFields = m_MD.GetFieldCount();
    m_nParams = m_MD.GetParamCount();
    SetMetadata(&m_MD);
    SetInitialFilterString();
}

//**********************************************************************

CString CSQLRecordSetUserText::GetDefaultSQL()
{
    return SQLTableNames::UserTexts; 
}

//**********************************************************************

void CSQLRecordSetUserText::LoadFromUserTextRow(CSQLRowUserText& RowText)
{
    CString strUserText = RowText.GetUserText();
    if (strUserText.Right(2) == "\r\n")
    {
        strUserText = strUserText.Left(strUserText.GetLength() - 2);
    }

    m_MD.m_strFieldLabel = RowText.GetFieldLabel();
    m_MD.m_strUserText = strUserText;
    m_MD.m_bUseFlag = RowText.GetUseFlag();
}

//**********************************************************************

void CSQLRecordSetUserText::SaveToUserTextRow(CSQLRowUserText& RowText)
{
    CString strUserText = m_MD.m_strUserText;
    if (strUserText.Right(2) == "\r\n")
    {
        strUserText = strUserText.Left(strUserText.GetLength() - 2);
    }

    RowText.SetFieldLabel(m_MD.m_strFieldLabel);
    RowText.SetUserText(strUserText);
    RowText.SetUseFlag(m_MD.m_bUseFlag);
}

//**********************************************************************

bool CSQLRecordSetUserText::StepSelectAll(CSQLRowUserText& RowText)
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

    SaveToUserTextRow(RowText);
    MoveNext();
    
    return TRUE;
}

//**********************************************************************
