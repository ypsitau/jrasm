// license:BSD-3-Clause
// copyright-holders:FIND
#ifndef __CJRFORMAT_H__
#define __CJRFORMAT_H__
#ifndef _WIN32
#include "stdafx.h"
#include <stdio.h>
#endif
#include <vector>
#include <cstdint>
#include "ITapeFormat.h"

using namespace std;

enum WriteType { CJR, BIN, S };

class CjrFormat : public ITapeFormat
{
public:
	CjrFormat();
	virtual ~CjrFormat();

	void SetCjrData(uint8_t* d, int size);
	uint8_t* GetCjrDataArray(char* jrFileName, int startAddress, bool basic);
	uint8_t* GetCjrDataArray();
	uint8_t* GetBinDataArray();
	int GetBinSize();
	int GetCjrSize();
	bool isBasic();
	int GetStartAddress();
	bool SetLoadDataBlock(int fileSize);
	int GetLoadData(uint8_t* data);
	uint8_t* MemToCjrBasic(char* jrFileName);
	uint8_t * MemToCjrBin(char * jrFileName, int start, int end);
	void AdjustBaudRate(int d);

	virtual bool Init(const TCHAR* fileName);
	virtual const TCHAR* GetType();
	virtual const TCHAR* GetFileName();
	virtual void WriteByte(uint8_t b);
	virtual uint8_t GetLoadData();
	virtual void TickCounter(int c);
	virtual uint32_t GetPoiner();
	virtual uint32_t GetTimeCounter();
	virtual void remoteOn();
	virtual void remoteOff();
	virtual void Top();
	virtual void Next();
	virtual void Prev();
	virtual void FF();
	virtual void Rew();
	virtual void SetReadOnly(bool b);

	template<class Archive>
	void serialize(Archive & ar, std::uint32_t const version);

protected:
	static const int DATA_CYCLE = 280; // 1データあたりのCPUサイクル数

	vector<uint8_t> cjrData;
	vector<uint8_t> binData;
	int startAddress = 0;

	vector<vector<uint8_t>*> loadDataBlock; // JRにロードするためにCJRをブロックごとに分割して格納
	vector<vector<uint8_t>*>::iterator itBlock; // ロード時に使うイテレータ
	vector<uint8_t> leaderData; // ロード時に使うリーダー部分
	vector<uint8_t> intermData; // インターミッション時に使うリーダー部分

	TCHAR fileName[MAX_PATH];
	uint32_t fileSize = 0;
	FILE *fp = nullptr;
	uint8_t* pLoadData = nullptr;
	unsigned int loadDataSize = 0;

	int counter = 0;
	uint32_t pointer = 0;
	bool bCountStart = false;
	bool bReadOnly = false;
};

template<class Archive>
inline void CjrFormat::serialize(Archive & ar, std::uint32_t const version)
{
	ar(counter, pointer, bCountStart, startAddress);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 以下は波形データ作成のためのサポートクラス
//
////////////////////////////////////////////////////////////////////////////////////////////////////
class ByteGetter
{
public:
	ByteGetter(vector<uint8_t>* vu)
	{
		b = vu;
		it = b->begin();
	}

	uint8_t Get()
	{
		return *(it++);
	}

protected:
	vector<uint8_t>* b;
	vector<uint8_t>::iterator it;
};

class BitGetterL // リーダー部分用
{
public:
	BitGetterL(uint8_t bp)
	{
		b = bp;
		p = 0;
	}
	int Get()
	{
		return ((b & (1 << p++)) ? 1: 0);
	}
protected:
	uint8_t b;
	int p;
};

class BitGetterD // データ部分用
{
public:
	BitGetterD(uint8_t bp)
	{
		b = bp;
		p = 0;

		array[0] = 0;
		array[9] = 1;
		array[10] = 1;
		array[11] = 1;

		for (int i = 1; i < 9; ++i) {
			array[i] = (b & (1 << (i - 1)) ? 1 : 0);
		}
	}

	int Get()
	{
		return array[p++];
	}
protected:
	uint8_t b;
	int p;
	int array[12];
};


class WaveGetter
{
public:
	WaveGetter()
	{
	}

	WaveGetter(int ip)
	{
		i = ip;
	}

	void Set(int ip)
	{
		i = ip;
		p = 0;
	}

	int Get(int baud /* 0 なら2400、それ以外なら600*/, int& sign)
	{
		if (baud != 0) {
			if (i == 0) // 600 baud
			{
				if (sign == 1)
					return zero_p[p++];
				else
					return zero_n[p++];
			}
			else {
				if (sign == 1)
					return one_p[p++];
				else
					return one_n[p++];
			}
		}
		else { // 2400baud
			if (i == 0) {
				if (sign == 1)
					return zero_f_p[p++];
				else
					return zero_f_n[p++];
			}
			else {
				int ret = 0;
				if (sign == 1)
					ret = one_f_p[p];
				else
					ret = one_f_n[p];

				if (++p == 2) {
					sign *= -1;
				};
				return ret;
			}
		}
	}

protected:
	int i;
	int p = 0;
	// 600baud
	int zero_p[8] = { 0, 1, 0, 1, 0, 1, 0, 1 };
	int zero_n[8] = { 1, 0, 1, 0, 1, 0, 1, 0 };
	int  one_p[8] = { 0, 0, 1, 1, 0, 0, 1, 1 };
	int  one_n[8] = { 1, 1, 0, 0, 1, 1, 0, 0 };
	// 2400baud
	int zero_f_p[2] = { 0, 1 };
	int zero_f_n[2] = { 1, 0 };
	int  one_f_p[2] = { 0, 0 };
	int  one_f_n[2] = { 1, 1 };
};


#endif