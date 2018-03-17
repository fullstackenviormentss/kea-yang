// Copyright (C) 2018 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>
#include <netconf/translator_network_ranges.h>

namespace isc {
namespace netconf {

TranslatorNetworkRanges::TranslatorNetworkRanges(const std::string& xpath)
    :Translator(xpath) {

}

void TranslatorNetworkRanges::translate() {
    // Get the Netconf data from netconf_data_
    // and convert it to a command that is understandable by kea
    //
    // In case of network-ranges, this should translate to
    // a series of:
    // - subnet6-get() - check if the subnet already exists
    // - if it does, call subnet6-del()
    // - call subnet6-add() with new information
}

};
};
