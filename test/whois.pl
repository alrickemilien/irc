#!/usr/bin/perl

use strict;
use warnings;
use IO::Socket::INET;
use File::Basename;
use lib dirname(__FILE__);
use ircunittest;

# ############################################# #
# Test connections on server with many clients  #
# ############################################# #

# Start server
ircunittest::start_server();

#
# Test clients connections
#

my $HOST = '127.0.0.1';
my $PORT = '5555';
my $CLIENTS_NUMBER = 10;
my $POOLS_NUMBER = 2;

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
    
    $tmp_s->setsockopt(SOL_SOCKET, SO_RCVTIMEO, pack('l!l!', 10, 0));

    # All clients joining specific channel
    $tmp_s->send("NICK client_$i\x0D\x0AUSER client$i microsoft.com :Client $i\x0D\x0A");

    push @s, $tmp_s
}
sleep(3);

#
# All client leave their channel
#

print "Joining clients channel\n";
for (my $i = 0; $i <= $CLIENTS_NUMBER; $i++) {
    # All clients joining their created channel
    $s[$i]->send("JOIN #channel_$i\x0D\x0A");
}
sleep(2);

# Get all connected people to the all channels
$s[0]->send("WHOIS client_3\x0D\x0AWHOIS client_5,client_8\x0D\x0A");
sleep(4);

$s[0]->recv(my $response, 2048);
print $response;

#
# Terminate clients
#

print "Closing clients\n";
for (my $i = 0; $i <= $CLIENTS_NUMBER; $i++) {
    $s[$i]->close();
}

# End
ircunittest::stop_server();
