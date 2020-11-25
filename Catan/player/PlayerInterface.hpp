#ifndef PLAYER_INTERFACE_HPP
#define PLAYER_INTERFACE_HPP

namespace player {
 
class Owner
{
public:
   virtual int getId() const = 0;
};

class PointReceiver : public Owner
{
public:
	virtual void receivePoints(int points) = 0;
};

} // namespace player

#endif // !PLAYER_INTERFACE_HPP