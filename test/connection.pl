#!/usr/bin/perl

use strict;
use warnings;

use Socket;

#
# Test connections on server with many clients
#

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

print "Connecting ...\n";


# create a socket
socket(TO_SERVER, PF_INET, SOCK_STREAM, getprotobyname('tcp'));

my $remote_host = '127.0.0.1';
my $remote_port = '5555';

# build the address of the remote machine
my $internet_addr = inet_aton($remote_host)
    or die "Couldn't convert $remote_host into an Internet address: $!\n";
my $paddr = sockaddr_in($remote_port, $internet_addr);

# connect
connect(TO_SERVER, $paddr)
    or die "Couldn't connect to $remote_host:$remote_port : $!\n";

# ... do something with the socket
print TO_SERVER "Why don't you call me anymore?\n";

# and terminate the connection when we're done
close(TO_SERVER);


#
# End
#

open(my $pidfd, 'ircserver.pid') or die "Can't read server pid file: $!\n";  
my $pidserver = <$pidfd>;
close($pidfd);
print "Killing $pidserver\n";
kill 9, -$pidserver;

# Killing self
kill 9, -$pid;
waitpid $pid, 0;