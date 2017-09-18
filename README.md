# sftp-pam - dead simple PAM auth plugin

## Purpose
This is a rudimentary but fully functional PAM auth plugin, intending to demonstrate the function of PAM plugin with respect to SFTP (i.e. sshd) authentication

Note, as a citation, that the original source code, including the PAM module source and test application, was forked from the following GitHub repo:

 https://github.com/beatgammit/simple-pam

The main change was expansion of pam_sm_authentication callback to perform a non-trivial password verification via execution of another process.

## Deployment Steps

 1) Firstly, you must compile the PAM plugin shared library. Do this by the steps:
 
      mkdir build
      cd build && cmake ../src
      make

 2) You must place the plugin shared library in the PAM install dir

      sudo cp build/libsftp-pam.so /lib/security

 3) You must update sshd's PAM configuration to leverage the custom plugin. To do this, you must (as root) edit the file /etc/pam.d/sshd. Comment out the line '@include common-auth' and place above it, the line 'auth requisite libsftp-pam.so'

 4) Finally, place the PAM auth extension script from this repo in the expected path

      sudo cp ./src/do_pam_auth.sh /usr/local/sbin

 5) Now, when running sshd and attempting an SFTP session, you will be able to use the logic from libsftp-pam to  permit authentication. 
