#pragma once


class IGroupTable
{
public:
   virtual int Order() const = 0;
   virtual int Identity() const = 0;
   virtual int Inverse(int) const = 0;
   virtual int operator()(int, int) const = 0;

   virtual ~IGroupTable() {};
};
