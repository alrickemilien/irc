#!/usr/bin/perl

# use strict;
# use warnings;

my $pid = fork();
print "MYPMID IS $pid";
if ($pid != 0)
{
    print 'pid', $pid;

    open(my $fd, '>', 'ircserver.pid') or die "Could not open pid file for irc server: $!";

    print $fd $pid;

    close($fd);

    # This is the child process.
    # exec() the external program.
    exec("./build/server") or die "Could not run server: $!";
}
elsif($pid == 0)
{
    print "Waiting ...\n";
    sleep(300);
    print "Closing server\n";

    open(my $pidfd, 'ircserver.pid') or die "Can't read server pid file: $!\n";  
    $pid_server = <$pidfd>;
    close (my $pidfd);  
    print "Killing $pid_server";

    # waitpid($pid_server, WNOHANG);    
}
else
{
        die "could not fork";
}

# if ($pid != 0){
# } 