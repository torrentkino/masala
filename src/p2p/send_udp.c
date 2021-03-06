/*
Copyright 2011 Aiko Barz

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/epoll.h>

#include "send_udp.h"

/*
	{
	"t": "aa",
	"y": "q",
	"q": "ping",
	"a": {
		"id": "abcdefghij0123456789"
		}
	}
*/

void send_ping(IP * sa, UCHAR * tid)
{
	BEN *dict = ben_init(BEN_DICT);
	BEN *key = NULL;
	BEN *val = NULL;
	RAW *raw = NULL;
	BEN *arg = ben_init(BEN_DICT);

	/* Node ID */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "id", 2);
	ben_str(val, _main->conf->node_id, SHA1_SIZE);
	ben_dict(arg, key, val);

	/* Argument */
	key = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "a", 1);
	ben_dict(dict, key, arg);

	/* Query type */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "q", 1);
	ben_str(val, (UCHAR *) "ping", 4);
	ben_dict(dict, key, val);

	/* Transaction ID */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "t", 1);
	ben_str(val, tid, TID_SIZE);
	ben_dict(dict, key, val);

	/* Type of message */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "y", 1);
	ben_str(val, (UCHAR *) "q", 1);
	ben_dict(dict, key, val);

	raw = ben_enc(dict);
#ifdef POLARSSL
	if (_main->conf->bool_encryption) {
		send_aes(sa, raw);
	} else {
		send_udp(sa, raw);
	}
#else
	send_udp(sa, raw);
#endif
	raw_free(raw);
	ben_free(dict);

	info(_log, sa, "PING");
}

/*
	{
	"t": "aa",
	"y": "r",
	"r": {
		"id": "mnopqrstuvwxyz123456"
		}
	}
*/

void send_pong(IP * sa, UCHAR * tid, int tid_size)
{
	BEN *dict = ben_init(BEN_DICT);
	BEN *key = NULL;
	BEN *val = NULL;
	RAW *raw = NULL;
	BEN *arg = ben_init(BEN_DICT);

	/* Node ID */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "id", 2);
	ben_str(val, _main->conf->node_id, SHA1_SIZE);
	ben_dict(arg, key, val);

	/* Argument */
	key = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "r", 1);
	ben_dict(dict, key, arg);

	/* Transaction ID */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "t", 1);
	ben_str(val, tid, tid_size);
	ben_dict(dict, key, val);

	/* Type of message */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "y", 1);
	ben_str(val, (UCHAR *) "r", 1);
	ben_dict(dict, key, val);

	raw = ben_enc(dict);
#ifdef POLARSSL
	if (_main->conf->bool_encryption) {
		send_aes(sa, raw);
	} else {
		send_udp(sa, raw);
	}
#else
	send_udp(sa, raw);
#endif
	raw_free(raw);
	ben_free(dict);

	info(_log, sa, "PONG");
}

/*
	{
	"t": "aa",
	"y": "q",
	"q": "find_node",
	"a": {
		"id": "abcdefghij0123456789",
		"target": "mnopqrstuvwxyz123456"
		}
	}
*/

void send_find_node_request(IP * sa, UCHAR * node_id, UCHAR * tid)
{
	BEN *dict = ben_init(BEN_DICT);
	BEN *key = NULL;
	BEN *val = NULL;
	RAW *raw = NULL;
	BEN *arg = ben_init(BEN_DICT);
	char hexbuf[HEX_LEN];

	/* Node ID */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "id", 2);
	ben_str(val, _main->conf->node_id, SHA1_SIZE);
	ben_dict(arg, key, val);

	/* Target */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "target", 6);
	ben_str(val, node_id, SHA1_SIZE);
	ben_dict(arg, key, val);

	/* Argument */
	key = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "a", 1);
	ben_dict(dict, key, arg);

	/* Query type */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "q", 1);
	ben_str(val, (UCHAR *) "find_node", 9);
	ben_dict(dict, key, val);

	/* Transaction ID */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "t", 1);
	ben_str(val, tid, TID_SIZE);
	ben_dict(dict, key, val);

	/* Type of message */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "y", 1);
	ben_str(val, (UCHAR *) "q", 1);
	ben_dict(dict, key, val);

	raw = ben_enc(dict);
#ifdef POLARSSL
	if (_main->conf->bool_encryption) {
		send_aes(sa, raw);
	} else {
		send_udp(sa, raw);
	}
#else
	send_udp(sa, raw);
#endif
	raw_free(raw);
	ben_free(dict);

	hex_hash_encode(hexbuf, node_id);
	info(_log, sa, "FIND_NODE %s at", hexbuf);
}

/*
	{
	"t": "aa",
	"y": "r",
	"r": {
		"id":"0123456789abcdefghij",
		"nodes": "def456..."
		}
	}
*/

void send_find_node_reply(IP * sa, UCHAR * nodes_compact_list,
			  int nodes_compact_size, UCHAR * tid, int tid_size)
{

	BEN *dict = ben_init(BEN_DICT);
	BEN *key = NULL;
	BEN *val = NULL;
	RAW *raw = NULL;
	BEN *arg = ben_init(BEN_DICT);

	/* Node ID */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "id", 2);
	ben_str(val, _main->conf->node_id, SHA1_SIZE);
	ben_dict(arg, key, val);

	/* Nodes */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
#ifdef IPV6
	ben_str(key, (UCHAR *) "nodes6", 6);
#elif IPV4
	ben_str(key, (UCHAR *) "nodes", 5);
#endif
	ben_str(val, nodes_compact_list, nodes_compact_size);
	ben_dict(arg, key, val);

	/* Argument */
	key = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "r", 1);
	ben_dict(dict, key, arg);

	/* Transaction ID */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "t", 1);
	ben_str(val, tid, tid_size);
	ben_dict(dict, key, val);

	/* Type of message */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "y", 1);
	ben_str(val, (UCHAR *) "r", 1);
	ben_dict(dict, key, val);

	raw = ben_enc(dict);
#ifdef POLARSSL
	if (_main->conf->bool_encryption) {
		send_aes(sa, raw);
	} else {
		send_udp(sa, raw);
	}
#else
	send_udp(sa, raw);
#endif
	raw_free(raw);
	ben_free(dict);

	info(_log, sa, "NODES_FN to");
}

/*
	{
	"t": "aa",
	"y": "q",
	"q": "get_peers",
	"a": {
		"id": "abcdefghij0123456789",
		"info_hash": "mnopqrstuvwxyz123456"
		}
	}
*/

void send_get_peers_request(IP * sa, UCHAR * node_id, UCHAR * tid)
{
	BEN *dict = ben_init(BEN_DICT);
	BEN *key = NULL;
	BEN *val = NULL;
	RAW *raw = NULL;
	BEN *arg = ben_init(BEN_DICT);
	char hexbuf[HEX_LEN];

	/* Node ID */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "id", 2);
	ben_str(val, _main->conf->node_id, SHA1_SIZE);
	ben_dict(arg, key, val);

	/* info_hash */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "info_hash", 9);
	ben_str(val, node_id, SHA1_SIZE);
	ben_dict(arg, key, val);

	/* Argument */
	key = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "a", 1);
	ben_dict(dict, key, arg);

	/* Query type */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "q", 1);
	ben_str(val, (UCHAR *) "get_peers", 9);
	ben_dict(dict, key, val);

	/* Transaction ID */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "t", 1);
	ben_str(val, tid, TID_SIZE);
	ben_dict(dict, key, val);

	/* Type of message */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "y", 1);
	ben_str(val, (UCHAR *) "q", 1);
	ben_dict(dict, key, val);

	raw = ben_enc(dict);
#ifdef POLARSSL
	if (_main->conf->bool_encryption) {
		send_aes(sa, raw);
	} else {
		send_udp(sa, raw);
	}
#else
	send_udp(sa, raw);
#endif
	raw_free(raw);
	ben_free(dict);

	hex_hash_encode(hexbuf, node_id);
	info(_log, sa, "GET_PEERS %s at", hexbuf);
}

/*
	{
	"t": "aa",
	"y": "r",
	"r": {
		"id":"abcdefghij0123456789",
		"token":"aoeusnth",
		"nodes": "def456..."
		}
	}
*/

void send_get_peers_nodes(IP * sa, UCHAR * nodes_compact_list,
			  int nodes_compact_size, UCHAR * tid, int tid_size)
{

	BEN *dict = NULL;
	BEN *key = NULL;
	BEN *val = NULL;
	RAW *raw = NULL;
	BEN *arg = NULL;

	dict = ben_init(BEN_DICT);
	arg = ben_init(BEN_DICT);

	/* Node ID */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "id", 2);
	ben_str(val, _main->conf->node_id, SHA1_SIZE);
	ben_dict(arg, key, val);

	/* Nodes */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
#ifdef IPV6
	ben_str(key, (UCHAR *) "nodes6", 6);
#elif IPV4
	ben_str(key, (UCHAR *) "nodes", 5);
#endif
	ben_str(val, nodes_compact_list, nodes_compact_size);
	ben_dict(arg, key, val);

	/* Token */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "token", 5);
	ben_str(val, tkn_read(), TOKEN_SIZE);
	ben_dict(arg, key, val);

	/* Argument */
	key = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "r", 1);
	ben_dict(dict, key, arg);

	/* Transaction ID */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "t", 1);
	ben_str(val, tid, tid_size);
	ben_dict(dict, key, val);

	/* Type of message */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "y", 1);
	ben_str(val, (UCHAR *) "r", 1);
	ben_dict(dict, key, val);

	raw = ben_enc(dict);
#ifdef POLARSSL
	if (_main->conf->bool_encryption) {
		send_aes(sa, raw);
	} else {
		send_udp(sa, raw);
	}
#else
	send_udp(sa, raw);
#endif
	raw_free(raw);
	ben_free(dict);

	info(_log, sa, "NODES_GP to");
}

/*
	{
	"t": "aa",
	"y": "r",
	"r": {
		"id":"abcdefghij0123456789",
		"token":"aoeusnth",
		"values": ["axje.u", "idhtnm"]
		}
	}
*/

void send_get_peers_values(IP * sa, UCHAR * nodes_compact_list,
			   int nodes_compact_size, UCHAR * tid, int tid_size)
{
	BEN *dict = NULL;
	BEN *list = NULL;
	BEN *arg = NULL;
	BEN *key = NULL;
	BEN *val = NULL;
	RAW *raw = NULL;
	UCHAR *p = nodes_compact_list;
	int j = 0;

	dict = ben_init(BEN_DICT);
	list = ben_init(BEN_LIST);
	arg = ben_init(BEN_DICT);

	/* Values list */
	for (j = 0; j < nodes_compact_size; j += IP_SIZE_META_PAIR) {
		val = ben_init(BEN_STR);
		ben_str(val, p, IP_SIZE_META_PAIR);
		ben_list(list, val);
		p += IP_SIZE_META_PAIR;
	}

	/* Node ID */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "id", 2);
	ben_str(val, _main->conf->node_id, SHA1_SIZE);
	ben_dict(arg, key, val);

	/* Token */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "token", 5);
	ben_str(val, tkn_read(), TOKEN_SIZE);
	ben_dict(arg, key, val);

	/* Values */
	key = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "values", 6);
	ben_dict(arg, key, list);

	/* Argument */
	key = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "r", 1);
	ben_dict(dict, key, arg);

	/* Transaction ID */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "t", 1);
	ben_str(val, tid, tid_size);
	ben_dict(dict, key, val);

	/* Type of message */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "y", 1);
	ben_str(val, (UCHAR *) "r", 1);
	ben_dict(dict, key, val);

	raw = ben_enc(dict);
#ifdef POLARSSL
	if (_main->conf->bool_encryption) {
		send_aes(sa, raw);
	} else {
		send_udp(sa, raw);
	}
#else
	send_udp(sa, raw);
#endif
	raw_free(raw);
	ben_free(dict);

	info(_log, sa, "VALUES_GP to");
}

/*
{
	"t": "aa",
	"y": "q",
	"q": "announce_peer",
	"a": {
		"id": "abcdefghij0123456789",
		"info_hash": "mnopqrstuvwxyz123456",
		"port": 8080,
		"token": "aoeusnth"
		}
	}
*/

void send_announce_request(IP * sa, UCHAR * tid, UCHAR * target,
			   UCHAR * token, int token_size)
{

	BEN *dict = ben_init(BEN_DICT);
	BEN *key = NULL;
	BEN *val = NULL;
	RAW *raw = NULL;
	BEN *arg = ben_init(BEN_DICT);

	/* Node ID */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "id", 2);
	ben_str(val, _main->conf->node_id, SHA1_SIZE);
	ben_dict(arg, key, val);

	/* My host ID ot my group ID */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "info_hash", 9);
	ben_str(val, target, SHA1_SIZE);
	ben_dict(arg, key, val);

	/* Port */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_INT);
	ben_str(key, (UCHAR *) "port", 4);
	ben_int(val, _main->conf->announce_port);
	ben_dict(arg, key, val);

	/* Token */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "token", 5);
	ben_str(val, token, token_size);
	ben_dict(arg, key, val);

	/* Argument */
	key = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "a", 1);
	ben_dict(dict, key, arg);

	/* Query type */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "q", 1);
	ben_str(val, (UCHAR *) "announce_peer", 13);
	ben_dict(dict, key, val);

	/* Transaction ID */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "t", 1);
	ben_str(val, tid, TID_SIZE);
	ben_dict(dict, key, val);

	/* Type of message */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "y", 1);
	ben_str(val, (UCHAR *) "q", 1);
	ben_dict(dict, key, val);

	raw = ben_enc(dict);
#ifdef POLARSSL
	if (_main->conf->bool_encryption) {
		send_aes(sa, raw);
	} else {
		send_udp(sa, raw);
	}
#else
	send_udp(sa, raw);
#endif
	raw_free(raw);
	ben_free(dict);

	info(_log, sa, "ANNOUNCE_PEER to");
}

/*
	{
	"t": "aa",
	"y": "r",
	"r": {
		"id": "mnopqrstuvwxyz123456"
		}
	}
*/

void send_announce_reply(IP * sa, UCHAR * tid, int tid_size)
{
	BEN *dict = ben_init(BEN_DICT);
	BEN *key = NULL;
	BEN *val = NULL;
	RAW *raw = NULL;
	BEN *arg = ben_init(BEN_DICT);

	/* Node ID */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "id", 2);
	ben_str(val, _main->conf->node_id, SHA1_SIZE);
	ben_dict(arg, key, val);

	/* Argument */
	key = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "r", 1);
	ben_dict(dict, key, arg);

	/* Transaction ID */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "t", 1);
	ben_str(val, tid, tid_size);
	ben_dict(dict, key, val);

	/* Type of message */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "y", 1);
	ben_str(val, (UCHAR *) "r", 1);
	ben_dict(dict, key, val);

	raw = ben_enc(dict);
#ifdef POLARSSL
	if (_main->conf->bool_encryption) {
		send_aes(sa, raw);
	} else {
		send_udp(sa, raw);
	}
#else
	send_udp(sa, raw);
#endif
	raw_free(raw);
	ben_free(dict);

	info(_log, sa, "ANNOUNCE SUCCESS to");
}

#ifdef POLARSSL
void send_aes(IP * sa, RAW * raw)
{
	BEN *dict = ben_init(BEN_DICT);
	BEN *key = NULL;
	BEN *val = NULL;
	struct obj_str *aes = NULL;
	RAW *enc = NULL;
	UCHAR salt[AES_IV_SIZE];

	/*
	   1:a[es] XX:LENGTH
	   1:s[alt] 32:SALT
	 */

	/* Create random salt */
	rand_urandom(salt, AES_IV_SIZE);

	/* Encrypt message */
	aes = aes_encrypt(raw->code, raw->size, salt,
			  _main->conf->key, strlen(_main->conf->key));
	if (aes == NULL) {
		info(_log, NULL, "Encoding AES message failed");
		ben_free(dict);
		return;
	}

	/* AES */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "a", 1);
	ben_str(val, aes->s, aes->i);
	ben_dict(dict, key, val);

	/* Salt */
	key = ben_init(BEN_STR);
	val = ben_init(BEN_STR);
	ben_str(key, (UCHAR *) "s", 1);
	ben_str(val, salt, AES_IV_SIZE);
	ben_dict(dict, key, val);

	enc = ben_enc(dict);
	send_udp(sa, enc);
	raw_free(enc);
	ben_free(dict);
	str_free(aes);
}
#endif

void send_udp(IP * sa, RAW * raw)
{
	socklen_t addrlen = sizeof(IP);

	if (_main->udp->sockfd < 0) {
		return;
	}

	sendto(_main->udp->sockfd, raw->code, raw->size, 0,
	       (const struct sockaddr *)sa, addrlen);
}
