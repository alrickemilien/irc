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

#
# Test clients connections
#

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

$s1->send("NICK client_1\x0D\x0A");
$s2->send("NICK client_2\x0D\x0A");

sleep(2);

# data to send to a server
my $req = "PRIVMSG client_2 Why don't you call me anymore?\x0D\x0A";
$s1->send($req);

# Wait message reception on the server
sleep(2);

print "Client 2 waiting from Client 1 message\n";

my $response = "";
$s2->recv($response, 1024);

if (index($response, "Why don't you call me anymore?") == -1) {
    print 'Bad response: ' . $response;
}

#
# Terminate clients
#

print "Closing client 1\n";
$s1->close();

print "Closing client 2\n";
$s2->close();

ircunittest::stop_server();