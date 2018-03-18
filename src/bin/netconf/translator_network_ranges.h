// Copyright (C) 2018 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef TRANSLATOR_NETWORK_RANGES_H
#define TRANSLATOR_NETWORK_RANGES_H

#include <config.h>

#include <netconf/translator.h>

namespace isc {
namespace netconf {

class TranslatorNetworkRanges : public Translator {
public:
    TranslatorNetworkRanges(NetconfConnection& connection, const std::string& xpath);
    virtual ~TranslatorNetworkRanges();
    static int configChanged();
};

};
};

#endif /* TRANSLATOR_NETWORK_RANGES_H */
