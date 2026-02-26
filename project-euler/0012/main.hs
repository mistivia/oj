divisors num = length $ filter divisable [1..num]
    where divisable x = num `mod` x == 0

pairDivisors (x,y) = (divisors x) * (divisors y)

triangleNums :: [(Integer,Integer)]
triangleNums = map toPair [1..]
    where
        toPair n = if n `mod` 2 == 0 then
                      (n `div` 2,n+1)
                   else
                      ((n+1) `div` 2, n)

firstWithDivisorNum n = impl triangleNums
    where impl ((a,b):xs) = if pairDivisors (a,b) >= n then (a * b) else impl xs

main = do
    putStrLn $ show $ firstWithDivisorNum 500
