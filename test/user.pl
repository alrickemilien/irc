#!/usr/bin/perl

use strict;
use warnings;
use IO::Socket::INET;

# ############################################# #
# Test connections on server with many clients  #
# ############################################# #

sub plseep {
    sleep(1);
    print $_[0];
}

# Start server
# `build/server --daemon`;

#
# Test clients connections
#

plseep "Starting server, wait ...\n";

my $HOST = '127.0.0.1';
my $PORT = '5555';

# create a connecting socket
my $s1 = new IO::Socket::INET (
    PeerHost => $HOST,
    PeerPort => $PORT,
    Proto => 'tcp',
);
die "Couldn't connect to $HOST:$PORT : $!\n" unless $s1;

my $req = "USER ".($ARGV[0] || "newuser")." thehostname theservername therealname \x0D\x0A";
$s1->send($req);

# Wait message reception on the server
sleep(1);

$s1->send("QUIT\x0D\x0A");
sleep(1);

#
# Terminate clients
#

print "Closing client 1\n";
$s1->close();

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
    if (kill(0, $pidserver)) {
        print "Closing server\n";
        kill 9, $pidserver;
        unlink 'ircserver.pid'; # Supress pid file of teh server
    }
}
