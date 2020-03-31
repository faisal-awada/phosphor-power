/**
 * Copyright © 2020 IBM Corporation
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
#pragma once

#include "chassis.hpp"
#include "id_map.hpp"
#include "rule.hpp"

#include <memory>
#include <utility>
#include <vector>

namespace phosphor::power::regulators
{

/**
 * @class System
 *
 * The computer system being controlled and monitored by the BMC.
 *
 * The system contains one or more chassis.  Chassis are large enclosures that
 * can be independently powered off and on by the BMC.
 */
class System
{
  public:
    // Specify which compiler-generated methods we want
    System() = delete;
    System(const System&) = delete;
    System(System&&) = delete;
    System& operator=(const System&) = delete;
    System& operator=(System&&) = delete;
    ~System() = default;

    /**
     * Constructor.
     *
     * @param rules rules used to monitor and control regulators in the system
     * @param chassis chassis in the system
     */
    explicit System(std::vector<std::unique_ptr<Rule>> rules,
                    std::vector<std::unique_ptr<Chassis>> chassis) :
        rules{std::move(rules)},
        chassis{std::move(chassis)}
    {
    }

    /**
     * Returns the chassis in the system.
     *
     * @return chassis
     */
    const std::vector<std::unique_ptr<Chassis>>& getChassis() const
    {
        return chassis;
    }

    /**
     * Returns the IDMap for the system.
     *
     * The IDMap provides a mapping from string IDs to the associated Device,
     * Rail, and Rule objects.
     *
     * @return IDMap
     */
    const IDMap& getIDMap() const
    {
        return idMap;
    }

    /**
     * Returns the rules used to monitor and control regulators in the system.
     *
     * @return rules
     */
    const std::vector<std::unique_ptr<Rule>>& getRules() const
    {
        return rules;
    }

  private:
    /**
     * Rules used to monitor and control regulators in the system.
     */
    std::vector<std::unique_ptr<Rule>> rules{};

    /**
     * Chassis in the system.
     */
    std::vector<std::unique_ptr<Chassis>> chassis{};

    /**
     * Mapping from string IDs to the associated Device, Rail, and Rule objects.
     */
    IDMap idMap{};
};

} // namespace phosphor::power::regulators