xfail _call x
check 1 --define 'x=1' x
check 1 x -Dx=1
xfail _call --define 'y=1' x
xfail _call x --define
xfail _call x --define hoge
xfail _call x --define x
xfail _call x --define x=
xfail _call x --define ==0
xfail _call x --define ,=0
