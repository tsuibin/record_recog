/* SpeechIndex */
insert into SpeechIndex(speech, ProcessName, TableName) values('终端', 'gnome-terminal', 'gnome_terminal');
insert into SpeechIndex(speech, ProcessName, TableName) values('浏览器', 'firefox', 'firefox');
insert into SpeechIndex(speech, ProcessName, TableName) values('火狐', 'firefox', 'firefox');
insert into SpeechIndex(speech, ProcessName, TableName) values('播放音乐', 'deepin-music-player', 'deepin_music_player');
insert into SpeechIndex(speech, ProcessName, TableName) values('听音乐', 'deepin-music-player', 'deepin_music_player');
insert into SpeechIndex(speech, ProcessName, TableName) values('听歌', 'deepin-music-player', 'deepin_music_player');
insert into SpeechIndex(speech, ProcessName, TableName) values('文件管理器', 'nautilus', 'nautilus');
insert into SpeechIndex(speech, ProcessName, TableName) values('演示文稿', 'wpp', 'wpp');

/* OpenCommand */
insert into OpenCommand(speech, cmd, type) values('firefox', 'firefox > /dev/null &', 'cmd');
insert into OpenCommand(speech, cmd, type) values('gnome-terminal', 'gnome-terminal', 'cmd');
insert into OpenCommand(speech, cmd, type) values('deepin-music-player', 'deepin-music-player > /dev/null  &', 'cmd');
insert into OpenCommand(speech, cmd, type) values('nautilus', 'nautilus', 'cmd');
insert into OpenCommand(speech, cmd, type) values('deepin-screenshot', 'deepin-screenshot > /dev/null &', 'cmd');
insert into OpenCommand(speech, cmd, type) values('wpp', 'wpp > /dev/null &', 'cmd');

/* deepin_music_player */
insert into deepin_music_player(speech, cmd, type) values('播放', 'PlayPause', 'func');
insert into deepin_music_player(speech, cmd, type) values('暂停', 'PlayPause', 'func');
 insert into deepin_music_player(speech, cmd, type) values('上一曲', 'Next', 'func');
 insert into deepin_music_player(speech, cmd, type) values('下一曲', 'Next', 'func');
 insert into deepin_music_player(speech, cmd, type) values('上一艘', 'Next', 'func');
insert into deepin_music_player(speech, cmd, type) values('下一艘', 'Next', 'func');
insert into deepin_music_player(speech, cmd, type) values('上一首', 'Prev', 'func');
insert into deepin_music_player(speech, cmd, type) values('下一首', 'Next', 'func');
 insert into deepin_music_player(speech, cmd, type) values('下一宿', 'Next', 'func');
  insert into deepin_music_player(speech, cmd, type) values('上一宿', 'Next', 'func');
insert into deepin_music_player(speech, cmd, type) values('快进', 'Forward', 'func');
insert into deepin_music_player(speech, cmd, type) values('重放', 'Rewind', 'func');
insert into deepin_music_player(speech, cmd, type) values('停止', 'Stop', 'func');
insert into deepin_music_player(speech, cmd, type) values('退出', 'Quit', 'func');

/* firefox */
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
insert into firefox(speech, cmd, type) values('白度', 'firefox -new-tab www.baidu.com > /dev/null &', 'cmd');
insert into firefox(speech, cmd, type) values('谷歌', 'firefox -new-tab www.google.com.hk > /dev/null &', 'cmd');

/*
insert into (speech, cmd, type) values();

insert into (speech, cmd, type) values();

insert into (speech, cmd, type) values();
*/
