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

$s1->send("NICK לקוח_1\x0D\x0AUSER לקוח 1 microsoft.com :One לקוח\x0D\x0A");
$s2->send("NICK לקוח_2\x0D\x0AUSER לקוח2 aws.com :Two לקוח\x0D\x0A");

# Wait client connection
sleep(1);

# data to send to a server
$s1->send("JOIN #ערוץ1\x0D\x0A");
$s2->send("JOIN #ערוץ1\x0D\x0A");

my $req = "PRIVMSG  איפה הביבליוטקה";
$s1->send($req . "\x0D\x0A");

# Wait message reception on the server
sleep(1);

my $response = "";
$s2->recv($response, 1024);
if (index($response, $req) == -1) {
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