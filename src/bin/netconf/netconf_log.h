// Copyright (C) 2012-2015,2017 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

/// @file dhcp4_log.h
/// Contains declarations for loggers used by the DHCPv4 server component.

#ifndef NETCONF_LOG_H
#define NETCONF_LOG_H

#include <log/logger_support.h>
#include <log/macros.h>
#include <netconf/netconf_messages.h>

namespace isc {
namespace netconf {

/// @name Constants defining debug levels for logging in netconf server.
//@{

/// @brief Debug level used to log the traces with some basic data.
///
/// The basic data includes summary information, e.g. summary of the
/// information returned by a particular function. It may also include
/// more detailed information in cases when it is warranted and the
/// extraction of the data doesn't impact the server's performance
/// significantly.
const int DBG_NETCONF_BASIC = 0;

//@}

/// @name Constants holding names of loggers for the DHCPv4 server.
//@{

/// @brief Defines the name of the root level (default) logger.
extern const char* NETCONF_ROOT_LOGGER_NAME;

//@}

/// @name Loggers used by the netconf agent
//@{

/// @brief Base logger for DHCPv4 server.
extern isc::log::Logger netconf_logger;

//@}

} // namespace netconf
} // namespace isc

#endif // NETCONF_LOG_H
