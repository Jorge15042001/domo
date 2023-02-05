#include "motorDriver.hpp"
#include "motorStructs.hpp"
#include <cstddef>

MotorResponse MotorDriver::goHome() {
  // splin to counter clockwise until the sensor stop activates

  // move clockwise until the sensor deactivates

  // after the node is at home
  this->position = 0.;
}
MotorResponse MotorDriver::goToPosition(const std::size_t postion) {
  if (this->needs_home)
    this->goHome();

  const long steps = position - this->position;

  if (steps == 0)
    return MotorResponse{true};

  if (steps > 0) {
    return this->moveSteps((std::size_t)steps, CLOCK_WISE);
  }

  return this->moveSteps((std::size_t)(steps * -1), COUNTER_CLOCK_wISE);
}
MotorResponse MotorDriver::moveSteps(const std::size_t stpes,
                                     const int direction) {
  if (this->needs_home)
    this->goHome();
  //move steps in the specified direction

  //reply to server

}
MotorResponse MotorDriver::moveContinuous(const std::size_t start,
                                          const std::size_t end) {
  if (this->needs_home)
    this->goHome();
  //  go to the start of the movement
  MotorResponse res = this->goToPosition(start);

  // move until end
  MotorResponse res2 = this->goToPosition(end);

  // reply to server
}
