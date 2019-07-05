import hashlib
import os
import sys
import json


def remplazarCaracter(File,car1,car2):
	with open(File+'replace','w') as file:
		with open(File,'r') as file1:
			data= file1.read()
			remplazo = data.replace(car1,car2)
		file.write(remplazo)


def main():
	remplazarCaracter("arrhythmia.dat",'?','0')



if __name__ == '__main__':
    main()