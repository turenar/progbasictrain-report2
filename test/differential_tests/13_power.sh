check 0 'Power[0,1]'
check 0 'Power[1,0]'
check 1 'Power[x,1]'
check 0 'Power[x,0]'
check 'Times[2, x]' 'Power[x, 2]'
check 'Times[3, Power[x, 2]]' 'Power[x, 3]'
check 'Times[0.5, Power[x, -0.5]]' 'Power[x, 0.5]'
check 0 'Power[1,x]'
check 'Times[Log[2], Power[2, x]]' 'Power[2,x]'
check 'Plus[Times[x, Power[x, Subtract[x, 1]]], Times[Log[x], Power[x, x]]]' 'Power[x, x]'
check 0 'Power[x,0]'
check 'Times[12, Power[Times[4, x], 2]]' 'Power[Times[4,x], 3]'
check 'Times[Times[3, Cos[x]], Power[Sin[x], 2]]' 'Power[Sin[x], 3]'