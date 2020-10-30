/*
https://rextester.com/l/r_online_compiler

f <- function(x1,x2,a,b1,b2) {a * (b1^x1) * (b2^x2) }

# generate some data
x1 <- c(70,70,50,50,50,50,50,49,54,74,60)
x2 <- c(10,7,7,10,13,17,24,7,26,26,20)
y <- c(50,40,22,30,38,53,100,22,151,225,96)
dat <- data.frame(x1,x2, y)

# fit a nonlinear model
fm <- nls(y ~ f(x1,x2,a,b1,b2), data = dat, start = c(a=3, b1=1.02,b2=1.09))

# get estimates of a, b
co <- coef(fm)

print(co)

OUTPUT:
       a       b1       b2 
3.737140 1.023652 1.095726 

power = pow(1.023652, cadence) * pow(1.095726, resisstance) * 3.737140

fmt.Println(math.Pow(1.023652,50.0) * math.Pow(1.095726,24.0) * 3.737140)

Values
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
 */

// Calculate the power
int getPower(int cadence, int resistance) {
  if(cadence == 0 || resistance == 0) return 0;
  return int(pow(1.028132, cadence) * pow(1.091815, resistance) * 3.024572);
}
