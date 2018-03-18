// Copyright (C) 2018 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "netconf_connection.h"

namespace isc {
namespace netconf {

NetconfConnection::NetconfConnection() : connection_(NULL), session_(NULL) {
}

NetconfConnection::~NetconfConnection() {
    if (session_) {
        sr_session_stop(session_);
    }
    if (connection_) {
        sr_disconnect(connection_);
    }
}

void
NetconfConnection::connect() {
    int rc = SR_ERR_OK;

    rc = sr_connect("netconf connection", SR_CONN_DEFAULT, &connection_);
    if (SR_ERR_OK != rc) {
        isc_throw(NetconfConnectionError,
                "NetconfConnection::connect(): can not create connection");
    }

    /* start session */
    rc = sr_session_start(connection_, SR_DS_STARTUP, SR_SESS_DEFAULT, &session_);
    if (SR_ERR_OK != rc) {
        sr_disconnect(connection_);
        connection_ = NULL;
        isc_throw(NetconfConnectionError,
                "NetconfConnection::connect(): can not create session");
    }
}

}
}
