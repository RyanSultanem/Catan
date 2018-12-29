#ifndef INTERFACE_HPP
#define INTERFACE_HPP

class Interface
{
public:
   virtual void showBoard(const std::string & board) = 0;
   virtual void updateBoard() = 0;
   virtual int getBuildingPlacementPosition(int playerId) = 0;

   virtual void showPlayerInfos(const std::string & playersInfo) = 0;
};


#endif // !INTERFACE_HPP