#!/bin/bash

echo "$ROOTPWD"|sudo -S sed -i -e '$aCiphers aes128-cbc,aes192-cbc,aes256-cbc,aes128-ctr,aes192-ctr,aes256-ctr,3des-cbc,arcfour128,arcfour256,arcfour,blowfish-cbc,cast128-cbc' /etc/ssh/sshd_config
if [ $? -ne 0 ]
then
    exit 9
fi

echo "$ROOTPWD"|sudo -S sed -i -e '$aMACs hmac-md5,hmac-sha1,umac-64@openssh.com,hmac-ripemd160,hmac-sha1-96,hmac-md5-96' /etc/ssh/sshd_config
if [ $? -ne 0 ]
then
    exit 9
fi

echo "$ROOTPWD"|sudo -S sed -i -e '$aKexAlgorithms diffie-hellman-group1-sha1,diffie-hellman-group14-sha1,diffie-hellman-group-exchange-sha1,diffie-hellman-group-exchange-sha256,ecdh-sha2-nistp256,ecdh-sha2-nistp384,ecdh-sha2-nistp521,diffie-hellman-group1-sha1,curve25519-sha256@libssh.org' /etc/ssh/sshd_config 
if [ $? -ne 0 ]
then
    exit 9
fi

echo "$ROOTPWD"|sudo -S sed -i -e "s/^PermitRootLogin prohibit-password/PermitRootLogin yes/g" /etc/ssh/sshd_config
if [ $? -ne 0 ]
then
    exit 9
fi

echo "$ROOTPWD"|sudo -S service sshd restart
if [ $? -ne 0 ]
then
    exit 9
fi
