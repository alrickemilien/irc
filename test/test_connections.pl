#!/usr/bin/perl

use strict;
use warnings;

my $pid = fork();

if ($pid != 0)
{
    open(my $fd, '>', 'ircserver.pid') or die "Could not open pid file for irc server: $!";
    print $fd $pid;
    close($fd);

    setpgrp(0, 0);

    # This is the child process.
    # exec() the external program.
    exec("build/server") or die "Could not run server: $!";

    exit
}
elsif(!defined($pid))
{
        die "could not fork";
}

print "Waiting ...\n";
sleep(5);

open(my $pidfd, 'ircserver.pid') or die "Can't read server pid file: $!\n";  
my $pidserver = <$pidfd>;
close($pidfd);
print "Killing $pidserver\n";
kill 9, -$pidserver;

# Killing self
kill 9, -$pid;
waitpid $pid, 0;