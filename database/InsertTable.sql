/* SpeechIndex */
insert into SpeechIndex(speech, ProcessName, TableName) values('zhongduan', 'gnome-terminal', 'gnome_terminal');
insert into SpeechIndex(speech, ProcessName, TableName) values('zongduan', 'gnome-terminal', 'gnome_terminal');

insert into SpeechIndex(speech, ProcessName, TableName) values('liulanqi', 'firefox', 'firefox');
insert into SpeechIndex(speech, ProcessName, TableName) values('liunanqi', 'firefox', 'firefox');

insert into SpeechIndex(speech, ProcessName, TableName) values('huohu', 'firefox', 'firefox');
insert into SpeechIndex(speech, ProcessName, TableName) values('bofangyinyue', 'deepin-music-player', 'deepin_music_player');
insert into SpeechIndex(speech, ProcessName, TableName) values('tingyinyue', 'deepin-music-player', 'deepin_music_player');
insert into SpeechIndex(speech, ProcessName, TableName) values('tingge', 'deepin-music-player', 'deepin_music_player');

insert into SpeechIndex(speech, ProcessName, TableName) values('wenjianguanliqi', 'nautilus', 'nautilus');

insert into SpeechIndex(speech, ProcessName, TableName) values('yanshiwengao', 'wpp', 'wpp');
insert into SpeechIndex(speech, ProcessName, TableName) values('yansiwengao', 'wpp', 'wpp');

/* OpenCommand */
insert into OpenCommand(speech, cmd, type) values('firefox', 'firefox > /dev/null &', 'cmd');
insert into OpenCommand(speech, cmd, type) values('gnome-terminal', 'gnome-terminal', 'cmd');
insert into OpenCommand(speech, cmd, type) values('deepin-music-player', 'deepin-music-player > /dev/null  &', 'cmd');
insert into OpenCommand(speech, cmd, type) values('nautilus', 'nautilus', 'cmd');
insert into OpenCommand(speech, cmd, type) values('deepin-screenshot', 'deepin-screenshot > /dev/null &', 'cmd');
insert into OpenCommand(speech, cmd, type) values('wpp', 'wpp > /dev/null &', 'cmd');

/* deepin_music_player */
insert into deepin_music_player(speech, cmd, type) values('bofang', 'PlayPause', 'func');
insert into deepin_music_player(speech, cmd, type) values('zanting', 'PlayPause', 'func');
insert into deepin_music_player(speech, cmd, type) values('zhanting', 'PlayPause', 'func');

insert into deepin_music_player(speech, cmd, type) values('shangyiqu', 'Prev', 'func');
insert into deepin_music_player(speech, cmd, type) values('sangyiqu', 'Prev', 'func');
insert into deepin_music_player(speech, cmd, type) values('sangyishou', 'Prev', 'func');
insert into deepin_music_player(speech, cmd, type) values('shangyishou', 'Prev', 'func');
insert into deepin_music_player(speech, cmd, type) values('sangyisou', 'Prev', 'func');
insert into deepin_music_player(speech, cmd, type) values('shangyisou', 'Prev', 'func');

insert into deepin_music_player(speech, cmd, type) values('xiayiqu', 'Next', 'func');
insert into deepin_music_player(speech, cmd, type) values('xiayishou', 'Next', 'func');
insert into deepin_music_player(speech, cmd, type) values('xiayisou', 'Next', 'func');

insert into deepin_music_player(speech, cmd, type) values('kuaijin', 'Forward', 'func');
insert into deepin_music_player(speech, cmd, type) values('chongfang', 'Rewind', 'func');
insert into deepin_music_player(speech, cmd, type) values('congfang', 'Rewind', 'func');
insert into deepin_music_player(speech, cmd, type) values('chongfa', 'Rewind', 'func');
insert into deepin_music_player(speech, cmd, type) values('congfa', 'Rewind', 'func');
insert into deepin_music_player(speech, cmd, type) values('tingzhi', 'Stop', 'func');
insert into deepin_music_player(speech, cmd, type) values('tingzi', 'Stop', 'func');
insert into deepin_music_player(speech, cmd, type) values('tuichu', 'Quit', 'func');
insert into deepin_music_player(speech, cmd, type) values('tuicu', 'Quit', 'func');

/* firefox */
insert into firefox(speech, cmd, type) values('xinjianbiaoqian', '0xffe3 0x0054', 'keys');
insert into firefox(speech, cmd, type) values('xinjianchuangkou', '0xffe3 0x004e', 'keys');
insert into firefox(speech, cmd, type) values('xinjiancuangkou', '0xffe3 0x004e', 'keys');
insert into firefox(speech, cmd, type) values('quanpin', '0xffc8', 'keys');
insert into firefox(speech, cmd, type) values('quanping', '0xffc8', 'keys');
insert into firefox(speech, cmd, type) values('tuichuquanping', '0xffc8', 'keys');
insert into firefox(speech, cmd, type) values('tuichuquanpin', '0xffc8', 'keys');
insert into firefox(speech, cmd, type) values('tuicuquanping', '0xffc8', 'keys');
insert into firefox(speech, cmd, type) values('tuicuquanpin', '0xffc8', 'keys');
insert into firefox(speech, cmd, type) values('zuidahua', '0xffe9 0xffc2', 'keys');
insert into firefox(speech, cmd, type) values('zhuidahua', '0xffe9 0xffc2', 'keys');
insert into firefox(speech, cmd, type) values('huifu', '0xffe9 0xffc3', 'keys');
insert into firefox(speech, cmd, type) values('guanbi', '0xffe9 0xffc1', 'keys');
insert into firefox(speech, cmd, type) values('huidaokaitou', '0xff50', 'keys');
insert into firefox(speech, cmd, type) values('tiaodaojiewei', '0xff57', 'keys');
insert into firefox(speech, cmd, type) values('sangyiye', '0xff55', 'keys');
insert into firefox(speech, cmd, type) values('shangyiye', '0xff55', 'keys');
insert into firefox(speech, cmd, type) values('xiayiye', '0xff56', 'keys');
insert into firefox(speech, cmd, type) values('baidu', 'firefox -new-tab www.baidu.com > /dev/null &', 'cmd');
insert into firefox(speech, cmd, type) values('guge', 'firefox -new-tab www.google.com.hk > /dev/null &', 'cmd');

/*
insert into (speech, cmd, type) values();

insert into (speech, cmd, type) values();

insert into (speech, cmd, type) values();
*/
