// Apart of the BORA Source which uses the TAOSU License
// Check LICENSE.md for more information regarding the BORA license.

/* ? [Common or PDS (Platform Dependent Source)]

 * FileName: BuildCommand.h
 * Title: ?
 * Author: ?
 * Purpose: ?

 * Compatibility: ?

 * Updates - ?
 * Known issues - ?
 */

#ifndef BORAC_BUILDCOMMAND_H
#define BORAC_BUILDCOMMAND_H
#include "CommandTemplate.h"


class BuildCommand : CommandTemplate {
public:
    std::string name() const override { return "build"; }

    CommandResult execute() override;

    std::string description() const override {
        return "Builds your C++ code into a bora formatted machine code";
    }
};

#endif //BORAC_BUILDCOMMAND_H
