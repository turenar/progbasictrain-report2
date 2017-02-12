FN_NAME=Plus
check 2 "${FN_NAME}[1,1]"
check 45 "${FN_NAME}[16,29]"
check 0 "${FN_NAME}[0,0]"
check -100 "${FN_NAME}[-49.1,-50.9]"
xfail _call "${FN_NAME}[]"
xfail _call "${FN_NAME}[1]"
xfail _call "${FN_NAME}[1,2,3]"
