#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#define CUSTOM_FIELD_SORTIDX_NA 999999
/**********************************************************************/

struct CCustomFieldValueMapEntry
{
	int m_nValueNo;
	int m_nValueIdx;
	int m_nSortIdx;
};

/**********************************************************************/

struct CCustomFieldValueMapInfo
{
	int m_nCurrentFilter;
	int m_nCurrentDbIdx;
	bool m_bIncludeNA;
	int m_nLastValueNo;
	int m_nLastSortIdx;
};

/**********************************************************************/

class CCustomFieldManager
{
public:
	CCustomFieldManager();

public:
	void Clear();
	void Initialise();
	
public:
	int GetSlotByFieldNo( int nFieldNo );

public:
	int GetSlotCount(){ return m_nSlotCount; }
	int GetFieldNoBySlot( int nSlot );
	const char* GetFieldNameBySlot( int nSlot );
	const char* GetFieldNameByFieldNo( int nFieldNo );
	
public:
	bool UpdateValueMapsFromComboSelectorsDatabase();
	bool UpdateValueMapsFromComboSelectorsSystem();
	void CreateValueMap( int nSlot, int nFilter );
	int GetSortIdxByValueNo( int nSlot, int nValueNo );

public:
	void SetIncludeAllFlag( bool b ){ m_bIncludeAll = b; }
	void SetIncludeNAFlag( bool b ){ m_bIncludeNA = b; }
	void SetIncludeSetsFlag( bool b ){ m_bIncludeSets = b; }
	void SetComboSelector( int nSlot, CSSComboBox* p );
	void FillComboSelectorDatabase( int nSlot );
	void FillComboSelectorSystem( int nSlot );
	int GetComboSelection( int nSlot );
	const char* GetComboText( int nSlot );
	void ForceComboSelection( int nSlot, int nFilter );
	void ForceComboSelection ( int nSlot, const char* szSelect );

private:
	bool FindMapValue( int nSlot, int nValue, int& nIdx ); 

private:
	int m_nSlotCount;

private:
	int m_nSlotByFieldNo[ MAX_ITEM_CUSTOM_FIELDS + 1 ];
	int m_nFieldNoBySlot[ MAX_ITEM_CUSTOM_FIELDS ];
	CString m_strFieldNameBySlot[ MAX_ITEM_CUSTOM_FIELDS ];

private:
	CCustomFieldValueMapInfo m_arrayValueMapInfo [ MAX_ITEM_CUSTOM_FIELDS ];
	CArray<CCustomFieldValueMapEntry,CCustomFieldValueMapEntry> m_arrayValueMap[ MAX_ITEM_CUSTOM_FIELDS ];
	CSSComboBox* m_pComboSelector[ MAX_ITEM_CUSTOM_FIELDS ];

private:
	bool m_bIncludeAll;
	bool m_bIncludeNA;
	bool m_bIncludeSets;
	
private:
	CString m_strFieldName;
	CString m_strComboText;
};

/**********************************************************************/
#endif
/**********************************************************************/
