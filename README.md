## gurt, a simple spotify client

[![cool badge](https://img.shields.io/badge/written_in-c-blue)](https://en.wikipedia.org/wiki/C_(programming_language))
[![cool badge](https://img.shields.io/badge/gurt-yo-white)](https://knowyourmeme.com/memes/yogurt-gurt-yo)
[![cool badge](https://img.shields.io/badge/language-c-darkblue)](https://www.w3schools.com/c/c_intro.php)
[![cool badge](https://img.shields.io/badge/uses-c-green)](https://en.wikipedia.org/wiki/C)
[![cool badge](https://img.shields.io/badge/seg-fault-darkred)](https://www.google.com/search?q=how+to+cope+with+segfaults)
[![cool badge](https://img.shields.io/badge/no-cmake-red)](https://twdev.blog/2021/08/cmake/)
[![cool badge](https://img.shields.io/badge/or-make-orange)](https://medium.com/@jasonpatricksc/i-hate-everything-i-make-cde1eaa1616e)
[![cool badge](https://img.shields.io/badge/or_any-build_system-yellow)](https://www.youtube.com/watch?v=B1SPP9oXWYI)
[![cool badge](https://img.shields.io/badge/but_its_in-c-blue)](https://www.reddit.com/r/no/comments/1991a8r/whats_the_third_letter_of_the_alphabet/)
[![cool badge](https://img.shields.io/badge/might_leak-memory-magenta)](https://downloadmoreram.com/)
[![cool badge](https://img.shields.io/badge/c-btw-blue)](https://en.wikipedia.org/wiki/USB-C)
[![cool badge](https://img.shields.io/badge/vibe-coded-purple)](https://www.youtube.com/watch?v=rWktMN6jGV0)
[![cool badge](https://img.shields.io/badge/pop-quiz!-gold)](https://www.buzzfeed.com/quizzes)
[![cool badge](https://img.shields.io/badge/what-language%3F-blue)](https://i.redd.it/the-sybau-image-without-text-v0-8wb4hwj8mj1f1.png?width=1080&format=png&auto=webp&s=b60334fa6fc701ba642f2c05bf913d0f8c5f4e55)
[![cool badge](https://img.shields.io/badge/option_1-rust-red)](https://www.imdb.com/title/tt1901040/)
[![cool badge](https://img.shields.io/badge/option_2-javascript-red)](https://www.npmjs.com/package/wrong)
[![cool badge](https://img.shields.io/badge/option_3-c-green)](https://github.com/lh3/fermi/blob/master/correct.c)
[![cool badge](https://img.shields.io/badge/option_4-go-red)](https://i1.sndcdn.com/artworks-000036846542-n6cmhr-t1080x1080.jpg)
[![cool badge](https://img.shields.io/badge/click-your_answer!-lime)](https://www.moonsighting.com/dont/don_a.html)
[![cool badge](https://img.shields.io/badge/god_i_love-badges-black)](https://shields.io/)

<img src="gurt.png" align="right" style="float:right; width: 256px;"></img>
written in c using:
- [ncurses](https://github.com/mirror/ncurses) for tui
- [curl](https://github.com/curl/curl) for http client requests
- [mongoose](https://github.com/cesanta/mongoose) for http server (auth)
- [json-c](https://github.com/json-c/json-c) for json parsing (http bodies)
- [msgpack-c](https://github.com/msgpack/msgpack-c/tree/c_master) for caching
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
