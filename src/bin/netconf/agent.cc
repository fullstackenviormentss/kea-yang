// Copyright (C) 2018 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>
#include <netconf/netconf_log.h>
#include <netconf/agent.h>
#include <netconf/translator_network_ranges.h>

#include <swig/cpp/src/Session.h>

#include <iostream>

//static const char * module_name = "ietf-dhcpv6-server";
//static const char * module_name = "turing-machine";

using namespace std;

namespace isc {
namespace netconf {


  bool shutdown_;

static void
sigint_handler(int signum)
{
  cout << "Signal received" << endl;
  shutdown_ = true;
}


  NetconfAgent::NetconfAgent()
    :config_(""), model_() {
    
  };
  
void NetconfAgent::init(const std::string& config) {
    // do somthing with the config file.
  config_ = config;
  model_ = config;
}

/* Function to print current configuration state.
 * It does so by loading all the items of a session and printing them out. */
static void
print_current_config(S_Session session, const char *module_name)
{
  cout << "print_current_config begin" << endl;
  const int MAX_LEN = 256;
    char select_xpath[MAX_LEN];
    try {
        snprintf(select_xpath, MAX_LEN, "/%s:*//*", module_name);

        auto values = session->get_items(&select_xpath[0]);
        if (values == nullptr)
            return;

        for(unsigned int i = 0; i < values->val_cnt(); i++)
            cout << values->val(i)->to_string();
    } catch( const std::exception& e ) {
      cout << "EXception in print_current_config: " << e.what() << endl;
    }
}

class My_Callback:public Callback {
    public:
    /* Function to be called for subscribed client of given session whenever configuration changes. */
    int module_change(S_Session sess, const char *module_name, sr_notif_event_t event, void *private_ctx)
    {
        cout << "\n\n ========== CONFIG HAS CHANGED, CURRENT RUNNING CONFIG: ==========\n" << endl;

        print_current_config(sess, module_name);
        return SR_ERR_OK;
    }
};


bool NetconfAgent::run() {

  cout << "### Agent::run()" << endl;
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

    shutdown_ = false;

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

void NetconfAgent::run_one() {
    // Implement callbacks here.
}

bool NetconfAgent::connectSysrepo() {
  
  // Connect to sysrepo
  S_Connection conn(new Connection("kea-netconf"));

  // Establish session
  S_Session sess(new Session(conn));

  cout << "\n\n ========== READING STARTUP CONFIG: ==========\n" << endl;
  
  S_Subscribe subscribe(new Subscribe(sess));
  S_Callback cb(new My_Callback());
  
  subscribe->module_change_subscribe(model_.c_str(), cb, nullptr, 0, SR_SUBSCR_DEFAULT | SR_SUBSCR_APPLY_ONLY);
  
  print_current_config(sess, model_.c_str());
  
  cout << "\n\n ========== STARTUP CONFIG APPLIED AS RUNNING ==========\n" << endl;


  // Register callbacks for event changes.
  print_current_config(sess, model_.c_str());

/* loop until ctrl-c is pressed / SIGINT is received */
 signal(SIGINT, sigint_handler);
 while (!shutdown_) {
   sleep(1000);  /* or do some more useful work... */
 }
 

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
