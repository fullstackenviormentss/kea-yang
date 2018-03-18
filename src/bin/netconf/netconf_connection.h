// Copyright (C) 2018 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef NETCONF_CONNECTION_H_
#define NETCONF_CONNECTION_H_

#include <exceptions/exceptions.h>

#include <sysrepo.h>

namespace isc {
namespace netconf {

class NetconfConnectionError : public Exception {
public:
    NetconfConnectionError(const char* file, size_t line, const char* what) :
        isc::Exception(file, line, what) {}
};

class NetconfConnection {
public:
    NetconfConnection();
    virtual ~NetconfConnection();
    void connect();
private:
    sr_conn_ctx_t* connection_;
    sr_session_ctx_t* session_;
};

}  // netconf
}  // isc

#endif /* NETCONF_CONNECTION_H_ */
