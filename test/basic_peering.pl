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

$s1->send("NICK client_1\x0D\x0AUSER client1 microsoft.com :Client One\x0D\x0A");
$s2->send("NICK client_2\x0D\x0AUSER client2 aws.com :Client Two\x0D\x0A");

ok(ircunittest::recv_eq $s1, "RPL_WELCOME");
ok(ircunittest::recv_eq $s2, "RPL_WELCOME");

#
# Test basic peering
#

# Data to send to a server
$s1->send("PRIVMSG client_2 Hi\x0D\x0A");
ok(ircunittest::recv_eq $s2, ":client_1 PRIVMSG :Hi");


#
# Test fat message
#

my $msg = "PRIVMSG ";
for (my $i = 0; $i <= 10000; $i++) {
    $msg .= "X";
}
$msg .= "\x0D\x0A PRIVMSG client_2 non \x0D\x0A";
$s1->send($msg);

ok(ircunittest::recv_eq $s2, ":client_1 PRIVMSG :non");

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
$s3->send("NICK לקוח3\x0D\x0AUSER לקוח3 aws.com :Three לקוח\x0D\x0A");
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
$s4->send("NICK לקוח4\x0D\x0AUSER לקוח4 aws.com :Four לקוח\x0D\x0A");
ircunittest::recv_ne $s4, "";

# data to send to a server
$s1->send("JOIN #ערוץ1\x0D\x0A");
$s2->send("JOIN #ערוץ1\x0D\x0A");
$s3->send("JOIN #ערוץ1\x0D\x0A");

$s3->send("PRIVMSG client_2,client_1 :איפה הביבליוטקה\x0D\x0A");

ok(ircunittest::recv_eq $s1, "לקוח3 PRIVMSG :איפה הביבליוטקה");
ok(ircunittest::recv_eq $s2, "לקוח3 PRIVMSG :איפה הביבליוטקה");
ok(ircunittest::recv_ne $s3, "לקוח3 PRIVMSG :איפה הביבליוטקה");

#
# Test PRIVMSG to all clients in the channel
#

$s3->send("PRIVMSG #ערוץ1 :Wake up\x0D\x0A");

ok(ircunittest::recv_eq $s1, "לקוח3 PRIVMSG :Wake up");
ok(ircunittest::recv_eq $s2, "לקוח3 PRIVMSG :Wake up");
ok(ircunittest::recv_ne $s4, "לקוח3 PRIVMSG :Wake up");

#
# Test away
#

$s2->send("AWAY :משם\x0D\x0A");
$s3->send("AWAY :Available between 00AM and 07AM\x0D\x0A");

ok(ircunittest::recv_eq $s2, "RPL_NOWAWAY :You have been marked as being away");
ok(ircunittest::recv_eq $s3, "RPL_NOWAWAY :You have been marked as being away");

$s1->send("PRIVMSG לקוח3,client_2 :איפה הביבליוטקה\x0D\x0A");

ok(ircunittest::recv_eq $s1, "RPL_AWAY client_2 :משם");
ok(ircunittest::recv_eq $s1, "RPL_AWAY לקוח3 :Available between 00AM and 07AM");

#
# Terminate clients
#

$s1->send("QUIT :bye\x0D\x0A");

ok(ircunittest::recv_eq $s2, "#ערוץ1 quit :bye");
ok(ircunittest::recv_eq $s3, "#ערוץ1 quit :bye");

diag "Closing clients";
$s1->close();
$s2->close();
$s3->close();
$s4->close();

# End
ircunittest::stop_server();
