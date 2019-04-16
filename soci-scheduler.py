import predict
import subprocess
import time
import numpy as np
import urllib.request
from io import BytesIO

nextObs = 0
name = 'QB50P2' #test satellite

def main(args):
	TLE = pullTLE(name)
	qth = (47.654867,-122.306501,100)  # lat (N), long (W), alt (meters) (get real altitude later + building)
	predict.observe(tle, qth)
	#get time, doppler and window
	check_TLE = False
	while(True):#main loop
		if(2000>nextObs-time.time() and not check_TLE) #check TLE 30 minutes before 
			TLE = pullTLE(name)
			check_TLE = True
		if(nextObs-time.time()>100)
			#comms(tx_bytes,doppler,time_window,mode)
			#compute nextObs
			check_TLE = False
		end
	return 0
def comms(tx_bytes,doppler,time_window,mode):
	#subprocess.call("gs_downlink.py", shell=True)
	#subprocess.call("gs_uplink.py", shell=True)
	end
def pullTLE(name):
	url = 'https://www.celestrak.com/NORAD/elements/cubesat.txt'
	response = urllib.request.urlopen(url)
	data = response.read()
	text = data.decode('utf-8')
	lines = text.splitlines()
	TLE = ''
	ind = 0
	for line in lines:
		if (line.startswith(name)):
			TLE = '\n'.join(lines[ind:ind+3])
			break
		ind = ind + 1
	return TLE
	
if __name__ == '__main__':
    import sys
    sys.exit(main(sys.argv))
