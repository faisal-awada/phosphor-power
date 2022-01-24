/**
 * Copyright © 2021 IBM Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "power_sequencer_monitor.hpp"

#include <fmt/format.h>

#include <phosphor-logging/elog.hpp>
#include <phosphor-logging/log.hpp>

#include <exception>
#include <map>

namespace phosphor::power::sequencer
{

using namespace phosphor::logging;

PowerSequencerMonitor::PowerSequencerMonitor(sdbusplus::bus::bus& bus) :
    bus(bus)
{}

void PowerSequencerMonitor::logError(
    const std::string& message,
    std::map<std::string, std::string>& additionalData)
{
    try
    {
        auto method = bus.new_method_call(
            "xyz.openbmc_project.Logging", "/xyz/openbmc_project/logging",
            "xyz.openbmc_project.Logging.Create", "Create");

        // Add PID to AdditionalData
        additionalData.emplace("_PID", std::to_string(getpid()));

        method.append(message,
                      sdbusplus::xyz::openbmc_project::Logging::server::Entry::
                          Level::Critical,
                      additionalData);
        bus.call_noreply(method);
    }
    catch (const std::exception& e)
    {
        log<level::ERR>(
            fmt::format("Unable to log error, message: {}, error {}", message,
                        e.what())
                .c_str());
    }
}

void PowerSequencerMonitor::onFailure(bool timeout,
                                      const std::string& powerSupplyError)
{
    std::map<std::string, std::string> additionalData{};
    if (!powerSupplyError.empty())
    {
        // Default to power supply error
        logError(powerSupplyError, additionalData);
    }
    else if (timeout)
    {
        // Default to timeout error
        logError("xyz.openbmc_project.Power.Error.PowerOnTimeout",
                 additionalData);
    }
    else
    {
        // Default to generic pgood error
        logError("xyz.openbmc_project.Power.Error.Shutdown", additionalData);
    }
}

} // namespace phosphor::power::sequencer