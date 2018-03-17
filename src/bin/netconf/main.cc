// Copyright (C) 2018 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>

#include <dhcpsrv/cfgmgr.h>
#include <netconf/agent.h>
#include <netconf/netconf_log.h>
#include <exceptions/exceptions.h>
#include <iostream>

using namespace std;
using namespace isc::netconf;
using namespace isc::dhcp;

/// @brief Prints Kea Usage and exits
///
/// Note: This function never returns. It terminates the process.
void
usage() {
    cerr << "Kea netconf daemon, version " << VERSION << endl;
    cerr << endl;
    cerr << "Usage: "
         << "  -c: config-file" << endl;
    cerr << "  -v: print version number and exit" << endl;
    cerr << "  -V: print extended version and exit" << endl;
    exit(EXIT_FAILURE);
}

int
main(int argc, char* argv[]) {
    // The standard config file
    std::string config_file("");
    int ch;

    while ((ch = getopt(argc, argv, "vVc:")) != -1) {
        switch (ch) {
        case 'v':
            cout << NetconfAgent::getVersion(false) << endl;
            return (EXIT_SUCCESS);

        case 'V':
            cout << NetconfAgent::getVersion(true) << endl;
            return (EXIT_SUCCESS);

        case 'c': // config file
            config_file = optarg;
            break;
        default:
            usage();
        }
    }

    // Check for extraneous parameters.
    if (argc > optind) {
        usage();
    }

    // Configuration file is required.
    if (config_file.empty()) {
        cerr << "Configuration file not specified." << endl;
        usage();
    }

    //CfgMgr::instance().setFamily(AF_INET);

    int ret = EXIT_SUCCESS;
    try {
        // It is important that we set a default logger name because this name
        // will be used when the user doesn't provide the logging configuration
        // in the Kea configuration file.
        CfgMgr::instance().setDefaultLoggerName(NETCONF_ROOT_LOGGER_NAME);

        // Initialize logging.  If verbose, we'll use maximum verbosity.
        bool verbose_mode = true;
        Daemon::loggerInit(NETCONF_ROOT_LOGGER_NAME, verbose_mode);
        LOG_INFO(netconf_logger, NETCONF_AGENT_STARTING).arg(VERSION).arg(getpid());

        // Create the server instance.
        NetconfAgent agent;

        // Remember verbose-mode
        agent.setVerbose(verbose_mode);

        // Create our PID file.
        //server.setProcName(DHCP4_NAME);
        //server.setConfigFile(config_file);
        //server.createPIDFile();

        try {
            // Initialize the server.
            agent.init(config_file);
        } catch (const std::exception& ex) {
            cerr << "Failed to initialize server: " << ex.what() << endl;
            return (EXIT_FAILURE);
        }

        // Tell the admin we are ready to process packets
        LOG_INFO(netconf_logger, NETCONF_AGENT_STATED).arg(VERSION);

        // And run the main loop of the server.
        agent.run();

        LOG_INFO(netconf_logger, NETCONF_AGENT_SHUTDOWN);

    } catch (const isc::Exception& ex) {
        // First, we parint the error on stderr (that should always work)
        cerr << "ERROR:" << ex.what() << endl;

    }

    return (ret);
}
