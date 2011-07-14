// Copyright (C) 2009-2011  Internet Systems Consortium, Inc. ("ISC")
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
// OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#include <config.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h>

#include <cassert>
#include <iostream>

#include <exceptions/exceptions.h>
#include <cc/session.h>
#include <config/ccsession.h>

#include <util/buffer.h>
#include <log/dummylog.h>

#include <dhcp6/spec_config.h>


using namespace std;
using namespace isc::util;
using namespace isc::data;
using namespace isc::cc;
using namespace isc::config;
using namespace isc::util;

namespace {

bool verbose_mode = false;

void
usage() {
    cerr << "Usage:  b10-dhcp6 [-u user] [-v]"
         << endl;
    cerr << "\t-u: change process UID to the specified user" << endl;
    cerr << "\t-v: verbose output" << endl;
    exit(1);
}
} // end of anonymous namespace

int
main(int argc, char* argv[]) {
    int ch;
    const char* uid = NULL;
    bool cache = true;

    while ((ch = getopt(argc, argv, ":nu:v")) != -1) {
        switch (ch) {
        case 'n':
            cache = false;
            break;
        case 'u':
            uid = optarg;
            break;
        case 'v':
            verbose_mode = true;
            isc::log::denabled = true;
            break;
        case '?':
        default:
            usage();
        }
    }

    if (argc - optind > 0) {
        usage();
    }

    int ret = 0;

    // XXX: we should eventually pass io_service here.
#if 0
    Session* cc_session = NULL;
    Session* xfrin_session = NULL;
    Session* statistics_session = NULL;
    bool xfrin_session_established = false; // XXX (see Trac #287)
    bool statistics_session_established = false; // XXX (see Trac #287)
    ModuleCCSession* config_session = NULL;
#endif
    try {
        string specfile;
        if (getenv("B10_FROM_BUILD")) {
            specfile = string(getenv("B10_FROM_BUILD")) +
                "/src/bin/auth/dhcp6.spec";
        } else {
            specfile = string(DHCP6_SPECFILE_LOCATION);
        }

        // auth_server = new AuthSrv(cache, xfrout_client);
        // auth_server->setVerbose(verbose_mode);
        cout << "[b10-dhcp6] Initiating DHCPv6 operation." << endl;

    } catch (const std::exception& ex) {
        cerr << "[b10-dhcp6] Server failed: " << ex.what() << endl;
        ret = 1;
    }

    while (true) {
            sleep(10);
            cout << "[b10-dhcp6] I'm alive." << endl;
    }

    return (ret);
}