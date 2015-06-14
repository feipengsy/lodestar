#!/usr/bin/python

import Sniper
task = Sniper.Task("task")
task.asTop()
task.setLogLevel(5)

import FirstAlg
alg = task.createAlg("CorsikaInputTestAlg/alg_example")

import MemoryMgr
bufMgr = task.createSvc("DataBufferMgr")

import DataIOSvc

iSvc = task.createSvc("DataInputSvc/InputSvc")
iSvc.property("InputStream").set({"/Event/CorsikaEvent" : "DAT040002.part"})
#iSvc.property("InputStream").set({"/Event/CorsikaEvent" : "DAT050001.part"})

oSvc = task.createSvc("DataOutputSvc/OutputSvc")
#oSvc.property("OutputStream").set({"/Event/CorsikaEvent" : "csk.root",})
oSvc.property("OutputStream").set({"/Event/KM2ASimEvent" : "csk.root"})

task.setEvtMax(-1)
task.show()
task.run()
