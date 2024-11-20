// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <string>
#include <exception>
#include <algorithm> //исправил

// Fake variables used as placeholders in tests
//static const int FAKE_INT = -1;
//static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
	if (len < 0) 
	{
		throw out_of_range("Negative length");
	}

	BitLen = len;

	MemLen = BitLen / (sizeof(TELEM) * 8) + (BitLen % (sizeof(TELEM) * 8) != 0);

	pMem = new TELEM[MemLen];
	
	if (pMem == nullptr) //исправил
	{
		throw bad_alloc();
	}

	memset(pMem, 0, MemLen * sizeof(TELEM));
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;

	MemLen = bf.MemLen;

	pMem = new TELEM[MemLen];

	if (pMem == nullptr) //исправил
	{
		throw bad_alloc();
	}

	for (int i = 0; i < MemLen; i++) 

	{
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] pMem;

	BitLen = 0;

	MemLen = 0;

	pMem = nullptr; //исправил
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0 || n >= BitLen) 
	{
		throw out_of_range("out of range");
	}

	return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n < 0 || n >= BitLen)
	{
		throw out_of_range("out of range");
	}

	return ((TELEM)1) << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0 || n >= BitLen)
		
	{
		throw out_of_range("out of range");
	}

	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0 || n >= BitLen)

	{
		throw out_of_range("out of range");
	}

	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{

  if (n < 0 || n >= BitLen)

  {
	  throw out_of_range("out of range");
  }

  return (pMem[GetMemIndex(n)] & GetMemMask(n)) > 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{

	if (this != &bf) 
	{
		if (bf.MemLen != MemLen) //исправил
		{
			MemLen = bf.MemLen;

			delete[] pMem;

			pMem = new TELEM[MemLen];

			if (pMem == nullptr) //исправил
			{
				throw bad_alloc();
			}
		}
		
		BitLen = bf.BitLen;

		for (int i = 0; i < MemLen; i++) 

		{
			pMem[i] = bf.pMem[i];
		}
	}

	return *this;

}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen) //исправил
	{
		return 0;
    }

	for (int i = 0; i < MemLen; i++)
	{
		if (pMem[i] != bf.pMem[i])
		{
			return 0;
		}
	}

	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf); //исправил
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{

	int b = max(BitLen, bf.BitLen);

	int c = min(MemLen, bf.MemLen);

	TBitField mas(b);

	for (int i = 0; i < c; i++) 

	{
		mas.pMem[i] = pMem[i] | bf.pMem[i];
	}
	if (MemLen < bf.MemLen) 

	{
		for (int i = c; i < bf.MemLen; i++) 

		{
			mas.pMem[i] = bf.pMem[i];
		}
	}
	else 
	{
		for (int i = c; i < MemLen; i++) 

		{
			mas.pMem[i] = pMem[i];
		}
	}
	//исправил

	return mas;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{

	int b = max(BitLen, bf.BitLen);

	int c = min(MemLen, bf.MemLen);

	TBitField mas(b);

	for (int i = 0; i < c; i++) 

	{
		mas.pMem[i] = pMem[i] & bf.pMem[i];
	}

	return mas;
}

TBitField TBitField::operator~(void) // отрицание
{

	TBitField mas(BitLen);

	for (int i = 0; i < MemLen; i++)
	{
		mas.pMem[i] = ~pMem[i];
	}

	TELEM d = ((TELEM)1 << (BitLen % (sizeof(TELEM) * 8))) - (TELEM)1;

	mas.pMem[MemLen - 1] &= d;

	return mas;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	string k;

	istr >> k;

	int f = k.size();

	if (f != bf.BitLen) 
	{
		throw out_of_range("different length");
	}

	for (int i = 0; i < f; i++)

	{
		if (k[i] == '1') 

		{
			bf.SetBit(i);
		}

		else 
		{
			bf.ClrBit(i);
		}
	}
	
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)

	{
		if (bf.GetBit(i) == 1)

		{
			ostr << 1;
		}

		else 

		{
			ostr << 0;
		}
	}

    return ostr;
}

//Fixtures

void TestTBitField::SetUp()
{
	bf = new TBitField(1);        

	a1 = new TBitField(2);     

	a2 = new TBitField(2);

	a3 = new TBitField(5);


	b0 = new TBitField(3); 

	b1 = new TBitField(2);

	b2 = new TBitField(1);

	b3 = new TBitField(35);

	c0 = new TBitField(2);

	c1 = new TBitField(5);


	d0 = new TBitField(43);   

	d1 = new TBitField(34);

	e0 = new TBitField(30);


	f0 = new TBitField(1);     

	f1 = new TBitField(2);

	f2 = new TBitField(3);

	f3 = new TBitField(4);

}

void TestTBitField::SetUp2(int k)
{
	bf = new TBitField(k);
}

void TestTBitField::TearDown()
{
	delete bf;
	delete a1;
	delete a2;
	delete a3;
	delete b0;
	delete b1;
	delete b2;
	delete b3;
	delete c0;
	delete c1;
	delete d0;
	delete d1;
	delete e0;
	delete f0;
	delete f1;
	delete f2;
	delete f3;
}
