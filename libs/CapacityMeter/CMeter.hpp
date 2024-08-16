#pragma once

#include <CapacityMeter/Battery.hpp>
#include <CapacityMeter/measurement/IDevice.hpp>

/**
 * @brief
 *
 */
class CMeter
{
  private:
    Battery battery;
    IMeasurementDevice &dev;

  public:
    CMeter(Profile &profile, IMeasurementDevice &dev) : battery{profile}, dev{dev} {};
    uint8_t procent() const;

    void process();
};
