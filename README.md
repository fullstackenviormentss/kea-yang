# Kea with NETCONF interface

This is an experimental Kea extension that will eventually provide NETCONF/YANG
interface. It is a result of two IETF hackathons.

This description below contain some instructions how to set up an environment that
allows doing experiments with Sysrepo plugin that is able to receive
configuration from Sysrepo and send it to Kea.

## Install sysrepo and netopeer2 (also docker tips) ##

See http://kea.isc.org/wiki/NetconfHackathon for details.

In general, installing docker is simple, however we encountered some problems with connectivity to running containers and supposedly the problem is resolved in docker CE. The engineers involved in this hackathon are not docker experts by any stretch of that definition, so it's entirely possible we did something incorrectly and regular docker would suffice.

Nevertheless, the instructions we followed were taken from here:
https://docs.docker.com/install/linux/docker-ce/ubuntu/#set-up-the-repository:

Allow apt repositories over HTTPS:
```bash
sudo apt-get install \
    apt-transport-https \
    ca-certificates \
    curl \
    software-properties-common
```

Add docker gpg key:
```bash
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
```

Install docker CE
```bash
sudo apt-get update
sudo apt-get install docker-ce
```

```bash
apt-get install docker.io
docker pull sysrepo/sysrepo-netopeer2:latest
docker run -v/path/on/your/host:/opt/dev sysrepo/sysrepo-netopeer2:latest
```

You can then log into the system using:
```bash
# docker ps
<see the container id>
docker exec -it <container id> /bin/bash
```

If you want to keep your changes in the container, commit them:
```bash
docker commit <containerid> sysrepo-netopeer2:kea1
```

## Importing DHCPv6 YANG models into sysrepo

Get the rfcstrip tool from http://yang-central.org
(also in the src/bin/netconf dir now.)

```bash
rfcstrip draft-ietf-dhc-dhcpv6-yang-06.txt
```

This will give you 5 models:
1. [ietf-dhcpv6-server](blob/master/src/bin/netconf/ietf-dhcpv6-server.yang)
1. [ietf-dhcpv6-options](blob/master/src/bin/netconf/ietf-dhcpv6-options.yang)
1. [ietf-dhcpv6-types](blob/master/src/bin/netconf/ietf-dhcpv6-types.yang)
1. [ietf-dhcpv6-client](blob/master/src/bin/netconf/ietf-dhcpv6-client.yang)
1. [ietf-dhcpv6-relay](blob/master/src/bin/netconf/ietf-dhcpv6-relay.yang)

Those models are checked into this repository.

## Install and setup Kea

1. Download kea-yang code:

```bash
git clone https://github.com/isc-projects/kea-yang
autoreconf -i
./configure --with-sysrepo=tools/sysrepo_config
make
```

You may need to first edit tools/sysrepo_config_defines.sh to match your path.

Refer to Kea User's Guide to details.

2. Run Kea-neconf

There's a new daemon in src/bin/netconf called kea-netconf. You can run it with:
```bash
$ ./kea-netconf -c ietf-dhcpv6-server
```

As of today, the code should be able to retrieve the netconf configuration.

You may use one of the examples from kea-dhcp6-netconf.conf in src/bin/netconf/configs as a
template. One item necessary for Sysrepo integration to work is to
have control socket defined. For example:

```json
      "control-socket": {
          "socket-type": "unix",
          "socket-name": "/tmp/kea-dhcp6-ctrl.sock"
      },
```

This is the location where Kea will create its unix domain socket and
will await commands. This has to match to whatever is specified in
sysrepo-plugin-kea.

3. Run Kea with:
```bash
cd src/bin/dhcp6
./kea-dhcp6 -c your-kea-config-file.json
```

## Using sysrepo with docker containers

1. Install latest docker version.

On ubuntu just follow the steps in:

https://docs.docker.com/install/linux/docker-ce/ubuntu/

Eg:
```bash
sudo apt-get remove docker docker-engine docker.io

sudo apt-get install \
    apt-transport-https \
    ca-certificates \
    curl \
    software-properties-common

curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -

sudo add-apt-repository \
   "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
   $(lsb_release -cs) \
   stable"

sudo apt-get update

sudo apt-get install docker-ce
```
2. Pull the container

choose the latest stable repo:
```bash
docker pull sysrepo/sysrepo-netopeer2:latest
```
or use the development version:
```bash
docker pull sysrepo/sysrepo-netopeer2:devel
```

3. Prepare the container
 - name the container and start it:
```bash
docker run -d --name sysrepo -v /local/persistent_path:/mnt sysrepo/sysrepo-netopeer2
```
 - connect to the container:
'''bash
docker exec -it sysrepo bash
'''
 - set the root password:
```bash
passwd
```
 - list installed yang modules inside netopeer2-server (the storage server):
```bash
sysrepoctl -l
```
 - install the dhcpv6 server model (ietf-dhcpv6-types.yang, ietf-dhcpv6-options.yang, ietf-dhcpv6-server.yang):
```bash
cd /path/to/yang/models/to/install

sysrepoctl -i -g ietf-dhcpv6-server.yang -s /etc/sysrepo/yang/ -s ./
```
 - list installed yang modules inside netopeer2-server to see the new modules installed successfully:
```bash
sysrepoctl -l
```
 - connect to the netopeer2-server with netopeer2-cli and display content:
```bash
netopeer2-cli
 -> connect
Interactive SSH Authentication
Type your password:
Password:
 -> get
```

4. Save the container state
```bash
docker commit sysrepo sysrepo/sysrepo-netopeer2:updated
```


## Old steps below (need cleanup)

5. List all yang modules
```bash
sysrepoctl -l
```

6. Install Kea YANG module into the sysrepo 
```bash
sysrepoctl --install --yang=ietf-kea-dhcpv6@2016-07-16.yang
```
7. Verify it's actually installed
```bash
sysrepoctl -l
```

8. Set the configuration. There are three ways to do it:
8.1: Manually (see step 11)
8.2: Run basic_config is a tool that sets first two leafs in the model.
     ./basic_config
8.3: There is an example in kea-configs/kea-cfg1-basic.xml. It can be
loaded with:

```bash
sysrepocfg --import=kea-cfg1-basic.xml --datastore=startup ietf-kea-dhcpv6
```

9. Start sysrepo daemon
```bash
sysrepo -l 4 -d
```
(-l 4 is logging level 4, the most verbose, -d means it stays in the
foreground, so you can see what's going on)

10. Start sysrepo-plugind - daemon that loads plugins from /usr/local/lib/sysrepo/plugins

```bash
sysrepo-plugind -l 4 -d
```
(make sure it prints that the plugin-kea is loaded) The callback
installed with sr_module_change_subscribe should be called.  This call
back should apply configuration to Kea.

At this stage the sysrepo-plugind should print out messages from
libplugin-kea that retrieves the configuration and sends it over the
unix socket to Kea.

Kea should print out messages about receiving
set-config command:

```
2016-07-17 15:36:56.699 INFO  [kea-dhcp6.commands/14236] COMMAND_RECEIVED Received command 'set-config'
```

## Extra steps

The following steps are not needed to have the whole setup
operational. They're useful to conduct certain additional
tasks.

11. Configuration can be inspected and edited with with:
```bash
sysrepocfg --editor=emacs --datastore=running ietf-kea-dhcpv6
```

If the configuration changes, the plugin should be notified.
The callback should get the new configuration and send it
to Kea.

12. Export current model configuration to a file:
```bash
sysrepocfg --export=/tmp/backup.json --format=json --datastore=startup  ietf-kea-dhcpv6
```


---------------------

Tools that may be useful to look at:
Yang Explorer - GUI tool for browsing and editing YANG,
requires flash and it's tricky to set up.
