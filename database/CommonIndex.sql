create table CommonIndex( speech text primary key not null, cmd text, type text);
/*全屏*/
insert into CommonIndex(speech, cmd, type) values('quanpin', '0xffc8', 'keys');
insert into CommonIndex(speech, cmd, type) values('quanping', '0xffc8', 'keys');
/*退出全屏*/
insert into CommonIndex(speech, cmd, type) values('tuichuquanping', '0xffc8', 'keys');
insert into CommonIndex(speech, cmd, type) values('tuichuquanpin', '0xffc8', 'keys');
insert into CommonIndex(speech, cmd, type) values('tuicuquanping', '0xffc8', 'keys');
insert into CommonIndex(speech, cmd, type) values('tuicuquanpin', '0xffc8', 'keys');
/*最大化*/
insert into CommonIndex(speech, cmd, type) values('zuidahua', '0xffe9 0xffc2', 'keys');
insert into CommonIndex(speech, cmd, type) values('zhuidahua', '0xffe9 0xffc2', 'keys');
/*恢复*/
insert into CommonIndex(speech, cmd, type) values('huifu', '0xffe9 0xffc3', 'keys');
/*关闭*/
insert into CommonIndex(speech, cmd, type) values('guanbi', '0xffe9 0xffc1', 'keys');
/*Home && End*/
insert into CommonIndex(speech, cmd, type) values('huidaokaitou', '0xff50', 'keys');
insert into CommonIndex(speech, cmd, type) values('tiaodaojiewei', '0xff57', 'keys');
insert into CommonIndex(speech, cmd, type) values('kaitou', '0xff50', 'keys');
insert into CommonIndex(speech, cmd, type) values('jiewei', '0xff57', 'keys');
/*换行*/
insert into CommonIndex(speech, cmd, type) values('huanhang', '0xff0d', 'keys');
insert into CommonIndex(speech, cmd, type) values('huiche', '0xff0d', 'keys');
/*上一页*/
insert into CommonIndex(speech, cmd, type) values('shangyiye', '0xff55', 'keys');
insert into CommonIndex(speech, cmd, type) values('sangyiye', '0xff55', 'keys');
/*下一页*/
insert into CommonIndex(speech, cmd, type) values('xiayiye', '0xff56', 'keys');
/*最小化*/
insert into CommonIndex(speech, cmd, type) values('zuixiaohua', '0xffe9 0xffc4', 'keys');
insert into CommonIndex(speech, cmd, type) values('zhuixiaohua', '0xffe9 0xffc4', 'keys');
/*保存*/
insert into CommonIndex(speech, cmd, type) values('baocun', '0xffe3 0x0053', 'keys');
insert into CommonIndex(speech, cmd, type) values('baochun', '0xffe3 0x0053', 'keys');
/*删除*/
insert into CommonIndex(speech, cmd, type) values('shanchu', '0xff08', 'keys');
insert into CommonIndex(speech, cmd, type) values('shancu', '0xff08', 'keys');
insert into CommonIndex(speech, cmd, type) values('sanchu', '0xff08', 'keys');
insert into CommonIndex(speech, cmd, type) values('sancu', '0xff08', 'keys');
