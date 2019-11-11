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

$s1->send("NICK client_1\x0D\x0AUSER client1 microsoft.com :Client One\x0D\x0A");
$s2->send("NICK client_2\x0D\x0AUSER client2 aws.com :Client Two\x0D\x0A");

sleep(1);

my $msg = "PRIVMSG ";
for (my $i = 0; $i <= 10000; $i++) {
    $msg .= $i;
}
$msg .= "\x0D\x0A PRIVMSG client_2 non \x0D\x0A";
$s1->send($msg);

# Allow time to handle it
sleep(1);

$s2->recv(my $response, 1024);

if (index($response, "non") == -1) {
    print 'Bad response: ' . $response;
}

#
# Terminate clients
#

print "Closing clients\n";
$s1->close();
$s2->close();

# End
ircunittest::stop_server();
