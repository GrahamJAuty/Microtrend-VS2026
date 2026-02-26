#pragma once
/**********************************************************************/

class CMapKeyInt64 
{
public:
    __int64 m_nID;
    
    CMapKeyInt64(__int64 i = 0) : m_nID(i){}

    bool operator<(const CMapKeyInt64& other) const 
    { 
        return m_nID < other.m_nID;
    }

    bool operator==(const CMapKeyInt64& other) const 
    {
        return m_nID == other.m_nID;
    }
};

/**********************************************************************/

class CMapKeyInt
{
public:
    int m_nID;

    CMapKeyInt(int i = 0) : m_nID(i) {}

    bool operator<(const CMapKeyInt& other) const
    {
        return m_nID < other.m_nID;
    }

    bool operator==(const CMapKeyInt& other) const
    {
        return m_nID == other.m_nID;
    }
};

/**********************************************************************/

class CMapDataEmpty
{
public:
    CMapDataEmpty(){}

    void Add(const CMapDataEmpty& source)
    {
    }
};

/**********************************************************************/

class CMapDataSortedInt 
{
public:
    int m_nValue;

    CMapDataSortedInt(int i = 0) : m_nValue(i) {}
	
    void Add(const CMapDataSortedInt& source)
	{
	}
};

/**********************************************************************/

class CMapDataConsolidatedInt
{
public:
    int m_nValue;

    CMapDataConsolidatedInt(int i = 0) : m_nValue(i) {}

    void Add(const CMapDataConsolidatedInt& source)
    {
		m_nValue += source.m_nValue;
    }
};

/**********************************************************************/
