#!/usr/bin/env python
import os
i = 1
while i < 2**16 :
	print ('\npage num ' + str(i))
	val = os.system('./a.out ' + str(i) + ' ' + str(100))
	i = i * 2