# 日月光明练习代码

#how to config auto pull and push without input username and password
```
[core]
    repositoryformatversion = 0
    filemode = true
    bare = false
    logallrefupdates = true
[remote "origin"]
    fetch = +refs/heads/*:refs/remotes/origin/*
    url = https://username:password@github.com/sunooms/learn.git
[branch "master"]
    mote = origin
    merge = refs/heads/master
```
