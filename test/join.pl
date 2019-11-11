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
for (my $i = 0; $i <= $CLIENTS_NUMBER; $i++) {
    # create a connecting socket    
    my $tmp_s = new IO::Socket::INET (
        PeerHost => $HOST,
        PeerPort => $PORT,
        Proto => 'tcp',
    );

    die "Couldn't connect to $HOST:$PORT : $!\n" unless $tmp_s;

    push @s, $tmp_s
}

#
# All clients joining specific channel
#

for (my $i = 0; $i <= $CLIENTS_NUMBER; $i++) {
    $s[$i]->send("JOIN #meeting\x0D\x0ANICK client_$i\x0D\x0A");
}
sleep(2);
my $response = "";
$s[0]->recv($response, 2048);

sleep(2);
# Get all connected people to the channel
$s[0]->send("NAMES #meeting\x0D\x0A");
$s[0]->recv($response, 1024);
print "NAMES result : " . $response;

#
# Terminate clients
#

print "Closing clients\n";
for (my $i = 0; $i <= $CLIENTS_NUMBER; $i++) {
    $s[$i]->close();
}


# Wait for any othe behavior from server
sleep(1);

#
# End
#
ircunittest::stop_server();
