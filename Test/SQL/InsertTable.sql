insert into SpeechIndex(speech, ProcessName, TableName) values('终端', 'gnome-terminal', 'gnome_terminal');
insert into SpeechIndex(speech, ProcessName, TableName) values('浏览器', 'firefox', 'firefox');
insert into SpeechIndex(speech, ProcessName, TableName) values('火狐', 'firefox', 'firefox');
insert into SpeechIndex(speech, ProcessName, TableName) values('深度音乐', 'deepin-music-player', 'deepin_music_player');
insert into SpeechIndex(speech, ProcessName, TableName) values('听音乐', 'deepin-music-player', 'deepin_music_player');
insert into SpeechIndex(speech, ProcessName, TableName) values('听歌', 'deepin-music-player', 'deepin_music_player');
insert into SpeechIndex(speech, ProcessName, TableName) values('文件管理器', 'nautilus', 'nautilus');
insert into SpeechIndex(speech, ProcessName, TableName) values('演示文稿', 'wpp', 'wpp');

insert into deepin_music_player(speech, cmd, type) values('播放', 'PlayPause', 'func');
insert into deepin_music_player(speech, cmd, type) values('暂停', 'PlayPause', 'func');
insert into deepin_music_player(speech, cmd, type) values('上一首', 'Prev', 'func');
insert into deepin_music_player(speech, cmd, type) values('下一首', 'Next', 'func');
insert into deepin_music_player(speech, cmd, type) values('快进', 'Forward', 'func');
insert into deepin_music_player(speech, cmd, type) values('重放', 'Rewind', 'func');
insert into deepin_music_player(speech, cmd, type) values('停止', 'Stop', 'func');
insert into deepin_music_player(speech, cmd, type) values('退出', 'Quit', 'func');

insert into firefox(speech, cmd, type) values('新建标签', '0xffe3 0x0054', 'keys');
insert into firefox(speech, cmd, type) values('新建窗口', '0xffe3 0x004e', 'keys');
insert into firefox(speech, cmd, type) values('全屏', '0xffc8', 'keys');
insert into firefox(speech, cmd, type) values('退出全屏', '0xffc8', 'keys');
insert into firefox(speech, cmd, type) values('最大化', '0xffe9 0xffc2', 'keys');
insert into firefox(speech, cmd, type) values('恢复', '0xffe9 0xffc3', 'keys');
insert into firefox(speech, cmd, type) values('关闭', '0xffe9 0xffc1', 'keys');
insert into firefox(speech, cmd, type) values('回到开头', '0xff50', 'keys');
insert into firefox(speech, cmd, type) values('跳到结尾', '0xff57', 'keys');
insert into firefox(speech, cmd, type) values('上一页', '0xff55', 'keys');
insert into firefox(speech, cmd, type) values('下一页', '0xff56', 'keys');
insert into firefox(speech, cmd, type) values('百度', 'firefox -new-tab www.baidu.com > /dev/null &', 'cmd');

/*
insert into (speech, cmd, type) values();

insert into (speech, cmd, type) values();

insert into (speech, cmd, type) values();
*/
