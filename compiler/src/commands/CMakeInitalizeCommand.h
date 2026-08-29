// Apart of the BORA Source which uses the TAOSU License
// Check LICENSE.md for more information regarding the BORA license.

/* ? [Common or PDS (Platform Dependent Source)]

 * FileName: CMakeInitalizeCommand.h
 * Title: CMake Initalize Command
 * Author: ?
 * Purpose: Initalizes the build directory for the user cmake project.

 * Compatibility: ?

 * Updates - ?
 * Known issues - ?
 */

#ifndef BORAC_CMAKEINITALIZECOMMAND_H
#define BORAC_CMAKEINITALIZECOMMAND_H

#include "CommandTemplate.h"


class CMakeInitalizeCommand : CommandTemplate {
public:
    std::string name() const override { return "cmakeinit"; }

    CommandResult execute() override;

    std::string description() const override {
        return "Initalize the CMake Build Project";
    }
};


#endif //BORAC_CMAKEINITALIZECOMMAND_H
