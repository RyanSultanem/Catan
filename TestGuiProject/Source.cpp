#include "../Catan/Interface.hpp"

#include <iostream>

class InterfaceStub : public Interface
{
public:
   void printBoard() override
   {
      std::cout << "printBoard" << std::endl;
   }
   void updateBoard() override
   {

   }
};

int main()
{
   Interface * interface = new InterfaceStub();
   interface->printBoard();

   system("PAUSE");

   return 0;
}