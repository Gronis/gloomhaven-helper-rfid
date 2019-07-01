#!/usr/bin/env python

import socket
import re
import binascii
import time

TCP_IP = '127.0.0.1'
TCP_PORT = 58888
BUFFER_SIZE = 1024
MESSAGE_NR_INDEX = 5


def setInitiative(targetClass, targetInitiative):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((TCP_IP, TCP_PORT))

    possibleHeader = '\x01\x73'
    #possibleFooter = '\x00\x00\x00'
    preMessage = '\x00\x01\x73'
    postMessage = '\x00'
    characterStart = '\x01\x80'

    def setInitiativeCharacter(index, tClass, tInitiative, data):
        cClass = data[index + 2]
        cExp = data[index + 3]
        cCurrentHealth = data[index + 4]
        cMaxHealth = data[index + 5]
        cLevel = data[index + 6]
        cCoins = data[index + 7]
        cInitiative = data[index + 8]
        print(index)
        print("class: ", cClass)
        print("level: ", cLevel)
        print("exp: ", cExp)
        print("hp: ", str(cCurrentHealth) + "/" + str(cMaxHealth))
        print("initiative: ", cInitiative)
        if tClass == cClass:
            data[index + 8] = tInitiative

    def increaseMessageNumber(data):
        data[MESSAGE_NR_INDEX] = (data[MESSAGE_NR_INDEX] + 1) % 256

    sendResp = False
    nrEmpties = 0
    while nrEmpties < 10 and not sendResp:
        data = s.recv(BUFFER_SIZE)
        print("got: ", data)
        if len(data) == 0:
            nrEmpties = nrEmpties + 1
        else:
            nrEmpties = 0
        possibleHeaderIndex = data.find(possibleHeader)
        if possibleHeaderIndex > -1:
            data = data[(possibleHeaderIndex + len(possibleHeader)):-1]
            print("Removed Extra Header...")
        # possibleFooterIndex = data.rfind(possibleFooter)
        # if possibleFooterIndex > -1:
        #     data = data[0:(possibleFooterIndex)]
        #     print("Removed Extra Footer...")
        cIndecies = [m.start() for m in re.finditer(characterStart, data)]
        bData = map(ord, data)
        if len(cIndecies) > 0:
            print("rec data:", data)
            [setInitiativeCharacter(i, targetClass, targetInitiative, bData) for i in cIndecies]
            increaseMessageNumber(bData)
            newData = bytearray(bData)
            print("new data:", newData)
            # time.sleep(1)
            s.send(preMessage + newData + postMessage)
            sendResp = True
    s.close()
    return sendResp


def main():
    targetClass = 4
    targetInitiative = 49
    result = False
    while not result:
        result = setInitiative(targetClass, targetInitiative)


if __name__ == '__main__':
    main()
