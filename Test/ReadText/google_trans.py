#encoding: utf-8
#Created on 2010-11-28

#@author: stone

import json
import re
import sys
import urllib2
import types

res = 'http://translate.google.cn/translate_a/t?client=t&text=%s&hl=zh-CN&sl=%s&tl=%s'
agent = 'Mozilla / 5.0 (X11; U; Linux i686; en - US) AppleWebKit / 534.7 (KHTML, like Gecko) Chrome / 7.0.517.44 Safari / 534.7'

def get_data(text, sl='en', tl='zh-CN'):
    req = urllib2.Request(res % (urllib2.quote(text), sl, tl))
    req.add_header('user-agent', agent)
    content = urllib2.urlopen(req).read()
    return json.loads(to_standard_json(content))

def show(data):
    #step1
    print u'翻译：n  %s' % (data[4][0][0])
    #step2
    if types.ListType == type(data[1]):
        print u'n字典：'
        for word in data[1]:
            print word[0]
            if len(word) > 1:
                for i, w in enumerate(word[1]):
                    print '  %s.%s' % (i + 1, w) 

def to_standard_json(json):
    p = re.compile(r',([,]])')
    while(p.search(json)):
        json = p.sub(lambda m:',null%s' % (m.group(1)), json)
    return json

def contains_cn(text):
    for c in text:
        if ord(c) > 127:
            return True
    return False

if __name__ == '__main__':
    if not len(sys.argv) == 2 or not sys.argv[1].strip():
        print 'Useage:translate.py word'
        sys.exit()
    word = sys.argv[1].strip()
    if contains_cn(word):
        show(get_data(word, 'zh-CN', 'en'))
    else:
		show(get_data(word, 'en', 'zh-CN'))
