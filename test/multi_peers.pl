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
# ircunittest::start_server();

my $HOST = '127.0.0.1';
my $PORT = '5555';
my $CLIENTS_NUMBER = 50;
my $POOLS_NUMBER = 5;

my @s;
for (my $i = 0; $i <= $CLIENTS_NUMBER; $i++) {
    # create a connecting socket    
    my $tmp_s = new IO::Socket::INET (
        PeerHost => $HOST,
        PeerPort => $PORT,
        Proto => 'tcp',
    );
    die "Couldn't connect to $HOST:$PORT : $!\n" unless $tmp_s;
    
    $tmp_s->setsockopt(SOL_SOCKET, SO_RCVTIMEO, pack('l!l!', 10, 0));

    $tmp_s->send("USER client$i microsoft.com :Client $i\x0D\x0ANICK client_$i\x0D\x0A");

    push @s, $tmp_s
}

# Wait clients connection
sleep(2);

# Wait 1 sec between each pool in order to let server empty the buffer
for (my $k = 0; $k <= $POOLS_NUMBER; $k++) {
    print "Sending data\n";
    for (my $i = 0; $i <= $CLIENTS_NUMBER; $i++) {
        $s[$i]->send("PRIVMSG &hub :I say: $i\x0D\x0A");
    }
    sleep(1); 
}

# Do not wait 1 sec between each pool in order to let server empty the buffer
for (my $k = 0; $k <= $POOLS_NUMBER; $k++) {
    print "Sending data\n";
    for (my $i = 0; $i <= $CLIENTS_NUMBER; $i++) {
        $s[$i]->send("PRIVMSG &hub :I say: $i\x0D\x0A");
    }
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