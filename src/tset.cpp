// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp)
{
    if (mp <= 0) throw out_of_range("mp should be greater than");
    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.BitField)
{
    MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(bf)
{
    MaxPower = bf.GetLength();
}

TSet::operator TBitField() // преобразование типа к битовому полю
{
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
     return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if ((Elem < 0) || (Elem >= MaxPower)) throw out_of_range("Elem should be greater than zero and less than Bitlen");
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if ((Elem < 0) || (Elem >= MaxPower)) throw out_of_range("Elem should be greater than zero and less than Bitlen");
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if ((Elem < 0) || (Elem >= MaxPower)) throw out_of_range("Elem should be greater than zero and less than Bitlen");
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    BitField = s.BitField;

    MaxPower = s.MaxPower;

    return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
    if (BitField == s.BitField) return 1;
    else return 0;
}

int TSet::operator!=(const TSet& s) const // сравнение
{
    if (BitField == s.BitField) return 0;
    else return 1;
}

TSet TSet::operator+ (const TSet& s) // объединение
{
    TSet res(std::max(MaxPower, s.MaxPower));

    res.BitField = BitField | s.BitField;

    return res;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if ((Elem < 0) || (Elem >= MaxPower)) throw out_of_range("Elem should be greater than zero and less than MaxPower");

    TSet res(*this);

    res.InsElem(Elem);

    return res;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if ((Elem < 0) || (Elem >= MaxPower)) throw out_of_range("Elem should be greater than zero and less than MaxPower");

    TSet res(*this);

    res.DelElem(Elem);

    return res;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    TSet res(s);

    res.BitField = BitField & s.BitField;

    return res;
}

TSet TSet::operator~(void) // дополнение
{
    TSet res(*this);

    res.BitField = ~res.BitField;

    return res;
}

// перегрузка ввода/вывода

istream& operator>> (istream& in, TSet& s) // ввод
{
    int count;
    in >> count;
    if ((count > s.MaxPower) || (count <= 0)) throw out_of_range("count should be less or equal than MaxPower and greater than zero");

    for (int i = 0; i < count; i++)
    {
        int tmp;

        in >> tmp;

        if ((tmp >= s.MaxPower) || (tmp < 0)) throw out_of_range("element should be less than MaxPower and greater or equal than zero");

        s.InsElem(tmp);
    }

}

ostream& operator<<(ostream& out, const TSet& s) //вывод
{
    for (int i = 0; i < s.MaxPower; i++)
        if (s.IsMember(i)) out << i << " ";

    out << "\n";

    return out;
}
