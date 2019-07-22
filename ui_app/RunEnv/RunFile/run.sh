#!/bin/sh

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib:/customer/lib:/lib:/customer/CSpotter/lib:
chmod 777 playerWnd
./playerWnd &
