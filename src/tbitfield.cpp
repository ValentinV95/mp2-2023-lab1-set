// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len <= 0) {
		throw length_error("negative len");
	}
	BitLen = len;
	MemLen = (BitLen - 1) / (sizeof(TELEM) * 8) + 1;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = 0;
	}
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
	MemLen = 0;
	BitLen = 0;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if ((n < 0) || (n >= BitLen)) {
		throw out_of_range("index out of range");
	}
	return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if ((n < 0) || (n >= BitLen)) {
		throw out_of_range("index out of range");
	}
	return 1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n < 0) || (n >= BitLen)) {
		throw out_of_range("index out of range");
	}
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n < 0) || (n >= BitLen)) {
		throw out_of_range("index out of range");
	}
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n < 0) || (n >= BitLen)) {
		throw out_of_range("index out of range");
	}
	if ((pMem[GetMemIndex(n)] & GetMemMask(n))==0) {
		return 0;
	}
	else {
		return 1;
	}
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this == &bf) {
		return *this;
	}

	BitLen = bf.BitLen;
	if (MemLen != bf.MemLen) {
		delete[] pMem;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];
	}
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen) {
		return 0;
    }
	for (int i = 0; i < MemLen; i++) {
		if (pMem[i] != bf.pMem[i]) {
			return 0;
		}
	}
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (*this == bf) {
		return 0;
    }
	return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	TBitField res(max(this->BitLen, bf.BitLen));
	if (BitLen > bf.BitLen) {
		res = *this;
	}
	else {
		res = bf;
	}
	for (int i = 0; i < min(this->MemLen, bf.MemLen); i++) {
		res.pMem[i] = this->pMem[i] | bf.pMem[i];
	}
	return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	TBitField res(max(this->BitLen, bf.BitLen));
	for (int i = 0; i < min(this->MemLen, bf.MemLen); i++) {
		res.pMem[i] = this->pMem[i] & bf.pMem[i];
	}
	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(BitLen);
	for (int i = 0; i < MemLen - 1; i++) {
		res.pMem[i] = ~pMem[i];
	}
	for (int i = ((MemLen-1)*sizeof(TELEM)*8); i < BitLen; i++) {
		if (GetBit(i) == 0) {
			res.SetBit(i);
		}
		else {
			res.ClrBit(i);
		}
	}
	return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int tmp;
	for (int i = 0; i < bf.BitLen; i++) {
		istr >> tmp;
		if (tmp == 0) {
			bf.ClrBit(i);
		}
		else if (tmp == 1) {
			bf.SetBit(i);
		}
		else {
			throw out_of_range("invalid bit");
		}
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++) {
		ostr << bf.GetBit(i);
	}
	return ostr;
}
