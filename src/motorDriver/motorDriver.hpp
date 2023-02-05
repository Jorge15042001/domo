#pragma  once
#include <cstddef>

struct MotorResponse{
  bool success;
};

class MotorDriver{
  // if this variable is set to true, it means "position" may not be accuarate and need to be recalibrated
  bool needs_home;
  std::size_t position;
  // MotorDriver();
public:
  MotorResponse goHome();
  MotorResponse goToPosition(const std::size_t position);
  MotorResponse moveSteps(const std::size_t stpes,const int direction);
  MotorResponse moveContinuous(const std::size_t start, const std::size_t end);


};
