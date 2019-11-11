# irc

IRC server RFC1459 complient for the feature it implements.

## Good doc for startup

- [Accept both ipv4 and ipv6 sample](https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_72/rzab6/xacceptboth.htm)
- [IRC protocol with pictures](http://chi.cs.uchicago.edu/chirc/irc_examples.html)

## Utils

- `netstat -avt` let see which port currently runnin on the machine
- `strace -f -e accept,socket,close,shutdown ./build/server` let see each IO socket operation run by the pid of your program at runtime
