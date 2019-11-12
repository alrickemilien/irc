#!/usr/bin/perl

use strict;
use warnings;
use IO::Socket::INET;
use File::Basename;
use Test::More qw( no_plan );

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

diag "Connecting client 1\n";
# create a connecting socket
my $s1 = new IO::Socket::INET (
    PeerHost => $HOST,
    PeerPort => $PORT,
    Proto => 'tcp',
);
die "Couldn't connect to $HOST:$PORT : $!\n" unless $s1;

diag "Connecting client 2\n";
# create a connecting socket
my $s2 = new IO::Socket::INET (
    PeerHost => $HOST,
    PeerPort => $PORT,
    Proto => 'tcp',
);
die "Couldn't connect to $HOST:$PORT : $!\n" unless $s2;


$s1->send("PASS dummy_password_1\x0D\x0A");
$s2->send("PASS dummy_password_1\x0D\x0A");
sleep(1);

$s1->send("NICK client_1\x0D\x0AUSER client1 microsoft.com :Client One\x0D\x0A");
$s2->send("NICK client_2\x0D\x0AUSER client2 aws.com :Client Two\x0D\x0A");
sleep(1);

#
# Test basic peering
#

# Data to send to a server
my $req = "PRIVMSG client_2 Why don't you call me anymore?\x0D\x0A";
$s1->send($req);
sleep(1); # Wait message reception on the server

my $response = "";
$s2->recv($response, 1024);
ok(index($response, "Why don't you call me anymore?") ne -1);

$s1->send("PRIVMSG client_2 YAAAA?\x0D\x0A");
sleep(1); # Wait message reception on the server

$s2->recv($response, 1024);
ok(index($response, "YAAAA") ne -1);

#
# Test fat message
#

my $msg = "PRIVMSG ";
for (my $i = 0; $i <= 10000; $i++) {
    $msg .= $i;
}
$msg .= "\x0D\x0A PRIVMSG client_2 non \x0D\x0A";
$s1->send($msg);

$s2->recv($response, 1024);
ok(index($response, "non") ne -1);

#
# Test utf8
#
diag "Connecting client 3 utf8\n";
my $s3 = new IO::Socket::INET (
    PeerHost => $HOST,
    PeerPort => $PORT,
    Proto => 'tcp',
);
die "Couldn't connect to $HOST:$PORT : $!\n" unless $s2;

$s3->send("NICK לקוח_3\x0D\x0AUSER לקוח3 aws.com :Three לקוח\x0D\x0A");
sleep(1);

# data to send to a server
$s1->send("JOIN #ערוץ1\x0D\x0A");
$s2->send("JOIN #ערוץ1\x0D\x0A");
$s3->send("JOIN #ערוץ1\x0D\x0A");
sleep(1);

$s3->send("PRIVMSG client_2,client_1 :איפה הביבליוטקה" . "\x0D\x0A");
sleep(2);

$s2->recv($response, 1024);
ok(index($response, "איפה הביבליוטקה") ne -1);
$s1->recv($response, 1024);
ok(index($response, "איפה הביבליוטקה") ne -1);

#
# Terminate clients
#

$s1->send("QUIT :#ערוץ1 bye\x0D\x0A");

sleep(1);

$s2->recv($response, 1024);
ok(index($response, "#ערוץ1 bye") ne -1);
ok(index($response, ":#ערוץ1 bye") eq -1);

$s3->recv($response, 1024);
ok(index($response, "#ערוץ1 bye") ne -1);

diag "Closing clients";
$s1->close();
$s2->close();
$s3->close();

# End
ircunittest::stop_server();
