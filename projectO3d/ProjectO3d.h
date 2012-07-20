/*
-----------------------------------------------------------------------------
Filename:    ProjectO3d.h
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#ifndef __ProjectO3d_h_
#define __ProjectO3d_h_

#include "O3dconfig.h"

class ProjectO3d : public O3dconfig
{
public:
    ProjectO3d(void);
    virtual ~ProjectO3d(void);

protected:
    virtual void createScene(void);
};

#endif // #ifndef __ProjectO3d_h_
