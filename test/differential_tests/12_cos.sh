check 0 'Cos[1]'
check 'Times[-1, Sin[x]]' 'Cos[x]'
check 'Times[-2, Sin[Plus[x, x]]]' 'Cos[Plus[x,x]]'
check 'Times[-2, Sin[Times[2, x]]]' 'Cos[Times[2,x]]'
