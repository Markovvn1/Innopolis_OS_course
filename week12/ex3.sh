mkdir test

dd if=/dev/urandom of=test/2.txt bs=1 count=100 2> /dev/null
dd if=/dev/urandom of=test/3.txt bs=10 count=100 2> /dev/null
dd if=/dev/urandom of=test/4.txt bs=100 count=100 2> /dev/null
dd if=/dev/urandom of=test/5.txt bs=1000 count=100 2> /dev/null
dd if=/dev/urandom of=test/6.txt bs=1000 count=1000 2> /dev/null
dd if=/dev/urandom of=test/7.txt bs=1000 count=10000 2> /dev/null
dd if=/dev/urandom of=test/8.txt bs=1000 count=100000 2> /dev/null

gcc copy.c -o copy -DSYNC -DBUF_SIZE=102400
time ./copy test/2.txt test/_2.txt
time ./copy test/3.txt test/_3.txt
time ./copy test/4.txt test/_4.txt
time ./copy test/5.txt test/_5.txt
time ./copy test/6.txt test/_6.txt
time ./copy test/7.txt test/_7.txt
time ./copy test/8.txt test/_8.txt

rm -rf test
rm copy