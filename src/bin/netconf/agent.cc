// Copyright (C) 2018 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>
#include <netconf/netconf_log.h>
#include <netconf/agent.h>
#include <netconf/translator_network_ranges.h>

namespace isc {
namespace netconf {

void NetconfAgent::init(const std::string& config) {
    // do somthing with the config file.
}

bool NetconfAgent::run() {

    // Now define all translators. Each will register specific callback.
    //translators_.push_back(new TranslatorNetworkRanges());

    // Those will be implemented next.
    // translators_.push_back(new TranslatorOptionSets());
    // translators_.push_back(new TranslatorDuid());
    // translators_.push_back(new TranslatorInterfaces());

    // We need to establish connection to the sysrepo first.
    if (!connectSysrepo()) {
        return (false);
    }    

    // Second, we need to establish connection to Kea control socket.
    if (!connectControlSocket()) {
        return (false);
    }

    while (!shutdown_) {
        try {
            run_one();
            io_service_.poll();
        } catch (const std::exception& e) {
            // General catch-all exception that are not caught by more specific
            // catches. This one is for exceptions derived from std::exception.
            LOG_ERROR(netconf_logger, NETCONF_EXCEPTION)
                .arg(e.what());
        }
    }

    return (true);
}

bool NetconfAgent::connectSysrepo() {
    // Connect to sysrepo

    // Establish session

    // Register callbacks for event changes.


    // Once implemented, change this to true.
    return (false);
}

bool NetconfAgent::connectControlSocket() {
    // Establish connection to Kea control socket.

    // Once implemented, change this to true.
    return (false);
}

};
};
