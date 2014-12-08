#!/usr/bin/python
#-*- coding: utf-8 -*-

import sys
import os 
import time
#  ./send.py mail  "  Confirmation: 6, reason:  high confirmation level  SIMULATION "  /home/abby/Alarm_data/2014-12-08/12-40-26/2014-12-08_12-40-26.wav
# xmpp " 2014-12-08.12:40:24  ALARM DETECTED: 6 high confirmation level;  <!SIMULATION!> " 

def sign(filename) :
	command = "$HOME/chainsign/chainsign --client " + filename
	print command

def getPackedFilename(filename) : 
	raw = filename.split(".")
	packed = raw[0]
	packed += ".tar.gz"
	return packed


def waitForFile(packedFile) :
	exist = False
	print "waiting for " + packedFile
	while exist == False : 
		exist = os.path.isfile(packedFile)
		print "sleep"
		time.sleep(2) 

def sendMail(message, filename) :
	command = "$HOME/PyMailSender/sendmail.py ALARM " + " \"" + message + "\" " + filename 
	print command 
	os.system(command)

def sendXMPP(message) : 
	command = "$HOME/motion-alert/soundsimple/scripts/send-xmpp.sh " + " \"" + message + "\" " 
	print command
	os.system(command)

#for a in sys.argv : 
#	print a
message = sys.argv[2] 

if sys.argv[1] == "mail" :
	filename = sys.argv[3]
	packedFile = getPackedFilename(filename)

	sign(filename) 
	waitForFile(packedFile) 
	sendMail(message, filename)

	getPackedFilename(sys.argv[3]) 
elif sys.argv[1] == "xmpp" : 
	sendXMPP(message) 
else :
	print "error"