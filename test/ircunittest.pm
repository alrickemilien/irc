package ircunittest;

use strict;
use warnings;

my $DBUF_BLOCK_SIZE = 512;

sub start_server {
  unless(open(my $pidfd, 'ircserver.pid')) {
    my $pidserver = <$pidfd>;
    close($pidfd);

    # If the pid is not here, it means something wring happened
    if (defined($pidserver) && kill(0, $pidserver) != 0) {
      return;
    }
  }

  `build/server --daemon`;
  print "Starting server, wait ...\n";
  sleep(1);
}

sub stop_server {
  # Wait for any othe behavior from server
  sleep(1);

    open(my $pidfd, 'ircserver.pid') or die "Can't read server pid file: $!\n";  
    my $pidserver = <$pidfd>;
    close($pidfd);
    # If the pid is not here, it means something wring happened
    if (kill(0, $pidserver) != 0) {
        print "Closing server\n";
        kill 9, $pidserver;
        unlink 'ircserver.pid'; # Supress pid file of teh server
    }
}

sub recv_eq {
  my ($sock, $match) = @_;

  # sleep(1); # Wait message reception on the server
  $sock->recv(my $response, $DBUF_BLOCK_SIZE);
  print "[+] recv_eq::response: $response\n";
  return (index($response, $match) ne -1);
}

sub recv_ne {
  my ($sock, $match) = @_;

  # sleep(1); # Wait message reception on the server
  $sock->recv(my $response, $DBUF_BLOCK_SIZE);
  print "[+] recv_ne::response: $response\n";
  return (index($response, $match) eq -1);
}

1;  # packages need to return true.
