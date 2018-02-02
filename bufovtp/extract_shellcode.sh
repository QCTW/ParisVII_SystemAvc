cat $1 | hexdump -v -e '"\\\x" 1/1 "%02x"' ; echo
