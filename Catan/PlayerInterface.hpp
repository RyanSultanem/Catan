#ifndef PLAYER_INTERFACE_HPP
#define PLAYER_INTERFACE_HPP

namespace player {
 
class Owner
{
public:
   virtual int getId() const = 0;
};

}

#endif // !PLAYER_INTERFACE_HPP