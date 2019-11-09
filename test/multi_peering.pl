#!/usr/bin/perl

use strict;
use warnings;

use IO::Socket::INET;

# ############################################# #
# Test connections on server with many clients  #
# ############################################# #

# Start server
# `build/server --daemon`;

#
# Test clients connections
#

print "Starting server, wait ...\n";
sleep(1);

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

# Wait client connection

for (my $k = 0; $k <= $POOLS_NUMBER; $k++) {
    print "Sending data\n";
    for (my $i = 0; $i <= $CLIENTS_NUMBER; $i++) {
        $s[$i]->send("MSG I say: $i\x0D\x0A");
    }

    # Wait full messages reception on the server
    sleep(1);
}

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
{
    open(my $fd, 'ircserver.pid') or die "Can't read server pid file: $!\n";  
    my $pidserver = <$fd>;
    close($fd);

    # If the pid is not here, it means something wring happened
    if (kill(0, $pidserver)) {
        print "Closing server\n";
        kill 9, -$pidserver;

        unlink 'ircserver.pid'; # Supress pid file of teh server
    }
}
