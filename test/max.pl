#!/usr/bin/perl

use strict;
use warnings;
use IO::Socket::INET;
use File::Basename;
use Test::More qw( no_plan );
use lib dirname(__FILE__);
use ircunittest;

# ############################################# #
# Test maximum number of socket clients         #
# ############################################# #

# Start server
# ircunittest::start_server();

#
# Test clients connections
#

my $HOST = '127.0.0.1';
my $PORT = '5555';
my $CLIENTS_NUMBER = 512;

my @s;

print "Connecting $CLIENTS_NUMBER clients\n";
for (my $i = 0; $i <= $CLIENTS_NUMBER; $i++) {
    # create a connecting socket    
    my $tmp_s = new IO::Socket::INET (
        PeerHost => $HOST,
        PeerPort => $PORT,
        Proto => 'tcp',
    );
    die "Couldn't connect to $HOST:$PORT : $!\n" unless $tmp_s;
    
    $tmp_s->setsockopt(SOL_SOCKET, SO_RCVTIMEO, pack('l!l!', 20, 0));

    # All clients joining specific channel
    $tmp_s->send("USER client$i microsoft.com :Client $i\x0D\x0ANICK client$i\x0D\x0A");

    push @s, $tmp_s
}

#
# Send continus garbage data
#

$s[$CLIENTS_NUMBER]->send("NAMES\x0D\x0A");
my $response = '';
do {
    $s[$CLIENTS_NUMBER]->recv($response, 2048);
    print $response;
} while(length $response > 0);

#
# Terminate clients
#

print "Closing clients\n";
for (my $i = 0; $i <= $CLIENTS_NUMBER; $i++) {
    $s[$i]->close();
}

# End
ircunittest::stop_server();
