# Generated automatically from Makefile.in by configure.
#***********************************************************************
#
# Makefile
#
# Makefile for Roaring Penguin's Linux PPPoE plugin.
#
# Copyright (C) 2001 Roaring Penguin Software Inc.
#
# This program may be distributed according to the terms of the GNU
# General Public License, version 2 or (at your option) any later version.
#
# $Id: Makefile.linux,v 1.7 2006/06/04 05:07:46 paulus Exp $
#***********************************************************************

DESTDIR = $(INSTROOT)@DESTDIR@
BINDIR = $(DESTDIR)/sbin
LIBDIR = $(DESTDIR)/lib/pppd/$(PPPDVERSION)

PPPDVERSION = $(shell awk -F '"' '/VERSION/ { print $$2; }' ../../patchlevel.h)

INSTALL	= install

# Version is set ONLY IN THE MAKEFILE!  Don't delete this!
VERSION=3.3

ifeq ($(wildcard $(DIR_USERS)/.config),)
include ../../../../.config
else
include $(DIR_USERS)/.config
endif

#include $(DIR_USERS)/.config

ifdef CONFIG_MULTIPLE_WAN_SUPPORT
        CFLAGS += -D MULTI_WAN_SUPPORT
endif
ifdef CONFIG_PPPOE_DISCONNECT_FLOW_PATCH
        CFLAGS += -D PPPOE_DISC_FLOW_PATCH
endif

#COPTS=-O2 -g
#CFLAGS=$(COPTS) -I../../../include/linux
CFLAGS+=-I../../../include/linux --static -s -DCOMPACK_SIZE -Os
CFLAGS += $(EXTRA_CFLAGS)
#all: rp-pppoe.so pppoe-discovery
all: need_modify libplugin.a

need_modify:    
	@echo "" > ./.diff_users_config;

	@if [ -e .users_config ]; then \
		diff .users_config $(DIR_USERS)/.config > .diff_users_config; \
		if [ -s .diff_users_config ]; then \
			cp $(DIR_USERS)/.config .users_config; \
			make    clean; \
		fi; \
	else \
		cp $(DIR_USERS)/.config ./.users_config; \
	fi;

pppoe-discovery: libplugin.a pppoe-discovery.o
	$(CC) -o pppoe-discovery pppoe-discovery.o libplugin.a
	$(STRIP) $@

pppoe-discovery.o: pppoe-discovery.c
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o pppoe-discovery.o pppoe-discovery.c

rp-pppoe.so: libplugin.a plugin.o
	$(CC) -o rp-pppoe.so -shared plugin.o libplugin.a

install: all
	$(INSTALL) -d -m 755 $(LIBDIR)
	$(INSTALL) -s -c -m 4550 rp-pppoe.so $(LIBDIR)
	$(INSTALL) -d -m 755 $(BINDIR)
	$(INSTALL) -s -c -m 555 pppoe-discovery $(BINDIR)

clean:
	rm -f *.o *.so *.a

plugin.o: plugin.c
##	$(CC) '-DRP_VERSION="$(VERSION)"' $(CFLAGS) -I../../..  -c -o plugin.o -fPIC plugin.c
	$(CC) '-DRP_VERSION="$(VERSION)"' $(CFLAGS) -I../../..  -c -o plugin.o plugin.c

libplugin.a: discovery.o if.o common.o debug.o plugin.o
	$(AR) -rc $@ $^

discovery.o: discovery.c
##	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o discovery.o -fPIC discovery.c
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o discovery.o discovery.c

if.o: if.c
##	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o if.o -fPIC if.c
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o if.o if.c

debug.o: debug.c
##	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o debug.o -fPIC debug.c
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o debug.o debug.c

common.o: common.c
##	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o common.o -fPIC common.c
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o common.o common.c

romfs:
##	echo "do nothing here"
#	$(ROMFSINST) /bin/pppoe-discovery

