#!/usr/bin/python

import Sniper
task = Sniper.Task("task")
task.asTop()
task.setLogLevel(0)

import FirstAlg
alg = task.createAlg("CorsikaInputTestAlg/alg_example")

import MemoryMgr
bufMgr = task.createSvc("DataBufferMgr")

import DataIOSvc

iSvc = task.createSvc("DataInputSvc/InputSvc")
iSvc.property("InputStream").set({"/Event/CorsikaEvent" : "DAT040002.corsika"})

oSvc = task.createSvc("DataOutputSvc/OutputSvc")
oSvc.property("OutputStream").set({"/Event/CorsikaEvent" : "test.root", "/Event/KM2ASimEvent" : "test.root"})

task.setEvtMax(100)
task.show()
task.run()
