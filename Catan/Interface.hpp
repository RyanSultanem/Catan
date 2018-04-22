#ifndef INTERFACE_HPP
#define INTERFACE_HPP

class Interface
{
public:
   virtual void printBoard() = 0;
   virtual void updateBoard() = 0;
   virtual int getBuildingPlacementPosition(int playerId) = 0;
};


#endif // !INTERFACE_HPP