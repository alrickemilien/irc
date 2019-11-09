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

print "Connecting client 1\n";
# create a connecting socket
my $s1 = new IO::Socket::INET (
    PeerHost => $HOST,
    PeerPort => $PORT,
    Proto => 'tcp',
);
die "Couldn't connect to $HOST:$PORT : $!\n" unless $s1;

print "Connecting client 2\n";
# create a connecting socket
my $s2 = new IO::Socket::INET (
    PeerHost => $HOST,
    PeerPort => $PORT,
    Proto => 'tcp',
);
die "Couldn't connect to $HOST:$PORT : $!\n" unless $s2;

sleep(1);

for (my $i = 0; $i <= 250; $i++) {
    $s1->send("Why don't you call me anymore?\x0D\x0A");
    $s2->send("No\x0D\x0A");
}

#
# Terminate clients
#

print "Closing client 1\n";
$s1->close();

print "Closing client 2\n";

$s2->close();

# Wait for any othe behavior from server
sleep(1);

#
# End
#
{
    open(my $pidfd, 'ircserver.pid') or die "Can't read server pid file: $!\n";  
    my $pidserver = <$pidfd>;
    close($pidfd);
    # If the pid is not here, it means something wring happened
    if (kill(0, -$pidserver) != 0) {
        print "Closing server\n";
        kill 9, -$pidserver;

        unlink 'ircserver.pid';  # Supress pid file of teh server
    }
}
