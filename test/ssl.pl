#!/usr/bin/perl

use strict;
use warnings;
use IO::Socket::SSL;
use File::Basename;
use lib dirname(__FILE__);
use ircunittest;

# ############################################# #
# Test ssl server                               #
# ############################################# #

# Start server
# ircunittest::start_server();

#
# Test clients connections
#

my $HOST = '127.0.0.1';
my $PORT = '5555';
my $CLIENTS_NUMBER = 2;
my $POOLS_NUMBER = 2;
my $SSL_cert_file = '.cert/server.crt';

my @s;

print "Connecting $CLIENTS_NUMBER clients\n";
for (my $i = 0; $i <= $CLIENTS_NUMBER; $i++) {
    # create a connecting socket    
    my $tmp_s = new IO::Socket::SSL (
        PeerHost => $HOST,
        PeerPort => $PORT,
        Proto => 'tcp',
    );
    die "Couldn't connect to $HOST:$PORT : $SSL_ERROR\n" unless $tmp_s;

    # $tmp_s->setsockopt(SOL_SOCKET, SO_RCVTIMEO, pack('l!l!', 10, 0));

    print $tmp_s;

    # All clients joining specific channel
    print $tmp_s "NICK client_$i\x0D\x0AUSER client$i microsoft.com :Client $i\x0D\x0A";

    push @s, $tmp_s
}

my $s0 = $s[0];

#
# All client leave their channel
#

print "Joining clients channel\n";
for (my $i = 0; $i <= $CLIENTS_NUMBER; $i++) {
    # All clients joining their created channel
        my $si = $s[$i];
        print $si "JOIN #channel_$i\x0D\x0A";
}

# Test WHOIS query
sleep(2);

print $s0 "WHOIS client_3\x0D\x0AWHOIS client_5,client_8\x0D\x0A";
sleep(4);

# $s[0]->recv(my $response, 2048);
# print $response;

# # Test WHO query
# $s[0]->send("WHO client_3\x0D\x0AWHO\x0D\x0A");
# sleep(4);

# $s[0]->recv($response, 2048);
# print $response;

#
# Terminate clients
#

print "Closing clients\n";
for (my $i = 0; $i <= $CLIENTS_NUMBER; $i++) {
    $s[$i]->close(
        SSL_no_shutdown => 1, 
        SSL_ctx_free    => 1,   
    ) or die "Couldn't close socker $i: $SSL_ERROR";
}

# End
ircunittest::stop_server();
