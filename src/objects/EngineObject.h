#ifndef ENGINE_OBJECT_H
#define ENGINE_OBJECT_H

// This is going to be the top level GameObject class.
// It doesn't have any real values or methods. It's job is to be
// inherited and have child classes be forced to implement these functions
class EngineObject {

protected:
  virtual void moveLeft(int delta_x) = 0;
  virtual void moveRight(int delta_x) = 0;
  virtual void moveUp(int delta_y) = 0;
  virtual void moveDown(int delta_y) = 0;
  virtual void rotateClockwise() = 0;
  virtual void rotateCounterClockwise() = 0;
};

#endif // !ENGINE_OBJECT_H
