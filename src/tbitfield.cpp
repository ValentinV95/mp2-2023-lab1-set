﻿// bitfield.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len <= 0)
        throw exception("uncorrect length");
    BitLen = len;
    if (len % 8 == 0)
        MemLen = len / (8 * sizeof(TELEM));
    else
        MemLen = len / (8 * sizeof(TELEM)) + 1;
    memset(pMem, 0, sizeof(TELEM) * MemLen);
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
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
    MemLen = 0;
    BitLen = 0;
}


int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / (8 * sizeof(TELEM));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n % (8 * sizeof(TELEM)));
}


// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if ((n < 0) || (n >= BitLen))
    {
        throw exception("out of range");
    }
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n < 0) || (n >= BitLen))
    {
        throw exception("out of range");
    }
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if ((n < 0) || (n >= BitLen))
    {
        throw exception("out of range");
    }
    return (pMem[GetMemIndex(n)] & GetMemMask(n));
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this == &bf)
    {
        return *this;
    }
    BitLen = bf.BitLen;
    if (MemLen != bf.MemLen)
    {
        delete[] pMem;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
    }
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;
    for (int i = 0; i < MemLen; i++)
    {
        if (pMem[i] != bf.pMem[i])
            return 0;
    }
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    if (*this == bf)
    {
        return 0;
    };
    return 1;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    if (BitLen != bf.BitLen)
        throw length_error("TBitField objects have different size");
    TBitField tmp(*this);
    for (int i = 0; i < bf.MemLen; i++)
        tmp.pMem[i] |= bf.pMem[i];
    return tmp;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    if (BitLen != bf.BitLen)
        throw length_error("TBitField objects have different size");
    TBitField tmp(*this);
    for (int i = 0; i < bf.MemLen; i++)
        tmp.pMem[i] &= bf.pMem[i];
    return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField temp(BitLen);
    for (int i = 0; i < MemLen; i++)
        temp.pMem[i] = ~pMem[i];
    return temp;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
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

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) {
        if (bf.GetBit(i) == 0)
            ostr << '0';
        else
            ostr << '1';
    }
    return ostr;
}
