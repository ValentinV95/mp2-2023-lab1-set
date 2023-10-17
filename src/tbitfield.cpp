// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len <= 0)
		throw out_of_range("Negative lenght");
	BitLen = len;
	if (len % (8 * sizeof(TELEM) == 0)
		MemLen = len / (8 * sizeof(TELEM));
	else
		MemLen = len / (8 * sizeof(TELEM)) + 1;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
	MemLen = 0;
	BitLen = 0;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if ((n < 0) || (n > BitLen))
			throw out_of_range("the bit doesn't belong to the bit field");
	return n / (8 * sizeof(TELEM));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if ((n < 0) || (n > BitLen))
			throw out_of_range("the bit doesn't belong to the bit field");
	return 1 << (n % (8 * sizeof(TELEM)));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n < 0) || (n > BitLen))
		throw out_of_range("Error, out of range");
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n < 0) || (n > BitLen))
		throw out_of_range("Error, out of range");
	pMem[GetMemIndex(n)] &= ~(GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n < 0) || (n > BitLen))
		throw out_of_range("Error, out of range");
	return (pMem[GetMemIndex(n)] & GetMemMask(n));
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (bf == *this)
		return *this;
	delete[] pMem;
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen)
		return 0;

	for (int i = 0; i < MemLen; i++)
		if (pMem[i] != bf.pMem[i])
			return 0;
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	TBitField res(std::max(BitLen, bf.BitLen));

	if (BitLen < bf.BitLen) 
		res = bf;
	else 
		res = *this;

	for (int i = 0; i < std::min(MemLen, bf.MemLen); i++)
		res.pMem[i] = pMem[i] | bf.pMem[i];

	return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	TBitField res(std::max(BitLen, bf.BitLen));

	const int maxlen = std::max(MemLen, bf.MemLen);
	const int minlen = std::min(MemLen, bf.MemLen);

	for (int i = 0; i < minlen; i++)
		res.pMem[i] = pMem[i] & bf.pMem[i];

	for (int i = minlen; i < maxlen; i++)
		res.pMem[i] = static_cast<TELEM>(0);

	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(BitLen);
	for (int i = 0; i < (MemLen - 1); i++)
		res.pMem[i] = ~pMem[i];
	for (int i = (MemLen - 1) * (8 * sizeof(TELEM)); i < BitLen; i++)
		if (GetBit(i))
			res.ClrBit(i);
		else
			res.SetBit(i);
	
	return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int element;
	for (int i = 0; i < bf.BitLen; i++)
	{
		istr >> element;
		if (element) 
			bf.SetBit(i);
		else 
			bf.ClrBit(i);
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++) {
		if (bf.GetBit(i)) {
			ostr << '1';
		}
		else {
			ostr << '0';
		}
	}
	return ostr;
}
