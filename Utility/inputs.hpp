#pragma once

class moveDirection
{
public:
    bool Neutral : 1;
    bool Up : 1;
    bool Down : 1;
    bool Left : 1;
    bool Right : 1;
    bool Jump : 1;
    bool Run : 1;
    bool Attack : 1;
    bool Special : 1;
    moveDirection() : Neutral(false), Up(false), Down(false), Left(false), Right(false), Jump(false), Run(false), Attack(false), Special(false) {}
};



