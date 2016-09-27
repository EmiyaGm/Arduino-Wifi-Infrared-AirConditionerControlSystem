__author__ = 'jimshen'

import datetime
import time

import sqlite3

from pylab import *

from django.http import HttpResponse
from django.shortcuts import render

def index(request):
    return render(request,'home.html',{'username':'jimshen'})

def update(request):
    a=request.GET["field1"]
    currTime=time.strftime("%Y-%m-%d %H:%M:%S",time.localtime(time.time()))
    cx = sqlite3.connect("d:/myprog/Arduino/lm35-esp8266-client/test.db")
    t=(float(a),currTime)
    cx.execute("INSERT INTO temp(tmpr,ts) VALUES (?,?)",t)
    cx.commit()
    cx.close()
    return HttpResponse("done")

def view(request):
    cx = sqlite3.connect("d:/myprog/Arduino/lm35-esp8266-client/test.db")
    cu = cx.cursor()
    cu.execute("select tmpr from temp order by ts asc")
    rows=cu.fetchall()
    t=list(rows)
    plot(t)
    savefig("d:/myprog/python/webtest/webtest/static/images/result.png")
    return render(request,'data.html')

def led(request):
    s=request.GET["led"]
    cx = sqlite3.connect("d:/myprog/Arduino/lm35-esp8266-client/test.db")
    cx.execute("UPDATE led set status="+s)
    cx.commit()
    cx.close()
    return render(request,'led.html')
