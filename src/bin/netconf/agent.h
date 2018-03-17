// Copyright (C) 2018 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef NETCONF_AGENT_H
#define NETCONF_AGENT_H

#include <dhcpsrv/daemon.h>
#include <asiolink/asio_wrapper.h>
#include <asiolink/asiolink.h>
#include <netconf/translator.h>

namespace isc {
namespace netconf {

    class NetconfAgent : public isc::dhcp::Daemon {
 public:
      NetconfAgent();

    void init(const std::string& config);
    
    bool run();

    void run_one();
    
private:

    bool connectSysrepo();

    bool connectControlSocket();

    /// @brief IOService object, used for all ASIO operations.
    isc::asiolink::IOService io_service_;

    std::vector <TranslatorPtr> translators_;

    std::string config_;

    // name of the mode (usually ietf-dhcpv6-server)
    std::string model_;

    //    bool shutdown_;

};

};
};


#endif

