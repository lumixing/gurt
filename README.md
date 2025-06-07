## gurt, a simple spotify client
<img src="gurt.png" align="right" style="float:right; width: 256px;"></img>
written in c using:
- [ncurses](https://github.com/mirror/ncurses) for tui
- [curl](https://github.com/curl/curl) for http client
- [mongoose](https://github.com/cesanta/mongoose) for http server
- [json-c](https://github.com/json-c/json-c) for json parsing
- [librespot](https://github.com/librespot-org/librespot) for spotify player
- [fifo](https://en.wikipedia.org/wiki/Named_pipe) for realtime communication between librespot and gurt
- [pthread](https://en.wikipedia.org/wiki/Segmentation_fault) for multithreading
- [str&arena&log](https://medium.com/@wieland.paul/why-i-think-rust-is-awesome-1a5b040241e4) custom built for basic dynamic strings, memory arenas and file logging
- [sweatntears.h](https://en.wiktionary.org/wiki/skill_issue) for handling everything

### disclaimer
this is still a work in proress and my first ~~big~~ not small c project, so use at your own risk, for now.  
also this is only tested on linux and probably only works on linux (using lots of nonportable functions such as `pthread_tryjoin_np`). no plans for other targets! :)

<!-- ### usage
first you need to install [librespot](https://github.com/librespot-org/librespot) (`yay -S librespot` for example on arch (btw))  
then youre on your own for now im still making this! -->
