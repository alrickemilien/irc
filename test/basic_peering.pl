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
# ircunittest::start_server();

#
# Test clients connections
#

my $HOST = '127.0.0.1';
my $PORT = '5555';
my $DBUF_BLOCK_SIZE = 512;

diag "Connecting client 1\n";
# create a connecting socket
my $s1 = new IO::Socket::INET (
    PeerHost => $HOST,
    PeerPort => $PORT,
    Proto => 'tcp',
);
die "Couldn't connect to $HOST:$PORT : $!\n" unless $s1;
$s1->setsockopt(SOL_SOCKET, SO_RCVTIMEO, pack('l!l!', 10, 0));

diag "Connecting client 2\n";
# create a connecting socket
my $s2 = new IO::Socket::INET (
    PeerHost => $HOST,
    PeerPort => $PORT,
    Proto => 'tcp',
);
die "Couldn't connect to $HOST:$PORT : $!\n" unless $s2;
$s2->setsockopt(SOL_SOCKET, SO_RCVTIMEO, pack('l!l!', 10, 0));


#
# Test registration
#

$s1->send("PASS dummy_password\x0D\x0A");
$s2->send("PASS dummy_password\x0D\x0A");

$s1->send("NICK client1\x0D\x0AUSER client1 microsoft.com :Client One\x0D\x0A");
$s2->send("NICK client2\x0D\x0AUSER client2 aws.com :Client Two\x0D\x0A");

ok(ircunittest::recv_eq $s1, "001");
ok(ircunittest::recv_eq $s2, "001");

#
# Test basic peering
#

# Data to send to a server
$s1->send("PRIVMSG client2 Hi\x0D\x0A");
ok(ircunittest::recv_eq $s2, ":client1 PRIVMSG :Hi");


#
# Test fat message
#

my $msg = "PRIVMSG ";
for (my $i = 0; $i <= 10000; $i++) {
    $msg .= "X";
}
$msg .= "\x0D\x0A PRIVMSG client2 non \x0D\x0A";
$s1->send($msg);

ok(ircunittest::recv_eq $s2, ":client1 PRIVMSG :non");

#
# Test utf8
#

diag "Connecting client 3 utf8\n";
my $s3 = new IO::Socket::INET (
    PeerHost => $HOST,
    PeerPort => $PORT,
    Proto => 'tcp',
);
die "Couldn't connect to $HOST:$PORT : $!\n" unless $s3;
$s3->setsockopt(SOL_SOCKET, SO_RCVTIMEO, pack('l!l!', 5, 0));
$s3->send("NICK client3\x0D\x0AUSER לקוח3 aws.com :Three לקוח\x0D\x0A");
ircunittest::recv_ne $s3, "";

diag "Connecting client 4 dummy\n";
# create a connecting socket
my $s4 = new IO::Socket::INET (
    PeerHost => $HOST,
    PeerPort => $PORT,
    Proto => 'tcp',
);
die "Couldn't connect to $HOST:$PORT : $!\n" unless $s4;
$s4->setsockopt(SOL_SOCKET, SO_RCVTIMEO, pack('l!l!', 5, 0));
$s4->send("NICK client4\x0D\x0AUSER לקוח4 aws.com :Four לקוח\x0D\x0A");
ircunittest::recv_ne $s4, "";

# data to send to a server
$s1->send("JOIN #ערוץ1\x0D\x0A");
$s2->send("JOIN #ערוץ1\x0D\x0A");
$s3->send("JOIN #ערוץ1\x0D\x0A");

ok(ircunittest::recv_ne $s4, "JOIN #ערוץ1");
ok(ircunittest::recv_eq $s3, "JOIN #ערוץ1");
ok(ircunittest::recv_eq $s2, "JOIN #ערוץ1");
ok(ircunittest::recv_eq $s1, "JOIN #ערוץ1");

$s3->send("PRIVMSG client2,client1 :איפה הביבליוטקה\x0D\x0A");

ok(ircunittest::recv_eq $s1, "client3 PRIVMSG :איפה הביבליוטקה");
ok(ircunittest::recv_eq $s2, "client3 PRIVMSG :איפה הביבליוטקה");
ok(ircunittest::recv_ne $s3, "client3 PRIVMSG :איפה הביבליוטקה");

$s3->send("PRIVMSG #ערוץ1 :איפה הביבליוטקה\x0D\x0A");

#
# Test PRIVMSG to all clients in the channel
#

$s3->send("PRIVMSG #ערוץ1 :Wake up\x0D\x0A");

diag "PRIVMSG #ערוץ1 :Wake up";

ok(ircunittest::recv_eq $s1, "client3 PRIVMSG :Wake up");
ok(ircunittest::recv_eq $s2, "client3 PRIVMSG :Wake up");
ok(ircunittest::recv_eq $s3, "client3 PRIVMSG :Wake up");
ok(ircunittest::recv_ne $s4, "client3 PRIVMSG :Wake up");

#
# Test away
#

$s2->send("AWAY :משם\x0D\x0A");
$s3->send("AWAY :Available between 00AM and 07AM\x0D\x0A");

ok(ircunittest::recv_eq $s2, "306 :You have been marked as being away");
ok(ircunittest::recv_eq $s3, "306 :You have been marked as being away");

$s1->send("PRIVMSG client3,client2 :איפה הביבליוטקה\x0D\x0A");

ok(ircunittest::recv_eq $s1, "301 client2 :משם\x0D\x0A301 client3 :Available between 00AM and 07AM\x0D\x0A");

#
# Terminate clients
#

$s1->send("QUIT :bye\x0D\x0A");

ok(ircunittest::recv_eq $s2, "client1 QUIT :bye");
ok(ircunittest::recv_eq $s3, "client1 QUIT :bye");

diag "Closing clients ...";
$s1->close();
$s2->close();
$s3->close();
$s4->close();

# End
ircunittest::stop_server();
