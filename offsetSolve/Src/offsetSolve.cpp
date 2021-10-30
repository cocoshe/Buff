#include"offsetSolve.h"

offsetSolve::offsetSolve(Point2f center, double dis)
{
    _offset = Point2f(0,0);
    _centerPoint = center;
    _distance = dis;
    _delayTime = dis / BULLET_SPEED;
}



Point2f offsetSolve::getoffset()
{
    //_offset.x = -10;
    // if(_distance < 2)
    // {
    //      _offset.y =  _distance *  90;
    // }
    // else if(_distance < 0.1)
    // {
    //     _offset.y =    100;
    // }
    // else{
    //     _offset.y =    100;
    // }
    _offset.x = -15;  //-20
    //if(_distance  < 1)
    //{
     //   _offset.y = 170;
    // }}
    // else  if(_distance < 3)
    // {
    //     _offset.y = 175;
    // }
    // else
    
    // else  if(_distance < 3)
    // {
    //     _offset.y = 175;
    // }
    // else
    // {
        _offset.y = 130;        
        if(_distance<0.5)
        {
           _offset.y=310;
        }
        else
        {
        _offset.y+=_distance*3;
        }
        //_offset.y+=7.0;
    //}

    return _offset;
}
