/*
 *      Copyright (C) 2014 Garrett Brown
 *      Copyright (C) 2014 Team XBMC
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "kodi/threads/mutex.h"
#include "kodi/xbmc_peripheral_utils.hpp"

#include <vector>

namespace JOYSTICK
{
  class CJoystick;
  class CJoystickInterface;

  class CJoystickManager
  {
  private:
    CJoystickManager(void) : m_nextJoystickIndex(0) { }

  public:
    static CJoystickManager& Get(void);
    virtual ~CJoystickManager(void) { Deinitialize(); }

    bool Initialize(void);
    void Deinitialize(void);

    bool PerformJoystickScan(std::vector<CJoystick*>& joysticks);

    CJoystick* GetJoystick(unsigned int index) const;

    /*!
    * @brief Get all events that have occurred since the last call to GetEvents()
    */
    bool GetEvents(std::vector<ADDON::PeripheralEvent>& events);

  private:
    std::vector<CJoystickInterface*> m_interfaces;
    std::vector<CJoystick*>          m_joysticks;
    unsigned int                     m_nextJoystickIndex;
    mutable PLATFORM::CMutex         m_joystickMutex;
  };
}
