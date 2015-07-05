#pragma once

#include "Vector3.h"

class Compass {
public:
  // Get the current heading vector, in microteslas (TODO: check this unit)
  virtual Vector3 getHeading() = 0;
};
