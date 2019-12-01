# irc

IRC server RFC1459 complient for the feature it implements, with tls data encryption.

## Good doc for startup

- [Accept both ipv4 and ipv6 sample](https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_72/rzab6/xacceptboth.htm)
- [IRC protocol with pictures](http://chi.cs.uchicago.edu/chirc/irc_examples.html)

## Utils

- `netstat -avt` let see which port currently runnin on the machine
- `strace -f -e accept,socket,close,shutdown ./build/server` let see each IO socket operation run by the pid of your program at runtime

## Generate self signed certs

Ensure `openssl` is installed on the machine.

```
mkdir -p .cert
cd .cert

openssl genrsa -des3 -out server.key 1024

mv server.key server.key.org
openssl rsa -in server.key.org -out server.key
rm -rf server.key.org

openssl req -new -key server.key -out server.csr

openssl x509 -req -days 365 -in server.csr -signkey server.key \
    -subj "/C=US/ST=Denial/L=Springfield/O=Dis/CN=www.example.com" \
    -keyout www.example.com.key  -out www.example.com.cert \
    -out server.crt
cd -
```

## Check packets

For packet tracing, need `sudo` priviledged or `pcap` permission :

```
sudo groupadd pcap
sudo usermod -a -G pcap $USER
sudo chgrp pcap /usr/sbin/tcpdump
sudo setcap cap_net_raw,cap_net_admin=eip /usr/sbin/tcpdump
sudo ln -s /usr/sbin/tcpdump /usr/bin/tcpdump
```

Use the following command to check packets

```
tcpdump -vv -A -i <network interface of binding ip server> port <running port of irc server>
```
