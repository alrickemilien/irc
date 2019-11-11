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
ircunittest::start_server();

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

my $msg = "MSG ";
for (my $i = 0; $i <= 10000; $i++) {
    $msg .= $i;
}
$msg .= "\x0D\x0A MSG  non \x0D\x0A";
$s1->send($msg);

# Allow time to handle it
sleep(1);

my $response = "";
$s2->recv($response, length($msg));

if (index($response, "non") == -1) {
    print 'Bad response: ' . $response;
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
ircunittest::stop_server();
