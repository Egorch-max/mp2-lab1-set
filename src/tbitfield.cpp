// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <string>
#include <exception>

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

	memset(pMem, 0, MemLen * sizeof(TELEM));
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;

	MemLen = bf.MemLen;

	pMem = new TELEM[MemLen];

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
		BitLen = bf.BitLen;

		MemLen = bf.MemLen;

		delete[] pMem;

		pMem = new TELEM[MemLen];

		for (int i = 0; i < MemLen; i++) 

		{
			pMem[i] = bf.pMem[i];
		}
	}

	return *this;

}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  int a = min(MemLen, bf.MemLen);

  for (int i = 0; i < a; i++) 

  {
	  if (pMem[i] != bf.pMem[i])

		  return 0;
  }

  if (MemLen < bf.MemLen) 
  {
	  for (int i = a; i < bf.MemLen; i++) 
	  {
		  if (bf.pMem[i] != 0)

			  return 0;
	  }
  }
  else
  {
	  for (int i = a; i < MemLen; i++) 
	  {
		  if (pMem[i] != 0)

			  return 0;
	  }
  }

  return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return 1 - (*this == bf);
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
	TELEM d = ((TELEM)1 << (b % (sizeof(TELEM) * 8))) - (TELEM)1;

	mas.pMem[max(MemLen, bf.MemLen)-1] &= d;

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
