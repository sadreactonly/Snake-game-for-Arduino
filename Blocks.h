#ifndef Blocks_h
#define Blocks_h

class Block
{
  public:
  	Block();
  	Block(int x, int y,int _direction);
  	Block(int x, int y);
  	int _x;
  	int _y;
  	int _direction;   
  private:
};
#endif
