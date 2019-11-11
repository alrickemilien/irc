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
ircunittest:start_server();

#
# Test clients connections
#

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

#
# Terminate clients
#

print "Closing client 1\n";
$s1->close();

# Wait for any othe behavior from server
sleep(1);

#
# End
ircunittest:stop_server();