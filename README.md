# sftp-ng-pam
## PAM implementation for next-gen SFTP

### Purpose
This is a PAM module intended to permit customized authentication & metadata retrieval for the nextgen SFTP system. The PAM module is configured via the sshd PAM pathway, i.e. /etc/pam.d/sshd

Note, as a citation, that the original source code, including the PAM module source and test application, was forked from the following GitHub repo:

 https://github.com/beatgammit/simple-pam

The main changes were
- expansion of pam_sm_authentication callback to perform a non-trivial password verification via execution of another process.
- inclusion of pam_sm_open_session callback to test insertion of custom variables into authenticated user's ENV

### Build Steps

Firstly, ensure the system your developing on has the PAM development package installed. This package will have the libraries and supporting doc necessary to develop against PAM. It has different names on different Linux distributions.

```
# Red Hat variants
sudo yum install pam-devel
# Debian variants, Ubuntu etc.
sudo apt-get install libpam0g-dev
```

See [here](https://mariadb.com/kb/en/library/installing-correct-libraries-for-pam-and-readline/) for more info on correct PAM package for your Linux OS.

You will also need [CMake](https://cmake.org/) build tool to compile the shared object.

CMake should be installed on most development machines.

One of CMake's principal properties is separating your source tree from your build artifacts.

Run the following commands to build the project:
```
mkdir build
cd build && cmake ../src
make
```
Your build directory is simply a dumping ground for object files and binary targets produced by make. You can delete & re-create it as you wish.

### Deployment Steps

 1) Firstly, you must compile the PAM plugin shared library using above steps.

 2) You must place the plugin shared library in the PAM install dir

      sudo cp build/libsftp-pam.so /lib/security

 3) You must update sshd's PAM configuration to leverage the custom plugin. To do this, you must (as root) edit the file /etc/pam.d/sshd. Comment out the line '@include common-auth' and place above it, the line 'auth requisite libsftp-pam.so'. You will also need to add another line within config file to make PAM aware that it should invoke the PAM module upon user session setup. This is done by adding line 'session requisite libsftp-pam.so' to the config file.

 4) Finally, place the PAM auth extension script from this repo in the expected path

      sudo cp ./src/do_pam_auth.sh /usr/local/sbin

 5) Now, when running sshd and attempting an SFTP session, you will be able to use the logic from libsftp-pam to  permit authentication. 
