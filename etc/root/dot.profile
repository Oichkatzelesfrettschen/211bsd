echo 'erase ^H, kill ^U, intr ^C'
stty dec
stty erase \^H kill \^U intr \^C
PATH=/bin:/sbin:/usr/sbin:/etc:/usr/ucb:/usr/bin:/usr/new
export PATH
HOME=/root
export HOME
export TERM
umask 22
