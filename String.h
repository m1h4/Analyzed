#pragma once

#define STRING_ALLOCATION_SIZE 16

class String
{
public:
	String(void);
	String(const char* buffer);
	String(const String& other);
	~String(void);

	float AsFloat() const;
	double AsDouble() const;
	int AsInt() const;
	unsigned int AsUnsignedInt() const;
	bool AsBool(void) const;

	void ToLower(unsigned int from,unsigned int to);
	void ToLower(void) { ToLower(0,m_Length); }
	void ToUpper(unsigned int from,unsigned int to);
	void ToUpper(void) { ToUpper(0,m_Length); }

	void TrimLeft(const char* str);
	void TrimRight(const char* str);
	inline void TrimAll(const char* str) { TrimLeft(str); TrimRight(str); }
	
	unsigned int FindInstances(unsigned int from,unsigned int to,const char* buffer,unsigned int length = UINT_MAX) const;
	unsigned int FindInstances(unsigned int from,unsigned int to,char chr) const { char str[] = {chr,0}; return FindInstances(from,to,str); }
	unsigned int FindLeft(unsigned int from,unsigned int to,const char* buffer,unsigned int length = UINT_MAX) const;
	unsigned int FindLeft(const char* buffer) const { return FindLeft(0,m_Length,buffer); }
	unsigned int FindRight(unsigned int from,unsigned int to,const char* buffer,unsigned int length = UINT_MAX) const;
	unsigned int FindRight(const char* buffer) const { return FindRight(m_Length,0,buffer); }
	unsigned int FindLeftLast(unsigned int from,unsigned int to,const char* buffer,unsigned int length = UINT_MAX) const;
	unsigned int FindLeftLast(const char* buffer) const { return FindLeftLast(0,m_Length,buffer); }
	unsigned int FindRightLast(unsigned int from,unsigned int to,const char* buffer,unsigned int length = UINT_MAX) const;
	unsigned int FindRightLast(const char* buffer) const { return FindRightLast(m_Length,0,buffer); }
	unsigned int FindLeftItem(unsigned int from) const { return FindLeftItem(from,0); }
	unsigned int FindLeftItem(unsigned int from,unsigned int to) const;
	unsigned int FindRightItem(unsigned int from) const { return FindRightItem(from,m_Length); }
	unsigned int FindRightItem(unsigned int from,unsigned int to) const;
	
	void Insert(unsigned int at,const char* buffer,unsigned int length = UINT_MAX);
	void Insert(unsigned int at,char chr) { char str[] = {chr,0}; Insert(at,str); }
	
	void Erase(unsigned int at,unsigned int count = 1);
	void Erase(void) { Erase(0,m_Length); }
	void EraseFront(unsigned int count = 1) { Erase(0,count); }
	void EraseBack(unsigned int count = 1) { Erase(m_Length-count,count); }
	void EraseDiference(const char* buffer,unsigned int length = UINT_MAX);

	bool SetBuffer(const char* data,unsigned int length = UINT_MAX);
	char* GetBuffer(void) const { return m_Buffer; }
	unsigned int GetBufferSize(void) const { return m_BufferSize; }
	unsigned int GetLength(void) const { return m_Length; }

	int Compare(const char* other) const { return strcmp(m_Buffer,other); }
	int ComparePart(const char* part) const { return strncmp(m_Buffer,part,strlen(part)); }
	int CompareNoCase(const char* other) const { return _stricmp(m_Buffer,other); }
	int CompareNoCasePart(const char* part) const { return _strnicmp(m_Buffer,part,strlen(part)); }

	String& operator=(const char* other) { SetBuffer(other); return *this; }
	String& operator=(const String& other) { SetBuffer(other.GetBuffer()); return *this; }

	bool operator==(const char* other) const { return Compare(other) == 0; }
	bool operator!=(const char* other) const { return Compare(other) != 0; }
	bool operator>(const char* other) const { return Compare(other) == 1; }
	bool operator<(const char* other) const { return Compare(other) == -1; }
	bool operator>=(const char* other) const { return Compare(other) != -1; }
	bool operator<=(const char* other) const { return Compare(other) != 1; }

	bool operator==(const String& other) const { return Compare(other) == 0; }
	bool operator!=(const String& other) const { return Compare(other) != 0; }
	bool operator>(const String& other) const { return Compare(other) == 1; }
	bool operator<(const String& other) const { return Compare(other) == -1; }
	bool operator>=(const String& other) const { return Compare(other) != -1; }
	bool operator<=(const String& other) const { return Compare(other) != 1; }

	String operator+(const char* other) const { String string(m_Buffer); string += other; return string; }
	String& operator+=(const char* other) { Insert(GetLength(),other); return *this; }
	
	String operator+(const String& other) const { String string(m_Buffer); string += other; return string; }
	String& operator+=(const String& other) { Insert(GetLength(),other.GetBuffer()); return *this; }

	String operator+(char other) const { String string(m_Buffer); string += other; return string; }
	String& operator+=(char other) { Insert(GetLength(),other); return *this; }

	operator char*(void) { return m_Buffer; }
	operator const char*(void) const { return m_Buffer; }

protected:
	char*			m_Buffer;
	unsigned int	m_BufferSize;
	unsigned int	m_Length;

protected:
	bool PrepareBuffer(unsigned int newSize,bool preserve);
	bool ResizeBuffer(unsigned int newSize,bool preserve);
};