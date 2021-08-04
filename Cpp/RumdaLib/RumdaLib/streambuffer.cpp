#include "pch.h"

namespace RumdaLib
{
	StreamBuffer::StreamBuffer()
	{
		_pos = 0;
		_size = 0;
		_data = nullptr;
	}
	StreamBuffer::~StreamBuffer()
	{
	}
	bool StreamBuffer::Set(char* buffer)
	{
		if (!buffer)
			return false;

		_data = buffer;
		_pos = 0;
		_size = 0;

		return true;
	}
	char* StreamBuffer::Get()
	{
		return _data;
	}
	uint32_t StreamBuffer::Size()
	{
		return _size;
	}
	void StreamBuffer::ReadData(bool* data)
	{
		memcpy(data, &_data[_pos], sizeof(bool));
		_pos -= sizeof(bool);
		_size = _pos;			// 읽어들인 것이니 크기를 뺀다. 쓸때는 더한다
	}
	void StreamBuffer::ReadData(uint8_t* data)
	{
		memcpy(data, &_data[_pos], sizeof(uint8_t));
		_pos -= sizeof(uint8_t);
		_size = _pos;
	}
	void StreamBuffer::ReadData(uint16_t* data)
	{
		memcpy(data, &_data[_pos], sizeof(uint16_t));
		_pos -= sizeof(uint16_t);
		_size = _pos;
	}
	void StreamBuffer::ReadData(uint32_t* data)
	{
		memcpy(data, &_data[_pos], sizeof(uint32_t));
		_pos -= sizeof(uint32_t);
		_size = _pos;
	}
	void StreamBuffer::ReadData(uint64_t* data)
	{
		memcpy(data, &_data[_pos], sizeof(uint64_t));
		_pos -= sizeof(uint64_t);
		_size = _pos;
	}
	void StreamBuffer::ReadData(int* data)
	{
		memcpy(data, &_data[_pos], sizeof(int));
		_pos -= sizeof(int);
		_size = _pos;
	}
	void StreamBuffer::ReadData(float* data)
	{
		memcpy(data, &_data[_pos], sizeof(float));
		_pos -= sizeof(float);
		_size = _pos;
	}
	void StreamBuffer::ReadData(double* data)
	{
		memcpy(data, &_data[_pos], sizeof(double));
		_pos -= sizeof(double);
		_size = _pos;
	}
	void StreamBuffer::ReadData(long double* data)
	{
		memcpy(data, &_data[_pos], sizeof(long double));
		_pos -= sizeof(long double);
		_size = _pos;
	}
	void StreamBuffer::ReadData(std::string* data, int strSize)
	{
		// string 자료형의 카피는 스트링의 길이를 받아야 할 듯? - 더 좋은 방법은?
		memcpy(data, &_data[_pos], strSize);
		_pos -= strSize;
		_size = _pos;
	}
	void StreamBuffer::ReadData(char* data)
	{
		memcpy(data, &_data[_pos], sizeof(char));
		_pos -= sizeof(char);
		_size = _pos;
	}

	void StreamBuffer::WriteData(bool* data)
	{
		memcpy(&_data[_pos], data, sizeof(bool));
		_pos += sizeof(bool);
		_size = _pos;
	}
	void StreamBuffer::WriteData(uint8_t* data)
	{
		memcpy(&_data[_pos], data, sizeof(uint8_t));
		_pos += sizeof(uint8_t);
		_size = _pos;
	}
	void StreamBuffer::WriteData(uint16_t* data)
	{
		memcpy(&_data[_pos], data, sizeof(uint16_t));
		_pos += sizeof(uint16_t);
		_size = _pos;
	}
	void StreamBuffer::WriteData(uint32_t* data)
	{
		memcpy(&_data[_pos], data, sizeof(uint32_t));
		_pos += sizeof(uint32_t);
		_size = _pos;
	}
	void StreamBuffer::WriteData(uint64_t* data)
	{
		memcpy(&_data[_pos], data, sizeof(uint64_t));
		_pos += sizeof(uint64_t);
		_size = _pos;
	}
	void StreamBuffer::WriteData(int* data)
	{
		memcpy(&_data[_pos], data, sizeof(int));
		_pos += sizeof(int);
		_size = _pos;
	}
	void StreamBuffer::WriteData(float* data)
	{
		memcpy(&_data[_pos], data, sizeof(float));
		_pos += sizeof(float);
		_size = _pos;
	}
	void StreamBuffer::WriteData(double* data)
	{
		memcpy(&_data[_pos], data, sizeof(double));
		_pos += sizeof(double);
		_size = _pos;
	}
	void StreamBuffer::WriteData(long double* data)
	{
		memcpy(&_data[_pos], data, sizeof(long double));
		_pos += sizeof(long double);
		_size = _pos;
	}
	void StreamBuffer::WriteData(std::string* data, int strSize)
	{
		// 테스트-확인필요
		memcpy(&_data[_pos], data, strSize);
		_pos += strSize;
		_size = _pos;
	}
	void StreamBuffer::WriteData(char* data)
	{
		memcpy(&_data[_pos], data, sizeof(char));
		_pos += sizeof(char);
		_size = _pos;
	}
}