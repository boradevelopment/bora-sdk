// Apart of the BORA Source which uses the TAOSU License
// Check LICENSE.md for more information regarding the BORA license.

/* ? [Common or PDS (Platform Dependent Source)]

 * FileName: CmakeBuildCommand.h
 * Title: ?
 * Author: ?
 * Purpose: ?

 * Compatibility: ?

 * Updates - ?
 * Known issues - ?
 */

#ifndef BORAC_CMAKEBUILDCOMMAND_H
#define BORAC_CMAKEBUILDCOMMAND_H
#include "CommandTemplate.h"


class CmakeBuildCommand  : CommandTemplate {
public:
    std::string name() const override { return "cmakeinit"; }

    CommandResult execute() override;

    std::string description() const override {
        return "Initalize the CMake Build Project";
    }
};


#endif //BORAC_CMAKEBUILDCOMMAND_H
