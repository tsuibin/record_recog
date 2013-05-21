#!/usr/bin/python  
#-*-coding:utf-8-*-  
  
# 使用httplib进行post数据  
  
import httplib, urllib; #加载web通讯类模块  
import sys,getopt; #加载获取命令行参数模块  
  
opts,argv = getopt.getopt(sys.argv[1:],'',['']); #获取命令行参数  
c = " ";  
argv = c.join(argv); #对参数进行整合 split 是对数据进行拆分  
print "你输入的是:",argv;  
params = urllib.urlencode({'sl':'en',  
                           'tl':'zh-CN',  
                           'text':argv,  
                           'client':'t'}); #进行GET or POST的参数列表  
headers = {"User-Agent": "Mozilla/5.0+(compatible;+Googlebot/2.1;++http://www.google.com/bot.html)"} #输出标头  
conn = httplib.HTTPConnection("translate.google.cn"); #创建一个特定连接  
conn.request("GET","/translate_a/t?" + params,headers=headers);  #发送数据请求  
data = conn.getresponse();  #返回得到的数据结果  
data1 = data.read(); # 读取返回结果  
conn.close(); #关闭连接  
print "翻译结果为:",data1; #输出翻译结果
