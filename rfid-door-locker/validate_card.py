import sys
import serial

CARD_IDS = set(['00E1F2A4'])

def determine_device():
	'''
	determine device with command line arguments
	'''
	args = sys.argv
	if len(args) > 1:
		return args[1]
	else:
		return '/dev/ttyACM0'

def loop(s):
	'''
	loop
	'''
	line = s.readline()
	if line and line.startswith('CARD_ID: '):
		card_id = line[9:-1].strip()
		print 'card: [' + card_id + ']'

		if card_id in CARD_IDS:
			print 'passed'
			s.writelines(['T'])
		else:
			print 'failed'
			s.writelines(['F'])

# main function
if __name__ == '__main__':
	# wait 1 second
	s = serial.Serial(determine_device(), 9600, timeout = 1)
	s.open()
	try:
	  while True:
			loop(s)
	except KeyboardInterrupt:
		s.close()
