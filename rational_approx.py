def farey(x, N, dev):
    if x*(1+dev)>1>x/(1+dev):
        return 1, 1
    switch = 0
    if x > 1.0:
        x = 1/x
        switch = 1 
    a, b = 0, 1
    c, d = 1, 1
    while (b <= N and d <= N):
        mediant = float(a+c)/(b+d)
        if mediant*(1+dev)>x>mediant/(1+dev):
            if b + d <= N:
                if switch == 1:
                    return b+d, a+c
                else: 
                    return a+c, b+d
            elif d > b:
                if switch == 1:
                    return d, c
                else:
                    return c, d
            else:
                if switch == 1:
                    return b, a
                else:
                    return a, b
        elif x > mediant:
            a, b = a+c, b+d
        else:
            c, d = a+c, b+d
 
    if (b > N):
        if switch == 1:
            return d, c
        else:
            return c, d
    else:
        if switch == 1:
            return b, a
        else:
            return a, b