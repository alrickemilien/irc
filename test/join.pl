#!/usr/bin/perl

use strict;
use warnings;
use IO::Socket::INET;

# ############################################# #
# Test connections on server with many clients  #
# ############################################# #

sub psleep {
    sleep(1);
    print $_[0];
}

# Start server
# `build/server --daemon`;
psleep "Starting server, wait ...\n";

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
psleep "Joining the room #meeting\n";
my $response = "";
$s[0]->recv($response, 1024);

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
