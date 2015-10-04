#include "Globals.h"
#include "String.h"

String::String(void) :
m_Buffer(NULL),
m_BufferSize(NULL),
m_Length(NULL)
{
	SetBuffer("");
}

String::String(const char* buffer) :
m_Buffer(NULL),
m_BufferSize(NULL),
m_Length(NULL)
{
	SetBuffer(buffer);
}

String::String(const String& other) :
m_Buffer(NULL),
m_BufferSize(NULL),
m_Length(NULL)
{
	SetBuffer(other.GetBuffer());
}

String::~String(void)
{
	ResizeBuffer(NULL,false);
}

bool String::PrepareBuffer(unsigned int newSize,bool preserve)
{
	if(m_BufferSize == newSize)
		return true;

	if(newSize < m_BufferSize)
		if(m_BufferSize - newSize < STRING_ALLOCATION_SIZE)
			return true;

	newSize = newSize - newSize % STRING_ALLOCATION_SIZE + STRING_ALLOCATION_SIZE;

	return ResizeBuffer(newSize,preserve);
}

bool String::ResizeBuffer(unsigned int newSize,bool preserve)
{
	if(m_BufferSize == newSize)
		return true;

	if(!newSize)
	{
		delete[] m_Buffer;

		m_Length = 0;
		m_Buffer = NULL;
		m_BufferSize = 0;

		return true;
	}

	if(!m_Buffer)
	{
		m_Buffer = new char[newSize];
		if(!m_Buffer)
			return false;

		m_BufferSize = newSize;
		m_Length = 0;

		return true;
	}

	char* newBuffer = new char[newSize];
	if(!newBuffer)
		return false;

	if(preserve)
		CopyMemory(newBuffer,m_Buffer,m_Length + 1);	// TODO What if the new buffer size is smaller that m_Length
	else
		m_Length = 0;

	delete[] m_Buffer;

	m_Buffer = newBuffer;
	m_BufferSize = newSize;

	return true;
}

bool String::SetBuffer(const char* buffer,unsigned int length)
{
	ASSERT(buffer);
	ASSERT(buffer != m_Buffer);

	if(length == UINT_MAX)
		length = (unsigned int)strlen(buffer);

	if(!PrepareBuffer(length + 1,false))
		return false;

	CopyMemory(m_Buffer,buffer,length);

	m_Buffer[length] = 0;
	
	m_Length = length;

	return true;
}

void String::Erase(unsigned int at,unsigned int count)
{
	ASSERT(m_Buffer);
	ASSERT(at + count <= m_Length);

	CopyMemory(m_Buffer + at,m_Buffer + at + count,m_Length - at - count + 1);
	
	m_Length -= count;

	PrepareBuffer(m_Length + 1,true);
}

void String::EraseDiference(const char* buffer,unsigned int length)
{
	ASSERT(m_Buffer);
	ASSERT(buffer);

	if(length == UINT_MAX)
		length = (unsigned int)strlen(buffer);

	for(unsigned int i = 0; i < length; ++i)
	{
		if(m_Buffer[i] != buffer[i])
		{
			Erase(i,m_Length - i);
			break;
		}
	}
}

void String::Insert(unsigned int at,const char* buffer,unsigned int length)
{
	ASSERT(at <= m_Length);
	ASSERT(buffer);
	ASSERT(buffer != m_Buffer);

	if(length == UINT_MAX)
		length = (unsigned int)strlen(buffer);

	PrepareBuffer(m_Length + 1 + length,true);

	MoveMemory(m_Buffer + at + length,m_Buffer + at,m_Length + 1 - at);
	CopyMemory(m_Buffer + at,buffer,length);

	m_Length += length;
}

unsigned int String::FindLeft(unsigned int from,unsigned int to,const char* buffer,unsigned int length) const
{
	ASSERT(from <= m_Length);
	ASSERT(to <= m_Length);
	ASSERT(from <= to);
	ASSERT(buffer);

	if(length == UINT_MAX)
		length = (unsigned int)strlen(buffer);

	for(unsigned int i = from; i < to; ++i)
		if(strncmp(m_Buffer + i,buffer,min(m_Length - i,length)) == 0)
			return i;

	return 0xFFFFFFFF;
}

unsigned int String::FindRight(unsigned int from,unsigned int to,const char* buffer,unsigned int length) const
{
	ASSERT(from <= m_Length);
	ASSERT(to <= m_Length);
	ASSERT(from >= to);
	ASSERT(buffer);

	if(length == UINT_MAX)
		length = (unsigned int)strlen(buffer);

	for(unsigned int i = from; i > to; --i)
		if(strncmp(m_Buffer + i,buffer,min(m_Length - i,length)) == 0)
			return i;

	return 0xFFFFFFFF;
}

unsigned int String::FindLeftLast(unsigned int from,unsigned int to,const char* buffer,unsigned int length) const
{
	ASSERT(from <= m_Length);
	ASSERT(to <= m_Length);
	ASSERT(from <= to);
	ASSERT(buffer);

	if(length == UINT_MAX)
		length = (unsigned int)strlen(buffer);

	if(length > m_Length)
		return 0xFFFFFFFF;

	unsigned int i;

	for(i = from; i < to; i += length)
		if(strncmp(m_Buffer + i,buffer,min(m_Length - i,length)) != 0)
			return i == from ? 0xFFFFFFFF : i;

	return i >= to ? 0xFFFFFFFF : i;
}

unsigned int String::FindRightLast(unsigned int from,unsigned int to,const char* buffer,unsigned int length) const
{
	ASSERT(from <= m_Length);
	ASSERT(to <= m_Length);
	ASSERT(from >= to);
	ASSERT(buffer);

	if(length == UINT_MAX)
		length = (unsigned int)strlen(buffer);

	if(from < length)
		return 0xFFFFFFFF;

	unsigned int i;

	for(i = from - length; i > to; i -= length > i ? i : length)
		if(strncmp(m_Buffer + i,buffer,min(m_Length - i,length)) != 0)
			return i == from ? 0xFFFFFFFF : i;

	return i <= to ? 0xFFFFFFFF : i;
}

unsigned int String::FindInstances(unsigned int from,unsigned int to,const char* buffer,unsigned int length) const
{
	ASSERT(from <= m_Length);
	ASSERT(to <= m_Length);
	ASSERT(from <= to);
	ASSERT(buffer);

	if(length == UINT_MAX)
		length = (unsigned int)strlen(buffer);

	unsigned int count = 0;

	for(unsigned int i = from; i < to; ++i)
	{
		if(strncmp(m_Buffer + i,buffer,min(m_Length - i,length)) == 0)
		{
			count++;
			i += length;
		}
	}

	return count;
}

unsigned int String::FindLeftItem(unsigned int from,unsigned int to) const
{
	ASSERT(from <= m_Length);
	ASSERT(to <= m_Length);
	ASSERT(to <= from);

	for(int i = from; i >= (int)to; --i)
		if(i == 0 || (!isspace(m_Buffer[i]) && isspace(m_Buffer[i-1])))
			return i;

	return 0xFFFFFFFF;
}

unsigned int String::FindRightItem(unsigned int from,unsigned int to) const
{
	ASSERT(from <= m_Length);
	ASSERT(to <= m_Length);
	ASSERT(from <= to);

	for(unsigned int i = from; i <= to; ++i)
		if(i == m_Length || (i && isspace(m_Buffer[i]) && !isspace(m_Buffer[i-1])))
			return i;

	return 0xFFFFFFFF;
}

void String::TrimLeft(const char* str)
{
	ASSERT(str);

	unsigned int last = FindLeftLast(str);
	if(last == 0xFFFFFFFF)
		return;

	Erase(0,last);
}

void String::TrimRight(const char* str)
{
	ASSERT(str);

	unsigned int last = FindRightLast(str);
	if(last == 0xFFFFFFFF)
		return;

	last++;

	Erase(last,m_Length - last);
}

void String::ToLower(unsigned int from,unsigned int to)
{
	ASSERT(from <= m_Length);
	ASSERT(to <= m_Length);
	ASSERT(from <= to);

	for(unsigned int i = from; i < to; ++i)
		m_Buffer[i] = tolower(m_Buffer[i]);
}

void String::ToUpper(unsigned int from,unsigned int to)
{
	ASSERT(from <= m_Length);
	ASSERT(to <= m_Length);
	ASSERT(from <= to);

	for(unsigned int i = from; i < to; ++i)
		m_Buffer[i] = toupper(m_Buffer[i]);
}

float String::AsFloat(void) const
{
	return (float)strtod(m_Buffer,NULL);
}

double String::AsDouble(void) const
{
	return strtod(m_Buffer,NULL);
}

int String::AsInt(void) const
{
	return strtol(m_Buffer,NULL,0);
}

unsigned int String::AsUnsignedInt(void) const
{
	return strtoul(m_Buffer,NULL,0);
}

bool String::AsBool(void) const
{
	if(_stricmp(m_Buffer,"true") == 0)
		return true;
	else if(AsUnsignedInt())
		return true;
	else
		return false;
}