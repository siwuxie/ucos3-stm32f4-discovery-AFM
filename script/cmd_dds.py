#! /usr/bin/python

from serial import *
import sys

DDS_CMD_WORD='0001'.decode('hex')
DDS_CMD_SET_FREQUENCY='0001'.decode('hex')
DDS_CMD_SET_START_FREQUENCY='0002'.decode('hex')
DDS_CMD_SET_END_FREQUENCY='0003'.decode('hex')
DDS_CMD_SET_NUMBER_OF_POINTS='0004'.decode('hex')
DDS_CMD_FREQUENCY_SWEEP_START='0005'.decode('hex')

def build_com():
    """Initialing the serial port /dev/ttyUSB0 115200"""
    return Serial('/dev/ttyUSB0',115200)

def set_frequency(com, startfre, endfre):
    cmdstartfre=[DDS_CMD_WORD, DDS_CMD_SET_START_FREQUENCY]
    cmdvalue = hex(startfre).split('x')[1]
    
    while len(cmdvalue)<8: cmdvalue = '0'+cmdvalue
    cmdstartfre.append(cmdvalue[0:4].decode('hex'))
    cmdstartfre.append(cmdvalue[4:].decode('hex'))

    cmdendfre=[DDS_CMD_WORD, DDS_CMD_SET_END_FREQUENCY]
    cmdvalue = hex(endfre).split('x')[1]
    
    while len(cmdvalue)<8: cmdvalue = '0'+cmdvalue
    cmdendfre.append(cmdvalue[0:4].decode('hex'))
    cmdendfre.append(cmdvalue[4:].decode('hex'))

    print cmdstartfre
    print cmdendfre
    
    for item in cmdstartfre: com.write(item)
    #for item in cmdendfre: com.write(item)

def set_number(com, number):
    cmdnumber=[DDS_CMD_WORD, DDS_CMD_SET_NUMBER_OF_POINTS]
    cmdvalue = hex(number).split('x')[1]
    
    while len(cmdvalue)<8: cmdvalue = '0'+cmdvalue
    cmdnumber.append(cmdvalue[0:4])
    cmdnumber.append(cmdvalue[4:])

    for item in cmdnumber: com.write(item)

def start_sweep(com):
    cmdsweep=[DDS_CMD_WORD, DDS_CMD_FREQUENCY_SWEEP_START,'0000'.decode('hex'),'0000'.decode('hex')]

    for item in cmdsweep: com.write(item)

if __name__ == "__main__":
	com = build_com()
	com.write('FFFFFFFFFFFFFFFF'.decode('hex'))
