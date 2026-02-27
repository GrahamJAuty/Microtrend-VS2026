#pragma once
/**********************************************************************/
#include <map>
#include <stdexcept>
#include <variant>
/**********************************************************************/

template <class Key, class M, class Compare = std::less<Key>>
class CReportConsolidationMap
{
public:
    using iterator = typename std::map<Key, M, Compare>::iterator;
    using const_iterator = typename std::map<Key, M, Compare>::const_iterator;

    explicit CReportConsolidationMap(Compare comp = Compare()) : m_map(comp) 
    {
    }

    void RemoveAll()
    { 
        m_map.clear();
    }

    void RemoveAt(const Key& key) 
    { 
        m_map.erase(key); 
    }

    // Consolidate: insert or merge based on key
    iterator Consolidate(const Key& key, const M& data)
    {
        auto it = m_map.find(key);
        if (it == m_map.end()) {
            return m_map.emplace(key, data).first;
        }
        else 
        {
            it->second.Add(data);
            return it;
        }
    }

    const_iterator Find(const Key& key) const
    {
        return m_map.find(key);
    }

    iterator Find(const Key& key)
    {
        return m_map.find(key);
    }

    iterator begin() { return m_map.begin(); }
    iterator end() { return m_map.end(); }
    const_iterator begin() const { return m_map.begin(); }
    const_iterator end() const { return m_map.end(); }

private:
    std::map<Key, M, Compare> m_map;
};


/**********************************************************************/

template <class Key, class M, class Compare = std::less<Key>>
class CReportUniqueMap
{
public:
    using map_type = std::map<Key, M, Compare>;
    using iterator = typename map_type::iterator;
    using const_iterator = typename map_type::const_iterator;

    explicit CReportUniqueMap(Compare comp = Compare()) : m_map(comp)
    {
    }

    void RemoveAll()
    {
        m_map.clear();
    }

    void RemoveAt(const Key& key)
    {
        m_map.erase(key);
    }

    iterator Insert(const Key& key, const M& data)
    {
        return m_map.emplace(key, data).first;
    }

    std::pair<iterator, bool> TryInsert(const Key& key, const M& data)
    {
        return m_map.emplace(key, data);
    }

    iterator Find(const Key& key)
    {
        return m_map.find(key);
    }

    const_iterator Find(const Key& key) const
    {
        return m_map.find(key);
    }

    size_t GetSize() const
    {
        return m_map.size();
    }

    bool IsEmpty() const
    {
        return m_map.empty();
    }

    size_t Count(const Key& key) const
    {
        return m_map.count(key);
    }

    iterator begin() { return m_map.begin(); }
    iterator end() { return m_map.end(); }
    const_iterator begin() const { return m_map.begin(); }
    const_iterator end() const { return m_map.end(); }

private:
    map_type m_map;
};

/**********************************************************************/

template <class Key, class M, class Compare = std::less<Key>>
class CReportMultiMap
{
public:
    using map_type = std::multimap<Key, M, Compare>;
    using iterator = typename map_type::iterator;
    using const_iterator = typename map_type::const_iterator;

    explicit CReportMultiMap(Compare comp = Compare()) : m_map(comp)
    {
    }

    void RemoveAll()
    {
        m_map.clear();
    }

    void RemoveAt(const Key& key)
    {
        m_map.erase(key);
    }

    iterator Insert(const Key& key, const M& data)
    {
        return m_map.emplace(key, data);
    }

    std::pair<const_iterator, const_iterator> EqualRange(const Key& key) const
    {
        return m_map.equal_range(key);
    }

    std::pair<iterator, iterator> EqualRange(const Key& key)
    {
        return m_map.equal_range(key);
    }

    iterator Find(const Key& key)
    {
        return m_map.find(key);
    }

    const_iterator Find(const Key& key) const
    {
        return m_map.find(key);
    }

    size_t GetSize() const
    {
        return m_map.size();
    }

    bool IsEmpty() const
    {
        return m_map.empty();
    }

    size_t Count(const Key& key) const
    {
        return m_map.count(key);
    }

    iterator begin() { return m_map.begin(); }
    iterator end() { return m_map.end(); }
    const_iterator begin() const { return m_map.begin(); }
    const_iterator end() const { return m_map.end(); }

private:
    map_type m_map;
};

//*********************************************************
