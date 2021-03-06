/*
Copyright 2010 Aiko Barz

This file is part of torrentkino.

torrentkino is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

torrentkino is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with torrentkino.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CONFIG_H
#define CONFIG_H

#define BUF_SIZE 1024
#define BUF_OFF1 1023
#define DATE_SIZE 32
#define SHA1_SIZE 20

#define TIMEOUT 10

#define TRUE 1
#define FALSE 0

#define GAMEOVER 0
#define RUMBLE 1

#define CONF_PORTMIN 1
#define CONF_PORTMAX 65535

#define CONF_BEQUIET 0
#define CONF_VERBOSE 1

#define CONF_DAEMON 0
#define CONF_CONSOLE 1

#define CONF_EPOLL_MAX_EVENTS 32

#define CONF_USERNAME "nobody"

#define PORT_WWW_USER 8080
#define PORT_WWW_PRIV 80

#ifdef TUMBLEWEED
#define CONF_EPOLL_WAIT 1000
#define LOG_NAME "tumbleweed"
#define CONF_INDEX_NAME "index.html"
#endif

#if TORRENTKINO
#define CONF_EPOLL_WAIT 2000
#define CONF_REALM "open.p2p"
#define NSS_DOMAIN "p2p"
#define TID_SIZE 4
#define TID_SIZE_MAX 20
#define PORT_DHT_DEFAULT 6881
#define PORT_DNS_DEFAULT 6853
#define DNS_ANSWERS_MAX 8
#define DNS_TTL 300

#ifdef IPV6
#define LOG_NAME "tk6"
#define BOOTSTRAP_MCAST "ff0e::1"
#define BOOTSTRAP_DEFAULT "dht.transmissionbt.com"
#define DNS_SERVER "::1"
#elif IPV4
#define LOG_NAME "tk4"
#define BOOTSTRAP_MCAST "224.0.0.252"
#define BOOTSTRAP_DEFAULT "dht.transmissionbt.com"
#define DNS_SERVER "127.0.0.1"
#endif

#endif

typedef unsigned long int ULONG;
typedef unsigned short USHORT;
typedef unsigned int UINT;
typedef unsigned char UCHAR;
typedef long int LONG;

#ifdef IPV6
#define IP_INET AF_INET6
#define IP_SIZE 16
#define IP_SIZE_LIST 128
#define IP_SIZE_META_PAIR 18
#define IP_SIZE_META_PAIR8 144
#define IP_SIZE_META_TRIPLE 38
#define IP_SIZE_META_TRIPLE8 304
#define IP_ADDRLEN INET6_ADDRSTRLEN
typedef struct sockaddr_in6 IP;
typedef struct in6_addr IN_ADDR;
#elif IPV4
#define IP_INET AF_INET
#define IP_SIZE 4
#define IP_SIZE_LIST 32
#define IP_SIZE_META_PAIR 6
#define IP_SIZE_META_PAIR8 48
#define IP_SIZE_META_TRIPLE 26
#define IP_SIZE_META_TRIPLE8 208
#define IP_ADDRLEN INET_ADDRSTRLEN
typedef struct sockaddr_in IP;
typedef struct in_addr IN_ADDR;
#endif

extern int status;

#endif				/* CONFIG_H */
