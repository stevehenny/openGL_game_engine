#ifndef ENGINE_OBJECT_H
#define ENGINE_OBJECT_H

// This is going to be the top level GameObject class.
// It doesn't have any real values or methods. It's job is to be
// inherited and have child classes be forced to implement these functions
class EngineObject {

public:
  virtual void moveHorizontally(float delta_x) = 0;
  virtual void moveVertically(float delta_Y) = 0;
  virtual void rotateClockwise() = 0;
  virtual void rotateCounterClockwise() = 0;
};

#endif // !ENGINE_OBJECT_H
