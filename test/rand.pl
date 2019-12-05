#!/usr/bin/perl

use strict;
use warnings;
use IO::Socket::INET;
use File::Basename;
use Test::More qw( no_plan );
use lib dirname(__FILE__);
use ircunittest;

# ############################################# #
# Test connections on server with many clients  #
# ############################################# #

# Start server
# ircunittest::start_server();

#
# Test clients connections
#

my $HOST = '127.0.0.1';
my $PORT = '5555';
my $CLIENTS_NUMBER = 3;
my $POOLS_NUMBER = 3;

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
    $tmp_s->send("USER client_attack$i microsoft.com :Client Attack $i\x0D\x0ANICK client_attack_$i\x0D\x0A");

    push @s, $tmp_s
}

my $response;

#
# Send continus garbage data
#

# Wait 1 sec between each pool in order to let server empty the buffer
for (my $k = 0; $k <= $POOLS_NUMBER; $k++) {
    my $buf_size = int(rand(1024));
    my $buf = '';

    for (my $bytes = 0; $bytes < $buf_size; $bytes += 4) {
        my $rand = int(rand(254 ** 4));
        for (1..4) {
            $buf .= chr($rand % 254);
            $rand = int($rand / 254);
        }
    }

    print "Sending data\n";
    for (my $i = 0; $i <= $CLIENTS_NUMBER; $i++) {
        $s[$i]->send($buf);
    }

    sleep(1); 
}

#
# Terminate clients
#

print "Closing clients\n";
for (my $i = 0; $i <= $CLIENTS_NUMBER; $i++) {
    $s[$i]->close();
}

# End
ircunittest::stop_server();
