check 0 'Cos[1]'
check 'Cos[x]' 'Sin[x]'
check 'Times[2, Cos[Plus[x, x]]]' 'Sin[Plus[x,x]]'
check 'Times[2, Cos[Times[2, x]]]' 'Sin[Times[2,x]]'
