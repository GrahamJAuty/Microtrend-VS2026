#pragma once
/**********************************************************************/
#include "EnhancedIniFile.h"
#include "FilenameUpdater.h"
/**********************************************************************/

class CTaxImportMap
{
public:
    CTaxImportMap();
    void CopyFrom(CTaxImportMap& source);
    void ReloadFromIniFile(CEnhancedIniFile& iniFile);
    void PrepareIniFile(CEnhancedIniFile& iniFile);
    void Reset();
    bool Read();
    bool Write();

public:
    CString GetTaxMap0(){ return m_TaxMapT0.GetValue(); }
    CString GetTaxMap1(){ return m_TaxMapT1.GetValue(); }
    CString GetTaxMap2(){ return m_TaxMapT2.GetValue(); }
    CString GetTaxMap3(){ return m_TaxMapT3.GetValue(); }
    CString GetTaxMap4(){ return m_TaxMapT4.GetValue(); }
    CString GetTaxMap5(){ return m_TaxMapT5.GetValue(); }
    CString GetTaxMap6(){ return m_TaxMapT6.GetValue(); }

private:
    bool ReadInternal();

    CIniFileString m_TaxMapT0;
    CIniFileString m_TaxMapT1;
    CIniFileString m_TaxMapT2;
    CIniFileString m_TaxMapT3;
    CIniFileString m_TaxMapT4;
    CIniFileString m_TaxMapT5;
    CIniFileString m_TaxMapT6;
    bool m_bFatalReadError;
};

/**********************************************************************/
extern CTaxImportMap TaxImportMap;
/**********************************************************************/
