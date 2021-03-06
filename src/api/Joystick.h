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
 */
#pragma once

#include "kodi/xbmc_peripheral_utils.hpp"

#include <vector>

namespace JOYSTICK
{
  class CJoystickInterface;

  class CJoystick : public ADDON::Joystick
  {
  public:
    CJoystick(CJoystickInterface* api);
    virtual ~CJoystick(void) { }

    /*!
     * The parent API that this joystick was discovered on
     */
    CJoystickInterface* API(void) const { return m_api; }

    /*!
     * The time that this joystick was discovered
     */
    int64_t DiscoverTimeMs(void) const { return m_discoverTimeMs; }

    /*!
     * The time that this joystick delivered its first event
     */
    int64_t FirstEventTimeMs(void) const { return m_firstEventTimeMs; }

    /*!
     * The most recent time that this joystick delivered an event
     */
    int64_t LastEventTimeMs(void) const { return m_lastEventTimeMs; }

    /*!
     * The analog stick deadzone. This is applied to each axis. Axis is scaled
     * appropriately, so position is continuous from -1.0 to 1.0:
     *
     *            |    / 1.0
     *            |   /
     *          __|__/
     *         /  |
     *        /   |--| Deadzone
     *  -1.0 /    |
     *
     * TODO: Disable deadzone for accelerometer
     */
    float DeadzoneRange(void) const { return m_deadzoneRange; }

    /*!
     * Initialize the joystick object. Joystick will be initialized before the
     * first call to GetEvents().
     */
    virtual bool Initialize(void);

    /*!
     * Deinitialize the joystick object. GetEvents() will not be called after
     * deinitialization.
     */
    virtual void Deinitialize(void) { }

    /*!
     * Get events that have occurred since the last call to GetEvents()
     */
    virtual bool GetEvents(std::vector<ADDON::PeripheralEvent>& events);

  protected:
    /*!
     * Implemented by derived class to scan for events
     */
    virtual bool ScanEvents(std::vector<ADDON::PeripheralEvent>& events) = 0;

    /*!
     * Helper functions for derived class to populate event vector from observed
     * button/hat/axis states
     */
    void GetButtonEvents(const std::vector<JOYSTICK_STATE_BUTTON>& buttons, std::vector<ADDON::PeripheralEvent>& events);
    void GetHatEvents(const std::vector<JOYSTICK_STATE_HAT>& hats, std::vector<ADDON::PeripheralEvent>& events);
    void GetAxisEvents(const std::vector<JOYSTICK_STATE_AXIS>& axes, std::vector<ADDON::PeripheralEvent>& events);

    /*!
     * Call this from derived class when events are discovered
     */
    void FoundEvents(void);

    /*!
     * Normalize the axis to the closed interval [-1.0, 1.0] subject to deadzone.
     */
    static float NormalizeAxis(long value, long maxAxisAmount);

    /*!
     * State buffer provided to derived classes
     */
    struct JoystickState
    {
      std::vector<JOYSTICK_STATE_BUTTON> buttons;
      std::vector<JOYSTICK_STATE_HAT>    hats;
      std::vector<JOYSTICK_STATE_AXIS>   axes;
    };

    JoystickState m_stateBuffer;

  private:
    void UpdateTimers(void);

    CJoystickInterface* const m_api;
    JoystickState             m_state;
    int64_t                   m_discoverTimeMs;
    int64_t                   m_firstEventTimeMs;
    int64_t                   m_lastEventTimeMs;
    static const float        m_deadzoneRange; // TODO: Get deadzone from settings
  };
}
