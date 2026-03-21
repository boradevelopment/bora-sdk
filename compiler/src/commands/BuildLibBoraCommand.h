// This header is apart of the BORA Source
// Check LICENSE.md for more information regarding the BORA license.
/* 
 * FileName: BuildLibBoraCommand.h
 * Purpose: Designated command for building LibBora automatically in command.
 */

#pragma once
#include "CommandTemplate.h"

class BuildLibBoraCommand : CommandTemplate {
public:
    [[nodiscard]] std::string name() const override { return "buildlibbora"; }

    CommandResult execute() override;

    [[nodiscard]] std::string description() const override {
        return "Builds LibBora [Requires full Bora Source]";
    }
};
