/*
https://rextester.com/l/r_online_compiler

f <- function(x1,x2,a,b1,b2) {a * (b1^x1) * (b2^x2) }

# generate some data
x1 <- c(70,70,50,50,50,50,50)
x2 <- c(10,7,7,10,13,17,24)
y <- c(50,40,22,30,38,53,100)
dat <- data.frame(x1,x2, y)

# fit a nonlinear model
fm <- nls(y ~ f(x1,x2,a,b1,b2), data = dat, start = c(a=1, b1=1,b2=1))

# get estimates of a, b
co <- coef(fm)

print(co)

       a       b1       b2 
3.024572 1.028132 1.091815 

pow(1.028132, cadence) * pow(1.091815, resisstance) * 3.024572


Values
70,10,50
70,7,40
50,7,22
50,10,30
50,13,38
50,17,53
50,24,100

 */
