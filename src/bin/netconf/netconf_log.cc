// Copyright (C) 2018 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

/// @file dhcp4_log.cc
/// Contains the loggers used by the DHCPv4 server component.

#include <config.h>

#include <netconf/netconf_log.h>

namespace isc {
namespace netconf {

const char* NETCONF_ROOT_LOGGER_NAME = "kea-netconf";
const char* NETCONF_APP_LOGGER_NAME = "netconf";

isc::log::Logger netconf_logger(NETCONF_APP_LOGGER_NAME);

} // namespace netconf
} // namespace isc

