// Apart of the BORA Source which uses the TAOSU License
// Check LICENSE.md for more information regarding the BORA license.

/* Common

 * FileName: InstallCommand.h
 * Title: Install Command
 * Author: Munashe Dirwayi
 * Purpose: The installation of Bora Compilier dependencies.

 * Compatibility: ?

 * Updates - ?
 * Known issues - ?
 */

#ifndef BORAC_INSTALLCOMMAND_H
#define BORAC_INSTALLCOMMAND_H
#include "CommandTemplate.h"


class InstallCommand : CommandTemplate {
public:
    std::string name() const override { return "install"; }

    CommandResult execute() override;

    std::string description() const override {
        return "Installs project dependencies.";
    }
};


#endif //BORAC_INSTALLCOMMAND_H
