mkdir /var/tmp
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib:/customer/lib:/lib:/customer/CSpotter/lib
echo 12 >/sys/class/gpio/export
echo out >/sys/class/gpio/gpio12/direction
echo 1 >/sys/class/gpio/gpio12/value
cd /customer
#./mginit &
