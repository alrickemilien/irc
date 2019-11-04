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

print "Connecting client 1\n";
# create a connecting socket
my $s1 = new IO::Socket::INET (
    PeerHost => $REMOTE_HOST,
    PeerPort => $REMORT_PORT,
    Proto => 'tcp',
);
die "Couldn't connect to $REMOTE_HOST:$REMORT_PORT : $!\n" unless $s1;

print "Connecting client 2\n";
# create a connecting socket
my $s2 = new IO::Socket::INET (
    PeerHost => $REMOTE_HOST,
    PeerPort => $REMORT_PORT,
    Proto => 'tcp',
);
die "Couldn't connect to $REMOTE_HOST:$REMORT_PORT : $!\n" unless $s2;

sleep(1);

for (my $i = 0; $i <= 250; $i++) {
    $s1->send("Why don't you call me anymore?\n");
    $s2->send("No");
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


    print('yayaya');

#
# End
#
{
    open(my $pidfd, 'ircserver.pid') or die "Can't read server pid file: $!\n";  
    my $pidserver = <$pidfd>;
    close($pidfd);
    print('yayaya');
    # If the pid is not here, it means something wring happened
    if (kill(0, -$pidserver) != 0) {
        print "Closing server\n";
        kill 9, -$pidserver;
        print "Closed\n";
    }

    # Supress pid file of teh server
    unlink 'ircserver.pid';

    # Killing self
    kill 9, -$pid;
    print 'ici';
    waitpid $pid, 0;
}
