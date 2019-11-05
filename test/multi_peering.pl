#!/usr/bin/perl

use strict;
use warnings;

use IO::Socket::INET;

# ############################################# #
# Test connections on server with many clients  #
# ############################################# #

#
# Start server
#

my $pid = fork();
if ($pid != 0)
{
    # This is the child process.

    open(my $fd, '>', 'ircserver.pid') or die "Could not open pid file for irc server: $!";
    print $fd $pid;
    close($fd);

    setpgrp(0, 0);
    # exec() the external program.
    exec("build/server > /dev/null") or die "Could not run server: $!";

    unlink 'ircserver.pid';

    exit
}
die "could not fork" unless defined($pid);

#
# Test clients connections
#

print "Starting server, wait ...\n";
sleep(1);

my $REMOTE_HOST = '127.0.0.1';
my $REMORT_PORT = '5555';

my @s;
for (my $i = 0; $i <= 20; $i++) {
    # create a connecting socket    
    my $tmp_s = new IO::Socket::INET (
        PeerHost => $REMOTE_HOST,
        PeerPort => $REMORT_PORT,
        Proto => 'tcp',
    );

    die "Couldn't connect to $REMOTE_HOST:$REMORT_PORT : $!\n" unless $tmp_s;

    push @s, $tmp_s
}

# Wait client connection

for (my $k = 0; $k <= 5; $k++) {
    print "Sending data\n";
    for (my $i = 0; $i <= 20; $i++) {
        $s[$i]->send("I say: $i");
    }

    # Wait full messages reception on the server
    sleep(1);
}

#
# Terminate clients
#

print "Closing clients\n";
for (my $i = 0; $i <= 20; $i++) {
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
        print "Closed\n";
    }

    print("Unlink\n");

    # Supress pid file of teh server
    unlink 'ircserver.pid';
}
