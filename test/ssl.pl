#!/usr/bin/perl

use strict;
use warnings;
use IO::Socket::SSL;
use IO::Select;
use File::Basename;
use lib dirname(__FILE__);
use ircunittest;

# ############################################# #
# Test ssl server                               #
# ############################################# #

sub tls_read {
    my $DBUF_BLOCK_SIZE = 512;

    my ($sock) = @_;

    # if (IO::Select->new($sock)->can_read(500)) {
        sysread($sock, my $buf, $DBUF_BLOCK_SIZE); 
        return $buf;
    # }

    return '';
}

# Start server
# ircunittest::start_server();

#
# Test clients connections
#

my $HOST = '127.0.0.1';
my $PORT = '5555';
my $CLIENTS_NUMBER = 2;
my $POOLS_NUMBER = 2;

my @s;

print "Connecting $CLIENTS_NUMBER clients\n";
for (my $i = 0; $i <= $CLIENTS_NUMBER; $i++) {
    # create a connecting socket    
    my $tmp_s = new IO::Socket::SSL (
        PeerHost => $HOST,
        PeerPort => $PORT,
        SSL_verify_mode => 0,
        Proto => 'tcp',
        Timeout  => 5,
    );
    die "Couldn't connect to $HOST:$PORT : $SSL_ERROR\n" unless $tmp_s;

    $tmp_s->blocking(1);

    # All clients joining specific channel
    print $tmp_s "NICK client_$i\x0D\x0AUSER client$i microsoft.com :Client $i\x0D\x0A";

    push @s, $tmp_s
}

my $s0 = $s[0];

# print tls_read $s0;
sysread($s0, my $buf, 512); 
print $buf;


#
# All client leave their channel
#

print "Joining clients channel\n";
for (my $i = 0; $i <= $CLIENTS_NUMBER; $i++) {
    # All clients joining their created channel
        my $si = $s[$i];
        print $si "JOIN #channel_$i\x0D\x0A";
}

sysread($s0, $buf, 512); 
print $buf;

sysread($s0, $buf, 512); 
print $buf;

sysread($s0, $buf, 512); 
print $buf;

sysread($s0, $buf, 512); 
print $buf;

exit 1;

print $s0 "WHOIS client_3\x0D\x0AWHOIS client_5,client_8\x0D\x0A";

sysread($s0, $buf, 512); 
print $buf;

# print tls_read $s0;


# Test WHO query
print $s0 "WHO client_3\x0D\x0AWHO\x0D\x0A";
print tls_read $s0;

#
# Terminate clients
#

print "Closing clients\n";
for (my $i = 0; $i <= $CLIENTS_NUMBER; $i++) {
    $s[$i]->close(
        SSL_no_shutdown => 1, 
        SSL_ctx_free    => 1,   
    ) or die "Couldn't close socker $i: $SSL_ERROR";
}

# End
ircunittest::stop_server();
