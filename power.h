/*
https://rextester.com/l/r_online_compiler

f <- function(x1,x2,a,b1,b2) {a * (b1^x1) * (b2^x2) }

# generate some data
x1 <- c(11,15,18,21,23,11,15,18,22,24)
x2 <- c(80,80,80,80,80,100,100,100,100,100)
y <- c(60,84,113,145,187,90,122,162,216,262)
dat <- data.frame(x1,x2, y)

# fit a nonlinear model
fm <- nls(y ~ f(x1,x2,a,b1,b2), data = dat, start = c(a=3, b1=1.02,b2=1.09))

# get estimates of a, b
co <- coef(fm)

print(co)

OUTPUT:
       a       b1       b2 
7.228959 1.090112 1.015343 

power = pow(1.090112, resisstance) * pow(1.015343, cadence) * 7.228958

fmt.Println(math.Pow(1.090112, 18) * math.Pow(1.015343, 30.0) * 7.228958)

Resistance
https://www.desmos.com/calculator/j5fmz904ps

Echelon Values
70,10,50
70,7,40
50,7,22
50,10,30
50,13,38
50,17,53
50,24,100
49,7,22
54,26,151
74,26,225
60,20,96
       a       b1       b2 
3.737140 1.023652 1.095726 

Peloton Values
80,30,60
80,35,84
80,40,113
80,45,145
80,50,187
100,30,90
100,35,122
100,40,162
100,45,216
100,50,262
       a       b1       b2 
3.165689 1.018363 1.053851 

Equation
1.023652^C * 1.095726^R * 3.737140 = 3.165689 * 1.018363^C * 1.053851^T
Peleton R = -19.0654(-0.00518019*C - 0.0914172*R - 0.16595)

*/

// Calculate the power
int getPower(int cadence, int resistance) {
  if(cadence == 0 || resistance == 0) return 0;
  return int(pow(1.090112, resistance) * pow(1.015343, cadence) * 7.228958);
}

int getPeletonResistance(int resistance) {
   int pr = int((0.0116058 * pow(resistance, 3)) + (-0.568462 * pow(resistance, 2)) + (10.4126 * resistance) - 31.4807);
   return pr < 0 ? 0 : pr;
}
